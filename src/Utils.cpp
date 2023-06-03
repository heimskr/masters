#include <charconv>
#include <cstring>
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
