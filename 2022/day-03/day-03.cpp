#include <iostream>
#include <string>
#include <fstream>
#include <cassert>
#include <vector>

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

namespace SingleSack {
    void get_common_items(std::string& common_items, const std::string& this_sack, const std::string& that_sack) {

        assert(!this_sack.empty());
        assert(!that_sack.empty());

        for (const auto item : this_sack) {
            if (that_sack.npos == that_sack.find(item)) { continue; }

            if (common_items.npos == common_items.find(item) || common_items.empty()) {
                common_items.push_back(item);
            }
        }
    }
    char get_common_item(const std::string& sack) {

        assert(!sack.empty());

        if (0 != sack.size() % 2) { throw(std::exception("Check if contents is divisible by 2")); }

        const auto compartment_size{ sack.size() / 2 };
        const auto& first_compartment{ sack.substr(0, compartment_size) };
        const auto& second_compartment{ sack.substr(compartment_size) };
        assert(first_compartment.size() == second_compartment.size());

        char common_item{ '\0' };
        for (const auto item : first_compartment) {
            if (second_compartment.npos == second_compartment.find(item)) { continue; }

            common_item = item;
            break;
        }

        return(common_item);
    }
}
namespace GroupSacks {

    bool item_exists_in_all_sacks(std::vector<std::string>&sacks, const char item) {

        assert(!sacks.empty());
        assert('\0' != item);

        auto exists_in_all_sacks{ true };
        for (const auto& sack : sacks) {

            if (sack.npos == sack.find(item)) { 
                exists_in_all_sacks = false;
                break;            
            }
        }
        return(exists_in_all_sacks);
    }
    char find_badge(std::vector<std::string>& sacks) {
         
        assert(!sacks.empty());

        const auto& first_sack{ sacks.front() };
        char badge_item{ '\0' };
        for (const auto item : first_sack) {

            auto exists_in_all_sacks{ item_exists_in_all_sacks(sacks, item) };
            if (!exists_in_all_sacks) { continue; }

            badge_item = item;
            break;
        }

        return(badge_item);
    }
}

constexpr auto input_path{ ".\\input.txt" };
int main() try {

    CInputFile input_file(input_path);

    std::cout << "Checking contents of rucksacks...\n";

    auto common_item_priority_total{ 0 };
    auto badges_priority_total{ 0 };
    std::vector<std::string> group_sacks;
    constexpr auto group_size{ 3 };
    for (std::string line; std::getline(input_file.file, line);) {

        const auto common_item{ SingleSack::get_common_item(line) };
        common_item_priority_total += get_priority(common_item);

        group_sacks.push_back(line);
        if (group_size == group_sacks.size()) {

            const auto badge_item{ GroupSacks::find_badge(group_sacks) };
            badges_priority_total += get_priority(badge_item);
            group_sacks.clear();
        }
    }

    std::cout
        << "Total from common item priorities: " << std::dec << common_item_priority_total << "\n"
        << "Total from  badge item priorities: " << std::dec << badges_priority_total << "\n";


    return(EXIT_SUCCESS);

} catch(const std::exception& exception) {

    std::cout << "Exception: " << exception.what() << "\n";
    return(EXIT_FAILURE);
}

