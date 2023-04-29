#include <fstream>
#include <iostream>
#include <vector>
#include "Lexer.h"
#include "Lector.h"
#include "Parser.h"
#include "FilesIO.h"
#include "Recurser.h"
#include <cstring>

using namespace std;

// MANAGEMENT FUNCTIONS

void print_man_page()
{
    std::cout << "Usage: sqbra <flags> path-to-code.sqbr [input]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "\t-pt\tPrints the parsing tree without executing the code" << std::endl;
    std::cout << "\t-c\tChecks the code and prints potential errors" << std::endl;
    std::cout << "\t-I\tIndicates that some initial input variables are given" << std::endl;
    std::cout << "\t-v\tPrints the current version of the interpreter" << std::endl;
}

string parse_inputcommand(int argc, char** argv, bool& pt, bool& c, bool& v, bool& inp) {
    string codepath;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (strcmp(argv[i], "-pt") == 0) {
                pt = true;
            }
            else if (strcmp(argv[i], "-c") == 0) {
                c = true;
            }
            else if (strcmp(argv[i], "-v") == 0) {
                v = true;
            }
            else if (strcmp(argv[i], "-I") == 0) {
                inp = true;
                ars["args"] = new vector<long double>;
            }
            else if (strcmp(argv[i], "--help") == 0) {
                print_man_page();
                exit(0);
            }
            else if (isdigit(argv[i][1]) && inp){
                string as_text = argv[i];
                long double val = stod(as_text);
                ars["args"]->push_back(val);
            }
        } else if (codepath.empty()){
            codepath = argv[i];
        } else if (inp && !codepath.empty()) {
            string as_text = argv[i];
            long double val = stod(as_text);
            ars["args"]->push_back(val);
        }
    }
    return codepath;
}

int main(int argc, char *argv[]) {
    cout.precision(17); // set precision for output
    string path_to_code; // the path to the code file
    bool pt = false; // flag for printing the parseTree
    bool c = false; // bool for checking the syntax only
    bool v = false; // print version
    bool inp = false; // bool for reading input

    // PARSE INPUT COMMANDS
    path_to_code = parse_inputcommand(argc, argv, pt, c, v, inp);

    if (v) {
        cout << "SquareBracket Interpreter (Version 2.2.1 -- Rapid Red Panda / 2023)" << endl;
        exit(0);
    }
    // CHECK WHETHER CODE IS AVAILABLE
    if (path_to_code.empty()){
        std::cout << "Error: path-to-code is not provided" << std::endl;
        print_man_page();
        return 1;
    }

    // READ CODE LINE BY LINE
    std::ifstream file(path_to_code);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file." << std::endl;
        return 1;
    }
    std::string code;
    std::string line;
    while (std::getline(file, line)) {
        code += line + '\n';
    } // read code line by line

    // INITIATE INTERPRETATION
    load_math_const();

    vector<vector<string> > tokens = lexer(code); // call the lexer to tokenize code
    repairTokens(&tokens); // fix small errors in the token vector
    if(c){ checkSyntax(tokens, true);} // if flag -c is activated -> only check code
    checkSyntax(tokens, false); // check code without being verbose
    Node* program_root = parseTree(tokens,0); // parse the tokens and create tree
    if(pt){ printTreeRoot(program_root); exit(0);} // print tree and stop if flag -pt is set
    execute(program_root); // execute code
    return 0;
}
