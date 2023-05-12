#ifndef SQBRA_LECTOR_H
#define SQBRA_LECTOR_H

/*
 *  LECTOR.CPP
 *  This file contains the lector function for the SquareBracket language.
 *  The lector checks the syntax of the tokens and shows potential errors.
 *  Copyright (c) 2023, Patrick De Smet
 */

#include <vector>
#include <string>

bool error;

//! Check the Syntax of most commands handling strings
inline void checkPrintSyntax(vector<string> line, int lcount){
    if (line[0] == "printm") {
        if (line.size() != 2) {
            cout << "False use: line <" << lcount << "> - correct usage: printm <matrix>" << endl;
            error = true;
        }
    }else if(line[0] == "printv"){
        if(line.size() != 2){
            cout << "False use: line <" << lcount << "> - correct usage: printv <variable>" << endl;
            error = true;
        }
    }else if(line[0] == "print" || line[0] == "printb"){
        if(line[1][0] != '[' || line[1][1] != '"' || line[line.size()-1][line[line.size()-1].size()-1] != ']'|| line[line.size()-1][line[line.size()-1].size()-2] != '"'){
            cout << "False string: line <" << lcount << "> - string needs to be enclosed [\"hello\"]" << endl;
            error = true;
        }
    }else if(line[0] == "printv"){
        if(line.size() != 2){
            cout << "False use: line <" << lcount << "> - correct usage: printv <variable>" << endl;
            error = true;
        }
    }else if(line[0] == "printm"){
        if(line.size() != 2){
            cout << "False use: line <" << lcount << "> - correct usage: printm <matrix>" << endl;
            error = true;
        }
    }else if(line[0] == "input"){
        if(line[2][0] != '[' || line[2][1] != '"' || line[line.size()-1][line[line.size()-1].size()-1] != ']'|| line[line.size()-1][line[line.size()-1].size()-2] != '"'){
            cout << "False string: line <" << lcount << "> - string needs to be enclosed [\"hello\"]" << endl;
            error = true;
        }
    }
}

//! Check Syntax of Control Flow Statements
inline void checkFlowSyntax(vector<string> line, int lcount){
    if(line[0] == "loop" || line[0] == "sloop"){
        if(line[2] != "do" || line[3] != "["){
            cout << "False loop statement: line <" << lcount << "> - correct usage: (s)loop <var> do [" << endl;
            error = true;
        }
    }else if(line[0] == "while"){
        if(line[line.size()-1] != "[" || (line.size() != 4 && line.size() != 3)){
            cout << "False while statement: line <" << lcount << "> - correct usage: while (statement) (do) [" << endl;
            error = true;
        }
    }else if(line[0] == "if" || line[0] == "elif"){
        if(line[line.size()-1] != "[" || (line.size() != 4 && line.size() != 3)){
            cout << "False if statement: line <" << lcount << "> - correct usage: (el)if (statement) (do) [" << endl;
            error = true;
        }
    }else if(line[0] == "else"){
        if(line[line.size()-1] != "[" || (line.size() != 3 && line.size() != 2)){
            cout << "False if statement: line <" << lcount << "> - correct usage: else (do) [" << endl;
            error = true;
        }
    }
}

//! Check Syntax of Commands that declare Variables
inline void checkVarSyntax(vector<string> line, int lcount){
    if(line[0] == "cvar" && line.size() != 3){
        cout << "False declaration of variable: line <" << lcount << "> - correct usage: cvar <var> <value>" << endl;
        error = true;
    }else if(line[0] == "clist" && line.size() != 3) {
        cout << "False declaration of list: line <" << lcount << "> - correct usage: clist <name> <length>" << endl;
        error = true;
    }else if(line[0] == "cmat" && line.size() != 4){
            cout << "False declaration of matrix: line <" << lcount << "> - correct usage: cmat <name> <dim1> <dim2>" << endl;
            error = true;
    }else if(line[0] == "getl" && line.size() != 3){
        cout << "False 'getl' command: line <" << lcount << "> - correct usage: getl <var> <list>" << endl;
        error = true;
    }else if(line[0] == "getdim" && line.size() != 4){
        cout << "False 'getdim' command: line <" << lcount << "> - correct usage: getdim <var_rows> <var_columns> <matrix>" << endl;
        error = true;
    }else if(line[0] == "set" && line.size() != 3){
        cout << "False assignment of value: line <" << lcount << "> - correct usage: set <var> (expression)" << endl;
        error = true;
    }
}

//! Check Syntax of ROUND Command
inline void checkSVSyntax(vector<string> line, int lcount){
    if(line[0] == "round" && line.size() != 3){
        cout << "False 'round' command: line <" << lcount << "> - correct usage: round <var> precision" << endl;
        error = true;
    }else if(line[0] != "round" && line.size() != 2){
        cout << "More arguments than required: <" << lcount << "> - correct usage: <command> <var>" << endl;
        error = true;
    }
}

//! Check Syntax of Trigonometric Functions
inline void checkTrigSyntax(const vector<string>& line, int lcount){
    if(line.size() != 3) {
        cout << "False trigonometric command: line <" << lcount << "> - correct usage: <trig> <var1> <var2>" << endl;
        error = true;
    }
}

