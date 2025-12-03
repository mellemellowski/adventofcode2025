#ifndef DATAFILEREADER_H
#define DATAFILEREADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

class DataFileReader {
public:
    DataFileReader(const std::string& filename)
        : filename_(filename) {}

    // Reads the entire file and returns its contents as a string
    std::string readAll() const {
        std::ifstream file(filename_);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to open file: " + filename_);
        }

        std::string content((std::istreambuf_iterator<char>(file)),
                             std::istreambuf_iterator<char>());
        return content;
    }

    // Reads line by line and prints to console
    void printLines() const {
        std::ifstream file(filename_);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to open file: " + filename_);
        }

        std::string line;
        while (std::getline(file, line)) {
            std::cout << line << '\n';
        }
    }

private:
    std::string filename_;
};

#endif
