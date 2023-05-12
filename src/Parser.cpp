/*
 *  PARSER.CPP
 *  This file contains the parser and print functions for the SquareBracket language.
 *  The parser analyzes the tokens and creates a distinct tree which can be executed in the next step.
 *  Copyright (c) 2023, Patrick De Smet
 */

#include "Tokens.h"

int rLine = -1; // this variable tells the parser where to continue after an indented block

//! Create a Single Variable
long double* makeVarEntry(const std::string& str){
    if (vars.find(str) == vars.end()){
        long double* newVar = (long double*)malloc(sizeof(long double));
        vars[str] = newVar;
        return newVar;
    } else {
        std::cerr << "Error: variable <" << str << "> is already defined." << std::endl;
        exit(0);
    }
}

//! Create Multiple Variables at Once
std::vector<long double*>* makeMultipleVarEntries(const std::string& str){
    auto* tmpVec = new std::vector<long double*>;
    std::istringstream iss(str);
    std::vector<std::string> words;
    for (std::string word; iss >> word; ) {
        words.push_back(word);
    }
    for (const auto& word : words) {
        (*tmpVec).push_back(makeVarEntry(word));
    }
    return tmpVec;
}

//! Create a New List and Return Pointer
std::vector<long double>* makeListEntry(const std::string& str){
    if (ars.find(str) == ars.end()){
        auto newArr = new std::vector<long double>;
        ars[str] = newArr;
        return newArr;
    } else {
        std::cerr << "Error: list <" << str << "> is already defined." << std::endl;
        exit(0);
    }
}

//! Get Pointer for List from String
std::vector<long double>* getListEntry(const std::string& str){
    if (ars.find(str) != ars.end()) {
        return ars[str];
    } else {
        std::cerr << "Error: list <" << str << "> is not defined." << std::endl;
        exit(0);
    }
}

//! Create a New Empty Matrix and Return Pointer
std::vector<std::vector<long double>* >* makeMatrixEntry(const std::string& str){
    if (mars.find(str) == mars.end()){
        auto newMat = new std::vector<std::vector<long double>* >;
        mars[str] = newMat;
        return newMat;
    } else {
        std::cerr << "Error: matrix <" << str << "> is already defined." << std::endl;
        exit(0);
    }
}

//! Get Pointer for Matrix from String
std::vector<std::vector<long double>* >* getMatrixEntry(const std::string& str){
    if (mars.find(str) != mars.end()) {
        return mars[str];
    } else {
        std::cerr << "Error: matrix <" << str << "> is not defined." << std::endl;
        exit(0);
    }
}