//! Check Syntax of Commands that require no parameters
inline void checkSingleSyntax(const vector<string>& line, int lcount){
    if(line.size() != 1) {
        cout << "More arguments than required: <" << lcount << "> - correct usage: <command>" << endl;
        error = true;
    }
}

//! Check Syntax of Logarithm Command
inline void checkLogSyntax(const vector<string>& line, int lcount){
    if(line.size() != 4) {
        cout << "False usage: <" << lcount << "> - correct usage: log <target> <source> <base>" << endl;
        error = true;
    }
}

//! Check Syntax of Commands handling Lists
inline void checkListFunctionsSyntax(const vector<string>& line, int lcount){
    if(line.size() != 3 && line[0] == "chsl") {
        cout << "False usage: <" << lcount << "> - correct usage: <chsl> <list> <length>" << endl;
        error = true;
    }else if(line.size() != 3 && (line[0] == "pop" || line[0] == "push")){
        cout << "False usage: <" << lcount << "> - correct usage: <pop / push> <variable> <list>" << endl;
        error = true;
    }else if(line.size() != 3 && line[0] == "ldef"){
        cout << "False usage: <" << lcount << "> - correct usage: ldef <list> [0.32,0.32,0.55]" << endl;
        error = true;
    }else if(line.size() != 3 && line[0] == "mdef"){
        cout << "False usage: <" << lcount << "> - correct usage: mdef <matrix> [[0.1,0.2],[0.3,0.4]]" << endl;
        error = true;
    }else if(line.size() != 3 && (line[0] == "readf" || line[0] == "writef")){
        cout << "False usage: <" << lcount << "> - correct usage: (readf / writef) <list> [\"path/to-your/file\"]" << endl;
        error = true;
    }
}

//! Check Syntax of Commands handling Functions
inline void checkFunctionSyntax(vector<string> line, int lcount){
    if(line[0] == "funct" && line.size() != 3){
        cout << "False declaration of a function: line <" << lcount << "> - correct usage: funct <function_name> [" << endl;
        error = true;
    }else if(line[0] == "call" && line.size() != 2) {
        cout << "False call of function: line <" << lcount << "> - correct usage: call <function_name>" << endl;
        error = true;
    }
}

//! Check Syntax of XROOT Command
inline void checkXROOTSyntax(const vector<string>& line, int lcount){
    if(line.size() != 4) {
        cout << "False usage: <" << lcount << "> - correct usage: xroot <target> <source> <root>" << endl;
        error = true;
    }
}

//! Check Syntax of RANDOM Command
inline void checkRANDOMSyntax(const vector<string>& line, int lcount){
    if(line.size() != 4) {
        cout << "False usage: <" << lcount << "> - correct usage: random <target> <max> <min>" << endl;
        error = true;
    }
}

//! Check Syntax of all Tokens
void checkSyntax(vector<vector<string> > &tokens, bool pl){
    int lcount = 1;
    error = false;
    if(pl) {
        cout << "SquareBracket Syntax Check (Version 2.2.2 Rapid Red Panda) *******" << endl << endl;
    }
    for(auto &line : tokens){
        string token = line[0];
        if(token == "print" || token == "printb" || token == "printv" || token == "printm" || token == "input"){
            checkPrintSyntax(line, lcount);
        }else if(token == "loop" || token == "sloop" || token == "while" || token == "if" || token == "elif" || token == "else"){
            checkFlowSyntax(line, lcount);
        }else if(token == "cvar" || token == "mvar"  || token == "cmat" || token == "clist" || token == "set" || token == "getl" || token == "getdim"){
            checkVarSyntax(line, lcount);
        }else if(token == "inc" || token == "dec" || token == "ceil" || token == "floor" || token == "round" || token == "abs"){
            checkSVSyntax(line, lcount);
        }else if(token == "sin" || token == "cos" || token == "tan" || token == "sec" || token == "csc" || token == "cot" || token == "asin" || token == "acos" || token == "atan") {
            checkTrigSyntax(line, lcount);
        }else if(token == "log"){
            checkLogSyntax(line, lcount);
        }else if(token == "exit" || token == "leave" || token == "newl") {
            checkSingleSyntax(line, lcount);
        }else if(token == "xroot") {
            checkXROOTSyntax(line, lcount);
        }else if(token == "random") {
            checkRANDOMSyntax(line, lcount);
        }else if(token == "funct" || token == "call") {
            checkFunctionSyntax(line, lcount);
        }else if(token == "pop" || token == "push" || token == "chsl" || token == "ldef" || token == "mdef" || token == "writef" || token == "readf") {
            checkListFunctionsSyntax(line, lcount);
        }else if(token == "]") {
            if (line.size() > 1) {
                cout << "Syntax error: line <" << lcount << "> - ']' must always be the only statement in a line"
                     << endl;
                error = true;
            }
        }else if(token == "#" && token[0] == '#'){
            // do nothing
        }else{
            cout << "Unknown command: line <" << lcount << "> - command (" << token << ") unknown" << endl;
            error = true;
        }
        lcount += 1;
    }
    if(error){
        cout << "Result: Errors were detected." << endl;
        exit(0);
    }else if(!error && pl){
        cout << "Result: No errors were detected." << endl;
        exit(0);
    }
}

#endif //SQBRA_LECTOR_H