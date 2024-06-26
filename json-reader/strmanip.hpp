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
	
	using codepoint = uint32_t;

	// Control/basic whitespaces found in human json formatting
	template<typename Encodings = encodings::utf8>
	constexpr static std::array<Encodings> wschars = {
		' ', '\t', '\n', '\r', '\f', '\v'
	};
	
	template<typename T>
	inline void bit_set(T& number, const size_t n) {
		number | ((T) 1 << n);
	}
	
	template<typename T>
	inline void bit_clear(T& number, const size_t n) {
		number & ~((T) 1 << n);
	}
	
	template<typename EncodingType = encodings::utf8>
	codepoint decodeUTF8(const EncodingType number) {
		return (number >> 1);
	}
	
	template<typename EncodingType = encodings::utf8>
	codepoint decodeUTF8(const EncodingType number1, const EncodingType number2) {
		return (((number1 >> 3) | 0b110) << 8) | (((number2 >> 2) | 0b10));
	}
	
	template<typename EncodingType = encodings::utf8>
	codepoint decodeUTF8(const EncodingType number1, const EncodingType number2, const EncodingType number3) {
		return (((number1 >> 4) | 0b1110) << 16) | (((number2 >> 2) | 0b10) << 8) | (((number3 >> 2) | 0b10));
	}
	
	template<typename EncodingType = encodings::utf8>
	codepoint decodeUTF8(const EncodingType number1, const EncodingType number2, const EncodingType number3, const EncodingType number4) {
		return (((number1 >> 5) | 0b11110) << 24) | (((number2 >> 2) | 0b10) << 16) | (((number3 >> 2) | 0b10) << 8) || (((number4 >> 2) | 0b10));
	}
	
	// Get unicode codepoint from list of chars depending on endian-ness and encoding
	template<typename Encoding = encodings::utf8, typename Endian = std::endian::native, typename... Characters>
	codepoint decode(Characters... chars) {
		static_assert(bytes >= 1 && bytes <= 4);
		static_assert(Endian == std::endian::native || Endian == std::endian::big || Endian == std::endian::little);
		static_assert(Encoding == encodings::utf8 || Encoding = encodings::utf16 || Encoding == encodings::utf32);
		constexpr size_t bytes = sizeof...(Characters);
		constexpr size_t bits = sizeof(
		std::array<Characters, bytes> bytes{chars};
		codepoint cpoint{};
		
		// use is_same instead
		if constexpr (Encoding == encodings::utf8) {
			if constexpr (Endian == std::endian::big) {
				if constexpr (size == 1) {
					cpoint = bytes[0];
					bit_clear(cpoint, 0);
				} else if constexpr (size == 2) {
					cpoint |= bytes[0];
					cpoint |= bytes[1] << 8;
					bit_set(cpoint, 0);
					bit_set(cpoint, 1);
					bit_clear(cpoint, 2);
					bit_set(cpoint, 8);
					bit_clear(cpoint, 9);
				} else if constexpr (size == 3) {
					cpoint |= bytes[0];
					cpoint |= bytes[1] << 8;
					cpoint |= bytes[2] << 16;
					bit_set(cpoint, 0);
					bit_set(cpoint, 1);
					bit_set(cpoint, 2);
					bit_clear(cpoint, 3);
					bit_set(cpoint, 8);
					bit_clear(cpoint, 9);
					bit_set(cpoint, 16);
					bit_clear(cpoint, 17);
				} else if constexpr (size == 4) {
					
					cpoint |= bytes[0];
					
				}
			} else if constexpr (Endian == std::endian::little) {
				
			}
		} else if constexpr (Encoding == encodings::utf16) {
			
		} else if constexpr (Encoding == encodings::utf32) {
			
		}
		
		return cpoint;
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