
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>

namespace PassportField {
    constexpr auto birth_year = "byr";
    constexpr auto issue_year = "iyr";
    constexpr auto expiration_year = "eyr";
    constexpr auto height = "hgt";
    constexpr auto hair_color = "hcl";
    constexpr auto eye_color = "ecl";
    constexpr auto passport_id = "pid";
    constexpr auto country_id = "cid";
}

int compare_lower(const std::string& this_string, const std::string& that_string) {
    // Quick check before iterating through equal
    if (this_string.size() != that_string.size()) { return (false); }

    // Rip through both strings and compare each character
    auto compare_char = [](const char this_char, const char that_char) {
        return (std::tolower(this_char) == std::tolower(that_char) ? true : false);
    };
    bool strings_equal = std::equal(
        this_string.begin(), this_string.end(),
        that_string.begin(),
        compare_char
    );
    if (!strings_equal) { return(false); }

    // Yay, made it this far, strings are equal!
    return(true);
}

struct KeyValuePair {
    std::string key;
    std::string value;

    KeyValuePair(const std::string& this_key="", const std::string& this_value="") :
    key(this_key), value(this_value) {}
};

struct PassportData {
    std::vector<KeyValuePair> key_value_pairs;
};

//struct PassportData {
//    std::string birth_year;
//    std::string issue_year;
//    std::string expiration_year;
//    std::string height;
//    std::string hair_color;
//    std::string eye_color;
//    std::string passport_id;
//    std::string country_id;

    //bool is_match(const PassportData& that_passport) {
    //    if (!compare_lower(this->birth_year, that_passport.birth_year)) { return(false); }
    //    if (!compare_lower(this->issue_year, that_passport.issue_year)) { return(false); }
    //    if (!compare_lower(this->expiration_year, that_passport.expiration_year)) { return(false); }
    //    if (!compare_lower(this->height, that_passport.height)) { return(false); }
    //    if (!compare_lower(this->hair_color, that_passport.hair_color)) { return(false); }
    //    if (!compare_lower(this->eye_color, that_passport.eye_color)) { return(false); }
    //    if (!compare_lower(this->passport_id, that_passport.passport_id)) { return(false); }
    //    if (!compare_lower(this->country_id, that_passport.country_id)) { return(false); }

    //    return(true);
    //}

    //bool is_valid() {
    //    if (birth_year.empty()) { return(false); }
    //    if (issue_year.empty()) { return(false); }
    //    if (expiration_year.empty()) { return(false); }
    //    if (height.empty()) { return(false); }
    //    if (hair_color.empty()) { return(false); }
    //    if (eye_color.empty()) { return(false); }
    //    if (passport_id.empty()) { return(false); }
    //    if (country_id.empty()) { return(false); }
    //    
    //    return(true);
    //}
//};

class DataFile {
public:
    DataFile(const std::string& file_name) {
        this->file_stream.open(file_name);
    }
    ~DataFile() {
        this->file_stream.close();
    }
public:
    std::ifstream file_stream;
};
std::vector<KeyValuePair> ParseLine(const std::vector<char>& line_characters) {
    assert(!line_characters.empty());

    constexpr auto key_pair_seperator = ':';
    constexpr auto whitespace_seperator = ' ';

    std::vector<KeyValuePair> key_value_pairs;
    std::vector<char> char_buffer;
    KeyValuePair key_pair_buffer;
    unsigned int line_characters_accessed(0);
    for (char character : line_characters) {

        line_characters_accessed++;

        if (key_pair_seperator == character) {
            // We just finished reading a key value, save it and move to value parsing
            key_pair_buffer.key = std::string(char_buffer.begin(), char_buffer.end());
            char_buffer.clear();
            continue;
        }

        if (whitespace_seperator == character || line_characters.size() == line_characters_accessed) {
            assert(!key_pair_buffer.key.empty());

            // End of current key:value pair, cash out buffer for the value half
            key_pair_buffer.value = std::string(char_buffer.begin(), char_buffer.end());
            char_buffer.clear();
            key_value_pairs.push_back(key_pair_buffer);
            key_pair_buffer = KeyValuePair();
            continue;
        }

        char_buffer.push_back(character);
    }
    assert(!key_value_pairs.empty());

    return(key_value_pairs);
}
std::vector<PassportData> ReadData(const std::string& file_name) {
    assert(!file_name.empty());

    std::cout << "Reading data file...\n";

    std::vector<PassportData> complete_passport_datas;
    auto data_file = DataFile(file_name);
    if (data_file.file_stream.is_open()) {
        char character;
        std::vector<char> char_buffer;
        PassportData passport_data_buffer;
        while (data_file.file_stream >> std::noskipws >> character) {

            if ('\n' == character && char_buffer.empty()) {
                // Signifies end of a data set, close out all current buffers and push
                // finished PassportData on the stack
                complete_passport_datas.push_back(passport_data_buffer);
                passport_data_buffer = PassportData();
                char_buffer.clear();
                continue;
            }

            // Parse line once it's read in, otherwise keep pushing characters onto the buffer
            if ('\n' == character) {
                std::vector<KeyValuePair> line_pairs;
                line_pairs = ParseLine(char_buffer);
                char_buffer.clear();

                std::vector<KeyValuePair>& passport_keypairs(passport_data_buffer.key_value_pairs);
                passport_keypairs.insert(passport_keypairs.begin(), line_pairs.begin(), line_pairs.end());
            }
            else {
                char_buffer.push_back(character);
            }
        }
    }
    assert(!complete_passport_datas.empty());

    std::stringstream string_stream;
    string_stream << "Finished reading data file, parsed " << std::dec << complete_passport_datas.size() << " passports\n";
    std::cout << string_stream.str();
    string_stream.str() = "";
    string_stream.clear();

    return(complete_passport_datas);
}

int main() {

    constexpr auto file_name = "data.txt";

    try {
        std::vector<PassportData>(ReadData(file_name));
    }
    catch (std::exception& exception) {
        std::cout << "Failed to read file: " << exception.what() << "\n";
        return(1);
    };

    std::stringstream string_stream;
    string_stream.str() = "";
    string_stream.clear();

    string_stream << "\n"
        << "+--------------------------------------------+\n"
        << "|  A D V E N T  O F  C O D E     D A Y  0 3  |\n"
        << "+--------------------------------------------+\n"
        << "\n"
        << "Solution A: Process passports and pass off your own ID as legit\n"
        << "Solution B: ???\n"
        << "\n";
    std::cout << string_stream.str();
    string_stream.str() = "";
    string_stream.clear();

    return(0);
}


