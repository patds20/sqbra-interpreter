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
#include <map>
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
map<string, double> vars; // global map for all vars
map<string, vector<double> > ars; // global map for all arrays
map<string, Node> funcs; // global map for all functions

int execute(Node* node);

// MATH FUNCTIONS ################################################################

int precedence(char op) {
    if (op == '^')
        return 3;
    else if (op == '*' || op == '/' || op == '%')
        return 2;
    else if (op == '+' || op == '-')
        return 1;
    else
        return -1;
}

double parseValue(const string& expression);

double eval_op(double val1, double val2, char op) {
    switch (op) {
        case '+':
            return val1 + val2;
        case '-':
            return val1 - val2;
        case '*':
            return val1 * val2;
        case '/':
            return val1 / val2;
        case '^':
            return pow(val1, val2);
        case '%':
            return (float)((int)val1 % (int)val2);
        default:
            return 0;
    }
}

double evaluateExpression(string expr) {
    stack<double> values;
    stack<char> ops;

    for (int i = 0; i < expr.length(); i++) {
        // Skip spaces
        if (expr[i] == ' ')
            continue;

        // Check if current character is a digit or dot
        if (isdigit(expr[i]) || expr[i] == '.') {
            string num;
            while (i < expr.length() && (isdigit(expr[i]) || expr[i] == '.'))
                num += expr[i++];
            i--;
            values.push(stof(num));
        } else if (isalpha(expr[i])) {
            string var;
            while (i < expr.length() && (isalpha(expr[i]) || expr[i] == '[' || expr[i] == ']' || isdigit(expr[i])))
                var += expr[i++];
            i--;
            values.push(parseValue(var));
        } else if (expr[i] == '(') {
            ops.push(expr[i]);
        } else if (expr[i] == ')') {
            while (ops.top() != '(') {
                double val2 = values.top();
                values.pop();
                double val1 = values.top();
                values.pop();
                char op = ops.top();
                ops.pop();
                values.push(eval_op(val1, val2, op));
            }
            ops.pop();
        } else {
            while (!ops.empty() && precedence(ops.top()) >= precedence(expr[i])) {
                double val2 = values.top();
                values.pop();
                double val1 = values.top();
                values.pop();
                char op = ops.top();
                ops.pop();
                values.push(eval_op(val1, val2, op));
            }
            ops.push(expr[i]);
        }
    }

    while (!ops.empty()) {
        double val2 = values.top();
        values.pop();
        double val1 = values.top();
        values.pop();
        char op = ops.top();
        ops.pop();
        values.push(eval_op(val1, val2, op));
    }

    return values.top();
}

void load_math_const(){
    vars["cpi"] = 3.14159265358979323846264338327;
    vars["ce"] = 2.7182818284590452353602874713527;
    vars["csqr2"] = 1.414213562373095048801688724209698078569;
    vars["csqr3"] = 1.732050807568877293527446341505872366942;
    vars["cemas"] = 0.57721566490153286060651209008240243104215933593992;
    vars["cproa"] = 1.6180339887498948482045868343656381177203091798057;
    vars["capry"] = 1.202056903159594285399738161511449990764986292;
}

// RESOLVE FUNCTIONS ##############################################################

double getListValue(const string& lid);

double stringToFloat(const std::string& str) {
    std::istringstream ss(str);
    double result;
    ss >> result;
    return result;
}

double parseValue(const string& expression){
    if(expression.empty()){
        cerr << "Error: no value provided" << endl;
        exit(0); // terminate program
    }else if (expression.find('(') != std::string::npos){
        return evaluateExpression(expression);
    }else if (expression.find('[') != std::string::npos){
        return getListValue(expression);
    }else if(vars.find(expression) != vars.end()) {
        return vars[expression];
    }else{
        return stringToFloat(expression);
    }
}

string extract_string(const string& input) {
    std::size_t start_pos = input.find("[\"");
    std::size_t end_pos = input.find("\"]");
    if (start_pos == std::string::npos || end_pos == std::string::npos || start_pos >= end_pos) {
        cerr << "Error: <" << input << "> string is not in the correct format";
        exit(0);
    }
    return input.substr(start_pos + 2, end_pos - start_pos - 2);
}

