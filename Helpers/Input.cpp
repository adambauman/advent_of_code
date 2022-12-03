#include "Input.h"

CInputFile::CInputFile(const std::string& path) {
    this->file.exceptions(std::ifstream::badbit);
    this->file.open(path);
}
CInputFile::~CInputFile() { this->file.close(); }