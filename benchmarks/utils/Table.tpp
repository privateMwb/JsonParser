#pragma once

#include "sstream"

namespace Table {
    
// =========================
// Text Formatting
// =========================
inline std::string center(
    const std::string& text,
    std::size_t width)
{
	if(text.size() >= width) {
		return text;
	}

	auto left = (width - text.size()) / 2;
	auto right = width - text.size() - left;

	return std::string(left, ' ')
	       + text
	       + std::string(right, ' ');
}

template<typename T>
std::string format(
    const T& value,
    const std::string& unit)
{
	std::ostringstream oss;
	oss << value;

	return oss.str()
	       + " "
	       + unit;
}

inline std::string repeat(
    const std::string& line,
    std::size_t count)
{
	std::string lines;

	for(std::size_t i = 0; i < count; ++i) {
		lines += line;
	}

	return lines;
}

template<typename T>
std::vector<std::string> convert(
    const std::vector<T>& data,
    const std::string& unit)
{
    std::vector<std::string> converted;
    
    for(std::size_t i = 0; i < data.size(); ++i) {
        converted.push_back(format(data[i], unit));
    }
    
    return converted;
}

// =========================
// Table Rendering
// =========================
inline void border(
    const std::string& left,
    const std::string& middle,
    const std::string& right,
    const std::string& line,
    const std::vector<std::size_t>& lengths)
{
	std::cout << left;

	for(std::size_t i = 0; i < lengths.size(); ++i) {
		std::cout << repeat(line, lengths[i]) << (i < lengths.size() - 1 ? middle : "");
	}

	std::cout << right << "\n";
}

inline void table_content(
    const std::string& side,
    const std::vector<std::string>& titles,
    const std::vector<std::size_t>& lengths)
{
	if(titles.size() != lengths.size()) {
		return;
	}

	for(std::size_t i = 0; i < lengths.size(); ++i) {
		std::cout << side
		          << center(titles[i], lengths[i])
		          << (i == lengths.size() - 1 ? side + "\n" : "");
	}
}

inline void table(
    const std::string& title,
    const std::vector<std::string>& headers,
    const TableData& data,
    std::size_t length)
{
	std::size_t each = length / data.size();
	std::size_t margin = ((each * data.size()) + data.size() + 1) - length - 2;
	std::vector<std::size_t> top = {length + margin};
	std::vector<std::size_t> section;
	for(std::size_t i = 0; i < data.size(); ++i) {
		section.push_back(each);
	}

	border("┌", "─", "┐", "─", top);
	table_content("│", {title}, top);
	border("├", "┬", "┤", "─", section);
	table_content("│", headers, section);
	border("├", "┼", "┤", "─", section);

	for(std::size_t i = 0; i < data[0].size(); ++i) {
	    std::vector<std::string> d;
	    
	    for(std::size_t j = 0; j < data.size(); ++j) {
	        d.push_back(data[j][i]);
	    }
	    
	    table_content("│", d, section);
	}

	border("└", "┴", "┘", "─", section);
}
};