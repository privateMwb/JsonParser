#include <cctype>
#include <stdexcept>
#include <string>

#include "Parser.h"

// =========================
// Character State
// =========================
char Parser::peek() const {
	if(pos_ >= input_.size()) {
		return '\0';
	}

	return input_[pos_];
}

char Parser::get() {
	if(pos_ >= input_.size()) {
		return '\0';
	}

	char c = input_[pos_++];

	if(c == '\n') {
		++line_;
		column_ = 1;
	} else {
		++column_;
	}

	return c;
}

void Parser::skipWhitespace() {
	while(std::isspace(static_cast<unsigned char>(peek()))) {
		get();
	}
}

void Parser::error(const std::string& msg) const {
	throw std::runtime_error(
	    "line " + std::to_string(line_) +
	    ", column " + std::to_string(column_) +
	    ": " + msg);
}

// =========================
// Validation
// =========================
void Parser::ensureEndOfInput() {
	skipWhitespace();

	if(peek() != '\0') {
		error("unexpected trailing characters");
	}
}

// =========================
// Unicode Helpers
// =========================
int Parser::hexDigitToInt(char c) {
	if(c >= '0' && c <= '9') {
		return c - '0';
	}

	if(c >= 'a' && c <= 'f') {
		return c - 'a' + 10;
	}

	if(c >= 'A' && c <= 'F') {
		return c - 'A' + 10;
	}

	error("invalid hexadecimal digit");

	return 0;
}

void Parser::appendUtf8(std::string& result, unsigned int codepoint) {
	if(codepoint >= 0xD800 && codepoint <= 0xDFFF) {
		error("invalid surrogate codepoint");
	}

	if (codepoint <= 0x7F) {
		result += static_cast<char>(codepoint);
	}
	else if (codepoint <= 0x7FF) {
		result += static_cast<char>(0xC0 | (codepoint >> 6));
		result += static_cast<char>(0x80 | (codepoint & 0x3F));
	}
	else if (codepoint <= 0xFFFF) {
		result += static_cast<char>(0xE0 | (codepoint >> 12));
		result += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
		result += static_cast<char>(0x80 | (codepoint & 0x3F));
	}
	else if (codepoint <= 0x10FFFF) {
		result += static_cast<char>(0xF0 | (codepoint >> 18));
		result += static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F));
		result += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
		result += static_cast<char>(0x80 | (codepoint & 0x3F));
	}
	else {
		error("invalid Unicode codepoint");
	}
}

// =========================
// Parsing
// =========================
Json Parser::parseValue() {
	skipWhitespace();

	switch(peek()) {
	case 'n' :
		return parseNull();

	case 't':
	case 'f':
		return parseBool();

	case '"':
		return parseString();

	case '[':
		return parseArray();

	case '{':
		return parseObject();

	default:
		if(std::isdigit(static_cast<unsigned char>(peek())) || peek() == '-') {
			return parseNumber();
		}

		error("invalid JSON value");

		return Json();
	}

}

Json Parser::parseNull() {
	if(input_.substr(pos_, 4) != "null") {
		error("expected 'null'");
	}

	pos_ += 4;

	return Json(nullptr);
}

Json Parser::parseBool() {
	std::string res;

	if(peek() == 't') {
		res = input_.substr(pos_, 4);
	} else {
		res = input_.substr(pos_, 5);
	}

	if(res != "true" && res != "false") {
		error("expected bool");
	}

	pos_ += res.size();

	return res == "true" ? Json(true) : Json(false);
}

