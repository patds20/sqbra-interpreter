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

// DEFINED CONSTANTS ################################################################

//! Predefined Constants
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

// AUXILIARY FUNCTIONS ##############################################################

//! Set the Value of a Variable, List or Matrix MathNode
inline void setVarListEntry(MathNode* varlist, long double value){
    if(varlist->type == MathNodeType::Variable){
        *(long double*)varlist->variable = value;
    }else if(varlist->type == MathNodeType::Array){
        auto list = reinterpret_cast<vector<long double>*>(varlist->variable);
        auto i = (int)(calculateExpression(varlist->left));
        if(list->size() > i){
            (*list)[i] = value;
        }else{
            cerr << "Error: Index out of bounds for list <" << varlist->variable << "> at index <" << i << ">." << endl;
            exit(0);
        }
    }else{
        auto mat = reinterpret_cast<vector<vector<long double>*>*>(varlist->variable);
        auto i1 = (int)(calculateExpression(varlist->left));
        auto i2 = (int)(calculateExpression(varlist->right));
        if(i1 < mat->size() && i2 < (*(*mat)[i1]).size()){
            (*(*mat)[i1])[i2] = value;
        }else{
            cerr << "Error: Index out of bounds for matrix <" << varlist->variable << "> at indices <" << i1 << "," << i2 << ">." << endl;
            exit(0);
        }
    }
}

//! Get the Value of a Variable, List or Matrix MathNode
inline long double getVarListEntry(MathNode* varlist){
    if(varlist->type == MathNodeType::Variable){
        return *(long double*)varlist->variable;
    }else if(varlist->type == MathNodeType::Array){
        auto list = reinterpret_cast<vector<long double>*>(varlist->variable);
        auto i = (int)(calculateExpression(varlist->left));
        if(list->size() > i){
            return (*list)[i];
        }else{
            cerr << "Error: Index out of bounds for list <" << varlist->variable << "> at index <" << i << ">." << endl;
            exit(0);
        }
    }else{
        auto mat = reinterpret_cast<vector<vector<long double>*>*>(varlist->variable);
        auto i1 = (int)(calculateExpression(varlist->left));
        auto i2 = (int)(calculateExpression(varlist->right));
        if(i1 < mat->size() && i2 < (*(*mat)[i1]).size()){
            return (*(*mat)[i1])[i2];
        }else{
            cerr << "Error: Index out of bounds for matrix <" << varlist->variable << "> at indices <" << i1 << "," << i2 << ">." << endl;
            exit(0);
        }
    }
}

//! Round Value of Variable to Specific Precision
inline long double round_to(long double value, long double precision) {
    return round(value / precision) * precision;
}

// COMMAND FUNCTIONS ##############################################################

//! Command: Define A Single Variable (CVAR)
inline void command_cvar(long double* varname, MathNode* expression){
    *varname = calculateExpression(expression);
}

//! Command: Define Multiple Variables (MVAR)
inline void command_mvar(vector<long double*>* variables, MathNode* val){
    long double tmp_val = calculateExpression(val);
    for(long double* svars : *variables){
        *svars = tmp_val;
    }
}

//! Command: Define A List (CLIST)
inline void command_clist(vector<long double>* listid, MathNode* expression){
    listid->resize((int)calculateExpression(expression));
}

//! Command: Define A Matrix (CMAT)
inline void command_cmat(vector<vector<long double>* >* matid, MathNode* dim1, MathNode* dim2){
    for(int i = 0; i < (int)calculateExpression(dim1); i++){
        auto vct = new vector<long double>((int)calculateExpression(dim2));
        matid->push_back(vct);
    }
}

//! Command: Assign A Value to Variable or Matrix / List Entry (SET)
inline void command_set(MathNode* varlist, MathNode* expression){
    setVarListEntry(varlist,calculateExpression(expression));
}

//! Command: Round Value of Variable Up (CEIL)
inline void command_ceil(MathNode* varlist){
    setVarListEntry(varlist, ceil(getVarListEntry(varlist)));
}

//! Command: Round Value of Variable Off (FLOOR)
inline void command_floor(MathNode* varlist){
    setVarListEntry(varlist, floor(getVarListEntry(varlist)));
}

//! Command: Get Absolute Value of Variable (ABS)
inline void command_abs(MathNode* varlist){
    setVarListEntry(varlist, abs(getVarListEntry(varlist)));
}

//! Command: Round Value to Specicic Accuracy (ROUND)
inline void command_round(MathNode* varlist, MathNode* precision){
    long double prec = calculateExpression(precision);
    setVarListEntry(varlist, round_to(getVarListEntry(varlist),prec));
}

//! Command: Print String and Break Line (PRINT)
inline void command_print(const string& stri){
    string str = extract_string(stri);
    cout << str << endl;
}

//! Command: Print String Without Breaking The Line (PRINTB)
inline void command_printb(const string& stri){
    string str = extract_string(stri);
    cout << str;
}

//! Command: Print Value of Variable (PRINTV)
inline void command_printv(MathNode* expression) {
    cout << calculateExpression(expression); // print output of processed expression
}

//! Command: Print Matrix to Screen (PRINTM)
inline void command_printm(vector<vector<long double>*>* matrix) {
    for(auto row : *matrix){
        for(auto val : *row){
            cout << val << " ";
        }
        cout << endl;
    }
}

//! Command: Increment Value of Variable (INC)
inline void command_inc(MathNode* varlist) {
    setVarListEntry(varlist, getVarListEntry(varlist) + 1);
}

