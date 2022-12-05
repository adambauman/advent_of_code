#include <iostream>
#include <fstream>
#include <cassert>
#include <sstream>
#include <vector>
#include <Windows.h>

#ifdef _DEBUG
constexpr auto debug{ true };
#else
constexpr auto debug{ false };
#endif

namespace Number {
    int from_string(const std::string& value) {

        assert(!value.empty());
        
        int number{ 0 };
        std::stringstream string_stream;
        string_stream << value;
        ::SetLastError(S_OK);
        string_stream >> number;
        const auto result{ ::GetLastError() };
        if (FAILED(result)) { throw(std::exception("Get number from string")); }

        return(number);
    }
}

namespace {
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

    std::pair<int, int> get_range(const std::string& pair) {

        assert(!pair.empty());

        std::vector<std::string> numbers;
        // Format NN-NN
        std::stringstream string_stream{ pair };
        for (std::string number; std::getline(string_stream, number, '-'); numbers.push_back(number)) {}
        if (2 != numbers.size()) { throw(std::exception("Check count of range numbers")); }

        return(std::pair<int, int>(Number::from_string(numbers[0]), Number::from_string(numbers[1])));
    }
    std::vector<std::pair<int, int>> get_ranges(const std::string& line) {

        assert(!line.empty());

        std::vector<std::pair<int, int>> ranges;
        // Format NN-NN,NN-NN
        std::stringstream string_stream{ line };
        for (std::string pair; std::getline(string_stream, pair, ','); ranges.push_back(get_range(pair))) {}
        if (2 != ranges.size()) { throw(std::exception("Check count of range pairs")); }

        return(ranges);
    }
    bool full_range_overlap(const std::pair<int, int>& this_range, const std::pair<int, int>& that_range) {

        auto full_overlap{ false };
        if (this_range.first <= that_range.first
            && this_range.second >= that_range.second) { 
            
            full_overlap = true; 
        }
       
        if(that_range.first <= this_range.first
            && that_range.second >= this_range.second) {
            
            full_overlap = true;
        }
        return(full_overlap);
    }
    int get_overlap_count(const std::string& input_path) {

        assert(!input_path.empty());

        CInputFile input_file(input_path);
        auto overlap_count{ 0 };
        for (std::string line; std::getline(input_file.file, line); ) {

            const auto ranges{ get_ranges(line) };
            if (full_range_overlap(ranges[0], ranges[1])) { ++overlap_count; }
        }
        return(overlap_count);
    }
}

constexpr auto input_path{ ".\\input.txt" };
int main() try {

    const auto overlap_count{ get_overlap_count(input_path) };
    std::cout << "Overlap count: " << std::dec << overlap_count << "\n";

    return(EXIT_SUCCESS);
}
catch (const std::exception& exception) {
    std::cerr << "Exception: " << exception.what() << "\n";
    return(EXIT_FAILURE);
}

