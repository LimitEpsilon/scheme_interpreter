#include <iostream>
#include "tables.h"
using namespace std;

char isNumber(const string &s)
{
	bool decimal = false;
	for (auto i = s.begin(); i != s.end(); ++i)
	{
		if (i == s.begin() && (*i == '-' || *i == '+'))
			continue;
		if (*i >= '0' && *i <= '9')
			continue;
		else if (!decimal && *i == '.')
		{
			decimal = true;
			continue;
		}
		return false;
	}
	if (!decimal)
		return 'i'; // int
	else
		return 'f'; // float
}

bool ListTable::check_structure(const int &r1, const int &r2)
{
	if (r1 == r2)
		return true; // if both are NIL, return true. Same for hashes
	else if (r1 == 0)
		return false;
	else if (r2 == 0)
		return false;
	else if (r2 * r1 < 0)
		return false;
	else if (r1 > 0 && r2 > 0)
		return (check_structure(car(r1), car(r2)) && check_structure(cdr(r1), cdr(r2)));
	else
	{
		if (isNumber(ht->find(r1)) && isNumber(ht->find(r2)) && stof(ht->find(r1)) == stof(ht->find(r2)))
			return true;
		else
			return false;
	}
}

bool check_predefined(int root)
{
	return (root == -PLUS || root == -MINUS || root == -TIMES || root == -isEQ || root == -isEQUAL || root == -isNUMBER || root == -isNULL || root == -isSYMBOL || root == -CONS || root == -COND || root == -CAR || root == -CDR || root == -DEFINE || root == -QUOTE || root == -TRUE || root == -FALSE || root == -ELSE);
}

