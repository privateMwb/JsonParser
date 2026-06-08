#pragma once

#include <cctype>
#include <stdexcept>
#include <string>

#include "Json.h"

class Parser {
    private:
    
    // =========================
    // Parser State
    // =========================
    const std::string& input_;
    std::size_t pos_;
    
    std::size_t line_ = 1;
    std::size_t column_ = 1;
    
    // =========================
    // Character Utilities 
    // =========================
    char peek() const;
    char get();
    void skipWhitespace();
    
    void error(const std::string& msg) const;
    
    // =========================
    // Validation
    // =========================
    void ensureEndOfInput();
    
    // =========================
    // Unicode Helpers
    // =========================
    int hexDigitToInt(char c);
    void appendUtf8(std::string& result, unsigned int codepoint);
    
    
    // =========================
    // Parsing
    // =========================
    Json parseValue();
    
    Json parseNull();
    Json parseBool();
    Json parseNumber();
    Json parseString();
    Json parseArray();
    Json parseObject();
    
    public:
    
    // =========================
    // Constructor
    // =========================
    explicit Parser(const std::string& input);
    
    // =========================
    // Parsing
    // =========================
    Json parse();
};