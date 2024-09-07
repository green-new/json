#pragma once

#include <sstream>
#include "iterable.hpp"
#include "null.hpp"
#include "value.hpp"
namespace json {
/**
* @class json::root
* @extends json::value
* @brief The json root class.
* Contains everything. Properties cannot be 'nullptr'. Rather, they are the 'null' json value.
*/
class root final : public value, public iterable<prop_map> {
public:
	root()
		: m_root() {}
	root(const root& other) {
		// Cannot copy unique ptrs. Must do a deep manual copy
		m_root.clear();
		for (const auto& [key, val] : other.m_root) {
			if (val) {
				// Copy ctor called. Objects cannot be nullptr
				m_root.insert({ key, val->clone() });
			}
		}
	}
	root(root&& other) noexcept
		m_root(other.m_root) {}
	root& operator=(const root& other) {
		if (this != &other) {
			m_root = other.m_root;
		}
		return *this;
	}
	root& operator=(root&& other) noexcept {
		if (this != &other) {
			root(other.m_root);
		}
		return *this;
	}
	~root() = default;
public:
	/**
	* @brief Inserts a new JSON value based on the provided template and ctor arguments.
	* @tparam JsonValue The type of the JSON value to insert.
	* @tparam CtorArgs The arguments to the JsonValue ctor.
	* @param name The name of this value.
	* @param ctorArgs The ctor args of JsonType.
	* @returns Lvalue reference to the newly created object member.
	*/
	template<typename JsonType, typename... CtorArgs>
		requires std::constructible_from<JsonType, CtorArgs...>
		&& std::derived_from<JsonType, json::value>
	JsonType& insert(const std::string& name, CtorArgs... ctorArgs) {
		m_props.insert({ name, std::make_unique<JsonType>(ctorArgs...) });
		/**
		* @todo Undefined behavior if m_props[name] does not instantiate a JsonValue (could happen due to memory issues).
		*/
		return (JsonType&)*m_props.at(name);
	}
	/**
	* @brief Return an lvalue reference to the specified JSON value.
	* @param name Name of the JSON value.
	* @return L-value reference to the object meant for moving.
	*/
	value& get(const std::string& name) {
		return *m_root.at(name);
	}
	/**
	* @brief Return a copy to the old specified JSON value, replacing it with a null value internally.
	* @param name Name of the JSON value.
	* @return Copy to the old object.
	*/
	value move(const std::string& name) {
		return std::exchange(*m_root.at(name), json::null());
	}
	/**
	* @brief Return a copy to the old specified JSON value, replacing it with a provided type.
	* @tparam JsonType Type to replace the original JSON value with.
	* @param name Name of the JSON value.
	* @return Copy to the old object.
	*/
	template<typename JsonType>
		requires std::derived_from<JsonType, json::value>
	value replace(const std::string& name) {
		return std::exchange(*m_root.at(name), JsonType());
	}
public:
	std::string to_string() const override {
		std::ostringstream ss;
		ss << '{';
		std::string padding = "";
		for (const auto& [key, val] : m_root) {
			ss << padding;
			ss << '\"' << key << "\":";
			if (val) {
				ss << *val;
			} else {
				ss << "null";
			}
			padding = ",";
		}
		ss << '}';
		return ss.str();
	}
protected:
	root* clone_impl() const override {
		return new root(*this);
	}
public:
	/**
	* @brief Begin iterator.
	* @return The begin iterator of the container.
	*/
	virtual iterator begin() noexcept override {
		return m_root.begin();
	}
	/**
	* @brief End iterator.
	* @return The end iterator of the container.
	*/
	virtual iterator end() noexcept override {
		return m_root.end();
	}
	/**
	* @brief Reverse begin iterator.
	* @return The beginning of the reverse iterator of the container.
	*/
	virtual reverse_iterator rbegin() noexcept override {
		return m_root.rbegin();
	}
	/**
	* @brief Reverse end iterator.
	* @return The end of the reverse iterator of the container.
	*/
	virtual reverse_iterator rend() noexcept override {
		return m_root.rend();
	}
	/**
	* @brief Begin const iterator.
	* @return The beginning of the const iterator of the container.
	*/
	virtual const_iterator cbegin() const noexcept override {
		return m_root.cbegin();
	}
	/**
	* @brief End const iterator.
	* @return The end of the reverse iterator of the container.
	*/
	virtual const_iterator cend() const noexcept override {
		return m_root.cend();
	}
	/**
	* @brief Begin const reverse iterator.
	* @return The beginning of the const reverse iterator of the container.
	*/
	virtual const_reverse_iterator crbegin() const noexcept override {
		return m_root.crbegin();
	}
	/**
	* @brief End const reverse iterator.
	* @return The end of the const reverse iterator of the container.
	*/
	virtual const_reverse_iterator crend() const noexcept override {
		return m_root.crend();
	}
private:
	prop_map m_root;
};
}
