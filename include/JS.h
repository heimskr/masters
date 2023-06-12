#pragma once

#include <map>
#include <memory>
#include <string>
#include <unordered_set>
#include <variant>
#include <vector>

#include "Concepts.h"
#include "Node.h"
#include "Utils.h"
#include "Value.h"

struct Closure;

class Scope {
	private:
		ssize_t depth = -1;

	public:
		std::map<std::string, Reference *> store;

		Scope() = delete;
		Scope(ssize_t depth_): depth(depth_) {}

		inline ssize_t getDepth() const { return depth; }
};

class ScopeStack {
	private:
		std::vector<Scope> scopes;

	public:
		std::unordered_map<std::string, Reference *> globals;
		std::vector<Closure *> closures;

		Reference * lookup(const std::string &name, ssize_t *depth_out = nullptr);
		void insert(const std::string &name, Value *value, bool is_const = false);
		inline void push() { scopes.emplace_back(static_cast<ssize_t>(scopes.size())); }
		inline void pop()  { scopes.pop_back(); }
		bool inLastScope(const std::string &) const;

		friend class Context;
};

class Globals {
	private:
		std::unordered_set<Value *> values;

	public:
		Globals() = default;

		inline void insert(Value *value) {
			values.insert(value);
		}

		bool contains(Value *) const;

		inline auto size()  const { return values.size();  }
		inline auto empty() const { return values.empty(); }
		inline auto begin() const { return values.begin(); }
		inline auto end()   const { return values.end();   }
};

struct PrototypeMethod {
	Function::FunctionType function;
	bool isProperty;
	PrototypeMethod(Function::FunctionType function_, bool is_property = false):
		function(std::move(function_)), isProperty(is_property) {}
};

class Context {
	public:
		std::unordered_set<Value *> valuePool;
		ScopeStack stack;
		Globals globals;
		size_t lineNumber = 0;
		size_t columnNumber = 0;
		bool writingMember = false;
		Reference *nextThis = nullptr;
		Function *currentFunction = nullptr;

		Context() = default;

		void addDefaults();
		void garbageCollect();
		Reference * makePrototype(const std::unordered_map<std::string, PrototypeMethod> &);

		template <typename T, typename... Args>
		T * makeValue(Args &&...args) {
			auto *new_value = new T(std::forward<Args>(args)...);
			new_value->context = this;
			valuePool.insert(new_value);
			return new_value;
		}

		template <typename T, typename... Args>
		Reference * makeReference(Args &&...args) {
			return makeValue<Reference>(makeValue<T>(std::forward<Args>(args)...), false);
		}

		Reference * makeReference(Value *value) {
			return makeValue<Reference>(value);
		}

		inline auto writeMember(bool new_value = true) {
			const bool was_writing = writingMember;
			writingMember = new_value;
			return FieldSaver<Context, bool>(*this, &Context::writingMember, was_writing);
		}

		inline void addGlobal(const std::string &name, Value *value) {
			if (stack.globals.contains(name))
				throw std::runtime_error("Context already contains global \"" + name + '"');

			auto *reference = makeReference(value);
			stack.globals.emplace(name, reference);
			globals.insert(value);
			valuePool.insert(value);
		}

		template <typename T, typename... Args>
		inline T * makeGlobal(const std::string &name, Args &&...args) {
			T *value = makeValue<T>(std::forward<Args>(args)...);
			addGlobal(name, value);
			return value;
		}

		inline String * toValue(std::string string) {
			return makeValue<String>(std::move(string));
		}

		inline String * toValue(const char *string) {
			return makeValue<String>(std::string(string));
		}

		template <Numeric N>
		inline Number * toValue(N number) {
			return makeValue<Number>(static_cast<double>(number));
		}

		inline Boolean * toValue(bool boolean) {
			return makeValue<Boolean>(boolean);
		}

		inline Function * toValue(Function::FunctionType fn, Reference *this_obj = nullptr, Closure closure = {}) {
			return makeValue<Function>(std::move(fn), this_obj, std::move(closure));
		}
};

struct ClosureGuard {
	Context &context;

	ClosureGuard(Context &context_, Closure &closure): context(context_) {
		context.stack.closures.push_back(&closure);
	}

	~ClosureGuard() {
		context.stack.closures.pop_back();
	}
};
