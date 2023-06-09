#pragma once

#include <cassert>
#include <cmath>
#include <functional>
#include <map>
#include <string>
#include <unordered_set>
#include <variant>
#include <vector>

class Context;
class Number;

#define VALUE_OPERATOR_OVERRIDES \
	virtual Value * operator==(const Value &) const override;

#define VALUE_USING \
	using Value::operator int64_t; \
	using Value::operator uint64_t;

enum class ValueType {Null, Undefined, Object, Array, Number, Boolean, String, Reference, Function};

class Value {
	public:
		Context *context = nullptr;
		virtual ~Value() = default;
		virtual Value * copy() const = 0;
		virtual ValueType getType() const = 0;
		virtual const Value * ultimateValue() const { return this; }
		virtual Value * ultimateValue() { return this; }
		virtual ValueType ultimateType() const { return getType(); }
		virtual std::unordered_set<Value *> getReferents() const { return {}; }
		virtual Number * toNumber() const {
			throw std::runtime_error("Cannot convert value of type " + getName() + " to a number");
		}
		virtual explicit operator bool() const = 0;
		virtual explicit operator std::string() const = 0;
		virtual explicit operator double() const = 0;
		virtual explicit operator int64_t()  const { return static_cast<int64_t>(static_cast<double>(*this));  }
		virtual explicit operator uint64_t() const { return static_cast<uint64_t>(static_cast<double>(*this)); }
		virtual std::string getName() const = 0;
		virtual bool subscriptable() const { return false; }
		virtual Value * operator+(const Value &)  const;
		virtual Value * operator-(const Value &)  const;
		virtual Value * operator*(const Value &)  const;
		virtual Value * operator/(const Value &)  const;
		virtual Value * operator%(const Value &)  const;
		virtual Value * operator&(const Value &)  const;
		virtual Value * operator|(const Value &)  const;
		virtual Value * operator^(const Value &)  const;
		/** For strict equality. */
		virtual Value * operator==(const Value &) const { return badOperator("==="); }
		/** For strict inequality. */
		virtual Value * operator!=(const Value &) const;
		virtual Value * operator<(const Value &)  const;
		virtual Value * operator>(const Value &)  const;
		virtual Value * operator<=(const Value &) const;
		virtual Value * operator>=(const Value &) const;
		virtual Value * power(const Value &)      const;
		virtual Value * operator&&(const Value &) const;
		virtual Value * operator||(const Value &) const;
		virtual Value * operator<<(const Value &) const;
		virtual Value * shiftRightLogical(const Value &) const;
		virtual Value * shiftRightArithmetic(const Value &) const;

	private:
		Value * badOperator(const std::string &op) const {
			throw std::logic_error("Invalid operator usage: " + op);
		}
};

class Null: public Value {
	public:
		Null() = default;
		Value * copy() const override { return new Null; }
		ValueType getType() const override { return ValueType::Null; };
		Number * toNumber() const override;
		std::string getName() const override { return "Null"; }
		explicit operator std::string() const override { return "null"; }
		explicit operator double() const override { return 0; }
		explicit operator bool() const override { return false; }
		VALUE_OPERATOR_OVERRIDES
		VALUE_USING
};

class Undefined: public Value {
	public:
		Undefined() = default;
		Value * copy() const override { return new Undefined; }
		ValueType getType() const override { return ValueType::Undefined; }
		Number * toNumber() const override;
		std::string getName() const override { return "Undefined"; }
		explicit operator std::string() const override { return "undefined"; }
		explicit operator double() const override { return nan(""); }
		explicit operator bool() const override { return false; }
		VALUE_OPERATOR_OVERRIDES
		VALUE_USING
};

class Array: public Value {
	public:
		using Holeless = std::vector<Value *>;
		using Holey    = std::map<size_t, Value *>;
		using Values   = std::variant<Holeless, Holey>;

		Values values;
		size_t holeyLength = -1;

		Array(): values(Holeless{}) {}
		Array(Holeless values_): values(std::move(values_)) {}
		Array(Holey values_, size_t holey_length): values(std::move(values_)), holeyLength(holey_length) {}
		Value * copy() const override;
		ValueType getType() const override { return ValueType::Array; }
		std::unordered_set<Value *> getReferents() const override;
		Number * toNumber() const override;
		std::string getName() const override { return "Array"; }
		bool subscriptable() const override { return true; }
		explicit operator std::string() const override;
		explicit operator double() const override;
		explicit operator bool() const override { return true; }
		inline bool isHoley() const { return std::holds_alternative<Holey>(values); }
		/** Will return nullptr for out-of-range accesses. */
		Value * operator[](size_t) const;
		/** Will add and return an Undefined value for out-of-range accesses. */
		Value *& fetchOrMake(size_t);
		size_t size() const;
		bool empty() const;
		VALUE_OPERATOR_OVERRIDES
		VALUE_USING

	private:
		void convertToHoley();
};

