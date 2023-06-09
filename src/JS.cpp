#include <functional>
#include <iostream>

#include "JS.h"
#include "Utils.h"

Value ** ScopeStack::lookup(const std::string &name, bool *const_out, ssize_t *depth_out) {
	ssize_t depth = scopes.size();
	for (Scope &scope: reverse(scopes)) {
		--depth;
		if (auto iter = scope.store.find(name); iter != scope.store.end()) {
			if (depth_out != nullptr)
				*depth_out = depth;
			auto &[value, is_const] = iter->second;
			if (const_out != nullptr)
				*const_out = is_const;
			return &value;
		}
	}

	if (auto iter = globals.find(name); iter != globals.end()) {
		if (depth_out != nullptr)
			*depth_out = 0;
		if (const_out != nullptr)
			*const_out = false;
		return &iter->second;
	}

	if (depth_out != nullptr)
		*depth_out = -1;

	return nullptr;
}

void ScopeStack::insert(const std::string &name, Value *value, bool is_const) {
	if (scopes.empty())
		throw std::runtime_error("Cannot insert into ScopeStack: no scopes present");

	scopes.back().store.try_emplace(name, value, is_const);
}

bool ScopeStack::inLastScope(const std::string &name) const {
	return !scopes.empty() && scopes.back().store.contains(name);
}

void Context::addDefaults() {
	makeGlobal<Object>("this");

	makeGlobal<Function>("print", [](Context &context, const std::vector<Value *> &arguments, Value *) {
		for (Value *value: arguments)
			std::cout << static_cast<std::string>(*value) << std::endl;
		return context.makeValue<Undefined>();
	});
}

void Context::garbageCollect() {
	std::unordered_set<Value *> marked;

	std::function<void(Value *)> visit = [&](Value *value) {
		if (marked.contains(value))
			return;
		marked.insert(value);
		for (Value *referent: value->getReferents())
			visit(referent);
	};

	for (const Scope &scope: stack.scopes)
		for (const auto &[name, pair]: scope.store)
			visit(pair.first);

	std::vector<Value *> unmarked;

	for (const auto &value: valuePool)
		if (!marked.contains(value) && !globalValues.contains(value))
			unmarked.push_back(value);

	for (const auto &value: unmarked) {
		valuePool.erase(value);
		delete value;
	}
}
