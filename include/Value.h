#pragma once

#include <map>
#include <string>
#include <vector>

#define VALUE_OPERATOR_OVERRIDES \
	virtual Value * operator+(const Value &)  const override; \
	virtual Value * operator-(const Value &)  const override; \
	virtual Value * operator*(const Value &)  const override; \
	virtual Value * operator/(const Value &)  const override; \
	virtual Value * operator%(const Value &)  const override; \
	virtual Value * operator&(const Value &)  const override; \
	virtual Value * operator|(const Value &)  const override; \
	virtual Value * operator^(const Value &)  const override; \
	virtual Value * operator==(const Value &) const override; \
	virtual Value * operator!=(const Value &) const override; \
	virtual Value * operator<(const Value &)  const override; \
	virtual Value * operator>(const Value &)  const override; \
	virtual Value * operator<=(const Value &) const override; \
	virtual Value * operator>=(const Value &) const override; \
	virtual Value * power(const Value &)      const override; \
	virtual Value * exactly(const Value &)    const override; \
	virtual Value * notExactly(const Value &) const override; \
	virtual Value * operator&&(const Value &) const override; \
	virtual Value * operator||(const Value &) const override;

class Value {
	public:
		Context *context = nullptr;
		virtual ~Value() = default;
		virtual std::vector<Value *> getReferents() const { return {}; }
		virtual operator bool() const { return false; }
		virtual explicit operator std::string() const = 0;
		virtual Value * operator+(const Value &)  const { return badOperator("+");   }
		virtual Value * operator-(const Value &)  const { return badOperator("-");   }
		virtual Value * operator*(const Value &)  const { return badOperator("*");   }
		virtual Value * operator/(const Value &)  const { return badOperator("/");   }
		virtual Value * operator%(const Value &)  const { return badOperator("%");   }
		virtual Value * operator&(const Value &)  const { return badOperator("&");   }
		virtual Value * operator|(const Value &)  const { return badOperator("|");   }
		virtual Value * operator^(const Value &)  const { return badOperator("^");   }
		virtual Value * operator==(const Value &) const { return badOperator("==");  }
		virtual Value * operator!=(const Value &) const { return badOperator("!=");  }
		virtual Value * operator<(const Value &)  const { return badOperator("<");   }
		virtual Value * operator>(const Value &)  const { return badOperator(">");   }
		virtual Value * operator<=(const Value &) const { return badOperator("<=");  }
		virtual Value * operator>=(const Value &) const { return badOperator(">=");  }
		virtual Value * power(const Value &)      const { return badOperator("**");  }
		virtual Value * exactly(const Value &)    const { return badOperator("==="); }
		virtual Value * notExactly(const Value &) const { return badOperator("!=="); }
		virtual Value * operator&&(const Value &) const { return badOperator("&&");  }
		virtual Value * operator||(const Value &) const { return badOperator("||");  }
	
	private:
		Value * badOperator(const std::string &op) const {
			throw std::logic_error("Invalid operator usage: " + op);
		}
};

class Null: public Value {
	public:
		Null() = default;
		~Null() override = default;
		explicit operator std::string() const override { return "null"; }
		VALUE_OPERATOR_OVERRIDES
};

class Undefined: public Value {
	public:
		Undefined() = default;
		~Undefined() override = default;
		explicit operator std::string() const override { return "undefined"; }
		VALUE_OPERATOR_OVERRIDES
};

class Symbol: public Value {
	public:
		std::string name;
		// True iff the symbol was created with Symbol.for.
		bool shared = false;
		explicit operator std::string() const override { return "Symbol(" + name + ')'; }
		operator bool() const override { return true; }
		VALUE_OPERATOR_OVERRIDES
};

class Object: public Value {
	public:
		std::map<std::string, Value *> map;
		std::vector<Value *> getReferents() const override;
		explicit operator std::string() const override { return "[object Object]"; }
		operator bool() const override { return true; }
		VALUE_OPERATOR_OVERRIDES
};

class Array: public Value {
	public:
		std::vector<Value *> values;
		std::map<std::string, Value *> map;
		std::vector<Value *> getReferents() const override;
		Array() = default;
		~Array() override = default;
		explicit operator std::string() const override;
		operator bool() const override { return true; }
		VALUE_OPERATOR_OVERRIDES
};

class Number: public Value {
	public:
		double number;
		Number(double number_): number(number_) {}
		explicit operator std::string() const override { return std::to_string(number); }
		operator bool() const override { return static_cast<bool>(number); }
		VALUE_OPERATOR_OVERRIDES
};

class Boolean: public Value {
	public:
		bool boolean;
		Boolean(double boolean_): boolean(boolean_) {}
		explicit operator std::string() const override { return boolean? "true" : "false"; }
		operator bool() const override { return boolean; }
		VALUE_OPERATOR_OVERRIDES
};

class String: public Value {
	public:
		std::string string;
		String(std::string string_): string(std::move(string_)) {}
		explicit operator std::string() const override { return string; }
		operator bool() const override { return !string.empty(); }
		VALUE_OPERATOR_OVERRIDES
};
