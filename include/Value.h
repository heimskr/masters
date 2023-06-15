#pragma once

#include <cassert>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <unordered_set>
#include <variant>
#include <vector>

class Context;
class Number;
class Object;

#define VALUE_OPERATOR_OVERRIDES \
	virtual Value * operator==(const Value &) const override;

#define VALUE_USING \
	using Value::operator int64_t; \
	using Value::operator uint64_t;

enum class ValueType {Null, Undefined, Object, Array, Number, Boolean, String, Reference, Function};

class Value {
	public:
		Context *context = nullptr;
		std::optional<Object *> customPrototype;
		virtual ~Value() = default;

		virtual Value * deepCopy() const = 0;
		virtual Value * shallowCopy() { return deepCopy(); }

		virtual ValueType getType() const = 0;
		virtual std::string getName() const = 0;
		virtual std::string className() const = 0;
		virtual std::unordered_set<Value *> getReferents() const;
		virtual std::string typeof() const = 0;

		virtual const Value * ultimateValue() const { return this; }
		virtual Value * ultimateValue() { return this; }
		virtual ValueType ultimateType() const { return getType(); }

		virtual bool subscriptable() const { return false; }
		virtual Number * toNumber() const {
			throw std::runtime_error("Cannot convert value of type " + getName() + " to a number");
		}

		virtual Object * getPrototype(Context &) const;

		/** Attempts to access a field of a value. If `can_create` is true and the field doesn't exist, this function
		 *  will create a field in a relevant location, initialize it to undefined and return a reference to the new
		 *  value. Otherwise, if the field doesn't exist, this function will return nullptr.
		 *  Note that for some values like null and undefined, this function will always return nullptr. */
		virtual Reference * access(Value *index, bool can_create);
		virtual Reference * access(const std::string &property, bool can_create);

		virtual bool doDelete(Value *) { return true; }
		virtual bool doDelete(const std::string &) { return true; }

		virtual explicit operator bool() const = 0;
		virtual explicit operator std::string() const = 0;
		virtual explicit operator double() const = 0;
		virtual explicit operator int64_t()  const { return static_cast<int64_t>(static_cast<double>(*this));  }
		virtual explicit operator uint64_t() const { return static_cast<uint64_t>(static_cast<double>(*this)); }

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
		/** This is like the "in" operator, but the other way around. */
		virtual Value * contains(const Value &) const;

		template <typename T>
		T * cast() {
			return dynamic_cast<T *>(this);
		}

		template <typename T>
		const T * cast() const {
			return dynamic_cast<const T *>(this);
		}

		template <typename T, typename... Args>
		T * make(Args &&...args) const;

	protected:
		Value * setContext(Context &context_) {
			assert(&context_);
			context = &context_;
			return this;
		}

	private:
		Value * badOperator(const std::string &op) const {
			throw std::logic_error("Invalid operator usage: " + op);
		}
};

class Null: public Value {
	public:
		Null() = default;
		Value * deepCopy() const override;
		std::string className() const override { return "Null"; }
		ValueType getType() const override { return ValueType::Null; };
		Number * toNumber() const override;
		std::string getName() const override { return "Null"; }
		std::string typeof() const override { return "object"; /* For some reason. */ }
		explicit operator std::string() const override { return "null"; }
		explicit operator double() const override { return 0; }
		explicit operator bool() const override { return false; }
		VALUE_OPERATOR_OVERRIDES
		VALUE_USING
};

class Undefined: public Value {
	public:
		Undefined() = default;
		Value * deepCopy() const override;
		std::string className() const override { return "Undefined"; }
		ValueType getType() const override { return ValueType::Undefined; }
		Number * toNumber() const override;
		std::string getName() const override { return "Undefined"; }
		std::string typeof() const override { return "undefined"; }
		explicit operator std::string() const override { return "undefined"; }
		explicit operator double() const override { return nan(""); }
		explicit operator bool() const override { return false; }
		VALUE_OPERATOR_OVERRIDES
		VALUE_USING
};

