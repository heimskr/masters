#include <charconv>
#include <cstring>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "Utils.h"

int64_t parseLong(std::string_view view, int base) {
	int64_t out = 0;
	const auto result = std::from_chars(view.begin(), view.end(), out, base);
	if (result.ec == std::errc::invalid_argument)
		throw std::invalid_argument("Not an integer: \"" + std::string(view) + '"');
	return out;
}

uint64_t parseUlong(std::string_view view, int base) {
	uint64_t out = 0;
	const auto result = std::from_chars(view.begin(), view.end(), out, base);
	if (result.ec == std::errc::invalid_argument)
		throw std::invalid_argument("Not an integer: \"" + std::string(view) + '"');
	return out;
}

double parseDouble(std::string_view view) {
	double out = 0;
	const auto result = std::from_chars(view.begin(), view.end(), out);
	if (result.ec == std::errc::invalid_argument)
		throw std::invalid_argument("Not a floating point number: \"" + std::string(view) + '"');
	return out;
}

std::string unescapeString(std::string_view str) {
	const size_t size = str.size();
	if (size == 0)
		return "";
	std::stringstream out;
	for (size_t i = 0; i < size; ++i) {
		char ch = str[i];
		if (ch == '\\') {
			if (i == size - 1)
				throw std::runtime_error("Backslash at end of string");
			switch (str[++i]) {
				case 'n':  out << '\n'; break;
				case 'r':  out << '\r'; break;
				case 'a':  out << '\a'; break;
				case 't':  out << '\t'; break;
				case 'b':  out << '\b'; break;
				case 'e':  out << '\e'; break;
				case '0':  out << '\0'; break;
				case '\\': out << '\\'; break;
				case '"':  out << '"';  break;
				case 'x': {
					if (size <= i + 2)
						throw std::runtime_error("Hexadecimal escape is too close to end of string");
					const char first  = str[++i];
					const char second = str[++i];
					if (isxdigit(first) == 0 || isxdigit(second) == 0)
						throw std::runtime_error(std::string("Invalid hexadecimal escape: \\x") + first + second);
					out << char(strtol((std::string(1, first) + second).c_str(), nullptr, 16));
					break;
				}
				default: throw std::runtime_error("Unrecognized character: \\" + std::string(1, str[i]));
			}
		} else
			out << ch;
	}
	return out.str();
}

template <>
std::vector<std::string_view> split<std::string_view>(std::string_view str, std::string_view delimiter, bool condense) {
	if (str.empty())
		return {};

	size_t next = str.find(delimiter);
	if (next == std::string::npos)
		return {str};

	std::vector<std::string_view> out;
	const size_t delimiter_length = delimiter.size();
	size_t last = 0;

	out.push_back(str.substr(0, next));

	while (next != std::string::npos) {
		last = next;
		next = str.find(delimiter, last + delimiter_length);
		std::string_view sub = str.substr(last + delimiter_length, next - last - delimiter_length);
		if (!sub.empty() || !condense)
			out.push_back(sub);
	}

	return out;
}

template <>
std::vector<std::string> split<std::string>(std::string_view str, std::string_view delimiter, bool condense) {
	const auto pieces = split<std::string_view>(str, delimiter, condense);
	std::vector<std::string> out;
	out.reserve(pieces.size());
	for (const auto &piece: pieces)
		out.emplace_back(piece);
	return out;
}

std::string readFile(const std::string &path) {
	std::ifstream file(path);
	if (!file.is_open())
		throw std::runtime_error("Couldn't open file for reading");
	std::string out;
	file.seekg(0, std::ios::end);
	out.reserve(file.tellg());
	file.seekg(0, std::ios::beg);
	out.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	return out;
}
