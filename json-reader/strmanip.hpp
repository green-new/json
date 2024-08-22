#pragma once
#include <cstddef>

namespace json {
namespace grammar {
	/**
	* @brief '[' left square bracket.
	*/
	char begin_array = 0x5b,
	/**
	* @brief '{' left curly bracket.
	*/
	char begin_object = 0x7b,
	/**
	* @brief ']' right square bracket.
	*/
	char end_array = 0x5d;
	/**
	* @brief '}' right curly bracket.
	*/
	char end_object = 0x7d;
	/**
	* @brief ':' colon.
	*/
	char name_separator = 0x3a;
	/**
	* @brief ',' comma.
	*/
	char value_separator = 0x2c;
	/**
	* @brief '\"' double quotation mark.
	*/
	char quotation_mark = 0x22;
	/**
	* @brief '\' backslash (escape) (must be escaped or is the character escaping the subsequent character).
	*/
	char escape = 0x5c;
	/**
	* @brief '/' forward slash (must be escaped, not a control character by itself).
	*/
	char forward_slash = 0x2f;
	/**
	* @brief 'b' character (must be escaped, not a control character by itself).
	*/
	char backspace = 0x62;
	/**
	* @brief 'f' form feed (must be escaped, not a control character by itself).
	*/
	char form_feed = 0x66;
	/**
	* @brief 'n' line feed (must be escaped, not a control character by itself).
	*/
	char line_feed = 0x6e;
	/**
	* @brief 'r' carriage return (must be escaped, not a control character by itself).
	*/
	char carriage_return = 0x72;
	/**
	* @brief 't' tab (must be escaped, not a control character by itself).
	*/
	char tab = 0x74;
	/**
	* @brief 'u' starts a unicode sequence (must be escaped, not a control character by itself).
	*/
	char unicode = 0x75;
	/**
	* @brief ' ' space.
	*/
	char space = 0x20;
	/**
	* @brief '\\b' backspace.
	*/
	char bs = 0x08;
	/**
	* @brief '\\t' horizontal tab.
	*/
	char ht = 0x09;
	/**
	* @brief '\\n' new line/line feed.
	*/
	char nl = 0x0a;
	/**
	* @brief '\\f' form feed. 
	*/
	char ff = 0x0c;
	/**
	* @brief '\\r' carriage return.
	*/
	char cr = 0x0d;
	/**
	* @brief Determines if the provided character @c is a whitespace character.
	*
	* @return True if whitespace, false if not.
	*/
	constexpr inline bool is_ws(char c) {
		return c == space || c == ht || c == lfnl || c == cr;
	}
	/**
	* @brief Determines if the provided character @c is a newline (carriage return or new line) character.
	*
	* @return True if newline, false if not.
	*/
	constexpr inline bool is_nl(char c) {
		return c == nl || c == cr;
	}
	/**
	 * begin characters -> end characters map.
	 */
	const std::map<char, char> closing_groups = {
		{ begin_object, end_object },
		{ begin_array, end_array },
	};
}
}