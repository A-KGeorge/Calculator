#include <iostream>
#include <stack>
#include <list>
#include <string>
#include <queue>

struct datum
{
	long double number;
	char charecters;
	datum()
	{
		charecters = ' ';
		number = 1;
	}
};

struct Calculator 
{
	std::stack<char> operators;
	std::stack<long double> operands;
	std::list<struct datum> inputExpressions;
	std::queue<struct datum> tempOperators1;
	std::queue<struct datum> tempOperators2;
	bool isDigit(char);
	bool validOperators(char);
	int precedence(char);
	void input(std::string);
	void infixToPostfix();
	void evaluate();
};

int Calculator::precedence(char c)
{
	switch (c)
	{
		case '^':return 3;
		case '/':
		case '*':return 2;
		case '+':
		case '-':return 1;
		default:return 0;
	}
}

bool Calculator::validOperators(char c)
{
	return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '(' || c == ')');
}

bool Calculator::isDigit(char c)
{
	return (isdigit(c) || c == '.');
}

void Calculator::input(std::string inp)
{
	int index = 0;
	while (index < inp.length())
	{
		if (inp[index] != ' ')
		{
			if (validOperators(inp[index]))
			{
				struct datum temp;
				temp.charecters = inp[index];
				inputExpressions.push_back(temp);
				index++;
			}
			else if (isDigit(inp[index]) == true)
			{
				std::string temp = "";
				while (isDigit(inp[index]) == true)
				{
					temp += inp[index];
					index++;
				}
				struct datum temp1;
				temp1.number = atof(temp.data());
				inputExpressions.push_back(temp1);
			}
			else
			{
				std::cout << "Exiting the program.....";
				exit(0);
			}
		}
		else
			index++;
	}
}
void Calculator::infixToPostfix()
{

	while (!tempOperators1.empty())
	{
		struct datum c = tempOperators1.front();
		tempOperators1.pop();

		if (c.charecters == '(')
		{
			operators.push('(');
		}

		else if (c.charecters == ')')
		{
			while (true)
			{
				char ctemp = operators.top();
				operators.pop();
				if (ctemp == '(' || ctemp == '\0' || operators.empty())
					break;
				else
				{
					struct datum z;
					z.charecters = ctemp;
					tempOperators2.push(z);
				}
			}
		}
		else if (c.charecters == '+' || c.charecters == '*' || c.charecters == '-' || c.charecters == '!' || c.charecters == '/' || c.charecters == '^')
		{
			if (!operators.empty() && precedence(operators.top()) <= precedence(c.charecters))
			{
				operators.push(c.charecters);
			}
			else
			{
				while (!operators.empty() && precedence(operators.top()) > precedence(c.charecters))
				{
					struct datum z;
					z.charecters = operators.top();
					tempOperators2.push(z);
					operators.pop();
				}
				operators.push(c.charecters);
			}
		}
		else
		{
			tempOperators2.push(c);
		}
	}
	while (!operators.empty())
	{
		struct datum z;
		z.charecters = operators.top();
		tempOperators2.push(z);
		operators.pop();
	}
}

void Calculator::evaluate()
{
	while (!tempOperators2.empty())
	{
		struct datum c = tempOperators2.front();
		tempOperators2.pop();

		if (c.charecters == '+' || c.charecters == '*' || c.charecters == '-' || c.charecters == '/' || c.charecters == '^') //binary operators
		{
			long double operand2 = operands.top();
			operands.pop();
			long double operand1 = operands.top();
			operands.pop();

			switch (c.charecters)
			{
				case '+':
					operand1 += operand2;
					break;
				case '-':
					operand1 -= operand2;
					break;
				case '*':
					operand1 *= operand2;
					break;
				case '/':
					operand1 /= operand2;
					break;
				case '^':
					int x = operand1;
					if ((int)operand2 == operand2)
						for (register int i = 1; i < (int)operand2; ++i)
							operand1 *= x;
					else
						operand1 = pow(operand1, operand2);
					break;
			}
			operands.push(operand1);
		}
		else if (c.charecters == ' ')
			operands.push(c.number);
	}
}


int main()
{
	std::cout << "Supported operations:\n";
	std::cout << "Binary operations:\n";
	std::cout << "+		      addition\n";
	std::cout << "-		   subtraction\n";
	std::cout << "*		multiplication\n";
	std::cout << "/			  division\n";
	std::cout << "^			 exponents\n";

	std::cout << "Enter invalid character (not a number or an operator) to quit the program.\n";

	while (true)
	{
		Calculator FX_991MS;
		std::cout << "Enter a mathematical expression: " << std::endl;
		std::string expression;
		std::getline(std::cin, expression, '\n');
		FX_991MS.input(expression);
		std::list<struct datum>::iterator iterator = FX_991MS.inputExpressions.begin();
		while (iterator != FX_991MS.inputExpressions.end())
		{
			FX_991MS.tempOperators1.push(*iterator);
			++iterator;
		}
		FX_991MS.inputExpressions.clear();
		FX_991MS.infixToPostfix();
		FX_991MS.evaluate();
		std::cout << "Result: \n" << FX_991MS.operands.top() << std::endl;
	}
	return 0;
}