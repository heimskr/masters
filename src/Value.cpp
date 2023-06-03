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

Value * Null::operator==(const Value &right) const {
	if (const auto *null = dynamic_cast<const Null *>(&right))
		return make<Boolean>(*this, true);

	return make<Boolean>(*this, false);
}
