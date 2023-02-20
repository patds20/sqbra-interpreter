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

using namespace std;

// STRUCTURES
enum TokenType {
    ROOT,
    CVAR,
    CLIST,
    MVAR,
    INPUT,
    SET,
    CEIL,
    FLOOR,
    LISTIDENT,
    ROUND,
    LOOP,
    SLOOP,
    WHILE,
    INC,
    DEC,
    IF,
    PRINT,
    PRINTB,
    PRINTV,
    NEWL,
    END,
    STRING,
    VARIABLE,
    VARIABLELIST,
    STATEMENT,
    EXPRESSION,
    VALUE,
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
    FUNCT,
    CALL,
    XROOT
}; // the type of data (type of command, expression, string, ...)

class Node {
public:
    TokenType type;
    std::vector<Node> children;
    std::string value;
}; // nodes contain commands, expressions, variables, values, statements, ...

#endif //SQBRA_TOKENS_H
