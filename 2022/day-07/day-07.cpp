// NOTE: This solution is will only work in Win32 environments!

#include <iostream>
#include <sstream>
#include <cassert>
#include <string_view>

#include <atlbase.h>
#include <Windows.h>

#include <FileSystem\FileSystem.h>
#include <String\StringHelpers.h>
#include <VirtualDisk/VirtualDisk.h>


#ifdef _UNICODE
#define tstring wstring
#define tstringstream wstringstream
#else
#define tstring string
#define tstringstream stringstream
#endif

namespace Command {
    constexpr auto prompt{ '$' };

    constexpr auto parent_dir{ ".." };

    constexpr auto cd{ "cd" };
    constexpr auto dir{ "dir" };
    constexpr auto ls{ "ls" };
}


namespace {

    uint64_t calculate_max_allocation(const std::string& input_path) {

        assert(!input_path.empty());
        
        uint64_t max_allocation{ 0 };
        CInputFile input(input_path);
        for (std::string line; std::getline(input.file, line); ) {

            if (!Helpers::Char::is_number(line[0])) { continue; }

            // Example file listing: 101350 gpbswq.njr
            const auto first_space_pos{ line.find_first_of(' ') };
            if (line.npos == first_space_pos) { throw(std::exception("Find space in file listing item")); }

            const auto file_size_string{ line.substr(0, first_space_pos) };
            max_allocation += Helpers::String::to_number<uint64_t>(file_size_string);
        }
        return(max_allocation);
    }

   
}



constexpr auto input_path{ ".\\input.txt" };
constexpr auto vhdx_path{ L"C:\\temp\\working.vhd" };
int main() try {

    //const auto input{ FileSystem::File::read(input_path) };

    const auto max_allocation{ calculate_max_allocation(input_path) };

    std::cout << "Creating working VHD file, max allocation: " << std::dec << max_allocation << "...\n";

    ATL::CHandle vhd_handle(
        VirtualDisk::Vhd::create_vhdx(vhdx_path, max_allocation, VirtualDisk::OverwriteExisting::yes));

    std::cout << "Finished creating working VHD file.\n";


    return(EXIT_SUCCESS);
}
catch (const std::exception& exception) {

    std::cerr << "Exception: " << exception.what() << "\n";
    return(EXIT_FAILURE);
}