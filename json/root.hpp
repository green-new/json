#pragma once

#include <sstream>
#include "iterable.hpp"
#include "null.hpp"
#include "value.hpp"
namespace json {
	/**
	* @class json::root
	* @extends json::value
	* @brief The json root class.
	* Contains everything. Properties cannot be 'nullptr'. Rather, they are the 'null' json value.
	*/
	class root final : public value, public iterable<prop_map> {
	public:
		root()
			: m_root() {}
		root(const root& other) {
			// Cannot copy unique ptrs. Must do a deep manual copy
			m_root.clear();
			for (const auto& [key, val] : other.m_root) {
				if (val) {
					// Copy ctor called. Objects cannot be nullptr
					m_root.insert({ key, val->clone() });
				}
			}
		}
		root(root&& other) noexcept
			: m_root(std::move(other.m_root)) {}
		root& operator=(const root& other) {
			if (this != &other) {
				m_root.clear();
				for (const auto& [key, val] : other.m_root) {
					if (val) {
						// Copy ctor called. Objects cannot be nullptr
						m_root.insert({ key, val->clone() });
					}
				}
			}
			return *this;
		}
		root& operator=(root&& other) noexcept {
			if (this != &other) {
				m_root = std::move(other.m_root);
			}
			return *this;
		}
		~root() = default;
	public:
		/**
		* @brief Inserts a new JSON value based on the provided template and ctor arguments.
		* @tparam JsonValue The type of the JSON value to insert.
		* @tparam CtorArgs The arguments to the JsonValue ctor.
		* @param name The name of this value.
		* @param ctorArgs The ctor args of JsonType.
		* @returns Lvalue reference to the newly created object member.
		*/
		template<typename JsonValueType, typename... CtorArgs>
			requires std::constructible_from<JsonValueType, CtorArgs...>
		&& std::derived_from<JsonValueType, json::value>
			root& insert(const std::string& name, CtorArgs... ctorArgs) {
			m_root.insert({ name, std::make_unique<JsonValueType>(std::forward<CtorArgs>(ctorArgs)...) });
			return *this;
		}
		/**
		* @brief Return an lvalue reference to the specified JSON value.
		* @param name Name of the JSON value.
		* @return L-value reference to the object meant for moving.
		*/
		value& get(const std::string& name) {
			return *m_root.at(name);
		}
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
	protected:
		root* clone_impl() const override {
			return new root(*this);
		}
		bool equals(const value* rhs) const override {
			if (const auto rhsroo = dynamic_cast<const root*>(rhs)) {
				return rhsroo->m_root == m_root;
			}
			return false;
		}
	public:
		/**
		* @brief Begin iterator.
		* @return The begin iterator of the container.
		*/
		virtual iterator begin() noexcept override {
			return m_root.begin();
		}
		/**
		* @brief End iterator.
		* @return The end iterator of the container.
		*/
		virtual iterator end() noexcept override {
			return m_root.end();
		}
		/**
		* @brief Reverse begin iterator.
		* @return The beginning of the reverse iterator of the container.
		*/
		virtual reverse_iterator rbegin() noexcept override {
			return m_root.rbegin();
		}
		/**
		* @brief Reverse end iterator.
		* @return The end of the reverse iterator of the container.
		*/
		virtual reverse_iterator rend() noexcept override {
			return m_root.rend();
		}
		/**
		* @brief Begin const iterator.
		* @return The beginning of the const iterator of the container.
		*/
		virtual const_iterator cbegin() const noexcept override {
			return m_root.cbegin();
		}
		/**
		* @brief End const iterator.
		* @return The end of the reverse iterator of the container.
		*/
		virtual const_iterator cend() const noexcept override {
			return m_root.cend();
		}
		/**
		* @brief Begin const reverse iterator.
		* @return The beginning of the const reverse iterator of the container.
		*/
		virtual const_reverse_iterator crbegin() const noexcept override {
			return m_root.crbegin();
		}
		/**
		* @brief End const reverse iterator.
		* @return The end of the const reverse iterator of the container.
		*/
		virtual const_reverse_iterator crend() const noexcept override {
			return m_root.crend();
		}
	private:
		prop_map m_root;
	};
}
