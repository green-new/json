#pragma once

#include <sstream>
#include "object.hpp"
namespace json {
	/**
	 * @class json::root
	 * @extends json::value
	 * @brief The json root class. Roots are objects by structure but are treated differently.
	 */
	class root final : public object {
	};
}
