// JsonParser Test Suite
//
// Comprehensive test suite for validating the functionality,
// correctness, and reliability of the Json parser implementation.
//
// Covers:
// - Object lifecycle (construction, copy, and move semantics)
// - Parsing and JSON type inspection
// - Primitive value access (bool, number, string)
// - Array and object navigation
// - Bounds-checked element access
// - Utility functions (size, contains)
// - Equality and inequality comparisons
// - JSON serialization using dump()
// - String escape sequence parsing
// - Unicode escape decoding and surrogate pairs
// - Numeric edge cases and scientific notation
// - Arrays and nested arrays
// - Objects and nested objects
// - Complex nested structures containing mixed JSON types
// - Error handling for invalid element access
//
// Each test validates behavior using assertions and prints a
// success message when completed.

#include <iostream>
#include <string>
#include <cassert>

#include "Json.h"

// Print Helper
void print(const std::string& title) {
	std::cout << "[PASS] " << title << "\n";
}

// Lifecycle
// Tests construction, copy semantics, and move semantics.
void lifecycle() {
	std::cout << "Lifecycle Test\n";

	Json j1 = Json::parse("\"Earth\"");

	assert(j1.isString());
	assert(j1.asString() == "Earth");

	print("Basic Construction");

	Json j2 = j1;

	assert(j2.isString());

	print("Copy Semantics");

	Json j3 = std::move(j1);

	assert(j1.isNull());
	assert(j3.isString());

	print("Move Semantics");


	std::cout << "\n";
}

// Type Inspection
// Verifies JSON type detection after parsing different values.
void typeInspection() {
	std::cout << "Type Inspection Test\n";

	Json j1 = Json::parse("null");
	assert(j1.isNull());

	print("Parse Null");

	Json j2 = Json::parse("true");
	assert(j2.isBool());

	print("Parse Bool");

	Json j3 = Json::parse("89.43");
	assert(j3.isNumber());

	print("Parse Double");

	Json j4 = Json::parse("9801");
	assert(j4.isNumber());

	print("Parse Int");

	Json j5 = Json::parse("\"Mango\"");
	assert(j5.isString());

	print("Parse String");

	Json j6 = Json::parse("\"A\"");
	assert(j6.isString());

	print("Parse Char");

	Json j7 = Json::parse("[34, 782, -98, 7, 0]");
	assert(j7.isArray());

	print("Parse Array");

	Json j8 = Json::parse("{\"name\":\"Mark\",\"age\":24}");
	assert(j8.isObject());

	print("Parse Object");

	std::cout << "\n";
}

// Value Access
// Validates typed accessors for primitive, array, and object values.
void valueAccess() {
	std::cout << "Value Access Test\n";

	Json j1 = Json::parse("true");
	assert(j1.asBool());

	print("asBool");

	Json j2 = Json::parse("1099.43");
	assert(j2.asNumber() == 1099.43);

	print("asNumber - Double");

	Json j3 = Json::parse("-9713");
	assert(j3.asNumber() == -9713);

	print("asNumber - Int");

	Json j4 = Json::parse("\"Mississippi\"");
	assert(j4.asString() == "Mississippi");

	print("asString");

	Json j5 = Json::parse("\"Z\"");
	assert(j5.asString() == "Z");

	print("asString - Char");

	Json::ArrayType expectedArr{
		true,
		false,
		false
	};
	Json j6(expectedArr);

	assert(j6.asArray() == expectedArr);

	print("asArray");

	Json::ObjectType expectedObj{
		{"x", 1000},
		{"y", 20.90},
		{"z", -999}
	};
	Json j7(expectedObj);

	assert(j7.asObject() == expectedObj);

	print("asObject");

	std::cout << "\n";
}

// Navigation
// Tests operator[] for accessing array elements and object members.
void navigation() {
	std::cout << "Navigation Test\n";

	Json::ArrayType arr {
		{
			"Coke",
			"Sprite",
			"Pepsi",
			"Dew"
		}
	};

	Json j1(arr);

	assert(j1[0].asString() == "Coke");
	assert(j1[3].asString() == "Dew");
	assert(j1[5].isNull());

	print("Operator[] - Index");

	Json::ObjectType obj {
		{"subject", "Math"},
		{"exam", 98},
		{"grade", 95.45},
		{"isPass", true}
	};

	Json j2(obj);

	assert(j2["subject"].asString() == "Math");
	assert(j2["exam"].asNumber() == 98);
	assert(j2["grade"].asNumber() == 95.45);
	assert(j2["isPass"].asBool() == true);
	assert(j2["subjects"].isNull());

	print("Operator[] - Keys");

	std::cout << "\n";
}

