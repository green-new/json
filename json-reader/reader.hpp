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
<<<<<<< HEAD
			: m_json(json), m_stream(json) { }
=======
			: m_json(json), m_stream(json) {
			// Only remove whitespace outside json content (not in strings)
			rmws(m_json);
		}
>>>>>>> d4e0c3282cca4ade32d25950e4a3bb841bd1d9f1
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