#pragma once
#include <vector>
#include <string>
#include <algorithm>

namespace json {

	// Removes whitespace from string
	static void rmws(std::string& s, const std::string::iterator& begin, const std::string::iterator& end) {
		s.erase(std::remove_if(begin, end, [](unsigned char c) {
			return std::isspace(c);
			}), end);
	}
}