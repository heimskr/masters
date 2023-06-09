#include <cmath>
#include <sstream>

#include "Errors.h"
#include "JS.h"
#include "Utils.h"
#include "Value.h"

template <typename T, typename... Args>
static T * make(const Value &base, Args &&...args) {
	if (base.context == nullptr)
		return new T(std::forward<Args>(args)...);
	return base.context->makeValue<T>(std::forward<Args>(args)...);
}

std::unordered_set<Value *> Object::getReferents() const {
	std::unordered_set<Value *> out;
	out.reserve(map.size());
	for (const auto &[name, value]: map)
		out.insert(value);
	return out;
}

Array::operator std::string() const {
	std::ostringstream oss;
	bool first = true;

	if (isHoley()) {
		for (const auto &[key, value]: std::get<Holey>(values)) {
			if (first)
				first = false;
			else
				oss << ',';
			if (value != nullptr)
				oss << static_cast<std::string>(*value);
		}
	} else {
		for (const Value *value: std::get<Holeless>(values)) {
			if (first)
				first = false;
			else
				oss << ',';
			assert(value != nullptr);
			oss << static_cast<std::string>(*value);
		}
	}

	return oss.str();
}

Array::operator double() const {
	if (empty())
		return 0;

	if (!isHoley()) {
		Value *first = (*this)[0];
		assert(first);
		return static_cast<double>(*first);
	}

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

std::unordered_set<Value *> Array::getReferents() const {
	if (!isHoley()) {
		const auto &holeless = std::get<Holeless>(values);
		return {holeless.begin(), holeless.end()};
	}

	std::unordered_set<Value *> out;
	for (const auto &[key, value]: std::get<Holey>(values))
		out.insert(value);
	return out;
}

Number * Array::toNumber() const {
	if (isHoley()) {
		const auto &holey = std::get<Holey>(values);
		if (holey.size() == 1) {
			const auto &[key, value] = *holey.begin();
			if (key == 0)
				return value->toNumber();
		}
	} else {
		const auto &holeless = std::get<Holeless>(values);
		if (holeless.size() == 1)
			return holeless.front()->toNumber();
	}

	return make<Number>(*this, nan(""));
}

Value * Array::operator[](size_t index) const {
	if (isHoley()) {
		const auto &holey = std::get<Holey>(values);
		if (auto iter = holey.find(index); iter != holey.end())
			return iter->second;
	} else {
		const auto &holeless = std::get<Holeless>(values);
		if (index < holeless.size())
			return holeless.at(index);
	}

	return nullptr;
}

size_t Array::size() const {
	if (isHoley())
		return std::get<Holey>(values).size();
	return std::get<Holeless>(values).size();
}

bool Array::empty() const {
	if (isHoley())
		return std::get<Holey>(values).empty();
	return std::get<Holeless>(values).empty();
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

Value * String::operator+(const Value &other) const {
	switch (other.getType()) {
		case ValueType::String:
			return make<String>(*this, string + dynamic_cast<const String &>(other).string);
		case ValueType::Number:
			return make<String>(*this, string + std::to_string(dynamic_cast<const Number &>(other).number));
		default:
			throw TypeError("Invalid string concatenation");
	}
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
	if (dynamic_cast<const Null *>(&right))
		return make<Boolean>(*this, true);

	return make<Boolean>(*this, false);
}

Value * Undefined::operator==(const Value &right) const {
	if (dynamic_cast<const Undefined *>(&right))
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

ValueType Reference::ultimateType() const {
	assertReferent();
	return (*referent)->ultimateType();
}

std::unordered_set<Value *> Reference::getReferents() const {
	assertReferent();
	return {*referent};
}

Reference::operator std::string() const {
	assertReferent();
	return static_cast<std::string>(**referent);
}

Reference::operator double() const {
	assertReferent();
	return static_cast<double>(**referent);
}

Reference::operator bool() const {
	assertReferent();
	return static_cast<bool>(**referent);
}

Value * Reference::operator+(const Value &other) const {
	assertReferent();
	return **referent + other;
}

Value * Reference::operator-(const Value &other) const {
	assertReferent();
	return **referent - other;
}

Value * Reference::operator*(const Value &other) const {
	assertReferent();
	return **referent * other;
}

Value * Reference::operator/(const Value &other) const {
	assertReferent();
	return **referent / other;
}

Value * Reference::operator%(const Value &other) const {
	assertReferent();
	return **referent % other;
}

Value * Reference::operator&(const Value &other) const {
	assertReferent();
	return **referent & other;
}

Value * Reference::operator|(const Value &other) const {
	assertReferent();
	return **referent | other;
}

Value * Reference::operator^(const Value &other) const {
	assertReferent();
	return **referent ^ other;
}

Value * Reference::operator==(const Value &other) const {
	assertReferent();
	return **referent == other;
}

Value * Reference::operator!=(const Value &other) const {
	assertReferent();
	return **referent != other;
}

Value * Reference::operator<(const Value &other) const {
	assertReferent();
	return **referent < other;
}

Value * Reference::operator>(const Value &other) const {
	assertReferent();
	return **referent > other;
}

Value * Reference::operator<=(const Value &other) const {
	assertReferent();
	return **referent <= other;
}

Value * Reference::operator>=(const Value &other) const {
	assertReferent();
	return **referent >= other;
}

Value * Reference::power(const Value &other) const {
	assertReferent();
	return (*referent)->power(other);
}

Value * Reference::operator&&(const Value &other) const {
	assertReferent();
	return **referent && other;
}

Value * Reference::operator||(const Value &other) const {
	assertReferent();
	return **referent || other;
}

Value * Reference::operator<<(const Value &other) const {
	assertReferent();
	return **referent << other;
}

Value * Reference::shiftRightLogical(const Value &other) const {
	assertReferent();
	return (*referent)->shiftRightLogical(other);
}

Value * Reference::shiftRightArithmetic(const Value &other) const {
	assertReferent();
	return (*referent)->shiftRightArithmetic(other);
}

Function::Function(FunctionType function_, Value *this_obj, std::unordered_set<Value *> closure_):
	function(std::move(function_)),
	thisObj(this_obj),
	closure(std::move(closure_)) {}

Number * Function::toNumber() const {
	return make<Number>(*this, nan(""));
}

Value * Function::operator==(const Value &other) const {
	return make<Boolean>(*this, this == &other);
}

std::unordered_set<Value *> Function::getReferents() const {
	auto out = closure;
	if (thisObj != nullptr)
		out.insert(thisObj);
	return out;
}
