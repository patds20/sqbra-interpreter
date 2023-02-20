#pragma once
#ifndef SQBRA_PARSER_H
#define SQBRA_PARSER_H

/*
 *  PARSER.H
 *  This file contains the parser and print functions for the SquareBracket language.
 *  The parser analyzes the tokens and creates a distinct tree which can be executed in the next step.
 *  All rights belong to Patrick De Smet (Copyright 2023).
 */

#include <string>
#include <vector>
#include <iostream>
#include "Tokens.h"

using namespace std;

int rLine = -1; // this variable tells the parser where to continue after an indented block

Node parseTree(const std::vector<std::vector<std::string> >& tokens, int startLine) {
    Node root;
    root.type = ROOT;
    std::vector<Node> stack;
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
        Node tmpNod1;
        Node tmpNod2;
        Node child;

        if (value == "cvar") {
            type = CVAR;
            tmpNod1.type = VARIABLE;
            tmpNod1.value = line[1];
            tmpNod2.type = VALUE;
            tmpNod2.value = line[2];
            child.children.push_back(tmpNod1);
            child.children.push_back(tmpNod2);
        } else if (value == "clist") {
            type = CLIST;
            tmpNod1.type = LISTIDENT;
            tmpNod1.value = line[1];
            tmpNod2.type = VALUE;
            tmpNod2.value = line[2];
            child.children.push_back(tmpNod1);
            child.children.push_back(tmpNod2);
        } else if (value == "mvar") {
            type = MVAR;
            tmpNod1.type = VARIABLELIST;
            string apStr;
            for(int i = 1; i < line.size() - 1; i++){
                if(i < line.size() - 2){
                    apStr.append(line[i]);
                    apStr.append(" ");
                }else{
                    apStr.append(line[i]);
                }
            }
            tmpNod1.value = apStr;
            tmpNod2.type = VALUE;
            tmpNod2.value = line[line.size() - 1];
            child.children.push_back(tmpNod1);
            child.children.push_back(tmpNod2);
        } else if (value == "set") {
            type = SET;
            tmpNod1.type = VARIABLE;
            tmpNod1.value = line[1];
            tmpNod2.type = VALUE;
            tmpNod2.value = line[2];
            child.children.push_back(tmpNod1);
            child.children.push_back(tmpNod2);
        } else if (value == "input") {
            type = INPUT;
            tmpNod1.type = VARIABLE;
            tmpNod1.value = line[1];
            tmpNod2.type = STRING;
            string pStr;
            for(int i = 2; i < line.size(); i++){
                if(i < line.size() - 1){
                    pStr.append(line[i] + " ");
                }else{
                    pStr.append(line[i]);
                }
            }
            tmpNod2.value = pStr;
            child.children.push_back(tmpNod1);
            child.children.push_back(tmpNod2);
        } else if (value == "loop") {
            type = LOOP;
            tmpNod1.type = VARIABLE;
            tmpNod1.value = line[1];
            tmpNod2.type = EXEC;
            tmpNod2.children = parseTree(tokens,  currentLine + 1).children;
            currentLine = rLine;
            child.children.push_back(tmpNod1);
            child.children.push_back(tmpNod2);
        } else if (value == "funct") {
            type = FUNCT;
            tmpNod1.type = STRING;
            tmpNod1.value = line[1];
            tmpNod2.type = EXEC;
            tmpNod2.children = parseTree(tokens,  currentLine + 1).children;
            currentLine = rLine;
            child.children.push_back(tmpNod1);
            child.children.push_back(tmpNod2);
        } else if (value == "while") {
            type = WHILE;
            tmpNod1.type = STATEMENT;
            tmpNod1.value = line[1];
            tmpNod2.type = EXEC;
            tmpNod2.children = parseTree(tokens, currentLine + 1).children;
            currentLine = rLine;
            child.children.push_back(tmpNod1);
            child.children.push_back(tmpNod2);
        } else if (value == "inc") {
            type = INC;
            tmpNod1.type = VARIABLE;
            tmpNod1.value = line[1];
            child.children.push_back(tmpNod1);
        } else if (value == "dec") {
            type = DEC;
            tmpNod1.type = VARIABLE;
            tmpNod1.value = line[1];
            child.children.push_back(tmpNod1);
        } else if (value == "if") {
            type = IF;
            tmpNod1.type = STATEMENT;
            tmpNod1.value = line[1];
            tmpNod2.type = EXEC;
            tmpNod2.children = parseTree(tokens, currentLine + 1).children;
            currentLine = rLine;
            child.children.push_back(tmpNod1);
            child.children.push_back(tmpNod2);
        } else if (value == "print") {
            type = PRINT;
            tmpNod1.type = STRING;
            string apStr;
            for(int i = 1; i < line.size(); i++){
                if(i < line.size() - 1){
                    apStr.append(line[i]);
                    apStr.append(" ");
                }else{
                    apStr.append(line[i]);
                }
            }
            tmpNod1.value = apStr;
            child.children.push_back(tmpNod1);
        } else if (value == "printb") {
            type = PRINTB;
            tmpNod1.type = STRING;
            string apStr;
            for(int i = 1; i < line.size(); i++){
                if(i < line.size() - 1){
                    apStr.append(line[i]);
                    apStr.append(" ");
                }else{
                    apStr.append(line[i]);
                }
            }
            tmpNod1.value = apStr;
            child.children.push_back(tmpNod1);
        } else if (value == "printv") {
            type = PRINTV;
            tmpNod1.type = EXPRESSION;
            tmpNod1.value = line[1];
            child.children.push_back(tmpNod1);
        } else if (value == "ceil") {
            type = CEIL;
            tmpNod1.type = VARIABLE;
            tmpNod1.value = line[1];
            child.children.push_back(tmpNod1);
        } else if (value == "floor") {
            type = FLOOR;
            tmpNod1.type = VARIABLE;
            tmpNod1.value = line[1];
            child.children.push_back(tmpNod1);
        } else if (value == "round") {
            type = ROUND;
            tmpNod1.type = VARIABLE;
            tmpNod1.value = line[1];
            tmpNod2.type = VARIABLE;
            tmpNod2.value = line[2];
            child.children.push_back(tmpNod1);
            child.children.push_back(tmpNod2);
        } else if (value == "newl") {
            type = NEWL;
        } else if (value == "exit") {
            type = EXIT;
        } else if (value == "leave") {
            type = LEAVE;
        } else if (value == "]") {
            rLine = currentLine;
            type = END;
            child.type = type;
            root.children.push_back(child);
            return root;
        } else if (value == "log") {
            type = LOG;
            tmpNod1.type = VARIABLE;
            tmpNod1.value = line[1];
            tmpNod2.type = VARIABLE;
            tmpNod2.value = line[2];
            Node tmpNod3;
            tmpNod3.value = line[3];
            tmpNod2.children.push_back(tmpNod3);
            child.children.push_back(tmpNod1);
            child.children.push_back(tmpNod2);
        } else if (value == "exp") {
            type = EXP;
            tmpNod1.type = VARIABLE;
            tmpNod1.value = line[1];
            tmpNod2.type = VARIABLE;
            tmpNod2.value = line[2];
            child.children.push_back(tmpNod1);
            child.children.push_back(tmpNod2);
        } else if (value == "sin") {
            type = SIN;
            tmpNod1.type = VARIABLE;
            tmpNod1.value = line[1];
            tmpNod2.type = VARIABLE;
            tmpNod2.value = line[2];
            child.children.push_back(tmpNod1);
            child.children.push_back(tmpNod2);
        } else if (value == "cos") {
            type = COS;
            tmpNod1.type = VARIABLE;
            tmpNod1.value = line[1];
            tmpNod2.type = VARIABLE;
            tmpNod2.value = line[2];
            child.children.push_back(tmpNod1);
            child.children.push_back(tmpNod2);
        } else if (value == "tan") {
            type = TAN;
            tmpNod1.type = VARIABLE;
            tmpNod1.value = line[1];
            tmpNod2.type = VARIABLE;
            tmpNod2.value = line[2];
            child.children.push_back(tmpNod1);
            child.children.push_back(tmpNod2);
        } else if (value == "sec") {
            type = SEC;
            tmpNod1.type = VARIABLE;
            tmpNod1.value = line[1];
            tmpNod2.type = VARIABLE;
            tmpNod2.value = line[2];
            child.children.push_back(tmpNod1);
            child.children.push_back(tmpNod2);
        } else if (value == "csc") {
            type = CSC;
            tmpNod1.type = VARIABLE;
            tmpNod1.value = line[1];
            tmpNod2.type = VARIABLE;
            tmpNod2.value = line[2];
            child.children.push_back(tmpNod1);
            child.children.push_back(tmpNod2);
        } else if (value == "sec") {
            type = SEC;
            tmpNod1.type = VARIABLE;
            tmpNod1.value = line[1];
            tmpNod2.type = VARIABLE;
            tmpNod2.value = line[2];
            child.children.push_back(tmpNod1);
            child.children.push_back(tmpNod2);
        } else if (value == "cot") {
            type = COT;
            tmpNod1.type = VARIABLE;
            tmpNod1.value = line[1];
            tmpNod2.type = VARIABLE;
            tmpNod2.value = line[2];
            child.children.push_back(tmpNod1);
            child.children.push_back(tmpNod2);
        } else if (value == "asin") {
            type = ASIN;
            tmpNod1.type = VARIABLE;
            tmpNod1.value = line[1];
            tmpNod2.type = VARIABLE;
            tmpNod2.value = line[2];
            child.children.push_back(tmpNod1);
            child.children.push_back(tmpNod2);
        } else if (value == "acos") {
            type = ACOS;
            tmpNod1.type = VARIABLE;
            tmpNod1.value = line[1];
            tmpNod2.type = VARIABLE;
            tmpNod2.value = line[2];
            child.children.push_back(tmpNod1);
            child.children.push_back(tmpNod2);
        } else if (value == "atan") {
            type = ATAN;
            tmpNod1.type = VARIABLE;
            tmpNod1.value = line[1];
            tmpNod2.type = VARIABLE;
            tmpNod2.value = line[2];
            child.children.push_back(tmpNod1);
            child.children.push_back(tmpNod2);
        }else if (value == "xroot") {
            type = XROOT;
            tmpNod1.type = VARIABLE;
            tmpNod1.value = line[1];
            tmpNod2.type = VARIABLE;
            tmpNod2.value = line[2];
            child.children.push_back(tmpNod1);
            child.children.push_back(tmpNod2);
        } else if (value == "getl") {
            type = GETL;
            tmpNod1.type = VARIABLE;
            tmpNod1.value = line[1];
            tmpNod2.type = LISTIDENT;
            tmpNod2.value = line[2];
            child.children.push_back(tmpNod1);
            child.children.push_back(tmpNod2);
        } else if (value == "call") {
            type = CALL;
            tmpNod1.type = STRING;
            tmpNod1.value = line[1];
            child.children.push_back(tmpNod1);
        } else if (value == "sloop") {
            type = SLOOP;
            tmpNod1.type = VARIABLE;
            tmpNod1.value = line[1];
            tmpNod2.type = EXEC;
            tmpNod2.children = parseTree(tokens, currentLine + 1).children;
            currentLine = rLine;
            child.children.push_back(tmpNod1);
            child.children.push_back(tmpNod2);
        }

        child.type = type;
        root.children.push_back(child);
        currentLine++;
    }
    return root;
} // parse tokens and create tree

void printTree(Node* root, int level) {
    for (int i = 0; i < level; i++) {
        cout << "  ";
    }

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
        case VARIABLELIST:
            cout << "VARIABLES <" << root->value << ">" << endl;
            break;
        case VALUE:
            cout << "VALUE <" << root->value << ">" << endl;
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
        case STATEMENT:
            cout << "STATEMENT <" << root->value << ">" << endl;
            break;
        case EXEC:
            cout << "EXEC" << endl;
            break;
        case EXPRESSION:
            cout << "EXPRESSION" << endl;
            break;
        case CLIST:
            cout << "CLIST" << endl;
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
    }

    int i = 0;
    for (auto& child : root->children) {
        for (int j = 0; j < level; j++) {
            cout << "  ";
        }
        if (i == root->children.size() - 1) {
            cout << "`--";
        } else {
            cout << "|--";
        }
        printTree(&child, level + 1);
        i++;
    }
} // print the parsed tree

void printTreeRoot(Node* root){
    cout << "SquareBracket Parser (Version 1.3) -- © 2023 Patrick De Smet ############" << endl << endl;
    printTree(root, 0);
    cout << endl;
} // print logo and the parsed tree starting from the root


#endif //SQBRA_PARSER_H
