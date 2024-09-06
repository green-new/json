#include <string>
#include "value.hpp"

namespace json {
/**
* @class json::boolean
* @brief Contains a simple true/false value. Cannot contain 'nothing' or 'null'.
*/
class boolean final : public json::value {
public:
	/**
	* @brief Default ctor cannot instantiate an 'empty' boolean type.
	*/
	boolean() = delete;
	boolean(bool val)
		: m_boolean(val) {}
	~boolean() = default;
public:
	operator bool() const;
public:
	void set(bool val);
public:
	/*
	* @copydoc json::value::to_string
	*/
	std::string to_string() const override {
		return m_boolean ? "true" : "false";
	}
protected:
	/*
	* @copydoc value::clone_impl
	*/
	boolean* clone_impl() const override {
		return new boolean(*this);
	}
private:
	bool m_boolean;
};
}
