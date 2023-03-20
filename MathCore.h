#pragma once
#ifndef SQBRA_MATHCORE_H
#define SQBRA_MATHCORE_H

/*
 *  MATHCORE.H
 *  This file contains the parser, optimizer and calculator for mathematical expressions.
 *  These functions are vital for the parser and many functions.
 *  Copyright (c) 2023, Patrick De Smet
 */

#include <iostream>
#include <string>
#include <stack>
#include <cmath>
#include <unordered_map>
#include <vector>
#include <iterator>

using namespace std;

unordered_map<string, long double> vars; // global map for all vars
unordered_map<string, vector<long double> > ars; // global map for all arrays

// Operator and MathNode Types
enum class MathNodeType { Constant, Variable, Array, Operator };
enum class MathOperatorType { PLUS, MINUS, MULT, DIV, MOD, EXP, AND, OR, EQ, LARGER, LESS, NOTSET};

struct MathNode {
    MathNodeType type;
    long double constant;
    MathOperatorType opt;
    string variable;
    MathNode* left;
    MathNode* right;
    MathNode(MathNodeType t, string v) : type(t), variable(std::move(v)), constant(0), left(nullptr), right(nullptr), opt(MathOperatorType::NOTSET) {}
    MathNode(MathNodeType t, long double v) : type(t), constant(v), left(nullptr), right(nullptr), opt(MathOperatorType::NOTSET) {}
};

inline bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^' || c == '&' || c == '|' || c == '=' || c == '<' || c == '>';
}

inline int precedence(char op) {
    if (op == '+' || op == '-') {
        return 1;
    } else if (op == '*' || op == '/' || op == '%') {
        return 2;
    } else if (op == '^') {
        return 3;
    } else {
        return 0;
    }
}

inline string removeSpaces(string str) {
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    return str;
}

MathNode* parseExpressionRaw(string expr) {
    stack<MathNode*> nodeStack;
    stack<char> opStack;
    int open_sqbr = 0;

    for (int i = 0; i < expr.size(); i++) {
        if (expr[i] == ' ') {
            continue;
        } else if (isdigit(expr[i]) || (expr[i] == '-' && (i == 0 || isOperator(expr[i-1]) || expr[i-1] == '(') || expr[i-1] == '[')) {
            string number;
            while (i < expr.size() && (isdigit(expr[i]) || expr[i] == '.' || expr[i] == '-')) {
                number += expr[i];
                i++;
            }
            i--;
            nodeStack.push(new MathNode(MathNodeType::Constant, stod(number)));
        } else if (isalpha(expr[i])) {
            string variable;
            while (i < expr.size() && (isalpha(expr[i]) || isdigit(expr[i]) || (isOperator(expr[i]) && open_sqbr >= 1) || (expr[i] == '[' || expr[i] == ']') || (open_sqbr >= 1 && (expr[i] == '(' || expr[i] == ')')))) {
                if(expr[i] == '['){
                    open_sqbr += 1;
                }else if(expr[i] == ']' && open_sqbr > 0){
                    open_sqbr -= 1;
                }
                variable += expr[i];
                i++;
            }
            i--;
            MathNode* tmp = new MathNode(MathNodeType::Variable, variable);
            if(variable.find('[') != std::string::npos){
                tmp->type = MathNodeType::Array;
                string arr_ind = variable.substr(variable.find('[')+1,variable.size()-variable.find('[')-2);
                tmp->left = parseExpressionRaw(arr_ind);
                string arr_id = variable.substr(0,variable.find('['));
                tmp->variable = arr_id;
            }
            nodeStack.push(tmp);
        } else if (expr[i] == '(') {
            opStack.push('(');
        } else if (expr[i] == ')') {
            while (!opStack.empty() && opStack.top() != '(') {
                char op = opStack.top();
                opStack.pop();
                MathNode* right = nodeStack.top();
                nodeStack.pop();
                MathNode* left = nodeStack.top();
                nodeStack.pop();
                MathNode* opMathNode = new MathNode(MathNodeType::Operator, string(1, op));
                opMathNode->left = left;
                opMathNode->right = right;
                nodeStack.push(opMathNode);
            }
            opStack.pop();
        } else if (isOperator(expr[i])) {
            char op = expr[i];
            while (!opStack.empty() && opStack.top() != '(' && precedence(opStack.top()) >= precedence(op)) {
                char op2 = opStack.top();
                opStack.pop();
                MathNode* right = nodeStack.top();
                nodeStack.pop();
                MathNode* left = nodeStack.top();
                nodeStack.pop();
                MathNode* opMathNode = new MathNode(MathNodeType::Operator, string(1, op2));
                opMathNode->left = left;
                opMathNode->right = right;
                nodeStack.push(opMathNode);
            }
            opStack.push(op);
        }
    }

    while (!opStack.empty()) {
        char op = opStack.top();
        opStack.pop();
        MathNode* right = nodeStack.top();
        nodeStack.pop();
        MathNode* left = nodeStack.top();
        nodeStack.pop();
        MathNode* opMathNode = new MathNode(MathNodeType::Operator, string(1, op));
        opMathNode->left = left;
        opMathNode->right = right;
        nodeStack.push(opMathNode);
    }

    return nodeStack.top();
}

