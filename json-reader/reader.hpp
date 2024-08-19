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
		 * @return The status of the check.
		 */
		void checkStartsWithRoot() {
			std::string_view::const_iterator begin = m_json.cbegin();
			std::string_view::const_iterator end = m_json.cend();
			size_t nlCount = 0;
			size_t charCount = 0;
			for (const auto& it = begin; it != end; it++) {
				charCount++;
				char character = (char) *it;
				if (json::grammar::is_nl(character)) {
					nlCount++;
					charCount = 0;
				}
				if (json::grammar::is_ws(character)) {
					continue;
				}
				if (character != json::grammar::control::begin_object) {
					throw std::runtime_exception(std::format("On line '%d', character '%d', expected '{' to declare beginning of JSON root element, found '%c'", nlCount, character));
				}
			}
		}
		/** @brief Checks to see if objects, strings, and arrays are correctly bounded by their bracket type (e.g., [], {}, "").
		 * @throws std::runtime_exception
		 * @return The status of the check.
		 */
		void checkGroupsAreClosed() {
			std::string_view::const_iterator begin = m_json.cbegin();
			std::string_view::const_iterator end = m_json.cend();
			size_t nlCount = 0;
			size_t charCount = 0;
			std::stack<char> stk{};
			for (const auto& it = begin; it != end; it++) {
				charCount++;
				char character = (char) *it;
				if (json::grammar::is_nl(character)) {
					nlCount++;
				}
				if (json::grammar::is_ws(character)
				|| character != json::grammar::control::begin_object
				|| character != json::grammar::control::begin_array
				|| character != json::grammar::control::quotation) {
					continue;
				}
				
				if (character == json::grammar::control::begin_object
				|| character == json::grammar::control::begin_array
				|| character == json::grammar::control::quotation) {
					stk.push(character);
				} else if (character == json::grammar::control
				// throw std::runtime_exception(std::format("On line '%d', character '%d', expected '%c' to declare end of JSON object element, found '%c'", nlCount, countChar, stk.top(), character));
			}
			if (!stk.empty()) {
				throw std::runtime_exception(std::format("On indeterminate line '?', indeterminate character '?', expected valid bounds ('{ }', '[ ]', '\" \"') to indeterminate JSON value, but could not identify which"));
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
			} catch (std::runtime_error& err) {
				throw err;
			}
			initialChecks = checkStartsWithRoot();
			if (initialChecks != reader_status::reader_codes::SUCCESS) {
				return res;
			}
			
			// Ignore whitespace if we have not declared a string
			bool ignoreWs = true;
			for (const auto& it = begin; it != end; it++) {
				char character = (char) *it;
				if (ignoreWs && json::grammar::ws::is_ws(character)) {
					continue;
				}
				// 'ignoreWs' is false, or 'ignoreWs' is true AND 'character' is not a whitespace
				switch (character) {
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