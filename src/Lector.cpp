/*
 *  LECTOR.CPP
 *  This file contains the lector function for the SquareBracket language.
 *  The lector checks the syntax of the tokens and shows potential errors.
 *  Copyright (c) 2023, Patrick De Smet
 */

#include "Lexer.h"

bool error;

//! Check the Syntax of most commands handling strings
inline void checkPrintSyntax(std::vector<std::string> line, int lcount){
    if (line[0] == "printm") {
        if (line.size() != 2) {
            std::cout << "False use: line <" << lcount << "> - correct usage: printm <matrix>" << std::endl;
            error = true;
        }
    }else if(line[0] == "printv"){
        if(line.size() != 2){
            std::cout << "False use: line <" << lcount << "> - correct usage: printv <variable>" << std::endl;
            error = true;
        }
    }else if(line[0] == "print" || line[0] == "printb"){
        if(line[1][0] != '[' || line[1][1] != '"' || line[line.size()-1][line[line.size()-1].size()-1] != ']'|| line[line.size()-1][line[line.size()-1].size()-2] != '"'){
            std::cout << "False string: line <" << lcount << "> - string needs to be enclosed [\"hello\"]" << std::endl;
            error = true;
        }
    }else if(line[0] == "printv"){
        if(line.size() != 2){
            std::cout << "False use: line <" << lcount << "> - correct usage: printv <variable>" << std::endl;
            error = true;
        }
    }else if(line[0] == "printm"){
        if(line.size() != 2){
            std::cout << "False use: line <" << lcount << "> - correct usage: printm <matrix>" << std::endl;
            error = true;
        }
    }else if(line[0] == "input"){
        if(line[2][0] != '[' || line[2][1] != '"' || line[line.size()-1][line[line.size()-1].size()-1] != ']'|| line[line.size()-1][line[line.size()-1].size()-2] != '"'){
            std::cout << "False string: line <" << lcount << "> - string needs to be enclosed [\"hello\"]" << std::endl;
            error = true;
        }
    }
}

//! Check Syntax of Control Flow Statements
inline void checkFlowSyntax(std::vector<std::string> line, int lcount){
    if(line[0] == "loop" || line[0] == "sloop"){
        if(line[2] != "do" || line[3] != "["){
            std::cout << "False loop statement: line <" << lcount << "> - correct usage: (s)loop <var> do [" << std::endl;
            error = true;
        }
    }else if(line[0] == "autoloop"){
        if(line[3] != "do" || line[4] != "[" || line.size() != 5){
            std::cout << "False autoloop statement: line <" << lcount << "> - correct usage: autoloop <var> <initvar> do [" << std::endl;
            error = true;
        }
    }else if(line[0] == "while"){
        if(line[line.size()-1] != "[" || (line.size() != 4 && line.size() != 3)){
            std::cout << "False while statement: line <" << lcount << "> - correct usage: while (statement) (do) [" << std::endl;
            error = true;
        }
    }else if(line[0] == "if" || line[0] == "elif"){
        if(line[line.size()-1] != "[" || (line.size() != 4 && line.size() != 3)){
            std::cout << "False if statement: line <" << lcount << "> - correct usage: (el)if (statement) (do) [" << std::endl;
            error = true;
        }
    }else if(line[0] == "else"){
        if(line[line.size()-1] != "[" || (line.size() != 3 && line.size() != 2)){
            std::cout << "False if statement: line <" << lcount << "> - correct usage: else (do) [" << std::endl;
            error = true;
        }
    }
}

//! Check Syntax of Commands that declare Variables
inline void checkVarSyntax(std::vector<std::string> line, int lcount){
    if(line[0] == "cvar" && line.size() != 3){
        std::cout << "False declaration of variable: line <" << lcount << "> - correct usage: cvar <var> <value>" << std::endl;
        error = true;
    }else if(line[0] == "clist" && line.size() != 3) {
        std::cout << "False declaration of list: line <" << lcount << "> - correct usage: clist <name> <length>" << std::endl;
        error = true;
    }else if(line[0] == "cmat" && line.size() != 4){
        std::cout << "False declaration of matrix: line <" << lcount << "> - correct usage: cmat <name> <dim1> <dim2>" << std::endl;
        error = true;
    }else if(line[0] == "getl" && line.size() != 3){
        std::cout << "False 'getl' command: line <" << lcount << "> - correct usage: getl <var> <list>" << std::endl;
        error = true;
    }else if(line[0] == "getdim" && line.size() != 4){
        std::cout << "False 'getdim' command: line <" << lcount << "> - correct usage: getdim <var_rows> <var_columns> <matrix>" << std::endl;
        error = true;
    }else if(line[0] == "set" && line.size() != 3){
        std::cout << "False assignment of value: line <" << lcount << "> - correct usage: set <var> (expression)" << std::endl;
        error = true;
    }
}

//! Check Syntax of ROUND Command
inline void checkSVSyntax(std::vector<std::string> line, int lcount){
    if(line[0] == "round" && line.size() != 3){
        std::cout << "False 'round' command: line <" << lcount << "> - correct usage: round <var> precision" << std::endl;
        error = true;
    }else if(line[0] != "round" && line.size() != 2){
        std::cout << "More arguments than required: <" << lcount << "> - correct usage: <command> <var>" << std::endl;
        error = true;
    }
}

//! Check Syntax of Trigonometric Functions
inline void checkTrigSyntax(const std::vector<std::string>& line, int lcount){
    if(line.size() != 3) {
        std::cout << "False trigonometric command: line <" << lcount << "> - correct usage: <trig> <var1> <var2>" << std::endl;
        error = true;
    }
}

