#pragma once

#include <stdexcept>

#include "ASTNode.h"

struct ConversionError: std::runtime_error {
	using std::runtime_error::runtime_error;
};

struct JSError: std::runtime_error {
	size_t line = 0;
	size_t column = 0;

	JSError(std::string message, size_t line_ = 0, size_t column_ = 0):
		std::runtime_error(std::move(message)),
		line(line_),
		column(column_) {}

	JSError(std::string message, const ASTLocation &location):
		JSError(std::move(message), location.line + 1, location.column + 1) {}
};

struct GenericError: std::runtime_error {
	ASTLocation location;
	using std::runtime_error::runtime_error;
	template <typename... Args>
	explicit GenericError(const ASTLocation &location_, Args &&...args):
		std::runtime_error(std::forward<Args>(args)...), location(location_) {}
};

struct ConstError: JSError {
	using JSError::JSError;
};

struct Unimplemented: std::runtime_error {
	Unimplemented(): std::runtime_error("Unimplemented") {}
	Unimplemented(std::string message): std::runtime_error(std::move(message)) {}
};

struct ReferenceError: JSError {
	ReferenceError(const std::string &name, const ASTLocation &location):
		JSError(name + " is not defined", location) {}
};

struct TypeError: JSError {
	using JSError::JSError;
};

struct SyntaxError: JSError {
	using JSError::JSError;
};
