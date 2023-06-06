#pragma once

#include <cstdio>
#include <string>

#include "ASTNode.h"

using YY_BUFFER_STATE = struct yy_buffer_state *;

class Parser {
	private:
		std::string filename;
		char *buffer = nullptr;
		YY_BUFFER_STATE bufferState = nullptr;

	public:
		ASTNode *root = nullptr;
		int errorCount = 0;

		Parser() = default;
		void open(const std::string &filename);
		void in(const std::string &text);
		void debug(bool flex, bool bison) const;
		void parse() const;
		void done();

		const char * getName(int symbol);
		[[nodiscard]] std::string getBuffer() const;
};

extern Parser jsParser;
