#include <string>

#pragma once

namespace Json {
	class Parser {
	public:
		Parser() { }
		~Parser() { }
	private:
		std::string m_token;
	};
}