// Basic JsonPro usage.
//
// Demonstrates:
// - Constructing scalar, array, and object values directly
// - Parsing JSON text with Json::parse()
// - Type inspection with type() / isX()
// - Value access with asX()
// - Serialization with dump()

#include <common/framework.h>

using namespace JsonPro;

static void run_examples() {

    // Constructs Json values directly from C++ types.
    setTitle("Construction");

    Json null;
    Json boolean(true);
    Json number(3.14);
    Json string("hello");
    Json array(Json::ArrayType{ Json(1), Json(2), Json(3) });

    std::cout << "null    : " << null.dump()    << "\n";
    std::cout << "bool    : " << boolean.dump() << "\n";
    std::cout << "number  : " << number.dump()  << "\n";
    std::cout << "string  : " << string.dump()  << "\n";
    std::cout << "array   : " << array.dump()   << "\n\n";

    // Parses a JSON document from text.
    setTitle("Parsing");

    Json doc = Json::parse(R"({
        "name": "Rain",
        "active": true,
        "score": 42.5,
        "tags": ["cpp", "json"]
    })");

    std::cout << "Parsed document:\n" << doc.dump() << "\n\n";

    // Inspects the type of a value before accessing it.
    setTitle("Type Inspection");

    std::cout << "doc.type() is Object : " << (doc.type() == Json::Type::Object) << "\n";
    std::cout << "doc[\"name\"].isString() : " << doc["name"].isString() << "\n";
    std::cout << "doc[\"tags\"].isArray()  : " << doc["tags"].isArray()  << "\n\n";

    // Accesses the underlying value once the type is known.
    setTitle("Value Access");

    std::cout << "name   : " << doc["name"].asString()  << "\n";
    std::cout << "active : " << doc["active"].asBool()  << "\n";
    std::cout << "score  : " << doc["score"].asNumber()  << "\n";
    std::cout << "tags[0]: " << doc["tags"][0].asString() << "\n\n";

    // Serializes a value back to a JSON string, with indentation.
    setTitle("Serialization");

    std::cout << doc.dump() << "\n";
}

REGISTER_EXAMPLE_SUITE();
