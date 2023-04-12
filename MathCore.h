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

unordered_map<string, long double*> vars; // global map for all vars
unordered_map<string, vector<long double>*> ars; // global map for all arrays
unordered_map<string, vector<vector<long double>* >*> mars; // global map for all matrices


// Operator and MathNode Types
enum class MathNodeType { Constant, Variable, Array, Matrix, Operator };
enum class MathOperatorType { PLUS, MINUS, MULT, DIV, MOD, EXP, AND, OR, EQ, NEQ, LARGER, LARGEREQ, LESS, LESSEQ, NOTSET };

MathOperatorType getOperatorType(const char c);

struct MathNode {
    MathNodeType type;
    long double constant;
    MathOperatorType opt;
    void* variable;
    MathNode* left;
    MathNode* right;
    MathNode(MathNodeType t, void* v) : type(t), variable(v), constant(0), left(nullptr), right(nullptr), opt(MathOperatorType::NOTSET) {}
    MathNode(MathNodeType t, long double v) : type(t), constant(v), left(nullptr), right(nullptr), opt(MathOperatorType::NOTSET) {}
    MathNode(MathNodeType t, MathOperatorType s) : type(t), variable(nullptr), constant(0), left(nullptr), right(nullptr), opt(s) {}
};

struct MathTypeReturn {
    MathNodeType type;
    string var;
    string ind1;
    string ind2;
};

inline void* getPointer(const string& str, MathNodeType type){
    if (type == MathNodeType::Variable) {
        if (vars.find(str) != vars.end()) {
            return vars[str];
        } else {
            cerr << "Error: variable <" << str << "> is not defined." << endl;
            exit(0);
        }
    } else if (type == MathNodeType::Array) {
        if (ars.find(str) != ars.end()) {
            return ars[str];
        } else {
            cerr << "Error: list <" << str << "> is not defined." << endl;
            exit(0);
        }
    } else {
        if (mars.find(str) != mars.end()) {
            return mars[str];
        } else {
            cerr << "Error: matrix <" << str << "> is not defined." << endl;
            exit(0);
        }
    }
}

inline bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^' || c == '&' || c == '|' || c == '=' || c == '<' || c == '>' || c == '!' || c == 0x1d || c == 0x1e || c == 0x1f;
}

inline int precedence(char op) {
    if (op == '|') {
        return 1;
    } else if (op == '&') {
        return 2;
    } else if (op == '=' || op == '<' || op == '>' || op == 0x1d || op == 0x1e || op == 0x1f) {
        return 3;
    } else if (op == '+' || op == '-') {
        return 4;
    } else if (op == '*' || op == '/' || op == '%') {
        return 5;
    } else if (op == '^') {
        return 6;
    } else {
        return 0;
    }
}


inline string removeSpaces(string str) {
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    return str;
}

// Check if the input string s represents a 2D matrix or a 1D list, and extract its indices
inline MathTypeReturn determineType(string& str) {
    int count = 0;
    int dimensions = 0;
    MathTypeReturn res;
    for(auto i : str){
        if(i == '['){
            // Increment dimensions only once per opening bracket
            if(count == 0){
                dimensions++;
            }
            count++;
        } else if(i == ']') {
            count--;
        }
        // Extract components based on the count and dimension
        if(count == 0 && i != ']' || count == 1 && i != '[' || count > 1){
            if(dimensions == 0){
                res.var.push_back(i);
            } else if(dimensions == 1){
                res.ind1.push_back(i);
            } else{
                res.ind2.push_back(i);
            }
        }
    }
    // Return type of variable
    if(dimensions == 0){
        res.type = MathNodeType::Variable;
    } else if(dimensions == 1){
        res.type = MathNodeType::Array;
    } else if(dimensions == 2){
        res.type = MathNodeType::Matrix;
    } else{
        cerr << "Error: expression <" << str << "> is incorrect." << endl;
        exit(0);
    }
    return res;
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
            while (i < expr.size() && (isalpha(expr[i]) || isdigit(expr[i]) || expr[i] == '_' || (isOperator(expr[i]) && open_sqbr >= 1) || (expr[i] == '[' || expr[i] == ']') || (open_sqbr >= 1 && (expr[i] == '(' || expr[i] == ')')))) {
                if(expr[i] == '['){
                    open_sqbr += 1;
                }else if(expr[i] == ']' && open_sqbr > 0){
                    open_sqbr -= 1;
                }
                variable += expr[i];
                i++;
            }
            i--;
            MathNode* tmp;
            MathTypeReturn detT = determineType(variable);
            if(detT.type == MathNodeType::Variable){
                tmp = new MathNode(MathNodeType::Variable, getPointer(variable, MathNodeType::Variable));
            }else if(detT.type == MathNodeType::Array){
                tmp = new MathNode(MathNodeType::Array, getPointer(detT.var, MathNodeType::Array));
                tmp->left = parseExpressionRaw(detT.ind1);
            }else{
                tmp = new MathNode(MathNodeType::Matrix, getPointer(detT.var, MathNodeType::Matrix));
                tmp->left = parseExpressionRaw(detT.ind1);
                tmp->right = parseExpressionRaw(detT.ind2);
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
                MathNode* opMathNode = new MathNode(MathNodeType::Operator, getOperatorType(op));
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
                MathNode* opMathNode = new MathNode(MathNodeType::Operator, getOperatorType(op2));
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
        MathNode* opMathNode = new MathNode(MathNodeType::Operator, getOperatorType(op));
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
        return *((long double*)root->variable);
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
        }else if(root->opt == MathOperatorType::NEQ){
            return (calculateExpression(root->left) != calculateExpression(root->right));
        }else if(root->opt == MathOperatorType::LARGEREQ){
            return (calculateExpression(root->left) >= calculateExpression(root->right));
        }else if(root->opt == MathOperatorType::LESSEQ){
            return (calculateExpression(root->left) <= calculateExpression(root->right));
        }else if(root->opt == MathOperatorType::LARGER){
            return (calculateExpression(root->left) > calculateExpression(root->right));
        }else if(root->opt == MathOperatorType::LESS) {
            return (calculateExpression(root->left) < calculateExpression(root->right));
        }
    }else if(root->type == MathNodeType::Array) {
        auto list = reinterpret_cast<vector<long double>*>(root->variable);
        auto i = (int)(calculateExpression(root->left));
        if(list->size() > i){
            return (*list)[i];
        }else{
            cerr << "Error: Index out of bounds for list <" << root->variable << "> at index <" << i << ">." << endl;
            exit(0);
        }
    }else if(root->type == MathNodeType::Matrix) {
        auto mat = reinterpret_cast<vector<vector<long double>*>*>(root->variable);
        auto i1 = (int)(calculateExpression(root->left));
        auto i2 = (int)(calculateExpression(root->right));
        if(i1 < mat->size() && i2 < (*(*mat)[i1]).size()){
            return (*(*mat)[i1])[i2];
        }else{
            cerr << "Error: Index out of bounds for matrix <" << root->variable << "> at indices <" << i1 << "," << i2 << ">." << endl;
            exit(0);
        }
    }
    return 0;
}