Json Parser::parseNumber() {
	std::size_t start = pos_;

	if(peek() == '-') {
		get();
	}

	if(!std::isdigit(static_cast<unsigned char>(peek()))) {
		error("expected digit");
	}

	if(peek() == '0') {
		get();

		if(std::isdigit(static_cast<unsigned char>(peek()))) {
			error("leading zeros are not allowed");
		}
	}

	while(std::isdigit(static_cast<unsigned char>(peek()))) {
			get();
	}

	if(peek() == '.') {
		get();

		if(!std::isdigit(static_cast<unsigned char>(peek()))) {
			error("expected digit after decimal point");
		}

		while(std::isdigit(static_cast<unsigned char>(peek()))) {
			get();
		}
	}

	if(peek() == 'e' || peek() == 'E') {
		get();

		if(peek() == '+' || peek() == '-') {
			get();
		}

		if(!std::isdigit(static_cast<unsigned char>(peek()))) {
			error("expected exponent digit");
		}

		while(std::isdigit(static_cast<unsigned char>(peek()))) {
			get();
		}
	}

	std::string number = input_.substr(start, pos_ - start);

	return Json(std::stod(number));
}

Json Parser::parseString() {
	if(get() != '"') {
		error("expected '\"'");
	}

	std::string result;

	while(true) {
		char c = get();

		if(c == '\0') {
			error("unterminated string");
		}

		if(c == '"') {
			break;
		}

		if(c == '\\') {
			char esc = get();

			if(esc == '\0') {
				error("unterminated escape sequence");
			}

			switch(esc) {
			case '"':
				result += '"';
				break;

			case '\\':
				result += '\\';
				break;

			case '/':
				result += '/';
				break;

			case 'n':
				result += '\n';
				break;

			case 't':
				result += '\t';
				break;

			case 'r':
				result += '\r';
				break;

			case 'b':
				result += '\b';
				break;

			case 'f':
				result += '\f';
				break;

			case 'u': {
				unsigned int codepoint = 0;

				for(int i = 0; i < 4; i++) {
					codepoint <<= 4;
					codepoint |= hexDigitToInt(get());
				}

				if (codepoint >= 0xD800 && codepoint <= 0xDBFF) {
					if (get() != '\\') {
						error("invalid surrogate pair");
					}

					if (get() != 'u') {
						error("invalid surrogate pair");
					}

					unsigned int low = 0;

					for(int i = 0; i < 4; ++i) {
						low <<= 4;
						low |= hexDigitToInt(get());
					}

					if (low < 0xDC00 || low > 0xDFFF) {
						error("invalid low surrogate");
					}

					codepoint = 0x10000
					            + ((codepoint - 0xD800) << 10)
					            + (low - 0xDC00);
				}


				appendUtf8(result, codepoint);
				break;
			}

			default:
				error("invalid escape  sequence");
			}
		}
		else {
			result += c;
		}
	}

	return Json(result);
}

Json Parser::parseArray() {
	if(get() != '[') {
		error("expected '['");
	}

	Json::Array arr;

	skipWhitespace();

	if(peek() == ']') {
		get();

		return Json(arr);
	}

	while(true) {
		arr.push_back(parseValue());

		skipWhitespace();

		if (peek() == ',') {
			get();

			skipWhitespace();
		}
		else if (peek() == ']') {
			get();

			break;
		}
		else {
			error("expected ',' or ']'");
		}

	}

	return Json(arr);
}

Json Parser::parseObject() {
	if(get() != '{') {
		error("expected '{'");
	}

	Json::Object obj;

	skipWhitespace();

	if(peek() == '}') {
		get();

		return Json(obj);
	}

	while(true) {
		skipWhitespace();

		if (peek() != '"') {
			error("expected string key");
		}

		Json key = parseString();
		std::string keyStr = key.asString();

		skipWhitespace();

		if (get() != ':') {
			error("expected ':'");
		}

		skipWhitespace();

		Json value = parseValue();
		obj[keyStr] = value;

		if (peek() == ',') {
			get();

			continue;
		}
		if (peek() == '}') {
			get();

			break;
		}

		error("expected ',' or '}'");

	}

	return Json(obj);
}

// =========================
// Constructor
// =========================
Parser::Parser(const std::string& input) :
	input_(input),
	pos_(0) {}

// =========================
// Parsing
// =========================
Json Parser::parse() {
	skipWhitespace();

	Json value = parseValue();

	ensureEndOfInput();

	return value;
}