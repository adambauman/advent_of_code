#pragma once

#include <string>
#include <fstream>
#include <sstream>

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

namespace FileSystem {
    namespace File {
        [[nodiscard]] inline std::string read(const std::string& input_path) {

            CInputFile input{ input_path };
            std::stringstream string_stream;
            string_stream << input.file.rdbuf();
            if (input.file.fail()) { throw(std::exception("Read input stream buffer")); }

            if (string_stream.str().empty()) { throw(std::exception("Check if contents read from buffer is empty")); }

            return(string_stream.str());
        }
    }
    namespace Disk {
        [[nodiscard]] std::string get_mount_point(const std::wstring& physical_path);
    }
}
