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
	json::root_builder b{};
	
	// Example builder (how we want the client to see the api)
	json::root obj = 
		// Create an object at the root called "parcel"
		b.insert("parcel")
			// Create an object at the object "parcel" called "199.8-3-6"
			.insert<json::object>("199.8-3-6")
				// Insert a number called "valuation" equal to 1e6, etc...
				.insert<json::number<json::storage_policy::uinteger>>("valuation", 1e6)
				.insert<json::boolean>("paid_taxes", false)
				.insert<json::string>("owner", "me")
				// Insert an array called "geodata"
				.insert<json::array>("geodata")
					// Insert a number into the array "geodata" with the value 42.06f, etc...
					.insert<json::number<json::storage_policy::floating>>(42.06f)
					.insert<json::boolean>(false)
					// Push a "null" value into the array
					.insert<json::nothing>()
					// Finish building the array "geodata"
					.build()
				// Finish building the object "199.8-3-6"
				.build()
			// Finish building the object "parcel"
			.build()
		// Finish building the root node
		.complete();

	std::cout << obj;

	return 0;
}

int main(int argc, char** argv) {
	int exit = 0;

	exit = json_test();
	// exit = json_test2();
	// exit = rmws_test();

	return exit;
}