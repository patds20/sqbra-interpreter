#pragma once
#ifndef SQBRA_RECURSER_H
#define SQBRA_RECURSER_H

/*
 *  RECURSER.H
 *  This file declares the required functions for the execution of the program.
 *  Copyright (c) 2023, Patrick De Smet
 */

#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <stack>
#include <string>
#include <sstream>
#include <fstream>
#include <cctype>
#include <algorithm>
#include <iterator>
#include <random>
#include <chrono>

#include "Tokens.h"

int execute(Node* node);

void load_math_const();

// FilesIO Declarations

std::string extract_string(const std::string& input);

std::string extract_list_string(const std::string& input);

void read_csv(const std::string& filename, std::vector<std::vector<long double>*>* output);

void write_csv(const std::string& filename, std::vector<std::vector<long double>*>* numbers);

void parse_numbers(const std::string& input, std::vector<long double>* output);

void parse_matrix(std::string& matrix_string, std::vector<std::vector<long double>* >* matrix);

#endif //SQBRA_RECURSER_H
