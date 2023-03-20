#pragma once
#ifndef SQBRA_RECURSER_H
#define SQBRA_RECURSER_H

/*
 *  RECURSER.H
 *  This file contains the execute function for the SquareBracket language.
 *  The function executes the parsed tree recursively and calls the specific command functions.
 *  Copyright (c) 2023, Patrick De Smet
 */

#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <stack>
#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>
#include "Tokens.h"
#include <iterator>

// GLOBAL VARIABLES
using namespace std;
unordered_map<string, Node> funcs; // global map for all functions
bool if_state = false; // was the previous if or elif false --> enables execution of next elif or else

int execute(Node* node);

// MATH FUNCTIONS ################################################################

inline void load_math_const(){
    vars["cpi"] = 3.14159265358979323846264338327;
    vars["ce"] = 2.7182818284590452353602874713527;
    vars["csqr2"] = 1.414213562373095048801688724209698078569;
    vars["csqr3"] = 1.732050807568877293527446341505872366942;
    vars["cemas"] = 0.57721566490153286060651209008240243104215933593992;
    vars["cproa"] = 1.6180339887498948482045868343656381177203091798057;
    vars["capry"] = 1.202056903159594285399738161511449990764986292;
}

// RESOLVE FUNCTIONS ##############################################################

inline long double getListValue(const string& lid);

inline long double stringToFloat(const std::string& str) {
    std::istringstream ss(str);
    long double result;
    ss >> result;
    return result;
}

inline void setVarListEntry(MathNode* varlist, long double value){
    if(varlist->type == MathNodeType::Variable){
        if(vars.find(varlist->variable) != vars.end()){
            vars[varlist->variable] = value;
        }else{
            cerr << "Error: variable <" << varlist->variable << "> does not exist" << endl;
            exit(0); // terminate program
        }
    }else{
        if(ars.find(varlist->variable) != ars.end()){
            ars[varlist->variable][calculateExpression(varlist->left)] = value;
        }else{
            cerr << "Error: list <" << varlist->variable << "> does not exist" << endl;
            exit(0); // terminate program
        }
    }
}

inline long double getVarListEntry(MathNode* varlist){
    if(varlist->type == MathNodeType::Variable){
        if(vars.find(varlist->variable) != vars.end()){
            return vars[varlist->variable];
        }else{
            cerr << "Error: variable <" << varlist->variable << "> does not exist" << endl;
            exit(0); // terminate program
        }
    }else{
        if(ars.find(varlist->variable) != ars.end()){
            return ars[varlist->variable][calculateExpression(varlist->left)];
        }else{
            cerr << "Error: list <" << varlist->variable << "> does not exist" << endl;
            exit(0); // terminate program
        }
    }
}

inline string extract_string(const string& input) {
    std::size_t start_pos = input.find("[\"");
    std::size_t end_pos = input.find("\"]");
    if (start_pos == std::string::npos || end_pos == std::string::npos || start_pos >= end_pos) {
        cerr << "Error: <" << input << "> string is not in the correct format";
        exit(0);
    }
    return input.substr(start_pos + 2, end_pos - start_pos - 2);
}

inline long double round_to(long double value, long double precision) {
    return round(value / precision) * precision;
}

// COMMAND FUNCTIONS ##############################################################

inline void command_cvar(const string& varname, MathNode* expression){
    if (vars.find(varname) == vars.end()) {
        vars[varname] = calculateExpression(expression); // there is no current entry -> resolve value and insert
    } else {
        cerr << "Error: variable <" << varname << "> does already exist" << endl;
        exit(0); // terminate program
    }
}

inline void command_clist(const string& listid, MathNode* expression){
    if (ars.find(listid) == ars.end()) {
        ars[listid] = vector<long double>((int)calculateExpression(expression)); // there is no current entry -> resolve value and create list
    } else {
        cerr << "Error: list <" << listid << "> does already exist" << endl;
        exit(0); // terminate program
    }
}

inline void command_set(MathNode* varlist, MathNode* expression){
    setVarListEntry(varlist,calculateExpression(expression));
}

inline void command_ceil(MathNode* varlist){
    setVarListEntry(varlist, ceil(getVarListEntry(varlist)));
}

inline void command_floor(MathNode* varlist){
    setVarListEntry(varlist, floor(getVarListEntry(varlist)));
}

inline void command_abs(MathNode* varlist){
    setVarListEntry(varlist, abs(getVarListEntry(varlist)));
}

inline void command_round(MathNode* varlist, MathNode* precision){
    long double prec = calculateExpression(precision);
    setVarListEntry(varlist, round_to(getVarListEntry(varlist),prec));
}

inline void command_print(const string& stri){
    string str = extract_string(stri);
    cout << str << endl;
}

inline void command_printb(const string& stri){
    string str = extract_string(stri);
    cout << str;
}

inline void command_printv(MathNode* expression) {
    cout << calculateExpression(expression); // print output of processed expression
}

