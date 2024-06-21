
#include <iostream>
#include "json.hpp"
#include "parser.hpp"

struct A {
	int a, b, c;
};

int json_test() {
	using namespace std::string_literals;
	json::object map{ "parcel" };
	auto& parcelObj = map.addObject("199.18-3-6");
	parcelObj.addValue("valuation", 1e6);
	parcelObj.addValue("bedrooms", "3"s);
	parcelObj.addValue("address", "4056 state route 19"s);
	parcelObj.addValue("municipality", "scio"s);
	auto& arr = parcelObj.addArray("geodata");
	arr.push({ new json::array("vectors"), new json::value("normalization", 10) });

	return 0;
}

int main(int argc, char** argv) {
	int exit = 0;

	exit = json_test();
	// exit = json2_test();

	return exit;
}