#pragma once

#include <cassert>
#include <cmath>
#include <functional>
#include <map>
#include <string>
#include <vector>

class Context;
class Number;

#define VALUE_OPERATOR_OVERRIDES \
	virtual Value * operator==(const Value &) const override;

enum class ValueType {Null, Undefined, Object, Array, Number, Boolean, String, Reference, Function};

class Value {
	public:
		Context *context = nullptr;
		virtual ~Value() = default;
		virtual ValueType getType() const = 0;
		virtual std::vector<Value *> getReferents() const { return {}; }
		virtual Number * toNumber() const {
			throw std::runtime_error("Cannot convert value of type " + getName() + " to a number");
		}
		virtual explicit operator bool() const = 0;
		virtual explicit operator std::string() const = 0;
		virtual explicit operator double() const = 0;
		virtual explicit operator int64_t()  const { return static_cast<int64_t>(static_cast<double>(*this));  }
		virtual explicit operator uint64_t() const { return static_cast<uint64_t>(static_cast<double>(*this)); }
		virtual std::string getName() const = 0;
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
		ValueType getType() const override { return ValueType::Null; };
		Number * toNumber() const override;
		std::string getName() const override { return "Null"; }
		explicit operator std::string() const override { return "null"; }
		explicit operator double() const override { return 0; }
		explicit operator bool() const override { return false; }
		VALUE_OPERATOR_OVERRIDES
};

class Undefined: public Value {
	public:
		Undefined() = default;
		ValueType getType() const override { return ValueType::Undefined; }
		Number * toNumber() const override;
		std::string getName() const override { return "Undefined"; }
		explicit operator std::string() const override { return "undefined"; }
		explicit operator double() const override { return nan(""); }
		explicit operator bool() const override { return false; }
		VALUE_OPERATOR_OVERRIDES
};

class Object: public Value {
	public:
		std::map<std::string, Value *> map;
		Object() = default;
		ValueType getType() const override { return ValueType::Object; }
		std::vector<Value *> getReferents() const override;
		Number * toNumber() const override;
		std::string getName() const override { return "Object"; }
		explicit operator std::string() const override { return "[object Object]"; }
		explicit operator double() const override { return nan(""); }
		explicit operator bool() const override { return true; }
		VALUE_OPERATOR_OVERRIDES
};

class Array: public Value {
	public:
		std::vector<Value *> values;
		// std::map<std::string, Value *> map;
		Array() = default;
		Array(std::vector<Value *> values_): values(std::move(values_)) {}
		ValueType getType() const override { return ValueType::Array; }
		std::vector<Value *> getReferents() const override { return values; }
		Number * toNumber() const override;
		std::string getName() const override { return "Array"; }
		explicit operator std::string() const override;
		explicit operator double() const override;
		explicit operator bool() const override { return true; }
		VALUE_OPERATOR_OVERRIDES
};

class Number: public Value {
	public:
		double number;
		Number(double number_): number(number_) {}
		ValueType getType() const override { return ValueType::Number; }
		Number * toNumber() const override;
		std::string getName() const override { return "Number"; }
		explicit operator std::string() const override { return std::to_string(number); }
		explicit operator double() const override { return number; }
		explicit operator bool() const override { return static_cast<bool>(number); }
		VALUE_OPERATOR_OVERRIDES
};

class Boolean: public Value {
	public:
		bool boolean;
		Boolean(double boolean_): boolean(boolean_) {}
		ValueType getType() const override { return ValueType::Boolean; }
		Number * toNumber() const override;
		std::string getName() const override { return "Boolean"; }
		explicit operator std::string() const override { return boolean? "true" : "false"; }
		explicit operator double() const override { return boolean? 1. : 0.; }
		explicit operator bool() const override { return boolean; }
		VALUE_OPERATOR_OVERRIDES
};

class String: public Value {
	public:
		std::string string;
		ValueType getType() const override { return ValueType::String; }
		String(std::string string_): string(std::move(string_)) {}
		Number * toNumber() const override;
		std::string getName() const override { return "String"; }
		explicit operator std::string() const override { return string; }
		explicit operator double() const override;
		explicit operator bool() const override { return !string.empty(); }
		VALUE_OPERATOR_OVERRIDES
		Value * operator+(const Value &) const override;
};

class Reference: public Value {
	public:
		Value *referent;
		ValueType getType() const override { return ValueType::Reference; }
		Reference(Value *referent_): referent(referent_) {}
		std::vector<Value *> getReferents() const override { assert(referent); return {referent}; }
		Number * toNumber() const override { assert(referent); return referent->toNumber(); }
		std::string getName() const override { return "Reference[" + referent->getName() + ']'; }
		explicit operator std::string() const override { assert(referent); return static_cast<std::string>(*referent); }
		explicit operator double()      const override { assert(referent); return static_cast<double>(*referent);      }
		explicit operator bool()        const override { assert(referent); return static_cast<bool>(*referent);        }
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
};

class Function: public Value {
	public:
		using FunctionType = std::function<Value *(Context &, const std::vector<Value *> &arguments, Value *this_obj)>;
		FunctionType function;
		Value *thisObj = nullptr;
		Function(FunctionType function_ = {}, Value *this_obj = nullptr);
		std::vector<Value *> getReferents() const override { if (thisObj != nullptr) return {thisObj}; return {}; }
		ValueType getType() const override { return ValueType::Function; }
		Number * toNumber() const override;
		std::string getName() const override { return "Function"; }
		explicit operator std::string() const override { return "function(...) {...}"; }
		explicit operator double()      const override { return nan("");               }
		explicit operator bool()        const override { return true;                  }
		VALUE_OPERATOR_OVERRIDES
};
