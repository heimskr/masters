#include <cmath>
#include <sstream>

#include "JS.h"
#include "Utils.h"
#include "Value.h"

template <typename T, typename... Args>
static T * make(const Value &base, Args &&...args) {
	if (base.context == nullptr)
		return new T(std::forward<Args>(args)...);
	return base.context->makeValue<T>(std::forward<Args>(args)...);
}

std::vector<Value *> Object::getReferents() const {
	std::vector<Value *> out;
	out.reserve(map.size());
	for (const auto &[name, value]: map)
		out.push_back(value);
	return out;
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

Array::operator double() const {
	if (values.empty())
		return 0;

	if (values.size() == 1)
		return static_cast<double>(*values.front());

	return nan("");
}

Number * Null::toNumber() const {
	return make<Number>(*this, 0.);
}

Number * Undefined::toNumber() const {
	return make<Number>(*this, nan(""));
}

Number * Object::toNumber() const {
	return make<Number>(*this, nan(""));
}

Number * Array::toNumber() const {
	// Not quite correct. Number([[[[[4]]]]]) evaluates to 4.
	return make<Number>(*this, nan(""));
}

Number * Number::toNumber() const {
	return make<Number>(*this, number);
}

Number * Boolean::toNumber() const {
	return make<Number>(*this, boolean? 1. : 0.);
}

String::operator double() const {
	if (string.empty())
		return 0; // For some reason.

	try {
		return parseDouble(string);
	} catch (const std::invalid_argument &) {
		return nan("");
	}
}

Number * String::toNumber() const {
	return make<Number>(*this, static_cast<double>(*this));
}

Value * Value::operator+(const Value &other) const {
	if (const auto *string = dynamic_cast<const String *>(&other))
		return make<String>(*this, static_cast<std::string>(*this) + string->string);

	return make<Number>(*this, static_cast<double>(*this) + static_cast<double>(other));
}

Value * Value::operator-(const Value &other) const {
	return make<Number>(*this, static_cast<double>(*this) - static_cast<double>(other));
}

Value * Value::operator*(const Value &other) const {
	return make<Number>(*this, static_cast<double>(*this) * static_cast<double>(other));
}

Value * Value::operator/(const Value &other) const {
	return make<Number>(*this, static_cast<double>(*this) / static_cast<double>(other));
}

Value * Value::operator%(const Value &other) const {
	return make<Number>(*this, std::fmod(static_cast<double>(*this), static_cast<double>(other)));
}

Value * Value::operator&(const Value &other) const {
	return make<Number>(*this, static_cast<size_t>(static_cast<double>(*this)) &
	                           static_cast<size_t>(static_cast<double>(other)));
}

Value * Value::operator|(const Value &other) const {
	return make<Number>(*this, static_cast<size_t>(static_cast<double>(*this)) |
	                           static_cast<size_t>(static_cast<double>(other)));
}

Value * Value::operator^(const Value &other) const {
	return make<Number>(*this, static_cast<size_t>(static_cast<double>(*this)) ^
	                           static_cast<size_t>(static_cast<double>(other)));
}

Value * Value::power(const Value &other) const {
	return make<Number>(*this, std::pow(static_cast<double>(*this), static_cast<double>(other)));
}

Value * Value::operator&&(const Value &other) const {
	return make<Boolean>(*this, static_cast<bool>(*this) && static_cast<bool>(other));
}

Value * Value::operator||(const Value &other) const {
	return make<Boolean>(*this, static_cast<bool>(*this) || static_cast<bool>(other));
}

Value * Value::operator!=(const Value &other) const {
	return make<Boolean>(*this, !(*this == other));
}

Value * Value::operator<(const Value &other) const {
	return make<Boolean>(*this, static_cast<double>(*this) < static_cast<double>(other));
}

Value * Value::operator>(const Value &other) const {
	return make<Boolean>(*this, static_cast<double>(*this) > static_cast<double>(other));
}

Value * Value::operator<=(const Value &other) const {
	return make<Boolean>(*this, static_cast<double>(*this) <= static_cast<double>(other));
}

Value * Value::operator>=(const Value &other) const {
	return make<Boolean>(*this, static_cast<double>(*this) >= static_cast<double>(other));
}

Value * Value::operator<<(const Value &other) const {
	return make<Number>(*this, static_cast<int64_t>(*this) << static_cast<int64_t>(other));
}

Value * Value::shiftRightLogical(const Value &other) const {
	return make<Number>(*this, static_cast<uint64_t>(*this) >> static_cast<uint64_t>(other));
}

Value * Value::shiftRightArithmetic(const Value &other) const {
	return make<Number>(*this, static_cast<int64_t>(*this) >> static_cast<int64_t>(other));
}

Value * Null::operator==(const Value &right) const {
	if (const auto *null = dynamic_cast<const Null *>(&right))
		return make<Boolean>(*this, true);

	return make<Boolean>(*this, false);
}

Value * Undefined::operator==(const Value &right) const {
	if (const auto *undefined = dynamic_cast<const Undefined *>(&right))
		return make<Boolean>(*this, true);

	return make<Boolean>(*this, false);
}

Value * Number::operator==(const Value &right) const {
	if (const auto *other_number = dynamic_cast<const Number *>(&right))
		return make<Boolean>(*this, number == other_number->number);

	return make<Boolean>(*this, false);
}

Value * Boolean::operator==(const Value &right) const {
	if (const auto *other_boolean = dynamic_cast<const Boolean *>(&right))
		return make<Boolean>(*this, boolean == other_boolean->boolean);

	return make<Boolean>(*this, false);
}

Value * String::operator==(const Value &right) const {
	if (const auto *other_string = dynamic_cast<const String *>(&right))
		return make<Boolean>(*this, string == other_string->string);

	return make<Boolean>(*this, false);
}

Value * Object::operator==(const Value &right) const {
	return make<Boolean>(*this, this == &right);
}

Value * Array::operator==(const Value &right) const {
	return make<Boolean>(*this, this == &right);
}

Value * Reference::operator+(const Value &other) const {
	assert(referent);
	return *referent + other;
}

Value * Reference::operator-(const Value &other) const {
	assert(referent);
	return *referent - other;
}

Value * Reference::operator*(const Value &other) const {
	assert(referent);
	return *referent * other;
}

Value * Reference::operator/(const Value &other) const {
	assert(referent);
	return *referent / other;
}

Value * Reference::operator%(const Value &other) const {
	assert(referent);
	return *referent % other;
}

Value * Reference::operator&(const Value &other) const {
	assert(referent);
	return *referent & other;
}

Value * Reference::operator|(const Value &other) const {
	assert(referent);
	return *referent | other;
}

Value * Reference::operator^(const Value &other) const {
	assert(referent);
	return *referent ^ other;
}

Value * Reference::operator==(const Value &other) const {
	assert(referent);
	return *referent == other;
}

Value * Reference::operator!=(const Value &other) const {
	assert(referent);
	return *referent != other;
}

Value * Reference::operator<(const Value &other) const {
	assert(referent);
	return *referent < other;
}

Value * Reference::operator>(const Value &other) const {
	assert(referent);
	return *referent > other;
}

Value * Reference::operator<=(const Value &other) const {
	assert(referent);
	return *referent <= other;
}

Value * Reference::operator>=(const Value &other) const {
	assert(referent);
	return *referent >= other;
}

Value * Reference::power(const Value &other) const {
	assert(referent);
	return referent->power(other);
}

Value * Reference::operator&&(const Value &other) const {
	assert(referent);
	return *referent && other;
}

Value * Reference::operator||(const Value &other) const {
	assert(referent);
	return *referent || other;
}

Value * Reference::operator<<(const Value &other) const {
	assert(referent);
	return *referent << other;
}

Value * Reference::shiftRightLogical(const Value &other) const {
	assert(referent);
	return referent->shiftRightLogical(other);
}

Value * Reference::shiftRightArithmetic(const Value &other) const {
	assert(referent);
	return referent->shiftRightArithmetic(other);
}

Function::Function(decltype(function) function_, Value *this_obj):
	function(std::move(function_)),
	thisObj(this_obj) {}

Number * Function::toNumber() const {
	return make<Number>(*this, nan(""));
}

Value * Function::operator==(const Value &other) const {
	return make<Boolean>(*this, this == &other);
}
