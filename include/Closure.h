#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

class Reference;

struct Closure {
	std::unordered_set<Reference *> references;
	/** Pair: (value, isConst) */
	std::unordered_map<std::string, Reference *> map;
	Closure() = default;
	Closure(decltype(references) references_, decltype(map) map_):
		references(std::move(references_)), map(std::move(map_)) {}
};