//! Check Syntax of Commands that require no parameters
inline void checkSingleSyntax(const std::vector<std::string>& line, int lcount){
    if(line.size() != 1) {
        std::cout << "More arguments than required: <" << lcount << "> - correct usage: <command>" << std::endl;
        error = true;
    }
}

//! Check Syntax of Logarithm Command
inline void checkLogSyntax(const std::vector<std::string>& line, int lcount){
    if(line.size() != 4) {
        std::cout << "False usage: <" << lcount << "> - correct usage: log <target> <source> <base>" << std::endl;
        error = true;
    }
}

//! Check Syntax of Sleeop Command
inline void checkSleepSyntax(const std::vector<std::string>& line, int lcount){
    if(line.size() != 2) {
        std::cout << "False usage: <" << lcount << "> - correct usage: sleep <time in ms>" << std::endl;
        error = true;
    }
}

//! Check Syntax of Commands handling Lists
inline void checkListFunctionsSyntax(const std::vector<std::string>& line, int lcount){
    if(line.size() != 3 && line[0] == "chsl") {
        std::cout << "False usage: <" << lcount << "> - correct usage: <chsl> <list> <length>" << std::endl;
        error = true;
    }else if(line.size() != 3 && (line[0] == "pop" || line[0] == "push")){
        std::cout << "False usage: <" << lcount << "> - correct usage: <pop / push> <variable> <list>" << std::endl;
        error = true;
    }else if(line.size() != 3 && line[0] == "ldef"){
        std::cout << "False usage: <" << lcount << "> - correct usage: ldef <list> [0.32,0.32,0.55]" << std::endl;
        error = true;
    }else if(line.size() != 3 && line[0] == "mdef"){
        std::cout << "False usage: <" << lcount << "> - correct usage: mdef <matrix> [[0.1,0.2],[0.3,0.4]]" << std::endl;
        error = true;
    }else if(line.size() != 3 && (line[0] == "readf" || line[0] == "writef")){
        std::cout << "False usage: <" << lcount << "> - correct usage: (readf / writef) <list> [\"path/to-your/file\"]" << std::endl;
        error = true;
    }
}

//! Check Syntax of Commands handling Functions
inline void checkFunctionSyntax(std::vector<std::string> line, int lcount){
    if(line[0] == "funct" && line.size() != 3){
        std::cout << "False declaration of a function: line <" << lcount << "> - correct usage: funct <function_name> [" << std::endl;
        error = true;
    }else if(line[0] == "call" && line.size() != 2) {
        std::cout << "False call of function: line <" << lcount << "> - correct usage: call <function_name>" << std::endl;
        error = true;
    }
}

//! Check Syntax of XROOT Command
inline void checkXROOTSyntax(const std::vector<std::string>& line, int lcount){
    if(line.size() != 4) {
        std::cout << "False usage: <" << lcount << "> - correct usage: xroot <target> <source> <root>" << std::endl;
        error = true;
    }
}

//! Check Syntax of RANDOM Command
inline void checkRANDOMSyntax(const std::vector<std::string>& line, int lcount){
    if(line.size() != 4) {
        std::cout << "False usage: <" << lcount << "> - correct usage: random <target> <max> <min>" << std::endl;
        error = true;
    }
}

//! Check Syntax of all Tokens
void checkSyntax(std::vector<std::vector<std::string> > &tokens, bool pl){
    int lcount = 1;
    error = false;
    if(pl) {
        std::cout << "SquareBracket Syntax Check (Version 2.2.3 Rapid Red Panda) *******" << std::endl << std::endl;
    }
    for(auto &line : tokens){
        std::string token = line[0];
        if(token == "print" || token == "printb" || token == "printv" || token == "printm" || token == "input"){
            checkPrintSyntax(line, lcount);
        }else if(token == "loop" || token == "sloop" || token == "autoloop" || token == "while" || token == "if" || token == "elif" || token == "else"){
            checkFlowSyntax(line, lcount);
        }else if(token == "cvar" || token == "mvar"  || token == "cmat" || token == "clist" || token == "set" || token == "getl" || token == "getdim"){
            checkVarSyntax(line, lcount);
        }else if(token == "inc" || token == "dec" || token == "ceil" || token == "floor" || token == "round" || token == "abs"){
            checkSVSyntax(line, lcount);
        }else if(token == "sin" || token == "cos" || token == "tan" || token == "sec" || token == "csc" || token == "cot" || token == "asin" || token == "acos" || token == "atan") {
            checkTrigSyntax(line, lcount);
        }else if(token == "log"){
            checkLogSyntax(line, lcount);
        }else if(token == "exit" || token == "leave" || token == "newl"){
            checkSingleSyntax(line, lcount);
        }else if(token == "sleep"){
            checkSleepSyntax(line, lcount);
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
                std::cout << "Syntax error: line <" << lcount << "> - ']' must always be the only statement in a line" << std::endl;
                error = true;
            }
        }else if(token == "#" && token[0] == '#'){
            // do nothing
        }else{
            std::cout << "Unknown command: line <" << lcount << "> - command (" << token << ") unknown" << std::endl;
            error = true;
        }
        lcount += 1;
    }
    if(error){
        std::cout << "Result: Errors were detected." << std::endl;
        exit(0);
    }else if(!error && pl){
        std::cout << "Result: No errors were detected." << std::endl;
        exit(0);
    }
}
