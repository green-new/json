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
json::array::iterator json::array::find(const json::value& json_value) {
	return std::find_if(begin(), end(), [&](value_ptr& p) { return *p.get() == json_value; });
}

// Modifiers
json::value_ptr json::array::pop() noexcept {
	value_ptr temp = std::unique_ptr(std::move(m_arr.back()));
	m_arr.pop_back();
	return temp; // RVO; will be moved
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
bool json::array::empty() const noexcept {
	return m_arr.empty();
}