inline void command_inc(MathNode* varlist) {
    setVarListEntry(varlist, getVarListEntry(varlist) + 1);
}

inline void command_dec(MathNode* varlist) {
    setVarListEntry(varlist, getVarListEntry(varlist) - 1);
}

inline void command_input(MathNode* varlist) {
    long double inputval;
    cin >> inputval;
    setVarListEntry(varlist, inputval);
}

inline void command_mvar(const string& vs, MathNode* val){
    std::stringstream ss(vs);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> tokens(begin, end);

    for (auto &s: tokens) {
        command_cvar(s,val);
    }
}

inline void command_trig(MathNode* varlist, MathNode* source, TokenType func){
    long double inpVal = calculateExpression(source); // get x
    long double outVal;
    if(func == SIN){
        outVal = sin(inpVal);
    }else if(func == COS){
        outVal = cos(inpVal);
    }else if(func == TAN){
        outVal = tan(inpVal);
    }else if(func == CSC){
        outVal = 1/(sin(inpVal));
    }else if(func == SEC){
        outVal = 1/(cos(inpVal));
    }else if(func == COT){
        outVal = (cos(inpVal)/sin(inpVal));
    }else if(func == ASIN){
        if(inpVal > 1 || inpVal < -1){
            cerr << "Error: <" << varlist->variable << "> asin only accepts values from 1 to -1" << endl;
            exit(0); // terminate program
        }
        outVal = asin(inpVal);
    }else if(func == ACOS){
        if(inpVal > 1 || inpVal < -1){
            cerr << "Error: <" << varlist->variable << "> acos only accepts values from 1 to -1" << endl;
            exit(0); // terminate program
        }
        outVal = acos(inpVal);
    }else if(func == ATAN){
        if(inpVal > 1 || inpVal < -1){
            cerr << "Error: <" << varlist->variable << "> atan only accepts values from 1 to -1" << endl;
            exit(0); // terminate program
        }
        outVal = atan(inpVal);
    }
    setVarListEntry(varlist, outVal);
}

inline void command_getl(MathNode* varlist, const string& listid){
    if (ars.find(listid) == ars.end()) {
        cerr << "Error: list <" << listid << "> does not exist" << endl;
        exit(0); // terminate program
    } else {
        int len = ars[listid].size();
        setVarListEntry(varlist, len);
    }
}

inline void command_xroot(MathNode* varlist, MathNode* source, MathNode* root){
    long double inpVal = calculateExpression(source);
    long double roottype = calculateExpression(root);
    long double outVal = pow(inpVal, 1.0/roottype);
    setVarListEntry(varlist, outVal);
}

inline void command_log(MathNode* varlist, MathNode* source, MathNode* exp){ //HERE
    long double inpVal = calculateExpression(source);
    long double expVal = calculateExpression(exp);
    long double outVal = (log10(inpVal)/log10(expVal));
    setVarListEntry(varlist, outVal);
}

inline void command_declarefunct(const string& identifier, Node* exec_block){
    if(funcs.find(identifier) == funcs.end()){
        funcs[identifier] = *exec_block;
    }else{
        cerr << "Error: function <" << identifier << "> is aleady defined" << endl;
        exit(0); // terminate program
    }
}

inline void command_poplist(MathNode* varlist, const string& listid){
    if (ars.find(listid) == ars.end()) {
        cerr << "Error: list <" << listid << "> does not exist" << endl;
        exit(0); // terminate program
    } else {
        if (!ars[listid].empty()){
            long double pop_val = ars[listid].back();
            ars[listid].pop_back();
            setVarListEntry(varlist,pop_val);
        } else {
            cerr << "Error: list <" << listid << "> is empty" << endl;
            exit(0); // terminate program
        }
    }
}

inline void command_pushlist(MathNode* expr, const string& listid){
    if (ars.find(listid) == ars.end()) {
        cerr << "Error: list <" << listid << "> does not exist" << endl;
        exit(0); // terminate program
    } else {
        long double val = calculateExpression(expr);
        ars[listid].push_back(val);
    }
}

inline void command_chsl(MathNode* expr, const string& listid){
    if (ars.find(listid) == ars.end()) {
        cerr << "Error: list <" << listid << "> does not exist" << endl;
        exit(0); // terminate program
    } else {
        int new_l = (int)calculateExpression(expr);
        int cur_l = ars[listid].size();
        if (cur_l > new_l) {
            for (int dif_l = cur_l - new_l; dif_l > 0; dif_l--) {
                ars[listid].pop_back();
            }
        }else if (cur_l < new_l) {
            for (int dif_l = new_l - cur_l; dif_l > 0; dif_l--) {
                ars[listid].push_back(0);
            }
        }
    }
}

// MAIN FUNCTIONS FOR THE EXECUTION OF COMMANDS

