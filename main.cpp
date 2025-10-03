#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include <cmath>
#include <algorithm>

using namespace std;

bool validExpr = true;

void getExprAndEval();

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

bool hasBalancedBrackets(const string& expr) {
    stack<char> brackets;
    for (char c : expr) {
        if (c == '(')
            brackets.push(c);
        else if (c == ')') {
            if (brackets.empty())
                return false;
            brackets.pop();
        }
    }
    return brackets.empty();
}

int precedence(char op) {
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    if (op == '^')
        return 3;
    return 0;
}

double applyOp(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        case '^': return pow(a, b);
        default: return 0;
    }
}

double evaluate(string expr) {
    expr.erase(remove_if(expr.begin(), expr.end(), ::isspace), expr.end());
    if (!hasBalancedBrackets(expr)) {
        cout << "Invalid expression: " << expr << "." << endl;
        validExpr = false;
        return 0;
    }
    stack<double> values;
    stack<char> ops;
    int i = 0;
    bool isNegative = false;
    while (i < expr.length()) {
        if (expr[i] == '(') {
            ops.push(expr[i]);
        } else if (isdigit(expr[i]) || (expr[i] == '-' && (i == 0 || isOperator(expr[i - 1]) || expr[i - 1] == '(')) || (expr[i] == '+' && (i == 0 || isOperator(expr[i - 1]) || expr[i - 1] == '('))) {
            double num = 0;
            isNegative = expr[i] == '-' ? true : false;
            if (expr[i] == '-' || expr[i] == '+')
                i++;
            while (i < expr.length() && isdigit(expr[i])) {
                num = num * 10 + (expr[i] - '0');
                i++;
            }
            values.push(isNegative ? -num : num);
            isNegative = false;
            i--;
        } else if (isOperator(expr[i])) {
            while (!ops.empty() && precedence(ops.top()) >= precedence(expr[i])) {
                double val2 = values.top();
                values.pop();
                double val1 = values.empty() ? 0 : values.top();
                values.pop();
                char op = ops.top();
                ops.pop();
                values.push(applyOp(val1, val2, op));
            }
            ops.push(expr[i]);
        } else if (expr[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                double val2 = values.top();
                values.pop();
                double val1 = values.empty() ? 0 : values.top();
                values.pop();
                char op = ops.top();
                ops.pop();
                values.push(applyOp(val1, val2, op));
            }
            if (!ops.empty())
                ops.pop();
        } else {
            cout << "Invalid expression: " << expr << "." << endl;
            validExpr = false;
            return 0;
        }
        i++;
    }
    while (!ops.empty()) {
        double val2 = values.top();
        values.pop();
        double val1 = values.empty() ? 0 : values.top();
        values.pop();
        char op = ops.top();
        ops.pop();
        values.push(applyOp(val1, val2, op));
    }
    if (values.size() != 1) {
        cout << "Invalid expression: " << expr << "." << endl;
        validExpr = false;
        return 0;
    }
    return values.top();
}

void getExprAndEval() {
    cout << "Enter an arithmetic expression (you can enter (), +, -, *, / and ^): ";
    string expr;
    getline(cin, expr);
    double result = evaluate(expr);
    if (validExpr)
        cout << "Result: " << fixed << result << "." << endl;
    else {
        validExpr = true;
        getExprAndEval();
    }
}

int main() {
    getExprAndEval();
    return 0;
}
