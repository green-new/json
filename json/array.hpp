#pragma once

#include "boolean.hpp"
#include <concepts>
#include <functional>
#include "iterable.hpp"
#include "null.hpp"
#include "number.hpp"
#include "object.hpp"
#include <set>
#include "string.hpp"
#include <sstream>
#include <typeinfo>
#include <utility>
#include <vector>

namespace json {
	// Array container type (owning)
	using array_container = std::vector<value_ptr>;

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
			: m_arr(std::move(other.m_arr)) {}
		array& operator=(const array& other) {
			m_arr.clear();
			if (this != &other) {
				for (const auto& elem : other.m_arr) {
					// Clone the value (runtime polymorphism)
					m_arr.push_back(elem->clone());
				}
			}
			return *this;
		}
		array& operator=(array&& other) noexcept {
			m_arr.clear();
			if (this != &other) {
				m_arr = std::move(other.m_arr);
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
		inline array_container::value_type::element_type& at(size_t index) {
			return *m_arr.at(index);
		}
		/*
		* @brief Gets the element at the provided index.
		* An index that is not within the bounds is undefined behavior.
		* @param index The index of the element within the array.
		*/
		inline json::value& operator[](size_t index) {
			return *m_arr[index];
		}
		/**
		* @brief Get the front 'value' in the array.
		* An index that is not within the bounds is undefined behavior.
		* @return The front 'value', type erased.
		*/
		inline json::value& front() noexcept {
			return *m_arr.front();
		}
		/**
		* @brief Get the back 'value' in the array.
		* An index that is not within the bounds is undefined behavior.
		* @return The back 'value', type erased.
		*/
		inline json::value& back() noexcept {
			return *m_arr.back();
		}
		/**
		* @brief Get the underlying contiguous pointer to the array, pointing to the first element.
		* An uninitalized empty array cannot be dereferenced.
		* @return The pointer to the first element, if it exists.
		*/
		inline array_container::value_type* data() noexcept {
			return m_arr.data();
		}
	public:
		/**
		 * @brief Copies and pushes a JSON value into the end of this array.
		 *
		 * @tparam JsonValueType The type of the JSON value to copy and push to the end of the array.
		 * @param value The value to copy.
		 * @return This array.
		 */
		template<std::derived_from<value> JsonValueType>
		array& push(const JsonValueType& value) {
			m_arr.push_back(std::make_unique<JsonValueType>(value)); // Copy ctor
			return *this;
		}
		/**
		 * @brief Moves and pushes a JSON value into the end of this array.
		 *
		 * @tparam JsonValueType The type of the JSON value to move and push to the end of the array.
		 * @param value The value to copy.
		 * @return This array.
		 */
		template<std::derived_from<value> JsonValueType>
		array& push(JsonValueType&& value) {
			m_arr.push_back(std::make_unique<JsonValueType>(std::forward<JsonValueType>(value))); // Move ctor
			return *this;
		}
		/**
		 * @brief Creates and pushes in-place a JSON value into the end of this array.
		 *
		 * @tparam JsonValueType The type of the JSON value to move and push to the end of the array.
		 * @tparam CtorArgs The list of constructor argument types for the JsonValueType.
		 * @param value The value to copy.
		 * @return This array.
		 */
		template<std::derived_from<value> JsonValueType, typename... CtorArgs>
			requires std::constructible_from<JsonValueType, CtorArgs...>
		array& emplace(CtorArgs&&... args) {
			m_arr.push_back(std::make_unique<JsonValueType>(std::forward<CtorArgs>(args)...));
			return *this;
		}
		/*
		 * @brief Removes the last element in the array and returns it.
		 *
		 * @return Last element in the array.
		 */
		array_container::value_type pop() noexcept;
		/*
		 * @brief Clears the array.
		 */
		void clear() noexcept;
		/**
		 * @brief Search the array for the provided value by contents.
		 *
		 * @param json_value The json value, type erased.
		 * @return The iterator position of the element found, end() otherwise.
		 */
		iterator find(const value& json_value);
		/**
		 * @brief Search the array based on the provided predicate.
		 *
		 * @param predicate The predicate.
		 * @return The iterator position of the first element found if the predicate is valid, end() otherwise.
		 */
		template<std::predicate<const value&, size_t> Predicate>
		iterator find_if(Predicate predicate) {
			size_t i = 0;
			for (iterator it = begin(); it != end(); it++) {
				if (predicate(**it, i++)) {
					return it;
				}
			}
			return end();
		}
		/**
		 * @brief Gets a set of the references to elements in the array of type JsonValueType.
		 *
		 * @tparam JsonValueType The type of the JSON value references to retrieve.
		 * @return The set of JSON values of the provided JSON value type.
		 */
		template<std::derived_from<value> JsonValueType>
		std::set<JsonValueType> of() {
			std::set<JsonValueType> res{};
			for (const_iterator it = cbegin(); it != cend(); it++) {
				try {
					JsonValueType& ref = dynamic_cast<JsonValueType&>(**it);
					res.insert(ref);
				} catch (const std::bad_cast& e) {
					throw e;
				}
			}
			return res;
		}
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
		/*
		 * @brief Cast the provided value pointer to a reference of the provided JSON value type.
		 * 
		 * @throws std::bad_cast if the provided ptr cannot be cast to the provided type.
		 * @tparam JsonValueType Type of the JSON value.
		 * @param ptr Value pointer to the JSON value.
		 * @return Reference to the casted type.
		 */
		template<std::derived_from<value> JsonValueType>
		static JsonValueType& deref(array_container::value_type& ptr) const {
			JsonValueType* o;
			try {
				// value_ptr -> value* -> cast to JsonValueType* -> set to o
				o = dynamic_cast<JsonValueType*>(ptr.get());
			} catch (const std::bad_cast& e ) {
				throw e;
			}
			return *o;
		}
	public:
		/*
		 * @brief Gets the size of the array.
		 * @returns The size of this array.
		 */
		size_t size() const noexcept;
		/*
		 * @brief Determines if this array is empty or not.
		 * @returns True/false if this array is empty or not.
		 */
		bool empty() const noexcept;
	protected:
		/*
		 * @brief Clone implementation.
		 *
		 * @return Raw pointer deep copy of this array.
		 */
		virtual array* clone_impl() const override {
			return new array(*this);
		}
		/**
		 * @brief Equals implementation.
		 *
		 * @param rhs The other value.
		 * @return True if the value is equal in type and lexiographically, false otherwise.
		 */
		virtual bool eq_impl(const value* rhs) const override {
			if (const auto rhsarr = dynamic_cast<const array*>(rhs)) {
				return rhsarr->m_arr == m_arr;
			}
			return false;
		}
	public:
		/*
		 * @brief String representation of this array.
		 *
		 * @return String representation.
		 */
		std::string to_string() const override {
			std::ostringstream ss;
			ss << '[';
			std::string sep = "";
			for (const auto& elem : m_arr) {
				ss << sep;
				ss << *(elem.get());
				sep = ",";
			}
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