//! Parse Tokens, Determine Type of Node and Build Tree
Node* parseTree(const std::vector<std::vector<std::string> >& tokens, int startLine) {
    Node* root = new Node;
    root->type = ROOT;
    std::vector<Node*> stack;
    stack.push_back(root);
    int currentLine = 0;

    while(currentLine < tokens.size()){
        if(currentLine < startLine){
            currentLine++;
            continue;
        }
        std::vector<std::string> line = tokens[currentLine];
        if(line[0] == "#"){
            currentLine++;
            continue;
        }
        TokenType type = ROOT;
        std::string value = line[0];
        Node* tmpNod1 = new Node;
        Node* tmpNod2 = new Node;
        Node* tmpNod3 = new Node;
        Node* child = new Node;

        if (value == "cvar") {
            type = CVAR;
            tmpNod1->type = VARIABLE;
            tmpNod1->value = makeVarEntry(line[1]);
            MathNode* expr = mathparse(line[2]);
            tmpNod2->type = EXPRESSION;
            tmpNod2->expression = expr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "clist") {
            type = CLIST;
            tmpNod1->type = LISTIDENT;
            tmpNod1->value = makeListEntry(line[1]);
            MathNode* expr = mathparse(line[2]);
            tmpNod2->type = EXPRESSION;
            tmpNod2->expression = expr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "cmat") {
            type = CMAT;
            tmpNod1->type = MATIDENT;
            tmpNod1->value = makeMatrixEntry(line[1]);
            MathNode* dim1 = mathparse(line[2]);
            MathNode* dim2 = mathparse(line[3]);
            tmpNod2->type = EXPRESSION;
            tmpNod2->expression = dim1;
            tmpNod3->type = EXPRESSION;
            tmpNod3->expression = dim2;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
            child->children.push_back(tmpNod3);
        } else if (value == "mvar") {
            type = MVAR;
            tmpNod1->type = VARIABLES;
            std::string apStr;
            for(int i = 1; i < line.size() - 1; i++){
                if(i < line.size() - 2){
                    apStr.append(line[i]);
                    apStr.append(" ");
                }else{
                    apStr.append(line[i]);
                }
            }
            tmpNod1->value = makeMultipleVarEntries(apStr);
            MathNode* expr = mathparse(line[line.size() - 1]);
            tmpNod2->type = EXPRESSION;
            tmpNod2->expression = expr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "set") {
            type = SET;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            MathNode* expr = mathparse(line[2]);
            tmpNod2->type = EXPRESSION;
            tmpNod2->expression = expr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "input") {
            type = INPUT;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            tmpNod2->type = STRING;
            std::string* pStr = new std::string;
            for(int i = 2; i < line.size(); i++){
                if(i < line.size() - 1){
                    pStr->append(line[i] + " ");
                }else{
                    pStr->append(line[i]);
                }
            }
            tmpNod2->value = pStr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "loop") {
            type = LOOP;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            tmpNod2->type = EXEC;
            tmpNod2->children = parseTree(tokens,  currentLine + 1)->children;
            currentLine = rLine;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "autoloop") {
            type = AUTOLOOP;
            MathNode* vrl1 = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl1;
            MathNode* vrl2 = mathparse(line[2]);
            tmpNod2->type = EXPRESSION;
            tmpNod2->expression = vrl2;
            tmpNod3->type = EXEC;
            tmpNod3->children = parseTree(tokens,  currentLine + 1)->children;
            currentLine = rLine;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
            child->children.push_back(tmpNod3);
        } else if (value == "funct") {
            type = FUNCT;
            tmpNod1->type = STRING;
            std::string* apStr = new std::string;
            *apStr = line[1];
            tmpNod1->value = apStr;
            tmpNod2->type = EXEC;
            tmpNod2->children = parseTree(tokens,  currentLine + 1)->children;
            currentLine = rLine;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "while") {
            type = WHILE;
            MathNode* expr = mathparse(line[1]);
            tmpNod1->type = EXPRESSION;
            tmpNod1->expression = expr;
            tmpNod2->type = EXEC;
            tmpNod2->children = parseTree(tokens, currentLine + 1)->children;
            currentLine = rLine;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "inc") {
            type = INC;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            child->children.push_back(tmpNod1);
        } else if (value == "dec") {
            type = DEC;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            child->children.push_back(tmpNod1);
        } else if (value == "sleep") {
            type = SLEEP;
            MathNode* vrl = mathparse(line[1]);
            tmpNod1->type = EXPRESSION;
            tmpNod1->expression = vrl;
            child->children.push_back(tmpNod1);
        } else if (value == "elif") {
            type = ELIF;
            MathNode* expr = mathparse(line[1]);
            tmpNod1->type = EXPRESSION;
            tmpNod1->expression = expr;
            tmpNod2->type = EXEC;
            tmpNod2->children = parseTree(tokens, currentLine + 1)->children;
            currentLine = rLine;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "if") {
            type = IF;
            MathNode* expr = mathparse(line[1]);
            tmpNod1->type = EXPRESSION;
            tmpNod1->expression = expr;
            tmpNod2->type = EXEC;
            tmpNod2->children = parseTree(tokens, currentLine + 1)->children;
            currentLine = rLine;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "else") {
            type = ELSE;
            tmpNod1->type = EXEC;
            tmpNod1->children = parseTree(tokens, currentLine + 1)->children;
            currentLine = rLine;
            child->children.push_back(tmpNod1);
        } else if (value == "print") {
            type = PRINT;
            tmpNod1->type = STRING;
            std::string* apStr = new std::string;
            for(int i = 1; i < line.size(); i++){
                if(i < line.size() - 1){
                    apStr->append(line[i]);
                    apStr->append(" ");
                }else{
                    apStr->append(line[i]);
                }
            }
            tmpNod1->value = apStr;
            child->children.push_back(tmpNod1);
        } else if (value == "printb") {
            type = PRINTB;
            tmpNod1->type = STRING;
            std::string* apStr = new std::string;
            for(int i = 1; i < line.size(); i++){
                if(i < line.size() - 1){
                    apStr->append(line[i]);
                    apStr->append(" ");
                }else{
                    apStr->append(line[i]);
                }
            }
            tmpNod1->value = apStr;
            child->children.push_back(tmpNod1);
        } else if (value == "printv") {
            type = PRINTV;
            MathNode* expr = mathparse(line[1]);
            tmpNod1->type = EXPRESSION;
            tmpNod1->expression = expr;
            child->children.push_back(tmpNod1);
        } else if (value == "printm") {
            type = PRINTM;
            tmpNod1->type = MATIDENT;
            tmpNod1->value = getMatrixEntry(line[1]);
            child->children.push_back(tmpNod1);
        } else if (value == "ceil") {
            type = CEIL;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            child->children.push_back(tmpNod1);
        } else if (value == "floor") {
            type = FLOOR;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            child->children.push_back(tmpNod1);
        } else if (value == "abs") {
            type = ABS;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            child->children.push_back(tmpNod1);
        } else if (value == "round") {
            type = ROUND;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            MathNode* expr = mathparse(line[2]);
            tmpNod2->type = EXPRESSION;
            tmpNod2->expression = expr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "newl") {
            type = NEWL;
        } else if (value == "exit") {
            type = EXIT;
        } else if (value == "leave") {
            type = LEAVE;
        } else if (value == "]") {
            rLine = currentLine;
            type = END;
            child->type = type;
            root->children.push_back(child);
            return root;
        } else if (value == "log") {
            type = LOG;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            MathNode* expr = mathparse(line[2]);
            tmpNod2->type = EXPRESSION;
            tmpNod2->expression = expr;
            MathNode* expr2 = mathparse(line[3]);
            tmpNod3->type = EXPRESSION;
            tmpNod3->expression = expr2;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
            child->children.push_back(tmpNod3);
        } else if (value == "sin") {
            type = SIN;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            tmpNod2->type = EXPRESSION;
            MathNode* expr = mathparse(line[2]);
            tmpNod2->expression = expr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "cos") {
            type = COS;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            tmpNod2->type = EXPRESSION;
            MathNode* expr = mathparse(line[2]);
            tmpNod2->expression = expr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "tan") {
            type = TAN;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            tmpNod2->type = EXPRESSION;
            MathNode* expr = mathparse(line[2]);
            tmpNod2->expression = expr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "sec") {
            type = SEC;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            tmpNod2->type = EXPRESSION;
            MathNode* expr = mathparse(line[2]);
            tmpNod2->expression = expr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "csc") {
            type = CSC;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            tmpNod2->type = EXPRESSION;
            MathNode* expr = mathparse(line[2]);
            tmpNod2->expression = expr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "sec") {
            type = SEC;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            tmpNod2->type = EXPRESSION;
            MathNode* expr = mathparse(line[2]);
            tmpNod2->expression = expr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "cot") {
            type = COT;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            tmpNod2->type = EXPRESSION;
            MathNode* expr = mathparse(line[2]);
            tmpNod2->expression = expr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "asin") {
            type = ASIN;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            tmpNod2->type = EXPRESSION;
            MathNode* expr = mathparse(line[2]);
            tmpNod2->expression = expr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "acos") {
            type = ACOS;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            tmpNod2->type = EXPRESSION;
            MathNode* expr = mathparse(line[2]);
            tmpNod2->expression = expr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "atan") {
            type = ATAN;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            tmpNod2->type = EXPRESSION;
            MathNode* expr = mathparse(line[2]);
            tmpNod2->expression = expr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        }else if (value == "xroot") {
            type = XROOT;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            tmpNod2->type = EXPRESSION;
            MathNode* expr = mathparse(line[2]);
            tmpNod2->expression = expr;
            MathNode* expr2 = mathparse(line[3]);
            tmpNod3->type = EXPRESSION;
            tmpNod3->expression = expr2;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
            child->children.push_back(tmpNod3);
        }else if (value == "random") {
            type = RANDOM;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            tmpNod2->type = EXPRESSION;
            MathNode* expr = mathparse(line[2]);
            tmpNod2->expression = expr;
            MathNode* expr2 = mathparse(line[3]);
            tmpNod3->type = EXPRESSION;
            tmpNod3->expression = expr2;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
            child->children.push_back(tmpNod3);
        } else if (value == "push") {
            type = PUSH;
            tmpNod1->type = EXPRESSION;
            MathNode* expr = mathparse(line[1]);
            tmpNod1->expression = expr;
            tmpNod2->type = LISTIDENT;
            tmpNod2->value = getListEntry(line[2]);
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "pop") {
            type = POP;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            tmpNod2->type = LISTIDENT;
            tmpNod2->value = getListEntry(line[2]);
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "chsl") {
            type = CHSL;
            tmpNod1->type = LISTIDENT;
            tmpNod1->value = getListEntry(line[1]);
            tmpNod2->type = EXPRESSION;
            MathNode* expr = mathparse(line[2]);
            tmpNod2->expression = expr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "getl") {
            type = GETL;
            MathNode* vrl = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl;
            tmpNod2->type = LISTIDENT;
            tmpNod2->value = getListEntry(line[2]);
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "getdim") {
            type = GETDIM;
            MathNode* vrl1 = varparse(line[1]);
            tmpNod1->type = VARLIST;
            tmpNod1->expression = vrl1;
            MathNode* vrl2 = varparse(line[2]);
            tmpNod2->type = VARLIST;
            tmpNod2->expression = vrl2;
            tmpNod3->type = LISTIDENT;
            tmpNod3->value = getMatrixEntry(line[3]);
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
            child->children.push_back(tmpNod3);
        } else if (value == "readf") {
            type = READF;
            tmpNod1->type = MATIDENT;
            tmpNod1->value = getMatrixEntry(line[1]);
            tmpNod2->type = STRING;
            std::string* apStr = new std::string;
            *apStr = line[2];
            tmpNod2->value = apStr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "writef") {
            type = WRITEF;
            tmpNod1->type = MATIDENT;
            tmpNod1->value = getMatrixEntry(line[1]);
            tmpNod2->type = STRING;
            std::string* apStr = new std::string;
            *apStr = line[2];
            tmpNod2->value = apStr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "ldef") {
            type = LDEF;
            tmpNod1->type = LISTIDENT;
            tmpNod1->value = makeListEntry(line[1]);
            tmpNod2->type = STRING;
            std::string* apStr = new std::string;
            *apStr = line[2];
            tmpNod2->value = apStr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "mdef") {
            type = MDEF;
            tmpNod1->type = MATIDENT;
            tmpNod1->value = makeMatrixEntry(line[1]);
            tmpNod2->type = STRING;
            std::string* apStr = new std::string;
            *apStr = line[2];
            tmpNod2->value = apStr;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        } else if (value == "call") {
            type = CALL;
            tmpNod1->type = STRING;
            std::string* apStr = new std::string;
            *apStr = line[1];
            tmpNod1->value = apStr;
            child->children.push_back(tmpNod1);
        } else if (value == "sloop") {
            type = SLOOP;
            MathNode* expr = mathparse(line[1]);
            tmpNod1->type = EXPRESSION;
            tmpNod1->expression = expr;
            tmpNod2->type = EXEC;
            tmpNod2->children = parseTree(tokens, currentLine + 1)->children;
            currentLine = rLine;
            child->children.push_back(tmpNod1);
            child->children.push_back(tmpNod2);
        }

        child->type = type;
        root->children.push_back(child);
        currentLine++;
    }
    return root;
}

