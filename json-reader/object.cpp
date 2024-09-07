#include "array.hpp"
#include "object.hpp"
#include <utility>

// Insertion with lvalues (copy)
json::object& json::object::insert_array(const std::string& name, const array& arr) {
	m_props.insert({ name, std::make_unique<array>(arr) });
	return *this;
}
json::object& json::object::insert_boolean(const std::string& name, const boolean& b) {
	m_props.insert({ name, std::make_unique<boolean>(b) });
	return *this;
}
json::object& json::object::insert_null(const std::string& name, const null& n) {
	m_props.insert({ name, std::make_unique<null>(n) });
	return *this;
}
json::object& json::object::insert_number(const std::string& name, const number& num) {
	m_props.insert({ name, std::make_unique<number>(num) });
	return *this;
}
json::object& json::object::insert_object(const std::string& name, const object& obj) {
	m_props.insert({ name, std::make_unique<object>(obj) });
	return *this;
}
json::object& json::object::insert_string(const std::string& name, const string& str) {
	m_props.insert({ name, std::make_unique<string>(str) });
	return *this;
}

// Insertion with rvalues (move)
json::object& json::object::insert_array(const std::string& name, array&& rval) {
	m_props.insert({ name, std::make_unique<array>(std::forward<array>(rval)) });
	return *this;
}
json::object& json::object::insert_boolean(const std::string& name, boolean&& rval) {
	m_props.insert({ name, std::make_unique<boolean>(std::forward<boolean>(rval)) });
	return *this;
}
json::object& json::object::insert_null(const std::string& name, null&& rval) {
	m_props.insert({ name, std::make_unique<null>(std::forward<null>(rval)) });
	return *this;
}
json::object& json::object::insert_number(const std::string& name, number&& rval) {
	m_props.insert({ name, std::make_unique<number>(std::forward<number>(rval)) });
	return *this;
}
json::object& json::object::insert_object(const std::string& name, object&& rval) {
	m_props.insert({ name, std::make_unique<object>(std::forward<object>(rval)) });
	return *this;
}
json::object& json::object::insert_string(const std::string& name, string&& rval) {
	m_props.insert({ name, std::make_unique<string>(std::forward<string>(rval)) });
	return *this;
}

// State
size_t json::object::size() const noexcept {
	return m_props.size();
}
bool json::object::empty() const noexcept {
	return m_props.empty();
}