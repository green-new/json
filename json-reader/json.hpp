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

#include <string>
#include <fstream>
#include <set>
#include <unordered_map>
#include <map>
#include <type_traits>
#include <cinttypes>
#include <cstdint>
#include <memory>
#include <initializer_list>
#include <vector>
#include <cmath>

#pragma once

/**
 * @brief JSON namespace that holds everything.
 */
namespace json {

	// Forward declarations
	class value;
	template<typename Storage>
	class number;
	class boolean;
	class array;
	class object;
	
	/**
	 * @namespace json::types
	 *
	 * @brief Typedefs for JSON types with standard library classes.
	 *
	 * @todo Look into possibly removing this namespace.
	 */
	namespace types {
		using string = std::string;
		using uint32 = std::uint32_t;
		using int32 = std::int32_t;
		using uint64 = std::uint64_t;
		using int64 = std::int64_t;
		using float32 = std::float_t;
		using float64 = std::double_t;
		using boolean = bool;
		using nothing = std::nullptr_t;
	}

	/**
	 * @enum json::enum_types
	 *
	 * @brief Enumeration of JSON types.
	 */
	enum class enum_types {
		value,
		object,
		array,
		nothing
	};

	/**
	 * @brief Returns the JSON type enum corresponding to the templated datatype.
	 *
	 * @tparam JsonType Concrete datatype from @c json::types @c.
	 *
	 * @return JSON type enum.
	 */
	template<typename JsonType>
	static constexpr enum_types getType() {
		enum_types runtimeType;
		if constexpr(std::is_same<JsonType, array>()) {
			runtimeType = enum_types::array;
		} else if constexpr(std::is_same<JsonType, object>()) {
			runtimeType = enum_types::object;
		} else if constexpr(std::is_same<JsonType, types::nothing>()) {
			runtimeType = enum_types::nothing;
		} else if constexpr(std::is_same<JsonType, types::string>()
			|| std::is_same<JsonType, types::int32>()
			|| std::is_same<JsonType, types::int64>()
			|| std::is_same<JsonType, types::uint32>()
			|| std::is_same<JsonType, types::uint64>()
			|| std::is_same<JsonType, types::float32>()
			|| std::is_same<JsonType, types::float64>()
			|| std::is_same<JsonType, types::boolean>()) {
			runtimeType = enum_types::value;
		} else {
			runtimeType = enum_types::nothing;
		}
		return runtimeType;
	}

	/**
	 * @class json::value
	 *
	 * @brief Abstract parent class used to represent concrete JSON types (see RFC for this list).
	 * Abstract parent class used to polymorphise concrete JSON classes used by this library.
	 * 
	 */
	struct value {
	public:
		/**
		 * @brief Trivial virtual destructor.
		 */
		virtual ~value() {}
	};
	
	/**
	 * @brief Typedef pointer for @c value -derived classes
	 */
	using value_ptr = std::unique_ptr<value>;
	
	/**
	 * @namespace JSON number storage policies.
	 *
	 * @brief Contains integers, unsigned integers, and floating point numbers. All are 64-bit.
	 *
	 * @todo Look into mega-huge number storage like with java's BigDecimal.
	 */
	namespace storage_policy {
		using integer = std::int64_t;
		using uinteger = std::uint64_t;
		using floating = std::double_t;
	}
	
	/**
	 * @class json::number
	 * 
	 * @brief Represents a JSON number. Currently can store a 64-bit signed/unsigned integer, or a 64-bit floating point number.
	 * Represents a JSON number. Capable of storing a 64-bit signed/unsigned integer or a 64-bit floating point number.
	 * Cannot be instantiated without a number.
	 
	 * @tparam Storage Data type for the number (see namespace @c json::storage_policy for details).
	 *
	 * @todo Class for JSON numbers (handling large values, integers, etc... and how to parse them properly.
	 * @todo Not completed. Needs more operators, and better is_int, is_float checks.
	 */
	template<typename Storage = storage_policy::floating>
	class number final : public value {
	/**
	 * Class-specific methods
	 */
	public:
<<<<<<< HEAD
		number()
			: m_number(nullptr) {}
		number(const Storage& num)
			: m_number(num) {}
=======
		/**
		 * @brief Deleted default ctor.
		 */
		number() = delete;
		/**
		 * @brief Store a number ctor.
		 *
		 * @param num Number to store.
		 */
		number(const Storage& num)
			: m_number(num) { }
		~number() = default;
	/**
	 * Operator overloads
	 */
>>>>>>> 6a636ec4b3d6ac7e2966017cca3875db4e15c7f4
	public:
		/**
		 * @todo More operator overloads.
		 */
		number& operator+(const number& rhs) {
			return number(rhs.m_number + m_number);
		}
	public:
		/**
		 * @todo These functions will need to be changed with static compile-time checks if we implement a BigDecimal-like class to handle gigantic numbers.
		 */
		 
