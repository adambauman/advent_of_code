#pragma once

#include <string>
#include <fstream>

class CInputFile {
    public:

        CInputFile(const std::string& path) {
            
            if (path.empty()) { throw(std::exception("Check if file path is empty")); }
            
            //this->file.exceptions(std::ifstream::failbit);
            this->file.open(path);
            if (!file.is_open()) { throw(std::exception("Check if file is open")); }

        }
        ~CInputFile() { this->file.close(); }

    public:
        std::ifstream file;
};