long double calculateExpression(MathNode* root){
    if(root->type == MathNodeType::Constant){
        return root->constant;
    }else if(root->type == MathNodeType::Variable) {
        if (vars.find(root->variable) == vars.end()) {
            cerr << "Error: variable (" << root->variable << ") does not exist" << endl;
            exit(0); // terminate program
        } else {
            return vars[root->variable]; // get entry of map
        }
    }else if(root->type == MathNodeType::Operator){
        if(root->opt == MathOperatorType::PLUS){
            return calculateExpression(root->left) + calculateExpression(root->right);
        }else if(root->opt == MathOperatorType::MINUS){
            return calculateExpression(root->left) - calculateExpression(root->right);
        }else if(root->opt == MathOperatorType::DIV){
            return calculateExpression(root->left) / calculateExpression(root->right);
        }else if(root->opt == MathOperatorType::MULT){
            return calculateExpression(root->left) * calculateExpression(root->right);
        }else if(root->opt == MathOperatorType::MOD){
            return fmod(calculateExpression(root->left), calculateExpression(root->right));
        }else if(root->opt == MathOperatorType::EXP){
            return pow(calculateExpression(root->left),calculateExpression(root->right));
        }else if(root->opt == MathOperatorType::AND) {
            return (calculateExpression(root->left) > 0 && calculateExpression(root->right) > 0);
        }else if(root->opt == MathOperatorType::OR) {
            return (calculateExpression(root->left) > 0 || calculateExpression(root->right) > 0);
        }else if(root->opt == MathOperatorType::EQ){
            return (calculateExpression(root->left) == calculateExpression(root->right));
        }else if(root->opt == MathOperatorType::LARGER){
            return (calculateExpression(root->left) > calculateExpression(root->right));
        }else if(root->opt == MathOperatorType::LESS) {
            return (calculateExpression(root->left) < calculateExpression(root->right));
        }
    }else if(root->type == MathNodeType::Array){
        if (ars.find(root->variable) == ars.end()) {
            cerr << "Error: list entry (" << root->variable << ") does not exist" << endl;
            exit(0); // terminate program
        } else {
            return ars[root->variable][(size_t)calculateExpression(root->left)]; // get entry of map
        }
    }
    cerr << "Error: math_core cannot calculate expression." << endl;
    exit(0);
}

inline char getOperatorChar(MathOperatorType type){
    if(type == MathOperatorType::PLUS){
        return '+';
    }else if(type == MathOperatorType::MINUS){
        return '-';
    }else if(type == MathOperatorType::MULT){
        return '*';
    }else if(type == MathOperatorType::DIV){
        return '/';
    }else if(type == MathOperatorType::MOD){
        return '%';
    }else if(type == MathOperatorType::EXP){
        return '^';
    }else if(type == MathOperatorType::AND){
        return '&';
    }else if(type == MathOperatorType::OR){
        return '|';
    }else if(type == MathOperatorType::EQ){
        return '=';
    }else if(type == MathOperatorType::LARGER){
        return '>';
    }else if(type == MathOperatorType::LESS){
        return '<';
    }
    return ' ';
}

inline void setOperatorType(MathNode* root){
    if(root->type == MathNodeType::Operator){
        if(root->variable == "+"){
            root->opt = MathOperatorType::PLUS;
        }else if (root->variable == "-"){
            root->opt = MathOperatorType::MINUS;
        }else if (root->variable == "*"){
            root->opt = MathOperatorType::MULT;
        }else if (root->variable == "/"){
            root->opt = MathOperatorType::DIV;
        }else if (root->variable == "%"){
            root->opt = MathOperatorType::MOD;
        }else if (root->variable == "^"){
            root->opt = MathOperatorType::EXP;
        }else if (root->variable == "&"){
            root->opt = MathOperatorType::AND;
        }else if (root->variable == "|"){
            root->opt = MathOperatorType::OR;
        }else if (root->variable == "="){
            root->opt = MathOperatorType::EQ;
        }else if (root->variable == ">"){
            root->opt = MathOperatorType::LARGER;
        }else if (root->variable == "<"){
            root->opt = MathOperatorType::LESS;
        }
        root->variable = "";
    }
}

