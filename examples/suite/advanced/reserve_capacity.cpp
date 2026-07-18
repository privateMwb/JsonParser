// Reserving capacity for large JsonPro objects.
//
// Demonstrates:
// - Why reserve() exists: avoiding incremental reallocation while building
// - Reserving both the entry vector and the index map in one call
// - Building a large object efficiently before serializing it

#include <common/framework.h>

using namespace JsonPro;

static void run_examples() {

    // Without reserve(), inserting many entries can trigger repeated
    // reallocation and rehashing as the object grows past its current
    // capacity, over and over.
    setTitle("Building Without Reserving");

    Json::ObjectType growing;
    for (int i = 0; i < 1000; ++i)
        growing.insert_or_assign("key" + std::to_string(i), Json(i));

    std::cout << "entries: " << growing.size() << "\n\n";

    // reserve(n) allocates capacity for both the entry vector and the
    // index map up front, so the loop below never reallocates.
    setTitle("Building With Reserve");

    Json::ObjectType reserved;
    reserved.reserve(1000);

    for (int i = 0; i < 1000; ++i)
        reserved.insert_or_assign("key" + std::to_string(i), Json(i));

    std::cout << "entries: " << reserved.size() << "\n\n";

    // Both objects hold identical content; reserve() is purely a
    // performance decision, not a correctness one.
    setTitle("Same Content Either Way");

    Json growingJson(std::move(growing));
    Json reservedJson(std::move(reserved));

    std::cout << "equal: " << (growingJson == reservedJson) << "\n";
}

REGISTER_EXAMPLE_SUITE();
