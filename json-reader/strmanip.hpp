#pragma once
#include <cstddef>

namespace json {

	namespace grammar {
		namespace ws {
			/**
			 * @brief ' ' space.
			 */
			std::byte space = std::byte{0x20};
			/**
			 * @brief '\t' horizontal tab.
			 */
			std::byte ht = std::byte{0x09};
			/**
			 * @brief '\n' new line.
			 */
			std::byte lfnl = std::byte{0x0a};
			/**
			 * @brief '\r' carriage return.
			 */
			std::byte cr = std::byte{0x0d};
			
			/**
			 * @brief Determines if the provided character @c is a whitespace character.
			 * 
			 * @return True if whitespace, false if not.
			 */
			bool is_ws(net::byte c) {
				return c == space || c == ht || c == lfnl || c == cr;
			}
		}
		enum class control : std::byte {
			/**
			 * @brief '[' left square bracket.
			 */
			begin_array = std::byte{0x5b},
			/**
			 * @brief '{' left curly bracket.
			 */
			begin_object = std::byte{0x7b},
			/**
			 * @brief ']' right square bracket.
			 */
			end_array = std::byte{0x5d},
			/**
			 * @brief '}' right curly bracket.
			 */
			end_object = std::byte{0x7d},
			/**
			 * @brief ':' colon.
			 */
			name_separator = std::byte{0x3a},
			/**
			 * @brief ',' comma.
			 */
			value_separator = std::byte{0x2c}
		};
		enum class parsing : std::byte {
			/**
			 * @brief '\"' double quotation mark.
			 */
			quotation_mark = std::byte{0x22},
			/**
			 * @brief '\' backslash (escape) (must be escaped or is the character escaping the subsequent character).
			 */
			escape = std::byte{0x5c},
			/**
			 * @brief '/' forward slash (must be escaped, not a control character by itself).
			 */
			forward_slash = std::byte{0x2f},
			/**
			 * @brief 'b' backspace (must be escaped, not a control character by itself).
			 */
			backspace = std::byte{0x62},
			/**
			 * @brief 'f' form feed (must be escaped, not a control character by itself).
			 */
			form_feed = std::byte{0x66},
			/**
			 * @brief 'n' line feed (must be escaped, not a control character by itself).
			 */
			line_feed = std::byte{0x6e},
			/**
			 * @brief 'r' carriage return (must be escaped, not a control character by itself).
			 */
			carriage_return = std::byte{0x72},
			/**
			 * @brief 't' tab (must be escaped, not a control character by itself).
			 */
			tab = std::byte{0x74},
			/**
			 * @brief 'u' starts a unicode sequence (must be escaped, not a control character by itself).
			 */
			unicode = std::byte{0x75}
		};
	}
}