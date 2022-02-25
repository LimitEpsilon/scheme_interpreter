#ifndef TABLES_H_
#define TABLES_H_
#include <iostream>
#include <string>
#define HASH_SIZE 1031
#define LIST_SIZE 30
#define STACK_SIZE 2048
#define NIL 0
#define LEFT_PARENS 1 //minus the hash values
#define RIGHT_PARENS 2
#define TRUE 3
#define FALSE 4
#define PLUS 5
#define MINUS 6
#define TIMES 7
#define isNUMBER 8
#define isSYMBOL 9
#define isNULL 10
#define CONS 11
#define COND 12
#define ELSE 13
#define CAR 14
#define CDR 15
#define DEFINE 16
#define QUOTE 17
#define LAMBDA 18
#define isEQ 19
#define isEQUAL 20
using namespace std;
class Preprocessor; //forward declaration
class ListTable;
class HashTable;
/*
* Class name: Node<T, S>
* Purpose: Used in ListTable to store the lchild and rchild, used in HashTable to store the token and value(in eval).
* Member variables:
* 1. T lchild: The first element in the node.
* 2. S rchild: The second element in the node.
* 3. Constructor: Assigns the basic form of each type, T and S, to the member variables.
* 4. Constructor(const T l, const S r): Assigns l to lchild and r to rchild.
*/
template <typename T, typename S, typename W>
class Node {
public:
	T lchild = T();
	S rchild = S();
	W mchild = W();
	Node() { lchild = T(); rchild = S(); mchild = W(); }
	Node(const T l, const S r, const W m): lchild(l), rchild(r), mchild(m) {}
};

/*
* Class name: HashTable
* Purpose: Used as a hash table for the tokens.
* Member variables:
* 1. Node<string, int> hashtable[HASH_SIZE + 1]: The hash table, stores (token, value). -index is the hash. The zeroth
*                                                index is not used, hence the size is HASH_SIZE + 1
*
* Private member functions:
* 1. hash(const string& t): Calculates the output of the hash function, when the input is t. The hash function adds up
*                           the ASCII values of the string, replaces it with the remanider when divided by HASH_SIZE,
*                           adds it by 1, then negates it to create a strictly negative hash value not below HASH_SIZE.
* 2. token(int i): Returns the reference to the lchild of hashtable[i].
* 3. value(int i): Returns the reference to the rchild of hashtable[i].
*
* Public member functions:
* 1. Constructor: Fills up the table with empty strings, and empty values.
* 2. get_hash(const string& t): Returns the hash value of t. Calculates the hash function value of t, compares t with
*                               the entry at that value, and if that entry is filled with something else, moves down 
*                               the table until either t is found or an empty entry is found(if t was not assigned a 
*                               hash value yet). If t was not assigned a hash, store t in hashtable and return hash. 
*                               If not, return hash.
* 3. initialize(): Updates predefined operators and constants to the hash table.
* 4. find(const int h): Returns the token with hash value h, by reference.
* 5. print_entire(): Prints all the hash entries, i.e. entries where the token is not empty.
*/
class HashTable {
private:
	Node<string, int, bool> hashtable[HASH_SIZE + 1];
	int hash(const string& t) {
		if (t == "(") return -LEFT_PARENS;
		else if (t == ")") return -RIGHT_PARENS;
		else if (t == "#t") return -TRUE;
		else if (t == "#f") return -FALSE;
		else if (t == "+") return -PLUS;
		else if (t == "-") return -MINUS;
		else if (t == "*") return -TIMES;
		else if (t == "number?") return -isNUMBER;
		else if (t == "symbol?") return -isSYMBOL;
		else if (t == "null?") return -isNULL;
		else if (t == "cons") return -CONS;
		else if (t == "cond") return -COND;
		else if (t == "else") return -ELSE;
		else if (t == "car") return -CAR;
		else if (t == "cdr") return -CDR;
		else if (t == "define") return -DEFINE;
		else if (t == "quote") return -QUOTE;
		else if (t == "lambda") return -LAMBDA;
		else if (t == "eq?") return -isEQ;
		else if (t == "equal?") return -isEQUAL;
		int j = 0;
		for (auto i = t.begin(); i != t.end(); ++i) {
			j += (*i) * (*i);
			j %= size;
		}
		return -(j + 1);
	}
	string& token(int i) {
		if (i <= 0 || i > size) { return hashtable[0].lchild; }
		else { return hashtable[i].lchild; }
	}
	int& value(int i) {
		return hashtable[i].rchild;
	}
	bool& defined(int i) {
		return hashtable[i].mchild;
	}
	int size = HASH_SIZE;
public:
	friend ListTable;
	HashTable() {
		this->token(0) = string();
		size = HASH_SIZE;
		for (int i = 1; i <= size; ++i) {
			this->token(i) = string();
			this->value(i) = 0;
			this->defined(i) = false;
		}
	}
	int get_hash(const string&);
	void initialize();
	string& find(const int h) { return this->token(-h); }
	void print_entire();
};
/*
* Class name: StackTable
* Purpose: Used as a stack to keep track of the values that are changed while evaluating a lambda expression.
* Member variables:
* 1. StackElement stack[STACK_SIZE]: The stack. Since we have to store the hash value of the symbol of which its value
*                                    is changed and the original value, the stack must store the hash element, and the root which called
*                                    the function.
* 2. int last: Holds the top index, i.e. the index where the top element is stored.
*
* Public member functions:
* 1. isEmpty(): Checks if the stack is empty. Since there are no elements in the stack, the top index must be invalid, i.e -1.
* 2. push(const StackElement n): Adds n to the last, and increases last.
* 3. pop(): Returns the top element of the stack, and decreases last, making the empty space free for the stack to push new
*           values.
*/
template <typename T>
class StackTable {
private:
	T stack[STACK_SIZE];
	int last = -1;
public:
	friend ListTable;
	bool isEmpty() { return last == -1; }
	void push(const T n) {
		if (last == STACK_SIZE - 1) { cout << "Stack overflow" << endl; return; }
		stack[++last] = n;
	}
	T pop() {
		if (isEmpty()) { cout << "StackTable is empty, cannot pop." << endl; return T(); }
		return stack[last--];
	}
};