class HasMap: public Value {
	public:
		std::map<std::string, Reference *> map;
		Reference * access(Value *index, bool can_create) override;
		Reference * access(const std::string &property, bool can_create) override;
		bool doDelete(Value *) override;
		bool doDelete(const std::string &) override;
		Value * contains(const Value &) const override;

	protected:
		HasMap() = default;
};

class Array: public HasMap {
	public:
		using Holeless = std::deque<Reference *>;
		using Holey    = std::map<size_t, Reference *>;
		using Values   = std::variant<Holeless, Holey>;

		Values values;
		size_t holeyLength = -1;

		Array(): values(Holeless{}) {}
		Array(Holeless values_): values(std::move(values_)) {}
		Array(Holey values_, size_t holey_length): values(std::move(values_)), holeyLength(holey_length) {}
		Value * deepCopy() const override;
		Value * shallowCopy() override;
		/** Doesn't copy (returns self) if this array is already holeless. */
		Array * holelessCopy();
		std::string className() const override { return "Array"; }
		ValueType getType() const override { return ValueType::Array; }
		std::unordered_set<Value *> getReferents() const override;
		Number * toNumber() const override;
		std::string getName() const override { return "Array"; }
		std::string typeof() const override { return "object"; }
		bool subscriptable() const override { return true; }
		explicit operator std::string() const override;
		explicit operator double() const override;
		explicit operator bool() const override { return true; }
		inline bool isHoley() const { return std::holds_alternative<Holey>(values); }
		/** Will return nullptr for out-of-range accesses. */
		Reference * operator[](size_t) const;
		/** Will add and return an Undefined value for out-of-range accesses. */
		Reference *& fetchOrMake(size_t);
		Reference * access(Value *index, bool can_create) override;
		Reference * access(const std::string &property, bool can_create) override;
		Reference * access(double);
		bool doDelete(Value *) override;
		/** Doesn't include holes. */
		size_t size() const;
		/** Includes holes. */
		size_t effectiveSize() const;
		bool empty() const;
		void push(Value *);
		void pushEmpty(size_t count);
		Value * pop(size_t count = 1);
		VALUE_OPERATOR_OVERRIDES
		VALUE_USING
		Value * contains(const Value &) const override;

	private:
		void convertToHoley();
};

class Object: public HasMap {
	public:
		Object() = default;
		Object(const Array &, Context &);
		Value * deepCopy() const override;
		Value * shallowCopy() override;
		std::string className() const override { return "Object"; }
		ValueType getType() const override { return ValueType::Object; }
		std::unordered_set<Value *> getReferents() const override;
		Number * toNumber() const override;
		std::string getName() const override { return "Object"; }
		std::string typeof() const override { return "object"; }
		bool subscriptable() const override { return true; }
		explicit operator std::string() const override;
		explicit operator double() const override { return nan(""); }
		explicit operator bool() const override { return true; }
		inline Reference *& operator[](const std::string &key) { return map[key]; }
		VALUE_OPERATOR_OVERRIDES
		VALUE_USING
};

class Number: public HasMap {
	public:
		double number;
		Number(double number_): number(number_) {}
		Value * deepCopy() const override;
		std::string className() const override { return "Number"; }
		ValueType getType() const override { return ValueType::Number; }
		Number * toNumber() const override;
		std::string getName() const override { return "Number"; }
		std::string typeof() const override { return "number"; }
		explicit operator std::string() const override;
		explicit operator double() const override { return number; }
		explicit operator bool() const override { return static_cast<bool>(number); }
		VALUE_OPERATOR_OVERRIDES
		VALUE_USING
};

class Boolean: public HasMap {
	public:
		bool boolean;
		Boolean(double boolean_): boolean(boolean_) {}
		Value * deepCopy() const override;
		std::string className() const override { return "Boolean"; }
		ValueType getType() const override { return ValueType::Boolean; }
		Number * toNumber() const override;
		std::string getName() const override { return "Boolean"; }
		std::string typeof() const override { return "boolean"; }
		explicit operator std::string() const override { return boolean? "true" : "false"; }
		explicit operator double() const override { return boolean? 1. : 0.; }
		explicit operator bool() const override { return boolean; }
		VALUE_OPERATOR_OVERRIDES
		VALUE_USING
};

