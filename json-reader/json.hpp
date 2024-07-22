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

#pragma once

/**
* List of todos.
* @todo Implementation file for all implementations.
* @todo Header file for builders, iterable, etc..
* @todo Doxygen header comment for every header/implementation file.
* @todo Forward declare all objects.
* @todo Operater overloads for json::number.
*/

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
	* @brief Iterable interface. Should be its own header file.
	*/
	template<class Container, class Type>
	struct iterable {
		/**
		* @brief Type alias to the array's iterator.
		*/
		using iterator = typename Container::iterator;
		/**
		* @brief Type alias to the array's const iterator.
		*/
		using const_iterator = typename Container::const_iterator;
		/**
		* @brief Type alias to the array's reverse iterator.
		*/
		using reverse_iterator = typename Container::reverse_iterator;
		/**
		* @brief Type alias to the array's const reverse iterator.
		*/
		using const_reverse_iterator = typename Container::const_reverse_iterator;
		/**
		* @brief Begin iterator.
		*/
		virtual iterator begin() noexcept = 0;
		/**
		* @brief End iterator.
		*/
		virtual iterator end() noexcept = 0;
		/**
		* @brief Reverse begin iterator.
		*/
		virtual reverse_iterator rbegin() noexcept = 0;
		/**
		* @brief Reverse end iterator.
		*/
		virtual reverse_iterator rend() noexcept = 0;
		/**
		* @brief Begin const iterator.
		*/
		virtual const_iterator cbegin() const noexcept = 0;
		/**
		* @brief End const iterator.
		*/
		virtual const_iterator cend() const noexcept = 0;
		/**
		* @brief Begin const reverse iterator.
		*/
		virtual const_reverse_iterator crbegin() const noexcept = 0;
		/**
		* @brief End const reverse iterator.
		*/
		virtual const_reverse_iterator crend() const noexcept = 0;
		/**
		* @brief Calls a user-provided callback that iterates through each element as a const reference in the collection.
		*/
		virtual void for_each(std::function<void, (const Type& element)> functor) const = 0;
		/**
		* @brief Calls a user-provided callback that iterates through each element as a mutable reference in the collection.
		*/
		virtual void for_each(std::function<void(Type& element)> functor) = 0;
		/**
		* @brief Moves each element out of the collection into user ownership.
		*/
		virtual void consume(std::function<void(Type&&)> functor) = 0;
	};

	/**
	 * @class json::value
	 *
	 * @brief Abstract parent class used to represent concrete JSON types (see RFC for this list).
	 * Abstract parent class used to polymorphise concrete JSON classes used by this library.
	 * 
	 */
	class value {
	public:
		/*
		* @brief Returns this object as a string.
		* 
		* @return The string representation of this object.
		*/
		virtual std::string to_string() const = 0;
		/*
		* @brief Clones this value object by calling the clone implementation.
		* This function is marked const.
		*/
		auto clone() const {
			return std::unique_ptr<value>(clone_impl());
		}
		/*
		* @brief Output stream free method.
		*
		* @param os The output stream.
		* @param val The value to print to output stream.
		*/
		friend std::ostream& operator<<(std::ostream& os, const value& val) {
			os << val.to_string();
			return os;
		}
		/**
		 * @brief Trivial virtual destructor.
		 */
		virtual ~value() {}
	/*
	* Abstract methods.
	*/
	protected:
		/*
		* @brief The cloning implementation for derived classes.
		* 
		* @returns A value pointer to a deep copy of this value.
		*/
		virtual value* clone_impl() const = 0;
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
		number(const Storage& num)
			: m_number(num) {}
		~number() = default;
		/**
		 * Operator overloads
		 */
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
		 * @todo Maybe just expose the m_number publicly for <cmath> operations.
		 */

		 /**
		  * @brief Determines if the current number is an integral type.
		  * This functions returns @c true if this number is storing an integer type, @c false if not.
		  * This function is marked @c noexcept .
		  *
		  * @return Boolean representing if this number is an integer or not.
		  */
		bool isint() noexcept {
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
		bool isfloat() noexcept {
			return std::is_same<Storage, std::double_t>::value;
		}
		/**
		 * @brief Determines if the current number is finite.
		 * This functions returns @c true if this number is finite, @c false if infinite.
		 * This function is marked @c noexcept .
		 *
		 * @return Boolean representing if this number is finite or infinite.
		 */
		bool isfinite() noexcept {
			return std::isfinite(m_number);
		}
		/**
		 * @brief Determines if the current number is NaN.
		 * This functions returns @c true if this number is NaN, @c false if its normal.
		 * This function is marked @c noexcept .
		 *
		 * @return Boolean representing if this number is NaN or normal.
		 */
		bool isnan() noexcept {
			return std::isnan(m_number);
		}
		/*
		* @brief Determines if the current number is normal.
		* This function returns @c true if this number is normal, @c false if not.
		* This function is marked noexcept.
		* 
		* @return Boolean @c true/false if this number is normal or not.
		*/
		bool isnormal() noexcept {
			return std::isnormal(m_number);
		}
	/*
	* Overload methods.
	*/
	public:
		/*
		* @copydoc json::value::to_string
		*/
		virtual std::string to_string() const override {
			return std::format("{}", m_number);
		}
		/*
		* @copydoc json::value::clone_impl
		*/
		virtual number* clone_impl() const override {
			return new number(*this);
		}
	private:
		/**
		 * The number.
		 */
		Storage m_number;
	};
	/*
	* @class json::string
	* 
	* @extends json::value
	* 
	* @brief Contains a UTF-8 null-deliminated string.
	* Contains a UTF-8 null-deliminated string. 
	* Strings can be empty.
	*/
	class string final : public value {
	/**
	* Class methods.
	*/
	public:
		/*
		* @brief Default ctor. Default initializes m_string to an empty string "".
		*/
		string()
			: m_string() { }
		/*
		* @brief Ctor expecting a string const reference.
		* Copies the string parameter into m_string.
		* 
		* @param str The string to copy into this class.
		*/
		string(const std::string& str)
			: m_string(str) { } 
		/*
		* @brief Copy ctor.
		*/
		string(const string& str)
			: m_string(str.m_string) { }
		/*
		* @brief Move ctor.
		*/
		string(std::string&& str) noexcept
			: m_string(std::forward(str)) { }
		/*
		* @brief Copy assignment.
		*/
		string& operator=(const string& other) {
			*this = string(other);
			return *this;
		}
		/*
		* @brief Move assignment.
		*/
		string& operator=(string&& other) noexcept {
			*this = string(other);
			return *this;
		}
		~string() = default;
	/*
	* Access.
	*/
	public:
		std::string& str() {
			return m_string;
		}
	/*
	* Overrides.
	*/
	public:
		/*
		* @copydoc json::value::to_string
		*/
		std::string to_string() const override {
			return std::format("\"{}\"", m_string);
		}		
		/*
		* @copydocs json::value::clone_impl
		*/
		virtual string* clone_impl() const override {
			return new string(*this);
		}
	private:
		std::string m_string{};
	};

	/**
	 * @class json::boolean
	 *
	 * @brief Contains a simple true/false value.
	 */
	class boolean final : public value {
	/*
	* Class methods.
	*/
	public:
		boolean() = delete;
		boolean(bool value)
			: m_boolean(value) { }
		boolean(const boolean& other)
			: m_boolean(other.m_boolean) { }
		boolean(boolean&& other) noexcept {
			std::swap(m_boolean, other.m_boolean);
		}
		boolean& operator=(const boolean& other) {
			if (this != &other) {
				m_boolean = other.m_boolean;
			}
			return *this;
		}
		boolean& operator=(boolean&& other) noexcept {
			if (this != &other) {
				std::swap(m_boolean, other.m_boolean);
			}
			return *this;
		}
		~boolean() = default;
	/*
	* Operator overloads.
	*/
	public:
		operator bool() const {
			return m_boolean;
		}
	/*
	* Overrides.
	*/
	public:
		/*
		* @copydoc json::value::to_string
		*/
		std::string to_string() const override {
			return std::format("{}", m_boolean ? "true" : "false");
		}
		/*
		* @copydoc value::clone_impl
		*/
		boolean* clone_impl() const override {
			return new boolean(*this);
		}
	private:
		bool m_boolean;
	};

	/**
	* @class json::array
	*
	* @extends json::node
	*
	* @brief Representation of a JSON array containing type agnostic data.
	* Representation of a JSON array containing type agnostic data. Interally uses @c std::vector to store data.
	*/
	class array final : public value, public iterable<array::container, array::container::value_type::element_type> {
		using container = std::vector<value_ptr>;
	/*
	* Class methods.
	*/
	public:
		/*
		* @brief Default ctor. Default initializes the internal array.
		*/
		array() 
			: m_arr() { }
		/*
		* @brief Copy ctor.
		*/
		array(const array& other) {
			m_arr.clear();
			for (const auto& elem : other.m_arr) {
				m_arr.push_back(elem->clone());
			}
		}
		/*
		* @brief Move ctor.
		*/
		array(array&& other) noexcept
			: m_arr(other.m_arr) { }
		/*
		* @brief Copy assignment.
		*/
		array& operator=(const array& other) {
			if (*this != &other) {
				*this = array(other);
			}
			return *this;
		}
		/*
		* @brief Move assignment.
		*/
		array& operator=(array&& other) noexcept {
			if (*this != &other) {
				*this = array(other);
			}
			return *this;
		}
		~array() {}
	/*
	* Access.
	*/
	public:
		/*
		* @brief Gets the element at the provided index.
		* 
		* @param index Index of the array.
		* 
		* @returns The array element at the index.
		*/
		json::value& at(size_t index) {
			/*
			* @todo Provide error callbacks instead of the array class handling the errors with a namespaced/enum class value of a json error.
			*/
			if (index >= m_arr.size()) {
				std::ostringstream ss;
				std::string err_msg = std::format("\'index\' was out of range for \'json::array\': array was size {}, index was {}", m_arr.size(), index);
				throw std::out_of_range(err_msg);
			}
			return *m_arr.at(index);
		}
		/*
		* @brief Gets the element at the provided index.
		* An index that is not within the bounds is undefined behavior.
		* 
		* @param index The index of the element within the array.
		*/
		json::value& operator[](size_t index) {
			return *m_arr[index];
		}
		json::value& front() {
			return *m_arr.front();
		}
		json::value& back() {
			return *m_arr.back();
		}
		container::value_type* data() {
			return m_arr.data();
		}
	/*
	* Modifiers.
	*/
	public:
		/*
		* @brief Pushes an element into the back of the array.
		* 
		* @tparam JsonType The type of the object thats being pushed.
		* 
		* @param temp The object being pushed.
		*/
		template<typename JsonType>
		JsonType& push(const JsonType& temp) {
			m_arr.push_back(std::make_unique<JsonType>(temp));
			return *m_arr.back();
		}
		/*
		* @brief Removes the last element from the array.
		* This function is marked noexcept.
		*/
		void pop() noexcept {
			m_arr.pop_back();
		}
		/*
		* @brief Clears the array.
		* This function is marked noexcept.
		*/
		void clear() noexcept {
			m_arr.clear();
		}
		/*
		* @brief Creates an element in place with the provided ctor args.
		* 
		* @tparam JsonType The JSON type.
		* 
		* @tparam CtorArgs Parameter pack of ctor arg types.
		* 
		* @param args The ctor args to forward.
		*/
		template<typename JsonType, typename... CtorArgs>
		void emplace(CtorArgs... args) {
			m_arr.push_back(std::make_unique<JsonType>(std::forward(args)));
		}
		/*
		* @brief Erases the element at the provided iterator.
		* 
		* @param pos The position of the element to erase.
		* 
		* @returns The iterator following the deleted element.
		*/
		iterator erase(iterator pos) {
			return m_arr.erase(pos);
		}
		/*
		* @brief Erases the element at the provided const iterator.
		* This function is marked constexpr.
		* 
		* @param pos The const iterator.
		* 
		* @returns The iterator following the deleted element.
		*/
		constexpr iterator erase(const_iterator pos) {
			return m_arr.erase(pos);
		}
		/*
		* @brief Erases the elements in the provided iterator range.
		* 
		* @param first The first iterator.
		* @param last The last iterator.
		* 
		* @returns The iterator following the last deleted element.
		*/
		iterator erase(iterator first, iterator last) {
			return m_arr.erase(first, last);
		}
		/*
		* @brief Erases the elements in the provided const iterator range.
		* 
		* @param first The first const iterator.
		* @param second The second const iterator.
		* 
		* @returns The iterator following the last deleted element.
		*/
		constexpr iterator erase(const_iterator first, const_iterator last) {
			return m_arr.erase(first, last);
		}
	/*
	* Capacity methods.
	*/
	public:
		/*
		* @brief Gets the size of the array.
		* This function is marked noexcept.
		* 
		* @returns The size of this array.
		*/
		size_t size() const noexcept {
			return m_arr.size();
		}
		/*
		* @brief Gets if this array is empty or not.
		* 
		* @returns True/false if this array is empty or not.
		*/
		bool is_empty() const noexcept {
			return m_arr.empty();
		}
	/*
	* Overrides.
	*/
	public:
		/*
		* @copydoc json::value::to_string
		*/
		std::string to_string() const override {
			std::ostringstream ss;
			ss << '[';
			for (size_t i = 0; i < m_arr.size() - 1; i++) {
				if (m_arr[i]) {
					ss << *m_arr[i];
				} else {
					ss << "null";
				}
				ss << ',';
			}
			ss << *m_arr[m_arr.size() - 1];
			ss << ']';
			return ss.str();
		}
		/*
		* @copydoc value::clone_impl
		*/
		virtual array* clone_impl() const override {
			return new array(*this);
		}
		/**
		* @brief Begin iterator.
		*/
		virtual iterator begin() noexcept override {
			return m_arr.begin();
		}
		/**
		* @brief End iterator.
		*/
		virtual iterator end() noexcept {
			return m_arr.end();
		}
		/**
		* @brief Reverse begin iterator.
		*/
		virtual reverse_iterator rbegin() noexcept {
			return m_arr.rbegin();
		}
		/**
		* @brief Reverse end iterator.
		*/
		virtual reverse_iterator rend() noexcept {
			return m_arr.rend();
		}
		/**
		* @brief Begin const iterator.
		*/
		virtual const_iterator cbegin() const noexcept {
			return m_arr.cbegin();
		}
		/**
		* @brief End const iterator.
		*/
		virtual const_iterator cend() const noexcept {
			return m_arr.cend();
		}
		/**
		* @brief Begin const reverse iterator.
		*/
		virtual const_reverse_iterator crbegin() const noexcept {
			return m_arr.crbegin();
		}
		/**
		* @brief End const reverse iterator.
		*/
		virtual const_reverse_iterator crend() const noexcept {
			return m_arr.crend();
		}
		/**
		* @brief Calls a user-provided callback that iterates through each element as a const reference in the collection.
		*/
		virtual void for_each(std::function<void(const Type& element)> functor) const {
			for (const auto& element : m_arr) {
				functor(*element);
			}
		}
		/**
		* @brief Calls a user-provided callback that iterates through each element as a mutable reference in the collection.
		*/
		virtual void for_each(std::function<void(Type& element)> functor) {
			for (auto& element : m_arr) {
				functor(*m_arr);
			}
		}
		/**
		* @brief Moves each element out of the collection into user ownership.
		*/
		virtual void consume(std::function<void(Type&&)> functor) {
			for (auto& element : m_arr) {
				functor(std::move(*element));
				element.release();
			}
			m_arr.clear();
		}
	/**
	 * Members.
	 */
	private:
		/*
		* Internal array element container.
		*/
		container m_arr{};
	};

	/**
	 * @class json::object
	 * 
	 * @extends json::value
	 * 
	 * @brief Represents a JSON object.
	 * Contains a map of ["name", value] pairs. Keeps track of the names of JSON values.
	 * A ["name", value] pair CANNOT have an empty or null name. A value may be null which is internally represented as 'nullptr'.
	 * Duplicate named members follow the same rules as the standard library's @c std::map::insert.
	 */
	class object final : public value, 
		public iterable<object::prop_map, object::prop_map::mapped_type::element_type> {
	/*
	* Class methods.
	*/
	public:
		/*
		* @brief Type alias for the JSON object property map.
		*/
		using prop_map = std::map<std::string, value_ptr>;
		/*
		* @brief Default ctor.
		*/
		object() 
			: m_name(), m_props() { }
		/*
		* @brief Explicit ctor.
		* 
		* @param name Name of this object.
		*/
		explicit object(const std::string& name) 
			: m_name(name), m_props() { }
		/*
		* @brief Copy ctor.
		*/
		object(const object& other) 
			: m_name(other.m_name) {
			// Cannot copy unique ptrs. Must do a deep manual copy
			m_props.clear();
			for (const auto& [key, val] : other.m_props) {
				if (val) {
					// Copy ctor called
					m_props.insert({ key, val->clone()});
				} else {
					m_props.insert({ key, nullptr });
				}
			}
		}
		/*
		* @brief Move ctor.
		*/
		object(object&& other) noexcept {
			std::swap(m_name, other.m_name);
			std::swap(m_props, other.m_props);
		}
		/*
		* @brief Copy assignment.
		*/
		object& operator=(const object& other) {
			*this = object(other);
			return *this;
		}
		/*
		* @brief Move assignment.
		*/
		object& operator=(object&& other) noexcept {
			*this = object(other);
			return *this;
		}
		~object() { }
	/**
	* Capacity.
	*/
	public:
		/*
		* @brief Returns the number of members in this object.
		* 
		* @returns Number of members in this object.
		*/
		size_t size() const noexcept {
			return m_props.size();
		}
		/**
		 * @brief Determines if this object is empty.
		 * 
		 * @return True if empty, false if non-empty.
		 */
		bool empty() const noexcept {
			return m_props.empty();
		}
	/*
	* Modifiers.
	*/
	public:
		/*
		* @brief Inserts a new JSON value based on the provided template and ctor arguments.
		* 
		* @tparam JsonValue The type of the JSON value to insert.
		* @tparam CtorArgs The arguments to the JsonValue ctor.
		* 
		* @param name The name of this value.
		* @param ctorArgs The ctor args of JsonType.
		* 
		* @returns Lvalue reference to the newly created object member.
		*/
		template<typename JsonValue = object, typename... CtorArgs>
		JsonValue& insert(const std::string& name, CtorArgs... ctorArgs)  {
			m_props.insert({ name, std::make_unique<JsonValue>(ctorArgs...) });
			/**
			 * @todo Undefined behavior if m_props[name] does not instantiate a JsonValue (could happen due to memory issues).
			 */
			return (JsonValue&) *m_props.at(name);
		}
	/*
	* Overrides.
	*/
	public:
		/*
		* @copydoc json::value::to_string
		*/
		std::string to_string() const override {
			std::ostringstream ss;
			if (m_name.has_value()) {
				ss << '\"' << m_name.value() << "\":";
			}
			ss << '{';
			std::string padding = "";
			for (const auto& [key, val] : m_props) {
				ss << padding;
				ss << '\"' << key << "\":";
				if (val) {
					ss << *val;
				} else {
					ss << "null";
				}
				padding = ",";
			}
			ss << '}';
			return ss.str();
		}
		/**
		* @copydoc json::value::clone_impl
		*/
		virtual object* clone_impl() const override {
			return new object(*this);
		}
	/**
	 * Members.
	 */
	private:
		/**
		* The object property (member) map.
		*/
		prop_map m_props{};
		/**
		 * The name of this object. 
		 */
		std::string m_name;
	};

	/**
	* @class json::root
	* 
	* @extends json::value
	* 
	* @brief The json root class.
	* Contains everything.
	*/
	class root final : public value, 
		public iterable<root::prop_map, root::value_type> {
	/**
	* Type aliases.
	*/
	public:
		/**
		* @brief Type alias for uncopyable JSON values.
		*/
		using value_type = std::unique_ptr<value>;
		/**
		* @brief Type alias for the JSON object property map.
		*/
		using prop_map = std::map<std::string, value_type>;
		/**
		* @brief Type alias for the map iterator.
		*/
		using iterator = prop_map::iterator;
		/**
		* @brief Type alias for the const map iterator.
		*/
		using const_iterator = prop_map::const_iterator;
	/**
	* Class methods.
	*/
	public:
		root()
			: m_root() { }
		root(const root& other) {
			// Cannot copy unique ptrs. Must do a deep manual copy
			m_props.clear();
			for (const auto& [key, val] : other.m_props) {
				if (val) {
					// Copy ctor called
					m_props.insert({ key, val->clone()});
				} else {
					m_props.insert({ key, nullptr });
				}
			}
		}
		root(root&& other) noexcept {
			std::swap(m_root, other.m_root);
		}
		root& operator=(const root& other) {
			if (this != &other) {
				m_root = other.m_root;
			}
			return *this;
		}
		root& operator=(root&& other) noexcept {
			if (this != &other) {
				std::swap(m_root, other.m_root);
			}
			return *this;
		}
		~root() = default;
	/**
	* Modifiers.
	*/
	public:
		/**
		* @brief Inserts a new JSON value based on the provided template and ctor arguments.
		* 
		* @tparam JsonValue The type of the JSON value to insert.
		* @tparam CtorArgs The arguments to the JsonValue ctor.
		* 
		* @param name The name of this value.
		* @param ctorArgs The ctor args of JsonType.
		* 
		* @returns Lvalue reference to the newly created object member.
		*/
		template<typename JsonValue = object, typename... CtorArgs>
		JsonValue& insert(const std::string& name, CtorArgs... ctorArgs)  {
			m_props.insert({ name, std::make_unique<JsonValue>(ctorArgs...) });
			/**
			 * @todo Undefined behavior if m_props[name] does not instantiate a JsonValue (could happen due to memory issues).
			 */
			return (JsonValue&) *m_props.at(name);
		}
	/**
	* Override methods.
	*/
	public:
		std::string to_string() const override {
			std::ostringstream ss;
			ss << '{';
			std::string padding = "";
			for (const auto& [key, val] : m_root) {
				ss << padding;
				ss << '\"' << key << "\":";
				if (val) {
					ss << *val;
				} else {
					ss << "null";
				}
				padding = ",";
			}
			ss << '}';
			return ss.str();
		}
		root* clone_impl() const override {
			return new root(*this);
		}
	private:
		prop_map m_root;
	};
}