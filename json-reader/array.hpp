#include "boolean.hpp"
#include "iterable.hpp"
#include "null.hpp"
#include "number.hpp"
#include "string.hpp"
#include <sstream>
#include "value.hpp"
#include <vector>

namespace json {
	// Array container type (owning)
	using array_container = std::vector<value_ptr>;

	// 'object' forward declaration
	class object;

	/**
	* @class json::array
	* @extends json::node
	* @brief Representation of a JSON array containing type agnostic data.
	* Representation of a JSON array containing type agnostic data. Interally uses @c std::vector to store data.
	*/
	class array final : public json::value, public iterable<array_container> {
	public:
		// Because 'container' contains a list of 'value_ptr' 
		// which is an alias of 'unique_ptr', we must define the 
		// other constructors.
		array()
			: m_arr() {}
		array(const array& other) {
			m_arr.clear();
			for (const auto& elem : other.m_arr) {
				// Clone the value (runtime polymorphism)
				m_arr.push_back(elem->clone());
			}
		}
		array(array&& other) noexcept
			: m_arr(other.m_arr) {}
		array& operator=(const array& other) {
			if (this != &other) {
				*this = array(other);
			}
			return *this;
		}
		array& operator=(array&& other) noexcept {
			if (this != &other) {
				*this = array(other);
			}
			return *this;
		}
		~array() = default;
	public:
		/*
		* @brief Gets the element at the provided index.
		* Assumes the index is in the valid range. Undefined behavior if not.
		* @param index Index of the array.
		* @returns The array element reference at the index, type erased to a 'value'.
		*/
		json::value& at(size_t index);
		/*
		* @brief Gets the element at the provided index.
		* An index that is not within the bounds is undefined behavior.
		* @param index The index of the element within the array.
		*/
		inline json::value& operator[](size_t index);
		/**
		* @brief Get the front 'value' in the array.
		* An index that is not within the bounds is undefined behavior.
		* @return The front 'value', type erased.
		*/
		inline json::value& front() noexcept;
		/**
		* @brief Get the back 'value' in the array.
		* An index that is not within the bounds is undefined behavior.
		* @return The back 'value', type erased.
		*/
		inline json::value& back() noexcept;
		/**
		* @brief Get the underlying contiguous pointer to the array, pointing to the first element.
		* An uninitalized empty array cannot be dereferenced.
		* @return The pointer to the first element, if it exists.
		*/
		inline array_container::value_type* data() noexcept;
	public:
		/**
		 * @brief Pushes a JSON value type 'array' into the end of this array by copy.
		 * 
		 * @param arr The array to push.
		 * @return This array that was modified.
		 */
		array& push_array(const array& arr);
		/**
		 * @brief Pushes a JSON value type 'array' into the end of this array by move.
		 *
		 * @param rval Rvalue reference to the 'array' (will be moved).
		 * @return This array that was modified.
		 */
		array& push_array(array&& rval);
		/**
		 * @brief Emplaces a JSON value type 'array' with the ctor args.
		 *
		 * @param arr The array to push.
		 * @return This array that was modified.
		 */
		template<typename... CtorArgs>
		array& emplace_array(CtorArgs&&... args) {
			m_arr.emplace_back<array>(std::forward(args)...);
			return *this;
		}
		/**
		 * @brief Pushes a JSON value type 'boolean' into the end of this array by copy.
		 *
		 * @param arr The array to push.
		 * @return This array that was modified.
		 */
		array& push_boolean(const boolean& b);
		/**
		 * @brief Pushes a JSON value type 'boolean' into the end of this array by move.
		 *
		 * @param rval Rvalue reference to the 'boolean' (will be moved).
		 * @return This array that was modified.
		 */
		array& push_boolean(boolean&& rval);
		/**
		 * @brief Emplaces a JSON value type 'boolean' with the ctor args.
		 *
		 * @param arr The array to push.
		 * @return This array that was modified.
		 */
		template<typename... CtorArgs>
		array& emplace_boolean(CtorArgs&&... args) {
			m_arr.emplace_back<boolean>(std::forward(args)...);
			return *this;
		}
		/**
		 * @brief Pushes a JSON value type 'null' into the end of this array by copy.
		 *
		 * @param arr The array to push.
		 * @return This array that was modified.
		 */
		array& push_null(const null& n);
		/**
		 * @brief Pushes a JSON value type 'null' into the end of this array by move.
		 *
		 * @param rval Rvalue reference to the 'null' (will be moved).
		 * @return This array that was modified.
		 */
		array& push_null(null&& rval);
		/**
		 * @brief Emplaces a JSON value type 'boolean' with the ctor args.
		 *
		 * @param arr The array to push.
		 * @return This array that was modified.
		 */
		array& emplace_null() {
			m_arr.emplace_back<null>(null{});
			return *this;
		}
		/**
		 * @brief Pushes a JSON value type 'number' into the end of this array by copy.
		 *
		 * @param arr The array to push.
		 * @return This array that was modified.
		 */
		array& push_number(const number& num);
		/**
		 * @brief Pushes a JSON value type 'number' into the end of this array by move.
		 *
		 * @param rval Rvalue reference to the 'number' (will be moved).
		 * @return This array that was modified.
		 */
		array& push_number(number&& rval);
		/**
		 * @brief Emplaces a JSON value type 'number' with the ctor args.
		 *
		 * @param arr The array to push.
		 * @return This array that was modified.
		 */
		template<typename... CtorArgs>
		array& emplace_number(CtorArgs&&... args) {
			m_arr.emplace_back<number>(std::forward(args)...);
			return *this;
		}
		/**
		 * @brief Pushes a JSON value type 'object' into the end of this array by copy.
		 *
		 * @param arr The array to push.
		 * @return This array that was modified.
		 */
		array& push_object(const object& obj);
		/**
		 * @brief Pushes a JSON value type 'object' into the end of this array by move.
		 *
		 * @param rval Rvalue reference to the 'object' (will be moved).
		 * @return This array that was modified.
		 */
		array& push_object(object&& rval);
		/**
		 * @brief Emplaces a JSON value type 'object' with the ctor args.
		 *
		 * @param arr The array to push.
		 * @return This array that was modified.
		 */
		template<typename... CtorArgs>
		array& emplace_object(CtorArgs&&... args) {
			m_arr.emplace_back<object>(std::forward(args)...);
			return *this;
		}
		/**
		 * @brief Pushes a JSON value type 'string' into the end of this array by copy.
		 *
		 * @param arr The array to push.
		 * @return This array that was modified.
		 */
		array& push_string(const string& str);
		/**
		 * @brief Pushes a JSON value type 'string' into the end of this array by move.
		 * 
		 * @param rval Rvalue reference to the 'string' (will be moved).
		 * @return This array that was modified.
		 */
		array& push_string(string&& rval);
		/**
		 * @brief Emplaces a JSON value type 'string' with the ctor args.
		 *
		 * @param arr The array to push.
		 * @return This array that was modified.
		 */
		template<typename... CtorArgs>
		array& emplace_string(CtorArgs&&... args) {
			m_arr.emplace_back<string>(std::forward(args)...);
			return *this;
		}
		/*
		* @brief Removes the last element from the array.
		*/
		void pop() noexcept;
		/*
		* @brief Clears the array.
		*/
		void clear() noexcept;
		/*
		* @brief Erases the element at the provided iterator.
		* @param pos The position of the element to erase.
		* @returns The iterator following the deleted element.
		*/
		constexpr iterator erase(iterator pos) noexcept;
		/*
		* @brief Erases the element at the provided const iterator.
		* This function is marked constexpr.
		* @param pos The const iterator.
		* @returns The iterator following the deleted element.
		*/
		constexpr iterator erase(const_iterator pos) noexcept;
		/*
		* @brief Erases the elements in the provided iterator range.
		* @param first The first iterator.
		* @param last The last iterator.
		* @returns The iterator following the last deleted element.
		*/
		constexpr iterator erase(iterator first, iterator last) noexcept;
		/*
		* @brief Erases the elements in the provided const iterator range.
		* @param first The first const iterator.
		* @param second The second const iterator.
		* @returns The iterator following the last deleted element.
		*/
		constexpr iterator erase(const_iterator first, const_iterator last) noexcept;
	public:
		/*
		* @brief Gets the size of the array.
		* @returns The size of this array.
		*/
		size_t size() const noexcept;
		/*
		* @brief Gets if this array is empty or not.
		* @returns True/false if this array is empty or not.
		*/
		bool is_empty() const noexcept;
	protected:
		/*
		* @copydoc value::clone_impl
		*/
		virtual array* clone_impl() const override {
			return new array(*this);
		}
	public:
		/*
		* @copydoc json::value::to_string
		*/
		std::string to_string() const override {
			std::ostringstream ss;
			ss << '[';
			for (size_t i = 0; i < m_arr.size() - 1; i++) {
				ss << *m_arr[i];
				ss << ',';
			}
			ss << *m_arr[m_arr.size() - 1];
			ss << ']';
			return ss.str();
		}
		virtual iterator begin() noexcept override {
			return m_arr.begin();
		}
		virtual iterator end() noexcept override {
			return m_arr.end();
		}
		virtual reverse_iterator rbegin() noexcept override {
			return m_arr.rbegin();
		}
		virtual reverse_iterator rend() noexcept override {
			return m_arr.rend();
		}
		virtual const_iterator cbegin() const noexcept override {
			return m_arr.cbegin();
		}
		virtual const_iterator cend() const noexcept override {
			return m_arr.cend();
		}
		virtual const_reverse_iterator crbegin() const noexcept override {
			return m_arr.crbegin();
		}
		virtual const_reverse_iterator crend() const noexcept override {
			return m_arr.crend();
		}
	private:
		/*
		* @brief Internal array element container. Every element is a valid non-'nullptr' element - although it could be a json::null.
		*/
		array_container m_arr{};
};
}
