#pragma once
#include <cstddef>

namespace json {
namespace grammar {
	enum class control : char {
		/**
		* @brief '[' left square bracket.
		*/
		begin_array = 0x5b,
		/**
		* @brief '{' left curly bracket.
		*/
		begin_object = 0x7b,
		/**
		* @brief ']' right square bracket.
		*/
		end_array = 0x5d,
		/**
		* @brief '}' right curly bracket.
		*/
		end_object = 0x7d,
		/**
		* @brief ':' colon.
		*/
		name_separator = 0x3a,
		/**
		* @brief ',' comma.
		*/
		value_separator = 0x2c
	};
	enum class parsing : char {
		/**
		* @brief '\"' double quotation mark.
		*/
		quotation_mark = 0x22,
		/**
		* @brief '\' backslash (escape) (must be escaped or is the character escaping the subsequent character).
		*/
		escape = 0x5c,
		/**
		* @brief '/' forward slash (must be escaped, not a control character by itself).
		*/
		forward_slash = 0x2f,
		/**
		* @brief 'b' backspace (must be escaped, not a control character by itself).
		*/
		backspace = 0x62,
		/**
		* @brief 'f' form feed (must be escaped, not a control character by itself).
		*/
		form_feed = 0x66,
		/**
		* @brief 'n' line feed (must be escaped, not a control character by itself).
		*/
		line_feed = 0x6e,
		/**
		* @brief 'r' carriage return (must be escaped, not a control character by itself).
		*/
		carriage_return = 0x72,
		/**
		* @brief 't' tab (must be escaped, not a control character by itself).
		*/
		tab = 0x74,
		/**
		* @brief 'u' starts a unicode sequence (must be escaped, not a control character by itself).
		*/
		unicode = 0x75
	};
	namespace ws {
		/**
		* @brief ' ' space.
		*/
		char space = 0x20;
		/**
		* @brief '\\t' horizontal tab.
		*/
		char ht = 0x09;
		/**
		* @brief '\\n' new line/line feed.
		*/
		char lfnl = 0x0a;
		/**
		* @brief '\\r' carriage return.
		*/
		char cr = 0x0d;

		/**
		* @brief Determines if the provided character @c is a whitespace character.
		*
		* @return True if whitespace, false if not.
		*/
		bool is_ws(char c) {
			return c == space || c == ht || c == lfnl || c == cr;
		}
	}
}
}