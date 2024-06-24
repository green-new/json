#pragma once
#include <vector>
#include <string>
#include <algorithm>

namespace json {

	enum class charset {
		utf8,
		utf16,
		utf32,
		iso88591,
		windows21251,
		windows1252
	};

	static const std::vector<uint8_t> wschars = std::vector<uint8_t>{
		' ', '\t', '\n', '\r', '\f', '\v'
	};

	// Removes whitespace from string
	static void rmws(std::string& s) {
		for (const auto& ws : wschars) {
			s.erase(std::remove(s.begin(), s.end(), ws), s.end());
		}
	}
}