void optimizeExpression(MathNode* root){
    setOperatorType(root); // Change char operator to enum for faster operations

    if(root->left != nullptr){ // Recurse the entire tree
        optimizeExpression(root->left);
    }
    if(root->right != nullptr){
        optimizeExpression(root->right);
    }

    // Simplify constant expressions
    if(root->type == MathNodeType::Operator && root->left->type == MathNodeType::Constant && root->right->type == MathNodeType::Constant){
        // Replace operator node with constant node
        root->constant = calculateExpression(root);
        root->type = MathNodeType::Constant;
        root->left = nullptr;
        root->right = nullptr;
        root->opt = MathOperatorType::NOTSET;
    }

    // Simplify multiplication by 0 or 1
    if(root->type == MathNodeType::Operator && root->opt == MathOperatorType::MULT){
        if(root->left->type == MathNodeType::Constant){
            if(root->left->constant == 0.0){
                // Multiplication by 0
                root->type = MathNodeType::Constant;
                root->constant = 0.0;
                root->left = nullptr;
                root->right = nullptr;
                root->opt = MathOperatorType::NOTSET;
            }else if(root->left->constant == 1.0){
                // Multiplication by 1
                MathNode* newRoot = root->right;
                *root = *newRoot;
                delete newRoot;
            }
        }else if(root->right->type == MathNodeType::Constant){
            if(root->right->constant == 0.0){
                // Multiplication by 0
                root->type = MathNodeType::Constant;
                root->constant = 0.0;
                root->left = nullptr;
                root->right = nullptr;
                root->opt = MathOperatorType::NOTSET;
            }else if(root->right->constant == 1.0){
                // Multiplication by 1
                MathNode* newRoot = root->left;
                *root = *newRoot;
                delete newRoot;
            }
        }
    }
    // Simplify addition or subtraction with 0
    if(root->type == MathNodeType::Operator && (root->opt == MathOperatorType::PLUS || root->opt == MathOperatorType::MINUS)){
        if(root->left->type == MathNodeType::Constant && root->left->constant == 0.0){
            // Addition or subtraction with 0
            MathNode* newRoot = root->right;
            *root = *newRoot;
            delete newRoot;
        }
        else if(root->right->type == MathNodeType::Constant && root->right->constant == 0.0){
            // Addition or subtraction with 0
            MathNode* newRoot = root->left;
            *root = *newRoot;
            delete newRoot;
        }
    }

    // Simplify multiplication by -1
    if(root->type == MathNodeType::Operator && root->opt == MathOperatorType::MULT){
        if(root->left->type == MathNodeType::Constant && root->left->constant == -1.0){
            // Multiplication by -1
            MathNode* newRoot = root->right;
            root->type = MathNodeType::Operator;
            root->opt = MathOperatorType::MINUS;
            root->left = new MathNode(MathNodeType::Constant, 0.0);
            root->right = newRoot;
        }
        else if(root->right->type == MathNodeType::Constant && root->right->constant == -1.0){
            // Multiplication by -1
            MathNode* newRoot = root->left;
            root->type = MathNodeType::Operator;
            root->opt = MathOperatorType::MINUS;
            root->left = new MathNode(MathNodeType::Constant, 0.0);
            root->right = newRoot;
        }
    }

    // Simplify division by 1
    if(root->type == MathNodeType::Operator && root->opt == MathOperatorType::DIV){
        if(root->right->type == MathNodeType::Constant && root->right->constant == 1.0){
            // Division by 1
            MathNode* newRoot = root->left;
            *root = *newRoot;
            delete newRoot;
        }
    }
}

bool checkExpression(const string& expr){
    int brackets = 0;
    int sqbrackets = 0;
    for(char i : expr){
        if(i == '('){
            brackets += 1;
        }else if(i == ')'){
            brackets -= 1;
        }else if(i == '['){
            sqbrackets += 1;
        }else if(i == ']'){
            sqbrackets -= 1;
        }else if(!(i == ' ' || i == '.' || isdigit(i) || isalpha(i) || isOperator(i))){
            return false;
        }
    }
    if(brackets != 0 || sqbrackets != 0){
        return false;
    }
    return true;
}

MathNode* mathparse(const string& expr){
    if(checkExpression(expr)) {
        string expr_ws = removeSpaces(expr);
        MathNode* expression = parseExpressionRaw(expr_ws);
        optimizeExpression(expression);
        return expression;
    }else{
        cerr << "Error: <" << expr << "> expression is incorrect." << endl;
        exit(0);
    }
}

MathNode* varparse(const string& expr){
    if(checkExpression(expr)) {
        string expr_ws = removeSpaces(expr);
        MathNode* expression = parseExpressionRaw(expr_ws);
        optimizeExpression(expression);
        if(expression->type == MathNodeType::Array || expression->type == MathNodeType::Variable){
            return expression;
        }
        cerr << "Error: <" << expr << "> must be only a variable or list entry." << endl;
        exit(0);
    }else{
        cerr << "Error: <" << expr << "> variable or list entry are incorrect." << endl;
        exit(0);
    }
}

#endif //SQBRA_MATHCORE_H