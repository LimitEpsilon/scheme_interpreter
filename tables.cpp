#include <iostream>
#include <string>
#include "tables.h"
#include "tokenized.h"
#include "preprocessor.h"
using namespace std;
int HashTable::get_hash(const string &t)
{
	int key = -hash(t);
	for (int i = 0; i < size; ++i)
	{
		if (t == token(key))
			return -key;
		else if (token(key).empty())
		{
			token(key) = t;
			return -key;
		}
		key = (key % size) + 1;
	}
	cout << "Hash table is full" << endl;
	return -LEFT_PARENS;
}

void HashTable::initialize()
{
	get_hash("(");
	defined(LEFT_PARENS) = true;
	get_hash(")");
	defined(RIGHT_PARENS) = true;
	get_hash("+");
	defined(PLUS) = true;
	get_hash("-");
	defined(MINUS) = true;
	get_hash("*");
	defined(TIMES) = true;
	get_hash("eq?");
	defined(isEQ) = true;
	get_hash("equal?");
	defined(isEQUAL) = true;
	get_hash("number?");
	defined(isNUMBER) = true;
	get_hash("symbol?");
	defined(isSYMBOL) = true;
	get_hash("null?");
	defined(isNULL) = true;
	get_hash("cons");
	defined(CONS) = true;
	get_hash("cond");
	defined(COND) = true;
	get_hash("else");
	defined(ELSE) = true;
	get_hash("car");
	defined(CAR) = true;
	get_hash("cdr");
	defined(CDR) = true;
	get_hash("define");
	defined(DEFINE) = true;
	get_hash("quote");
	defined(QUOTE) = true;
	get_hash("lambda");
	defined(LAMBDA) = true;
	get_hash("#t");
	defined(TRUE) = true;
	get_hash("#f");
	defined(FALSE) = true;
}

void HashTable::print_entire()
{
	cout << "Hash table" << endl;
	cout << "(hash value, symbol, link)" << endl;
	for (int i = 1; i <= size; ++i)
	{
		if (token(i).empty())
			;
		else
			cout << "(" << -i << ", " << token(i) << ", " << value(i) << ")" << endl;
	}
}

void ListTable::initialize()
{
	freeindex = 1;
	for (int i = 1; i <= size; ++i)
	{
		list[i].rchild = i % size + 1;
	}
	list[size].rchild = NIL;
}

void ListTable::doubling()
{
	cout << "doubling listtable" << endl;
	int beforesize = size;
	size *= 2;
	Node<int, int, bool> *lt = list;
	list = new Node<int, int, bool>[size + 1];
	for (int i = 1; i <= beforesize; ++i)
	{
		list[i].lchild = lt[i].lchild;
		list[i].rchild = lt[i].rchild;
		list[i].mchild = lt[i].mchild;
	}
	for (int i = beforesize + 1; i <= size - 1; ++i)
	{
		list[i].rchild = i + 1;
	}
	list[size].rchild = NIL;
	freeindex = beforesize + 1;
	delete[] lt;
}

int ListTable::allocate()
{
	if (freeindex == NIL)
		gc();
	if (freeindex == NIL)
		doubling();
	int temp = freeindex;
	freeindex = list[freeindex].rchild;
	return temp;
}

void ListTable::deallocate(int i)
{
	list[i].rchild = freeindex;
	list[i].lchild = NIL;
	freeindex = i;
}

int ListTable::read(Preprocessor &input)
{
	int root = NIL;
	int cursor = NIL;
	bool first = true; // initialize to the null list
	string temp = input.read_token();
	int hash = ht->get_hash(temp);
	if (hash == -LEFT_PARENS)
	{ // when the expression is a list
		int newmemory = 0, r = 0;
		while ((hash = ht->get_hash(temp = input.read_token())) != -RIGHT_PARENS)
		{ // read until the end of the list
			// allocation of cdr
			if (first)
			{ // if the list is not a null list, allocate the root.
				newmemory = allocate();
				if (error(newmemory))
				{
					cout << "Cannot read input, from the beginning" << endl;
					return -LEFT_PARENS;
				}
				root = newmemory;
				gcstack->push(root);
				cursor = root;
				first = false;
			}
			else
			{ // if this is not the first of the list, i.e. there is an element at the cdr, allocate the rchild.
				newmemory = allocate();
				if (error(newmemory))
				{
					cout << "Cannot read input" << endl;
					gcstack->pop();
					return -LEFT_PARENS;
				}
				list[cursor].rchild = newmemory;
				cursor = list[cursor].rchild;
			}
			list[cursor].rchild = NIL; // assume that the cdr is (), if not, the while loop will allocate the cdr.
			// reading the car
			if (hash == -LEFT_PARENS)
			{ // if the first expression is a list, recursion.
				input.putback_token();
				r = read(input);
				if (error(r))
				{
					cout << "Cannot read input" << endl;
					gcstack->pop();
					return -LEFT_PARENS;
				}
				list[cursor].lchild = r;
			}
			else
			{
				list[cursor].lchild = hash;
			} // if the first expression is a symbol, store hash.
		}
		if (root != NIL)
			gcstack->pop();
		return root;
	}
	else
		return hash;
}

void ListTable::print(int root, bool first)
{
	if (root == 0)
	{
		cout << "() ";
	}
	else if (root < 0)
	{
		cout << ht->find(root) << " ";
	}
	else
	{
		if (first)
			cout << "( "; // if root of list, print left parenthesis
		// print car
		print(list[root].lchild, true);
		// print cdr, minus the left parenthesis
		if (list[root].rchild != 0)
		{ // print cdr, minus the left parenthesis
			print(list[root].rchild, false);
		}
		else
			cout << ")"; // print cdr, minus the left parenthesis(if cdr is (), minus '(' is just ')')
	}
}

void ListTable::deletetree(int root)
{
	if (root <= 0)
		return;
	else
	{
		deletetree(list[root].lchild);
		deletetree(list[root].rchild);
		deallocate(root);
	}
}

void ListTable::copy(int &pasted, int &copied)
{
	if (copied <= 0)
	{
		pasted = copied;
		return;
	}
	pasted = allocate();
	if (error(pasted))
		return;
	copy(list[pasted].lchild, list[copied].lchild);
	copy(list[pasted].rchild, list[copied].rchild);
}

void ListTable::print_entire(int root)
{
	cout << "Free list's root = " << freeindex << endl;
	cout << "List's root = " << root << endl;
	cout << "Tree array" << endl;
	cout << "(node, left child, right child, flag)" << endl;
	for (int i = 1; i <= size; ++i)
	{
		cout << "(" << i << ", " << list[i].lchild << ", " << list[i].rchild << ", " << list[i].mchild << ")" << endl;
	}
	ht->print_entire();
}

void ListTable::make_true(int root)
{
	if (root <= 0)
		return;
	make_true(car(root));
	make_true(cdr(root));
	list[root].mchild = true;
	return;
}

void ListTable::gc()
{
	cout << "----- garbage collecting -----" << endl;
	make_false();
	for (int i = 1; i <= ht->size; ++i)
	{
		if (ht->value(i) > 0)
			make_true(ht->hashtable[i].rchild);
	}
	Node<int, int, bool> temp;
	for (int i = 0; i <= stack->last; ++i)
	{
		temp = stack->stack[i];
		make_true(temp.rchild);
	}
	for (int i = 0; i <= gcstack->last; ++i)
	{
		make_true(gcstack->stack[i]);
	}
	for (int i = size; i >= 1; --i)
	{
		if (!list[i].mchild)
			deallocate(i);
	}
}