#include "object.hpp"

size_t json::object::size() const noexcept {
	return m_props.size();
}

bool json::object::empty() const noexcept {
	return m_props.empty();
}
