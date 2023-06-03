#pragma once

#include <cmath>
#include <map>
#include <string>
#include <vector>

class Number;

#define VALUE_OPERATOR_OVERRIDES \
	virtual Value * operator==(const Value &) const override;

enum class ValueType {Null, Undefined, Object, Array, Number, Boolean, String};

class Value {
	public:
		Context *context = nullptr;
		virtual ~Value() = default;
		virtual ValueType getType() const = 0;
		virtual std::vector<Value *> getReferents() const { return {}; }
		virtual Number * toNumber() const {
			throw std::runtime_error("Cannot convert value of type " + getName() + " to a number");
		}
		virtual operator bool() const { return false; }
		virtual explicit operator std::string() const = 0;
		virtual explicit operator double() const = 0;
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
		explicit operator std::string() const override { return "null"; }
		explicit operator double() const override { return 0; }
		std::string getName() const override { return "Null"; }
		VALUE_OPERATOR_OVERRIDES
};

class Undefined: public Value {
	public:
		Undefined() = default;
		ValueType getType() const override { return ValueType::Undefined; }
		Number * toNumber() const override;
		explicit operator std::string() const override { return "undefined"; }
		explicit operator double() const override { return nan(""); }
		std::string getName() const override { return "Undefined"; }
		VALUE_OPERATOR_OVERRIDES
};

class Object: public Value {
	public:
		std::map<std::string, Value *> map;
		Object() = default;
		ValueType getType() const override { return ValueType::Object; }
		std::vector<Value *> getReferents() const override;
		Number * toNumber() const override;
		explicit operator std::string() const override { return "[object Object]"; }
		explicit operator double() const override { return nan(""); }
		std::string getName() const override { return "Object"; }
		operator bool() const override { return true; }
		VALUE_OPERATOR_OVERRIDES
};

class Array: public Value {
	public:
		std::vector<Value *> values;
		// std::map<std::string, Value *> map;
		Array() = default;
		ValueType getType() const override { return ValueType::Array; }
		std::vector<Value *> getReferents() const override { return values; }
		Number * toNumber() const override;
		explicit operator std::string() const override;
		explicit operator double() const override;
		std::string getName() const override { return "Array"; }
		operator bool() const override { return true; }
		VALUE_OPERATOR_OVERRIDES
};

class Number: public Value {
	public:
		double number;
		Number(double number_): number(number_) {}
		ValueType getType() const override { return ValueType::Number; }
		Number * toNumber() const override;
		explicit operator std::string() const override { return std::to_string(number); }
		explicit operator double() const override { return number; }
		std::string getName() const override { return "Number"; }
		operator bool() const override { return static_cast<bool>(number); }
		VALUE_OPERATOR_OVERRIDES
};

class Boolean: public Value {
	public:
		bool boolean;
		Boolean(double boolean_): boolean(boolean_) {}
		ValueType getType() const override { return ValueType::Boolean; }
		Number * toNumber() const override;
		explicit operator std::string() const override { return boolean? "true" : "false"; }
		explicit operator double() const override { return boolean? 1. : 0.; }
		std::string getName() const override { return "Boolean"; }
		operator bool() const override { return boolean; }
		VALUE_OPERATOR_OVERRIDES
};

class String: public Value {
	public:
		std::string string;
		ValueType getType() const override { return ValueType::String; }
		String(std::string string_): string(std::move(string_)) {}
		Number * toNumber() const override;
		explicit operator std::string() const override { return string; }
		explicit operator double() const override;
		std::string getName() const override { return "String"; }
		operator bool() const override { return !string.empty(); }
		VALUE_OPERATOR_OVERRIDES
		// TODO: operator+
};
