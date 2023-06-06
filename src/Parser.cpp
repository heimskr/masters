#include <cstring>
#include <fstream>
#include <sstream>

#include "Lexer.h"
#include "Parser.h"
#include "StringSet.h"

int jswrap() { return 1; }

extern YY_BUFFER_STATE js_scan_buffer(char *, size_t);
extern void js_delete_buffer(YY_BUFFER_STATE);

void Parser::open(const std::string &filename_) {
	errorCount = 0;
	filename = filename_;
	jsin = fopen(filename.c_str(), "re");
}

void Parser::in(const std::string &text) {
	errorCount = 0;
	buffer = new char[text.size() + 2];
	std::strncpy(buffer, text.c_str(), text.size() + 1);
	buffer[text.size() + 1] = '\0'; // Input to flex needs two null terminators.
	bufferState = js_scan_buffer(buffer, text.size() + 2);
}

void Parser::debug(bool flex, bool bison) const {
	js_flex_debug = int(flex);
	jsdebug = int(bison);
}

void Parser::parse() const {
	jsparse();
}

void Parser::done() {
	jslex_destroy();
	delete root;
	delete buffer;
	root = nullptr;
	buffer = nullptr;
}

std::string Parser::getBuffer() const {
	return buffer != nullptr? buffer : "";
}

Parser jsParser;
