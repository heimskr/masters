#pragma once

#include <stdexcept>

struct ConversionError: std::runtime_error {
	using std::runtime_error::runtime_error;
};

struct JSException: std::runtime_error {
	size_t line = 0;
	size_t column = 0;

	JSException(std::string message, size_t line_ = 0, size_t column_ = 0):
		std::runtime_error(std::move(message)),
		line(line_),
		column(column_) {}
};

struct ConstError: JSException {
	using JSException::JSException;
};