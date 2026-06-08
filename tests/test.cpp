#include <iostream>
#include <cassert>

#include "Json.h"

// ============================================================
// Basic Type Tests
// Verifies parsing of null, booleans, and numbers.
// ============================================================
void basic_types() {
	Json n = Json::parse("null");
	assert(n.isNull());

	Json b1 = Json::parse("true");
	Json b2 = Json::parse("false");
	assert(b1.asBool());
	assert(!b2.asBool());

	Json num1 = Json::parse("22.34");
	Json num2 = Json::parse("-7.9999");
	assert(num1.asNumber() == 22.34);
	assert(num2.asNumber() == -7.9999);

	std::cout << "\n[PASS] Basic Types Test\n";
}

// ============================================================
// String Escape Tests
// Verifies handling of JSON escape sequences.
// ============================================================
void string_escapes() {
	Json s1 = Json::parse("\"Hello\"");
	assert(s1.isString());
	assert(s1.asString() == "Hello");

	Json s2 = Json::parse("\"a\\nb\"");
	assert(s2.isString());
	assert(s2.asString() == "a\nb");

	Json s3 = Json::parse("\"a\\tb\"");
	assert(s3.isString());
	assert(s3.asString() == "a\tb");
	
	Json s4 = Json::parse("\"a\\rb\"");
	assert(s4.isString());
	assert(s4.asString() == "a\rb");
	
	Json s5 = Json::parse("\"a\\bb\"");
	assert(s5.isString());
	assert(s5.asString() == "a\bb");
	
	Json s6 = Json::parse("\"a\\fb\"");
	assert(s6.isString());
	assert(s6.asString() == "a\fb");
	
	Json s7 = Json::parse("\"a\\\\b\"");
	assert(s7.isString());
	assert(s7.asString() == "a\\b");
	
	std::cout << "\n[PASS] String Escapes Test\n";
}

// ============================================================
// Unicode Tests
// Verifies Unicode escape sequences and surrogate pairs.
// ============================================================
void string_unicode() {
    Json u1 = Json::parse("\"\\u0041\"");
    assert(u1.asString() == "A");

    Json u2 = Json::parse("\"\\u03A9\"");
    assert(u2.asString() == "Ω");
    
    Json u3 = Json::parse("\"\\uD83D\\uDE00\"");
    assert(u3.asString() == "😀");

    Json u4 = Json::parse("\"\\uD83D\\uDC4D\"");
    assert(u4.asString() == "👍");
    
    std::cout << "\n[PASS] String Unicode Test\n";
}

// ============================================================
// Array Tests
// Verifies array parsing, storage, and element access.
// ============================================================
void array_types() {
	Json a1 = Json::parse("[21, 65, 98]");
	Json::Array arr1 = {
		Json(21), Json(65), Json(98)
	};

	assert(a1.isArray());
	assert(a1.asArray() == arr1);

	assert(a1[0].asNumber() == 21);
	assert(a1[1].asNumber() == 65);
	assert(a1[2].asNumber() == 98);

	Json a2 = Json::parse ("[[\"apple\", \"banana\"],[\"fire\", \"water\"]]");
	Json::Array arr2 = {
		Json::Array{ Json("apple"), Json("banana") },
		Json::Array{ Json("fire"), Json("water") }
	};

	assert(a2.isArray());
	assert(a2.asArray() == arr2);

	assert(a2[0][0].asString() == "apple");
	assert(a2[0][1].asString() == "banana");
	assert(a2[1][0].asString() == "fire");
	assert(a2[1][1].asString() == "water");

	std::cout << "\n[PASS] Array Types Test\n";
}

// ============================================================
// Object Tests
// Verifies object parsing, storage, and member access.
// ============================================================
void object_types() {
	Json o1 = Json::parse(
	              "{"
	              "\"a\":1,"
	              "\"b\":2,"
	              "\"c\":3"
	              "}"
	          );

	Json::Object obj1 = {
		{ "a", 1 },
		{ "b", 2 },
		{ "c", 3 }
	};

	assert(o1.isObject());
	assert(o1.asObject() == obj1);

	assert(o1["a"].asNumber() == 1);
	assert(o1["b"].asNumber() == 2);
	assert(o1["c"].asNumber() == 3);

	Json o2 = Json::parse(
	              "{"
	              "\"people\":["
	              "{"
	              "\"name\":\"Mark\","
	              "\"age\":24,"
	              "\"sex\":\"M\""
	              "},"
	              "{"
	              "\"name\":\"Elora\","
	              "\"age\":23,"
	              "\"sex\":\"F\""
	              "}"
	              "]"
	              "}"
	          );

	Json::Object obj2 = {
		{
			"people",
			Json(Json::Array{
				Json(Json::Object{
					{ "name", "Mark" },
					{ "age", 24 },
					{ "sex", "M" }
				}),
				Json(Json::Object{
					{"name", "Elora" },
					{ "age", 23 },
					{ "sex", "F" }
				})
			})
		}
	};

	assert(o2.isObject());
	assert(o2.asObject() == obj2);

	assert(o2["people"][0]["name"].asString() == "Mark");
	assert(o2["people"][0]["age"].asNumber() == 24);
	assert(o2["people"][0]["sex"].asString() == "M");

	assert(o2["people"][1]["name"].asString() == "Elora");
	assert(o2["people"][1]["age"].asNumber() == 23);
	assert(o2["people"][1]["sex"].asString() == "F");

	std::cout << "\n[PASS] Object Types Test\n";
}

