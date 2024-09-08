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
		 * @return Always returns true.
		 */
		virtual bool equals(const value* rhs) const override {
			return true;
		}
	};
}
