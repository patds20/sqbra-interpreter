#pragma once
#ifndef SQBRA_PARSER_H
#define SQBRA_PARSER_H

/*
 *  PARSER.H
 *  This file contains the parser and print functions for the SquareBracket language.
 *  The parser analyzes the tokens and creates a distinct tree which can be executed in the next step.
 *  Copyright (c) 2023, Patrick De Smet
 */

#include <string>
#include <vector>
#include <iostream>
#include "Tokens.h"
#include "MathCore.h"

using namespace std;

int rLine = -1; // this variable tells the parser where to continue after an indented block

long double* makeVarEntry(const string& str){
    if (vars.find(str) == vars.end()){
        long double* newVar = (long double*)malloc(sizeof(long double));
        vars[str] = newVar;
        return newVar;
    } else {
        cerr << "Error: variable <" << str << "> is already defined." << endl;
        exit(0);
    }
}

vector<long double*>* makeMultipleVarEntries(const string& str){
    auto* tmpVec = new vector<long double*>;
    std::istringstream iss(str);
    std::vector<std::string> words;
    for (std::string word; iss >> word; ) {
        words.push_back(word);
    }
    for (const auto& word : words) {
        (*tmpVec).push_back(makeVarEntry(word));
    }
    return tmpVec;
}

vector<long double>* makeListEntry(const string& str){
    if (ars.find(str) == ars.end()){
        auto newArr = new vector<long double>;
        ars[str] = newArr;
        return newArr;
    } else {
        cerr << "Error: list <" << str << "> is already defined." << endl;
        exit(0);
    }
}

vector<long double>* getListEntry(const string& str){
    if (ars.find(str) != ars.end()) {
        return ars[str];
    } else {
        cerr << "Error: list <" << str << "> is not defined." << endl;
        exit(0);
    }
}

