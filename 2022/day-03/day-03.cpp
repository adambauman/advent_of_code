#include <iostream>
#include <string>
#include <fstream>
#include <cassert>

#ifdef _DEBUG
constexpr auto debug{ true };
#else
constexpr auto debug{ false };
#endif

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

    char get_common_item(const std::string& sack_contents) {
    
        assert(!sack_contents.empty());
     
        if (0 != sack_contents.size() % 2) { throw(std::exception("Check if contents is divisible by 2")); }

        const auto compartment_size{ sack_contents.size() / 2 };
        const auto& first_compartment{ sack_contents.substr(0, compartment_size)};
        const auto& second_compartment{ sack_contents.substr(compartment_size) };
        assert(first_compartment.size() == second_compartment.size());

        char common_item{ 0 };
        for (const auto item : first_compartment) {

            if (second_compartment.npos == second_compartment.find(item)) { continue; }

            common_item = item; 
            break;
        }

        return(common_item);
    }

    int inline get_priority(const char item) {

        assert(0 != item);

        //Char   Char Val      Pri Val    Diff
        // F       070           32        -38   UCase stops at 90
        // f       102            6        -96   Lcase starts at 97
        constexpr auto ucase_offset{ 38 };
        constexpr auto lcase_offset{ 96 };
        return(item <= 90 ? item - ucase_offset : item - lcase_offset);
    }
}

constexpr auto input_path{ ".\\input.txt" };
int main() try {

    CInputFile input_file(input_path);

    std::cout << "Checking contents of rucksacks...\n";

    auto common_item_priority_total{ 0 };
    for (std::string line; std::getline(input_file.file, line);) {

        const auto common_item{ get_common_item(line) };
        common_item_priority_total += get_priority(common_item);

        //if constexpr (debug) { std::cout << line << ": " << common_item << " priority: " << std::dec << item_priority << "\n"; }
    }

    std::cout << "Total from common item priorities: " << std::dec << common_item_priority_total << "\n";

    return(EXIT_SUCCESS);

} catch(const std::exception& exception) {

    std::cout << "Exception: " << exception.what() << "\n";
    return(EXIT_FAILURE);
}