//! Print MathTree Subtree Recursively
void printMathTree(MathNode* root, int level) {
    std::cout << " ";
    switch (root->type) {
        case MathNodeType::Variable:
            std::cout << "VAR <" << root->variable << ">" << std::endl;
            break;
        case MathNodeType::Constant:
            std::cout << "CONST <" << root->constant << ">" << std::endl;
            break;
        case MathNodeType::Operator:
            //std::cout << "Operator <" << getOperator(root->opt) << ">" << std::endl;
            break;
        case MathNodeType::Array:
            std::cout << "ARR <" << root->variable << ">" << std::endl;
            break;
        case MathNodeType::Matrix:
            std::cout << "MAT <" << root->variable << ">" << std::endl;
            break;
    }

    int i = 0;
    if (root->left != nullptr) {
        for (int j = 0; j < level; j++) {
            std::cout << "   ";
        }
        if(root->type == MathNodeType::Array || root->type == MathNodeType::Matrix){
            std::cout << "`--";
        }else{
            std::cout << "|--";
        }
        printMathTree(root->left, level + 1);
        i++;
    }
    if (root->right != nullptr) {
        for (int j = 0; j < level; j++) {
            std::cout << "   ";
        }
        std::cout << "`--";
        printMathTree(root->right, level + 1);
        i++;
    }
}

