// Navigating parsed JsonPro documents.
//
// Demonstrates:
// - Parsing a realistic nested document
// - Accessing values with operator[]
// - Safe access with at() and exception handling
// - Traversing nested arrays and objects
// - Iterating over array and object contents
// - Checking type/presence before accessing to avoid exceptions

#include <common/framework.h>

using namespace JsonPro;

static void run_examples() {

    Json doc = Json::parse(R"({
        "name": "Rain",
        "active": true,
        "projects": ["CachePro", "VectorPro", "JsonPro"],
        "address": {
            "city": "Metropolis",
            "zip": "00000"
        }
    })");

    // Accesses top-level values directly by key.
    setTitle("Accessing Top-Level Values");

    std::cout << "name   : " << doc["name"].asString() << "\n";
    std::cout << "active : " << doc["active"].asBool()  << "\n\n";

    // Uses at() for bounds/key-checked access, catching the exception
    // it throws for a missing key.
    setTitle("Safe Access with at()");

    try {
        std::cout << doc.at("missing_key").asString() << "\n";
    } catch (const std::out_of_range& e) {
        std::cout << "Caught expected error: " << e.what() << "\n\n";
    }

    // Traverses into nested objects and arrays via chained access.
    setTitle("Traversing Nested Structures");

    std::cout << "city         : " << doc["address"]["city"].asString() << "\n";
    std::cout << "first project: " << doc["projects"][0].asString()     << "\n\n";

    // Iterates over an array's underlying container.
    setTitle("Iterating an Array");

    for (const Json& project : doc["projects"].asArray())
        std::cout << "- " << project.asString() << "\n";
    std::cout << "\n";

    // Iterates over an object's underlying container, in the order its
    // keys were originally inserted.
    setTitle("Iterating an Object");

    const Json::ObjectType& address = doc["address"].asObject();

    for (const std::string& key : address.insertionOrder())
        std::cout << key << ": " << address.find(key)->asString() << "\n";
    std::cout << "\n";

    // Checks presence and type before accessing, avoiding exceptions entirely.
    setTitle("Checking Before Accessing");

    if (doc.contains("address") && doc["address"].isObject())
        std::cout << "address is present and is an object\n";

    if (!doc.contains("phone"))
        std::cout << "phone is not present, skipping access\n";
}

REGISTER_EXAMPLE_SUITE();
