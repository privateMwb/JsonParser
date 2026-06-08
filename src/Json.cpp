#include <sstream>
#include <stdexcept>
#include <string>


#include "Json.h"
#include "Parser.h"

namespace {
    std::string pad(int n) {
        return std::string(n, ' ');
    }
}


// =========================
// Constructors
// =========================
Json::Json() :
	type_(Type::Null),
	value_(nullptr) {}

Json::Json(std::nullptr_t) :
	type_(Type::Null),
	value_(nullptr) {}

Json::Json(bool value) :
	type_(Type::Bool),
	value_(value) {}

Json::Json(double value) :
	type_(Type::Number),
	value_(value) {}

Json::Json(int value) :
	type_(Type::Number),
	value_(static_cast<double>(value)) {}

Json::Json(const std::string& value) :
	type_(Type::String),
	value_(value) {}

Json::Json(const char* value) :
	type_(Type::String),
	value_(value) {}

Json::Json(const Array& value) :
	type_(Type::Array),
	value_(value) {}

Json::Json(const Object& value) :
	type_(Type::Object),
	value_(value) {}

Json::Json(const Json& other) :
type_(other.type_),
value_(other.value_) {}

Json& Json::operator=(const Json& other) {
    if (this != &other) {
        type_ = other.type_;
        value_ = other.value_;
    }
    
    return *this;
}

Json::Json(Json&& other) noexcept :
type_(other.type_),
value_(std::move(other.value_)) {}

Json& Json::operator=(Json&& other) noexcept {
    if (this != &other) {
        type_ = other.type_;
        value_ = std::move(other.value_);
    }
    
    return *this;
}

// =========================
// Parsing
// =========================
Json Json::parse(const std::string& text) {
    Parser parser(text);
    
    return parser.parse();
}

// =========================
// Type Inspection
// =========================
Json::Type Json::type() const noexcept {
	return type_;
}

bool Json::isNull() const noexcept {
	return type_ == Type::Null;
}

bool Json::isBool() const noexcept {
	return type_ == Type::Bool;
}

bool Json::isNumber() const noexcept {
	return type_ == Type::Number;
}

bool Json::isString() const noexcept {
	return type_ == Type::String;
}

bool Json::isArray() const noexcept {
	return type_ == Type::Array;
}

bool Json::isObject() const noexcept {
	return type_ == Type::Object;
}

// =========================
// Value Access
// =========================
bool Json::asBool() const {
	if(!isBool()) {
		throw std::runtime_error("Json: not a bool");
	}

	return std::get<bool>(value_);
}

double Json::asNumber() const {
	if(!isNumber()) {
		throw std::runtime_error("Json: not a number");
	}

	return std::get<double>(value_);
}

const std::string& Json::asString() const {
	if(!isString()) {
		throw std::runtime_error("Json: not a string");
	}

	return std::get<std::string>(value_);
}

Json::Array& Json::asArray() {
	if(!isArray()) {
		throw std::runtime_error("Json: not an array");
	}

	return std::get<Array>(value_);
}

const Json::Array& Json::asArray() const {
	if(!isArray()) {
		throw std::runtime_error("Json: not an array");
	}

	return std::get<Array>(value_);
}

Json::Object& Json::asObject() {
	if(!isObject()) {
		throw std::runtime_error("Json: not an object");
	}

	return std::get<Object>(value_);
}

const Json::Object& Json::asObject() const {
	if(!isObject()) {
		throw std::runtime_error("Json: not an object");
	}

	return std::get<Object>(value_);
}

// =========================
// Navigation
// =========================
Json& Json::operator[](const std::string& key) {
	if(!isObject()) {
		throw std::runtime_error("Json: not an object");
	}

	return std::get<Object>(value_)[key];
}

const Json& Json::operator[](const std::string& key) const {
	if(!isObject()) {
		throw std::runtime_error("Json: not an object");
	}

	const auto& obj = std::get<Object>(value_);

	return obj.at(key);
}

Json& Json::operator[](std::size_t index) {
	if(!isArray()) {
		throw std::runtime_error("Json: not an array");
	}

	return std::get<Array>(value_)[index];
}

const Json& Json::operator[](std::size_t index) const {
	if(!isArray()) {
		throw std::runtime_error("Json: not an array");
	}

	const auto& arr = std::get<Array>(value_);

	return arr.at(index);
}

// =========================
// Utilities
// =========================
std::size_t Json::size() const noexcept {
	if(isArray()) {
		return std::get<Array>(value_).size();
	}

	if(isObject()) {
		return std::get<Object>(value_).size();
	}

	return 0;
}

bool Json::contains(const std::string& key) const {
	if(!isObject()) {
		return false;
	}

	const auto& obj = std::get<Object>(value_);

	return obj.find(key) != obj.end();
}

// =========================
// Comparison
// =========================
bool Json::operator==(const Json& other) const {
    if(type_ != other.type_) {
        return false;
    }
    
    switch(type_) {
        case Type::Null:
        return true;
        
        case Type::Bool:
        return asBool() == other.asBool();
        
        case Type::Number:
        return asNumber() == other.asNumber();
        
        case Type::String:
        return asString() == other.asString();
        
        case Type::Array:
        return asArray() == other.asArray();
        
        case Type::Object:
        return asObject() == other.asObject();
    }
    
    return false;
}

bool Json::operator!=(const Json& other) const {
    return !(*this == other);
}

// =========================
// Serialization
// =========================
std::string Json::dump(int indent) const {
	switch(type_) {
	case Type::Null:
		return "null";

	case Type::Bool:
		return asBool() ? "true" : "false";

	case Type::Number: {
		std::ostringstream oss;
		oss << asNumber();

		return oss.str();
	}

	case Type::String:
		return "\"" + asString() + "\"";

	case Type::Array: {
		const auto& arr = asArray();
        
        if(arr.empty()) {
            return "[]";
        }
        
		std::ostringstream oss;
		oss << "[\n";

		for(std::size_t i = 0; i < arr.size(); ++i) {
			oss << pad(indent + 2)
			    << arr[i].dump(indent + 2);
			if(i + 1 != arr.size()) {
				oss << ", ";
			}

			oss << "\n";
		}

		oss << pad(indent) << "]";

		return oss.str();
	}

	case Type::Object: {
		const auto& obj = asObject();
        
        if(obj.empty()) {
            return "{}";
        }
        
		std::ostringstream oss;
		oss << "{\n";

		bool first = true;

		for(const auto& [key, value] : obj) {
			if(!first) {
				oss << ",\n";
			}

			oss << pad(indent + 2)
			    << "\"" << key << "\": " 
			    << value.dump(indent + 2);

			if(first) {
				first = false;
			}
		}

		oss << "\n" << pad(indent) << "}";

		return oss.str();
	}

	}

	return {};
}






