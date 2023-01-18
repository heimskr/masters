#pragma once

// Credit: https://stackoverflow.com/a/28139075/227663

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
