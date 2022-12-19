// NOTE: This solution is will only work in Win32 environments!

#include <iostream>
#include <sstream>
#include <cassert>
#include <string_view>
#include <vector>
#include <filesystem>

#include <atlbase.h>
#include <Windows.h>


#include <FileSystem\FileSystem.h>
#include <String\StringHelpers.h>


#ifdef _UNICODE
#define tstring wstring
#define tstringstream wstringstream
#else
#define tstring string
#define tstringstream stringstream
#endif

namespace Args {
    constexpr auto input_path{ ".\\input.txt" };
    constexpr auto working_path{ "C:\\temp\\day7" };
}

namespace Command {

    constexpr auto start_pos{ 2 };
    constexpr auto command_size{ 2 };

    constexpr auto prompt{ '$' };
    constexpr auto dir_specifier{ "dir" };
    constexpr auto parent_dir{ ".." };

    constexpr auto cd{ "cd" };
    constexpr auto ls{ "ls" };

    namespace Dos {
        constexpr auto cd{ "cd" };
        constexpr auto ls{ "dir" };
        constexpr auto mkdir{ "mkdir" };
    }
}


namespace {

    //uint64_t calculate_max_allocation(const std::string& input_path) {

    //    assert(!input_path.empty());
    //    
    //    uint64_t max_allocation{ 0 };
    //    CInputFile input(input_path);
    //    for (std::string line; std::getline(input.file, line); ) {

    //        if (!Helpers::Char::is_number(line[0])) { continue; }

    //        // Example file listing: 101350 gpbswq.njr
    //        const auto first_space_pos{ line.find_first_of(' ') };
    //        if (line.npos == first_space_pos) { throw(std::exception("Find space in file listing item")); }

    //        const auto file_size_string{ line.substr(0, first_space_pos) };
    //        max_allocation += Helpers::String::to_number<uint64_t>(file_size_string);
    //    }
    //    return(max_allocation);
    //}

    bool is_cd_command(const std::string& line) {

        assert(!line.empty());

        const auto command{ line.substr(Command::start_pos, Command::command_size) };

        if (0 == command.compare(Command::cd)) { return(true); }
        return(false);
    }
    void change_directory(std::string& current_directory, const std::string& line) {

        const auto directory{ line.substr(line.find_last_of(' ') + 1, line.length()) };

        if ('/' == directory[0]) { 
            current_directory.clear(); 
        } 
        else if (0 == directory.compare(Command::parent_dir)) {
            current_directory = current_directory.substr(0, current_directory.find_last_of('\\'));
        }
        else {
            current_directory += '\\';
            current_directory += directory;
        }
    }
    void create_directory(const std::string& dir_name, const std::string& current_directory) {

        assert(!dir_name.empty());

        std::string path = Args::working_path;
        if ('\\' != current_directory[0]) { path += '\\'; }
        path += current_directory;
        path += dir_name;

        std::error_code throwaway_code; // Ignoring errors.
        std::filesystem::create_directory(path, throwaway_code);
    }
    void process_dir_items(const std::vector<std::string>& dir_items, const std::string& current_directory) {

        assert(!dir_items.empty());

        constexpr auto dir_command_size{ 3 };
        for (const auto& dir_item : dir_items) {
            const auto dir_command_pos{ dir_item.find(Command::dir_specifier) };
            if (dir_item.npos != dir_item.substr(0, dir_command_size).compare(Command::dir_specifier)) {
                const auto dir_name{ dir_item.substr(dir_command_size + 1) };
                create_directory(dir_name, current_directory);
            }
        }

    }
    std::string create_file_script(const std::string& input_path) {

        assert(!input_path.empty());

        std::string script;

        CInputFile input(input_path);
        std::vector<std::string> dir_items;
        std::string current_directory;
        for (std::string line; std::getline(input.file, line); ) {

            if (Command::prompt != line[0]) { // listing output
                dir_items.push_back(line);
                continue; 
            }

            if (!dir_items.empty()) { // last chunk was listing output
                process_dir_items(dir_items, current_directory);
                dir_items.clear();
            }

            if (is_cd_command(line)) {
                change_directory(current_directory, line);
            }

        }
        assert(!script.empty());

        return(script);
    }

   
}


int main() try {

    //const auto input{ FileSystem::File::read(input_path) };

    //const auto max_allocation{ calculate_max_allocation(input_path) };




    std::cout << "Creating file script...\n";

    const auto file_script{ create_file_script(Args::input_path) };

    std::cout << "Finished creating file script.\n";


    return(EXIT_SUCCESS);
}
catch (const std::exception& exception) {

    std::cerr << "Exception: " << exception.what() << "\n";
    return(EXIT_FAILURE);
}