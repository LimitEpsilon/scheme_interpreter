#ifndef PREPROCESSOR_H_
#define PREPROCESSOR_H_
#include <iostream>
#include <string>
using namespace std;
class Tokenized;
/*
* Class name: Preprocessor
* Purpose: Takes an expression tokenized by the Tokenized class, and stores the preprocessed version of the expression.
* - This class is almost identical to the Tokenized class, as it also stores tokenized expressions for use by other
*   classes as a buffer for the expressions that are read.
* - Thus, only the methods not implemented in Tokenized will be explained.
* 1. insert(const string& s): Inserts s to the data, which is read by read_token, with the end symbol " ".
* 2. preprocessing(Tokenized& expression): Converts function declarations with lambda statements, and the ' symbol by
*                                          the quote operator.
*/
class Preprocessor {
private:
	string data = " ";
	int read = 0;
public:
	void empty_buffer() { data = " "; read = 0; };
	void insert(const string&);
	void preprocessing(Tokenized&);
	bool read_all() { return (read == data.length() - 1); } //no input or read until the last
	void putback_token() { if (read == 0) return; else { --read; while (data[read] != ' ') --read; } }
	string read_token();
	void print_token();
};
#endif