#pragma once
#include <iostream>
#include "json.hpp"

namespace json {
	
	// Class owns stream object, client creates stream object.
	class reader {
	public:
		reader(std::istream&& source)
			: m_reader(source) { }
		reader(const reader& other) = delete;
		reader(reader&& other) noexcept = default;
		operator=(const reader& other) = delete;
		operator=(reader&& other) noexcept = default;
		~reader() = default;
	public:
		std::unique_ptr<json::object> read() {
			std::unique_ptr<json::object> res = *std::make_unique<json::object>();
			
			return std::move(res);
		}
	private:
		std::istream m_source;
	};
}