class Object: public Value {
	public:
		std::map<std::string, Value *> map;
		Object() = default;
		Object(const Array &);
		Value * copy() const override;
		ValueType getType() const override { return ValueType::Object; }
		std::unordered_set<Value *> getReferents() const override;
		Number * toNumber() const override;
		std::string getName() const override { return "Object"; }
		bool subscriptable() const override { return true; }
		explicit operator std::string() const override { return "[object Object]"; }
		explicit operator double() const override { return nan(""); }
		explicit operator bool() const override { return true; }
		VALUE_OPERATOR_OVERRIDES
		VALUE_USING
};

class Number: public Value {
	public:
		double number;
		Number(double number_): number(number_) {}
		Value * copy() const override { return new Number(number); }
		ValueType getType() const override { return ValueType::Number; }
		Number * toNumber() const override;
		std::string getName() const override { return "Number"; }
		explicit operator std::string() const override;
		explicit operator double() const override { return number; }
		explicit operator bool() const override { return static_cast<bool>(number); }
		VALUE_OPERATOR_OVERRIDES
		VALUE_USING
};

class Boolean: public Value {
	public:
		bool boolean;
		Boolean(double boolean_): boolean(boolean_) {}
		Value * copy() const override { return new Boolean(boolean); }
		ValueType getType() const override { return ValueType::Boolean; }
		Number * toNumber() const override;
		std::string getName() const override { return "Boolean"; }
		explicit operator std::string() const override { return boolean? "true" : "false"; }
		explicit operator double() const override { return boolean? 1. : 0.; }
		explicit operator bool() const override { return boolean; }
		VALUE_OPERATOR_OVERRIDES
		VALUE_USING
};

class String: public Value {
	public:
		std::string string;
		String(std::string string_): string(std::move(string_)) {}
		Value * copy() const override { return new String(string); }
		ValueType getType() const override { return ValueType::String; }
		Number * toNumber() const override;
		std::string getName() const override { return "String"; }
		bool subscriptable() const override { return true; }
		explicit operator std::string() const override { return string; }
		explicit operator double() const override;
		explicit operator bool() const override { return !string.empty(); }
		VALUE_OPERATOR_OVERRIDES
		Value * operator+(const Value &) const override;
		VALUE_USING
};

class Reference: public Value {
	public:
		Value **referent;
		bool isConst;
		Reference() = delete;
		Reference(Value **referent_, bool is_const = false): referent(referent_), isConst(is_const) {}
		/** Note: this creates a copy of the referred-to value, not of the reference! */
		Value * copy() const override { assertReferent(); return (*referent)->copy(); }
		ValueType getType() const override { return ValueType::Reference; }
		const Value * ultimateValue() const override { assertReferent(); return (*referent)->ultimateValue(); }
		Value * ultimateValue() override { assertReferent(); return (*referent)->ultimateValue(); }
		ValueType ultimateType() const override;
		void assertReferent() const { assert(referent != nullptr); assert(*referent != nullptr); }
		std::unordered_set<Value *> getReferents() const override;
		Number * toNumber() const override { assertReferent(); return (*referent)->toNumber(); }
		std::string getName() const override { assertReferent(); return "Reference[" + (*referent)->getName() + ']'; }
		bool subscriptable() const override { assertReferent(); return (*referent)->subscriptable(); }
		explicit operator std::string() const override;
		explicit operator double()      const override;
		explicit operator bool()        const override;
		// Alas.
		Value * operator+(const Value &)  const override;
		Value * operator-(const Value &)  const override;
		Value * operator*(const Value &)  const override;
		Value * operator/(const Value &)  const override;
		Value * operator%(const Value &)  const override;
		Value * operator&(const Value &)  const override;
		Value * operator|(const Value &)  const override;
		Value * operator^(const Value &)  const override;
		Value * operator==(const Value &) const override;
		Value * operator!=(const Value &) const override;
		Value * operator<(const Value &)  const override;
		Value * operator>(const Value &)  const override;
		Value * operator<=(const Value &) const override;
		Value * operator>=(const Value &) const override;
		Value * power(const Value &)      const override;
		Value * operator&&(const Value &) const override;
		Value * operator||(const Value &) const override;
		Value * operator<<(const Value &) const override;
		Value * shiftRightLogical(const Value &) const override;
		Value * shiftRightArithmetic(const Value &) const override;
		VALUE_USING
};

class Function: public Value {
	public:
		using FunctionType = std::function<Value *(Context &, const std::vector<Value *> &arguments, Value *this_obj)>;
		FunctionType function;
		Value *thisObj = nullptr;
		std::unordered_set<Value *> closure;
		Function(FunctionType function_ = {}, Value *this_obj = nullptr, std::unordered_set<Value *> closure_ = {});
		/** This doesn't deep-clone thisObj. */
		Value * copy() const override { return new Function(function, thisObj, closure); }
		std::unordered_set<Value *> getReferents() const override;
		ValueType getType() const override { return ValueType::Function; }
		Number * toNumber() const override;
		std::string getName() const override { return "Function"; }
		explicit operator std::string() const override { return "function(...) {...}"; }
		explicit operator double()      const override { return nan("");               }
		explicit operator bool()        const override { return true;                  }
		VALUE_OPERATOR_OVERRIDES
		VALUE_USING
};
