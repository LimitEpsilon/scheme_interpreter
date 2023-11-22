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
	return (root < 0 && root >= -isEQUAL);
}

int ListTable::apply(const int root)
{
	int ret = 0;
	gcstack->push(root); // prepare in case gc happens during evaluation
	int function = eval(car(root));
	if (function == -PLUS)
	{
		int op1 = eval(car(cdr(root)));		 // get first operand.
		int op2 = eval(car(cdr(cdr(root)))); // get second operand
		if (op1 >= 0 || op2 >= 0 || error(op1) || error(op2))
		{
			cout << "PLUS error" << endl;
			ret = -LEFT_PARENS;
			goto cleanup;
		}
		string arg1 = ht->find(op1);
		string arg2 = ht->find(op2);
		char num1 = isNumber(arg1), num2 = isNumber(arg2);
		if (!num1 || !num2)
		{
			cout << "PLUS error" << endl;
			ret = -LEFT_PARENS;
			goto cleanup;
		}

		if (num1 == 'f' || num2 == 'f')
			ret = ht->get_hash(to_string(stof(arg1) + stof(arg2)));
		else
			ret = ht->get_hash(to_string(stoi(arg1) + stoi(arg2)));
	}
	else if (function == -MINUS)
	{
		int op1 = eval(car(cdr(root)));
		int op2 = eval(car(cdr(cdr(root))));
		if (op1 >= 0 || op2 >= 0 || error(op1) || error(op2))
		{
			cout << "MINUS error" << endl;
			ret = -LEFT_PARENS;
			goto cleanup;
		}
		string arg1 = ht->find(op1);
		string arg2 = ht->find(op2);
		char num1 = isNumber(arg1), num2 = isNumber(arg2);
		if (!num1 || !num2)
		{
			cout << "MINUS error" << endl;
			ret = -LEFT_PARENS;
			goto cleanup;
		}

		if (num1 == 'f' || num2 == 'f')
			ret = ht->get_hash(to_string(stof(arg1) - stof(arg2)));
		else
			ret = ht->get_hash(to_string(stoi(arg1) - stoi(arg2)));
	}
	else if (function == -TIMES)
	{
		int op1 = eval(car(cdr(root)));
		int op2 = eval(car(cdr(cdr(root))));
		if (op1 >= 0 || op2 >= 0 || error(op1) || error(op2))
		{
			cout << "TIMES error" << endl;
			ret = -LEFT_PARENS;
			goto cleanup;
		}
		string arg1 = ht->find(op1);
		string arg2 = ht->find(op2);
		char num1 = isNumber(arg1), num2 = isNumber(arg2);
		if (!num1 || !num2)
		{
			cout << "TIMES error" << endl;
			ret = -LEFT_PARENS;
			goto cleanup;
		}

		if (num1 == 'f' || num2 == 'f')
			ret = ht->get_hash(to_string(stof(arg1) * stof(arg2)));
		else
			ret = ht->get_hash(to_string(stoi(arg1) * stoi(arg2)));
	}
	else if (function == -isEQ)
	{
		int op1 = eval(car(cdr(root)));
		int op2 = eval(car(cdr(cdr(root))));
		if (error(op1) || error(op2))
		{
			cout << "EQ error" << endl;
			ret = -LEFT_PARENS;
			goto cleanup;
		}
		if (op1 == op2)
		{
			ret = -TRUE;
		}
		else if (op1 < 0 && op2 < 0)
		{
			string arg1 = ht->find(op1);
			string arg2 = ht->find(op2);
			if (isNumber(arg1) && isNumber(arg2) && (stof(arg1) == stof(arg2)))
			{
				ret = -TRUE;
			}
			else
			{
				ret = -FALSE;
			}
		}
		else
		{
			ret = -FALSE;
		}
	}
	else if (function == -isEQUAL)
	{
		int op1 = eval(car(cdr(root)));
		int op2 = eval(car(cdr(cdr(root))));
		if (error(op1) || error(op2))
		{
			cout << "EQUAL error" << endl;
			ret = -LEFT_PARENS;
			goto cleanup;
		}
		if (check_structure(op1, op2))
		{
			ret = -TRUE;
		}
		else
		{
			ret = -FALSE;
		}
	}
	else if (function == -isNUMBER)
	{
		int op = eval(car(cdr(root)));
		if (error(op))
		{
			cout << "NUMBER? error" << endl;
			ret = -LEFT_PARENS;
			goto cleanup;
		}
		if (op >= 0)
		{
			ret = -FALSE;
		}
		else if (isNumber(ht->find(op)))
		{
			ret = -TRUE;
		}
		else
		{
			ret = -FALSE;
		}
	}
	else if (function == -isSYMBOL)
	{
		int op = eval(car(cdr(root)));
		if (error(op))
		{
			cout << "SYMBOL? error" << endl;
			ret = -LEFT_PARENS;
			goto cleanup;
		}
		if (op >= 0)
		{
			ret = -FALSE;
		}
		else if (isNumber(ht->find(op)) || check_predefined(op))
		{
			ret = -FALSE;
		}
		else
		{
			ret = -TRUE;
		}
	}
	else if (function == -isNULL)
	{
		int op = eval(car(cdr(root)));
		if (error(op))
		{
			cout << "NULL? error" << endl;
			ret = -LEFT_PARENS;
			goto cleanup;
		}
		if (op == 0)
		{
			ret = -TRUE;
		}
		else
		{
			ret = -FALSE;
		}
	}
	else if (function == -CONS)
	{
		int op1 = eval(car(cdr(root)));
		int op2 = eval(car(cdr(cdr(root))));
		if (error(op1) || error(op2))
		{
			cout << "CONS error" << endl;
			ret = -LEFT_PARENS;
			goto cleanup;
		}
		int newmemory = allocate();
		if (error(newmemory))
		{
			cout << "CONS error" << endl;
			ret = -LEFT_PARENS;
			goto cleanup;
		}
		car(newmemory) = op1;
		cdr(newmemory) = op2;
		ret = newmemory;
	}
	else if (function == -COND)
	{ // root is root of (cond (P1 E1) (P2 E2) ... (Pn En) (else E))
		int cursor = root;
		while (cdr(cdr(cursor)) != 0)
		{ // cursor is the root of ((Pm Em) (Pm+1 Em+1) ... (Pn En) (else E)). (Pm Em) already evaluated.
			cursor = cdr(cursor);
			int condition = eval(car(car(cursor)));
			if (error(condition))
			{
				cout << "COND error at condition" << endl;
				ret = -LEFT_PARENS;
				goto cleanup;
			}
			if (condition == -TRUE)
			{
				int result = eval(car(cdr(car(cursor))));
				if (error(result))
				{
					cout << "COND error at result" << endl;
					ret = -LEFT_PARENS;
					goto cleanup;
				}
				ret = result;
				goto cleanup;
			}
		}
		if (car(car(cdr(cursor))) != -ELSE)
		{
			cout << "COND error, no ELSE" << endl;
			ret = -LEFT_PARENS;
			goto cleanup;
		}
		int result = eval(car(cdr(car(cdr(cursor)))));
		if (error(result))
		{
			cout << "COND error at ELSE result" << endl;
			ret = -LEFT_PARENS;
			goto cleanup;
		}
		ret = result;
	}
	else if (function == -CAR)
	{
		int op = eval(car(cdr(root)));
		if (error(op))
		{
			cout << "CAR error" << endl;
			ret = -LEFT_PARENS;
			goto cleanup;
		}
		if (op > 0)
		{
			ret = car(op);
		}
		else
		{
			cout << "Cannot CAR a symbol or an empty list" << endl;
			ret = -LEFT_PARENS;
			goto cleanup;
		}
	}
	else if (function == -CDR)
	{
		int op = eval(car(cdr(root)));
		if (error(op))
		{
			cout << "CDR error" << endl;
			ret = -LEFT_PARENS;
			goto cleanup;
		}
		if (op > 0)
		{
			ret = cdr(op);
		}
		else
		{
			cout << "Cannot CDR a symbol or an empty list" << endl;
			ret = -LEFT_PARENS;
			goto cleanup;
		}
	}
	else if (function == -DEFINE)
	{
		if (isNumber(ht->find(car(cdr(root)))))
		{
			cout << "Don't redefine numbers" << endl;
			ret = -LEFT_PARENS;
			goto cleanup;
		}
		else if (check_predefined(car(cdr(root))))
		{
			cout << "Don't redefine pre-defined symbols" << endl;
			ret = -LEFT_PARENS;
			goto cleanup;
		}
		int definition = eval(car(cdr(cdr(root))));
		if (error(definition))
		{
			cout << "DEFINE error" << endl;
			ret = -LEFT_PARENS;
			goto cleanup;
		}
		ht->value(-car(cdr(root))) = definition;
		ht->defined(-car(cdr(root))) = true;
		ret = car(cdr(root));
	}
	else if (function == -QUOTE)
	{
		ret = car(cdr(root));
	}
	else if (function == -LAMBDA)
	{
		ret = root;
	}
	else if (car(function) == -LAMBDA) // root is the root to (user-defined-function real-parameter-list)
	{
		function = cdr(function);	   // function is the root to ((function-parameters) (function-body))
		int paramlist = car(function); // paramlist is the root to (function-parameters)
		int body = car(cdr(function)); // body is the root to (function-body)
		int cursor = root;
		int cursor_pre = cdr(root); // used to pre-evaluate the real parameters
		LinkedStack preeval = LinkedStack();
		while (cursor_pre != 0 && paramlist != 0)
		{ // pre-evaluate real parameters
			int temp = eval(car(cursor_pre));
			if (error(temp))
			{
				cout << "FUNCTION PARAMETER error" << endl;
				ret = -LEFT_PARENS;
				goto cleanup;
			}
			preeval.push(car(paramlist), temp);
			cursor_pre = cdr(cursor_pre);
			paramlist = cdr(paramlist);
		}
		if (cursor_pre != 0)
		{
			cout << "Too many parameters given" << endl;
			ret = -LEFT_PARENS;
			goto cleanup;
		}
		else if (paramlist != 0)
		{
			cout << "Too little parameters given" << endl;
			ret = -LEFT_PARENS;
			goto cleanup;
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
			ret = -LEFT_PARENS;
			goto cleanup;
		}
		// restore bindings: currying is not supported
		Node<int, int, bool> temp;
		for (int i = 0; i < numofparam; ++i)
		{
			temp = stack->pop();
			ht->value(-temp.lchild) = temp.rchild;
			ht->defined(-temp.lchild) = temp.mchild;
		}
		ret = result;
	}
	else
	{
		print(root, true);
		print(car(root), true);
		cout << "Function part of apply is not a function" << endl;
		ret = -LEFT_PARENS;
	}
cleanup:
	gcstack->pop();
	return ret;
}

int ListTable::eval(const int root)
{
	if (error(root) || root == 0) // null list or error
		return root;
	if (root < 0) // symbol
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
	return apply(root); // application
}