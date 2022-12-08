#pragma once

#include <string>
#include <fstream>

class CInputFile {
    public:

        CInputFile(const std::string& path) {
            this->file.exceptions(std::ifstream::badbit);
            this->file.open(path);
        }
        ~CInputFile() { this->file.close(); }

    public:
        std::ifstream file;
};