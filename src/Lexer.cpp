#include <array>
#include <iostream>

#include "Lexer.h"
#include "Parser.h"
#include "Utils.h"

Lexer jsLexer(jsParser, jsleng, jslval);

Lexer::Lexer(Parser &parser_, yysize &yyleng_ref, ASTNode *&yylval_ref):
	parser(&parser_), leng(&yyleng_ref), lval(&yylval_ref) {}

void Lexer::advance(const char *text) {
	line += text;
	location.column += lastYylength;
	lastYylength = *leng;

	size_t newline_count = 0;
	size_t i = 0;
	size_t col = location.column;
	char ch = text[0];

	while (ch != '\0') {
		if (ch == '\n') {
			++newline_count;
			col = 0;
		} else ++col;
		ch = text[++i];
	}

	if (1 < newline_count) {
		lastYylength = int(col);
		line.clear();
		location.line += newline_count;
	}
}

void Lexer::newline() {
	lines.insert({location.line, line});
	line.clear();
	++location.line;
	location.column = 0;
}

void Lexer::badchar(unsigned char ch) {
	failed = true;
	std::cerr << "\e[31mBad character at \e[1m" << location << "\e[22m:\e[39m ";
	if (isgraph(ch) != 0) {
		std::cerr << "'" << ch << "'\n";
	} else {
		std::array<char, 10> buffer {};
		sprintf(buffer.data(), "'\\%03o'", ch);
		std::cerr << buffer.data() << "\n";
	}
}

int Lexer::token(const char *text, int symbol) {
	*lval = new ASTNode(*parser, symbol, location, text);
	return symbol;
}

void jserror(const std::string &message) {
	jserror(message, jsLexer.location);
}

void jserror(const std::string &message, const ASTLocation &location) {
	std::cerr << split(jsParser.getBuffer(), "\n", false).at(location.line) << "\n";
	std::cerr << "\e[31mParsing error at \e[1m" << location << "\e[22m: " << message << "\e[0m\n";
	++jsParser.errorCount;
	jsLexer.errors.emplace_back(message, location);
}
