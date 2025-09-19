
#include <iostream>
#include <stack>
#include <list>
#include <queue>
#include <string>


void printPostfixString(std::queue<std::string> postfix) {
	std::cout << "Postfix String: ";
	
	while (!postfix.empty()) {
		std::cout << postfix.front() << " ";
		postfix.pop();
	}

	std::cout << "\n\n";
}


double performCalculation(double operand1, double operand2, std::string _operator) {
	if (_operator == "+") {
		return operand1 + operand2;
	}
	else if (_operator == "-") {
		return operand1 - operand2;
	} 
	else if (_operator == "*") {
		return operand1 * operand2;
	} 
	else if (_operator == "/") {
		if (operand2 == 0) {
			throw std::invalid_argument("Can not divide by 0.");
		}
		return operand1 / operand2;
	} 
}

int getPrecedenceOrder(char _operator) {
	switch (_operator) {
	case '-':
		return 1;
	case '+':
		return 1;
	case '*':
		return 2;
	case '/':
		return 2;
	}
}

int main()
{
	std::queue<std::string> postfix;
	std::stack<char> operators;
	std::string infix = "24 + 4 / 2";
	std::string currentNumber = "";

	std::cout << "Infix String: " << infix << "\n\n";

	for (char c : infix) {
		if (c == ' ') continue;

		if (isdigit(c) || c == '.') {
			currentNumber += c;
			continue;
		}
		if (c == '+' || c == '-' || c == '*'|| c == '/') {
			postfix.push(currentNumber);
			currentNumber = "";
			std::string parsedOperator;
			char currentOperator = c;

			if (operators.empty()) {
				operators.push(currentOperator);
			} 
			else if (getPrecedenceOrder(currentOperator) <= getPrecedenceOrder(operators.top())) {
				parsedOperator += operators.top();
				operators.pop();
				postfix.push(parsedOperator);

				if (getPrecedenceOrder(currentOperator) == getPrecedenceOrder(operators.top())) {
					parsedOperator = "";
					parsedOperator += operators.top();
					operators.pop();
					postfix.push(parsedOperator);
				}

				operators.push(currentOperator);
			}
			else if (getPrecedenceOrder(currentOperator) > getPrecedenceOrder(operators.top())) {
				operators.push(currentOperator);
			}
		}
	}

	postfix.push(currentNumber);
	while (!operators.empty()) {
		std::string parsedOperator;
		parsedOperator += operators.top();
		operators.pop();
		postfix.push(parsedOperator);
	}

	printPostfixString(postfix);


	std::stack<std::string> numbers;

	while (!postfix.empty()) {
		std::string token = postfix.front();
		

		if (token == "+" || token == "-" || token == "*" || token == "/") {
			double operand2 = std::stod(numbers.top());
			numbers.pop();
			double operand1 = std::stod(numbers.top());
			numbers.pop();

			double result = performCalculation(operand1, operand2, token);
			numbers.push(std::to_string(result));

			/*
			* For Debugging
			* 
			std::cout << operand1 << " " << token << " " << operand2 << " = " << result << "\n";
			std::cout << "Result After Parsing: " << numbers.top() << "\n\n";
			*/
		}
		else {
			numbers.push(token);
		}
		postfix.pop();
	}

	std::cout << "Result: " << numbers.top() << "\n\n";
}
