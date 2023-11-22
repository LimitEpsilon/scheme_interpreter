#include <iostream>
#include <string>
#include "tokenized.h"
using namespace std;
istream &operator>>(istream &is, Tokenized &t)
{
	cout << "> ";
	int lparens = 0;
	char c = 0;
	while ((c = is.get()) != ENDOFEXP)
	{
		if (c == ' ' || c == '\n' || c == '\t')
		{
			if (t.data.back() != ' ')
				t.data.push_back(' ');
		}
		else if (c == '(' || c == ')' || c == '\'')
		{
			if (t.data.back() != ' ')
				t.data.push_back(' ');
			t.data.push_back(c);
			t.data.push_back(' ');
			if (c == '(')
				++lparens;
			else if (c == ')')
				--lparens;
			if (0 > lparens)
			{
				t.invalid = true;
				cout << "Invalid input, parenthesis doesn't match up." << endl;
				return is;
			}
		}
		else if (c == ']')
		{
			if (t.data.back() != ' ')
				t.data.push_back(' ');
			int temp = lparens;
			for (int i = 0; i < temp; ++i)
			{
				t.data += ") ";
				--lparens;
			}
		}
		else
		{
			t.data.push_back(c);
		}
	}
	if (lparens != 0)
	{
		t.invalid = true;
		cout << "Invalid input, parenthesis doesn't match up." << endl;
		return is;
	}
	if (t.data.back() != ' ')
		t.data.push_back(' '); // If the end of the input was a symbol
	return is;
}

string Tokenized::read_token()
{
	if (read >= data.length() - 1)
	{
		return string();
	}
	auto temp = read;
	++read;
	while (data[read] != ' ')
	{
		++read;
	}
	string s = string();
	for (auto i = temp + 1; i < read; ++i)
	{
		s.push_back(tolower(this->data[i]));
	}
	return s;
}

Tokenized *Tokenized::read_expression()
{
	string token;
	int lparen = 0;
	Tokenized *expression = new Tokenized();
	token = read_token();
	if (token.empty())
		return nullptr;
	else if (token == "'")
	{ // is a quote
		expression->data += token;
		expression->data += " ";
	}
	else if (token == "(")
	{ // is a list
		expression->data += token;
		expression->data += " ";
		++lparen;
	}
	else
	{ // is a symbol
		expression->data += token;
		expression->data += " ";
		return expression;
	}
	while (1)
	{ // if quote or a list, read the operands
		token = read_token();
		expression->data += token;
		expression->data += " ";
		if (token == "(")
			++lparen;
		else if (token == ")")
			--lparen;
		if (lparen == 0)
			break;
	}
	return expression;
}

void Tokenized::print_token()
{
	cout << data << "x" << endl;
	return;
}
