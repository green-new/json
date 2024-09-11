#pragma once

#include <map>
#include <memory>
#include <ostream>
#include <string>

#pragma once

namespace json {
	/**
	 * @class json::value
	 * @brief Abstract parent class used to represent concrete JSON types (see RFC for this list).
	 * Abstract parent class used to polymorphise concrete JSON classes used by this library.
	 */
	class value {
	public:
		/*
		 * @brief Returns this object as a string.
		 *
		 * @return The string representation of this JSON value.
		 */
		virtual std::string to_string() const = 0;
		/*
		 * @brief Clones this value by calling the clone implementation.
		 *
		 * @return Copy of the object, raw pointer.
		 */
		auto clone() const {
			return std::unique_ptr<value>(clone_impl());
		}
		/**
		 * @brief Equality operator. Calls the 'equals' implementation.
		 * 
		 * @return true/false, if val is equal to this value by content, determined by 'equals' implementation.
		 */
		bool operator==(const value& val) const {
			return eq_impl(&val);
		}
		/**
		 * @brief Less than operator. Calls the 'lt' implementation.
		 * 
		 * @return true/false, if val is less than to this value by content, determined by 'lt' implementation.
		 */
		bool operator<(const value& val) const {
			return lt_impl(&val);
		}
		/**
		 * @brief Greater than operator. Calls the 'gt' implementation.
		 * 
		 * @return true/false, if val is less than to this value by content, determined by 'gt' implementation.
		 */
		bool operator<(const value& val) const {
			return gt_impl(&val);
		}
		/*
		 * @brief Output stream method.
		 * @param os The output stream.
		 * @param val The value to output to output stream.
		 */
		friend std::ostream& operator<<(std::ostream& os, const value& val) {
			os << val.to_string();
			return os;
		}
		/**
		 * @brief Trivial virtual destructor.
		 */
		virtual ~value() = default;
	protected:
		/*
		 * @brief The cloning implementation for derived classes.
		 * @returns A value pointer to a deep copy of this value.
		 */
		virtual value* clone_impl() const = 0;
		/*
		 * @brief Determines if the provided value is equal in terms of content.
		 * Implementation dependent.
		 * @return True if equal, false if not.
		 */
		virtual bool eq_impl(const value* val) const = 0;
		/*
		 * @brief Determines if the provided value is less than in terms of content.
		 * Implementation dependent.
		 * @return True if equal, false if not.
		 */
		virtual bool lt_impl(const value* val) const = 0;
		/*
		 * @brief Determines if the provided value is greater than in terms of content.
		 * Implementation dependent.
		 * @return True if equal, false if not.
		 */
		virtual bool gt_impl(const value* val) const = 0;
	};

	/**
	 * @brief Type alias for value pointers that are owned and managed by other json values (objects, arrays, and roots)
	 */
	using value_ptr = std::unique_ptr<value>;

	/**
	 * @brief Type alias for value pointers that are unowning.
	 */
	using value_sptr = std::shared_ptr<value>;

	/*
	 * @brief Type alias for the JSON object property map.
	 */
	using prop_map = std::map<std::string, value_ptr>;
}