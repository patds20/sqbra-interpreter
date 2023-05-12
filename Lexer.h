#pragma once
#ifndef SQBRA_LEXER_H
#define SQBRA_LEXER_H

/*
 *  LEXER.H
 *  This file declares the required functions for the lexing of the program.
 *  Copyright (c) 2023, Patrick De Smet
 */

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

std::vector<std::vector<std::string> > lexer(const std::string& input);

void repairTokens(std::vector<std::vector<std::string> > *strings);

void checkSyntax(std::vector<std::vector<std::string> > &tokens, bool pl);

#endif //SQBRA_LEXER_H
