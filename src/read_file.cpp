#include "read_file.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

std::string file_read_to_string(std::string filePath) {
    std::stringstream stream;
    std::ifstream source;
    std::string result;
    // Ensure ifstream objects can throw exceptions:
    source.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        source.open(filePath);
        stream << source.rdbuf();
        source.close();
        result = stream.str();
    }
    catch (std::ifstream::failure& e) {
        std::cout << "File read failed:" << e.what() << "\n";
    }
    return result;
}