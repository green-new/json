#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "strmanip.hpp"
#include "json.hpp"

namespace json {
	class reader {
	public:
		reader(const std::string& json)
			: m_json(json.c_str(), json.size) { }
		reader(const reader& other) = delete;
		reader(reader&& other) noexcept = default;
		reader& operator=(const reader& other) = delete;
		~reader() = default;
	private:
		/** @brief Checks to see if the json string starts with an object.
		 * @throws std::runtime_exception
		 */
		void checkStartsWithRoot() {
			std::string_view::const_iterator begin = m_json.cbegin();
			std::string_view::const_iterator end = m_json.cend();
			size_t nlCount = 0;
			size_t charCount = 0;
			for (const auto& it = begin; it != end; it++) {
				charCount++;
				char c = (char) *it;
				if (json::grammar::is_nl(c)) {
					nlCount++;
					charCount = 0;
				}
				if (json::grammar::is_ws(c)) {
					continue;
				}
				if (c != json::grammar::control::begin_object) {
					throw std::runtime_exception(std::format("On line '%d', character '%d', expected '{' to declare beginning of JSON root element, found '%c'", nlCount, c));
				}
			}
		}
		/** @brief Checks to see if objects, strings, and arrays are correctly bounded by their bracket type (e.g., [], {}, "").
		 * @throws std::runtime_exception
		 */
		void checkGroupsAreClosed() {
			std::string_view::const_iterator begin = m_json.cbegin();
			std::string_view::const_iterator end = m_json.cend();
			size_t nlCount = 0;
			size_t charCount = 0;
			std::stack<char> stk{};
			// Parse through each char.
			bool escaping = false;
			for (const auto& it = begin; it != end; it++) {
				charCount++;
				char c = (char) *it;
				if (json::grammar::is_nl(c)) {
					nlCount++;
					charCount = 0;
				}
				if (escaping && c == json::grammar::control::escape) {
					escaping = false;
					continue;
				}
				if (json::grammar::is_ws(c)
				|| (c != json::grammar::control::begin_object
				&& c != json::grammar::control::begin_array
				&& c != json::grammar::control::quotation_mark) {
					if (c == json::grammar::control::escape) {
						escaping = true;
					}
					continue;
				}
				const std::string fmt = "On line '%d', character '%d', expected '%c', but found '%c'";
				// Push a valid opening char to the stack.
				if (c == json::grammar::control::begin_object
				|| c == json::grammar::control::begin_array
				|| c == json::grammar::control::quotation_mark) {
					stk.push(c);
				} else if (c == json::grammar::control::end_object) {
					// If the top of the stack is not a 'begin_object', then we know the grouping is invalid.
					if (stk.empty() || stk.top() != json::grammar::control::begin_object) {
						char expect = stk.empty()? json::grammar::control::groups[stk.top()]) : '?';
						throw std::runtime_exception(std::format(fmt, nlCount, charCount, expect, c));
					}
					stk.pop();
				} else if (c == json::grammar::control::end_array) {
					if (stk.empty() || stk.top() != json::grammar::control::begin_array) {
						char expect = stk.empty()? json::grammar::control::groups[stk.top()]) : '?';
						throw std::runtime_exception(std::format(fmt, nlCount, charCount, expect, c));
					}
					stk.pop();
				} else if (!escaping && c == json::grammar::control::quotation_mark) {
					if (stk.empty() || stk.top() != json::grammar::control::quotation_mark) {
						char expect = stk.empty()? json::grammar::control::groups[stk.top()]) : '?';
						throw std::runtime_exception(std::format(fmt, nlCount, charCount, expect, c));
					}
					stk.pop();
				}
			}
			if (!stk.empty()) {
				throw std::runtime_exception(std::format("On indeterminate line '?', indeterminate c '?', expected valid bounds ('{ }', '[ ]', '\" \"') to indeterminate JSON value, but could not identify which"));
			}
		}
		/** @brief Checks to see if name separators (':') are used correctly.
		 * @throws std::runtime_exception
		 */
		void checkNameSeparators() {
			std::string_view::const_iterator begin = m_json.cbegin();
			std::string_view::const_iterator end = m_json.cend();
			size_t nlCount = 0;
			size_t charCount = 0;
			bool parsingString = false;
			for (const auto& it = begin; it != end; it++) {
				charCount++;
				char c = (char) *it;
				if (json::grammar::is_nl(c)) {
					nlCount++;
					charCount = 0;
				}
				// At the start of this function, we have two guarantees:
				// - JSON consists of the root object
				// - All groups are closed correctly
				if (!parsingArray 
				&& (json::grammar::is_ws(c)
				|| c != json::grammar::control::begin_array)) {
					continue;
				}
				if (c == json::grammar::control::begin_array) {
					parsingArray = true;
					continue;
				}
				// At this point, we are reading an array.
				
			}				
		}
		/** @brief Checks to see if arrays are enumerated correctly with the JSON value separator ','.
		 * @throws std::runtime_exception
		 */
		void checkArrays() {
			std::string_view::const_iterator begin = m_json.cbegin();
			std::string_view::const_iterator end = m_json.cend();
			size_t nlCount = 0;
			size_t charCount = 0;
			bool parsingArray = false;
			bool parsingElement = false;
			for (const auto& it = begin; it != end; it++) {
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
				if (!parsingArray 
				&& (json::grammar::is_ws(c)
				|| c != json::grammar::control::begin_array)) {
					continue;
				}
				if (c == json::grammar::control::begin_array) {
					parsingArray = true;
					continue;
				}
				// At this point, we are reading an array.
				
			}	
		}
	public:
		/**	@brief Read the json string.
		 *
		 * @return The json root object.
		 */
		json::root read() {
			json::root res{};
			std::string_view::const_iterator begin = m_json.cbegin();
			std::string_view::const_iterator end = m_json.cend();
			
			// Initial checks
			try {
				checkStartsWithRoot();
				checkGroupsAreClosed();
				checkNameSeparators();
				checkArrays();
			} catch (std::runtime_error& err) {
				throw err;
			}
			// At this point, JSON data is correctly validated in the following ways:
			// - Consists of a root object (the global '{ }')
			// - All syntactically important groups are opened and closed correctly
			// - Arrays are syntactically correct
			
			// Ignore whitespace if we have not declared a string
			bool ignoreWs = true;
			for (const auto& it = begin; it != end; it++) {
				char c = (char) *it;
				if (ignoreWs && json::grammar::ws::is_ws(c)) {
					continue;
				}
				// 'ignoreWs' is false, or 'ignoreWs' is true AND 'c' is not a whitespace
				switch (c) {
					case 
				}
			}
			
			return res;
		}
	private:
		// @todo Not thread safe if m_json is modified while the reader parses the json.
		std::string_view m_json{};
	};
}