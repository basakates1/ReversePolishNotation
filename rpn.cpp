#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <functional>

class Stack {
private:
    struct Node {
        int value;
        Node* next;
        Node(int v, Node* n = nullptr) : value(v), next(n) {}
    };
    Node* topNode;

public:
    Stack() : topNode(nullptr) {}
    ~Stack() {
        while (topNode) {
            Node* temp = topNode;
            topNode = topNode->next;
            delete temp;
        }
    }

    void push(int value) {
        topNode = new Node(value, topNode);
    }

    int pop() {
        if (!topNode) throw std::runtime_error("Stack underflow");
        int value = topNode->value;
        Node* temp = topNode;
        topNode = topNode->next;
        delete temp;
        return value;
    }

    bool isEmpty() const {
        return topNode == nullptr;
    }
};

int evaluatePostfix(const std::string& expression) {
    Stack stack;
    std::istringstream tokens(expression);
    std::string token;

    while (tokens >> token) {
        if (isdigit(token[0]) || (token[0] == '-' && token.size() > 1)) {
            // Token is a number (supports negative numbers)
            stack.push(std::stoi(token));
        }
        else if (token == "+" || token == "-" || token == "*" || token == "/") {
            // Token is an operator
            if (stack.isEmpty()) throw std::runtime_error("Invalid expression");
            int b = stack.pop();
            if (stack.isEmpty()) throw std::runtime_error("Invalid expression");
            int a = stack.pop();

            int result;
            if (token == "+") result = a + b;
            else if (token == "-") result = a - b;
            else if (token == "*") result = a * b;
            else if (token == "/") {
                if (b == 0) throw std::runtime_error("Division by zero");
                result = a / b;
            }
            stack.push(result);
        }
        else {
            throw std::runtime_error("Invalid token: " + token);
        }
    }

    if (stack.isEmpty()) throw std::runtime_error("Invalid expression");
    int result = stack.pop();

    if (!stack.isEmpty()) throw std::runtime_error("Invalid expression: leftover items in stack");

    return result;
}

int main() {
    std::string expression;
    std::cout << "Enter a postfix expression: ";
    std::getline(std::cin, expression);

    try {
        int result = evaluatePostfix(expression);
        std::cout << "Result: " << result << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
