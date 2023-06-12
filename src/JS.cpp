#include <cmath>
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

static String * doSubstring(Context &context, const std::vector<Value *> &args, Reference *this_obj, bool is_substr) {
	auto *string = this_obj->ultimateValue()->cast<String>();
	assert(string != nullptr);

	size_t start = 0;
	size_t count = std::string::npos;

	if (!args.empty()) {
		if (const auto first = static_cast<double>(*args.front()); isFinite(first))
			start = static_cast<size_t>(first);

		if (1 < args.size())
			if (const auto second = static_cast<double>(*args[1]); isFinite(second))
				count = static_cast<size_t>(second);
	}

	if (string->string.size() <= start || (!is_substr && count <= start))
		return context.toValue("");

	if (!is_substr)
		count -= start;

	return context.toValue(string->string.substr(start, count));
}


static String * doCharAt(Context &context, const std::vector<Value *> &args, Reference *this_obj) {
	auto *string = this_obj->ultimateValue()->cast<String>();
	assert(string != nullptr);

	size_t index = 0;
	if (!args.empty())
		if (const auto number = static_cast<double>(*args.front()); isFinite(number))
			index = static_cast<size_t>(number);

	if (index < string->string.size())
		return context.toValue(std::string(1, string->string.at(index)));

	return context.toValue("");
}

static Value * doPrint(Context &context, const std::vector<Value *> &arguments, Value *) {
	bool first = true;
	for (Value *value: arguments) {
		if (first == true)
			first = false;
		else
			std::cout << ' ';
		std::cout << static_cast<std::string>(*value);
	}
	std::cout << std::endl;
	return context.makeValue<Undefined>();
}

void Context::addDefaults() {
	makeGlobal<Object>("this");

	makeGlobal<Function>("print", doPrint);

	auto *console = makeGlobal<Object>("console");
	(*console)["log"] = makeReference<Function>(doPrint);

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
		{"substr", {[](Context &context, auto &args, Reference *this_obj) {
			return doSubstring(context, args, this_obj, true);
		}}},
		{"substring", {[](Context &context, auto &args, Reference *this_obj) {
			return doSubstring(context, args, this_obj, false);
		}}},
		{"charAt", {[](Context &context, auto &args, Reference *this_obj) {
			return doCharAt(context, args, this_obj);
		}}},
		{"charCodeAt", {[](Context &, auto &, Reference *) -> Value * {
			throw Unimplemented();
		}}},
		{"concat", {[](Context &context, auto &args, Reference *this_obj) {
			auto *string = this_obj->ultimateValue()->cast<String>();
			assert(string != nullptr);
			std::string out = string->string;
			for (const Value *arg: args)
				out += static_cast<std::string>(*arg);
			return context.toValue(out);
		}}},
		{"endsWith", {[](Context &context, auto &args, Reference *this_obj) {
			auto *string = this_obj->ultimateValue()->cast<String>();
			assert(string != nullptr);

			std::string search_string;
			size_t expected = string->string.size();

			if (!args.empty()) {
				search_string = static_cast<std::string>(*args.front());
				if (2 <= args.size()) {
					if (const auto number = static_cast<double>(*args[1]); isFinite(number))
						expected = static_cast<size_t>(number);
					else
						expected = 0;
				}
			} else
				search_string = "undefined"; // certified JavaScript moment

			const size_t search_position = std::min(expected, string->string.size()) - search_string.size();

			try {
				return context.toValue(string->string.substr(search_position, search_string.size()) == search_string);
			} catch (const std::out_of_range &) {
				return context.toValue(false);
			}
		}}},
		{"includes", {[](Context &context, auto &args, Reference *this_obj) {
			if (args.empty())
				return context.toValue(false);

			auto *string = this_obj->ultimateValue()->cast<String>();
			assert(string != nullptr);

			const auto arg = static_cast<std::string>(*args.front());
			return context.toValue(arg.empty() || string->string.find(arg) != std::string::npos);
		}}},
		{"indexOf", {[](Context &context, auto &args, Reference *this_obj) {
			auto *string = this_obj->ultimateValue()->cast<String>();
			assert(string != nullptr);

			const auto arg = args.empty()? "undefined" : static_cast<std::string>(*args.front());
			size_t start = 0;
			if (1 < args.size()) {
				const auto number = static_cast<double>(*args[1]);
				if (std::isinf(number))
					return context.toValue(-1.);
				if (!std::isnan(number))
					start = static_cast<size_t>(number);
			}

			const size_t position = string->string.find(arg, start);
			return context.toValue(position == std::string::npos? -1. : static_cast<double>(position));
		}}},
	});

	(*string)["fromCharCode"] = makeReference<Function>([](Context &context, const auto &args, auto) {
		std::string out;
		for (const Value *arg: args) {
			size_t char_code = 0;
			if (const auto number = static_cast<double>(*arg); isFinite(number))
				char_code = static_cast<size_t>(number);
			assert(char_code <= 255);
			out += static_cast<char>(char_code);
		}
		return context.makeValue<String>(std::move(out));
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
