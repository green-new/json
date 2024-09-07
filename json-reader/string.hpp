#pragma once

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
	public:
		string()
			: m_string() {}
		string(const std::string& str)
			: m_string(str) {}
		~string() = default;
	public:
		std::string& str();
	public:
		/*
		* @brief Get the string representation of this string.
		* 
		* @return The string.
		*/
		virtual std::string to_string() const override {
			return std::format("\"{}\"", m_string);
		}
	protected:
		/*
		* @brief Clone implementation.
		* 
		* @return Copy of this string, raw pointer.
		*/
		virtual string* clone_impl() const override {
			return new string(*this);
		}
		/**
		 * @brief Equals implementation.
		 * 
		 * @param rhs The other value.
		 * @return True if the value is equal in type and lexiographically, false otherwise.
		 */
		virtual bool equals(value* rhs) const override {
			if (auto rhsstr = dynamic_cast<string*>(rhs)) {
				return rhsstr->m_string == m_string;
			}
			return false;
		}
	private:
		std::string m_string{};
	};
}
