#include <format>
#include <string>
#include "value.hpp"
namespace json {
/*
* @class json::string
* @extends json::value
* @brief Contains a string. Strings are arrays of 1-byte (char) values. Strings can be empty.
*/
class string final : public value {
	/**
	* Class methods.
	*/
public:
	/*
	* @brief Default initializes m_string to an empty string "".
	*/
	string()
		: m_string() {}
	string(const std::string& str)
		: m_string(str) {}
	~string() = default;
public:
	std::string& str();
public:
	/*
	* @copydoc json::value::to_string
	*/
	virtual std::string to_string() const override {
		return std::format("\"{}\"", m_string);
	}
protected:
	/*
	* @copydocs json::value::clone_impl
	*/
	virtual string* clone_impl() const override {
		return new string(*this);
	}
private:
	std::string m_string{};
};
}