inline void executeCommand(Node* statement) {
    switch (statement->type) {
        case CVAR:
            command_cvar(statement->children[0].value, statement->children[1].expression);
            break;
        case SET:
            command_set(statement->children[0].expression, statement->children[1].expression);
            break;
        case PRINT:
            command_print(statement->children[0].value);
            break;
        case PRINTB:
            command_printb(statement->children[0].value);
            break;
        case PRINTV:
            command_printv(statement->children[0].expression);
            break;
        case DEC:
            command_dec(statement->children[0].expression);
            break;
        case INC:
            command_inc(statement->children[0].expression);
            break;
        case CLIST:
            command_clist(statement->children[0].value, statement->children[1].expression);
            break;
        case INPUT:
            command_printb(statement->children[1].value);
            command_input(statement->children[0].expression);
            break;
        case MVAR:
            command_mvar(statement->children[0].value, statement->children[1].expression);
            break;
        case ROUND:
            command_round(statement->children[0].expression, statement->children[1].expression);
            break;
        case CEIL:
            command_ceil(statement->children[0].expression);
            break;
        case FLOOR:
            command_floor(statement->children[0].expression);
            break;
        case ABS:
            command_abs(statement->children[0].expression);
            break;
        case SIN:
            command_trig(statement->children[0].expression, statement->children[1].expression, SIN);
            break;
        case COS:
            command_trig(statement->children[0].expression, statement->children[1].expression, COS);
            break;
        case TAN:
            command_trig(statement->children[0].expression, statement->children[1].expression, TAN);
            break;
        case CSC:
            command_trig(statement->children[0].expression, statement->children[1].expression, CSC);
            break;
        case SEC:
            command_trig(statement->children[0].expression, statement->children[1].expression, SEC);
            break;
        case COT:
            command_trig(statement->children[0].expression, statement->children[1].expression, COT);
            break;
        case ASIN:
            command_trig(statement->children[0].expression, statement->children[1].expression, ASIN);
            break;
        case ACOS:
            command_trig(statement->children[0].expression, statement->children[1].expression, ACOS);
            break;
        case ATAN:
            command_trig(statement->children[0].expression, statement->children[1].expression, ATAN);
            break;
        case GETL:
            command_getl(statement->children[0].expression, statement->children[1].value);
            break;
        case XROOT:
            command_xroot(statement->children[0].expression, statement->children[1].expression, statement->children[2].expression);
            break;
        case LOG:
            command_log(statement->children[0].expression, statement->children[1].expression, statement->children[2].expression);
            break;
        case FUNCT:
            command_declarefunct(statement->children[0].value, &statement->children[1]);
            break;
        case PUSH:
            command_pushlist(statement->children[0].expression,statement->children[1].value);
            break;
        case POP:
            command_poplist(statement->children[0].expression,statement->children[1].value);
            break;
        case CHSL:
            command_chsl(statement->children[1].expression,statement->children[0].value);
            break;
        case NEWL:
            cout << endl;
            break;
    }
}

int execute(Node* node){
    int loopc;
    switch (node->type) {
        case EXEC:
        case ROOT:
            for(Node com : node->children){
                if(execute(&com) == -1){ // execute all statements in ROOT or EXEC iteratively
                    return -1;
                }
            }
            break;
        case LOOP:
            while (getVarListEntry(node->children[0].expression) > 0) {
                if (execute(&(node->children[1])) == -1) {
                    return -1;
                }
                setVarListEntry(node->children[0].expression, getVarListEntry(node->children[0].expression)-1);
            }
            break;
        case SLOOP:
            loopc = (int)calculateExpression(node->children[0].expression);
            for(;loopc > 0; loopc--){
                if (execute(&(node->children[1])) == -1) {
                    return -1;
                }
                loopc -= 1;
            }
            break;
        case IF:
            if(calculateExpression(node->children[0].expression) > 0){
                if_state = false;
                if (execute(&(node->children[1])) == -1) {
                    return -1;
                }
            }else{
                if_state = true;
            }
            break;
        case ELIF:
            if(if_state){
                if(calculateExpression(node->children[0].expression) > 0){
                    if_state = false;
                    if (execute(&(node->children[1])) == -1) {
                        return -1;
                    }
                }else{
                    if_state = true;
                }
            }
            break;
        case ELSE:
            if(if_state){
                if_state = false;
                if (execute(&(node->children[0])) == -1) {
                    return -1;
                }
            }
            break;
        case WHILE:
            while(calculateExpression(node->children[0].expression) > 0){
                if (execute(&(node->children[1])) == -1) {
                    return -1;
                }
            }
            break;
        case CALL:
            if(funcs.find(node->children[0].value) == funcs.end()){
                cerr << "Error: function <" << node->children[0].value << "> is not defined" << endl;
                exit(0);
            }
            execute(&funcs[node->children[0].value]);
            break;
        case EXIT:
            exit(0);
        case LEAVE:
            return -1;
        default:
            // handle standard commands
            executeCommand(node);
            break;
    }
    return 0;
}



#endif //SQBRA_RECURSER_H
