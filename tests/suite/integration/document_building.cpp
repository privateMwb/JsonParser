// Json document building integration test suite.
//
// Coverage:
// - Building a flat object incrementally via operator[]
// - Building a nested object, explicitly typing each intermediate level
// - Building an array incrementally via asArray().push_back()
// - Building an array of objects incrementally
// - Building a document via JsonObject directly, then wrapping it in Json
// - A hand-built document survives dump() -> parse() and matches the original

#include <common/framework.h>

using namespace JsonPro;

// Verifies a flat object can be built key-by-key via operator[].
static void build_flat_object_via_index_operator() {
    Json j = Json(Json::ObjectType{});
    j["name"] = Json("Rain");
    j["age"] = Json(5);
    j["active"] = Json(true);

    CHK(j.isObject() == true);
    CHK(j["name"].asString() == "Rain");
    CHK(j["age"].asNumber() == 5.0);
    CHK(j["active"].asBool() == true);
}

// Verifies nested structure requires each intermediate level to be
// explicitly typed as an object before it can be indexed into further.
static void build_nested_document_with_explicit_intermediate_objects() {
    Json j = Json(Json::ObjectType{});
    j["address"] = Json(Json::ObjectType{});
    j["address"]["city"] = Json("Metro City");
    j["address"]["zip"] = Json("00000");

    CHK(j["address"].isObject() == true);
    CHK(j["address"]["city"].asString() == "Metro City");
    CHK(j["address"]["zip"].asString() == "00000");
}

// Verifies an array can be built incrementally via asArray().push_back().
static void build_array_via_asArray_pushback() {
    Json j = Json(Json::ArrayType{});
    j.asArray().push_back(Json(1));
    j.asArray().push_back(Json(2));
    j.asArray().push_back(Json(3));

    CHK(j.isArray() == true);
    CHK(j.size() == 3);
    CHK(j[2].asNumber() == 3.0);
}

// Verifies an array of objects can be assembled one entry at a time.
static void build_array_of_objects_incrementally() {
    Json j = Json(Json::ArrayType{});

    for (int i = 0; i < 3; ++i) {
        Json entry = Json(Json::ObjectType{});
        entry["index"] = Json(i);
        j.asArray().push_back(std::move(entry));
    }

    CHK(j.size() == 3);
    CHK(j[0]["index"].asNumber() == 0.0);
    CHK(j[2]["index"].asNumber() == 2.0);
}

// Verifies a document can be built via JsonObject directly, then wrapped
// into a Json value.
static void build_document_via_jsonobject_then_wrap() {
    JsonObject obj;
    obj.insert_or_assign("title", Json("Report"));
    obj.insert_or_assign("pages", Json(12));

    Json j(std::move(obj));

    CHK(j.isObject() == true);
    CHK(j["title"].asString() == "Report");
    CHK(j["pages"].asNumber() == 12.0);
}

// Verifies a hand-built document round-trips through dump() and parse()
// and compares equal to the original.
static void built_document_round_trips_through_dump_and_parse() {
    Json j = Json(Json::ObjectType{});
    j["name"] = Json("Widget");
    j["count"] = Json(3);
    j["tags"] = Json(Json::ArrayType{Json("a"), Json("b")});

    std::string dumped = j.dump();
    Json reparsed = Json::parse(dumped);

    CHK(reparsed == j);
    CHK(reparsed["tags"][1].asString() == "b");
}

// Executes all document building test cases.
static void run_tests() {
    RUN(build_flat_object_via_index_operator);
    RUN(build_nested_document_with_explicit_intermediate_objects);
    RUN(build_array_via_asArray_pushback);
    RUN(build_array_of_objects_incrementally);
    RUN(build_document_via_jsonobject_then_wrap);
    RUN(built_document_round_trips_through_dump_and_parse);
}

REGISTER_TEST_SUITE();
