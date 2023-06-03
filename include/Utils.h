#pragma once

#include <cstdint>
#include <string>

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