bool checkStatement(string statement) {
    string op;
    string var1, var2;
    statement = statement.substr(1, statement.size() - 2); // remove the front and end brackets
    int opIndex = -1;
    for (int i = 0; i < statement.size(); i++) {
        if (statement[i] == '=' || statement[i] == '<' || statement[i] == '>' || statement[i] == '!') {
            opIndex = i;
            break;
        }
    }
    var1 = statement.substr(0, opIndex);
    op = statement.substr(opIndex, 2);
    var2 = statement.substr(opIndex + 2);
    auto var1Val = parseValue(var1);
    auto var2Val = parseValue(var2);
    if (op == "==") return var1Val == var2Val;
    if (op == "!=") return var1Val != var2Val;
    if (op == ">>") return var1Val > var2Val;
    if (op == "<<") return var1Val < var2Val;
    if (op == ">=") return var1Val >= var2Val;
    if (op == "<=") return var1Val <= var2Val;
    cerr << "Error: <" << statement << "> statement could not be checked" << endl;
    exit(0);
}

double getListValue(const string& lid){
    std::string listname;
    std::string index;
    bool in_brackets = false;
    int brackets_depth = 0;
    for (char c : lid) {
        if (c == '[') {
            in_brackets = true;
            brackets_depth++;
        } else if (c == ']') {
            brackets_depth--;
            if (brackets_depth == 0) {
                in_brackets = false;
            }
        } else if (!in_brackets) {
            listname += c;
        } else {
            index += c;
        }
    }

    if(ars.find(listname) == ars.end()){
        cerr << "Error: list <" << listname << "> does not exist" << endl;
        exit(0); // terminate program
    }else{
        vector<double> cV = ars[listname];
        int pIndex = (int)parseValue(index);
        if(pIndex >= cV.size() || pIndex < 0){
            cerr << "Error: list entry <" << listname << "> at <" << pIndex << "> does not exist" << endl;
            exit(0); // terminate program
        }else{
            return cV[pIndex];
        }
    }
}

void setListValue(const string& lid, const string& expression){
    std::string listname;
    std::string index;

    bool in_brackets = false;
    int brackets_depth = 0;
    for (char c : lid) {
        if (c == '[') {
            in_brackets = true;
            brackets_depth++;
        } else if (c == ']') {
            brackets_depth--;
            if (brackets_depth == 0) {
                in_brackets = false;
            }
        } else if (!in_brackets) {
            listname += c;
        } else {
            index += c;
        }
    }

    if(ars.find(listname) == ars.end()){
        cerr << "Error: list <" << listname << "> does not exist" << endl;
        exit(0); // terminate program
    }else{
        int pIndex = (int)parseValue(index);
        if(pIndex >= ars[listname].size() || pIndex < 0){
            cerr << "Error: list entry <" << listname << "> at <" << pIndex << "> does not exist" << endl;
            exit(0); // terminate program
        }else{
            ars[listname][pIndex] = parseValue(expression);
        }
    }
}

double round_to(double value, double precision) {
    return round(value / precision) * precision;
}

// COMMAND FUNCTIONS ##############################################################

void command_cvar(const string& varname, const string& expression){
    if (vars.find(varname) == vars.end()) {
        vars[varname] = parseValue(expression); // there is no current entry -> resolve value and insert
    } else {
        cerr << "Error: variable <" << varname << "> does already exist" << endl;
        exit(0); // terminate program
    }
}

void command_clist(const string& listid, const string& expression){
    if (ars.find(listid) == ars.end()) {
        ars[listid] = vector<double>((int)parseValue(expression)); // there is no current entry -> resolve value and create list
    } else {
        cerr << "Error: list <" << listid << "> does already exist" << endl;
        exit(0); // terminate program
    }
}