inline void replace_special_operators(string& input) {
    size_t pos = 0;
    while ((pos = input.find("<=", pos)) != std::string::npos) {
        input.replace(pos, 2, 1, (char)0x1d);
        pos += 1;
    }
    pos = 0;
    while ((pos = input.find(">=", pos)) != std::string::npos) {
        input.replace(pos, 2, 1, (char)0x1e);
        pos += 1;
    }
    pos = 0;
    while ((pos = input.find("!=", pos)) != std::string::npos) {
        input.replace(pos, 2, 1, (char)0x1f);
        pos += 1;
    }
}


inline string getOperator(MathOperatorType type){
    if(type == MathOperatorType::PLUS){
        return "ADD";
    }else if(type == MathOperatorType::MINUS){
        return "SUB";
    }else if(type == MathOperatorType::MULT){
        return "MULT";
    }else if(type == MathOperatorType::DIV){
        return "DIV";
    }else if(type == MathOperatorType::MOD){
        return "MOD";
    }else if(type == MathOperatorType::EXP){
        return "EXP";
    }else if(type == MathOperatorType::AND){
        return "AND";
    }else if(type == MathOperatorType::OR){
        return "OR";
    }else if(type == MathOperatorType::EQ){
        return "EQ";
    }else if(type == MathOperatorType::LARGER){
        return "LARGER";
    }else if(type == MathOperatorType::LESS){
        return "LESS";
    }else if(type == MathOperatorType::NEQ){
        return "UNEQUAL";
    }else if(type == MathOperatorType::LESSEQ){
        return "LESSQ";
    }else if(type == MathOperatorType::LARGEREQ){
        return "LARGERQ";
    }
    return "";
}

MathOperatorType getOperatorType(const char c){
        if(c == '+'){
            return MathOperatorType::PLUS;
        }else if (c == '-'){
            return MathOperatorType::MINUS;
        }else if (c == '*'){
            return MathOperatorType::MULT;
        }else if (c == '/'){
            return MathOperatorType::DIV;
        }else if (c == '%'){
            return MathOperatorType::MOD;
        }else if (c == '^'){
            return MathOperatorType::EXP;
        }else if (c == '&'){
            return MathOperatorType::AND;
        }else if (c == '|'){
            return MathOperatorType::OR;
        }else if (c == '='){
            return MathOperatorType::EQ;
        }else if (c == '>'){
            return MathOperatorType::LARGER;
        }else if (c == '<'){
            return MathOperatorType::LESS;
        }else if (c == 0x1f){
            return MathOperatorType::NEQ;
        }else if (c == 0x1d){
            return MathOperatorType::LESSEQ;
        }else if (c == 0x1e){
            return MathOperatorType::LARGEREQ;
        }
        return MathOperatorType::NOTSET;
}

void optimizeExpression(MathNode* root){

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
        }else if(!(i == ' ' || i == '.' || i == '_' || isdigit(i) || isalpha(i) || isOperator(i))){
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
        replace_special_operators(expr_ws);
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
        replace_special_operators(expr_ws);
        MathNode* expression = parseExpressionRaw(expr_ws);
        optimizeExpression(expression);
        if(expression->type == MathNodeType::Array || expression->type == MathNodeType::Variable || expression->type == MathNodeType::Matrix){
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
