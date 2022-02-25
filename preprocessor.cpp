#include <iostream>
#include <string>
#include "tokenized.h"
#include "tables.h"
#include "preprocessor.h"
using namespace std;
void Preprocessor::insert(const string& s) {
	data += s;
	data += " ";
}

void Preprocessor::preprocessing(Tokenized& expression) {
	string token;
	while (!(token = expression.read_token()).empty()) { //if there are still tokens inside the expression
		if (token == "define") { //if the expression defines something
			insert(token);
			token = expression.read_token();
			if (token == "(") { //if the define defines a function
				token = expression.read_token();
				insert(token);
				insert("(");
				insert("lambda");
				insert("(");
				this->preprocessing(expression);
				insert(")");
			}
			else insert(token);
		}
		else if (token == "'") {//if the expression is a quote
			insert("(");
			insert("quote");
			int nleft = 0;
			do {
				token = expression.read_token();
				insert(token);
				if (token == "(") ++nleft;
				else if (token == ")") --nleft;
			} while (nleft > 0);
			insert(")");
		}
		else insert(token);
	}
}
string Preprocessor::read_token() {
	if (read >= data.length() - 1) {
		return string();
	}
	int temp = read;
	++read;
	while (data[read] != ' ') {
		++read;
	}
	string s = string();
	for (int i = temp + 1; i < read; ++i) {
		s.push_back(tolower(this->data[i]));
	}
	return s;
}
void Preprocessor::print_token() {
	cout << data << "x" << endl;
}