#include <iostream>
#include <fstream>
#include <uchar.h>
#include "json.hpp"
#include "reader.hpp"
#include "strmanip.hpp"

std::string build_bad_string() {
	std::ifstream f;
	f.open("blns.txt", std::ios_base::binary);
	std::stringstream b;
	b << f.rdbuf();
	return b.str();
}

int rmws_test() {
	std::string s = build_bad_string();
	//json::rmws(s, s.begin(), s.end());
	std::cout << s << '\n';
	return 0;
}

int json_test2() {
	// Maybe just assume all json strings are utf8 (web standard) (nvm its cooked)
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
		std::cout << json[i];
	}
	std::cout << std::endl;
	// json::reader reader(json);
	
	return 0;
}

int json_test() {
	using namespace std::string_literals;
	json::object map("parcel");
	auto& parcelObj = map.insert<json::object>("199.18-3-6");
	parcelObj.insert<json::number<json::storage_policy::integer>>("valuation", 1e6);
	parcelObj.insert<json::string>("bedrooms", "3"s);
	parcelObj.insert<json::string>("address", "my address"s);
	parcelObj.insert<json::string>("municipality", "my town"s);
	auto& bool1 = parcelObj.insert<json::boolean>("paid", false);
	bool1 = true;
	auto& arr = parcelObj.insert<json::array>("geodata");
	arr.push<json::string>("hello"s);

	std::cout << map;

	return 0;
}

int main(int argc, char** argv) {
	int exit = 0;

	exit = json_test();
	// exit = json_test2();
	// exit = rmws_test();

	return exit;
}