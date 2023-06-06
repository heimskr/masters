#pragma once

#include <cstdint>
#include <string>
#include <vector>

// Credit for reverse: https://stackoverflow.com/a/28139075/227663

template <typename T>
struct reverse {
	T &iterable;
	reverse() = delete;
	reverse(T &iterable_): iterable(iterable_) {}
};

template <typename T>
auto begin(reverse<T> r) {
	return std::rbegin(r.iterable);
}

template <typename T>
auto end(reverse<T> r) {
	return std::rend(r.iterable);
}

int64_t  parseLong(std::string_view, int base = 10);
uint64_t parseUlong(std::string_view, int base = 10);
double   parseDouble(std::string_view);

std::string unescapeString(std::string_view);

/** Splits a string by a given delimiter. If condense is true, empty strings won't be included in the output. */
template <typename T = std::string_view>
std::vector<T> split(std::string_view str, std::string_view delimiter, bool condense = true);

std::string readFile(const std::string &);

template <typename T, typename M>
struct FieldSaver {
	T &object;
	M T::*member;
	M oldValue;

	FieldSaver(T &object_, M T::*member_): object(object_), member(member_), oldValue(object_.*member_) {}
	~FieldSaver() { object.*member = oldValue; }
};
