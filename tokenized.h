#ifndef TOKENIZED_H_
#define TOKENIZED_H_
#include <iostream>
#include <string>
#define ENDOFEXP '\n' // flag to stop reading in expressions
using namespace std;
/*
 * Class name: Tokenized
 * Purpose: Serves as a buffer for inputs. Using the input operator, the command is stored in tokenized form.
 * Member variables:
 * 1. string data: Stores the whole message typed by the user, until ENDOFEXP, in tokenized form.
 * 2. int read: Stores the number of characters already read by read_token
 * 3. bool invalid: Flag to identify whether the input is invalid by mismatching parenthesis.
 *
 * Public member functions:
 * 1. Input operator: Reads the input char by char. Upon encountering the start of a token, stores ' ' at the start of the token,
 *                    and after reading all of the valid expressions(until ENDOFEXP, i.e. end of expression), stores ' ' at the
 *                    end. Results in the data being stored as: "' '(Token1)' '(Token2)' '(Token3)' '...' '(Last Token)' '". Note
 *                    that when the start of a token is encountered, the operator first checks if the last element of data is ' '
 *                    or not. If it is, the token is inserted immediately. If not, ' ' is first inserted then the token is inserted.
 *                    This process ensures that the message is stored in its proper tokenized form.
 * 2. empty_buffer(): Replaces data by " ", which is the state of the data when it was initialized. read = 0 signifies that read_token
 *                    must again read the data from the first. The invalid flag is set to false, since it is not invalid.
 * 3. read_all(): Answers if the data stored is read until the end. Since the message is always stored in the form of ' '(token)' '...
 *                ' '(last token)' ' and the member variable read points to the ' ' right after the token that was previously read,
 *                reading the last token results read pointing the last ' ', i.e the last element in data. Thus, determining whether
 *                read(as an array index) is the length of the string minus 1(last index of string) is the operation that tells whether
 *                the message is read in its entirety.
 * 4. is_invalid(): Answers if the input was invalid by mismatching parenthesis.
 * 5. putback_token(): Was used by the read function of the ListTable class. Puts back the token that was just read in the buffer, as
 *                     if it was not read yet. Done by moving this->read backwards, from the '\0' after the token to the '\0' before the
 *                     parenthesis.
 * 6. read_token(): Reads the token from this->data, while storing the token in a string, then returns the string by value. Implemented
 *                  by reading until the next '\0' is encountered. Returns empty string when reaching the end of the expression being
 *                  read, i.e when readparen is 0.
 * 7. read_expression(): Since the message given by the user might consist of multiple expressions, read_expression is called to extract
 *                       each expression that makes up the whole message, which is then evaluated, until all expressions are read.
 * 8. print_token(): Prints the buffer until the last '\0'. Made for debugging purposes.
 */
class Tokenized
{
private:
	string data = " ";
	typeof(data.length()) read = 0;
	bool invalid = false;

public:
	friend istream &operator>>(istream &, Tokenized &);
	void empty_buffer()
	{
		data = " ";
		read = 0;
		invalid = false;
	}
	bool read_all() { return (read == (data.length() - 1)); } // no input or read until the last blank space
	bool is_invalid() { return this->invalid; }
	void putback_token()
	{
		if (read == 0)
			return;
		else
		{
			--read;
			while (data[read] != ' ')
				--read;
		}
	}
	string read_token();
	Tokenized *read_expression();
	void print_token();
};
#endif