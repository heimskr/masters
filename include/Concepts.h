#pragma once

#include <concepts>

template <typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;