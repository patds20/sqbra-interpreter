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

inline string extract_string(const string& input) {
    std::size_t start_pos = input.find("[\"");
    std::size_t end_pos = input.find("\"]");
    if (start_pos == std::string::npos || end_pos == std::string::npos || start_pos >= end_pos) {
        cerr << "Error: <" << input << "> string is not in the correct format";
        exit(0);
    }
    return input.substr(start_pos + 2, end_pos - start_pos - 2);
}

inline string extract_list_string(const string& input) {
    std::size_t start_pos = input.find('[');
    std::size_t end_pos = input.find(']');
    if (start_pos == std::string::npos || end_pos == std::string::npos || start_pos >= end_pos) {
        cerr << "Error: <" << input << "> list is not in the correct format";
        exit(0);
    }
    return input.substr(start_pos + 1, end_pos - start_pos - 1);
}

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

#endif //SQBRA_FILESIO_H