/*
* Class name: ListTable
* Purpose: Used as a memory to store the expression in parse tree format.
* Member variables:
* 1. Node<int, int, bool> list[LIST_SIZE + 1]: The memory. When the entry is free for allocation, the rchild of the entry
*                                              holds the index of the next entry. The last entry holds NIL, which is used
*                                              as a marker for the end of the linked list. Again, list[0] is not used.
* 2. HashTable *ht: The hash table used by the memory while reading and evaluating expressions.
* 3. StackTable *stack: The hash table used by the memory while evaluating expressions.
* 4. int freeindex: Holds the root of the list where the entries are free for allocation.
* 5. int errorindex: Stores the expression that caused an error.
* 6. int& car(int root): Returns the car of the list pointed by root, by reference.
* 7. int& cdr(int root): Returns the cdr of the list pointed by root, by reference.
* 
* Public member functions:
* 1. Constructor: Sets up the list as a linked list, with the freeindex pointing at the root of the list, and initializes
*                 the hash table and the stack.
* 2. Destructor: Deletes the hash table and the stack that was allocated in the heap.
* 3. Copy assignment operator: Copies the elements of the rvalue into this->list, this->ht, this->stack.
* 4. initialize(): Empties out the memory in its entirety.
* 5. allocate(): Returns the index of the root of the free list, and moves the freeindex to the rchild of the orginal
*                root. If the original root was the last entry, freeindex will be NIL. If allocate() is called after
*                this happens, since freeindex will equal NIL, there will be no more memory available and an error 
*                message will be printed.
* 6. copy(int& pasted, int& copied): Copies the contents of copied into pasted. If copied is a symbol, assign the same hash value.
*                                    If copied is a list, allocate memory to pasted, using pasted as the root, to build the same
*                                    tree structure.
* 7. deletetree(int root): Deletes the tree starting from the root root.
* 8. deallocate(int root): The entry with the index root will be put back into the free list. Implemented by inserting
*                          the entry at the top, with the rchild of list[root] pointing to the original root of the 
*                          free list, and freeindex pointing to root.
* 9. error(int root): Only inputs with an error deallocates the root without reassigning it with a proper value. Therefore, 
*                     checking if root is equal to the freeindex right after evaluating the root suffices to check if there was an
*                     error or not.
* 10. eval(int& root): Evaluates the expression starting at root, updating symbols in this->ht and using this->stack as the stack.
*                      If the expression is an error, update the errorindex and return 0. If the expression is a symbol, determine 
*                      if it is a number or a not. If it is numeric, assign the hash value. If it is not, assign the value specified
*                      in ht. If the expression is in the form of a list, check if the first argument is an operator and perform
*                      suitable operations. If the first argument is not an operator, return an error.
* 11. check_structure(const int& r1, const int& r2): Compares the structures of the lists pointed by roots r1 and r2. If r1
*                                                   or r2 are symbols, compare r1 and r2. Else, compare the structure between
*                                                   each car and each cdr, to reture true iff both the car and cdr have the
*                                                   same structure.
* 12. print_entire(int root): After the tree with root index root is made, print_entire might be called to debug the
*                            program. Prints the entire memory, telling where the tree is, and where the free list is.
* 13. read(Preprocessed& input): Reads one expression from the input, whether it be a symbol or a list, and
*                                stores it in the memory, returning the hash value of the expression. If the 
*                                first symbol is '(', reads all the expressions by recursion until ')' is
*                                encountered, then returns the root.
* 14. print(int root, bool first, HashTable& ht): Prints one expression at root. If first is triggered, it means that the
*                                                start of a list must be printed, so if the root is a valid index for the
*                                                ListTable, the left parenthesis is printed, then car, then cdr is printed.
*                                                If first is not triggered, it means that the root is a cdr of some list, 
*                                                so the cdr is printed w/o the left parenthesis.
*/
class ListTable {
private:
	Node<int, int, bool>* list;
	HashTable* ht;
	StackTable<Node<int, int, bool>>* stack;
	StackTable<int>* gcstack;
	int freeindex = 1;
	int size = LIST_SIZE;
	int& car(const int root) { return list[root].lchild; }
	int& cdr(const int root) { return list[root].rchild; }
public:
	ListTable() {
		list = new Node<int, int, bool>[LIST_SIZE + 1];
		ht = new HashTable();
		stack = new StackTable<Node<int, int, bool>>();
		gcstack = new StackTable<int>();
		size = LIST_SIZE;
		this->freeindex = 1;
		for (int i = 1; i <= size; ++i) {
			this->list[i].rchild = i % size + 1;
			this->list[i].mchild = false;
		}
		this->list[size].rchild = NIL;
		ht->initialize();
	}
	~ListTable() {
		delete[] list;
		delete ht;
		delete stack;
		delete gcstack;
	}
	ListTable& operator=(const ListTable& memory) {
		for (int i = 0; i <= size; ++i) list[i] = memory.list[i];
		for (int i = 0; i <= ht->size; ++i) ht->hashtable[i] = memory.ht->hashtable[i];
		for (int i = 0; i < STACK_SIZE; ++i) { stack->stack[i] = memory.stack->stack[i]; gcstack->stack[i] = memory.gcstack->stack[i]; }
	}
	void initialize();
	void doubling();
	void make_true(int root);
	void make_false() { for (int i = 1; i <= size; ++i) if (list[i].mchild) list[i].mchild = false; };
	void gc();
	int allocate();
	void copy(int& pasted, int& copied);
	void deallocate(int i);
	void deletetree(int root);
	bool error(int root) { return (root == -LEFT_PARENS); }
	int eval(int& root);
	bool check_structure(const int&, const int&);
	void print_entire(int root);
	int read(Preprocessor& input);
	void print(int root, bool first);
};
class ChainNode {
public:
	int hash, value;
	ChainNode* next;
	ChainNode() : hash(0), value(0), next(nullptr) {}
	ChainNode(int h, int v, ChainNode* n) : hash(h), value(v), next(n) {}
};
class LinkedStack {
private:
	ChainNode* head;
public:
	LinkedStack() { head = new ChainNode(); head->next = head; }
	void push(ChainNode* c) { ChainNode* temp = head->next; head->next = c; c->next = temp; }
	void push(int h, int v) { ChainNode* c = new ChainNode(h, v, nullptr); push(c); }
	ChainNode pop() { 
		ChainNode* top = head->next; 
		if (top == head) { cout << "LinkedStack is empty" << endl; return ChainNode(0, 0, nullptr); }
		else { head->next = top->next; int h = top->hash; int v = top->value; delete top; return ChainNode(h, v, nullptr); }
	}
	bool is_empty() { return (head == head->next); }
	~LinkedStack(){
		ChainNode* top = head->next;
		ChainNode* temp = nullptr;
		while (top != head) {
			temp = top->next;
			delete top;
			top = temp;
		}
		delete head;
	}
};
#endif