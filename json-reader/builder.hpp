#pragma once
#include "json.hpp"

namespace json {
	/**
	* @todo Abstract builder class. To be renamed.
	*/
	template<typename T, typename Parent>
	class builder {
	protected:
		template<typename... CtorArgs>
		builder(const Parent& parent, CtorArgs... args)
			: m_parent(parent), m_object(std::make_unique<T>(args...)) { }
		virtual builder() = default;
	public:
		virtual Parent finish() noexcept {
			return m_parent;
		}
	public:
		std::unique_ptr<T> m_object;
		Parent& m_parent;
	};


































	/**
	* @class json::array_builder
	*
	* @brief Builder pattern for JSON arrays.
	*/
	template<typename Parent>
	class array_builder final : public builder<array, Parent&> {
	public:
		array_builder(const Parent& parent)
			: builder<array, Parent>(parent) {}
		~array_builder() = default;
	public:
		void null() {
			this->m_object->
		}
	};

	/**
	* @class json::obj_builder
	*
	* @brief Builder pattern for JSON objects.
	*/
	template<typename Parent>
	class obj_builder final : public builder<object, Parent&> {
	public:
		obj_builder(const Parent& parent, const std::string& name)
			: builder<object, Parent&>(parent) {}
		~obj_builder() = default;
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
			: builder<root, root&&>(m_object) { }
		~root_builder() = default;
		/**
		* Modifiers.
		*/
	public:
		root&& finish() noexcept override {
			return std::move(*m_object.get());
		}
	};

}
