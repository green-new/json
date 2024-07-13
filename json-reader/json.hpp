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
	class node;
	template<typename JsonType>
	class value;
	class array;
	class object;
	
	/**
	 * @brief Typedefs for JSON types with standard library classes.
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
	 * @class json::node
	 *
	 * @brief Abstract parent class used to represent concrete types.
	 * Abstract parent class used to polymorphise concrete JSON classes used by this library. Contains the name of the node as a @c std::string .
	 * 
	 */
	class node {
	protected:
		/**
		 * @brief Default ctor.
		 * Default ctor. Default initializes @c m_name @c to an empty string.
		 */
		node() : m_name({}) { }
		/**
		 * @brief Name provided ctor.
		 * Explicit ctor constructing this node with a provided `name`.
		 * @param name Name/key of the node
		 */
		explicit node(const types::string& name) 
			: m_name(name) { }
		/**
		 * @brief Copy ctor.
		 */
		node(const node& other) 
			: m_name(other.m_name) { }
		/**
		 * @brief Move ctor.
		 */
		node(node&& other) noexcept {
			std::swap(m_name, other.m_name);
		}
		/**
		 * @brief Copy assignment.
		 */
		node& operator=(const node& other) {
			*this = node(other);
			return *this;
		}
		/**
		 * @brief Move assignment.
		 */
		node& operator=(node&& other) noexcept {
			*this = node(other);
			return *this;
		}
		/**
		 * @brief Trivial virtual destructor.
		 */
		virtual ~node() {}
	public:
		/**
		 * @brief The name for this JSON node.
		 */
		types::string m_name{};
	};
	
	struct nullable {
		virtual bool is_null() = 0;
	};
	
	/**
	 * @brief Typedef pointer for @c node -derived classes
	 */
	using node_ptr = std::unique_ptr<node>;
	
	/**
	 * @todo Class for JSON numbers (handling large values, integers, etc... and how to parse them properly.
	 */
	namespace number_types {
		using uint = int64_t;
	}
	
	namespace storage_policy {
		using integer = std::int64_t;
		using uinteger = std::uint64_t;
		using floating = std::double_t;
	}
	
	template<typename Storage = storage_policy::floating>
	class number final : public nullable {
	public:
		number()
			: m_number(nullptr) {}
		number(const Storage& num)
			: m_number(num) {}
	public:
		number& operator+(const number& rhs) {
			m_number += rhs;
			return *this;
		}
	public:
		bool is_int() noexcept {
			return std::is_same<Storage, std::int64_t>()
				|| std::is_same<Storage, std::uint64_t>();
		}
		bool is_float() noexcept {
			return std::is_same<Storage, std::double_t>::value;
		}
	private:
		Storage m_number;
	};
	/**
	 * @class json::value
	 *
	 * @extends json::node
	 * 
	 * @brief JSON class representing a @code "name": value @endcode pair.
	 * JSON class representing a @code "name: value" @endcode pair. This class uses templates to take in the desired JSON value to store. The JSON value may be of the following (defined in https://datatracker.ietf.org/doc/html/rfc8259):
	 * @li @c null (literal)
	 * @li @c false (literal)
	 * @li @c true (literal)
	 * @li JSON object
	 * @li JSON array
	 * @li JSON number
	 * @li JSON string
	 *
	 * @tparam JsonType The JSON type (literal or JSON type) to encapsulate.
	 *
	 */
	template<typename JsonType>
	class value final : public node {
	public:
		using value_opt = std::optional<std::unique_ptr<JsonType>>;
		value() 
			: node(), m_value() {}
		explicit value(const JsonType& value) 
			: node(), m_value(std::make_optional<std::unique_ptr<JsonType>>(std::make_unique<JsonType>(value))) { }
		explicit value(const char* name, const JsonType& value)
			: node(types::string(name)), m_value(value) { }
		explicit value(const types::string& name, const JsonType& value)
			: node(name), m_value(value) { }
		value(const value& other)
			: node(other.m_name) {
			m_value = other.m_value;
		}
		value(value&& other) noexcept {
			std::swap(this->m_name, other.m_name);
			std::swap(m_value, other.m_value);
		}
		value& operator=(const value& other) {
			*this = value(other);
			return *this;
		}
		value& operator=(value&& other) noexcept {
			*this = value(other);
			return *this;
		}
		~value() { }
	public:
		value& operator=(const JsonType& temp) {
			m_value = temp;
			return *this;
		}
		value& operator=(JsonType&& temp) noexcept {
			m_value = temp;
			return *this;
		}
		bool is_null() const noexcept {
			return m_value.has_value();
		}
		bool is_nan() const noexcept {
			return std::isnan(m_value.value().get());
		}
		bool is_finite() const noexcept {
			return std::isfinite(m_value.value_or(0).get());
		}
	public:
		value_opt m_value{};
	};

	// ["a", 1, null, 1.0, {}, []] json object (can contain a variety of json types, which are either values, arrays, or objects)
	class array : public node {
	private:
		// name-agnostic, type-erased value used by the array class
		class array_element {
		private:
			class array_element_concept {
			public:
				virtual ~array_element_concept() = default;
			};
			template<typename JsonType>
			class array_element_model : public array_element_concept {
			public:
				array_element_model() 
					: m_value(), m_runtimeType(getType<JsonType>()) { }
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
			array_element() = delete; // Cannot have array elements with absolutely nothing in them (e.g., [, , , ])
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
		array() 
			: node() { }
		explicit array(const types::string& name)
			: node(name) { }
		array(const array& other) 
			: node(other.m_name) {
			m_arr.clear();
			for (const auto& elem : other.m_arr) {
				m_arr.push_back(elem);
			}
		}
		array(array&& other) noexcept {
			std::swap(this->m_name, other.m_name);
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

	class object : public node {
	public:
		using prop_map = std::map<std::string, node_ptr>;
		using iterator = prop_map::iterator;
		using const_iterator = prop_map::const_iterator;
		object() 
			: node(), m_props() { }
		explicit object(const types::string& name)
			: node(name) { }
		explicit object(const types::string& name, std::initializer_list<node_ptr> list)
			: node(name) {
			for (node_ptr& elem : list) {
				m_props.insert({elem.name, std::move(elem)});
			}
		}
		object(const object& other) 
			: node(other.m_name) {
			// Cannot copy unique ptrs. Must do a deep manual copy
			m_props.clear();
			for (const auto& [key, nodePtr] : other.m_props) {
				if (nodePtr) {
					m_props.insert({ key, new_node<node>(*nodePtr) });
				} else {
					m_props.insert({ key, node_ptr(nullptr) });
				}
			}
		}
		object(object&& other) noexcept {
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
		template<typename JsonNode = object, typename... CtorArgs>
		JsonNode& insert(const types::string& name, CtorArgs... ctorArgs)  {
			m_props.insert({ name, std::make_unique<JsonNode>(name, ctorArgs...) });
			return (JsonNode&) *m_props[name];
		}
		template<typename JsonType>
		value<JsonType>& insert_value(const types::string& name, const JsonType& temp) {
			m_props.insert({ name, std::make_unique<value<JsonType>>(name, temp) });
			return (value<JsonType>&) *m_props[name];
		}
		array& insert_array(const types::string& name) {
			m_props.insert({ name, std::make_unique<array>(name) });
			return (array&) *m_props[name];
		}
		object& insert_object(const types::string& name) {
			m_props.insert({ name, std::make_unique<object>(name) });
			return (object&) *m_props[name];
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
	};
}