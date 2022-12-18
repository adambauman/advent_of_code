#include <iostream>
#include <sstream>
#include <string_view>
#include <set>

#include <FileSystem\FileSystem.h>
#include <String\StringHelpers.h>

#ifdef _DEBUG
constexpr auto debug{ true };
#else
constexpr auto debug{ false };
#endif

namespace Packet {
    namespace Header {
        constexpr auto length{ 4 };
    }
    namespace Message {
        constexpr auto length{ 14 };
    }
}

namespace {
    [[nodiscard]] std::string read_input(const std::string& input_path) {

        CInputFile input{ input_path };
        std::stringstream string_stream;
        string_stream << input.file.rdbuf();
        if (input.file.fail()) { throw(std::exception("Read input stream buffer")); }

        if (string_stream.str().empty()) { throw(std::exception("Check if contents read from buffer is empty")); }

        return(string_stream.str());
    }
}


constexpr auto input_path{ ".\\input.txt" };
int main() try {

    const auto input{ read_input(input_path) };
    //const auto input{ "bvwbjplbgvbhsrlpgdmjqwftvncz" };

    // Construct initial view and "sub view", iterate and increase current_step.
    int current_step{ 0 };
    std::string_view string_view(input);

    // Part 1
    //const auto search_length{ Packet::Header::length };

    // Part 2
    const auto search_length{ Packet::Message::length };

    namespace Char = Helpers::String::Char;
    auto header_offset{ 0 };
    bool first_run{ true };
    for (auto current_step(0);
        string_view.length() > current_step + search_length;
        ++current_step) {

        if constexpr (debug) { std::cout << std::dec << current_step << " "; }
        
        std::string_view sub_view;
        sub_view = string_view.substr(current_step, search_length);
        const auto test_set{ std::set<char>(sub_view.cbegin(), sub_view.cend()) };
        if (sub_view.size() != test_set.size()) { continue; }

        // Found the header.
        header_offset = current_step;
        break;
    }

    std::cout << "Header offset: " << std::dec << (header_offset + search_length) << "\n";
    
    return(EXIT_SUCCESS);
}
catch (const std::exception& exception) {

    std::cerr << "Exception: " << exception.what() << "\n";
    return(EXIT_FAILURE);
}