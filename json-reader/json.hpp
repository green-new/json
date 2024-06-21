#include <string>
#include <fstream>
#include <set>
#include <unordered_map>
#include <map>
#include <type_traits>
#include <cinttypes>
#include <memory>
#include <initializer_list>

#pragma once

namespace json {

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

	// Basic json node (key, value pair, value being a value, object, or array)
	class node {
	public:
		node() : m_name({}) { }
		explicit node(const types::string& name) 
			: m_name(name) { }
		node(const node& other) {
			m_name = other.m_name;
		}
		node(node&& other) noexcept {
			std::swap(m_name, other.m_name);
		}
		node& operator=(const node& other) {
			*this = node(other);
			return *this;
		}
		node& operator=(node&& other) noexcept {
			*this = node(other);
			return *this;
		}
		virtual ~node() {}
	public:
		types::string m_name{};
	};

	using node_ptr = std::unique_ptr<node>;

	template<typename JsonType>
	class value : public node {
	public:
		value() 
			: node(), m_value() {}
		explicit value(const JsonType& value) 
			: node(), m_value(value) { }
		explicit value(const types::string& name, const JsonType& value)
			: node(name), m_value(value) { }
		value(const value& other) {
			this->m_name = other.m_name;
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
		value& operator=(JsonType temp) {
			m_value = temp;
		}
	public:
		JsonType m_value{};
	};

	class array : public node {
	public:
		array() 
			: node() { }
		explicit array(const types::string& name)
			: node(name) { }
		array(const array& other) {
			this->m_name = other.m_name;

			m_arr.clear();
			for (const auto& elem : other.m_arr) {
				m_arr.push_back(std::make_unique<node>(*elem));
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
		void push(node* ptr) {
			m_arr.push_back(std::unique_ptr<node>(ptr));
		}
		void push(std::initializer_list<node*> list) {
			for (node* elem : list) {
				push(elem);
			}
		}
		void pop() {
			m_arr.pop_back();
		}
	public:
		std::vector<node_ptr> m_arr{};
	};

	class object : public node {
	public:
		object() 
			: node(), m_props() { }
		explicit object(const types::string& name)
			: node(name) { }
		object(const object& other) {
			this->m_name = other.m_name;

			// Cannot copy unique ptrs. Must do a deep manual copy
			m_props.clear();
			for (const auto& [key, nodePtr] : other.m_props) {
				if (nodePtr) {
					m_props.insert({ key, std::make_unique<node>(*nodePtr) });
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
		template<typename JsonType>
		value<JsonType>& addValue(const types::string& name, const JsonType& temp) {
			m_props.insert({ name, std::make_unique<value<JsonType>>(name, temp) });
			return (value<JsonType>&) *m_props[name];
		}
		array& addArray(const types::string& name) {
			m_props.insert({ name, std::make_unique<array>(name) });
			return (array&) *m_props[name];
		}
		object& addObject(const types::string& name) {
			m_props.insert({ name, std::make_unique<object>(name) });
			return (object&) *m_props[name];
		}
	private:
		std::map<types::string, node_ptr> m_props{};
	};
}