// Element Access
// Verifies bounds-checked access using at() for arrays and objects.
void elementAccess() {
	std::cout << "Element Access Test\n";

	Json::ArrayType arr{
		67.98,
		999.89,
		-789.91,
		0.00541,
		1000.0
	};

	Json j1(arr);

	assert(j1.at(0) == 67.98);
	assert(j1.at(3) == 0.00541);
	assert(j1.at(1) == 999.89);

	try {
		(void)j1.at(5);
		assert(false);
	} catch(std::out_of_range&) {}

	print("At() - Index");

	Json::ObjectType obj{
		{"Money", 99999},
		{"Popularity", 100.00},
		{"Influence", 100}
	};

	Json j2(obj);

	assert(j2.at("Money") == 99999);
	assert(j2.at("Popularity") == 100.00);

	try {
		(void)j2.at("Fame");
		assert(false);
	} catch(std::out_of_range&) {}

	print("At() - Keys");

	std::cout << "\n";
}

// Utility
// Tests helper functions such as size() and contains().
void utility() {
	std::cout << "Utility Test\n";

	Json::ArrayType arr{
		100,
		200,
		400,
		800
	};

	Json j1(arr);

	assert(j1.size() == 4);

	Json::ObjectType obj{
		{"1", "a"},
		{"2", "a"},
		{"3", "c"},
		{"4", true},
		{"5", false}
	};

	Json j2(obj);

	assert(j2.size() == 5);

	print("Size");

	assert(!j1.contains("800"));
	assert(!j1.contains("1600"));
	assert(j2.contains("1"));
	assert(j2.contains("5"));
	assert(!j2.contains("a"));

	print("Contains");

	std::cout << "\n";
}

// Comparison
// Verifies equality and inequality operators.
void comparison() {
	std::cout << "Comparison Test\n";

	Json j1 = Json::parse("\"1111\"");
	Json j2 = Json::parse("1111");
	Json j3 = Json::parse("\"1111\"");
	Json j4 = Json::parse("\"2222\"");

	assert(j1 == j3);

	print("Operator==");

	assert(j1 != j2);
	assert(j1 != j4);

	print("Operator!=");

	std::cout << "\n";
}

// Serialization
// Tests conversion of JSON values back into JSON text using dump().
void serialization() {
	std::cout << "Serialization Test\n";

	Json::ArrayType arr{
		"Spaghetti",
		"Carbonara",
		"Palabok"
	};

	Json j1(arr);

	std::string arrDump = j1.dump();

	assert(arrDump.find("\"Spaghetti\"") != std::string::npos);
	assert(arrDump.find("\"Carbonara\"") != std::string::npos);
	assert(arrDump.find("\"Palabok\"") != std::string::npos);
	assert(arrDump.find("\"Pancit\"") == std::string::npos);

	Json::ObjectType obj{
		{"x", 10.50},
		{"y", 20.64},
		{"z", 15.91}
	};

	Json j2(obj);

	std::string objDump = j2.dump();

	assert(objDump.find("\"x\"") != std::string::npos);
	assert(objDump.find("\"y\"") != std::string::npos);
	assert(objDump.find("\"z\"") != std::string::npos);
	assert(objDump.find("\"a\"") == std::string::npos);

	print("Dump");

	std::cout << "\n";
}

// String Escapes
// Validates parsing of supported JSON escape sequences.
void stringEscapes() {
	std::cout << "String Escapes Test\n";

	Json s1 = Json::parse("\"Hello\"");

	assert(s1.isString());
	assert(s1.asString() == "Hello");

	print("String — Basic");

	Json s2 = Json::parse("\"a\\nb\"");

	assert(s2.isString());
	assert(s2.asString() == "a\nb");

	print("String — \\n");

	Json s3 = Json::parse("\"a\\tb\"");

	assert(s3.isString());
	assert(s3.asString() == "a\tb");

	print("String — \\t");

	Json s4 = Json::parse("\"a\\rb\"");

	assert(s4.isString());
	assert(s4.asString() == "a\rb");

	print("String — \\r");

	Json s5 = Json::parse("\"a\\bb\"");

	assert(s5.isString());
	assert(s5.asString() == "a\bb");

	print("String — \\b");

	Json s6 = Json::parse("\"a\\fb\"");

	assert(s6.isString());
	assert(s6.asString() == "a\fb");

	print("String — \\f");

	Json s7 = Json::parse("\"a\\\\b\"");

	assert(s7.isString());
	assert(s7.asString() == "a\\b");

	print("String — \\\\");

	std::cout << "\n";
}