void command_set(const string& varname, const string& expression){
    if (varname.find('[') != std::string::npos) { // set array instead of variable
        setListValue(varname, expression);
    }else if (vars.find(varname) == vars.end()) {
        cerr << "Error: variable or list entry (" << varname << ") does not exist" << endl;
        exit(0); // terminate program
    } else {
        vars[varname] = parseValue(expression); // change the current entry
    }
}

void command_ceil(const string& varname){
    if (varname.find('[') != std::string::npos) { // set array instead of variable
        std::string varAsString = std::to_string(ceil(getListValue(varname)));
        setListValue(varname, varAsString);
    }else if (vars.find(varname) == vars.end()) {
        cerr << "Error: variable or list entry (" << varname << ") does not exist" << endl;
        exit(0); // terminate program
    } else {
        vars[varname] = ceil(vars[varname]); // change the current entry
    }
}

void command_floor(const string& varname){
    if (varname.find('[') != std::string::npos) { // set array instead of variable
        std::string varAsString = std::to_string(floor(getListValue(varname)));
        setListValue(varname, varAsString);
    }else if (vars.find(varname) == vars.end()) {
        cerr << "Error: variable or list entry (" << varname << ") does not exist" << endl;
        exit(0); // terminate program
    } else {
        vars[varname] = floor(vars[varname]); // change the current entry
    }
}


void command_abs(const string& varname){
    if (varname.find('[') != std::string::npos) { // set array instead of variable
        std::string varAsString = std::to_string(abs(getListValue(varname)));
        setListValue(varname, varAsString);
    }else if (vars.find(varname) == vars.end()) {
        cerr << "Error: variable or list entry (" << varname << ") does not exist" << endl;
        exit(0); // terminate program
    } else {
        vars[varname] = abs(vars[varname]); // change the current entry
    }
}

void command_round(const string& varname, const string& precision){
    double prec = parseValue(precision);
    if (varname.find('[') != std::string::npos) { // set array instead of variable
        std::string varAsString = std::to_string(round_to(getListValue(varname),prec));
        setListValue(varname, varAsString);
    }else if (vars.find(varname) == vars.end()) {
        cerr << "Error: variable or list entry (" << varname << ") does not exist" << endl;
        exit(0); // terminate program
    } else {
        vars[varname] = round_to(vars[varname],prec); // change the current entry
    }
}

void command_print(const string& stri){
    string str = extract_string(stri);
    cout << str << endl;
}

void command_printb(const string& stri){
    string str = extract_string(stri);
    cout << str;
}

void command_printv(const string& expression) {
    cout << parseValue(expression); // print output of processed expression
}

void command_inc(const string& varname) {
    if (varname.find('[') != std::string::npos) { // set array instead of variable
        std::string varAsString = std::to_string(getListValue(varname) + 1);
        setListValue(varname, varAsString);
    }else if (vars.find(varname) == vars.end()) {
        cerr << "Error: variable or list entry (" << varname << ") does not exist" << endl;
        exit(0); // terminate program
    } else {
        vars[varname] = vars[varname] + 1; // change the current entry
    }
}

void command_dec(const string& varname) {
    if (varname.find('[') != std::string::npos) { // set array instead of variable
        std::string varAsString = std::to_string(getListValue(varname) - 1);
        setListValue(varname, varAsString);
    }else if (vars.find(varname) == vars.end()) {
        cerr << "Error: variable or list entry (" << varname << ") does not exist" << endl;
        exit(0); // terminate program
    } else {
        vars[varname] = vars[varname] - 1; // change the current entry
    }
}

void command_input(const string& var) {
    double inputval;
    cin >> inputval;
    if (var.find('[') != std::string::npos) {
        string varAsString = to_string(inputval);
        setListValue(var,varAsString);
        return;
    }
    auto it = vars.find(var);
    if (it == vars.end()) {
        cerr << "Error: <" << var << "> unknown variable";
        exit(0);
    }
    vars[var] = inputval;
}

void command_mvar(const string& vs, const string& val){
    std::stringstream ss(vs);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> tokens(begin, end);

    for (auto &s: tokens) {
        command_cvar(s,val);
    }
}