// ============================================================
// Mixed Structure Tests
// Verifies deeply nested combinations of objects,
// arrays, strings, numbers, and booleans.
// ============================================================
void mixed_types() {
    Json m = Json::parse(
        "{"
        "\"pokemon\":["
        "{"
        "\"name\":\"Rayquaza\","
        "\"health\": 5000,"
        "\"stats\": [100, 90, 100, 95],"
        "\"shiny\": true,"
        "\"height(in.)\": 275.95"
        "},"
        "{"
        "\"name\":\"Groudon\","
        "\"health\": 6000,"
        "\"stats\": [90, 100, 100, 90],"
        "\"shiny\": false,"
        "\"height(in.)\": 137.57"
        "}"
        "]"
        "}"
    );
    
    Json::Object obj = {
        {
            "pokemon",
            Json(Json::Array{
                Json(Json::Object{
                    { "name", "Rayquaza" },
                    { "health", 5000},
                    { "stats", Json::Array{100, 90, 100, 95} },
                    { "shiny", true },
                    { "height(in.)", 275.95 }
                }),
                Json(Json::Object{
                    { "name", "Groudon" },
                    { "health", 6000},
                    { "stats", Json::Array{90, 100, 100, 90} },
                    { "shiny", false },
                    { "height(in.)", 137.57 }
                })
            })
        }
    };
    
    Json::Array arr1 = Json::Array{100, 90, 100, 95};
    Json::Array arr2 = Json::Array{90, 100, 100, 90};
    
    assert(m.isObject());
    assert(m.asObject() == obj);
    
    assert(m["pokemon"][0]["name"].asString() == "Rayquaza");
    assert(m["pokemon"][0]["health"].asNumber() == 5000);
    assert(m["pokemon"][0]["stats"].asArray() == arr1);
    assert(m["pokemon"][0]["shiny"].asBool() == true);
    assert(m["pokemon"][0]["height(in.)"].asNumber() == 275.95);
    
    assert(m["pokemon"][0]["stats"][0].asNumber() == 100);
    assert(m["pokemon"][0]["stats"][1].asNumber() == 90);
    assert(m["pokemon"][0]["stats"][2].asNumber() == 100);
    assert(m["pokemon"][0]["stats"][3].asNumber() == 95);
    
    assert(m["pokemon"][1]["name"].asString() == "Groudon");
    assert(m["pokemon"][1]["health"].asNumber() == 6000);
    assert(m["pokemon"][1]["stats"].asArray() == arr2);
    assert(m["pokemon"][1]["shiny"].asBool() == false);
    assert(m["pokemon"][1]["height(in.)"].asNumber() == 137.57);
    
    assert(m["pokemon"][1]["stats"][0].asNumber() == 90);
    assert(m["pokemon"][1]["stats"][1].asNumber() == 100);
    assert(m["pokemon"][1]["stats"][2].asNumber() == 100);
    assert(m["pokemon"][1]["stats"][3].asNumber() == 90);
    
    std::cout << "\n[PASS] Mixed Types Test\n";
}

// ============================================================
// Number Edge Case Tests
// Verifies parsing of special numeric formats such as
// zero, negative zero, decimals, and exponents.
// ============================================================
void edge_numbers() {
    Json e1 = Json::parse("0");
    Json e2 = Json::parse("0.0");
    Json e3 = Json::parse("1e3");
    Json e4 = Json::parse("-1.5e2");
    Json e5 = Json::parse("-0");
    
    assert(e1.asNumber() == 0);
    assert(e2.asNumber() == 0.0);
    assert(e3.asNumber() == 1000);
    assert(e4.asNumber() == -150);
    assert(e5.asNumber() == -0);
    
    std::cout << "\n[PASS] Edge Numbers Test\n";
}

// ============================================================
// Serialization Tests
// Verifies JSON serialization using dump().
// ============================================================
void dump() {
    Json d = Json::parse(
        "{"
        "\"x\":78.45,"
        "\"y\":93.5632,"
        "\"z\":100"
        "}"
    );
    
    std::string out = d.dump(0);
    
    assert(out.find("\"x\"") != std::string::npos);
    assert(out.find("\"y\"") != std::string::npos);
    assert(out.find("\"z\"") != std::string::npos);
    
    std::cout << "\n[PASS] Dump Test\n";
}

int main() {
	basic_types();

	string_escapes();
	
	string_unicode();

	array_types();

	object_types();
	
	mixed_types();
	
	edge_numbers();
	
	dump();
	
	std::cout << "\nAll Test Completed\n";
	
	return 0;
}

