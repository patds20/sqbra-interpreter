#pragma once
#ifndef SQBRA_FILESIO_H
#define SQBRA_FILESIO_H

/*
 *  FILESIO.H
 *  This file contains commands for reading and writing files.
 *  Copyright (c) 2023, Patrick De Smet
 */

#include <string>
#include <iostream>
#include <fstream>

//! Extract [" and "] from strings
inline string extract_string(const string& input) {
    std::size_t start_pos = input.find("[\"");
    std::size_t end_pos = input.find("\"]");
    if (start_pos == std::string::npos || end_pos == std::string::npos || start_pos >= end_pos) {
        cerr << "Error: <" << input << "> string is not in the correct format";
        exit(0);
    }
    return input.substr(start_pos + 2, end_pos - start_pos - 2);
}

//! Extract outer [ and ] from strings containing lists or matrices
inline string extract_list_string(const string& input) {
    std::size_t start_pos = input.find('[');
    std::size_t end_pos = input.find(']');
    if (start_pos == std::string::npos || end_pos == std::string::npos || start_pos >= end_pos) {
        cerr << "Error: <" << input << "> list is not in the correct format";
        exit(0);
    }
    return input.substr(start_pos + 1, end_pos - start_pos - 1);
}

//! Read and input csv files
inline void read_csv(const std::string& filename, std::vector<long double>* output) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file: " << filename << std::endl;
        exit(0);
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string token;
        while (std::getline(ss, token, ',')) {
            try {
                long double value = std::stold(token);
                output->push_back(value);
            } catch (const std::exception& e) {
                std::cerr << "Error: Failed to convert token to long double: " << token << std::endl;
                exit(0);
            }
        }
    }
}

//! Write lists to csv files
inline void write_csv(const std::string& filename, std::vector<long double>* numbers) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file: " << filename << std::endl;
        exit(0);
    }
    for (const auto& number : *numbers) {
        file << number << ",";
    }
    // Remove the last comma
    file.seekp(-1, std::ios_base::end);
    file << std::endl;
}

//! Parse a string [0.32, 0.435, 3.141] to a list
inline void parse_numbers(const std::string& input, std::vector<long double>* output) {
    std::istringstream ss(input);
    std::string token;
    while (std::getline(ss, token, ',')) {
        try {
            long double value = std::stold(token);
            output->push_back(value);
        } catch (const std::exception& e) {
            // Handle conversion error
        }
    }
}

//! Parse a string [[1,2,3],[4,5,6],[7,8,9]] to a matrix
inline void parse_matrix(string& matrix_string, vector<vector<long double>* >* matrix)
{
    // Remove the outermost brackets from the matrix string
    std::string stripped_string = matrix_string.substr(1, matrix_string.length() - 2);

    // Split the matrix string into rows
    std::stringstream row_stream(stripped_string);
    std::string row_string;
    while (std::getline(row_stream, row_string, ']'))
    {
        // Remove the leading "," character from each row string
        if(row_string[0] == ','){
            row_string = row_string.substr(1);
        }

        // Remove the leading "[" character from each row string
        row_string = row_string.substr(1);

        // Parse the row string into a vector of long doubles
        auto row = new std::vector<long double>();
        std::stringstream value_stream(row_string);
        std::string value_string;
        while (std::getline(value_stream, value_string, ','))
        {
            long double value = std::stold(value_string);
            row->push_back(value);
        }

        // Add the row to the matrix
        matrix->push_back(row);
    }
}

#endif //SQBRA_FILESIO_H
