#include <iostream>
#include <string>
#include <vector>

#include "Json.h"

// ============================================================
// Example Utilities
//
// Used only by examples.cpp to improve console output.
// Not part of Json, Parser, or the public library API.
// ============================================================
std::string center(
    const std::string& text,
    std::size_t width)
{
	if(text.size() >= width) {
		return text;
	}

	auto left = (width - text.size()) / 2;
	auto right = width - text.size() - left;

	return std::string(left, ' ')
	       + text
	       + std::string(right, ' ');
}

std::string repeat(
    const std::string& line,
    std::size_t count)
{
	std::string lines;

	for(std::size_t i = 0; i < count; ++i) {
		lines += line;
	}

	return lines;
}

std::string ln = repeat("─", 30);

void line() {
	std::cout << ln << "\n";
}

void title(
    const std::string& name,
    std::size_t length = 30)
{
	std::cout << center(name, length) << "\n";
}

void gap() {
	std::cout << "\n\n";
}

// ============================================================
// Parsing Examples
// Demonstrates parsing JSON text into Json objects.
// ============================================================
void parse() {
	line();
	title("Parsing Examples", 30);

	// =========================
	// String Parsing
	// =========================
	line();
	title("String Parsing\n", 30);

	std::string t1 = "\"Hello World\"";

	std::cout << "Input : " << t1 << "\n";

	Json v1 = Json::parse(t1);

	std::cout << "Output: " << v1.asString() << "\n\n";

	// =========================
	// Boolean Parsing
	// =========================
	line();
	title("Bool Parsing\n", 30);

	std::string t2 = "true";

	std::cout << "Input : " << t2 << "\n";

	Json v2 = Json::parse(t2);

	std::cout << "Output: " << (v2.asBool() ? "true" : "false") << "\n\n";

	// =========================
	// Number Parsing
	// =========================
	line();
	title("Number Parsing\n", 30);

	std::string t3 = "1000";

	std::cout << "Input : " << t3 << "\n";

	Json v3 = Json::parse(t3);

	std::cout << "Output: " << v3.asNumber() << "\n\n";

	// =========================
	// Array Parsing
	// =========================
	line();
	title("Array Parsing\n", 30);

	std::string t4 = "[1, 2, 3]";

	std::cout << "Input : " << t4 << "\n";

	Json v4 = Json::parse(t4);

	std::cout << "Output:\n";
	std::cout << v4.dump() << "\n\n";

	// =========================
	// Object Parsing
	// =========================
	line();
	title("Object Parsing\n", 30);

	std::string t5 =
	    "{"
	    "\"name\":\"Mark\","
	    "\"age\":24"
	    "}";

	std::cout << "Input:\n";
	std::cout << t5 << "\n\n";

	Json v5 = Json::parse(t5);

	std::cout << "Parsed:\n";
	std::cout << v5.dump() << "\n\n";

	line();
	gap();
}

// ============================================================
// Build Examples
// Demonstrates constructing Json values programmatically.
// ============================================================
void build() {
	line();
	title("Build Examples", 30);

	// =========================
	// Primitive Value
	// =========================
	line();
	title("Primitive Value\n", 30);

	Json jsonStr = "Hello";
	Json jsonNum = 42;
	Json jsonPi = 3.14;
	Json jsonFlag = true;
	Json jsonNull = nullptr;

	std::cout << jsonStr.dump() << "\n";
	std::cout << jsonNum.dump() << "\n";
	std::cout << jsonPi.dump() << "\n";
	std::cout << jsonFlag.dump() << "\n";
	std::cout << jsonNull.dump() << "\n\n";

	// =========================
	// Arrays
	// =========================
	line();
	title("Arrays\n", 30);

	Json jsonArray = Json::Array{
		1,
		2,
		3,
		"Hello",
		true
	};

	std::cout << jsonArray.dump() << "\n\n";

	// =========================
	// Objects
	// =========================
	line();
	title("Objects\n", 30);

	Json jsonObject = Json::Object{
		{"name", "Mark"},
		{"age", 24},
		{"student", true}
	};

	std::cout << jsonObject.dump() << "\n\n";

	// =========================
	// Nested Structures
	// =========================
	line();
	title("Nested Structures\n", 30);

	Json root = Json::Object{
		{"name", "Mark"},
		{"age", 24},
		{
			"scores",
			Json::Array{95, 88, 91}
		},
		{
			"address",
			Json::Object{
				{"city", "Manila"},
				{"country", "Philippines"}
			}
		}
	};

	std::cout << root.dump() << "\n\n";

	line();
	gap();
}

// ============================================================
// Access Examples
// Demonstrates object lookup, array indexing,
// nested access, and type conversion.
// ============================================================
void access() {
	line();
	title("Access Examples", 30);

	// =========================
	// Object Member Access
	// =========================
	line();
	title("Object Member Access\n", 30);

	Json obj = Json::parse(
	               "{"
	               "\"name\":\"Mark\","
	               "\"age\":24"
	               "}"
	           );

	std::cout << obj.dump() << "\n\n";

	std::cout << "name: " << obj["name"].asString() << "\n";
	std::cout << "age: " << obj["age"].asNumber() << "\n\n";

	// =========================
	// Array Element Access
	// =========================
	line();
	title("Array Element Access\n", 30);

	Json arr = Json::parse("[10, 20, 30, 40]");

	std::cout << arr.dump() << "\n\n";

	std::cout << "arr[0]: " << arr[0].asNumber() << "\n";
	std::cout << "arr[2]: " << arr[2].asNumber() << "\n\n";

	// =========================
	// Nested Access
	// =========================
	line();
	title("Nested Access\n", 30);

	Json data = Json::parse(
	                "{"
	                "\"user\":{"
	                "\"name\":\"Mark\","
	                "\"scores\":[90,95,88]"
	                "}"
	                "}"
	            );

	std::cout << data.dump() << "\n\n";

	std::cout << "user.name: " << data["user"]["name"].asString() << "\n";
	std::cout << "user.scores[1]: " << data["user"]["scores"][1].asNumber() << "\n\n";

	// =========================
	// Type Conversion
	// =========================
	line();
	title("Type Conversion\n", 30);

	Json str = Json::parse("\"Hello\"");
	Json num = Json::parse("42");
	Json flag = Json::parse("true");

	std::cout << str.asString() << "\n";
	std::cout << num.asNumber() << "\n";
	std::cout << flag.asBool() << "\n\n";

	line();
	gap();
}


int main() {
	parse();

	build();

	access();

	return 0;
}

