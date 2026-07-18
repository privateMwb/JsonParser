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

#include <cmath>
#include <limits>
#include <sstream>

#include <common/framework.h>

using namespace JsonPro;

// Verifies dump() throws for a NaN value.
static void dump_throws_for_nan() {
    Json j(std::numeric_limits<double>::quiet_NaN());
    CHK_THROWS(j.dump(), std::runtime_error);
}

// Verifies dump() throws for positive and negative Infinity.
static void dump_throws_for_infinity() {
    Json positive(std::numeric_limits<double>::infinity());
    Json negative(-std::numeric_limits<double>::infinity());

    CHK_THROWS(positive.dump(), std::runtime_error);
    CHK_THROWS(negative.dump(), std::runtime_error);
}

// Verifies dump(indent) throws identically to the no-argument overload.
static void dump_with_indent_throws_for_non_finite() {
    Json j(std::numeric_limits<double>::quiet_NaN());
    CHK_THROWS(j.dump(4), std::runtime_error);
}

// Verifies dump(ostream&) throws identically to the string-returning overload.
static void dump_to_stream_throws_for_non_finite() {
    Json j(std::numeric_limits<double>::infinity());
    std::ostringstream out;

    CHK_THROWS(j.dump(out), std::runtime_error);
}

// Verifies a non-finite value nested inside an array still throws when the
// enclosing array is serialized.
static void nested_non_finite_value_in_array_throws() {
    Json j(Json::ArrayType{Json(1), Json(std::numeric_limits<double>::quiet_NaN()), Json(3)});
    CHK_THROWS(j.dump(), std::runtime_error);
}

// Verifies a non-finite value nested inside an object still throws when the
// enclosing object is serialized.
static void nested_non_finite_value_in_object_throws() {
    Json::ObjectType obj;
    obj.emplace("value", Json(-std::numeric_limits<double>::infinity()));

    Json j(std::move(obj));
    CHK_THROWS(j.dump(), std::runtime_error);
}

// Verifies construction and numeric access work normally for non-finite
// values -- only serialization is rejected.
static void construction_and_access_unaffected_by_non_finite_value() {
    Json j(std::numeric_limits<double>::quiet_NaN());

    CHK(j.isNumber() == true);
    CHK(std::isnan(j.asNumber()) == true);
}

// Executes all non-finite number regression test cases.
static void run_tests() {
    RUN(dump_throws_for_nan);
    RUN(dump_throws_for_infinity);
    RUN(dump_with_indent_throws_for_non_finite);
    RUN(dump_to_stream_throws_for_non_finite);
    RUN(nested_non_finite_value_in_array_throws);
    RUN(nested_non_finite_value_in_object_throws);
    RUN(construction_and_access_unaffected_by_non_finite_value);
}

REGISTER_TEST_SUITE();