void command_trig(const string& target, const string& source, TokenType func){
    double inpVal = parseValue(source); // get x
    double outVal;
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
            cerr << "Error: asin only accepts values from 1 to -1" << endl;
            exit(0); // terminate program
        }
        outVal = asin(inpVal);
    }else if(func == ACOS){
        if(inpVal > 1 || inpVal < -1){
            cerr << "Error: acos only accepts values from 1 to -1" << endl;
            exit(0); // terminate program
        }
        outVal = acos(inpVal);
    }else if(func == ATAN){
        if(inpVal > 1 || inpVal < -1){
            cerr << "Error: atan only accepts values from 1 to -1" << endl;
            exit(0); // terminate program
        }
        outVal = atan(inpVal);
    }

    if (target.find('[') != std::string::npos) { // set array instead of variable
        setListValue(target, to_string(outVal));
    }else if (vars.find(target) == vars.end()) {
        cerr << "Error: variable or list entry (" << target << ") does not exist" << endl;
        exit(0); // terminate program
    } else {
        vars[target] = parseValue(to_string(outVal)); // change the current entry
    }
}

void command_getl(const string& var, const string& listid){
    if (ars.find(listid) == ars.end()) {
        cerr << "Error: list <" << listid << "> does not exist" << endl;
        exit(0); // terminate program
    } else {
        int len = ars[listid].size();
        if (var.find('[') != std::string::npos) { // set array instead of variable
            setListValue(var, to_string(len));
        }else if (vars.find(var) == vars.end()) {
            cerr << "Error: variable or list entry (" << var << ") does not exist" << endl;
            exit(0); // terminate program
        } else {
            vars[var] = (double)len; // change the current entry
        }

    }
}

void command_xroot(const string& targetsource, const string& root){
    double inpVal = parseValue(targetsource);
    double roottype = parseValue(root);
    double outVal = pow(inpVal, 1.0/roottype);

    if (targetsource.find('[') != std::string::npos) { // set array instead of variable
        setListValue(targetsource, to_string(outVal));
    }else if (vars.find(targetsource) == vars.end()) {
        cerr << "Error: variable or list entry (" << targetsource << ") does not exist" << endl;
        exit(0); // terminate program
    } else {
        vars[targetsource] = parseValue(to_string(outVal)); // change the current entry
    }
}

void command_log(const string& target, const string& source, const string& exp){
    double inpVal = parseValue(source);
    double expVal = parseValue(exp);
    double outVal = (log10(inpVal)/log10(expVal));

    if (target.find('[') != std::string::npos) { // set array instead of variable
        setListValue(target, to_string(outVal));
    }else if (vars.find(target) == vars.end()) {
        cerr << "Error: variable or list entry (" << target << ") does not exist" << endl;
        exit(0); // terminate program
    } else {
        vars[target] = parseValue(to_string(outVal)); // change the current entry
    }
}

void command_declarefunct(const string& identifier, Node* exec_block){
    if(funcs.find(identifier) == funcs.end()){
        funcs[identifier] = *exec_block;
    }else{
        cerr << "Error: function <" << identifier << "> is aleady defined" << endl;
        exit(0); // terminate program
    }
}

// MAIN FUNCTIONS FOR THE EXECUTION OF COMMANDS

