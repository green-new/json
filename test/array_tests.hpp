#pragma once
#include <cassert>
#include "test.hpp"

#define TESTFUNC(funcname, parameters) test_result funcname(parameters)
#define ARRAY test->m_testarr

TESTFUNC(ArrayTest_At, testing* test) {
	TASSERT(ARRAY.at(0), json::array{});
	TASSERT(ARRAY.at(1), json::boolean{false});
	TASSERT(ARRAY.at(2), json::null{});
	TASSERT(ARRAY.at(3), json::number{22.0f});
	TASSERT(ARRAY.at(4), json::object{});
	TASSERT(ARRAY.at(5), json::string{"array string"});
	return test_result::SUCCESS;
}

TESTFUNC(ArrayTest_OperatorAcc, testing* test) {
	TASSERT(ARRAY[0], json::array{});
	TASSERT(ARRAY[1], json::boolean{ false });
	TASSERT(ARRAY[2], json::null{});
	TASSERT(ARRAY[3], json::number{22.0f});
	TASSERT(ARRAY[4], json::object{});
	TASSERT(ARRAY[5], json::string{"array string"});
	return test_result::SUCCESS;
}

TESTFUNC(ArrayTest_Front, testing* test) {
	TASSERT(ARRAY.front(), json::array{});
	return test_result::SUCCESS;
}

TESTFUNC(ArrayTest_Back, testing* test) {
	TASSERT(ARRAY.back(), json::string{"array string"});
	return test_result::SUCCESS;
}

TESTFUNC(ArrayTest_Data, testing* test) {
	TASSERT(ARRAY.data()[0].get(), &ARRAY[0]);
	TASSERT(ARRAY.data()[1].get(), &ARRAY[1]);
	TASSERT(ARRAY.data()[2].get(), &ARRAY[2]);
	TASSERT(ARRAY.data()[3].get(), &ARRAY[3]);
	TASSERT(ARRAY.data()[4].get(), &ARRAY[4]);
	TASSERT(ARRAY.data()[5].get(), &ARRAY[5]);
	return test_result::SUCCESS;
}

TESTFUNC(ArrayTest_PushLvalue, testing* test) {
	// Assumes this must work from initialization
	return test_result::SUCCESS;
}

TESTFUNC(ArrayTest_PushRvalue, testing* test) {
	// Assumes this must work from initialization
	return test_result::SUCCESS;
}

TESTFUNC(ArrayTest_Emplace, testing* test) {
	ARRAY.emplace<json::null>();
	TASSERT(ARRAY.back(), json::null{});
	ARRAY.emplace<json::string>(std::string{ "orange" });
	TASSERT(ARRAY.back(), json::string{"orange"});
	ARRAY.emplace<json::number>(22.23f);
	TASSERT(ARRAY.back(), json::number{ 22.23f });
	ARRAY.emplace<json::object>();
	TASSERT(ARRAY.back(), json::object{});
	ARRAY.emplace<json::array>();
	TASSERT(ARRAY.back(), json::array{});
	ARRAY.emplace<json::boolean>(true);
	TASSERT(ARRAY.back(), json::boolean{true});
	return test_result::SUCCESS;
}

TESTFUNC(ArrayTest_Pop, testing* test) {
	ARRAY.pop();
	TASSERT(ARRAY.back(), json::array{}) // After emplace test
	return test_result::SUCCESS;
}

TESTFUNC(ArrayTest_FindIf, testing* test) {
	struct find_if_predicate {
		bool operator()(const json::value& val, size_t index) {
			return index == 0 && dynamic_cast<const json::array&>(val) == json::array{};
		}
	};
	find_if_predicate pred{};
	TASSERT(**ARRAY.find_if(pred), json::array{});
	return test_result::SUCCESS;
}

TESTFUNC(ArrayTest_Of, testing* test) {
	auto s = ARRAY.of<json::array>();
	TASSERT(s.size(), 1);
	return test_result::SUCCESS;
}

TESTFUNC(ArrayTest_Erase, testing* test) {

	return test_result::SUCCESS;
}

TESTFUNC(ArrayTest_EraseC, testing* test) {

	return test_result::SUCCESS;
}

TESTFUNC(ArrayTest_EraseRange, testing* test) {

	return test_result::SUCCESS;
}

TESTFUNC(ArrayTest_EraseCRange, testing* test) {

	return test_result::SUCCESS;
}

TESTFUNC(ArrayTest_Size, testing* test) {

	return test_result::SUCCESS;
}

TESTFUNC(ArrayTest_Empty, testing* test) {

	return test_result::SUCCESS;
}
