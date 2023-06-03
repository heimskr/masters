#include <functional>

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

	while (!marked.empty()) {
		Value *value = *marked.begin();
		marked.erase(value);
		valuePool.erase(value);
		delete value;
	}
}
