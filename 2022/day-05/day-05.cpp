#include <iostream>
#include <vector>
#include <fstream>
#include <cassert>
#include <sstream>
#include <Windows.h>

#include <FileSystem/FileSystem.h>
#include <String/StringHelpers.h>

//TODO: Parse out the stack count and max stack height.
constexpr auto stack_count{ 9 };
constexpr auto initial_stack_height{ 8 };

// Forwards
namespace Instructions {

}

namespace Stacks {
    void output_stacks_to_console(const std::vector<std::vector<char>>& stacks) {

        namespace Char = Helpers::String::Char;
        std::cout
            << "Stack Output:\n"
            << "------------------------------------------\n";
        for (const auto stack : stacks) {
            for (const auto container : stack) {
                std::cout << (Char::is_empty_or_whitespace(container) ? '.' : container);
            }
            std::cout << "\n";
        }
        std::cout
            << "------------------------------------------\n";
    }
    [[nodiscard]] std::vector<char> process_line(const std::string& line) {

        assert(!line.empty());

        //TODO: Proper stack parsing, this will break if stack schema changes.
        std::vector<char> crates;
        for (auto index{ 1 }; line.size() >= index; index += 4) {

            crates.push_back(line[index]);
        }
        return(crates);
    }
    [[nodiscard]] std::vector<std::vector<char>> read_stacks(std::ifstream& file) {

        assert(file.is_open());

        //getline() with space separator
        //[<char>]<space>[<char>] etc...
        // 1   2   3   4   5   6   7   8   9  Stack numbers
        // <blank line>
        // <move data starts>

        // Pad the the top of stack storage to account for possible moves that 
        // place all crates in a single stack.
        const auto max_stack_size{ initial_stack_height * stack_count };
        std::vector<std::vector<char>> stacks(max_stack_size, std::vector<char>(stack_count, '\0'));

        constexpr auto open_bracket{ '[' };
        file.clear();
        file.seekg(file.beg);
        for (std::string line; std::getline(file, line); ) {

            if (open_bracket != line[0]) { break; }

            const auto crates{ process_line(line) };
            stacks.push_back(crates);
        }
        assert(stack_count == stacks.front().size());

#ifdef _DEBUG
        Stacks::output_stacks_to_console(stacks);
#endif

        return(stacks);
    }
}
namespace Instructions {
    namespace Text {
        const std::string move { "move" };
        const std::string from { "from" };
        const std::string to   { "to" };
    }
    struct Move {
        int crate_count  { 0 };
        int source_stack { 0 };
        int dest_stack   { 0 };
        
    public:
        [[nodiscard]] inline bool is_valid() const {
            return(0 != crate_count && 0 != source_stack && 0 != dest_stack);
        }
    };
#ifdef _DEBUG
    void output_move_to_console(const Instructions::Move& move) {

        namespace Text = Instructions::Text;
        std::cout
            << Text::move << " " << std::dec << move.crate_count << " "
            << Text::from << " " << std::dec << move.source_stack << " "
            << Text::to << " " << std::dec << move.dest_stack << "\n";
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
        namespace Char = Helpers::String::Char;
        constexpr auto space{ 1 };
        size_t line_pos{ Text::move.size() + space }; // Move position to the stack count.
        {
            std::string crate_count;
            for (; 
                !Char::is_empty_or_whitespace(line[line_pos]) || line.size() < line_pos; 
                crate_count += line[line_pos], ++line_pos) {}
            move.crate_count = int_from_string(crate_count);
        }
        line_pos += (space + Text::from.size() + space); // Move to the source number.
        {
            std::string source;
            for (; 
                !Char::is_empty_or_whitespace(line[line_pos]) || line.size() < line_pos; 
                source += line[line_pos], ++line_pos) {}
            move.source_stack = int_from_string(source);
        }
        line_pos += (space + Text::to.size() + space); // Move to the destination number.
        {
            std::string destination;
            for (; 
                !Char::is_empty_or_whitespace(line[line_pos]) && line.size() > line_pos; 
                destination += line[line_pos], ++line_pos) {}
            move.dest_stack = int_from_string(destination);
        }

        return(move);
    }
    [[nodiscard]] std::vector<Move> read_moves(std::ifstream& file) {

    assert(file.is_open());

    //move <N-NN> from <N> to <N>
    std::vector<Move> moves;

    file.clear();
    file.seekg(file.beg);
    for (std::string line; std::getline(file, line); ) {

        if (line.npos == line.find(Text::move)) { continue; }

#ifdef _DEBUG
        output_move_to_console(read_move(line));
#endif
        moves.push_back(read_move(line));
    }

        return(moves);
    }
}
namespace Calculate {
    int get_top_crate_position(const std::vector<std::vector<char>>& stacks, const int stack_index) {

        assert(stack_index <= stack_count);

        namespace Char = Helpers::String::Char;
        auto& stack{ stacks[stack_index] };
        for (auto crate_index{ 0 }; stack.size() > crate_index; ++crate_index) {

            if (Char::is_empty_or_whitespace(stack[crate_index])) { continue; }

            return(crate_index);
        }
    }
    void process_move(std::vector<std::vector<char>>& stacks, const Instructions::Move &move) {

        assert(move.is_valid());

        for (auto crates_moved{ 0 }; move.crate_count > crates_moved; ++crates_moved) {
            const auto top_crate_pos{ get_top_crate_position(stacks, move.source_stack) };

        }



    }
    std::vector<std::vector<char>> crate_positions(
        const std::vector<Instructions::Move>& moves,
        std::vector<std::vector<char>> stacks) {

        assert(!moves.empty());
        assert(!stacks.empty());

        for (const auto& move : moves) {
            process_move(stacks, move);
        }

        return(stacks);
    }
}



constexpr auto input_path{ ".\\input.txt" };
int main() try {
    
    // Crates moved one at a time.
    // Which crate ends up on top of each stack?

    CInputFile input_file(input_path);

    std::cout << "Reading moves...\n";
    const auto moves{ Instructions::read_moves(input_file.file) };

    std::cout << "Reading stacks...\n";
    auto stacks{ Stacks::read_stacks(input_file.file) };

    std::cout << "Calculation crate positions...\n";
    //const auto post_crane_stacks{ Calculate::crate_positions(moves, stacks) };
    //assert(stacks.size() == post_crane_stacks.size());

    //Stacks::output_stacks_to_console(post_crane_stacks);

    return(EXIT_SUCCESS);
}
catch (const std::exception& exception) {

    std::cerr << "Exception: " << exception.what() << "\n";
    return(EXIT_FAILURE);
}


