#include "number.hpp"

json::number json::number::operator+(const json::number& rhs) {
	return json::number(rhs.m_number + rhs.m_number);
}
json::number json::number::operator-(const json::number& rhs) {
	return json::number(rhs.m_number - rhs.m_number);
}
json::number json::number::operator/(const json::number& rhs) {
	return json::number(rhs.m_number / rhs.m_number);
}
json::number json::number::operator*(const json::number& rhs) {
	return json::number(rhs.m_number * rhs.m_number);
}
inline bool json::number::is_finite() noexcept {
	// Cast to bits
	uint64_t bits = std::bit_cast<uint64_t, number_type>(m_number);
	// Infinity is defined as 1111... in the 11 bit exponent part and 
	// 1111... in the fractional part.
	return ((bits & 0xFFFFFFFFFFFFF) != 0x0) && (((bits >> 52) & 0x7FF) != 0x7FF);
}
inline bool json::number::is_nan() noexcept {
	uint64_t bits = std::bit_cast<uint64_t, json::number::number_type>(m_number);
	return (((bits >> 52) & 0x7FF) == 0x7FF);
}
inline bool json::number::is_normal() noexcept {
	return !is_finite() && !is_nan();
}
