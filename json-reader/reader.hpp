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
			: m_json(json), m_stream(json) {
			// Only remove whitespace outside json content (not in strings)
			rmws(m_json);
		}
		reader(const reader& other) = delete;
		reader(reader&& other) noexcept = default;
		reader& operator=(const reader& other) = delete;
		~reader() = default;
	public:
		// Reading a json string can be anything, not just an object
		std::unique_ptr<json::node> read() {
			std::unique_ptr<json::node> res;

			return std::move(res);
		}
	private:
		std::string m_json{};
		std::stringstream m_stream;
	};
}