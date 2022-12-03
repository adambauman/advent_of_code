
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>

#ifdef _DEBUG
constexpr auto debug{ true };
#else
constexpr auto debug{ false };
#endif

namespace Score {
    namespace Shape {
        constexpr auto rock     { 1 };
        constexpr auto paper    { 2 };
        constexpr auto scissors { 3 };
    }
    namespace Result {
        constexpr auto loss { 0 };
        constexpr auto draw { 3 };
        constexpr auto win  { 6 };
    }
}
namespace Guide {
    namespace OpponentShape {
        constexpr auto rock     { 'A' };
        constexpr auto paper    { 'B' };
        constexpr auto scissors { 'C' };
    }
    namespace YourResponse {
        constexpr auto rock     { 'X' };
        constexpr auto paper    { 'Y' };
        constexpr auto scissors { 'Z' };
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

    inline int get_rock_result(const char your_response) {
        switch (your_response) {
        case Guide::YourResponse::paper:
            return(Score::Shape::paper + Score::Result::win);
        case Guide::YourResponse::scissors:
            return(Score::Shape::scissors + Score::Result::loss);
        case Guide::YourResponse::rock:
            return(Score::Shape::rock + Score::Result::draw);
        default:
            throw(std::exception("Get rock score"));
        }
    }
    inline int get_paper_result(const char your_response) {
        switch (your_response) {
        case Guide::YourResponse::scissors:
            return(Score::Shape::scissors + Score::Result::win);
        case Guide::YourResponse::rock:
            return(Score::Shape::rock + Score::Result::loss);
        case Guide::YourResponse::paper:
            return(Score::Shape::paper + Score::Result::draw);
        default:
            throw(std::exception("Get paper score"));
        }
    }
    inline int get_scissors_result(const char your_response) {
        switch (your_response) {
        case Guide::YourResponse::rock:
            return(Score::Shape::rock + Score::Result::win);
        case Guide::YourResponse::paper:
            return(Score::Shape::paper + Score::Result::loss);
        case Guide::YourResponse::scissors:
            return(Score::Shape::scissors + Score::Result::draw);
        default:
            throw(std::exception("Get scissors score"));
        }
    }
    int calculate_total_score(std::ifstream& input_file) {
        
        assert(input_file.is_open());

        constexpr auto expected_line_char_count { 3 }; // <ShapeChar><Space><ResponseChar>
        constexpr auto opponent_shape_pos       { 0 };
        constexpr auto response_shape_pos       { 2 };

        auto total_score{ 0 };
        for (std::string line; std::getline(input_file, line);) {

            if (expected_line_char_count > line.size()) { throw(std::exception("Check character count")); }

            switch (line[opponent_shape_pos]) {
            case Guide::OpponentShape::rock:
                total_score += get_rock_result(line[response_shape_pos]);
                break;
            case Guide::OpponentShape::paper:
                total_score += get_paper_result(line[response_shape_pos]);
                break;
            case Guide::OpponentShape::scissors:
                total_score += get_scissors_result(line[response_shape_pos]);
                break;
            default:
                throw(std::exception("Get score for shape"));
            }

        }

        return(total_score);
    }
}

constexpr auto input_file_path{ ".\\input.txt" };
//constexpr auto input_file_path{ ".\\example.txt" };
int main() try {

    std::cout << "Opening input file...\n";

    CInputFile input_file(input_file_path);

    std::cout << "Calculating...\n";

    const auto score{ calculate_total_score(input_file.file) };

    std::cout << "Score: " << std::dec << score << "\n";

    return(EXIT_SUCCESS);
}
catch (const std::exception& exception) {

    std::cout << "Exception: " << exception.what() << "\n";
    return(EXIT_FAILURE);
}
