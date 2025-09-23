
#include <iostream>
#include <stack>
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
	default: throw std::invalid_argument("Operator have no precedence. " + _operator); break;
	}
}

int main()
{
	std::queue<std::string> postfix;
	std::stack<char> operators;
	std::string infix = "(2 + 2) / 2";
	std::string currentNumber = "";

	std::cout << "Infix String: " << infix << "\n\n";

	for (char c : infix) {
		if (c == ' ') continue;

		if (isdigit(c) || c == '.') {
			currentNumber += c;
			continue;
		}
		if (c == '(') {
			if (currentNumber != "") {
				postfix.push(currentNumber);
				currentNumber = "";
			}
			if (!operators.empty() && operators.top() == '*') {
				operators.push(c);
			}
			else {
				if (!postfix.empty()) operators.push('*');
				operators.push(c);
			}
		}
		if (c == ')') {
			postfix.push(currentNumber);
			currentNumber = "";
			std::string parsedOperator;
			
			while (operators.top() != '(') {
				parsedOperator += operators.top();
				postfix.push(parsedOperator);
				operators.pop();
				parsedOperator = "";
			}
			operators.pop(); // remove '(' from operators stack
		}
		if (c == '+' || c == '-' || c == '*'|| c == '/') {
			if (currentNumber != "") {
				postfix.push(currentNumber);
				currentNumber = "";
			}

			char currentOperator = c;

			if (operators.empty()) {
				operators.push(currentOperator);
			} 
			else if (getPrecedenceOrder(currentOperator) <= getPrecedenceOrder(operators.top())) {
				std::string parsedOperator;
				parsedOperator += operators.top();
				operators.pop();
				postfix.push(parsedOperator);
				operators.push(currentOperator);
			}
			else if (getPrecedenceOrder(currentOperator) > getPrecedenceOrder(operators.top())) {
				operators.push(currentOperator);
			}
		}
	}

	if (currentNumber != "") postfix.push(currentNumber);
	while (!operators.empty()) {
		std::string parsedOperator;
		parsedOperator += operators.top();
		operators.pop();
		postfix.push(parsedOperator);
	}

	printPostfixString(postfix);


	std::stack<std::string> numbers;
	std::cout << "Evaluating the postfix string calculation: \n\n";

	while (!postfix.empty()) {
		std::string token = postfix.front();

		if (token == "+" || token == "-" || token == "*" || token == "/") {
			std::cout << "Operator (" << token << ") pop 2 operands to perform calculation." << "\n";

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
			std::cout << "Operand (" << token << ") push to the numbers stack" << "\n";
			numbers.push(token);
		}
		postfix.pop();	
	}

	std::cout << "\nFinal Result: " << numbers.top() << "\n\n";
	numbers.pop();
}
