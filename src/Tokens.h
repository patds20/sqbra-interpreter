#pragma once
#ifndef SQBRA_TOKENS_H
#define SQBRA_TOKENS_H

/*
 *  TOKENS.H
 *  This file declares the required funtions and variables for the Parser and all possible tokens.
 *  Copyright (c) 2023, Patrick De Smet
 */

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <stack>
#include <cmath>
#include <unordered_map>
#include <vector>
#include <iterator>

/// MathCore Declarations #############

extern std::unordered_map<std::string, long double*> vars; // global map for all vars (only required during parsing)
extern std::unordered_map<std::string, std::vector<long double>*> ars; // global map for all arrays (only required during parsing)
extern std::unordered_map<std::string, std::vector<std::vector<long double>* >*> mars; // global map for all matrices (only required during parsing)

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
    std::string var;
    std::string ind1;
    std::string ind2;
};

long double calculateExpression(MathNode* root);

inline std::string getOperator(MathOperatorType type);

MathNode* mathparse(const std::string& expr);

MathNode* varparse(const std::string& expr);

std::string getOperator(MathOperatorType type);

// TOKENTYPE (all possible tokens)

enum TokenType {
    ROOT,
    CVAR,
    CLIST,
    CMAT,
    MVAR,
    INPUT,
    SET,
    CEIL,
    FLOOR,
    LISTIDENT,
    MATIDENT,
    ROUND,
    LOOP,
    SLOOP,
    AUTOLOOP,
    WHILE,
    INC,
    DEC,
    IF,
    ELIF,
    ELSE,
    PRINT,
    PRINTB,
    PRINTV,
    PRINTM,
    NEWL,
    END,
    STRING,
    VARIABLE,
    VARIABLES,
    EXPRESSION,
    VARLIST,
    EXEC,
    EXIT,
    SLEEP,
    LEAVE,
    LOG,
    EXP,
    SIN,
    COS,
    TAN,
    CSC,
    SEC,
    COT,
    ATAN,
    ACOS,
    ASIN,
    GETL,
    GETDIM,
    FUNCT,
    CALL,
    XROOT,
    ABS,
    PUSH,
    POP,
    CHSL,
    LDEF,
    MDEF,
    READF,
    WRITEF,
    RANDOM
};

class Node {
public:
    TokenType type;
    std::vector<Node*> children;
    void *value;
    MathNode* expression;
};

/// FUNCTIONS

Node* parseTree(const std::vector<std::vector<std::string> >& tokens, int startLine);

void printMathTree(MathNode* root, int level);

void printTree(Node* root, int level);

void printTreeRoot(Node* root);

#endif //SQBRA_TOKENS_H
