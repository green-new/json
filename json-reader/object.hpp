#pragma once

#include "boolean.hpp"
#include "iterable.hpp"
#include "null.hpp"
#include "number.hpp"
#include <sstream>
#include "string.hpp"

namespace json {
	// 'array' forward declaration
	class array;

	/**
	* @class json::object
	*
	* @extends json::value
	*
	* @brief Represents a JSON object.
	* Contains a map of ["name", value] pairs. Keeps track of the names of JSON values.
	* Values cannot be nullptr.
	* Duplicate named members follow the same rules as the standard library's @c std::map::insert.
	*/
	class object final : public value, public iterable<prop_map> {
	public:
		object()
			: m_name(), m_props() {}
		explicit object(const std::string& name)
			: m_name(name), m_props() {}
		object(const object& other)
			: m_name(other.m_name) {
			// Cannot copy unique ptrs. Must do a deep manual copy
			m_props.clear();
			for (const auto& [key, val] : other.m_props) {
				if (val) {
					// Copy ctor called
					m_props.insert({ key, val->clone() });
				}
			}
		}
		object(object&& other) noexcept 
			: m_name(std::move(other.m_name)), m_props(std::move(other.m_props)) {}
		object& operator=(const object& other) {
			if (this != &other) {
				*this = object(other);
			}
			return *this;
		}
		object& operator=(object&& other) noexcept {
			if (this != &other) {
				*this = object(other);
			}
			return *this;
		}
		~object() {}
	public:
		/*
		* @brief Returns the number of members in this object.
		*
		* @returns Number of members in this object.
		*/
		size_t size() const noexcept;
		/**
		* @brief Determines if this object is empty.
		*
		* @return True if empty, false if non-empty.
		*/
		bool empty() const noexcept;
		/**
		 * @brief Determines if this and another object are equal by contents.
		 * 
		 * @param rhs The other object.
		 * @return True if equal, false if not.
		 */
		bool operator==(const object& rhs) const;
		/**
		 * @brief Determines if this object contains the given key/name.
		 * 
		 * @param name The name of the JSON value.
		 * @return True if this object contains this key, false if not.
		 */
		bool contains(const std::string& name) const;
	public:
		/**
		 * @brief Inserts a JSON value type 'array' into this object by copy.
		 *
		 * @param arr The array to push.
		 * @param name The name of the array.
		 * @return This object that was modified.
		 */
		object& insert_array(const std::string& name, const array& arr);
		/**
		 * @brief Emplaces a JSON value type 'array' with the ctor args into this object.
		 *
		 * @tparam CtorArgs List of ctor argument types.
		 * @param name The name of the array.
		 * @param arr The array to push.
		 * @return This object that was modified.
		 */
		template<typename... CtorArgs>
		object& emplace_array(const std::string& name, CtorArgs&&... args) {
			m_props.emplace<std::unique_ptr<array>>(name, std::forward(args)...);
			return *this;
		}
		/**
		 * @brief Inserts a JSON value type 'boolean' into this object by copy.
		 *
		 * @param name The name of the boolean.
		 * @param b The boolean to insert.
		 * @return This object that was modified.
		 */
		object& insert_boolean(const std::string& name, const boolean& b);
		/**
		 * @brief Emplaces a JSON value type 'boolean' with the ctor args into this object.
		 *
		 * @tparam CtorArgs List of ctor argument types.
		 * @param name The name of the boolean.
		 * @param args The ctor args.
		 * @return This object that was modified.
		 */
		template<typename... CtorArgs>
		object& emplace_boolean(const std::string& name, CtorArgs&&... args) {
			m_props.emplace<std::unique_ptr<boolean>>(name, std::forward(args)...);
			return *this;
		}
		/**
		 * @brief Inserts a JSON value type 'null' into this object by copy.
		 *
		 * @param name The name of the null value.
		 * @param n The null to insert.
		 * @return This object that was modified.
		 */
		object& insert_null(const std::string& name, const null& n);
		/**
		 * @brief Emplaces a JSON value type 'null'.
		 * 
		 * @param name The name of the null value.
		 * @return This object that was modified.
		 */
		object& emplace_null(const std::string& name) {
			m_props.try_emplace(name);
			return *this;
		}
		/**
		 * @brief Inserts a JSON value type 'number' into this object by copy.
		 *
		 * @param name The name of the number.
		 * @param num The number to insert.
		 * @return This object that was modified.
		 */
		object& insert_number(const std::string& name, const number& num);
		/**
		 * @brief Emplaces a JSON value type 'number' with the ctor args into this object.
		 *
		 * @tparam CtorArgs List of ctor argument types.
		 * @param name The name of the number.
		 * @param args The number args.
		 * @return This object that was modified.
		 */
		template<typename... CtorArgs>
		object& emplace_number(const std::string& name, CtorArgs&&... args) {
			m_props.emplace<std::unique_ptr<number>>(name, std::forward<CtorArgs>(args)...);
			return *this;
		}
		/**
		 * @brief Inserts a JSON value type 'object' into this object by copy.
		 *
		 * @param name The name of the object.
		 * @param obj The object to insert.
		 * @return This object that was modified.
		 */
		object& insert_object(const std::string& name, const object& obj);
		/**
		 * @brief Emplaces a JSON value type 'object' with the ctor args into this object.
		 *
		 * @tparam CtorArgs List of ctor argument types.
		 * @param name The name of the object.
		 * @param args The ctor args for the object.
		 * @return This object that was modified.
		 */
		template<typename... CtorArgs>
		object& emplace_object(const std::string& name, CtorArgs&&... args) {
			m_props.emplace<std::unique_ptr<object>>(name, std::forward<CtorArgs>(args)...);
			return *this;
		}
		/**
		 * @brief Inserts a JSON value type 'string' into this object by copy.
		 *
		 * @param name The name of the string.
		 * @param str The array to insert.
		 * @return This object that was modified.
		 */
		object& insert_string(const std::string& name, const string& str);
		/**
		 * @brief Emplaces a JSON value type 'string' with the ctor args into this object.
		 *
		 * @tparam CtorArgs List of ctor argument types.
		 * @param name The name of the string.
		 * @param The ctor args for the string.
		 * @return This object that was modified.
		 */
		template<typename... CtorArgs>
		object& emplace_string(const std::string& name, CtorArgs&&... args) {
			m_props.emplace<std::unique_ptr<string>>(name, std::forward<CtorArgs>(args)...);
			return *this;
		}
		/**
		 * @brief Emplaces an rvalue 'array' into this object.
		 * 
		 * @param name Name of the array.
		 * @param rval Rvalue reference to the array (will be moved).
		 * @return This 'object' that was modified.
		 */
		object& insert_array(const std::string& name, array&& rval);
		/**
		 * @brief Emplaces an rvalue 'boolean' into this object.
		 *
		 * @param name Name of the boolean.
		 * @param rval Rvalue reference to the 'boolean' (will be moved).
		 * @return This 'object' that was modified.
		 */
		object& insert_boolean(const std::string& name, boolean&& rval);
		/**
		 * @brief Emplaces an rvalue 'null' into this object.
		 *
		 * @param name Name of the null value.
		 * @param rval Rvalue reference to the 'null' (will be moved).
		 * @return This 'object' that was modified.
		 */
		object& insert_null(const std::string& name, null&& rval);
		/**
		 * @brief Emplaces an rvalue 'number' into this object.
		 *
		 * @param name Name of the number.
		 * @param rval Rvalue reference to the 'number' (will be moved).
		 * @return This 'object' that was modified.
		 */
		object& insert_number(const std::string& name, number&& rval);
		/**
		 * @brief Emplaces an rvalue 'object' into this object.
		 *
		 * @param name Name of the object.
		 * @param rval Rvalue reference to the 'object' (will be moved).
		 * @return This 'object' that was modified.
		 */
		object& insert_object(const std::string& name, object&& rval);
		/**
		 * @brief Emplaces an rvalue 'string' into this object.
		 *
		 * @param name Name of the string.
		 * @param rval Rvalue reference to the 'string' (will be moved).
		 * @return This 'object' that was modified.
		 */
		object& insert_string(const std::string& name, string&& rval);
	public:
		/*
		* @brief Get the string representation of this 'object'.
		* @return String representation.
		*/
		std::string to_string() const override {
			std::ostringstream ss;
			ss << '\"' << m_name << "\":";
			ss << '{';
			std::string padding = "";
			for (const auto& [key, val] : m_props) {
				ss << padding;
				ss << '\"' << key << "\":";
				ss << *val;
				padding = ",";
			}
			ss << '}';
			return ss.str();
		}
		/**
		* @brief Clone implementation.
		* @return Raw pointer deep copy of this 'object'.
		*/
		virtual object* clone_impl() const override {
			return new object(*this);
		}
		/**
		 * @brief Equals implementation.
		 *
		 * @param rhs The other value.
		 * @return True if the value is equal in type and lexiographically, false otherwise.
		 */
		virtual bool equals(value* rhs) const override {
			if (auto rhsobj = dynamic_cast<object*>(rhs)) {
				return rhsobj->m_props == m_props;
			}
			return false;
		}
	public:
		/**
		* @brief Begin iterator.
		* @return The begin iterator of the container.
		*/
		virtual iterator begin() noexcept override {
			return m_props.begin();
		}
		/**
		* @brief End iterator.
		* @return The end iterator of the container.
		*/
		virtual iterator end() noexcept override {
			return m_props.end();
		}
		/**
		* @brief Reverse begin iterator.
		* @return The beginning of the reverse iterator of the container.
		*/
		virtual reverse_iterator rbegin() noexcept override {
			return m_props.rbegin();
		}
		/**
		* @brief Reverse end iterator.
		* @return The end of the reverse iterator of the container.
		*/
		virtual reverse_iterator rend() noexcept override {
			return m_props.rend();
		}
		/**
		* @brief Begin const iterator.
		* @return The beginning of the const iterator of the container.
		*/
		virtual const_iterator cbegin() const noexcept override {
			return m_props.cbegin();
		}
		/**
		* @brief End const iterator.
		* @return The end of the reverse iterator of the container.
		*/
		virtual const_iterator cend() const noexcept override {
			return m_props.cend();
		}
		/**
		* @brief Begin const reverse iterator.
		* @return The beginning of the const reverse iterator of the container.
		*/
		virtual const_reverse_iterator crbegin() const noexcept override {
			return m_props.crbegin();
		}
		/**
		* @brief End const reverse iterator.
		* @return The end of the const reverse iterator of the container.
		*/
		virtual const_reverse_iterator crend() const noexcept override {
			return m_props.crend();
		}
	private:
		/**
		* @brief The object property (member) map.
		*/
		prop_map m_props{};
		/**
		* @brief The name of this object.
		*/
		std::string m_name;
	};
}
