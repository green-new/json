#include "pch.h"
#include "../json-reader/array.hpp"
#include "../json-reader/object.hpp"

// Array tests
class array_testf : public testing::Test {
public:
	void SetUp() override{
		m_arr.push_number(json::number{ 45.43f })
			.push_number(json::number{ 123.43f })
			.push_number(json::number{ 54.0f })
			.push_number(json::number{ 45.1243f })
			.push_number(json::number{ 93.43f });

		m_arr.push_array(json::array{ })
			.push_array(json::array{ })
			.push_array(json::array{ })
			.push_array(json::array{ })
			.push_array(json::array{ });

		m_arr.push_boolean(json::boolean { true })
			.push_boolean(json::boolean{ false })
			.push_boolean(json::boolean{ false })
			.push_boolean(json::boolean{ true })
			.push_boolean(json::boolean{ true });

		m_arr.push_null(json::null{});

		m_arr.push_object(json::object{})
			.push_object(json::object{})
			.push_object(json::object{})
			.push_object(json::object{})
			.push_object(json::object{})
			.push_object(json::object{});

		m_arr.push_string(json::string{ "Hello" })
			.push_string(json::string{ "World" })
			.push_string(json::string{ "Orange" })
			.push_string(json::string{ "pumpkin" })
			.push_string(json::string{ "pie" })
			.push_string(json::string{ "apple" });
	}
protected:
	json::array m_arr;
};

TEST_F(array_testf, ArrayAt) {
	ASSERT_EQ(m_arr.at(0), 45.43f);
	ASSERT_EQ(m_arr.at(0), 45.43f);
	ASSERT_EQ(m_arr.at(0), 45.43f);
	ASSERT_EQ(m_arr.at(0), 45.43f);
	ASSERT_EQ(m_arr.at(0), 45.43f);
	ASSERT_EQ(m_arr.at(0), 45.43f);
	ASSERT_EQ(m_arr.at(0), 45.43f);
	ASSERT_EQ(m_arr.at(0), 45.43f);
	ASSERT_EQ(m_arr.at(0), 45.43f);
	ASSERT_EQ(m_arr.at(0), 45.43f);
	ASSERT_EQ(m_arr.at(0), 45.43f);
	ASSERT_EQ(m_arr.at(0), 45.43f);
	ASSERT_EQ(m_arr.at(0), 45.43f);
	ASSERT_EQ(m_arr.at(0), 45.43f);
	ASSERT_EQ(m_arr.at(0), 45.43f);
	ASSERT_EQ(m_arr.at(0), 45.43f);
}

TEST(ArrayTests, ArrayOperatorAccess) {

}

TEST(ArrayTests, ArrayFront) {

}

TEST(ArrayTests, ArrayBack) {

}

// Boolean tests

// Number tests

// Object tests

// Root tests

// String tests