class String: public HasMap {
	public:
		std::string string;
		String(std::string string_): string(std::move(string_)) {}
		Value * deepCopy() const override;
		std::string className() const override { return "String"; }
		ValueType getType() const override { return ValueType::String; }
		Number * toNumber() const override;
		std::string getName() const override { return "String"; }
		std::string typeof() const override { return "string"; }
		bool subscriptable() const override { return true; }
		explicit operator std::string() const override { return string; }
		explicit operator double() const override;
		explicit operator bool() const override { return !string.empty(); }
		VALUE_OPERATOR_OVERRIDES
		Value * operator+(const Value &) const override;
		VALUE_USING
};

struct ReferenceContext {
	Reference *thisObj = nullptr;
};

class Reference: public Value {
	public:
		Value *referent;
		bool isConst;
		ReferenceContext referenceContext {};

		Reference() = delete;
		Reference(Value *referent_, bool is_const = false): referent(referent_), isConst(is_const) {}

		template <typename... Args>
		Reference(Value *referent_, bool is_const, Args &&...args):
			referent(referent_), isConst(is_const), referenceContext{std::forward<Args>(args)...} {}

		/** Note: this creates a copy of the referred-to value, not of the reference! */
		Value * deepCopy() const override { assertReferent(); return referent->deepCopy(); }
		Value * shallowCopy() override { assertReferent(); return referent->shallowCopy(); }
		std::string className() const override { return referent->className(); }
		ValueType getType() const override { return ValueType::Reference; }
		const Value * ultimateValue() const override { assertReferent(); return referent->ultimateValue(); }
		Value * ultimateValue() override { assertReferent(); return referent->ultimateValue(); }
		ValueType ultimateType() const override;
		std::string typeof() const override { assertReferent(); return referent->typeof(); }
		void assertReferent() const { assert(referent != nullptr); }
		std::unordered_set<Value *> getReferents() const override;
		Number * toNumber() const override { assertReferent(); return referent->toNumber(); }
		std::string getName() const override { assertReferent(); return "Reference[" + referent->getName() + ']'; }
		bool subscriptable() const override { assertReferent(); return referent->subscriptable(); }
		Reference * access(Value *index, bool can_create) override { return referent->access(index, can_create); }
		bool doDelete(Value *value) override { return referent->doDelete(value); }
		bool doDelete(const std::string &property) override { return referent->doDelete(property); }

		Reference * withContext(ReferenceContext);
		Reference * unwrap(ReferenceContext * = nullptr);

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
		Value * contains(const Value &) const override;
};

class Function: public HasMap {
	public:
		using FunctionType = std::function<Value *(Context &, const std::vector<Value *> &args, Reference *this_obj)>;
		FunctionType function;
		Reference *thisObj = nullptr;
		Closure closure;
		Object *prototype = nullptr;
		bool isProperty = false;
		Function(FunctionType function_ = {}, Reference *this_obj = nullptr, Closure closure_ = {},
		         bool is_property = false);
		/** This doesn't actually deep-clone thisObj. */
		Value * deepCopy() const override;
		std::string className() const override { return "Function"; }
		std::unordered_set<Value *> getReferents() const override;
		ValueType getType() const override { return ValueType::Function; }
		Number * toNumber() const override;
		std::string getName() const override { return "Function"; }
		std::string typeof() const override { return "function"; }
		Object * getFunctionPrototype();
		explicit operator std::string() const override { return "function(...) {...}"; }
		explicit operator double()      const override { return nan("");               }
		explicit operator bool()        const override { return true;                  }
		VALUE_OPERATOR_OVERRIDES
		VALUE_USING
};

std::ostream & operator<<(std::ostream &, const Value &);
