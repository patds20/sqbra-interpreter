#ifndef SQBRA_LECTOR_H
#define SQBRA_LECTOR_H

/*
 *  LECTOR.H
 *  This file contains the lector function for the SquareBracket language.
 *  The lector checks the syntax of the tokens and shows potential errors.
 *  Copyright (c) 2023, Patrick De Smet
 */

#include <vector>
#include <string>

bool error;

void checkPrintSyntax(vector<string> line, int lcount){
    if(line[0] == "print" || line[0] == "printb"){
        if(line[1][0] != '[' || line[1][1] != '"' || line[line.size()-1][line[line.size()-1].size()-1] != ']'|| line[line.size()-1][line[line.size()-1].size()-2] != '"'){
            cout << "False string: line <" << lcount << "> - string needs to be enclosed [\"hello\"]" << endl;
            error = true;
        }
    }else if(line[0] == "printv"){
        if(line.size() != 2){
            cout << "False use: line <" << lcount << "> - correct usage: printv <variable>" << endl;
            error = true;
        }
    }else if(line[0] == "input"){
        if(line[2][0] != '[' || line[2][1] != '"' || line[line.size()-1][line[line.size()-1].size()-1] != ']'|| line[line.size()-1][line[line.size()-1].size()-2] != '"'){
            cout << "False string: line <" << lcount << "> - string needs to be enclosed [\"hello\"]" << endl;
            error = true;
        }
    }
}

void checkFlowSyntax(vector<string> line, int lcount){
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
    }else if(line[0] == "if"){
        if(line[line.size()-1] != "[" || (line.size() != 4 && line.size() != 3)){
            cout << "False if statement: line <" << lcount << "> - correct usage: if (statement) (do) [" << endl;
            error = true;
        }
    }
}

void checkVarSyntax(vector<string> line, int lcount){
    if(line[0] == "cvar" && line.size() != 3){
        cout << "False declaration of variable: line <" << lcount << "> - correct usage: cvar <var> <value>" << endl;
        error = true;
    }else if(line[0] == "clist" && line.size() != 3){
        cout << "False declaration of list: line <" << lcount << "> - correct usage: clist <name> <length>" << endl;
        error = true;
    }else if(line[0] == "getl" && line.size() != 3){
        cout << "False getl: line <" << lcount << "> - correct usage: getl <var> <list>" << endl;
        error = true;
    }else if(line[0] == "set" && line.size() != 3){
        cout << "False assignment of value: line <" << lcount << "> - correct usage: set <var> (expression)" << endl;
        error = true;
    }
}

void checkSVSyntax(vector<string> line, int lcount){
    if(line[0] == "round" && line.size() != 3){
        cout << "False 'round' command: line <" << lcount << "> - correct usage: round <var> precision" << endl;
        error = true;
    }else if(line[0] != "round" && line.size() != 2){
        cout << "More arguments than required: <" << lcount << "> - correct usage: <command> <var>" << endl;
        error = true;
    }
}

void checkTrigSyntax(const vector<string>& line, int lcount){
    if(line.size() != 3) {
        cout << "False trigonometric command: line <" << lcount << "> - correct usage: <trig> <var1> <var2>" << endl;
        error = true;
    }
}

void checkSingleSyntax(const vector<string>& line, int lcount){
    if(line.size() != 1) {
        cout << "More arguments than required: <" << lcount << "> - correct usage: <command>" << endl;
        error = true;
    }
}

void checkLogSyntax(const vector<string>& line, int lcount){
    if(line.size() != 4) {
        cout << "False usage: <" << lcount << "> - correct usage: log <target> <source> <base>" << endl;
        error = true;
    }
}

void checkFunctionSyntax(vector<string> line, int lcount){
    if(line[0] == "funct" && line.size() != 3){
        cout << "False declaration of a function: line <" << lcount << "> - correct usage: funct <function_name> [" << endl;
        error = true;
    }else if(line[0] == "call" && line.size() != 2) {
        cout << "False call of function: line <" << lcount << "> - correct usage: call <function_name>" << endl;
        error = true;
    }
}

void checkSyntax(vector<vector<string> > &tokens, bool pl){
    int lcount = 1;
    error = false;
    if(pl) {
        cout << "SquareBracket Syntax Check (Version 1.3) *******" << endl << endl;
    }
    for(auto &line : tokens){
        string token = line[0];
        if(token == "print" || token == "printb" || token == "printv" || token == "input"){
            checkPrintSyntax(line, lcount);
        }else if(token == "loop" || token == "sloop" || token == "while" || token == "if"){
            checkFlowSyntax(line, lcount);
        }else if(token == "cvar" || token == "mvar" || token == "clist" || token == "set" || token == "getl"){
            checkVarSyntax(line, lcount);
        }else if(token == "inc" || token == "dec" || token == "ceil" || token == "floor" || token == "round"){
            checkSVSyntax(line, lcount);
        }else if(token == "sin" || token == "cos" || token == "tan" || token == "sec" || token == "csc" || token == "cot" || token == "asin" || token == "acos" || token == "atan") {
            checkTrigSyntax(line, lcount);
        }else if(token == "log"){
            checkLogSyntax(line, lcount);
        }else if(token == "exit" || token == "leave" || token == "newl") {
            checkSingleSyntax(line, lcount);
        }else if(token == "funct" || token == "call") {
            checkFunctionSyntax(line, lcount);
        }else if(token == "]") {
            if (line.size() > 1) {
                cout << "Syntax error: line <" << lcount << "> - ']' must always be the only statement in a line"
                     << endl;
                error = true;
            }
        }else if(token == "#"){
            // do nothing
        }else{
            cout << "Unknown command: line <" << lcount << "> - command (" << token << ") unknown" << endl;
            error = true;
        }
        lcount += 1;
    }
    if(error){
        cout << "Result: Errors were detected. **********************************" << endl;
        exit(0);
    }else if(!error && pl){
        cout << "Result: No errors were detected. **********************************" << endl;
        exit(0);
    }
}

#endif //SQBRA_LECTOR_H
