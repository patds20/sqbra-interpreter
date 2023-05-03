#pragma once
#ifndef SQBRA_TOKENS_H
#define SQBRA_TOKENS_H

/*
 *  TOKENS.H
 *  This file defines the Node structure for tree and all the possible commands.
 *  Copyright (c) 2023, Patrick De Smet
 */

#include <string>
#include <vector>
#include "MathCore.cpp"

using namespace std;

//! Enum for Token Types
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
}; // the type of data (type of command, expression, string, ...)

class Node {
public:
    TokenType type;
    std::vector<Node*> children;
    void *value;
    MathNode* expression;
}; // nodes contain commands, expressions, variables, values, statements, ...

#endif //SQBRA_TOKENS_H
