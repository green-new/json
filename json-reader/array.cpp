#include "array.hpp"

// Because 'container' contains a list of 'value_ptr' 
// which is an alias of 'unique_ptr', we must define the 
// other constructors.

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
size_t json::array::size() const noexcept {
	return m_arr.size();
}
bool json::array::is_empty() const noexcept {
	return m_arr.empty();
}