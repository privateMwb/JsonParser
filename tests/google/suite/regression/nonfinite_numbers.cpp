// Non-finite number regression test suite.
//
// RFC 8259 has no representation for NaN or Infinity, so they can never
// arise from Json::parse() -- but a Json Number can still be *constructed*
// directly from a non-finite double (e.g. from application code doing
// arithmetic). Guards against dump() silently emitting invalid JSON (such
// as a bare `nan` or `inf` token) for such a value, and confirms the
// failure is isolated to serialization, not construction or access.
//
// Coverage:
// - dump() (string form) throws for NaN, +Infinity, -Infinity
// - dump(indent) throws identically
// - dump(ostream&) throws identically
// - A non-finite value nested inside an array or object still throws when
//   the enclosing structure is serialized
// - Construction and numeric access (isNumber()/asNumber()) are unaffected;
//   only serialization is rejected

#include <JsonPro/Json.h>

#include <gtest/gtest.h>

#include <cmath>
#include <limits>
#include <sstream>

using namespace JsonPro;

// Verifies dump() throws for a NaN value.
TEST(NonfiniteNumbers, dump_throws_for_nan) {
    Json j(std::numeric_limits<double>::quiet_NaN());
    EXPECT_THROW(j.dump(), std::runtime_error);
}

// Verifies dump() throws for positive and negative Infinity.
TEST(NonfiniteNumbers, dump_throws_for_infinity) {
    Json positive(std::numeric_limits<double>::infinity());
    Json negative(-std::numeric_limits<double>::infinity());

    EXPECT_THROW(positive.dump(), std::runtime_error);
    EXPECT_THROW(negative.dump(), std::runtime_error);
}

// Verifies dump(indent) throws identically to the no-argument overload.
TEST(NonfiniteNumbers, dump_with_indent_throws_for_non_finite) {
    Json j(std::numeric_limits<double>::quiet_NaN());
    EXPECT_THROW(j.dump(4), std::runtime_error);
}

// Verifies dump(ostream&) throws identically to the string-returning overload.
TEST(NonfiniteNumbers, dump_to_stream_throws_for_non_finite) {
    Json j(std::numeric_limits<double>::infinity());
    std::ostringstream out;

    EXPECT_THROW(j.dump(out), std::runtime_error);
}

// Verifies a non-finite value nested inside an array still throws when the
// enclosing array is serialized.
TEST(NonfiniteNumbers, nested_non_finite_value_in_array_throws) {
    Json j(Json::ArrayType{Json(1), Json(std::numeric_limits<double>::quiet_NaN()), Json(3)});
    EXPECT_THROW(j.dump(), std::runtime_error);
}

// Verifies a non-finite value nested inside an object still throws when the
// enclosing object is serialized.
TEST(NonfiniteNumbers, nested_non_finite_value_in_object_throws) {
    Json::ObjectType obj;
    obj.emplace("value", Json(-std::numeric_limits<double>::infinity()));

    Json j(std::move(obj));
    EXPECT_THROW(j.dump(), std::runtime_error);
}

// Verifies construction and numeric access work normally for non-finite
// values -- only serialization is rejected.
TEST(NonfiniteNumbers, construction_and_access_unaffected_by_non_finite_value) {
    Json j(std::numeric_limits<double>::quiet_NaN());

    EXPECT_TRUE(j.isNumber());
    EXPECT_TRUE(std::isnan(j.asNumber()));
}
