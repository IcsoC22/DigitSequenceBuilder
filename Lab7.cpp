#include <iostream>
#include <string>

#define STRINGS "String"

using namespace std;

string current_token;
string current_attribute;

//function prototypes
void match(string);
string expr();
string term();
string factor();
string get_token();
void error(string);

//The Program constructs sequences of strings of digits
//Operators : '^', '.', and '(' ')'
//String1 '^' String2: results in String1 repeated String2 times. Right associative, i.e. String1 '^' String2 '^' String3
//		does String2 repeated String3 times, first, and then String1 repeated that many times
//String1 '.' String2: results in the concatenation, String1String2. Left associative, i.e String1 '.' String2 '.' String3
//		does String1String2 and then thatString3
//'(' Expression1 ')' Expression2: results in Expression1 evaluated first and then Expression2
//Precedence: '(' ')' > '^' > '.'
//'(' ')' operations will be performed first, followed by '^' operations, and finally '.' operations

//The program keeps running until either force closed or an error is thrown.
//To exit you can you pass through any character that is not a digit, ., or ^ or break the syntax rules

int main()
{
	string value;
	cout << ">> ";
	current_token = get_token();
	while (1)
	{
		value = expr();
		cout << "= " << value <<"\n>> ";
		match("\n");
	}


}

/* Grammar
E -> T E'
E' -> '.' T E' | empty
T -> F T'
T' -> '^' T | empty
F -> '(' E ')' | STRING
*/


// Handler for E and E'
//Left Associative
string expr()
{
	string value;
	value = term();
	while (1)
	{
		if (current_token == ".")
		{
			match(".");
			value = value + term();
		}
		else
			break;
	}
	return value;
}

// Handler for T and T'
// Right Associative
string term()
{
	string value;
	value = factor();
		if (current_token == "^")
		{
			match("^");
			string v = value;
			int a = stoi(term());
			for (int i = 0; i < a - 1; i++)
			{
				value = value + v;
			}
	}
	return value;
}

// Handler for F
string factor()
{
	string value;
	if (current_token == "(")
	{
		match("(");
		value = expr();
		match(")");
	}
	else if (current_token == STRINGS)
	{
		value = current_attribute;
		match(STRINGS);
	}

	else
		error("Illegal token in Factor");

	return value;
}


// Match
// Used to move from one toke to the next.
// Will through an error if the current token is not what is expected, i.e. tries to match '^' with '.'
void match(string expected_token)
{
	if (current_token == expected_token)
		current_token = get_token();

	else
		error("Unexpected token");
}

// Scanner
// Used to get the different tokens
// Will throw an error if there is any token other than what is accepted by the program
// Legal tokens are digits, '^', '.', '(', ')', and '\n'. Whitespace is ignored
string get_token()
{
	int c;
	string r;
	int value;

	while (1)
	{
		switch (c = getchar())
		{
			//Operators
		case '.': case '^':
		{
			r = c;
			return r;
		}

		//Whitespace
		case ' ': case '\t':
			continue;

			//Parenthesis
		case '(': case ')':
		{
			r = c;
			return r;
		}

		//Newline
		case '\n':
		{
			r = c;
			return r;
		}

		//The rest
		default:
			if (isdigit(c))
			{
				value = 0;
				//Collecting the Number
				do
				{
					value = 10 * value + c - '0';
				} while (isdigit(c = getchar()));
				current_attribute = to_string(value);
				ungetc(c, stdin);
				return STRINGS;
			}
			else error("Illegal Character");
		}
	}
}


// Error
// prints out the error message given
// has a cin statement to keep the window open so the user can see the error message
void error(string message)
{
	cout << "Error: "<< message << "\n";
	cin >> current_attribute;
	exit(1);
}