//! Command: Decrement Value of Variable (DEC)
inline void command_dec(MathNode* varlist) {
    setVarListEntry(varlist, getVarListEntry(varlist) - 1);
}

//! Command: Get Input from CLI and Save to Variable (INPUT)
inline void command_input(MathNode* varlist) {
    long double inputval;
    cin >> inputval;
    setVarListEntry(varlist, inputval);
}

//! Command: Calculate Trigonometric Function and Save to Target (TRIG)
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

//! Command: Save the Length of a List (GETL)
inline void command_getl(MathNode* varlist, vector<long double>* listid){
    setVarListEntry(varlist, (long double)listid->size());
}

//! Command: Save the Dimensions of a Matrix (GETDIM)
inline void command_getdim(MathNode* rows, MathNode* columns, vector<vector<long double>*>* listid){
    setVarListEntry(rows, (long double)listid->size());
    setVarListEntry(columns, (long double)((*listid)[0])->size());
}

//! Command: Calculate a Specific Root of the Variable and Save it (XROOT)
inline void command_xroot(MathNode* varlist, MathNode* source, MathNode* root){
    long double inpVal = calculateExpression(source);
    long double roottype = calculateExpression(root);
    long double outVal = pow(inpVal, 1.0/roottype);
    setVarListEntry(varlist, outVal);
}

//! Command: Calculate a Specific Logarithm of the Variable and Save it (LOG)
inline void command_log(MathNode* varlist, MathNode* source, MathNode* exp){ //HERE
    long double inpVal = calculateExpression(source);
    long double expVal = calculateExpression(exp);
    long double outVal = (log10(inpVal)/log10(expVal));
    setVarListEntry(varlist, outVal);
}

//! Command: Declare a Void Function Without Parameters (FUNCT)
inline void command_declarefunct(const string& identifier, Node* exec_block){
    if(funcs.find(identifier) == funcs.end()){
        funcs[identifier] = exec_block;
    }else{
        cerr << "Error: function <" << identifier << "> is aleady defined" << endl;
        exit(0); // terminate program
    }
}

//! Command: Pop The Last Value Of a List And Save It To a Variable(POP)
inline void command_poplist(MathNode* varlist, vector<long double>* listid){
    setVarListEntry(varlist, listid->back());
    listid->pop_back();
}

//! Command: Push A Specific Value to A List (PUSH)
inline void command_pushlist(MathNode* expr, vector<long double>* listid){
    long double val = calculateExpression(expr);
    listid->push_back(val);
}

//! Command: Change The Size of A List (CHSL)
inline void command_chsl(MathNode* expr, vector<long double>* listid){
    int new_l = (int)calculateExpression(expr);
    listid->resize(new_l);
}

//! Command: Define a List With Predefined Values (LDEF)
inline void command_ldef(vector<long double>* listid, string* stri){
    string content = removeSpaces(extract_list_string(*stri));
    parse_numbers(content,listid);
}

//! Command: Define a Matrix With Predefined Values (MDEF)
inline void command_mdef(vector<vector<long double>* >* matid, string* stri){
    parse_matrix(*stri,matid);
}

//! Command: Read a CSV File and Save it to a Matrix (READF)
inline void command_readf(vector<vector<long double>*>* listid, string* filename){
    string filepath = extract_string(*filename);
    read_csv(filepath,listid);
}

//! Command: Write a Matrix to a CSV File (WRITEF)
inline void command_writef(vector<vector<long double>*>* listid, string* filename){
    string filepath = extract_string(*filename);
    write_csv(filepath,listid);
}

//! Command: Define a Random Value in the Closed Interval From Max to Min and Save it to a Variable (RANDOM)
inline void command_random(MathNode* varlist, MathNode* max, MathNode* min){
    std::random_device rd;
    std::seed_seq seed{ rd(), static_cast<uint32_t>(std::chrono::high_resolution_clock::now().time_since_epoch().count()) };
    std::mt19937_64 eng(seed);
    std::uniform_real_distribution<long double> distr(calculateExpression(min), calculateExpression(max));
    setVarListEntry(varlist,distr(eng));
}

// MAIN FUNCTIONS FOR THE EXECUTION OF COMMANDS

//! Determine the Specific Function to Call for The Command
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
        case PRINTM:
            command_printm((vector<vector<long double>* >*)statement->children[0]->value);
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
        case CMAT:
            command_cmat((vector<vector<long double>* >*)statement->children[0]->value, statement->children[1]->expression, statement->children[2]->expression);
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
        case GETDIM:
            command_getdim(statement->children[0]->expression, statement->children[1]->expression, (vector<vector<long double>*>*)statement->children[2]->value);
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
            command_readf((vector<vector<long double>*>*)statement->children[0]->value, (string*)statement->children[1]->value);
            break;
        case WRITEF:
            command_writef((vector<vector<long double>*>*)statement->children[0]->value, (string*)statement->children[1]->value);
            break;
        case LDEF:
            command_ldef((vector<long double>*)statement->children[0]->value, (string*)statement->children[1]->value);
            break;
        case MDEF:
            command_mdef((vector<vector<long double>* >*)statement->children[0]->value, (string*)statement->children[1]->value);
            break;
        case RANDOM:
            command_random(statement->children[0]->expression,statement->children[1]->expression,statement->children[2]->expression);
            break;
    }
}

//! Execute the Tree Recursively and Implement Control Structures
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
            if_state = false;
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