Node* parseTree(const std::vector<std::vector<std::string> >& tokens, int startLine) {
    Node* root = new Node;
    root->type = ROOT;
    std::vector<Node*> stack;
    stack.push_back(root);
    int currentLine = 0;

    while(currentLine < tokens.size()){
        if(currentLine < startLine){
            currentLine++;
            continue;
        }
        vector<string> line = tokens[currentLine];
        if(line[0] == "#"){
            currentLine++;
            continue;
        }
        TokenType type = ROOT;
        std::string value = line[0];
        Node* tmpNod1 = new Node;
        Node* tmpNod2 = new Node;
        Node* tmpNod3 = new Node;
        Node* child = new Node;

        if (value == "cvar") {
            type = CVAR;
            tmpNod1->type = VARIABLE;
            tmpNod1->value = makeVarEntry(line[1]);
            MathNode* expr = mathparse(line[2]);
            tmpNod2->type = EXPRESSION;
            tmpNod2->expression = expr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "clist") {
            type = CLIST;
            tmpNod1->type = LISTIDENT;
            tmpNod1->value = makeListEntry(line[1]);
            MathNode* expr = mathparse(line[2]);
            tmpNod2->type = EXPRESSION;
            tmpNod2->expression = expr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "mvar") {
            type = MVAR;
            tmpNod1->type = VARIABLES;
            string apStr;
            for(int i = 1; i < line.size() - 1; i++){
                if(i < line.size() - 2){
                    apStr.append(line[i]);
                    apStr.append(" ");
                }else{
                    apStr.append(line[i]);
                }
            }
            tmpNod1->value = makeMultipleVarEntries(apStr);
            MathNode* expr = mathparse(line[line.size() - 1]);
            tmpNod2->type = EXPRESSION;
            tmpNod2->expression = expr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "set") {
            type = SET;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            MathNode* expr = mathparse(line[2]);
            tmpNod2->type = EXPRESSION;
            tmpNod2->expression = expr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "input") {
            type = INPUT;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            tmpNod2->type = STRING;
            string* pStr = new string;
            for(int i = 2; i < line.size(); i++){
                if(i < line.size() - 1){
                    pStr->append(line[i] + " ");
                }else{
                    pStr->append(line[i]);
                }
            }
            tmpNod2->value = pStr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "loop") {
            type = LOOP;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            tmpNod2->type = EXEC;
            tmpNod2->children = parseTree(tokens,  currentLine + 1)->children;
            currentLine = rLine;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "funct") {
            type = FUNCT;
            tmpNod1->type = STRING;
            string* apStr = new string;
            *apStr = line[1];
            tmpNod1->value = apStr;
            tmpNod2->type = EXEC;
            tmpNod2->children = parseTree(tokens,  currentLine + 1)->children;
            currentLine = rLine;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "while") {
            type = WHILE;
            MathNode* expr = mathparse(line[1]);
            tmpNod1->type = EXPRESSION;
            tmpNod1->expression = expr;
            tmpNod2->type = EXEC;
            tmpNod2->children = parseTree(tokens, currentLine + 1)->children;
            currentLine = rLine;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "inc") {
            type = INC;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            child->children.push_back(tmpNod1);
        } else if (value == "dec") {
            type = DEC;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            child->children.push_back(tmpNod1);
        } else if (value == "elif") {
            type = ELIF;
            MathNode* expr = mathparse(line[1]);
            tmpNod1->type = EXPRESSION;
            tmpNod1->expression = expr;
            tmpNod2->type = EXEC;
            tmpNod2->children = parseTree(tokens, currentLine + 1)->children;
            currentLine = rLine;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "if") {
            type = IF;
            MathNode* expr = mathparse(line[1]);
            tmpNod1->type = EXPRESSION;
            tmpNod1->expression = expr;
            tmpNod2->type = EXEC;
            tmpNod2->children = parseTree(tokens, currentLine + 1)->children;
            currentLine = rLine;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "else") {
            type = ELSE;
            tmpNod1->type = EXEC;
            tmpNod1->children = parseTree(tokens, currentLine + 1)->children;
            currentLine = rLine;
            child->children.push_back(tmpNod1);
        } else if (value == "print") {
            type = PRINT;
            tmpNod1->type = STRING;
            string* apStr = new string;
            for(int i = 1; i < line.size(); i++){
                if(i < line.size() - 1){
                    apStr->append(line[i]);
                    apStr->append(" ");
                }else{
                    apStr->append(line[i]);
                }
            }
            tmpNod1->value = apStr;
            child->children.push_back(tmpNod1);
        } else if (value == "printb") {
            type = PRINTB;
            tmpNod1->type = STRING;
            string* apStr = new string;
            for(int i = 1; i < line.size(); i++){
                if(i < line.size() - 1){
                    apStr->append(line[i]);
                    apStr->append(" ");
                }else{
                    apStr->append(line[i]);
                }
            }
            tmpNod1->value = apStr;
            child->children.push_back(tmpNod1);
        } else if (value == "printv") {
            type = PRINTV;
            MathNode* expr = mathparse(line[1]);
            tmpNod1->type = EXPRESSION;
            tmpNod1->expression = expr;
            child->children.push_back(tmpNod1);
        } else if (value == "ceil") {
            type = CEIL;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            child->children.push_back(tmpNod1);
        } else if (value == "floor") {
            type = FLOOR;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            child->children.push_back(tmpNod1);
        } else if (value == "abs") {
            type = ABS;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            child->children.push_back(tmpNod1);
        } else if (value == "round") {
            type = ROUND;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            MathNode* expr = mathparse(line[2]);
            tmpNod2->type = EXPRESSION;
            tmpNod2->expression = expr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "newl") {
            type = NEWL;
        } else if (value == "exit") {
            type = EXIT;
        } else if (value == "leave") {
            type = LEAVE;
        } else if (value == "]") {
            rLine = currentLine;
            type = END;
            child->type = type;
            root->children.push_back(child);
            return root;
        } else if (value == "log") {
            type = LOG;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            MathNode* expr = mathparse(line[2]);
            tmpNod2->type = EXPRESSION;
            tmpNod2->expression = expr;
            MathNode* expr2 = mathparse(line[3]);
            tmpNod3->type = EXPRESSION;
            tmpNod3->expression = expr2;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
            child->children.push_back(tmpNod3);
        } else if (value == "sin") {
            type = SIN;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            tmpNod2->type = EXPRESSION;
            MathNode* expr = mathparse(line[2]);
            tmpNod2->expression = expr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "cos") {
            type = COS;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            tmpNod2->type = EXPRESSION;
            MathNode* expr = mathparse(line[2]);
            tmpNod2->expression = expr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "tan") {
            type = TAN;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            tmpNod2->type = EXPRESSION;
            MathNode* expr = mathparse(line[2]);
            tmpNod2->expression = expr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "sec") {
            type = SEC;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            tmpNod2->type = EXPRESSION;
            MathNode* expr = mathparse(line[2]);
            tmpNod2->expression = expr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "csc") {
            type = CSC;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            tmpNod2->type = EXPRESSION;
            MathNode* expr = mathparse(line[2]);
            tmpNod2->expression = expr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "sec") {
            type = SEC;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            tmpNod2->type = EXPRESSION;
            MathNode* expr = mathparse(line[2]);
            tmpNod2->expression = expr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "cot") {
            type = COT;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            tmpNod2->type = EXPRESSION;
            MathNode* expr = mathparse(line[2]);
            tmpNod2->expression = expr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "asin") {
            type = ASIN;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            tmpNod2->type = EXPRESSION;
            MathNode* expr = mathparse(line[2]);
            tmpNod2->expression = expr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "acos") {
            type = ACOS;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            tmpNod2->type = EXPRESSION;
            MathNode* expr = mathparse(line[2]);
            tmpNod2->expression = expr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "atan") {
            type = ATAN;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            tmpNod2->type = EXPRESSION;
            MathNode* expr = mathparse(line[2]);
            tmpNod2->expression = expr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        }else if (value == "xroot") {
            type = XROOT;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            tmpNod2->type = EXPRESSION;
            MathNode* expr = mathparse(line[2]);
            tmpNod2->expression = expr;
            MathNode* expr2 = mathparse(line[3]);
            tmpNod3->type = EXPRESSION;
            tmpNod3->expression = expr2;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
            child->children.push_back(tmpNod3);
        }else if (value == "random") {
            type = RANDOM;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            tmpNod2->type = EXPRESSION;
            MathNode* expr = mathparse(line[2]);
            tmpNod2->expression = expr;
            MathNode* expr2 = mathparse(line[3]);
            tmpNod3->type = EXPRESSION;
            tmpNod3->expression = expr2;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
            child->children.push_back(tmpNod3);
        } else if (value == "push") {
            type = PUSH;
            tmpNod1->type = EXPRESSION;
            MathNode* expr = mathparse(line[1]);
            tmpNod1->expression = expr;
            tmpNod2->type = LISTIDENT;
            tmpNod2->value = getListEntry(line[2]);
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "pop") {
            type = POP;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            tmpNod2->type = LISTIDENT;
            tmpNod2->value = getListEntry(line[2]);
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "chsl") {
            type = CHSL;
            tmpNod1->type = LISTIDENT;
            tmpNod1->value = getListEntry(line[1]);
            tmpNod2->type = EXPRESSION;
            MathNode* expr = mathparse(line[2]);
            tmpNod2->expression = expr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "getl") {
            type = GETL;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            tmpNod2->type = LISTIDENT;
            tmpNod2->value = getListEntry(line[2]);
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "readf") {
            type = READF;
            tmpNod1->type = LISTIDENT;
            tmpNod1->value = makeListEntry(line[1]);
            tmpNod2->type = STRING;
            string* apStr = new string;
            *apStr = line[2];
            tmpNod2->value = apStr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "writef") {
            type = WRITEF;
            tmpNod1->type = LISTIDENT;
            tmpNod1->value = getListEntry(line[1]);
            tmpNod2->type = STRING;
            string* apStr = new string;
            *apStr = line[2];
            tmpNod2->value = apStr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "ldef") {
            type = LDEF;
            tmpNod1->type = LISTIDENT;
            tmpNod1->value = makeListEntry(line[1]);
            tmpNod2->type = STRING;
            string* apStr = new string;
            *apStr = line[2];
            tmpNod2->value = apStr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "call") {
            type = CALL;
            tmpNod1->type = STRING;
            string* apStr = new string;
            *apStr = line[1];
            tmpNod1->value = apStr;
            child->children.push_back(tmpNod1);
        } else if (value == "sloop") {
            type = SLOOP;
            MathNode* expr = mathparse(line[1]);
            tmpNod1->type = EXPRESSION;
            tmpNod1->expression = expr;
            tmpNod2->type = EXEC;
            tmpNod2->children = parseTree(tokens, currentLine + 1)->children;
            currentLine = rLine;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        }

        child->type = type;
        root->children.push_back(child);
        currentLine++;
    }
    return root;
} // parse tokens and create tree

