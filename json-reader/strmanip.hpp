#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <bit>

namespace json {
	
	namespace encodings {
		using utf8 = uint8_t;
		using utf16 = uint16_t;
		using utf32 = uint32_t;
	}
	
	using codepoint = uint64_t;

	// Control/basic whitespaces found in human json formatting
	template<typename Encodings = encodings::utf8>
	constexpr static std::array<Encodings> wschars = {
		' ', '\t', '\n', '\r', '\f', '\v'
	};
	
	template<typename Encoding = encodings::utf8, typename Endian = std::endian::native, typename... Characters>
	codepoint get_codepoint(Characters... chars) {
		// Get unicode codepoint from list of chars depending on endian-ness and encoding
	}

	// Removes whitespace from string range
	void rmws(const std::string::iterator& begin, const std::string::iterator& end) {
		// iterate from begin to end and remove ws
		for (const auto& ws : wschars) {
			// how to manip string from here? 
			s.erase(std::remove(s.begin(), s.end(), ws), s.end());
		}
	}
}