		/**
		 * @brief Determines if the current number is an integral type.
		 * This functions returns @c true if this number is storing an integer type, @c false if not.
		 * This function is marked @c noexcept .
		 *
		 * @return Boolean representing if this number is an integer or not.
		 */
		bool is_int() noexcept {
			return std::is_same<Storage, std::int64_t>()
				|| std::is_same<Storage, std::uint64_t>();
		}
		/**
		 * @brief Determines if the current number is a floating type.
		 * This functions returns @c true if this number is storing a floating/decimal type, @c false if not.
		 * This function is marked @c noexcept .
		 *
		 * @return Boolean representing if this number is a floating type or not.
		 */
		bool is_float() noexcept {
			return std::is_same<Storage, std::double_t>::value;
		}
		/**
		 * @brief Determines if the current number is finite.
		 * This functions returns @c true if this number is finite, @c false if infinite.
		 * This function is marked @c noexcept .
		 *
		 * @return Boolean representing if this number is finite or infinite.
		 */
		bool is_finite() noexcept {
			return std::isfinite(m_number);
		}
		/**
		 * @brief Determines if the current number is NaN.
		 * This functions returns @c true if this number is NaN, @c false if its normal.
		 * This function is marked @c noexcept .
		 *
		 * @return Boolean representing if this number is NaN or normal.
		 */
		bool is_nan() noexcept {
			return std::is_nan(m_number);
		}
	private:
		/**
		 * The number.
		 */
		Storage m_number;
	};
<<<<<<< HEAD
=======
	
>>>>>>> 6a636ec4b3d6ac7e2966017cca3875db4e15c7f4
	/**
	 * @class json::boolean
	 *
	 * @brief Contains a simple true/false value.
	 * Contains a true/false value. Cannot be default initialized, thereof, cannot be empty.
	 */
	class boolean final : public value {
	public:
		boolean() = delete;
		boolean(bool value)
			: m_boolean(value) { }
		~boolean() = default;
		/**
		 * @todo Operator overloads that copy boolean operators on m_boolean.
		 */
	private:
		/**
		 * The boolean. @c true or @c false are the only states for this member.
		 */
		bool m_boolean;
	}
	
