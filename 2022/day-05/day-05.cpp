#include <iostream>
#include <vector>
#include <fstream>
#include <cassert>
#include <sstream>
#include <Windows.h>
//#include <algorithm>

#include <FileSystem/FileSystem.h>
#include <String/StringHelpers.h>

//TODO: Parse out the stack count and max stack height.
constexpr auto stack_count{ 9 };
constexpr auto initial_stack_height{ 8 };


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
    };
#ifdef _DEBUG
    void output_move_to_console(const Instructions::Move& move) {

        namespace Text = Instructions::Text;
        // Don't forget to bump source and destination up by one. In text they're 1-based!
        std::cout
            << Text::move << " " << std::dec << move.crate_count << " "
            << Text::from << " " << std::dec << move.source_stack + 1 << " "
            << Text::to << " " << std::dec << move.dest_stack + 1 << "\n";
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
            move.source_stack = int_from_string(source) - 1; // File is 1-based, rollback 1 to make 0-based.
        }
        line_pos += (space + Text::to.size() + space); // Move to the destination number.
        {
            std::string destination;
            for (; 
                !Char::is_empty_or_whitespace(line[line_pos]) && line.size() > line_pos; 
                destination += line[line_pos], ++line_pos) {}
            move.dest_stack = int_from_string(destination) - 1; // File is 1-based, rollback 1 to make 0-based.
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
        //output_move_to_console(read_move(line));
#endif
        moves.push_back(read_move(line));
    }

        return(moves);
    }
}
namespace Calculate {
    int get_top_crate_y(const std::vector<std::vector<char>>& stacks, const int source_stack) {

        //.....
        //.....
        //S....
        //L...B
        //F.S.Z
        //ZRN.R
        namespace Char = Helpers::String::Char;
        for (auto index{ 0 }; stacks.size() > index; ++index) {

            const auto& stack_row{ stacks[index] };
            if (Char::is_empty_or_whitespace(stack_row[source_stack])) { continue; }

            return(index);
        }
    }
    void process_move(std::vector<std::vector<char>>& stacks, const Instructions::Move &move) {

        Instructions::output_move_to_console(move);
        if (8 == move.crate_count && 1 == move.source_stack && 7 == move.dest_stack) {

            std::cout << "Gotcha\n";
        }
        for (auto crates_moved{ 0 }; move.crate_count > crates_moved; ++crates_moved) {

            auto source_y{ get_top_crate_y(stacks, move.source_stack) };
            auto& source_value{ stacks[source_y][move.source_stack] };

            // Back it up one to target open space above.
            const auto dest_y{ get_top_crate_y(stacks, move.dest_stack) - 1 }; 

            stacks[dest_y][move.dest_stack] = source_value;
            source_value = '\0';
        }
    }
    std::vector<std::vector<char>> crate_positions(
        const std::vector<Instructions::Move>& moves,
        std::vector<std::vector<char>> stacks) {

        assert(!moves.empty());
        assert(!stacks.empty());

        for (const auto& move : moves) {
            process_move(stacks, move);

            //Stacks::output_stacks_to_console(stacks);
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
    const auto post_crane_stacks{ Calculate::crate_positions(moves, stacks) };

    Stacks::output_stacks_to_console(post_crane_stacks);

    return(EXIT_SUCCESS);
}
catch (const std::exception& exception) {

    std::cerr << "Exception: " << exception.what() << "\n";
    return(EXIT_FAILURE);
}


