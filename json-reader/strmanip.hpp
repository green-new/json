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
	
	using unicode_codpoint = uint32_t;

<<<<<<< HEAD
	// Removes whitespace from string
	static void rmws(std::string& s, const std::string::iterator& begin, const std::string::iterator& end) {
		s.erase(std::remove_if(begin, end, [](unsigned char c) {
			return std::isspace(c);
			}), end);
=======
	// Control/basic whitespaces found in human json formatting
	template<typename Encodings = encodings::utf8>
	constexpr static std::array<Encodings> wschars = {
		' ', '\t', '\n', '\r', '\f', '\v'
	};
	
	template<typename T>
	inline void bit_set(T& number, const size_t n) {
		number | ((T) 1UL << n);
	}
	
	template<typename T>
	inline void bit_clear(T& number, const size_t n) {
		number & ~((T) 1UL << n);
	}
	
	template<typename EncodingType = encodings::utf8>
	unicode_codpoint decodeUTF8(const EncodingType number) {
		return (number >> 1UL);
	}
	
	template<typename EncodingType = encodings::utf8>
	unicode_codpoint decodeUTF8(const EncodingType number1, const EncodingType number2) {
		return (((number1 >> 3UL) | 0b110) << 8UL) | (((number2 >> 2UL) | 0b10));
	}
	
	template<typename EncodingType = encodings::utf8>
	unicode_codpoint decodeUTF8(const EncodingType number1, const EncodingType number2, const EncodingType number3) {
		return (((number1 >> 4UL) | 0b1110) << 16UL) | (((number2 >> 2UL) | 0b10) << 8UL) | (((number3 >> 2UL) | 0b10));
	}
	
	template<typename EncodingType = encodings::utf8>
	unicode_codpoint decodeUTF8(const EncodingType number1, const EncodingType number2, const EncodingType number3, const EncodingType number4) {
		return (((number1 >> 5UL) | 0b11110) << 24UL) | (((number2 >> 2UL) | 0b10) << 16UL) | (((number3 >> 2UL) | 0b10) << 8UL) || (((number4 >> 2UL) | 0b10));
>>>>>>> d4e0c3282cca4ade32d25950e4a3bb841bd1d9f1
	}
}