#include "boolean.hpp"

json::boolean::operator bool() const {
	return m_boolean;
}
void json::boolean::set(bool val) {
	m_boolean = val;
}
