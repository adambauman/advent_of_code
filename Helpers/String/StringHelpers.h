#pragma once

namespace Helpers {
	namespace String {
		namespace Char {
			[[nodiscard]] inline bool is_empty_or_whitespace(const char contents) {
				return((' ' == contents || '\0' == contents) ? true : false);
			}
		}
	}
}