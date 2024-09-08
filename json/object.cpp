#include "array.hpp"
#include "object.hpp"
#include <utility>

// State
size_t json::object::size() const noexcept {
	return m_props.size();
}
bool json::object::empty() const noexcept {
	return m_props.empty();
}
bool json::object::operator==(const object& rhs) const {
	if (size() != rhs.size()) {
		return false;
	}
	for (prop_map::const_iterator jt = rhs.cbegin(); jt != rhs.cend(); jt++) {
		if (!contains((*jt).first)) {
			return false;
		}
	}
	return true;
}
bool json::object::contains(const std::string& name) const {
	return m_props.contains(name);
}