	/**
	 * @class json::array
	 *
	 * @extends json::node
	 *
	 * @brief Representation of a JSON array containing type agnostic data.
	 * Representation of a JSON array containing type agnostic data. Interally uses @c std::vector to store data.
	 */
	class array final : public value {
	private:
		/**
		 * @class json::array::array_element
		 *
		 * @brief Type erased class meant for JSON array elements.
		 * Type erased class meant for JSON array containers. Uses the concept -> model pattern with templates to hide type T behind @c array_element.
		 * The default ctor is deleted as empty array elements cannot logically exist.
		 */
		class array_element {
		private:
			/**
			 * @class json::array::array_element_concept
			 *
			 * @brief Parent class of @c array_element_model in order to group @code array_element_model<T> @endcode 's together.
			 *
			 * @todo Stream output friend methods, any other inherited methods that do not contain templates can go here.
			 */
			class array_element_concept {
			public:
				virtual ~array_element_concept() = default;
			};
			/**
			 * @class json::array::array_element_model
			 *
			 * @extends json::array::array_element_concept
			 *
			 * @brief Concrete implementation of the array element containing the type.
			 * Concrete implementation of the array element containing the type. Stores the type erased data on the stack.
			 *
			 * @tparam JsonType The type of the data to be stored.
			 *
			 * @todo See performance differences between storing on the stack vs on the heap for different use cases. Maybe eventually have the end user decide the best location for it.
			 */
			template<typename JsonType>
			class array_element_model : public array_element_concept {
			public:
				/**
				 * @brief Default ctor.
				 */
				array_element_model() 
					: m_value(), m_runtimeType(getType<JsonType>()) { }
				/**
				 * @brief Ctor expecting a lvalue to the array element to create.
				 */
				array_element_model(const JsonType& temp)
					: m_value(temp), m_runtimeType(getType<JsonType>()) { }
				array_element_model(const array_element_model<JsonType>& other) 
					: m_value(other.value), m_runtimeType(getType<JsonType>()) { }
				array_element_model(array_element_model<JsonType>&& other) noexcept {
					std::swap(m_value, other.m_value);
				}
				array_element_model& operator=(const array_element_model<JsonType>& other) {
					*this = array_element_model<JsonType>(other);
					return *this;
				}
				array_element_model& operator=(array_element_model<JsonType>&& other) noexcept {
					*this = array_element_model<JsonType>(other);
					return *this;
				}
				~array_element_model() = default;
			public:
				JsonType m_value;
				enum_types m_runtimeType;
			};
		public:
			/**
			 * @brief Default ctor deleted.
			 */
			array_element() = delete;
			/**
			 * @brief Ctor expecting a lvalue to the array element to create.
			 *
			 * @tparam The type of the array element.
			 */
			template<typename JsonType>
			array_element(const JsonType& temp)
				: m_data(std::make_shared<array_element_model<JsonType>>(temp)) { }
			~array_element() = default;
		public:
			std::shared_ptr<array_element_concept> m_data;
		};
	public:
		using container = std::vector<array_element>;
		using iterator = typename container::iterator;
		using const_iterator = typename container::const_iterator;
		array() { }
		array(const array& other) {
			m_arr.clear();
			for (const auto& elem : other.m_arr) {
				m_arr.push_back(elem);
			}
		}
		array(array&& other) noexcept {
			std::swap(m_arr, other.m_arr);
		}
		array& operator=(array const& other) {
			*this = array(other);
			return *this;
		}
		array& operator=(array&& other) noexcept {
			*this = array(other);
			return *this;
		}
		~array() {}
	public:
		template<typename JsonType>
		void push(const JsonType& temp) {
			m_arr.push_back(array_element(temp));
		}
		void push(std::initializer_list<array_element> list) {
			for (const auto& elem : list) {
				m_arr.push_back(elem);
			}
		}
		void pop() {
			m_arr.pop_back();
		}
	public:
		iterator begin() { 
			return m_arr.begin();
		}
		iterator end() {
			return m_arr.end();
		}
		const_iterator begin() const {
			return m_arr.begin();
		}
		const_iterator end() const {
			return m_arr.end();
		}
	private:
		container m_arr{};
	};

	/**
	 * @todo Need to update 'has-a' relationships with the deleted `node` class (name member needs to go somewhere)
	 */
	class object : public value {
	public:
		using value_type = std::unique_ptr<value>
		using prop_map = std::map<std::string, value_type>;
		using iterator = prop_map::iterator;
		using const_iterator = prop_map::const_iterator;
		object() 
			: m_name(), m_props() { }
		explicit object(const types::string& name) 
			: m_name(name) { }
		explicit object(const types::string& name, std::initializer_list<value_type> list) 
			: m_name(name) {
			for (auto& elem : list) {
				m_props.insert({ elem.name, std::move(elem) });
			}
		}
		object(const object& other) 
			: m_name(other.name) {
			// Cannot copy unique ptrs. Must do a deep manual copy
			m_props.clear();
			for (const auto& [key, val] : other.m_props) {
				if (val) {
					m_props.insert({ key, std::make_unique<value>(*value_type)) });
				} else {
					m_props.insert({ key, nullptr });
				}
			}
		}
		object(object&& other) noexcept {
			std::swap(m_name, other.m_name);
			std::swap(m_props, other.m_props);
		}
		object& operator=(const object& other) {
			*this = object(other);
			return *this;
		}
		object& operator=(object&& other) noexcept {
			*this = object(other);
			return *this;
		}
		~object() { }
	public:
		template<typename JsonValue = object, typename... CtorArgs>
		JsonValue& insert(const types::string& name, CtorArgs... ctorArgs)  {
			m_props.insert({ name, std::make_unique<JsonValue>(name, ctorArgs...) });
			/**
			 * @todo Undefined behavior if m_props[name] does not instantiate a JsonValue (could happen due to memory issues).
			 */
			return (JsonValue&) *m_props[name];
		}
	public:
		iterator begin() {
			return m_props.begin();
		}
		iterator end() {
			return m_props.end();
		}
		const_iterator begin() const {
			return m_props.begin();
		}
		const_iterator end() const {
			return m_props.end();
		}
	private:
		prop_map m_props{};
		/**
		 * The name of this object. May be absent.
		 */
		std::optional<std::string> m_name;
	};
}