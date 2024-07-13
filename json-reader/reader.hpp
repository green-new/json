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
			: m_json(json) {
		}
=======
			: m_json(json), m_stream(json) { }
>>>>>>> 6a636ec4b3d6ac7e2966017cca3875db4e15c7f4
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
	};
}