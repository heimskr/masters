#include <cmath>
#include <sstream>

#include "Errors.h"
#include "JS.h"
#include "Log.h"
#include "Utils.h"
#include "Value.h"

template <typename T, typename... Args>
T * Value::make(Args &&...args) const {
	assert(context != nullptr);
	return context->makeValue<T>(std::forward<Args>(args)...);
}

std::unordered_set<Value *> Value::getReferents() const {
	if (customPrototype && *customPrototype != nullptr)
		return {*customPrototype};
	return {};
}

Object * Value::getPrototype(Context &context) const {
	if (customPrototype)
		return *customPrototype;

	if (auto iter = context.stack.globals.find(className()); iter != context.stack.globals.end()) {
		auto *object = iter->second->referent->cast<Object>();
		assert(object != nullptr);
		if (auto iter = object->map.find("prototype"); iter != object->map.end()) {
			auto *prototype = iter->second->referent->cast<Object>();
			assert(prototype != nullptr);
			return prototype;
		}
	}

	throw TypeError("Cannot get prototype of type " + getName());
}

Reference * Value::access(Value *, bool) {
	return nullptr;
}

Reference * Value::access(const std::string &, bool) {
	return nullptr;
}

Array::operator std::string() const {
	std::ostringstream oss;

	if (isHoley()) {
		size_t prev_key = 0;
		std::optional<size_t> last_key;
		for (const auto &[key, value]: std::get<Holey>(values)) {
			if (prev_key < key)
				oss << std::string(key - prev_key, ',');
			if (value != nullptr)
				oss << static_cast<std::string>(*value);
			prev_key = key;
			last_key = key;
		}

		if (last_key) {
			if (*last_key + 1 < holeyLength)
				oss << std::string(holeyLength - *last_key - 1, ',');
		} else if (0 < holeyLength)
			oss << std::string(holeyLength - 1, ',');
	} else {
		bool first = true;
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

std::unordered_set<Value *> Array::getReferents() const {
	auto out = Value::getReferents();

	if (!isHoley()) {
		const auto &holeless = std::get<Holeless>(values);
		out.insert(holeless.begin(), holeless.end());
	} else {
		for (const auto &[key, value]: std::get<Holey>(values))
			out.insert(value);
	}

	return out;
}

Value * Array::copy() const {
	if (isHoley()) {
		const auto &holey = std::get<Holey>(values);
		Holey copies;
		for (const auto &[key, reference]: holey)
			copies[key] = make<Reference>(reference->referent, reference->isConst);
		return context->makeValue<Array>(std::move(copies), holeyLength);
	}

	const auto &holeless = std::get<Holeless>(values);
	Holeless copies;
	for (const auto &reference: holeless)
		// copies.push_back(make<Reference>(reference->referent));
		copies.push_back(reference); // TODO: verify
	return context->makeValue<Array>(std::move(copies));
}

Array * Array::holelessCopy() const {
	INFO("holelessCopy()");

	if (!isHoley())
		return copy()->cast<Array>();

	Holeless new_values;
	const auto &holey = std::get<Holey>(values);
	for (const auto &[key, reference]: holey) {
		while (new_values.size() < key)
			new_values.push_back(context->makeReference<Undefined>());
		new_values.push_back(reference);
	}

	return context->makeValue<Array>(std::move(new_values));
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

	return make<Number>(nan(""));
}

Reference * Array::operator[](size_t index) const {
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

Reference *& Array::fetchOrMake(size_t index) {
	if (isHoley()) {
		auto &holey = std::get<Holey>(values);
		if (auto iter = holey.find(index); iter != holey.end())
			return iter->second;
		holeyLength = std::max(holeyLength, index + 1);
		return holey[index] = make<Reference>(make<Undefined>(), false);
	}

	auto &holeless = std::get<Holeless>(values);
	if (index < holeless.size())
		return holeless.at(index);

	if (index == holeless.size()) {
		holeless.push_back(make<Reference>(make<Undefined>(), false));
		return holeless.back();
	}

	convertToHoley();
	return fetchOrMake(index);
}

Reference * Array::access(Value *index, bool can_create) {
	if (auto *found = access(static_cast<double>(*index)))
		return found;
	return HasMap::access(index, can_create);
}

Reference * Array::access(const std::string &property, bool can_create) {
	try {
		return access(parseDouble(property));
	} catch (const std::invalid_argument &) {
		return HasMap::access(property, can_create);
	}
}

Reference * Array::access(double number) {
	double intpart = 0.;
	if (isFinite(number) && std::modf(number, &intpart) == 0.) {
		const auto index = static_cast<size_t>(intpart);
		if (isHoley()) {
			const auto &holey = std::get<Holey>(values);
			if (auto iter = holey.find(index); iter != holey.end())
				return iter->second;
		} else {
			const auto &holeless = std::get<Holeless>(values);
			if (index < holeless.size())
				return holeless.at(index);
		}
	}

	return nullptr;
}

bool Array::doDelete(Value *property) {
	const auto number = static_cast<double>(*property);
	double intpart = 0.;
	if (isFinite(number) && std::modf(number, &intpart) == 0.) {
		const auto index = static_cast<size_t>(intpart);
		if (isHoley()) {
			auto &holey = std::get<Holey>(values);
			if (auto iter = holey.find(index); iter != holey.end()) {
				holey.erase(iter);
				return true;
			}
		} else {
			auto &holeless = std::get<Holeless>(values);
			if (index < holeless.size()) {
				holeless.at(index)->referent = context->makeValue<Undefined>();
				return true;
			}
		}
	}

	return HasMap::doDelete(property);
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

void Array::push(Value *value) {
	if (isHoley())
		std::get<Holey>(values)[holeyLength++] = context->makeReference(value);
	else
		std::get<Holeless>(values).push_back(context->makeReference(value));
}

Value * Array::pop() {
	if (isHoley()) {
		auto &holey = std::get<Holey>(values);
		assert((holeyLength == 0) == (holey.empty()));
		if (!holey.empty()) {
			auto iter = holey.rbegin().base();
			auto *out = iter->second;
			holey.erase(iter);
			return out;
		}
	} else {
		auto &holeless = std::get<Holeless>(values);
		if (!holeless.empty()) {
			auto *out = holeless.back();
			holeless.pop_back();
			return out;
		}
	}

	return make<Undefined>();
}

void Array::convertToHoley() {
	if (isHoley())
		return;

	Holey holey;
	size_t i = 0;
	for (auto *value: std::get<Holeless>(values))
		holey[i++] = value;
	values = std::move(holey);
}

Value * Array::contains(const Value &other) const {
	const auto string = static_cast<std::string>(other);

	if (map.contains(string))
		return context->toValue(true);

	const auto number = static_cast<double>(other);
	double intpart;
	if (!isFinite(number) || std::modf(number, &intpart) != 0.)
		return context->toValue(false);

	const auto index = static_cast<size_t>(number);

	if (isHoley())
		return context->toValue(std::get<Holey>(values).contains(index));

	return context->toValue(index < size());
}

Object::Object(const Array &array, Context &context_) {
	setContext(context_);

	if (array.isHoley()) {
		const auto &holey = std::get<Array::Holey>(array.values);
		for (const auto &[key, reference]: holey)
			map[std::to_string(key)] = make<Reference>(reference->referent);
	} else {
		const auto &holeless = std::get<Array::Holeless>(array.values);
		size_t i = 0;
		for (const auto &reference: holeless)
			map[std::to_string(i++)] = make<Reference>(reference->referent);
	}
}

Value * Object::copy() const {
	auto *out = new Object;
	for (const auto &[key, reference]: map)
		out->map[key] = make<Reference>(reference->referent);
	out->setContext(*context);
	return out;
}

Number * Object::toNumber() const {
	return make<Number>(nan(""));
}

Object::operator std::string() const {
	std::stringstream ss;
	ss << '{';
	bool first = true;
	for (const auto &[key, value]: map) {
		if (first)
			first = false;
		else
			ss << ", ";
		ss << key << ": " << static_cast<std::string>(*value);
	}
	ss << '}';
	return ss.str();
}

std::unordered_set<Value *> Object::getReferents() const {
	auto out = Value::getReferents();
	out.reserve(map.size() + 1); // + 1 for customPrototype
	for (const auto &[name, value]: map)
		out.insert(value);
	return out;
}

Value * Null::copy() const {
	return context->makeValue<Null>();
}

Number * Null::toNumber() const {
	return make<Number>(0.);
}

Value * Undefined::copy() const {
	return context->makeValue<Undefined>();
}

Number * Undefined::toNumber() const {
	return make<Number>(nan(""));
}

Value * Number::copy() const {
	return context->makeValue<Number>(number);
}

Number * Number::toNumber() const {
	return make<Number>(number);
}

Number::operator std::string() const {
	std::string out = std::to_string(number);
	if (out.find('.') != std::string::npos)
		while (out.back() == '0')
			out.pop_back();
	if (!out.empty() && out.back() == '.')
		out.pop_back();
	return out;
}

Value * Boolean::copy() const {
	return context->makeValue<Boolean>(boolean);
}

Number * Boolean::toNumber() const {
	return make<Number>(boolean? 1. : 0.);
}

Value * String::copy() const {
	return context->makeValue<String>(string);
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
	return make<Number>(static_cast<double>(*this));
}

Value * String::operator+(const Value &other) const {
	switch (other.getType()) {
		case ValueType::String:
			return make<String>(string + dynamic_cast<const String &>(other).string);
		case ValueType::Number:
			return make<String>(string + static_cast<std::string>(other));
		case ValueType::Reference:
			return *this + *dynamic_cast<const Reference &>(other).referent;
		default:
			throw TypeError("Invalid string concatenation");
	}
}

Value * Value::operator+(const Value &other) const {
	if (const auto *string = dynamic_cast<const String *>(&other))
		return make<String>(static_cast<std::string>(*this) + string->string);

	return make<Number>(static_cast<double>(*this) + static_cast<double>(other));
}

Value * Value::operator-(const Value &other) const {
	return make<Number>(static_cast<double>(*this) - static_cast<double>(other));
}

Value * Value::operator*(const Value &other) const {
	return make<Number>(static_cast<double>(*this) * static_cast<double>(other));
}

Value * Value::operator/(const Value &other) const {
	return make<Number>(static_cast<double>(*this) / static_cast<double>(other));
}

Value * Value::operator%(const Value &other) const {
	return make<Number>(std::fmod(static_cast<double>(*this), static_cast<double>(other)));
}

Value * Value::operator&(const Value &other) const {
	return make<Number>(static_cast<size_t>(static_cast<double>(*this)) &
	                           static_cast<size_t>(static_cast<double>(other)));
}

Value * Value::operator|(const Value &other) const {
	return make<Number>(static_cast<size_t>(static_cast<double>(*this)) |
	                           static_cast<size_t>(static_cast<double>(other)));
}

Value * Value::operator^(const Value &other) const {
	return make<Number>(static_cast<size_t>(static_cast<double>(*this)) ^
	                           static_cast<size_t>(static_cast<double>(other)));
}

Value * Value::power(const Value &other) const {
	return make<Number>(std::pow(static_cast<double>(*this), static_cast<double>(other)));
}

Value * Value::operator&&(const Value &other) const {
	return make<Boolean>(static_cast<bool>(*this) && static_cast<bool>(other));
}

Value * Value::operator||(const Value &other) const {
	return make<Boolean>(static_cast<bool>(*this) || static_cast<bool>(other));
}

Value * Value::operator!=(const Value &other) const {
	return make<Boolean>(!(*this == other)->cast<Boolean>()->boolean);
}

Value * Value::operator<(const Value &other) const {
	return make<Boolean>(static_cast<double>(*this) < static_cast<double>(other));
}

Value * Value::operator>(const Value &other) const {
	return make<Boolean>(static_cast<double>(*this) > static_cast<double>(other));
}

Value * Value::operator<=(const Value &other) const {
	return make<Boolean>(static_cast<double>(*this) <= static_cast<double>(other));
}

Value * Value::operator>=(const Value &other) const {
	return make<Boolean>(static_cast<double>(*this) >= static_cast<double>(other));
}

Value * Value::operator<<(const Value &other) const {
	return make<Number>(static_cast<int64_t>(*this) << static_cast<int64_t>(other));
}

Value * Value::shiftRightLogical(const Value &other) const {
	return make<Number>(static_cast<uint64_t>(*this) >> static_cast<uint64_t>(other));
}

Value * Value::shiftRightArithmetic(const Value &other) const {
	return make<Number>(static_cast<int64_t>(*this) >> static_cast<int64_t>(other));
}

Value * Value::contains(const Value &other) const {
	throw TypeError("Cannot use \"in\" operator to search for \"" + static_cast<std::string>(other) + "\" in " +
		static_cast<std::string>(*this));
}

Value * Null::operator==(const Value &right) const {
	if (dynamic_cast<const Null *>(&right))
		return make<Boolean>(true);

	return make<Boolean>(false);
}

Value * Undefined::operator==(const Value &right) const {
	if (dynamic_cast<const Undefined *>(&right))
		return make<Boolean>(true);

	return make<Boolean>(false);
}

Value * Number::operator==(const Value &right) const {
	if (const auto *other_number = dynamic_cast<const Number *>(&right))
		return make<Boolean>(number == other_number->number);

	return make<Boolean>(false);
}

Value * Boolean::operator==(const Value &right) const {
	if (const auto *other_boolean = dynamic_cast<const Boolean *>(&right))
		return make<Boolean>(boolean == other_boolean->boolean);

	return make<Boolean>(false);
}

Value * String::operator==(const Value &right) const {
	if (const auto *other_string = dynamic_cast<const String *>(&right))
		return make<Boolean>(string == other_string->string);

	return make<Boolean>(false);
}

Value * Object::operator==(const Value &right) const {
	return make<Boolean>(this == &right);
}

Value * Array::operator==(const Value &right) const {
	return make<Boolean>(this == &right);
}

ValueType Reference::ultimateType() const {
	assertReferent();
	return referent->ultimateType();
}

std::unordered_set<Value *> Reference::getReferents() const {
	assertReferent();
	auto out = Value::getReferents();
	out.insert(referent);
	return out;
}

Reference * Reference::withContext(ReferenceContext new_context) {
	assertReferent();
	return make<Reference>(this, isConst, std::move(new_context));
}

Reference * Reference::unwrap(ReferenceContext *out_context) {
	if (referent->getType() != ValueType::Reference) {
		if (out_context != nullptr)
			*out_context = referenceContext;
		return this;
	}

	return referent->cast<Reference>()->unwrap(out_context);
}

Reference::operator std::string() const {
	assertReferent();
	return static_cast<std::string>(*referent);
}

Reference::operator double() const {
	assertReferent();
	return static_cast<double>(*referent);
}

Reference::operator bool() const {
	assertReferent();
	return static_cast<bool>(*referent);
}

Value * Reference::operator+(const Value &other) const {
	assertReferent();
	return *referent + other;
}

Value * Reference::operator-(const Value &other) const {
	assertReferent();
	return *referent - other;
}

Value * Reference::operator*(const Value &other) const {
	assertReferent();
	return *referent * other;
}

Value * Reference::operator/(const Value &other) const {
	assertReferent();
	return *referent / other;
}

Value * Reference::operator%(const Value &other) const {
	assertReferent();
	return *referent % other;
}

Value * Reference::operator&(const Value &other) const {
	assertReferent();
	return *referent & other;
}

Value * Reference::operator|(const Value &other) const {
	assertReferent();
	return *referent | other;
}

Value * Reference::operator^(const Value &other) const {
	assertReferent();
	return *referent ^ other;
}

Value * Reference::operator==(const Value &other) const {
	assertReferent();
	return *referent == other;
}

Value * Reference::operator!=(const Value &other) const {
	assertReferent();
	return *referent != other;
}

Value * Reference::operator<(const Value &other) const {
	assertReferent();
	return *referent < other;
}

Value * Reference::operator>(const Value &other) const {
	assertReferent();
	return *referent > other;
}

Value * Reference::operator<=(const Value &other) const {
	assertReferent();
	return *referent <= other;
}

Value * Reference::operator>=(const Value &other) const {
	assertReferent();
	return *referent >= other;
}

Value * Reference::power(const Value &other) const {
	assertReferent();
	return referent->power(other);
}

Value * Reference::operator&&(const Value &other) const {
	assertReferent();
	return *referent && other;
}

Value * Reference::operator||(const Value &other) const {
	assertReferent();
	return *referent || other;
}

Value * Reference::operator<<(const Value &other) const {
	assertReferent();
	return *referent << other;
}

Value * Reference::shiftRightLogical(const Value &other) const {
	assertReferent();
	return referent->shiftRightLogical(other);
}

Value * Reference::shiftRightArithmetic(const Value &other) const {
	assertReferent();
	return referent->shiftRightArithmetic(other);
}

Value * Reference::contains(const Value &other) const {
	assertReferent();
	return referent->contains(other);
}

Function::Function(FunctionType function_, Reference *this_obj, Closure closure_, bool is_property):
	function(std::move(function_)),
	thisObj(this_obj),
	closure(std::move(closure_)),
	isProperty(is_property) {}

Value * Function::copy() const {
	return context->makeValue<Function>(function, thisObj, closure);
}

Number * Function::toNumber() const {
	return make<Number>(nan(""));
}

Value * Function::operator==(const Value &other) const {
	return make<Boolean>(this == &other);
}

std::unordered_set<Value *> Function::getReferents() const {
	auto out = Value::getReferents();
	out.insert(closure.references.begin(), closure.references.end());
	if (thisObj != nullptr)
		out.insert(thisObj);
	return out;
}

Object * Function::getFunctionPrototype() {
	if (prototype == nullptr) {
		prototype = context->makeValue<Object>();
		prototype->map["constructor"] = context->makeReference(this);
	}

	return prototype;
}

Reference * HasMap::access(Value *index, bool can_create) {
	const auto string_index = static_cast<std::string>(*index);

	if (auto iter = map.find(string_index); iter != map.end())
		return iter->second;

	if (can_create)
		return map[string_index] = context->makeReference<Undefined>();

	return nullptr;
}

Reference * HasMap::access(const std::string &property, bool can_create) {
	if (auto iter = map.find(property); iter != map.end())
		return iter->second;

	if (can_create)
		return map[property] = context->makeReference<Undefined>();

	return nullptr;
}

bool HasMap::doDelete(Value *property) {
	return doDelete(static_cast<std::string>(*property));
}

bool HasMap::doDelete(const std::string &property) {
	if (auto iter = map.find(property); iter != map.end()) {
		map.erase(iter);
		return true;
	}

	return false;
}

Value * HasMap::contains(const Value &other) const {
	return context->toValue(map.contains(static_cast<std::string>(other)));
}

std::ostream & operator<<(std::ostream &os, const Value &value) {
	return os << static_cast<std::string>(value);
}
