#pragma once

#include <string>
#include <unordered_set>

struct StringSet {
	StringSet();

	static std::unordered_set<std::string> set;
	static const std::string * intern(const char *);
	static const std::string * intern(std::string_view);
	static const std::string * intern(const std::string &);
};
