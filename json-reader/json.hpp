/**
 * @file json.hpp
 *
 * @brief JSON classes for objects, arrays, numbers, values, and literals
 *
 * @addtogroup json
 *
 * @author green-new
 * Contact: tristenwoodruff2@gmail.com
 */

#include <bit>
#include <concepts>
#include <string>
#include <sstream>
#include <unordered_map>
#include <map>
#include <type_traits>
#include <memory>
#include <vector>
#include <cmath>
#include <optional>
#include <stdexcept>
#include <format>
#include <functional>
#include <utility>
#include <stdfloat>
#include "iterable.hpp"

#pragma once

/**
* List of todos.
* @todo Implementation file for all implementations.
* @todo Header file for builders, iterable, etc..
* @todo Forward declare all objects.
*/

namespace json {

	// Forward declarations
	class value;
	class number;
	class boolean;
	class array;
	class object;
	class null;

}