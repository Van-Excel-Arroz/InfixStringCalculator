#include <iostream>
#include <stack>
#include <queue>
#include <string>
#include <cmath>

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
	else if (_operator == "^") {
		return pow(operand1, operand2);
	}
	else {
		throw std::invalid_argument("Invalid operator !" + _operator);
	}
}

int getPrecedenceOrder(char _operator) {
	switch (_operator) {
	case '(':
		return 0;
	case '-':
		return 1;
	case '+':
		return 1;
	case '*':
		return 2;
	case '/':
		return 2;
	case '^':
		return 3;
	default: throw std::invalid_argument("Operator have no precedence. " + _operator); break;
	}
}

bool isOperator(char _operator) {
	return _operator == '+' || _operator == '-' || _operator == '*' || _operator == '/' || _operator == '^';
}

void convertInfixToPostfix(std::string infix, std::queue<std::string>& postfix) {
	std::stack<char> operators;
	std::string currentNumber = "";

	std::cout << "\n\n==== Start of postfix conversion ====\n\n";

	for (char c : infix) {
		if (c == ' ') continue;

		if (isdigit(c) || c == '.') {
			currentNumber += c;
			continue;
		}
		else if (c == '(') {
			if (currentNumber != "") {
				postfix.push(currentNumber);
				std::cout << "Operand  (" << currentNumber << "): enqueue to the postfix queue\n";
				currentNumber = "";
			}
			if (!operators.empty()) {
				operators.push(c);
				std::cout << "Operator '" << c << "': push to the operators stack\n";
			}
			else {
				if (!postfix.empty()) {
					operators.push('*');
					std::cout << "Operator (*): push to the operators stack (implicit multiplication)\n";
				}
				operators.push(c);
				std::cout << "Operator '" << c << "': push to the operators stack\n";
			}
		}
		else if (c == ')') {
			if (currentNumber != "") {
				postfix.push(currentNumber);
				std::cout << "Operand  (" << currentNumber << "): enqueue to the postfix queue\n";
			}
			currentNumber = "";
			std::string parsedOperator;

			std::cout << "Operator ')': pop from operators stack ";

			while (!operators.empty() && operators.top() != '(') {
				parsedOperator += operators.top();
				std::cout << "(" << parsedOperator << ") ";
				postfix.push(parsedOperator);
				operators.pop();
				parsedOperator = "";
			}

			if (!operators.empty()) {
				operators.pop();
				std::cout << "and enqueue to postfix queue, also pop '(' from operators stack\n";
			}
		}
		else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
			if (currentNumber != "") {
				postfix.push(currentNumber);
				std::cout << "Operand  (" << currentNumber << "): enqueue to the postfix queue\n";
				currentNumber = "";
			}

			char currentOperator = c;

			if (operators.empty()) {
				operators.push(currentOperator);
				std::cout << "Operator (" << currentOperator << "): push to the operators stack\n";
			}
			else if (getPrecedenceOrder(currentOperator) <= getPrecedenceOrder(operators.top())) {
				std::string parsedOperator;
				parsedOperator += operators.top();
				std::cout << "Operator (" << currentOperator << "): pop (" << parsedOperator << ") from operators stack and enqueue it to postfix queue (precedence rule), ";
				operators.pop();
				postfix.push(parsedOperator);
				operators.push(currentOperator);
				std::cout << "then push (" << currentOperator << ") to the operators stack\n";
			}
			else if (getPrecedenceOrder(currentOperator) > getPrecedenceOrder(operators.top())) {
				operators.push(currentOperator);
				std::cout << "Operator (" << currentOperator << "): push to the operators stack\n";
			}
		}
		else {
			throw std::invalid_argument("Invalid Expression");
		}
	}

	if (currentNumber != "") {
		postfix.push(currentNumber);
		std::cout << "Operand  (" << currentNumber << "): enqueue to the postfix queue\n";
	}

	while (!operators.empty()) {
		std::string parsedOperator;
		parsedOperator += operators.top();
		std::cout << "Operator (" << parsedOperator << "): pop from operators stack and enqueue to postfix queue (remaining operators)\n";
		operators.pop();
		postfix.push(parsedOperator);
	}

	std::cout << "\n==== End of postfix conversion ======\n\n";
}

std::string evaluatePostfixExpression(std::queue<std::string>& postfix) {
	std::stack<std::string> numbers;
	std::cout << "==== Start of postfix string calculation ==== \n\n";

	while (!postfix.empty()) {
		std::string token = postfix.front();

		if (token == "+" || token == "-" || token == "*" || token == "/" || token == "^") {
			std::cout << "Operator (" << token << "): pop 2 operands to perform calculation: ";

			try {
				if (numbers.size() < 2) throw std::invalid_argument("Invalid Expression");
				double operand2 = std::stod(numbers.top());
				numbers.pop();
				double operand1 = std::stod(numbers.top());
				numbers.pop();

				double result = performCalculation(operand1, operand2, token);
				numbers.push(std::to_string(result));

				std::cout << operand1 << " " << token << " " << operand2 << " = " << result << "\n";
			}
			catch (std::invalid_argument) {
				std::cout << "Invalid Expression";
			}
		}
		else {
			std::cout << "Operand  (" << token << "): push to the numbers stack" << "\n";
			numbers.push(token);
		}
		postfix.pop();
	}
	std::cout << "\n==== End of postfix string calculation ======";

	if (!numbers.empty()) {
		return numbers.top();
	}
	return "0";
}


int main()
{
	std::string infix;
	char choice;

	do {
		std::cout << "Enter infix expression: ";
		std::getline(std::cin, infix);

		try {
			std::queue<std::string> postfix;
			convertInfixToPostfix(infix, postfix);
			printPostfixString(postfix);
			std::string result = evaluatePostfixExpression(postfix);
			std::cout << "\n\n\nFinal Result: " << result << "\n";
		}
		catch (std::invalid_argument) {
			std::cout << "Invalid Expression";
		}

		std::cout << "\n\nCalculate another? (y/n): ";
		std::cin >> choice;
		std::cin.ignore();
		std::cout << "\n\n";
	} while (choice == 'y' || choice == 'Y');

	return 0;
}