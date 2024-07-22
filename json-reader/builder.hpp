#pragma once
#include "json.hpp"

namespace json {
	/**
	* @todo LOTS of code dupliation here, maybe we can get rid of it with policy pattern or adding to our abstract class.
	*/
	
	/**
	* @todo Abstract builder class. To be renamed.
	*/
	template<typename T, typename Escape>
	class builder {
	protected:
		builer(T& obj, Escape& esc) 
			: m_object(obj), m_escape(esc) { }
		template<typename... CtorArgs>
		builder(Escape& esc, CtorArgs&& ...args)
			: m_object(T(std::forward(args)...)), m_escape(esc) { }
		virtual builder() = default;
	public:
		/** @todo Something like json::value::clone() here for number, boolean, string, null */
		virtual Escape finish() noexcept {
			return m_escape;
		}
	public:
		T& m_object;
		Escape m_escape;
	};

	/**
	* @class json::array_builder
	*
	* @brief Builder pattern for JSON arrays.
	*/
	template<typename Escape>
	class array_builder final : public builder<array, Escape> {
	public:
		array_builder(const Escape& esc)
			: builder<array, Escape>(esc) {}
		~array_builder() = default;
	public:
		template<typename... CtorArgs>
		obj_builder object(CtorArgs&&... args) { // Cannot return by reference or else we get a dangling reference
			return obj_builder<array_builder>(*this, this->m_object->push<object>(std::forward(args)...));
		}
		template<typename... CtorArgs>
		array_builder array(CtorArgs&&... args) { // Cannot return by reference or else we get a dangling reference
			return array_builder<array_builder>(*this, this->m_object->push<array>(std::forward(args)...));
		}
		template<typename Storage>
		array_builder& number(Storage number) {
			this->m_object->push<number<Storage>>(number);
			return *this;
		}
		array_builder& boolean(bool b) {
			this->m_object->push<boolean>(b);
			return *this;
		}
		array_builder& string(std::string&& str) {
			this->m_object->push<string>(str);
			return *this;
		}
		array_builder& null() {
			// nullptr_t? json null would be better
			this->m_object->push<std::nullptr_t>(nullptr);
			return *this;
		}
	};

	/**
	* @class json::obj_builder
	*
	* @brief Builder pattern for JSON objects.
	*/
	template<typename Escape>
	class obj_builder final : public builder<object, Escape> {
	public:
		obj_builder(const Escape& esc, const std::string& name)
			: builder<object, Escape&>(esc) {}
		~obj_builder() = default;
	public:
		template<typename... CtorArgs>
		obj_builder object(const std::string& name, CtorArgs&&... args) { // Cannot return by reference or else we get a dangling reference
			return obj_builder<array_builder>(*this, this->m_object->insert<object>(name, std::forward(args)...));
		}
		template<typename... CtorArgs>
		array_builder array(const std::string& name, CtorArgs&&... args) { // Cannot return by reference or else we get a dangling reference
			return array_builder<array_builder>(*this, this->m_object->insert<array>(name, std::forward(args)...));
		}
		template<typename Storage>
		obj_builder& number(const std::string& name, Storage number) {
			this->m_object->insert<number<Storage>>(name, number);
			return *this;
		}
		obj_builder& boolean(const std::string& name, bool b) {
			this->m_object->insert<boolean>(name, b);
			return *this;
		}
		obj_builder& string(const std::string& name, std::string&& str) {
			this->m_object->insert<string>(name, str);
			return *this;
		}
		obj_builder& null(const std::string& name) {
			// nullptr_t? json null would be better
			this->m_object->insert<std::nullptr_t>(nullptr);
			return *this;
		}
	};

	/**
	* @class json::root_builder
	*
	* @extends json::builder
	*
	* @brief Builds a json root object.
	*/
	class root_builder final : public builder<root, root&&> {
	/**
	* Class methods.
	*/
	public:
		root_builder()
			: builder<root, root&&>(root{}, this->m_object) { }
		~root_builder() = default;
		/**
		* Modifiers.
		*/
	public:
		template<typename... CtorArgs>
		obj_builder object(const std::string& name, CtorArgs&&... args) { // Cannot return by reference or else we get a dangling reference
			return obj_builder<root_builder&>(*this, this->m_object->insert<object>(name, std::forward(args)...));
		}
		template<typename... CtorArgs>
		array_builder array(const std::string& name, CtorArgs&&... args) { // Cannot return by reference or else we get a dangling reference
			return array_builder<root_builder&>(*this, this->m_object->insert<array>(name, std::forward(args)...));
		}
		template<typename Storage>
		root_builder& number(const std::string& name, Storage number) {
			this->m_object->insert<number<Storage>>(name, number);
			return *this;
		}
		root_builder& boolean(const std::string& name, bool b) {
			this->m_object->insert<boolean>(name, b);
			return *this;
		}
		root_builder& string(const std::string& name, std::string&& str) {
			this->m_object->insert<string>(name, str);
			return *this;
		}
		root_builder& null(const std::string& name) {
			// nullptr_t? json null would be better
			this->m_object->insert<std::nullptr_t>(nullptr);
			return *this;
		}
	public:
		root&& finish() noexcept override {
			return std::move(*m_object);
		}
	};

}