// Unicode
// Tests Unicode escape decoding, including surrogate pairs.
void stringUnicode() {
	std::cout << "String Unicode Test\n";

	Json u1 = Json::parse("\"\\u0041\"");
	assert(u1.asString() == "A");

	print("String - Unicode ASCII");

	Json u2 = Json::parse("\"\\u03A9\"");
	assert(u2.asString() == "Ω");

	print("String - Unicode Escapes");

	Json u3 = Json::parse("\"\\uD83D\\uDE00\"");
	assert(u3.asString() == "😀");

	Json u4 = Json::parse("\"\\uD83D\\uDC4D\"");
	assert(u4.asString() == "👍");

	print("String - Surrogate Pair");

	std::cout << "\n";
}

// Edge Numbers
// Verifies parsing of numeric edge cases and scientific notation.
void edgeNumbers() {
	std::cout << "Edge Numbers Test\n";

	Json e1 = Json::parse("0");
	assert(e1.asNumber() == 0);

	print("Number - Zero");

	Json e2 = Json::parse("0.0");
	assert(e2.asNumber() == 0.0);

	print("Number - Zero Double");

	Json e3 = Json::parse("-0");
	assert(e3.asNumber() == -0);

	print("Number - Negative Zero");

	Json e4 = Json::parse("1e3");
	assert(e4.asNumber() == 1000);

	print("Number - Scientific Notation");

	Json e5 = Json::parse("-1.5e2");
	assert(e5.asNumber() == -150);

	print("Number - Negative Scientific Notation");

	std::cout << "\n";
}

// Array Types
// Tests arrays, nested arrays, and indexed element access.
void arrayTypes() {
	std::cout << "Array Type Test\n";

	Json::ArrayType arr1{
		12,
		13,
		14,
		15
	};

	Json a1(arr1);

	assert(a1.isArray());
	assert(a1[1] == 13);
	assert(a1.at(2) == 14);
	assert(a1.size() == 4);

	print("Normal Array");

	Json::ArrayType arr2{
		Json::ArrayType{
			"Earth",
			"Mars"
		},
		Json::ArrayType{
			"Neptune",
			"Jupiter"
		}
	};

	Json a2(arr2);

	assert(a2.isArray());
	assert(a2[0][0] == "Earth");
	assert(a2[1][0] == "Neptune");
	assert(a2[0].at(1) == "Mars");
	assert(a2.size() == 2);

	print("Nested Array");

	std::cout << "\n";
}

// Object Types
// Tests objects, nested objects, and key-based access.
void objectTypes() {
	std::cout << "Object Type Test\n";

	Json::ObjectType obj1{
		{"atk", 99},
		{"def", 60},
		{"lvl", 21},
		{"isHurt", true}
	};

	Json o1(obj1);

	assert(o1.isObject());
	assert(o1["atk"] == 99);
	assert(o1.at("lvl") == 21);
	assert(o1.size() == 4);

	print("Normal Object");

	Json::ArrayType people{
		Json::ObjectType{
			{"name", "Maek"},
			{"money", 0.0}
		},
		Json::ObjectType{
			{"name", "Kaen"},
			{"money", 1300.0}
		}
	};

	Json::ObjectType obj2{
		{"Person", people}
	};

	Json o2(obj2);

	assert(o2.isObject());
	assert(o2["Person"][0]["name"] == "Maek");
	assert(o2["Person"][0]["money"] == 0.0);
	assert(o2["Person"][1].at("name") == "Kaen");
	assert(o2["Person"][1].at("money") == 1300.0);
	assert(o2.size() == 1);

	print("Nested Object");

	std::cout << "\n";
}

// Mixed Types
// Verifies parsing and access of complex nested JSON structures.
void mixedTypes() {
	std::cout << "Mixed Type Test\n";

	Json::ObjectType obj{
		{
			"legendary",
			Json::ArrayType{
				Json::ObjectType{
					{ "name", "Rayquaza" },
					{ "health", 5000},
					{ "stats", Json::ArrayType{100, 90, 100, 95} },
					{ "shiny", true },
					{ "height(in.)", 275.95 }
				},
				Json::ObjectType{
					{ "name", "Groudon" },
					{ "health", 6000},
					{ "stats", Json::ArrayType{90, 100, 100, 90} },
					{ "shiny", false },
					{ "height(in.)", 137.57 }
				}
			}
		}
	};
	
	Json o(obj);
	
	assert(o["legendary"].size()                       == 2);
    assert(o["legendary"][0]["name"].asString()        == "Rayquaza");
    assert(o["legendary"][1]["name"].asString()        == "Groudon");
    assert(o["legendary"][0]["shiny"].asBool()         == true);
    assert(o["legendary"][1]["height(in.)"].asNumber() == 137.57);

    print("Nested Structures");
	
	std::cout << "\n";
}

// Entry Point
int main()
{
	lifecycle();
	typeInspection();
	valueAccess();
	navigation();
	elementAccess();
	utility();
	comparison();
	serialization();

	stringEscapes();
	stringUnicode();
	edgeNumbers();
	arrayTypes();
	objectTypes();
	mixedTypes();

	return 0;
}

