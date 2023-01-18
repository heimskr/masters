#include <cmath>
#include <sstream>

#include "JS.h"
#include "Value.h"

template <typename T, typename... Args>
static T * make(const Value &base, Args &&...args) {
	if (base.context == nullptr)
		return new T(std::forward<Args>(args)...);
	return base.context->makeValue<T>(std::forward<Args>(args)...);
}

Array::operator std::string() const {
	std::ostringstream oss;
	bool first = true;
	for (const Value *value: values) {
		if (first)
			first = false;
		else
			oss << ',';
		oss << std::string(*value);	
	}
	return oss.str();
}

Value * Null::operator+(const Value &right) const {
	if (const auto *null = dynamic_cast<const Null *>(&right))
		return make<Number>(*this, 0.);

	if (const auto *undefined = dynamic_cast<const Undefined *>(&right))
		return make<Number>(*this, nan(""));

	if (const auto *symbol = dynamic_cast<const Symbol *>(&right))
		throw JSError("Cannot convert Symbol to number");

	if (const auto *object = dynamic_cast<const Object *>(&right))
		return make<String>(*this, std::string(*this) + std::string(*object));

	if (const auto *array = dynamic_cast<const Array *>(&right))
		return make<String>(*this, std::string(*this) + std::string(*array));

	if (const auto *number = dynamic_cast<const Number *>(&right))
		return make<Number>(*this, number->number);

	if (const auto *boolean = dynamic_cast<const Boolean *>(&right))
		return make<Number>(*this, boolean->boolean? 1. : 0.);

	if (const auto *string = dynamic_cast<const Array *>(&right))
		return make<String>(*this, std::string(*this) + std::string(*string));

	throw std::runtime_error("Null::operator+ not valid for RHS of type " + std::string(typeid(right).name()));
}

Value * Null::operator-(const Value &right) const {
	if (const auto *null = dynamic_cast<const Null *>(&right))
		return make<Number>(*this, 0.);

	if (const auto *undefined = dynamic_cast<const Undefined *>(&right))
		return make<Number>(*this, nan(""));

	if (const auto *symbol = dynamic_cast<const Symbol *>(&right))
		throw JSError("Cannot convert Symbol to number");

	if (const auto *object = dynamic_cast<const Object *>(&right))
		return make<Number>(*this, nan(""));

	if (const auto *array = dynamic_cast<const Array *>(&right))
		return make<Number>(*this, nan(""));

	if (const auto *number = dynamic_cast<const Number *>(&right))
		return make<Number>(*this, -number->number);

	if (const auto *boolean = dynamic_cast<const Boolean *>(&right))
		return make<Number>(*this, boolean->boolean? -1. : 0.);

	if (const auto *string = dynamic_cast<const Array *>(&right))
		return make<Number>(*this, nan(""));

	throw std::runtime_error("Null::operator- not valid for RHS of type " + std::string(typeid(right).name()));
}

Value * Null::operator*(const Value &right) const {
	if (const auto *null = dynamic_cast<const Null *>(&right))
		return make<Number>(*this, 0.);

	if (const auto *undefined = dynamic_cast<const Undefined *>(&right))
		return make<Number>(*this, nan(""));

	if (const auto *symbol = dynamic_cast<const Symbol *>(&right))
		throw JSError("Cannot convert Symbol to number");

	if (const auto *object = dynamic_cast<const Object *>(&right))
		return make<Number>(*this, nan(""));

	if (const auto *array = dynamic_cast<const Array *>(&right))
		return make<Number>(*this, nan(""));

	if (const auto *number = dynamic_cast<const Number *>(&right))
		return make<Number>(*this, 0.);

	if (const auto *boolean = dynamic_cast<const Boolean *>(&right))
		return make<Number>(*this, 0.);

	if (const auto *string = dynamic_cast<const Array *>(&right))
		return make<Number>(*this, nan(""));

	throw std::runtime_error("Null::operator* not valid for RHS of type " + std::string(typeid(right).name()));
}

