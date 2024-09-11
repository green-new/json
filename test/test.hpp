#pragma once

#include "json.hpp"

#define TASSERT(thing1, thing2) if (!(thing1 == thing2)) { return test_result::FAILED; }

enum class test_result {
	SUCCESS,
	FAILED
};

struct testing {
	testing() {
		// Initialize object; assumes insert works correctly
		m_testobject = json::object{};
		m_testobject.insert("name", json::string{ "Tristen Woodruff" });
		m_testobject.insert("salary", json::number{ 30000.1245f });
		m_testobject.insert("stuff", json::array{ });
		m_testobject.insert("employed", json::boolean{ true });
		m_testobject.insert("nothing", json::null{});
		m_testobject.insert("home", json::object{});

		// Initialize array. Assumes 'push' executes correctly
		m_testarr = m_testobject.get<json::array>("stuff");
		m_testarr.push(json::array{});
		m_testarr.push(json::boolean{false});
		m_testarr.push(json::null{});
		m_testarr.push(json::number{22.0f});
		m_testarr.push(json::object{});
		m_testarr.push(json::string{"array string"});

		// Initialize bool
		m_testbool = json::boolean{true};

		// Initialize null :)
		m_testnull = json::null{};

		// Initialize number :)
		m_testnumber = json::number{1.0f};

		// Initialize root
		m_testroot = json::root{};

		// Initialize string
		m_teststr = json::string{"the string"};
	}
	~testing() = default;
	json::array m_testarr;
	json::boolean m_testbool{ false };
	json::null m_testnull;
	json::number m_testnumber{ 0.0f };
	json::object m_testobject;
	json::root m_testroot;
	json::string m_teststr;
};
