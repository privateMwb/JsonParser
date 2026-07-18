// Converting between user-defined types and JsonPro documents.
//
// Demonstrates:
// - Writing a toJson() function for a user-defined struct
// - Writing a fromJson() function to reconstruct the struct
// - Composing conversions for nested user-defined types
// - Round-tripping a struct through serialized JSON text

#include <common/framework.h>

using namespace JsonPro;

namespace {

struct Address {
    std::string city;
    std::string zip;
};

struct User {
    int id;
    std::string name;
    Address address;
};

Json toJson(const Address& address) {
    Json::ObjectType object;
    object.insert_or_assign("city", Json(address.city));
    object.insert_or_assign("zip", Json(address.zip));
    return Json(std::move(object));
}

Address addressFromJson(const Json& json) {
    return Address{json.at("city").asString(), json.at("zip").asString()};
}

Json toJson(const User& user) {
    Json::ObjectType object;
    object.insert_or_assign("id", Json(user.id));
    object.insert_or_assign("name", Json(user.name));
    object.insert_or_assign("address", toJson(user.address));
    return Json(std::move(object));
}

User userFromJson(const Json& json) {
    return User{static_cast<int>(json.at("id").asNumber()), json.at("name").asString(),
                addressFromJson(json.at("address"))};
}

} // namespace

static void run_examples() {

    // Converts a user-defined struct into a Json value.
    setTitle("Struct to Json");

    User original{1, "Rain", Address{"Metropolis", "00000"}};
    Json json = toJson(original);

    std::cout << json.dump() << "\n\n";

    // Reconstructs the struct from a parsed Json value.
    setTitle("Json to Struct");

    User restored = userFromJson(json);

    std::cout << "id  : " << restored.id << "\n";
    std::cout << "name: " << restored.name << "\n";
    std::cout << "city: " << restored.address.city << "\n";
    std::cout << "zip : " << restored.address.zip << "\n\n";

    // Round-trips the struct all the way through serialized JSON text.
    setTitle("Full Round Trip Through Text");

    std::string text = toJson(original).dump();
    User fromText = userFromJson(Json::parse(text));

    bool matches = fromText.id == original.id && fromText.name == original.name &&
                   fromText.address.city == original.address.city &&
                   fromText.address.zip == original.address.zip;

    std::cout << "matches original: " << matches << "\n";
}

REGISTER_EXAMPLE_SUITE();
