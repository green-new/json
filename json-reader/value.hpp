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
	* @return The string representation of this JSON value.
	*/
	virtual std::string to_string() const = 0;
	/*
	* @brief Clones this value by calling the clone implementation.
	*/
	auto clone() const {
		return std::unique_ptr<value>(clone_impl());
	}
	/*
	* @brief Output stream free method.
	* @param os The output stream.
	* @param val The value to print to output stream.
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
};

class value_parent : public value {
public:
	virtual void remove_child(value* child) = 0;
}

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