void printMathTree(MathNode* root, int level) {
    cout << " ";
    switch (root->type) {
        case MathNodeType::Variable:
            cout << "VAR <" << root->variable << ">" << endl;
            break;
        case MathNodeType::Constant:
            cout << "CONST <" << root->constant << ">" << endl;
            break;
        case MathNodeType::Operator:
            cout << "Operator <" << getOperator(root->opt) << ">" << endl;
            break;
        case MathNodeType::Array:
            cout << "ARR <" << root->variable << ">" << endl;
            break;
    }

    int i = 0;
    if (root->left != nullptr) {
        for (int j = 0; j < level; j++) {
            cout << "   ";
        }
        if(root->type == MathNodeType::Array){
            cout << "`--";
        }else{
            cout << "|--";
        }
        printMathTree(root->left, level + 1);
        i++;
    }
    if (root->right != nullptr) {
        for (int j = 0; j < level; j++) {
            cout << "   ";
        }
        cout << "`--";
        printMathTree(root->right, level + 1);
        i++;
    }
} // print the parsed expression subtrees

void printTree(Node* root, int level) {
    cout << " ";
    switch (root->type) {
        case SET:
            cout << "SET" << endl;
            break;
        case CVAR:
            cout << "CVAR" << endl;
            break;
        case MVAR:
            cout << "MVAR" << endl;
            break;
        case LOOP:
            cout << "LOOP" << endl;
            break;
        case WHILE:
            cout << "WHILE" << endl;
            break;
        case INC:
            cout << "INC" << endl;
            break;
        case DEC:
            cout << "DEC" << endl;
            break;
        case IF:
            cout << "IF" << endl;
            break;
        case ELIF:
            cout << "ELIF" << endl;
            break;
        case ELSE:
            cout << "ELSE" << endl;
            break;
        case PRINT:
            cout << "PRINT" << endl;
            break;
        case PRINTB:
            cout << "PRINTB" << endl;
            break;
        case PRINTV:
            cout << "PRINTV" << endl;
            break;
        case NEWL:
            cout << "NEWL" << endl;
            break;
        case VARIABLE:
            cout << "VARIABLE <" << root->value << ">" << endl;
            break;
        case VARIABLES:
            cout << "VARIABLES <" << root->value << ">" << endl;
            break;
        case VARLIST:
            cout << "VARLIST" << endl;
            for (int j = 0; j < level; j++) {
                cout << "   ";
            }
            cout << "`--";
            printMathTree(root->expression, level+1);
            break;
        case STRING:
            cout << "STRING" << endl;
            break;
        case ROOT:
            cout << "ROOT" << endl;
            break;
        case END:
            cout << "END" << endl;
            break;
        case EXEC:
            cout << "EXEC" << endl;
            break;
        case EXPRESSION:
            cout << "EXPRESSION" << endl;
            for (int j = 0; j < level; j++) {
                cout << "   ";
            }
            cout << "`--";
            printMathTree(root->expression, level+1);
            break;
        case CLIST:
            cout << "CLIST" << endl;
            break;
        case LDEF:
            cout << "LDEF" << endl;
            break;
        case READF:
            cout << "READF" << endl;
            break;
        case WRITEF:
            cout << "WRITEF" << endl;
            break;
        case LISTIDENT:
            cout << "LISTID" << endl;
            break;
        case INPUT:
            cout << "INPUT" << endl;
            break;
        case CEIL:
            cout << "CEIL" << endl;
            break;
        case FLOOR:
            cout << "FLOOR" << endl;
            break;
        case ROUND:
            cout << "ROUND" << endl;
            break;
        case EXIT:
            cout << "EXIT" << endl;
            break;
        case LEAVE:
            cout << "LEAVE" << endl;
            break;
        case SLOOP:
            cout << "SLOOP" << endl;
            break;
        case LOG:
            cout << "LOG" << endl;
            break;
        case EXP:
            cout << "EXP" << endl;
            break;
        case SIN:
            cout << "SIN" << endl;
            break;
        case COS:
            cout << "COS" << endl;
            break;
        case TAN:
            cout << "TAN" << endl;
            break;
        case CSC:
            cout << "SCS" << endl;
            break;
        case SEC:
            cout << "SEC" << endl;
            break;
        case COT:
            cout << "COT" << endl;
            break;
        case ATAN:
            cout << "ATAN" << endl;
            break;
        case ACOS:
            cout << "ACOS" << endl;
            break;
        case ASIN:
            cout << "ASIN" << endl;
            break;
        case GETL:
            cout << "GETL" << endl;
            break;
        case XROOT:
            cout << "XROOT" << endl;
            break;
        case FUNCT:
            cout << "FUNCTION" << endl;
            break;
        case CALL:
            cout << "CALL" << endl;
            break;
        case ABS:
            cout << "ABS" << endl;
            break;
        case POP:
            cout << "POP" << endl;
            break;
        case PUSH:
            cout << "PUSH" << endl;
            break;
        case CHSL:
            cout << "CHSL" << endl;
            break;
        case RANDOM:
            cout << "RANDOM" << endl;
            break;
    }

    int i = 0;
    for (auto& child : root->children) {
        for (int j = 0; j < level; j++) {
            cout << "   ";
        }
        if (i == root->children.size() - 1) {
            cout << "`--";
        } else {
            cout << "|--";
        }
        printTree(child, level + 1);
        i++;
    }
} // print the parsed tree

void printTreeRoot(Node* root){
    cout << "SquareBracket Parser (Version 2.1.0) -- © 2023 Patrick De Smet" << endl << endl;
    printTree(root, 0);
    cout << endl;
} // print logo and the parsed tree starting from the root


#endif //SQBRA_PARSER_H
