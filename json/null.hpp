#pragma once

#include <string>
#include "value.hpp"

namespace json {
	/**
	* @class json::null
	* @brief JSON class representing a null value.
	*/
	class null final : public value {
	public:
		null() = default;
		~null() = default;
	public:
		/**
		 * @brief Get the string representation.
		 * @return "null".
		 */
		std::string to_string() const override {
			return "null";
		}
	protected:
		/**
		 * @brief Clone this null object.
		 *
		 * @return Raw pointer deep copy of this object.
		 */
		virtual null* clone_impl() const override {
			return new null(*this);
		}
		/**
		 * @brief Equals implementation.
		 *
		 * @param rhs The other value.
		 * @return Returns true if rhs is of runtime type 'null'
		 */
		virtual bool eq_impl(const value* rhs) const override {
			if (const auto rhsnull = dynamic_cast<const null*>(rhs)) {
				return true; 
			}
			return false;
		}
	};
}
