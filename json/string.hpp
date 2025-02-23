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
		/*
		 * @brief Determines if the provided value is less than in terms of content.
		 * Implementation dependent.
		 * @return True if equal, false if not.
		 */
		virtual bool lt_impl(const value* rhs) const override {
			if (const auto rhsstr = dynamic_cast<const string*>(rhs)) {
				return m_string < rhsstr->m_string;
			}
			return false;
		}
		/*
		 * @brief Determines if the provided value is greater than in terms of content.
		 * Implementation dependent.
		 * @return True if equal, false if not.
		 */
		virtual bool gt_impl(const value* rhs) const override {
			if (const auto rhsstr = dynamic_cast<const string*>(rhs)) {
				return m_string > rhsstr->m_string;
			}
			return false;
		}
		/**
		 * @brief Equals implementation.
		 *
		 * @param rhs The other value.
		 * @return True if the value is equal in type and lexiographically, false otherwise.
		 */
		virtual bool eq_impl(const value* rhs) const override {
			if (const auto rhsstr = dynamic_cast<const string*>(rhs)) {
				return rhsstr->m_string == m_string;
			}
			return false;
		}
	private:
		std::string m_string{};
	};
}
