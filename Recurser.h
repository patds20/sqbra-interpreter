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
#include <iterator>
#include <random>
#include "Tokens.h"

// GLOBAL VARIABLES
using namespace std;
unordered_map<string, Node*> funcs; // global map for all functions
bool if_state = false; // was the previous if or elif false --> enables execution of next elif or else

int execute(Node* node);

// LOAD CONSTANTS ################################################################

inline void load_math_const(){
    auto predefined = new vector<long double>(7);
    (*predefined)[0] = 3.14159265358979323846264338327;
    (*predefined)[1] = 2.7182818284590452353602874713527;
    (*predefined)[2] = 1.414213562373095048801688724209698078569;
    (*predefined)[3] = 1.732050807568877293527446341505872366942;
    (*predefined)[4] = 0.57721566490153286060651209008240243104215933593992;
    (*predefined)[5] = 1.6180339887498948482045868343656381177203091798057;
    (*predefined)[6] = 1.202056903159594285399738161511449990764986292;

    vars["cpi"] = (*predefined).data();
    vars["ce"] = (*predefined).data() + 1;
    vars["csqr2"] = (*predefined).data() + 2;
    vars["csqr3"] = (*predefined).data() + 3;
    vars["cemas"] = (*predefined).data() + 4;
    vars["cproa"] = (*predefined).data() + 5;
    vars["capry"] = (*predefined).data() + 6;
}

// RESOLVE FUNCTIONS ##############################################################

inline void setVarListEntry(MathNode* varlist, long double value){
    if(varlist->type == MathNodeType::Variable){
        *(long double*)varlist->variable = value;

    }else{
        (*reinterpret_cast<vector<long double>*>(varlist->variable))[(int)(calculateExpression(varlist->left))] = value;
    }
}

inline long double getVarListEntry(MathNode* varlist){
    if(varlist->type == MathNodeType::Variable){
        return *(double*)varlist->variable;
    }else{
        return (*reinterpret_cast<vector<long double>*>(varlist->variable))[(int)(calculateExpression(varlist->left))];
    }
}

inline long double round_to(long double value, long double precision) {
    return round(value / precision) * precision;
}

// COMMAND FUNCTIONS ##############################################################

inline void command_cvar(long double* varname, MathNode* expression){
    *varname = calculateExpression(expression);
}