int ListTable::eval(int &root)
{
	if (error(root) || root == 0)
		return root;
	if (root < 0)
	{
		if (isNumber(ht->find(root)) || check_predefined(root))
			return root;
		if (!ht->defined(-root))
		{
			cout << "Unbound symbol " << ht->hashtable[-root].lchild << endl;
			return -LEFT_PARENS;
		}
		return ht->value(-root);
	}
	gcstack->push(root); // prepare in case gc happens during evaluation
	int &tokenindex = car(root);
	// root is the list (operator operands), tokenindex is the operator
	// car(cdr(root)) is the first operand, car(cdr(cdr(root)) is the second operand
	if (tokenindex == -PLUS)
	{
		int op1 = eval(car(cdr(root)));		 // get first operand.
		int op2 = eval(car(cdr(cdr(root)))); // get second operand
		if (op1 >= 0 || op2 >= 0 || error(op1) || error(op2))
		{
			cout << "PLUS error" << endl;
			gcstack->pop();
			return -LEFT_PARENS;
		}
		string arg1 = ht->find(op1);
		string arg2 = ht->find(op2);
		char num1 = isNumber(arg1), num2 = isNumber(arg2);
		if (!num1 || !num2)
		{
			cout << "PLUS error" << endl;
			gcstack->pop();
			return -LEFT_PARENS;
		}
		gcstack->pop();
		if (num1 == 'f' || num2 == 'f')
			return ht->get_hash(to_string(stof(arg1) + stof(arg2)));
		else
			return ht->get_hash(to_string(stoi(arg1) + stoi(arg2)));
	}
	else if (tokenindex == -MINUS)
	{
		int op1 = eval(car(cdr(root)));
		int op2 = eval(car(cdr(cdr(root))));
		if (op1 >= 0 || op2 >= 0 || error(op1) || error(op2))
		{
			cout << "MINUS error" << endl;
			gcstack->pop();
			return -LEFT_PARENS;
		}
		string arg1 = ht->find(op1);
		string arg2 = ht->find(op2);
		char num1 = isNumber(arg1), num2 = isNumber(arg2);
		if (!num1 || !num2)
		{
			cout << "MINUS error" << endl;
			gcstack->pop();
			return -LEFT_PARENS;
		}
		gcstack->pop();
		if (num1 == 'f' || num2 == 'f')
			return ht->get_hash(to_string(stof(arg1) - stof(arg2)));
		else
			return ht->get_hash(to_string(stoi(arg1) - stoi(arg2)));
	}
	else if (tokenindex == -TIMES)
	{
		int op1 = eval(car(cdr(root)));
		int op2 = eval(car(cdr(cdr(root))));
		if (op1 >= 0 || op2 >= 0 || error(op1) || error(op2))
		{
			cout << "TIMES error" << endl;
			gcstack->pop();
			return -LEFT_PARENS;
		}
		string arg1 = ht->find(op1);
		string arg2 = ht->find(op2);
		char num1 = isNumber(arg1), num2 = isNumber(arg2);
		if (!num1 || !num2)
		{
			cout << "TIMES error" << endl;
			gcstack->pop();
			return -LEFT_PARENS;
		}
		gcstack->pop();
		if (num1 == 'f' || num2 == 'f')
			return ht->get_hash(to_string(stof(arg1) * stof(arg2)));
		else
			return ht->get_hash(to_string(stoi(arg1) * stoi(arg2)));
	}
	else if (tokenindex == -isEQ)
	{
		int op1 = eval(car(cdr(root)));
		int op2 = eval(car(cdr(cdr(root))));
		if (error(op1) || error(op2))
		{
			cout << "EQ error" << endl;
			gcstack->pop();
			return -LEFT_PARENS;
		}
		if (op1 == op2)
		{
			gcstack->pop();
			return -TRUE;
		}
		else if (op1 < 0 && op2 < 0)
		{
			string arg1 = ht->find(op1);
			string arg2 = ht->find(op2);
			if (isNumber(arg1) && isNumber(arg2) && (stof(arg1) == stof(arg2)))
			{
				gcstack->pop();
				return -TRUE;
			}
			else
			{
				gcstack->pop();
				return -FALSE;
			}
		}
		else
		{
			gcstack->pop();
			return -FALSE;
		}
	}
	else if (tokenindex == -isEQUAL)
	{
		int op1 = eval(car(cdr(root)));
		int op2 = eval(car(cdr(cdr(root))));
		if (error(op1) || error(op2))
		{
			cout << "EQUAL error" << endl;
			gcstack->pop();
			return -LEFT_PARENS;
		}
		if (check_structure(op1, op2))
		{
			gcstack->pop();
			return -TRUE;
		}
		else
		{
			gcstack->pop();
			return -FALSE;
		}
	}
	else if (tokenindex == -isNUMBER)
	{
		int op = eval(car(cdr(root)));
		if (error(op))
		{
			cout << "NUMBER? error" << endl;
			gcstack->pop();
			return -LEFT_PARENS;
		}
		if (op >= 0)
		{
			gcstack->pop();
			return -FALSE;
		}
		else if (isNumber(ht->find(op)))
		{
			gcstack->pop();
			return -TRUE;
		}
		else
		{
			gcstack->pop();
			return -FALSE;
		}
	}
	else if (tokenindex == -isSYMBOL)
	{
		int op = eval(car(cdr(root)));
		if (error(op))
		{
			cout << "SYMBOL? error" << endl;
			gcstack->pop();
			return -LEFT_PARENS;
		}
		if (op >= 0)
		{
			gcstack->pop();
			return -FALSE;
		}
		else if (isNumber(ht->find(op)) || check_predefined(op))
		{
			gcstack->pop();
			return -FALSE;
		}
		else
		{
			gcstack->pop();
			return -TRUE;
		}
	}
	else if (tokenindex == -isNULL)
	{
		int op = eval(car(cdr(root)));
		if (error(op))
		{
			cout << "NULL? error" << endl;
			gcstack->pop();
			return -LEFT_PARENS;
		}
		if (op == 0)
		{
			gcstack->pop();
			return -TRUE;
		}
		else
		{
			gcstack->pop();
			return -FALSE;
		}
	}
	else if (tokenindex == -CONS)
	{
		int op1 = eval(car(cdr(root)));
		int op2 = eval(car(cdr(cdr(root))));
		if (error(op1) || error(op2))
		{
			cout << "CONS error" << endl;
			gcstack->pop();
			return -LEFT_PARENS;
		}
		int newmemory = allocate();
		if (error(newmemory))
		{
			cout << "CONS error" << endl;
			gcstack->pop();
			return -LEFT_PARENS;
		}
		car(newmemory) = op1;
		cdr(newmemory) = op2;
		gcstack->pop();
		return newmemory;
	}
	else if (tokenindex == -COND)
	{ // root is root of (cond (P1 E1) (P2 E2) ... (Pn En) (else E))
		int cursor = root;
		while (cdr(cdr(cursor)) != 0)
		{ // cursor is the root of ((Pm Em) (Pm+1 Em+1) ... (Pn En) (else E)). (Pm Em) already evaluated.
			cursor = cdr(cursor);
			int condition = eval(car(car(cursor)));
			if (error(condition))
			{
				cout << "COND error" << endl;
				gcstack->pop();
				return -LEFT_PARENS;
			}
			if (condition == -TRUE)
			{
				int result = eval(car(cdr(car(cursor))));
				if (error(result))
				{
					cout << "COND error" << endl;
					gcstack->pop();
					return -LEFT_PARENS;
				}
				gcstack->pop();
				return result;
			}
		}
		if (car(car(cdr(cursor))) != -ELSE)
		{
			cout << "COND error, no ELSE" << endl;
			gcstack->pop();
			return -LEFT_PARENS;
		}
		int result = eval(car(cdr(car(cdr(cursor)))));
		if (error(result))
		{
			cout << "COND error" << endl;
			gcstack->pop();
			return -LEFT_PARENS;
		}
		gcstack->pop();
		return result;
	}
	else if (tokenindex == -CAR)
	{
		int op = eval(car(cdr(root)));
		if (error(op))
		{
			cout << "CAR error" << endl;
			gcstack->pop();
			return -LEFT_PARENS;
		}
		if (op > 0)
		{
			gcstack->pop();
			return car(op);
		}
		else
		{
			cout << "Cannot CAR a symbol or an empty list" << endl;
			gcstack->pop();
			return -LEFT_PARENS;
		}
	}
	else if (tokenindex == -CDR)
	{
		int op = eval(car(cdr(root)));
		if (error(op))
		{
			cout << "CDR error" << endl;
			gcstack->pop();
			return -LEFT_PARENS;
		}
		if (op > 0)
		{
			gcstack->pop();
			return cdr(op);
		}
		else
		{
			cout << "Cannot CDR a symbol or an empty list" << endl;
			gcstack->pop();
			return -LEFT_PARENS;
		}
	}
	else if (tokenindex == -DEFINE)
	{
		if (isNumber(ht->find(car(cdr(root)))))
		{
			cout << "Don't redefine numbers" << endl;
			gcstack->pop();
			return -LEFT_PARENS;
		}
		else if (check_predefined(car(cdr(root))))
		{
			cout << "Don't redefine pre-defined symbols" << endl;
			gcstack->pop();
			return -LEFT_PARENS;
		}
		int &def = car(cdr(cdr(root))); // definition of object defined
		if (def > 0 && car(def) == -LAMBDA)
		{
			ht->value(-car(cdr(root))) = def;
		}
		else
		{
			int definition = eval(def);
			if (error(definition))
			{
				cout << "DEFINE error" << endl;
				gcstack->pop();
				return -LEFT_PARENS;
			}
			ht->value(-car(cdr(root))) = definition;
		}
		ht->defined(-car(cdr(root))) = true;
		gcstack->pop();
		return car(cdr(root));
	}
	else if (tokenindex == -QUOTE)
	{
		gcstack->pop();
		return car(cdr(root));
	}
	else if (tokenindex < 0)
	{ // root is the root to (function real-parameter-list)
		if (check_predefined(ht->value(-tokenindex)))
		{ // predefined function
			tokenindex = ht->value(-tokenindex);
			gcstack->pop();
			return eval(root);
		}
		else if (ht->value(-tokenindex) == 0)
		{
			cout << "Not a user defined function, not list" << endl;
			gcstack->pop();
			return -LEFT_PARENS;
		}
		else if (car(ht->value(-tokenindex)) != -LAMBDA)
		{
			cout << "Not a user defined function, not lambda expression" << endl;
			gcstack->pop();
			return -LEFT_PARENS;
		}
		int function = cdr(ht->value(-tokenindex)); // function is the root to ((function-parameters) (function-body))
		int paramlist = car(function);				// paramlist is the root to (function-parameters)
		int body = car(cdr(function));				// body is the root to (function-body)
		int cursor = root;
		int cursor_pre = cdr(root); // used to pre-evaluate the real parameters
		LinkedStack preeval = LinkedStack();
		while (cursor_pre != 0 && paramlist != 0)
		{ // pre-evaluate real parameters
			int temp = eval(car(cursor_pre));
			if (error(temp))
			{
				cout << "FUNCTION PARAMETER error" << endl;
				gcstack->pop();
				return -LEFT_PARENS;
			}
			preeval.push(car(paramlist), temp);
			cursor_pre = cdr(cursor_pre);
			paramlist = cdr(paramlist);
		}
		if (cursor_pre != 0)
		{
			cout << "Too many parameters given" << endl;
			gcstack->pop();
			return -LEFT_PARENS;
		}
		else if (paramlist != 0)
		{
			cout << "Too little parameters given" << endl;
			gcstack->pop();
			return -LEFT_PARENS;
		}
		int numofparam = 0;
		paramlist = car(function);
		ChainNode chain = ChainNode();
		while (paramlist != 0)
		{
			cursor = cdr(cursor); // in the first loop, cursor is the root to (real-parameter-list).
			int parameter = car(paramlist);
			stack->push(Node<int, int, bool>(parameter, ht->value(-parameter), ht->defined(-parameter)));
			paramlist = cdr(paramlist);
			++numofparam;
		}
		while (!preeval.is_empty())
		{
			chain = preeval.pop();
			ht->value(-chain.hash) = chain.value;
			ht->defined(-chain.hash) = true;
		}
		int result = eval(body);
		if (error(result))
		{
			cout << "FUNCTION EVALUATION error" << endl;
			gcstack->pop();
			return -LEFT_PARENS;
		}
		Node<int, int, bool> temp;
		for (int i = 0; i < numofparam; ++i)
		{
			temp = stack->pop();
			ht->value(-temp.lchild) = temp.rchild;
			ht->defined(-temp.lchild) = temp.mchild;
		}
		gcstack->pop();
		return result;
	}
	else if (car(tokenindex) == -LAMBDA)
	{
		int function = cdr(tokenindex); // function is the root to ((function-parameters) (function-body))
		int paramlist = car(function);	// paramlist is the root to (function-parameters)
		int body = car(cdr(function));	// body is the root to (function-body)
		int cursor = root;
		int cursor_pre = cdr(root); // used to pre-evaluate the real parameters
		LinkedStack preeval = LinkedStack();
		while (cursor_pre != 0 && paramlist != 0)
		{ // pre-evaluate real parameters
			int temp = eval(car(cursor_pre));
			if (error(temp))
			{
				cout << "LAMBDA PARAMETER error" << endl;
				gcstack->pop();
				return -LEFT_PARENS;
			}
			preeval.push(car(paramlist), temp);
			cursor_pre = cdr(cursor_pre);
			paramlist = cdr(paramlist);
		}
		if (cursor_pre != 0)
		{
			cout << "Too many parameters given" << endl;
			gcstack->pop();
			return -LEFT_PARENS;
		}
		else if (paramlist != 0)
		{
			cout << "Too little parameters given" << endl;
			gcstack->pop();
			return -LEFT_PARENS;
		}
		int numofparam = 0;
		paramlist = car(function);
		ChainNode chain = ChainNode();
		while (paramlist != 0)
		{
			cursor = cdr(cursor); // in the first loop, cursor is the root to (real-parameter-list).
			int parameter = car(paramlist);
			stack->push(Node<int, int, bool>(parameter, ht->value(-parameter), ht->defined(-parameter)));
			paramlist = cdr(paramlist);
			++numofparam;
		}
		while (!preeval.is_empty())
		{
			chain = preeval.pop();
			ht->value(-chain.hash) = chain.value;
			ht->defined(-chain.hash) = true;
		}
		int result = eval(body);
		if (error(result))
		{
			cout << "LAMBDA EVALUATION error" << endl;
			gcstack->pop();
			return -LEFT_PARENS;
		}
		Node<int, int, bool> temp;
		for (int i = 0; i < numofparam; ++i)
		{
			temp = stack->pop();
			ht->value(-temp.lchild) = temp.rchild;
			ht->defined(-temp.lchild) = temp.mchild;
		}
		gcstack->pop();
		return result;
	}
	else
	{
		cout << "Not a valid expression" << endl;
		gcstack->pop();
		return -LEFT_PARENS;
	}
	return 0;
}