Value * Null::operator/(const Value &right) const {
	if (const auto *null = dynamic_cast<const Null *>(&right))
		return make<Number>(*this, nan(""));

	if (const auto *undefined = dynamic_cast<const Undefined *>(&right))
		return make<Number>(*this, nan(""));

	if (const auto *symbol = dynamic_cast<const Symbol *>(&right))
		throw JSError("Cannot convert Symbol to number");

	if (const auto *object = dynamic_cast<const Object *>(&right))
		return make<Number>(*this, nan(""));

	if (const auto *array = dynamic_cast<const Array *>(&right))
		return make<Number>(*this, nan("")); // Not always correct: in Node, null / [[[[1]]]] is 0

	if (const auto *number = dynamic_cast<const Number *>(&right))
		return make<Number>(*this, 0. / number->number);

	if (const auto *boolean = dynamic_cast<const Boolean *>(&right))
		return make<Number>(*this, 0. / (boolean->boolean? 1. : 0.));

	if (const auto *string = dynamic_cast<const Array *>(&right))
		return make<Number>(*this, nan(""));

	throw std::runtime_error("Null::operator/ not valid for RHS of type " + std::string(typeid(right).name()));
}

Value * Null::operator%(const Value &right) const {
	if (const auto *null = dynamic_cast<const Null *>(&right))
		return make<Number>(*this, nan(""));

	if (const auto *undefined = dynamic_cast<const Undefined *>(&right))
		return make<Number>(*this, nan(""));

	if (const auto *symbol = dynamic_cast<const Symbol *>(&right))
		throw JSError("Cannot convert Symbol to number");

	if (const auto *object = dynamic_cast<const Object *>(&right))
		return make<Number>(*this, nan(""));

	if (const auto *array = dynamic_cast<const Array *>(&right))
		return make<Number>(*this, nan("")); // Not always correct: in Node, null % [[[[1]]]] is 0

	if (const auto *number = dynamic_cast<const Number *>(&right))
		return make<Number>(*this, std::fmod(0., number->number));

	if (const auto *boolean = dynamic_cast<const Boolean *>(&right))
		return make<Number>(*this, std::fmod(0., boolean->boolean? 1. : 0.));

	if (const auto *string = dynamic_cast<const Array *>(&right))
		return make<Number>(*this, nan(""));

	throw std::runtime_error("Null::operator% not valid for RHS of type " + std::string(typeid(right).name()));
}

Value * Null::operator&(const Value &right) const {
	return *make<Number>(*this, 0.) & right;
}

Value * Null::operator|(const Value &right) const {
	return *make<Number>(*this, 0.) | right;
}

Value * Null::operator^(const Value &right) const {
	return *make<Number>(*this, 0.) ^ right;
}

Value * Null::operator==(const Value &right) const {
	if (const auto *null = dynamic_cast<const Null *>(&right))
		return make<Boolean>(*this, true);

	if (const auto *undefined = dynamic_cast<const Undefined *>(&right))
		return make<Boolean>(*this, true);

	return make<Boolean>(*this, false);
}

Value * Null::operator!=(const Value &right) const {
	if (const auto *null = dynamic_cast<const Null *>(&right))
		return make<Boolean>(*this, false);

	if (const auto *undefined = dynamic_cast<const Undefined *>(&right))
		return make<Boolean>(*this, false);

	return make<Boolean>(*this, true);
}

Value * Null::operator<(const Value &right) const {
	if (const auto *null = dynamic_cast<const Null *>(&right))
		return make<Boolean>(*this, false);

	if (const auto *undefined = dynamic_cast<const Undefined *>(&right))
		return make<Boolean>(*this, false);

	if (const auto *symbol = dynamic_cast<const Symbol *>(&right))
		throw JSError("Cannot convert Symbol to number");

	if (const auto *object = dynamic_cast<const Object *>(&right))
		return make<Boolean>(*this, false);

	if (const auto *array = dynamic_cast<const Array *>(&right))
		return make<Boolean>(*this, false); // Not always correct: in Node, null < [[[[1]]]] is true

	if (const auto *number = dynamic_cast<const Number *>(&right))
		return make<Boolean>(*this, 0. < number->number);

	if (const auto *boolean = dynamic_cast<const Boolean *>(&right))
		return make<Boolean>(*this, boolean->boolean);

	if (const auto *string = dynamic_cast<const Array *>(&right))
		return make<Boolean>(*this, false); // Potentially actually true if the string is of a number...

	throw std::runtime_error("Null::operator< not valid for RHS of type " + std::string(typeid(right).name()));
}
