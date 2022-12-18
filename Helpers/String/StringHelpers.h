#pragma once

#include <string_view>
#include <sstream>
#include <Windows.h>
#include <comdef.h>

namespace Helpers {
	namespace String {
		template <typename T>
		T to_number(const std::string& the_string) {

			// TODO: Cross platform!
			
			T output{ 0 };
			
			std::stringstream string_stream(the_string);
			::SetLastError(S_OK);
			string_stream >> output;
			const auto result{ ::GetLastError() };
			if (FAILED(result)) { throw(std::exception("Convert string to number")); }

			return(output);
		}
	}
	namespace Char {
		[[nodiscard]] inline bool is_empty_or_whitespace(const char contents) {
			return((' ' == contents || '\0' == contents) ? true : false);
		}

		[[nodiscard]] inline unsigned occurences_in_string(const std::string_view string, const char character) {

			auto count{ 0 };
			for (auto index{ 0 }; index < string.length(); ++index) {
				if (string[index] != character) { continue; }
				++count;
			}
			return(count);
		}
		[[nodiscard]] inline bool is_number(const char character) {
			// ASCII digits: 48-57
			return(character >= 48 && character <= 57);
		}


	} // namespace Char

} // namespace Helpers