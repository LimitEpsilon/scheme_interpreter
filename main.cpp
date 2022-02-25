#include <iostream>
#include <string>
#include "tokenized.h"
#include "tables.h"
#include "preprocessor.h"
using namespace std;

int main() {
	cout << "Scheme interpreter, ver. 191015, by Joonhyup Lee, Department of ECE at SNU." << endl;
	cout << "Welcome to the Scheme interpreter! To end your input, press ENTER." << endl;
	Tokenized* input = new Tokenized(); //make buffer
	ListTable* memory = new ListTable(); //make memory
	Preprocessor* preprocessed = new Preprocessor();
	while (1) {
		cin >> *input;
		if (input->is_invalid()) { input->empty_buffer(); continue; }
		if (input->read_token() == "quit") break; input->putback_token();
		while (!input->read_all()) { //read until the last expression
			Tokenized* expression = input->read_expression();
			if (expression != nullptr) preprocessed->preprocessing(*expression);
			else continue;
			delete expression;
			int root = memory->read(*preprocessed);
			preprocessed->empty_buffer();
			int result = memory->eval(root);
			if (!memory->error(result)) {
				cout << "] "; memory->print(result, true); cout << endl;
				cout << endl;
			}
		}
		input->empty_buffer();
	}
	delete input;
	delete memory;
	delete preprocessed;
	return 0;
}