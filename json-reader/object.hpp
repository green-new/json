#include <sstream>
#include "iterable.hpp"
#include "value.hpp"

namespace json {
/**
* @class json::object
*
* @extends json::value
*
* @brief Represents a JSON object.
* Contains a map of ["name", value] pairs. Keeps track of the names of JSON values.
* A ["name", value] pair CANNOT have an empty or null name. A value may be null which is internally represented as 'nullptr'.
* Duplicate named members follow the same rules as the standard library's @c std::map::insert.
*/
class object final : public value, public iterable<prop_map> {
public:
	/*
	* @brief Default ctor.
	*/
	object()
		: m_name(), m_props() {}
	/*
	* @brief Explicit ctor.
	* @param name Name of this object.
	*/
	explicit object(const std::string& name)
		: m_name(name), m_props() {}
	/*
	* @brief Copy ctor.
	*/
	object(const object& other)
		: m_name(other.m_name) {
		// Cannot copy unique ptrs. Must do a deep manual copy
		m_props.clear();
		for (const auto& [key, val] : other.m_props) {
			if (val) {
				// Copy ctor called
				m_props.insert({ key, val->clone() });
			}
		}
	}
	/*
	* @brief Move ctor.
	*/
	object(object&& other) noexcept {
		std::swap(m_name, other.m_name);
		std::swap(m_props, other.m_props);
	}
	/*
	* @brief Copy assignment.
	*/
	object& operator=(const object& other) {
		if (this != &other) {
			*this = object(other);
		}
		return *this;
	}
	/*
	* @brief Move assignment.
	*/
	object& operator=(object&& other) noexcept {
		*this = object(other);
		return *this;
	}
	~object() {}
public:
	/*
	* @brief Returns the number of members in this object.
	*
	* @returns Number of members in this object.
	*/
	size_t size() const noexcept;
	/**
	* @brief Determines if this object is empty.
	*
	* @return True if empty, false if non-empty.
	*/
	bool empty() const noexcept;
public:
	/*
	* @brief Inserts a new JSON value based on the provided template and ctor arguments.
	*
	* @tparam JsonValue The type of the JSON value to insert.
	* @tparam CtorArgs The arguments to the JsonValue ctor.
	*
	* @param name The name of this value.
	* @param ctorArgs The ctor args of JsonType.
	*
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
	return (JsonValue&)*m_props.at(name);
	}
public:
	/*
	* @copydoc json::value::to_string
	*/
	std::string to_string() const override {
		std::ostringstream ss;
		ss << '\"' << m_name << "\":";
		ss << '{';
		std::string padding = "";
		for (const auto& [key, val] : m_props) {
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
	/**
	* @copydoc json::value::clone_impl
	*/
	virtual object* clone_impl() const override {
		return new object(*this);
	}
public:
	/**
	* @brief Begin iterator.
	* @return The begin iterator of the container.
	*/
	virtual iterator begin() noexcept override {
		return m_props.begin();
	}
	/**
	* @brief End iterator.
	* @return The end iterator of the container.
	*/
	virtual iterator end() noexcept override {
		return m_props.end();
	}
	/**
	* @brief Reverse begin iterator.
	* @return The beginning of the reverse iterator of the container.
	*/
	virtual reverse_iterator rbegin() noexcept override {
		return m_props.rbegin();
	}
	/**
	* @brief Reverse end iterator.
	* @return The end of the reverse iterator of the container.
	*/
	virtual reverse_iterator rend() noexcept override {
		return m_props.rend();
	}
	/**
	* @brief Begin const iterator.
	* @return The beginning of the const iterator of the container.
	*/
	virtual const_iterator cbegin() const noexcept override {
		return m_props.cbegin();
	}
	/**
	* @brief End const iterator.
	* @return The end of the reverse iterator of the container.
	*/
	virtual const_iterator cend() const noexcept override {
		return m_props.cend();
	}
	/**
	* @brief Begin const reverse iterator.
	* @return The beginning of the const reverse iterator of the container.
	*/
	virtual const_reverse_iterator crbegin() const noexcept override {
		return m_props.crbegin();
	}
	/**
	* @brief End const reverse iterator.
	* @return The end of the const reverse iterator of the container.
	*/
	virtual const_reverse_iterator crend() const noexcept override {
		return m_props.crend();
	}
private:
	/**
	* @brief The object property (member) map.
	*/
	prop_map m_props{};
	/**
	* @brief The name of this object.
	*/
	std::string m_name;
};
}
