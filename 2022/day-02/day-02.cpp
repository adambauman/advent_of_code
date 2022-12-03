
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
    namespace Response {
        namespace Shape {
            constexpr auto rock     { 'X' };
            constexpr auto paper    { 'Y' };
            constexpr auto scissors { 'Z' };
        }
        namespace Result {
            constexpr auto loss { 'X' };
            constexpr auto draw { 'Y' };
            constexpr auto win  { 'Z' };
        }
    }

}

namespace ByPlay {
    inline int get_rock_result(const char your_response) {
        switch (your_response) {
        case Guide::Response::Shape::paper:
            return(Score::Shape::paper + Score::Result::win);
        case Guide::Response::Shape::scissors:
            return(Score::Shape::scissors + Score::Result::loss);
        case Guide::Response::Shape::rock:
            return(Score::Shape::rock + Score::Result::draw);
        default:
            throw(std::exception("Get rock play score"));
        }
    }
    inline int get_paper_result(const char your_response) {
        switch (your_response) {
        case Guide::Response::Shape::scissors:
            return(Score::Shape::scissors + Score::Result::win);
        case Guide::Response::Shape::rock:
            return(Score::Shape::rock + Score::Result::loss);
        case Guide::Response::Shape::paper:
            return(Score::Shape::paper + Score::Result::draw);
        default:
            throw(std::exception("Get paper play score"));
        }
    }
    inline int get_scissors_result(const char your_response) {
        switch (your_response) {
        case Guide::Response::Shape::rock:
            return(Score::Shape::rock + Score::Result::win);
        case Guide::Response::Shape::paper:
            return(Score::Shape::paper + Score::Result::loss);
        case Guide::Response::Shape::scissors:
            return(Score::Shape::scissors + Score::Result::draw);
        default:
            throw(std::exception("Get scissors play score"));
        }
    }

} // namespace PartOne
namespace ByResult {
    inline int get_rock_result(const char desired_result) {
        switch (desired_result) {
        case Guide::Response::Result::draw:
            return(Score::Shape::rock + Score::Result::draw);
        case Guide::Response::Result::loss:
            return(Score::Shape::scissors + Score::Result::loss);
        case Guide::Response::Result::win:
            return(Score::Shape::paper + Score::Result::win);
        default:
            throw(std::exception("Get rock result score"));
        }
    }
    inline int get_paper_result(const char desired_result) {
        switch (desired_result) {
        case Guide::Response::Result::draw:
            return(Score::Shape::paper + Score::Result::draw);
        case Guide::Response::Result::loss:
            return(Score::Shape::rock + Score::Result::loss);
        case Guide::Response::Result::win:
            return(Score::Shape::scissors + Score::Result::win);
        default:
            throw(std::exception("Get paper result score"));
        }
    }
    inline int get_scissors_result(const char desired_result) {
        switch (desired_result) {
        case Guide::Response::Result::draw:
            return(Score::Shape::scissors + Score::Result::draw);
        case Guide::Response::Result::loss:
            return(Score::Shape::paper + Score::Result::loss);
        case Guide::Response::Result::win:
            return(Score::Shape::rock + Score::Result::win);
        default:
            throw(std::exception("Get scissors result score"));
        }
    }
} // namespace PartTwo

namespace {

    constexpr auto expected_line_char_count{ 3 }; // <ShapeChar><Space><ResponseChar>

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

    struct Scores {
        int by_play{ 0 };
        int by_result{ 0 };
    };
    Scores calculate_total_scores(std::ifstream& input_file) {

        assert(input_file.is_open());

        // First character is the opponent's play, the second is the response you should throw.
        constexpr auto opponent_shape_pos{ 0 };
        constexpr auto guide_recommendation_pos{ 2 };
        Scores scores;
        for (std::string line; std::getline(input_file, line);) {

            if (expected_line_char_count > line.size()) { throw(std::exception("Check character count")); }

            switch (line[opponent_shape_pos]) {
            case Guide::OpponentShape::rock:
                scores.by_play += ByPlay::get_rock_result(line[guide_recommendation_pos]);
                scores.by_result += ByResult::get_rock_result(line[guide_recommendation_pos]);
                break;
            case Guide::OpponentShape::paper:
                scores.by_play += ByPlay::get_paper_result(line[guide_recommendation_pos]);
                scores.by_result += ByResult::get_paper_result(line[guide_recommendation_pos]);
                break;
            case Guide::OpponentShape::scissors:
                scores.by_play += ByPlay::get_scissors_result(line[guide_recommendation_pos]);
                scores.by_result += ByResult::get_scissors_result(line[guide_recommendation_pos]);
                break;
            default:
                throw(std::exception("Get score for shape"));
            }

        }
        return(scores);
    }
}

constexpr auto input_file_path{ ".\\input.txt" };
//constexpr auto input_file_path{ ".\\example.txt" };
int main() try {

    std::cout << "Opening input file...\n";

    CInputFile input_file(input_file_path);

    std::cout << "Calculating Scores...\n";
    const auto scores{ calculate_total_scores(input_file.file) };
    std::cout << "By Play Score: " << std::dec << scores.by_play << "\n\n";

    std::cout << "Calculating Part Two...\n";
    std::cout << "By Result Score: " << std::dec << scores.by_result << "\n\n";

    return(EXIT_SUCCESS);
}
catch (const std::exception& exception) {

    std::cout << "Exception: " << exception.what() << "\n";
    return(EXIT_FAILURE);
}
