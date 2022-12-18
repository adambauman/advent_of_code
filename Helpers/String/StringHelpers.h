#pragma once

#include <string_view>

namespace Helpers {
	namespace String {
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
		}
	}
}