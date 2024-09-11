#pragma once

#include "array.hpp"
#include "boolean.hpp"
#include "iterable.hpp"
#include "null.hpp"
#include "number.hpp"
#include <sstream>
#include "string.hpp"

namespace json {
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
			: m_props() {}
		object(const object& other) {
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
			: m_props(std::move(other.m_props)) {}
		object& operator=(const object& other) {
			if (this != &other) {
				m_props.clear();
				for (const auto& [key, val] : other.m_props) {
					if (val) {
						// Copy ctor called
						m_props.insert({ key, val->clone() });
					}
				}
			}
			return *this;
		}
		object& operator=(object&& other) noexcept {
			if (this != &other) {
				m_props = std::move(other.m_props);
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
		 * @brief Inserts and copies a JSON value into this object.
		 *
		 * @tparam JsonValueType The type of the JSON value.
		 * @param name The name of the array.
		 * @param value The value to be copied.
		 * @return This object that was modified.
		 */
		template<std::derived_from<value> JsonValueType>
		object& insert(const std::string& name, const JsonValueType& value) {
			m_props.insert({ name, std::make_unique<JsonValueType>(value) }); // Copy ctor
			return *this;
		}
		/**
		 * @brief Inserts and moves a JSON value into this object.
		 *
		 * @tparam JsonValueType The type of the JSON value.
		 * @param name The name of the array.
		 * @param value The value to be moved.
		 * @return This object that was modified.
		 */
		template<std::derived_from<value> JsonValueType>
		object& insert(const std::string& name, JsonValueType&& value) {
			m_props.insert({ name, std::make_unique<JsonValueType>(std::forward<JsonValueType>(value)) }); // Move ctor
			return *this;
		}
		/**
		 * @brief Creates and inserts in-place a JSON value into this object.
		 *
		 * @tparam JsonValueType The type of the JSON value.
		 * @tparam CtorArgs The argument list for JsonValueType's ctor.
		 * @param name The name of the array.
		 * @param value The value to be moved.
		 * @return This object that was modified.
		 */
		template<std::derived_from<value> JsonValueType, typename... CtorArgs>
			requires std::constructible_from<JsonValueType, CtorArgs...>
		object& emplace(const std::string& name, CtorArgs&&... args) {
			m_props.try_emplace(name, std::forward(args)...); // Ctor
			return *this;
		}
		template<std::derived_from<value> JsonValueType>
		JsonValueType& get(const std::string& name) {
			return dynamic_cast<JsonValueType&>(*(m_props.at(name).get()));
		}
	public:
		/*
		* @brief Get the string representation of this 'object'.
		* @return String representation.
		*/
		std::string to_string() const override {
			std::ostringstream ss;
			ss << '{';
			std::string sep = "";
			for (const auto& [key, val] : m_props) {
				ss << sep;
				ss << '\"' << key << "\":";
				ss << *(val.get());
				sep = ",";
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
		virtual bool equals(const value* rhs) const override {
			if (const auto rhsobj = dynamic_cast<const object*>(rhs)) {
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
	};
}
