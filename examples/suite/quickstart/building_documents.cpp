// Building JsonPro documents programmatically.
//
// Demonstrates:
// - Building an array via push_back
// - Building an object via insert_or_assign vs operator[]
// - Nesting arrays inside objects and objects inside arrays
// - Assembling a full multi-level document from scratch

#include <common/framework.h>

using namespace JsonPro;

static void run_examples() {

    // Builds an array by appending elements one at a time.
    setTitle("Building an Array");

    Json::ArrayType numbers;
    numbers.push_back(Json(1));
    numbers.push_back(Json(2));
    numbers.push_back(Json(3));

    Json numbersJson(std::move(numbers));

    std::cout << numbersJson.dump() << "\n\n";

    // Builds an object two ways: insert_or_assign (single lookup+insert)
    // vs operator[] (default-construct then assign on first insertion).
    setTitle("Building an Object");

    Json::ObjectType viaInsert;
    viaInsert.insert_or_assign("name", Json("Rain"));
    viaInsert.insert_or_assign("age", Json(30));

    Json::ObjectType viaBracket;
    viaBracket["name"] = Json("Rain");
    viaBracket["age"] = Json(30);

    std::cout << "via insert_or_assign: " << Json(std::move(viaInsert)).dump() << "\n";
    std::cout << "via operator[]      : " << Json(std::move(viaBracket)).dump() << "\n\n";

    // Nests an array inside an object.
    setTitle("Nesting an Array in an Object");

    Json::ObjectType profile;
    profile.insert_or_assign("name", Json("Rain"));
    profile.insert_or_assign("skills", Json(Json::ArrayType{Json("C++"), Json("Systems")}));

    Json profileJson(std::move(profile));
    std::cout << profileJson.dump() << "\n\n";

    // Nests objects inside an array.
    setTitle("Nesting Objects in an Array");

    Json::ObjectType user1;
    user1.insert_or_assign("id", Json(1));
    user1.insert_or_assign("name", Json("Alpha"));

    Json::ObjectType user2;
    user2.insert_or_assign("id", Json(2));
    user2.insert_or_assign("name", Json("Beta"));

    Json::ArrayType users;
    users.push_back(Json(std::move(user1)));
    users.push_back(Json(std::move(user2)));

    Json usersJson(std::move(users));
    std::cout << usersJson.dump() << "\n\n";

    // Assembles a full multi-level document from scratch.
    setTitle("Assembling a Full Document");

    Json::ObjectType address;
    address.insert_or_assign("city", Json("Metropolis"));
    address.insert_or_assign("zip", Json("00000"));

    Json::ObjectType document;
    document.insert_or_assign("name", Json("Rain"));
    document.insert_or_assign("active", Json(true));
    document.insert_or_assign("address", Json(std::move(address)));
    document.insert_or_assign("tags", Json(Json::ArrayType{Json("cpp"), Json("json")}));

    Json documentJson(std::move(document));
    std::cout << documentJson.dump() << "\n";
}

REGISTER_EXAMPLE_SUITE();
