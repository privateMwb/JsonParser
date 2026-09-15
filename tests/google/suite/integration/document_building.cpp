// Json document building integration test suite.
//
// Coverage:
// - Building a flat object incrementally via operator[]
// - Building a nested object, explicitly typing each intermediate level
// - Building an array incrementally via asArray().push_back()
// - Building an array of objects incrementally
// - Building a document via JsonObject directly, then wrapping it in Json
// - A hand-built document survives dump() -> parse() and matches the original

#include <gtest/gtest.h>

#include <JsonPro/Json.h>

using namespace JsonPro;

// Verifies a flat object can be built key-by-key via operator[].
TEST(DocumentBuilding, BuildFlatObjectViaIndexOperator) {
    Json j = Json(Json::ObjectType{});
    j["name"] = Json("Rain");
    j["age"] = Json(5);
    j["active"] = Json(true);

    EXPECT_EQ(j.isObject(), true);
    EXPECT_EQ(j["name"].asString(), "Rain");
    EXPECT_EQ(j["age"].asNumber(), 5.0);
    EXPECT_EQ(j["active"].asBool(), true);
}

// Verifies nested structure requires each intermediate level to be
// explicitly typed as an object before it can be indexed into further.
TEST(DocumentBuilding, BuildNestedDocumentWithExplicitIntermediateObjects) {
    Json j = Json(Json::ObjectType{});
    j["address"] = Json(Json::ObjectType{});
    j["address"]["city"] = Json("Metro City");
    j["address"]["zip"] = Json("00000");

    EXPECT_EQ(j["address"].isObject(), true);
    EXPECT_EQ(j["address"]["city"].asString(), "Metro City");
    EXPECT_EQ(j["address"]["zip"].asString(), "00000");
}

// Verifies an array can be built incrementally via asArray().push_back().
TEST(DocumentBuilding, BuildArrayViaAsArrayPushback) {
    Json j = Json(Json::ArrayType{});
    j.asArray().push_back(Json(1));
    j.asArray().push_back(Json(2));
    j.asArray().push_back(Json(3));

    EXPECT_EQ(j.isArray(), true);
    EXPECT_EQ(j.size(), 3);
    EXPECT_EQ(j[2].asNumber(), 3.0);
}

// Verifies an array of objects can be assembled one entry at a time.
TEST(DocumentBuilding, BuildArrayOfObjectsIncrementally) {
    Json j = Json(Json::ArrayType{});

    for (int i = 0; i < 3; ++i) {
        Json entry = Json(Json::ObjectType{});
        entry["index"] = Json(i);
        j.asArray().push_back(std::move(entry));
    }

    EXPECT_EQ(j.size(), 3);
    EXPECT_EQ(j[0]["index"].asNumber(), 0.0);
    EXPECT_EQ(j[2]["index"].asNumber(), 2.0);
}

// Verifies a document can be built via JsonObject directly, then wrapped
// into a Json value.
TEST(DocumentBuilding, BuildDocumentViaJsonObjectThenWrap) {
    JsonObject obj;
    obj.insert_or_assign("title", Json("Report"));
    obj.insert_or_assign("pages", Json(12));

    Json j(std::move(obj));

    EXPECT_EQ(j.isObject(), true);
    EXPECT_EQ(j["title"].asString(), "Report");
    EXPECT_EQ(j["pages"].asNumber(), 12.0);
}

// Verifies a hand-built document round-trips through dump() and parse()
// and compares equal to the original.
TEST(DocumentBuilding, BuiltDocumentRoundTripsThroughDumpAndParse) {
    Json j = Json(Json::ObjectType{});
    j["name"] = Json("Widget");
    j["count"] = Json(3);
    j["tags"] = Json(Json::ArrayType{Json("a"), Json("b")});

    std::string dumped = j.dump();
    Json reparsed = Json::parse(dumped);

    EXPECT_EQ(reparsed, j);
    EXPECT_EQ(reparsed["tags"][1].asString(), "b");
}
