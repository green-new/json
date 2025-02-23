#include "json.hpp"
#include "test.hpp"
#include "array_tests.hpp"

#include <iostream>

#define TEST(tr, func, str) if ((tr = func) != test_result::SUCCESS) { std::cout << str << '\n'; }

int main(int argc, char** argv) {
	testing test{};
	test_result tr{};

	// Array test
	std::cout << test.m_testarr << '\n'; // Print the array
	TEST(tr, ArrayTest_At(&test), "Array: at() test failed");
	TEST(tr, ArrayTest_OperatorAcc(&test), "Array: [] test failed");
	TEST(tr, ArrayTest_Front(&test), "Array: front() test failed");
	TEST(tr, ArrayTest_Back(&test), "Array: back() test failed");
	TEST(tr, ArrayTest_Data(&test), "Array: data() test failed");
	TEST(tr, ArrayTest_PushLvalue(&test), "Array: push(Lvalue) test failed");
	TEST(tr, ArrayTest_PushRvalue(&test), "Array: push(Rvalue) test failed");
	TEST(tr, ArrayTest_Emplace(&test), "Array: emplace() test failed");
	TEST(tr, ArrayTest_Pop(&test), "Array: pop() test failed");
	TEST(tr, ArrayTest_FindIf(&test), "Array: find_if() test failed");
	TEST(tr, ArrayTest_Erase(&test), "Array: erase(i) test failed");
	TEST(tr, ArrayTest_EraseC(&test), "Array: erase(ci) test failed");
	TEST(tr, ArrayTest_EraseRange(&test), "Array: erase(i, i) test failed");
	TEST(tr, ArrayTest_EraseCRange(&test), "Array: erase(ci, ci) test failed");
	TEST(tr, ArrayTest_Size(&test), "Array: size() test failed");
	TEST(tr, ArrayTest_Empty(&test), "Array: empty() test failed");

	// Boolean test
	std::cout << test.m_testbool << '\n'; // Print the boolean

	// Null test
	std::cout << test.m_testnull << '\n'; // Print the null

	// Number test
	std::cout << test.m_testnumber << '\n'; // Print the number

	// Object test
	std::cout << test.m_testobject << '\n'; // Print the object

	// Root test
	std::cout << test.m_testroot << '\n'; // Print the root

	// String test
	std::cout << test.m_teststr << '\n'; // Print the string

}
