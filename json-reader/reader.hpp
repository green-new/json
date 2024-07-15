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
			: m_json(json) { }
		reader(const reader& other) = delete;
		reader(reader&& other) noexcept = default;
		reader& operator=(const reader& other) = delete;
		~reader() = default;
	public:
	private:
		std::string m_json{};
	};
}