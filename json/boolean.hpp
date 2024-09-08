#pragma once

#include <string>
#include "value.hpp"

namespace json {
	/**
	* @class json::boolean
	* @brief Contains a simple true/false value. Cannot contain 'nothing' or 'null'.
	*/
	class boolean final : public json::value {
	public:
		/**
		* @brief Default ctor cannot instantiate an 'empty' boolean type.
		*/
		boolean() = delete;
		boolean(bool val)
			: m_boolean(val) {}
		~boolean() = default;
	public:
		/**
		 * @brief Bool conversion operator.
		 * @return Boolean value.
		 */
		explicit operator bool() const;
	public:
		/**
		 * @brief Set the boolean value.
		 * @param val The value to set it to.
		 */
		void set(bool val);
	public:
		/*
		* @brief Get the string representation.
		* @return "true"/"false".
		*/
		std::string to_string() const override {
			return m_boolean ? "true" : "false";
		}
	protected:
		/*
		* @brief Clone this object.
		* @return Raw pointer deep copy of this object.
		*/
		boolean* clone_impl() const override {
			return new boolean(*this);
		}
		/**
		 * @brief Equals implementation.
		 *
		 * @param rhs The other value.
		 * @return True if the value is equal in type and lexiographically, false otherwise.
		 */
		virtual bool equals(const value* rhs) const override {
			if (const auto rhsbool = dynamic_cast<const boolean*>(rhs)) {
				return rhsbool->m_boolean == m_boolean;
			}
			return false;
		}
	private:
		bool m_boolean;
	};
}
