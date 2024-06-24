
#include <iostream>
#include <fstream>
#include "json.hpp"
#include "reader.hpp"

int json_test2() {
	// Maybe just assume all json strings are utf8 (web standard)
	// Need special characters defined in Unicode
	using namespace std::string_literals;
	std::string json = "{"
		"\"Hello\": 123,"
		"\"asdf\" : ["
		"1,"
		"	\"2\","
		"	3.0,"
		"{ \"4\": 4 },"
		"{},"
		"	[\"💀💀💀💀💀💀\"],"
		"null"
		"] ,"
		"\"true\": true,"
		"\"false\" : false,"
		"\"object\" : null,"
		"\"apple\" : {"
		"\"rotting\": false,"
		"\"delicious\" : true,"
		"\"weight\" : 120.0,"
		"\"parents\" : [\"joe\", \"maria\", \"ryan\"],"
		"\"哈\": \"haha\""
		"}"s;
	for (int i = 0; i < json.length(); i++) {
		std::cout << json[i] << ' ';
	}
	std::cout << std::endl;
	// json::reader reader(json);
	
	return 0;
}

int json_test() {
	using namespace std::string_literals;
	json::object map{ "parcel" };
	auto& parcelObj = map.addObject("199.18-3-6");
	parcelObj.addValue("valuation", 1e6);
	parcelObj.addValue("bedrooms", "3"s);
	parcelObj.addValue("address", "4056 state route 19"s);
	parcelObj.addValue("municipality", "scio"s);
	auto& arr = parcelObj.addArray("geodata");
	arr.push("hello"s);

	json::value<json::types::int32>* val = new json::value{ "hello", 10 };

	return 0;
}

int main(int argc, char** argv) {
	int exit = 0;

	// exit = json_test();
	exit = json_test2();

	return exit;
}