void executeCommand(Node* statement){
    if(statement->type == CVAR){
        command_cvar(statement->children[0].value,statement->children[1].value);
    }else if(statement->type == SET){
        command_set(statement->children[0].value,statement->children[1].value);
    }else if(statement->type == PRINT){
        command_print(statement->children[0].value);
    }else if(statement->type == PRINTB){
        command_printb(statement->children[0].value);
    }else if(statement->type == PRINTV){
        command_printv(statement->children[0].value);
    }else if(statement->type == DEC){
        command_dec(statement->children[0].value);
    }else if(statement->type == INC){
        command_inc(statement->children[0].value);
    }else if(statement->type == CLIST){
        command_clist(statement->children[0].value,statement->children[1].value);
    }else if(statement->type == INPUT){
        command_printb(statement->children[1].value);
        command_input(statement->children[0].value);
    }else if(statement->type == MVAR) {
        command_mvar(statement->children[0].value, statement->children[1].value);
    }else if(statement->type == ROUND) {
        command_round(statement->children[0].value, statement->children[1].value);
    }else if(statement->type == CEIL) {
        command_ceil(statement->children[0].value);
    }else if(statement->type == FLOOR) {
        command_floor(statement->children[0].value);
    }else if(statement->type == ABS) {
        command_abs(statement->children[0].value);
    }else if(statement->type == SIN) {
        command_trig(statement->children[0].value,statement->children[1].value,SIN);
    }else if(statement->type == COS) {
        command_trig(statement->children[0].value,statement->children[1].value,COS);
    }else if(statement->type == TAN) {
        command_trig(statement->children[0].value,statement->children[1].value,TAN);
    }else if(statement->type == CSC) {
        command_trig(statement->children[0].value,statement->children[1].value,CSC);
    }else if(statement->type == SEC) {
        command_trig(statement->children[0].value,statement->children[1].value,SEC);
    }else if(statement->type == COT) {
        command_trig(statement->children[0].value,statement->children[1].value,COT);
    }else if(statement->type == ASIN) {
        command_trig(statement->children[0].value,statement->children[1].value,ASIN);
    }else if(statement->type == ACOS) {
        command_trig(statement->children[0].value,statement->children[1].value,ACOS);
    }else if(statement->type == ATAN) {
        command_trig(statement->children[0].value,statement->children[1].value,ATAN);
    }else if(statement->type == GETL) {
        command_getl(statement->children[0].value,statement->children[1].value);
    }else if(statement->type == XROOT) {
        command_xroot(statement->children[0].value,statement->children[1].value);
    }else if(statement->type == LOG) {
        command_log(statement->children[0].value,statement->children[1].value,statement->children[1].children[0].value);
    }else if(statement->type == FUNCT) {
        command_declarefunct(statement->children[0].value,&statement->children[1]);
    }else if(statement->type == NEWL) {
        cout << endl;
    }
}

int execute(Node* node){
    if(node->type == EXEC || node->type == ROOT){
        for(Node com: node->children){
            if(execute(&com) == -1){ // execute all statements in ROOT or EXEC iteratively
                break;
            }
        }
    }else if(node->type == CVAR || node->type == MVAR || node->type == SET || node->type == INC || node->type == DEC || node->type == PRINT || node->type == PRINTB || node->type == PRINTV || node->type == CLIST || node->type == INPUT || node->type == CEIL || node->type == FLOOR || node->type == ROUND || node->type == NEWL || node->type == LOG || node->type == EXP || node->type == SIN || node->type == COS || node->type == CSC || node->type == SEC || node->type == TAN || node->type == COT || node->type == ASIN || node->type == ACOS || node->type == ATAN || node->type == GETL || node->type == XROOT  || node->type == FUNCT || node->type == ABS){
        executeCommand(node);
    }else if(node->type == LOOP) {
        while (vars[node->children[0].value] > 0) {
            execute(&(node->children[1]));
            vars[(node->children[0]).value] -= 1;
        }
    }else if(node->type == SLOOP){
        int loopc = (int)vars[node->children[0].value];
        while(vars[node->children[0].value] > 0){
            execute(&(node->children[1]));
            vars[(node->children[0]).value] -= 1;
        }
    }else if(node->type == IF){
        if(checkStatement(node->children[0].value)){
            execute(&(node->children[1]));
        }
    }else if(node->type == WHILE){
        while(checkStatement(node->children[0].value)){
            execute(&(node->children[1]));
        }
    }else if(node->type == CALL){
        if(funcs.find(node->children[0].value) == funcs.end()){
            cerr << "Error: function <" << node->children[0].value << "> is not defined" << endl;
            exit(0);
        }
        execute(&funcs[node->children[0].value]);
    }else if(node->type == EXIT){
        exit(0);
    }else if(node->type == LEAVE){
        return -1;
    }
    return 0;
} // execute the parsed tree


#endif //SQBRA_RECURSER_H
