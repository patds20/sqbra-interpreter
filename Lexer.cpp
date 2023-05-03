#pragma once
#ifndef SQBRA_LEXER_H
#define SQBRA_LEXER_H

/*
 *  LEXER.CPP
 *  This file contains the lexer and repair (fix) functions for the SquareBracket language.
 *  The lexer reads the raw code, tokenizes it and repairs small mistakes.
 *  Copyright (c) 2023, Patrick De Smet
 */

#include <vector>
#include <string>
#include <sstream>

using namespace std;

#include <string>

vector<vector<string> > lexer(const string& input) {
    vector<vector<string> > result;
    string line;
    stringstream ss(input);
    while (getline(ss, line)) {
        if (line.empty()) {
            continue;
        }
        vector<string> tokens;
        string token;
        int bracket_count = 0;
        bool in_sqbra = false;
        for (char c : line) {
            if (c == ' ' || c == '\t') {
                if (!token.empty() && bracket_count == 0) {
                    if(!in_sqbra){
                        tokens.push_back(token);
                        token = "";
                    }else{
                        token += c;
                    }
                }
            } else if (c == '(') {
                bracket_count++;
                token += c;
            } else if (c == ')') {
                bracket_count--;
                token += c;
                if (bracket_count == 0) {
                    tokens.push_back(token);
                    token = "";
                }
            } else if (c == '\r') {
                cerr << "Error: SQBRA does not accept CRLF line endings." << endl;
                exit(0);
            } else if (c == '[') {
                in_sqbra = true;
                token += c;
            } else if (c == ']') {
                in_sqbra = false;
                token += c;
            } else {
                token += c;
            }
        }
        if (!token.empty()) {
            tokens.push_back(token);
        }
        result.push_back(tokens);
    }
    return result;
} // splits the code and creates a vector for each line with tokens

void fix_while_syntax(std::vector<std::string> *if_strings) {
    if(if_strings[0][0] != "while"){
        string pStr = if_strings[0][0].substr (5);
        if_strings->erase(if_strings->begin());
        if_strings->insert(if_strings->begin(), pStr);
        if_strings->insert(if_strings->begin(), "while");
    }
}  // fix while syntax

void fix_if_syntax(std::vector<std::string> *if_strings) {
    if(if_strings[0][0] != "if"){
        string pStr = if_strings[0][0].substr (2);
        if_strings->erase(if_strings->begin());
        if_strings->insert(if_strings->begin(), pStr);
        if_strings->insert(if_strings->begin(), "if");
    }
} // fix if syntax

void fix_elif_syntax(std::vector<std::string> *elif_strings) {
    if(elif_strings[0][0] != "elif"){
        string pStr = elif_strings[0][0].substr (4);
        elif_strings->erase(elif_strings->begin());
        elif_strings->insert(elif_strings->begin(), pStr);
        elif_strings->insert(elif_strings->begin(), "elif");
    }
} // fix elif syntax

void fix_else_syntax(std::vector<std::string> *else_strings) {
    if(else_strings[0][0] != "else"){
        string pStr = else_strings[0][0].substr (4);
        else_strings->erase(else_strings->begin());
        else_strings->insert(else_strings->begin(), pStr);
        else_strings->insert(else_strings->begin(), "else");
    }
} // fix else syntax

void fix_printb_syntax(std::vector<std::string> *print_strings) {
    if(print_strings[0][0] != "printb"){
        string pStr = print_strings[0][0].substr (6);
        print_strings->erase(print_strings->begin());
        print_strings->insert(print_strings->begin(), pStr);
        print_strings->insert(print_strings->begin(), "printb");
    }
} // fix printb syntax

void fix_print_syntax(std::vector<std::string> *print_strings) {
    if(print_strings[0][0] != "print"){
        string pStr = print_strings[0][0].substr (5);
        print_strings->erase(print_strings->begin());
        print_strings->insert(print_strings->begin(), pStr);
        print_strings->insert(print_strings->begin(), "print");
    }
} // fix print syntax

void repairTokens(std::vector<std::vector<std::string> > *strings) {
    int tokenIndex = 0;
    for (auto &vec : *strings) {
        tokenIndex = 0;
        for (auto &str : vec) {
            if(str.find("[") != std::string::npos){
                break;
            }
            for (char & i : str) {
                if (isupper(i)) {
                    i = (char)tolower(i);
                }
            }
        }
        if(vec[0].find("printb") != std::string::npos){
            fix_printb_syntax(&vec);
        }else if(vec[0].find("print") != std::string::npos && vec[0].find("printv") == std::string::npos && vec[0].find("printm") == std::string::npos){
            fix_print_syntax(&vec);
        }else if(vec[0].find("elif") != std::string::npos){
            fix_elif_syntax(&vec);
        }else if(vec[0].find("else") != std::string::npos){
            fix_else_syntax(&vec);
        }else if(vec[0].find("if") != std::string::npos){
            fix_if_syntax(&vec);
        }else if(vec[0].find("while") != std::string::npos){
            fix_while_syntax(&vec);
        }
    }// turn all alpha chars to lowercase
} // prepare tokens for parsing

#endif //SQBRA_LEXER_H
