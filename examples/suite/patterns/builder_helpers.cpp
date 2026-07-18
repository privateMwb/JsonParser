// Composable builder helpers for JsonPro documents.
//
// Demonstrates:
// - Writing small helper functions that each build one reusable fragment
// - Composing those fragments into a larger document
// - Keeping construction code readable as documents grow in complexity

#include <common/framework.h>

using namespace JsonPro;

namespace {

// Builds a single address fragment.
Json makeAddress(std::string city, std::string zip) {
    Json::ObjectType address;
    address.insert_or_assign("city", Json(std::move(city)));
    address.insert_or_assign("zip", Json(std::move(zip)));
    return Json(std::move(address));
}

// Builds a single user fragment, composing makeAddress().
Json makeUser(int id, std::string name, Json address) {
    Json::ObjectType user;
    user.insert_or_assign("id", Json(id));
    user.insert_or_assign("name", Json(std::move(name)));
    user.insert_or_assign("address", std::move(address));
    return Json(std::move(user));
}

} // namespace

static void run_examples() {

    // Builds a single user from small, focused helper functions.
    setTitle("Building One Fragment at a Time");

    Json user = makeUser(1, "Alpha", makeAddress("Metropolis", "00000"));
    std::cout << user.dump() << "\n\n";

    // Composes several fragments into a list, without repeating the
    // object-assembly code inline for every entry.
    setTitle("Composing a List of Fragments");

    Json::ArrayType users;
    users.push_back(makeUser(1, "Alpha", makeAddress("Metropolis", "00000")));
    users.push_back(makeUser(2, "Beta", makeAddress("Gotham", "11111")));
    users.push_back(makeUser(3, "Gamma", makeAddress("Star City", "22222")));

    Json usersJson(std::move(users));
    std::cout << usersJson.dump() << "\n\n";

    // Wraps the composed list in a top-level document.
    setTitle("Assembling the Final Document");

    Json::ObjectType document;
    document.insert_or_assign("users", std::move(usersJson));
    document.insert_or_assign("count", Json(static_cast<int>(document["users"].size())));

    std::cout << Json(std::move(document)).dump() << "\n";
}

REGISTER_EXAMPLE_SUITE();