inline void command_clist(vector<long double>* listid, MathNode* expression){
    listid->resize((int)calculateExpression(expression));
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

inline void command_mvar(vector<long double*>* variables, MathNode* val){
    long double tmp_val = calculateExpression(val);
    for(long double* svars : *variables){
        *svars = tmp_val;
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

inline void command_getl(MathNode* varlist, vector<long double>* listid){
    setVarListEntry(varlist, (long double)(*reinterpret_cast<vector<long double>*>(listid)).size());
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
        funcs[identifier] = exec_block;
    }else{
        cerr << "Error: function <" << identifier << "> is aleady defined" << endl;
        exit(0); // terminate program
    }
}

inline void command_poplist(MathNode* varlist, vector<long double>* listid){
    setVarListEntry(varlist, listid->back());
    listid->pop_back();
}

inline void command_pushlist(MathNode* expr, vector<long double>* listid){
    long double val = calculateExpression(expr);
    listid->push_back(val);
}

inline void command_chsl(MathNode* expr, vector<long double>* listid){
    int new_l = (int)calculateExpression(expr);
    listid->resize(new_l);
}

inline void command_ldef(vector<long double>* listid, string* stri){
    string content = removeSpaces(extract_list_string(*stri));
    parse_numbers(content,listid);
}

inline void command_readf(vector<long double>* listid, string* filename){
    string filepath = extract_string(*filename);
    read_csv(filepath,listid);
}

inline void command_writef(vector<long double>* listid, string* filename){
    string filepath = extract_string(*filename);
    write_csv(filepath,listid);
}

inline void command_random(MathNode* varlist, MathNode* max, MathNode* min){
    std::random_device rd;
    std::seed_seq seed{ rd(), static_cast<uint32_t>(std::chrono::high_resolution_clock::now().time_since_epoch().count()) };
    std::mt19937_64 eng(seed);
    std::uniform_real_distribution<long double> distr(calculateExpression(min), calculateExpression(max));
    setVarListEntry(varlist,distr(eng));
}

// MAIN FUNCTIONS FOR THE EXECUTION OF COMMANDS

inline void executeCommand(Node* statement) {
    switch (statement->type) {
        case CVAR:
            command_cvar((long double*)statement->children[0]->value, statement->children[1]->expression);
            break;
        case SET:
            command_set(statement->children[0]->expression, statement->children[1]->expression);
            break;
        case PRINT:
            command_print(*(string*)statement->children[0]->value);
            break;
        case PRINTB:
            command_printb(*(string*)statement->children[0]->value);
            break;
        case PRINTV:
            command_printv(statement->children[0]->expression);
            break;
        case DEC:
            command_dec(statement->children[0]->expression);
            break;
        case INC:
            command_inc(statement->children[0]->expression);
            break;
        case CLIST:
            command_clist((vector<long double>*)statement->children[0]->value, statement->children[1]->expression);
            break;
        case INPUT:
            command_printb(*(string*)statement->children[1]->value);
            command_input(statement->children[0]->expression);
            break;
        case MVAR:
            command_mvar((vector<long double*>*)statement->children[0]->value, statement->children[1]->expression);
            break;
        case ROUND:
            command_round(statement->children[0]->expression, statement->children[1]->expression);
            break;
        case CEIL:
            command_ceil(statement->children[0]->expression);
            break;
        case FLOOR:
            command_floor(statement->children[0]->expression);
            break;
        case ABS:
            command_abs(statement->children[0]->expression);
            break;
        case SIN:
            command_trig(statement->children[0]->expression, statement->children[1]->expression, SIN);
            break;
        case COS:
            command_trig(statement->children[0]->expression, statement->children[1]->expression, COS);
            break;
        case TAN:
            command_trig(statement->children[0]->expression, statement->children[1]->expression, TAN);
            break;
        case CSC:
            command_trig(statement->children[0]->expression, statement->children[1]->expression, CSC);
            break;
        case SEC:
            command_trig(statement->children[0]->expression, statement->children[1]->expression, SEC);
            break;
        case COT:
            command_trig(statement->children[0]->expression, statement->children[1]->expression, COT);
            break;
        case ASIN:
            command_trig(statement->children[0]->expression, statement->children[1]->expression, ASIN);
            break;
        case ACOS:
            command_trig(statement->children[0]->expression, statement->children[1]->expression, ACOS);
            break;
        case ATAN:
            command_trig(statement->children[0]->expression, statement->children[1]->expression, ATAN);
            break;
        case GETL:
            command_getl(statement->children[0]->expression, (vector<long double>*)statement->children[1]->value);
            break;
        case XROOT:
            command_xroot(statement->children[0]->expression, statement->children[1]->expression, statement->children[2]->expression);
            break;
        case LOG:
            command_log(statement->children[0]->expression, statement->children[1]->expression, statement->children[2]->expression);
            break;
        case FUNCT:
            command_declarefunct(*(string*)statement->children[0]->value, statement->children[1]);
            break;
        case PUSH:
            command_pushlist(statement->children[0]->expression,(vector<long double>*)statement->children[1]->value);
            break;
        case POP:
            command_poplist(statement->children[0]->expression,(vector<long double>*)statement->children[1]->value);
            break;
        case CHSL:
            command_chsl(statement->children[1]->expression,(vector<long double>*)statement->children[0]->value);
            break;
        case NEWL:
            cout << endl;
            break;
        case READF:
            command_readf((vector<long double>*)statement->children[0]->value, (string*)statement->children[1]->value);
            break;
        case WRITEF:
            command_writef((vector<long double>*)statement->children[0]->value, (string*)statement->children[1]->value);
            break;
        case LDEF:
            command_ldef((vector<long double>*)statement->children[0]->value, (string*)statement->children[1]->value);
            break;
        case RANDOM:
            command_random(statement->children[0]->expression,statement->children[1]->expression,statement->children[2]->expression);
            break;
    }
}

int execute(Node* node){
    int loopc;
    switch (node->type) {
        case EXEC:
        case ROOT:
            for(Node* com : node->children){
                if(execute(com) == -1){ // execute all statements in ROOT or EXEC iteratively
                    return -1;
                }
            }
            break;
        case LOOP:
            while (getVarListEntry(node->children[0]->expression) > 0) {
                if (execute(node->children[1]) == -1) {
                    return -1;
                }
                setVarListEntry(node->children[0]->expression, getVarListEntry(node->children[0]->expression)-1);
            }
            break;
        case SLOOP:
            loopc = (int)calculateExpression(node->children[0]->expression);
            for(; loopc > 0; loopc--){
                if (execute(node->children[1]) == -1) {
                    return -1;
                }
            }
            break;
        case IF:
            if(calculateExpression(node->children[0]->expression) > 0){
                if_state = false;
                if (execute(node->children[1]) == -1) {
                    return -1;
                }
            }else{
                if_state = true;
            }
            break;
        case ELIF:
            if(if_state){
                if(calculateExpression(node->children[0]->expression) > 0){
                    if_state = false;
                    if (execute(node->children[1]) == -1) {
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
                if (execute(node->children[0]) == -1) {
                    return -1;
                }
            }
            break;
        case WHILE:
            while(calculateExpression(node->children[0]->expression) > 0){
                if (execute(node->children[1]) == -1) {
                    return -1;
                }
            }
            break;
        case CALL:
            if(funcs.find(*((string*)node->children[0]->value)) == funcs.end()){
                cerr << "Error: function <" << node->children[0]->value << "> is not defined" << endl;
                exit(0);
            }
            execute(funcs[*(string*)(node->children[0]->value)]);
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
