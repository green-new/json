#include <bit>
#include <format>
#include <type_traits>
#include "value.hpp"

namespace json {
/**
* @class json::number
* @brief Represents a JSON number. JSON and JavaScript numbers are doubles, so the class uses a 64-bit floating point number.
* An object of this class type cannot be instantiated or exist without a numerical value.
*/
class number final : public value {
public:
	using number_type = long double;
	static_assert(std::numeric_limits<number_type>::is_iec559 == true,
		"In compilation of the translation unit 'json.hpp', \
		in namespace 'json', class 'json::number', \
		type alias 'number_type': said alias requires a floating \
		point type specified by the IEEE 754-2008 standard, which the current \
		system cannot provide or support");
	number() = delete;
	number(number_type num)
		: m_number(num) {}
	~number() = default;
public:
	/**
	* @brief Add two numbers.
	* @param rhs The second number.
	* @return A new number adding this and rhs.
	*/
	number operator+(const number& rhs);
	/**
	* @brief Subtracts two numbers.
	* @param rhs The second number.
	* @return A new number subtracting this and rhs.
	*/
	number operator-(const number& rhs);
	/**
	* @brief Divides two numbers.
	* @param rhs The second number.
	* @return A new number dividing this and rhs.
	*/
	number operator/(const number& rhs);
	/**
	* @brief Multiplies two numbers.
	* @param rhs The second number.
	* @return A new number multiplying this and rhs.
	*/
	number operator*(const number& rhs);
public:
	/**
	* @brief Return the floating point casted to the type defined by template parameter "Integer".
	* @tparam Integer the integer type.
	* @return This number casted to the integer type.
	*/
	template<typename Integer>
	Integer as_int() {
		return (Integer)m_number;
	}
	/**
	* @brief Determines if the current number is finite using the stdlib.
	* This functions returns true if this number is finite, false if infinite.
	* @return Boolean representing if this number is finite or infinite.
	*/
	inline bool is_finite() noexcept;
	/**
	* @brief Determines if the current number is NaN using the stdlib.
	* This functions returns true if this number is NaN, false if its normal.
	* @return Boolean representing if this number is NaN or normal.
	*/
	inline bool is_nan() noexcept;
	/*
	* @brief Determines if the current number is normal.
	* This function returns @c true if this number is normal, @c false if not.
	* This function is marked noexcept.
	* @return Boolean true/false if this number is normal or not.
	*/
	inline bool is_normal() noexcept;
public:
	/*
	* @brief Get the string representation of this 'number'.
	* @return String representation.
	*/
	std::string to_string() const override {
		return std::format("{}", m_number);
	}
protected:
	/*
	* @brief Clone implementation.
	* @return Raw pointer deep copy of this number.
	*/
	virtual number* clone_impl() const override {
		return new number(*this);
	}
private:
	/* 64-bit floating type. */
	number_type m_number;
};
}
