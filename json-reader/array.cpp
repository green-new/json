#include "array.hpp"
#include "object.hpp"
#include <utility>

// Access 
json::value& json::array::at(size_t index) {
	return *m_arr.at(index);
}
inline json::value& json::array::operator[](size_t index) {
	return *m_arr[index];
}
inline json::value& json::array::front() noexcept {
	return *m_arr.front();
}
inline json::value& json::array::back() noexcept {
	return *m_arr.back();
}
inline json::array_container::value_type* json::array::data() noexcept {
	return m_arr.data();
}

// Insertion with lvalues (copy)
json::array& json::array::push_array(const array& arr) {
	m_arr.push_back(std::make_unique<array>(arr));
	return *this;
}
json::array& json::array::push_boolean(const boolean& b) {
	m_arr.push_back(std::make_unique<boolean>(b));
	return *this;
}
json::array& json::array::push_null(const null& n) {
	m_arr.push_back(std::make_unique<null>(n));
	return *this;
}
json::array& json::array::push_number(const number& num) {
	m_arr.push_back(std::make_unique<number>(num));
	return *this;
}
json::array& json::array::push_object(const object& obj) {
	m_arr.push_back(std::make_unique<object>(obj));
	return *this;
}
json::array& json::array::push_string(const string& str) {
	m_arr.push_back(std::make_unique<string>(str));
	return *this;
}

// Insertion with rvalues (move)
json::array& json::array::push_array(array&& rval) {
	m_arr.emplace_back(std::forward<array>(rval));
	return *this;
}
json::array& json::array::push_boolean(boolean&& rval) {
	m_arr.emplace_back(std::forward<boolean>(rval));
	return *this;
}
json::array& json::array::push_null(null&& rval) {
	m_arr.emplace_back(std::forward<null>(rval));
}
json::array& json::array::push_number(number&& rval) {
	m_arr.emplace_back(std::forward<number>(rval));
	return *this;
}
json::array& json::array::push_object(object&& rval) {
	m_arr.emplace_back(std::forward<object>(rval));
	return *this;
}
json::array& json::array::push_string(string&& rval) {
	m_arr.emplace_back(std::forward<string>(rval));
	return *this;
}

// Modifiers
void json::array::pop() noexcept {
	m_arr.pop_back();
}
void json::array::clear() noexcept {
	m_arr.clear();
}
constexpr json::array::iterator json::array::erase(iterator pos) noexcept {
	return m_arr.erase(pos);
}
constexpr json::array::iterator json::array::erase(const_iterator pos) noexcept {
	return m_arr.erase(pos);
}
constexpr json::array::iterator json::array::erase(iterator first, iterator last) noexcept {
	return m_arr.erase(first, last);
}
constexpr json::array::iterator json::array::erase(const_iterator first, const_iterator last) noexcept {
	return m_arr.erase(first, last);
}

// State
size_t json::array::size() const noexcept {
	return m_arr.size();
}
bool json::array::is_empty() const noexcept {
	return m_arr.empty();
}