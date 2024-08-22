#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <stack>
#include <exception>
#include "strmanip.hpp"
#include "json.hpp"

namespace json {
	class reader {
	public:
		reader(const std::string& json)
			: m_json(json) { }
		reader(const reader& other) = delete;
		reader(reader&& other) noexcept = default;
		reader& operator=(const reader& other) = delete;
		~reader() = default;
	private:
		/** @brief Converts all escaped characters and unicode sequences into 
		 * their byte representation. Modifies the string. Follows the UTF-8
		 * encoding.
		 */
		void byteify() {
			// Basic control characters 
			size_t i;
			while (i = m_json.find("\\\"" != std::string::npos) {
				m_json.replace(i, 2, "\"");
			}
			i = 0;
			while (i = m_json.find("\\\\") != std::string::npos) {
				m_json.replace(i, 2, "\\");
			}
			i = 0;
			while (i = m_json.find("\\b") != std::string::npos) {
				m_json.replace(i, 2, json::grammar::bs);
			}
			i = 0;
			while (i = m_json.find("\\f") != std::string::npos) {
				m_json.replace(i, 2, json::grammar::ff);
			}
			i = 0;
			while (i = m_json.find("\\n") != std::string::npos) {
				m_json.replace(i, 2, json::grammar::nl);
			}
			i = 0;
			while (i = m_json.find("\\r") != std::string::npos) {
				m_json.replace(i, 2, json::grammar::cr);
			}
			i = 0;
			while (i = m_json.find("\\t") != std::string::npos) {
				m_json.replace(i, 2, json::grammar::ht);
			}
			i = 0;
			// Unicode control characters
			// Assumes escaped unicode codepoints are 4 hex digits
			constexpr static size_t codepoint_len = 4;
			using code_type = std::conditional<codepoint_len <= 4, short, int>;
			constexpr static size_t byte_len = sizeof code_type;
			while (i = m_json.find("\\u") != std::string::npos) {
				code_type code = (code_type) std::stoi(m_json.substr(i, i + codepoint_len), nullptr, 16);
				char bytes[byte_len] = {0};
				for (size_t i = 0; i < byte_len; i++) {
					bytes[i] = (code >> ((sizeof byte_len - 1 - i) * 8)) & 0xff;
				}					
				m_json.replace(i, 2 + codepoint_len, bytes, codepoint_len);
			}
		}
		/** @brief Checks to see if the json string starts with an object.
		 * @throws std::runtime_error
		 */
		void checkRoot() {
			std::string::const_iterator begin = m_json.cbegin();
			std::string::const_iterator end = m_json.cend();
			size_t nlCount = 0;
			size_t charCount = 0;
			bool parsingRoot = false;
			for (auto& it = begin; it != end; it++) {
				charCount++;
				char c = (char) *it;
				if (json::grammar::is_nl(c)) {
					nlCount++;
					charCount = 0;
				}
				if (json::grammar::is_ws(c)) {
					continue;
				}
				if (!parsingRoot && c != json::grammar::begin_object) {
					throw std::runtime_error(std::format("On line '%d', character '%d', expected '{' to declare beginning of JSON root element, found '%c'", nlCount, c));
				}
				if (parsingRoot && c == json::grammar::end_object) {
					parsingRoot = false;
					break;
				}
				// 'c' must be a '{'.
				parsingRoot = true;
			}
			if (parsingRoot) {
				throw std::runtime_error(std::format("On line '%d', character '%d', expected '}' to terminate JSON root object, could not find such character", nlCount, charCount));
			}
		}
		/** @brief Checks to see if objects, strings, and arrays are correctly bounded by their bracket type (e.g., [], {}, "").
		 * @throws std::runtime_error
		 */
		void checkGroups() {
			std::string::const_iterator begin = m_json.cbegin();
			std::string::const_iterator end = m_json.cend();
			size_t nlCount = 0;
			size_t charCount = 0;
			// Parse through each char.
			// Must skip escaped quotation mark characters.
			bool escaping = false;
			const std::string fmt = "On line '%d', character '%d', expected '%c', but found '%c'";
			std::stack<char> stk{};
			for (auto& it = begin; it != end; it++) {
				charCount++;
				char c = (char) *it;
				if (json::grammar::is_nl(c)) {
					nlCount++;
					charCount = 0;
				}
				if (escaping && c == json::grammar::escape) {
					escaping = false;
					continue;
				}
				if (c == json::grammar::escape) {
					escaping = true;
					continue;
				}
				if (json::grammar::is_ws(c)
				|| (c != json::grammar::begin_object
				&& c != json::grammar::begin_array
				&& c != json::grammar::quotation_mark)) {
					continue;
				}
				if (escaping && c == json::grammar::quotation_mark) {
					escaping = false;
					continue;
				}
				const char begin = stk.top();
				// Push a valid opening char to the stack.
				if (c == json::grammar::begin_object
				|| c == json::grammar::begin_array
				|| c == json::grammar::quotation_mark) {
					stk.push(c);
				} else if (!escaping
				&& !stk.empty()
				&& c== json::grammar::closing_groups[c]) {
					stk.pop();
				} else {	
					// We know the grouping is invalid at this point
					char expect = stk.empty()? json::grammar::closing_groups[stk.top()] : '?';
					throw std::runtime_error(std::format(fmt, nlCount, charCount, expect, c));
				}
			}
			if (!stk.empty()) {
				throw std::runtime_error(std::format("On indeterminate line '?', indeterminate c '?', expected valid bounds ('{ }', '[ ]', '\" \"') to indeterminate JSON value, but could not identify which"));
			}
		}
		/** @brief Checks to see if name separators (':') are used correctly.
		 * @throws std::runtime_error
		 */
		void checkNameSeparators() {
			std::string::const_iterator begin = m_json.cbegin();
			std::string::const_iterator end = m_json.cend();
			size_t nlCount = 0;
			size_t charCount = 0;
			bool parsingString = false;

			for (auto& it = begin; it != end; it++) {
				charCount++;
				char c = (char) *it;
				if (json::grammar::is_nl(c)) {
					nlCount++;
					charCount = 0;
				}
				// At the start of this function, we have two guarantees:
				// - JSON consists of the root object
				// - All groups are closed correctly
				if (!parsingString
				&& (json::grammar::is_ws(c)
				|| c != json::grammar::begin_array)) {
					continue;
				}
				if (c == json::grammar::begin_array) {
					parsingString = true;
					continue;
				}
				// At this point, we are reading an array.
				
			}				
		}
		/** @brief Checks to see if arrays are enumerated correctly with the JSON value separator ','.
		 * @throws std::runtime_error
		 */
		void checkArrays() {
			std::string::const_iterator begin = m_json.cbegin();
			std::string::const_iterator end = m_json.cend();
			size_t nlCount = 0;
			size_t charCount = 0;
			bool escaping = false;
			bool parsingArray = false;
			bool parsingElement = false;
			bool parsingStr = false;
			for (auto& it = begin; it != end; it++) {
				charCount++;
				char c = (char) *it;
				if (json::grammar::is_nl(c)) {
					nlCount++;
					charCount = 0;
				}
				// At the start of this function, we have two guarantees:
				// - JSON consists of the root object
				// - All groups are closed correctly
				// Therefore, we can assume after finding a 'begin_array' we
				// read until there is a 'end_array'.
				// If c is whitespace or not a control c, skip it.
				// [ {,} ]
				if (!parsingArray 
				&& (json::grammar::is_ws(c)
				|| c != json::grammar::begin_array)) {
					continue;
				}
				if (c == json::grammar::begin_array) {
					parsingArray = true;
					continue;
				}
				// At this point, we are reading an array.
				if (c == json::grammar::end_array) {
					parsingArray = false;
					continue;
				}
				if (c == json::grammar::escape) {
					escaping = true;
					continue;
				}
				if (!escaping && c == json::grammar::quotation_mark) {
					parsingStr = !parsingStr;
					continue;
				}
				if (!parsingStr && !parsingElement && !escaping && parsingArray) {
					throw std::runtime_error(std::format("On line '%d', character '%d', whitespace cannot be a JSON array element"), nlCount, charCount);
				}
				if (!parsingStr && parsingElement && !escaping && parsingArray) {
					throw std::runtime_error(std::format("On line '%d', character '%d', cannot declare end of JSON array element with the comma (',') control while ));
				}
			}	
		}
	public:
		/**	@brief Read the json string.
		 *
		 * @return The json root object.
		 */
		json::root read() {
			json::root res{};
			std::string::const_iterator begin = m_json.cbegin();
			std::string::const_iterator end = m_json.cend();
			
			// Byteify phase:
			// Convert escaped characters and unicode characters to bytes
			// Should not throw a json exception
			byteify();
			
			// Validation phase:
			// Determine if the string is valid JSON
			try {
				checkRoot();
				checkGroups();
				checkNameSeparators();
				checkArrays();
			} catch (const std::runtime_error& err) {
				// Let the user handle the error
				throw err;
			}
			// At this point, JSON data is validated
			// Generation phase:
			// Generate the json root object
			
			/**
			* Generate the json here ...
			*/
			
			return res;
		}
	private:
		// @todo Not thread safe if m_json is modified while the reader parses the json.
		// Mark the class as non-threadsafe and maybe offer a threadsafe option by copying the string.
		std::string m_json{};
	};
}