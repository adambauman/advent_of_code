#include <iostream>
#include <vector>
#include <fstream>
#include <cassert>
#include <sstream>
#include <Windows.h>

#include <FileSystem/FileSystem.h>

//TODO: Parse out the stack count and max stack height.
constexpr auto stack_count{ 9 };
constexpr auto max_stack_height{ 8 };

// Forwards
namespace Instructions {

}

namespace Stacks {
#ifdef _DEBUG
    void output_stacks(const std::vector<std::vector<char>>& stacks) {

        std::cout
            << "Stack Output:\n"
            << "------------------------------------------\n";
        for (const auto stack : stacks) {
            for (const auto container : stack) {
                std::cout << container;
            }
            std::cout << "\n";
        }
        std::cout
            << "------------------------------------------\n";
    }
#endif
    [[nodiscard]] std::vector<char> process_line(const std::string& line) {

        assert(!line.empty());

        //TODO: Proper stack parsing, this will break if stack schema changes.
        std::vector<char> containers;
        for (auto index{ 1 }; line.size() >= index; index += 4) {

            containers.push_back(line[index]);
        }
        return(containers);
    }
    [[nodiscard]] std::vector<std::vector<char>> read_stacks(std::ifstream& file) {

        assert(file.is_open());

        //getline() with space separator
        //[<char>]<space>[<char>] etc...
        // 1   2   3   4   5   6   7   8   9  Stack numbers
        // <blank line>
        // <move data starts>

        constexpr auto open_bracket{ '[' };
        std::vector<std::vector<char>> stacks;

        for (std::string line; std::getline(file, line); ) {

            if (open_bracket != line[0]) { break; }

            const auto containers{ process_line(line) };
            stacks.push_back(containers);
        }
        assert(stack_count == stacks.front().size());
        assert(max_stack_height == stacks.size());

        return(stacks);
    }
}
namespace Instructions {
    namespace Text {
        const std::string move{ "move" };
        const std::string from{ "from" };
        const std::string to{ "to" };
    }
    struct Move {
        int crate_count{ 0 };
        int source{ 0 };
        int destination{ 0 };
    };
#ifdef _DEBUG
    void output_move(const Instructions::Move& move) {

        namespace Text = Instructions::Text;
        std::cout
            << Text::move << " " << std::dec << move.crate_count << " "
            << Text::from << " " << std::dec << move.source << " "
            << Text::to << " " << std::dec << move.destination << "\n";
    }
#endif
    [[nodiscard]] int int_from_string(const std::string& the_string) {

        assert(!the_string.empty());

        // TODO: Figure out your own cross-platform version.
        int the_number{ 0 };
        std::stringstream string_stream;
        string_stream << the_string;
        ::SetLastError(S_OK);
        string_stream >> the_number;
        if(FAILED(::GetLastError())) { throw(std::exception("Convert string to int")); }
        
        return(the_number);
    }
    [[nodiscard]] Move read_move(const std::string& line) {

        assert(!line.empty());

        Move move;
        constexpr auto space{ 1 };
        size_t line_pos{ Text::move.size() + space }; // Move position to the stack count.
        {
            std::string crate_count;
            for (; ' ' != line[line_pos] || line.size() < line_pos; crate_count += line[line_pos], ++line_pos) {}
            move.crate_count = int_from_string(crate_count);
        }
        line_pos += (space + Text::from.size() + space); // Move to the source number.
        {
            std::string source;
            for (; ' ' != line[line_pos] || line.size() < line_pos; source += line[line_pos], ++line_pos) {}
            move.source = int_from_string(source);
        }
        line_pos += (space + Text::to.size() + space); // Move to the destination number.
        {
            std::string destination;
            for (; ' ' != line[line_pos] && line.size() > line_pos; destination += line[line_pos], ++line_pos) {}
            move.destination = int_from_string(destination);
        }

        return(move);
    }
    [[nodiscard]] std::vector<Move> read_moves(std::ifstream& file) {

    assert(file.is_open());

    //move <N-NN> from <N> to <N>
    std::vector<Move> moves;

    for (std::string line; std::getline(file, line); ) {

        if (line.npos == line.find(Text::move)) { continue; }

#ifdef _DEBUG
        output_move(read_move(line));
#endif
        moves.push_back(read_move(line));
    }

        return(moves);
    }
}




constexpr auto input_path{ ".\\input.txt" };
int main() try {
    
    // Crates moved one at a time.
    // Which crate ends up on top of each stack?

    CInputFile input_file(input_path);

    const auto stacks{ Stacks::read_stacks(input_file.file) };

#ifdef _DEBUG
    Stacks::output_stacks(stacks);
#endif


    const auto moves{ Instructions::read_moves(input_file.file) };



    return(EXIT_SUCCESS);
}
catch (const std::exception& exception) {


    std::cout << "Exception: " << exception.what() << "\n";
    return(EXIT_FAILURE);
}


