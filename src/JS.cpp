#include <functional>
#include <iostream>

#include "JS.h"
#include "Log.h"
#include "Utils.h"

Reference * ScopeStack::lookup(const std::string &name, ssize_t *depth_out) {
	if (!closures.empty()) {
		Closure &closure = *closures.back();
		if (auto iter = closure.map.find(name); iter != closure.map.end()) {
			if (depth_out != nullptr)
				*depth_out = -1;
			return iter->second;
		}
	}

	ssize_t depth = scopes.size();
	for (Scope &scope: reverse(scopes)) {
		--depth;
		if (auto iter = scope.store.find(name); iter != scope.store.end()) {
			if (depth_out != nullptr)
				*depth_out = depth;
			return iter->second;
		}
	}

	if (auto iter = globals.find(name); iter != globals.end()) {
		if (depth_out != nullptr)
			*depth_out = 0;
		return iter->second;
	}

	if (depth_out != nullptr)
		*depth_out = -1;

	return nullptr;
}

void ScopeStack::insert(const std::string &name, Value *value, bool is_const) {
	if (scopes.empty())
		throw std::runtime_error("Cannot insert into ScopeStack: no scopes present");

	assert(value != nullptr);
	assert(value->context != nullptr);
	scopes.back().store.try_emplace(name, value->context->makeValue<Reference>(value, is_const));
}

bool ScopeStack::inLastScope(const std::string &name) const {
	return !scopes.empty() && scopes.back().store.contains(name);
}

bool Globals::contains(Value *value) const {
	if (values.contains(value))
		return true;

	for (auto *reference = value->cast<Reference>(); reference != nullptr; reference = reference->referent->cast<Reference>()) {
		if (values.contains(reference->referent))
			return true;
		// Won't prevent a cycle containing more than two nodes. Shouldn't ever come up anyway.
		if (reference->referent == reference)
			break;
	}

	return false;
}

void Context::addDefaults() {
	makeGlobal<Object>("this");

	makeGlobal<Function>("print", [](Context &context, const std::vector<Value *> &arguments, Value *) {
		for (Value *value: arguments)
			std::cout << static_cast<std::string>(*value) << std::endl;
		return context.makeValue<Undefined>();
	});

	makeGlobal<Function>("gc", [](Context &context, const std::vector<Value *> &, Value *) {
		const size_t old_size = context.valuePool.size();
		context.garbageCollect();
		return context.makeValue<Number>(old_size - context.valuePool.size());
	});

	auto *object = makeGlobal<Object>("Object");
	(*object)["prototype"] = makePrototype({
		{"toString", {[](Context &context, auto &, auto) {
			return context.toValue("[object Object]");
		}}},
	});

	auto *string = makeGlobal<Object>("String");
	(*string)["prototype"] = makePrototype({
		{"toString", {[](Context &context, auto &, Reference *this_obj) {
			// Just copy the string.
			return context.toValue(this_obj->referent->cast<String>()->string);
		}}},
		{"length", {[](Context &context, auto &args, Reference *this_obj) {
			// Since this is a property, args must have at most a single member (the value to write, if any).
			// We ignore it because length isn't assignable.
			assert(args.size() <= 1);
			return context.toValue(this_obj->referent->cast<String>()->string.size());
		}, true}},
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
		for (const auto &[name, reference]: scope.store)
			visit(reference);

	for (const auto &value: globals)
		visit(value);

	std::vector<Value *> unmarked;

	for (const auto &value: valuePool)
		if (!marked.contains(value) && !globals.contains(value))
			unmarked.push_back(value);

	for (Value *value: unmarked) {
		valuePool.erase(value);
		delete value;
	}
}

Reference * Context::makePrototype(const std::unordered_map<std::string, PrototypeMethod> &map) {
	Object *prototype = makeValue<Object>();

	for (const auto &[method_name, method]: map) {
		Function *function = makeValue<Function>(method.function);
		function->isProperty = method.isProperty;
		prototype->map[method_name] = makeReference(function);
	}

	return makeReference(prototype);
}
