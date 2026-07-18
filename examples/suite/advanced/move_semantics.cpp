// Move semantics in JsonPro.
//
// Demonstrates:
// - parse() copying its input vs parseOwned() taking ownership
// - Json's move constructor/assignment leaving the source null
// - Moving values into containers instead of copying them

#include <common/framework.h>

using namespace JsonPro;

static void run_examples() {

    // parse() copies the input text internally, since the parser needs a
    // stable, owned buffer for the duration of the parse. The original
    // string is left untouched and still usable afterward.
    setTitle("parse() Copies Its Input");

    std::string source = R"({"value": 1})";
    Json fromCopy = Json::parse(source);

    std::cout << "source is still usable: " << source << "\n";
    std::cout << "parsed                : " << fromCopy.dump() << "\n\n";

    // parseOwned() takes ownership of the buffer instead of copying it,
    // avoiding that copy when the caller doesn't need the string afterward.
    setTitle("parseOwned() Takes Ownership");

    std::string owned = R"({"value": 2})";
    Json fromOwned = Json::parseOwned(std::move(owned));

    std::cout << "parsed: " << fromOwned.dump() << "\n\n";

    // Moving a Json value transfers its storage and leaves the source null.
    setTitle("Move Leaves the Source Null");

    Json original(Json::ArrayType{Json(1), Json(2), Json(3)});
    Json moved = std::move(original);

    std::cout << "moved   : " << moved.dump() << "\n";

    // NOLINTNEXTLINE(clang-analyzer-cplusplus.Move)
    std::cout << "original: " << original.dump() << " (now null)\n";
    std::cout << "original.isNull(): " << original.isNull() << "\n\n";

    // Moving values into a container avoids copying their storage.
    setTitle("Moving Into a Container");

    Json largeString(std::string(64, 'x'));
    Json::ArrayType array;
    array.push_back(std::move(largeString));

    std::cout << "largeString.isNull() after move: " << largeString.isNull() << "\n";
    std::cout << "array[0] length: " << array[0].asString().size() << "\n";
}

REGISTER_EXAMPLE_SUITE();
