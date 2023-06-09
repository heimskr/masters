#pragma once

#include <map>
#include <memory>
#include <string>
#include <unordered_set>
#include <variant>
#include <vector>

#include "Node.h"
#include "Utils.h"
#include "Value.h"

class Scope {
	private:
		ssize_t depth = -1;

	public:
		std::map<std::string, std::pair<Value *, bool>> store;

		Scope() = delete;
		Scope(ssize_t depth_): depth(depth_) {}

		inline ssize_t getDepth() const { return depth; }
};

class ScopeStack {
	private:
		std::vector<Scope> scopes;

	public:
		std::unordered_map<std::string, Value *> globals;

		Value ** lookup(const std::string &name, bool *const_out = nullptr, ssize_t *depth_out = nullptr);
		void insert(const std::string &name, Value *value, bool is_const = false);
		inline void push() { scopes.emplace_back(static_cast<ssize_t>(scopes.size())); }
		inline void pop()  { scopes.pop_back(); }
		bool inLastScope(const std::string &) const;

		friend class Context;
};

class Context {
	public:
		std::unordered_set<Value *> valuePool;
		ScopeStack stack;
		size_t lineNumber = 0;
		size_t columnNumber = 0;
		bool writingMember = false;
		Value *nextThis = nullptr;

		Context() = default;

		void interpret();
		void garbageCollect();

		template <typename T, typename... Args>
		T * makeValue(Args &&...args) {
			auto *new_value = new T(std::forward<Args>(args)...);
			new_value->context = this;
			valuePool.insert(new_value);
			return new_value;
		}

		inline auto writeMember() {
			return FieldSaver(*this, &Context::writingMember);
		}

		inline void addGlobal(const std::string &name, Value *value) {
			if (stack.globals.contains(name))
				throw std::runtime_error("Context already contains global \"" + name + '"');

			stack.globals.emplace(name, value);
			valuePool.insert(value);
		}

		template <typename T, typename... Args>
		inline void makeGlobal(const std::string &name, Args &&...args) {
			if (stack.globals.contains(name))
				throw std::runtime_error("Context already contains global \"" + name + '"');

			Value *value = makeValue<T>(std::forward<Args>(args)...);
			stack.globals.try_emplace(name, value);
			valuePool.insert(value);
		}
};