//! Print the Parsed Tree Recursively
void printTree(Node* root, int level) {
    std::cout << " ";
    switch (root->type) {
        case SET:
            std::cout << "SET" << std::endl;
            break;
        case CVAR:
            std::cout << "CVAR" << std::endl;
            break;
        case MVAR:
            std::cout << "MVAR" << std::endl;
            break;
        case LOOP:
            std::cout << "LOOP" << std::endl;
            break;
        case AUTOLOOP:
            std::cout << "AUTOLOOP" << std::endl;
            break;
        case WHILE:
            std::cout << "WHILE" << std::endl;
            break;
        case INC:
            std::cout << "INC" << std::endl;
            break;
        case DEC:
            std::cout << "DEC" << std::endl;
            break;
        case IF:
            std::cout << "IF" << std::endl;
            break;
        case ELIF:
            std::cout << "ELIF" << std::endl;
            break;
        case ELSE:
            std::cout << "ELSE" << std::endl;
            break;
        case PRINT:
            std::cout << "PRINT" << std::endl;
            break;
        case PRINTB:
            std::cout << "PRINTB" << std::endl;
            break;
        case PRINTV:
            std::cout << "PRINTV" << std::endl;
            break;
        case PRINTM:
            std::cout << "PRINTM" << std::endl;
            break;
        case NEWL:
            std::cout << "NEWL" << std::endl;
            break;
        case VARIABLE:
            std::cout << "VARIABLE <" << root->value << ">" << std::endl;
            break;
        case VARIABLES:
            std::cout << "VARIABLES <" << root->value << ">" << std::endl;
            break;
        case VARLIST:
            std::cout << "VARLIST" << std::endl;
            for (int j = 0; j < level; j++) {
                std::cout << "   ";
            }
            std::cout << "`--";
            printMathTree(root->expression, level+1);
            break;
        case STRING:
            std::cout << "STRING" << std::endl;
            break;
        case ROOT:
            std::cout << "ROOT" << std::endl;
            break;
        case END:
            std::cout << "END" << std::endl;
            break;
        case EXEC:
            std::cout << "EXEC" << std::endl;
            break;
        case EXPRESSION:
            std::cout << "EXPRESSION" << std::endl;
            for (int j = 0; j < level; j++) {
                std::cout << "   ";
            }
            std::cout << "`--";
            printMathTree(root->expression, level+1);
            break;
        case CLIST:
            std::cout << "CLIST" << std::endl;
            break;
        case CMAT:
            std::cout << "CMAT" << std::endl;
            break;
        case LDEF:
            std::cout << "LDEF" << std::endl;
            break;
        case MDEF:
            std::cout << "MDEF" << std::endl;
            break;
        case READF:
            std::cout << "READF" << std::endl;
            break;
        case WRITEF:
            std::cout << "WRITEF" << std::endl;
            break;
        case LISTIDENT:
            std::cout << "LISTID" << std::endl;
            break;
        case MATIDENT:
            std::cout << "MATID" << std::endl;
            break;
        case INPUT:
            std::cout << "INPUT" << std::endl;
            break;
        case CEIL:
            std::cout << "CEIL" << std::endl;
            break;
        case FLOOR:
            std::cout << "FLOOR" << std::endl;
            break;
        case ROUND:
            std::cout << "ROUND" << std::endl;
            break;
        case EXIT:
            std::cout << "EXIT" << std::endl;
            break;
        case LEAVE:
            std::cout << "LEAVE" << std::endl;
            break;
        case SLOOP:
            std::cout << "SLOOP" << std::endl;
            break;
        case LOG:
            std::cout << "LOG" << std::endl;
            break;
        case EXP:
            std::cout << "EXP" << std::endl;
            break;
        case SIN:
            std::cout << "SIN" << std::endl;
            break;
        case COS:
            std::cout << "COS" << std::endl;
            break;
        case TAN:
            std::cout << "TAN" << std::endl;
            break;
        case CSC:
            std::cout << "SCS" << std::endl;
            break;
        case SEC:
            std::cout << "SEC" << std::endl;
            break;
        case COT:
            std::cout << "COT" << std::endl;
            break;
        case ATAN:
            std::cout << "ATAN" << std::endl;
            break;
        case ACOS:
            std::cout << "ACOS" << std::endl;
            break;
        case ASIN:
            std::cout << "ASIN" << std::endl;
            break;
        case GETL:
            std::cout << "GETL" << std::endl;
            break;
        case GETDIM:
            std::cout << "GETDIM" << std::endl;
            break;
        case XROOT:
            std::cout << "XROOT" << std::endl;
            break;
        case FUNCT:
            std::cout << "FUNCTION" << std::endl;
            break;
        case CALL:
            std::cout << "CALL" << std::endl;
            break;
        case ABS:
            std::cout << "ABS" << std::endl;
            break;
        case POP:
            std::cout << "POP" << std::endl;
            break;
        case PUSH:
            std::cout << "PUSH" << std::endl;
            break;
        case CHSL:
            std::cout << "CHSL" << std::endl;
            break;
        case RANDOM:
            std::cout << "RANDOM" << std::endl;
            break;
        case SLEEP:
            std::cout << "SLEEP" << std::endl;
            break;
    }

    int i = 0;
    for (auto& child : root->children) {
        for (int j = 0; j < level; j++) {
            std::cout << "   ";
        }
        if (i == root->children.size() - 1) {
            std::cout << "`--";
        } else {
            std::cout << "|--";
        }
        printTree(child, level + 1);
        i++;
    }
}

//! Print Information and Initiate Output of Tree
void printTreeRoot(Node* root){
    std::cout << "SquareBracket Parser (Version 2.2.2) -- © 2023 Patrick De Smet" << std::endl << std::endl;
    printTree(root, 0);
    std::cout << std::endl;
}
