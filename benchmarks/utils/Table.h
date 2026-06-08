#pragma once

#include <string>
#include <cstddef>
#include <vector>

namespace Table {
    
// =========================
// Type Aliases
// =========================
using TableData = std::vector<std::vector<std::string>>;

// =========================
// Text Formatting
// =========================
std::string center(
    const std::string& text,
    std::size_t width
);

std::string repeat(
    const std::string& line,
    std::size_t count
);

template<typename T>
std::string format(
    const T& value,
    const std::string& unit = ""
);

template<typename T>
std::vector<std::string> convert(
    const std::vector<T>& data,
    const std::string& unit = ""
);

// =========================
// Table Rendering
// =========================
void border(
    const std::string& left,
    const std::string& middle,
    const std::string& right,
    const std::string& line,
    const std::vector<std::size_t>& lengths
);

void content(
    const std::string& side,
    const std::vector<std::string>& titles,
    const std::vector<std::size_t>& lengths
);

void table(
    const std::string& title,
    const std::vector<std::string>& headers,
    const TableData& data,
    std::size_t length
);

};

#include "Table.tpp"