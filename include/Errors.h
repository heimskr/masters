#pragma once

#include <stdexcept>

#include "ASTNode.h"

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

struct GenericError: std::runtime_error {
	ASTLocation location;
	using std::runtime_error::runtime_error;
	template <typename... Args>
	explicit GenericError(const ASTLocation &location_, Args &&...args):
		std::runtime_error(std::forward<Args>(args)...), location(location_) {}
} __attribute__((packed, aligned(16)));

struct ConstError: JSException {
	using JSException::JSException;
};
