#pragma once

#include <cstdio>
#include <unordered_map>
#include <vector>

#include "ASTNode.h"

#define JSSTYPE_IS_DECLARED
using JSSTYPE = ASTNode *;

#ifndef NO_YYPARSE
#include "bison.h"
#endif

#ifdef __APPLE__
using yysize = size_t;
#else
using yysize = int;
#endif

extern FILE *jsin;
extern char *jstext;
extern yysize jsleng;
extern int js_flex_debug;

class Parser;

class Lexer {
	private:
		Parser *parser;
		yysize *leng;
		ASTNode **lval;

	public:
		ASTLocation location {0, 1};
		std::string line;
		yysize lastYylength = 0;
		std::unordered_map<int, std::string> lines;
		bool failed = false;
		std::vector<std::pair<std::string, ASTLocation>> errors;

		Lexer(Parser &, yysize &, ASTNode *&);
		const std::string * filename(int fileno);
		void advance(const char *);
		void newline();
		void badchar(unsigned char);
		int token(const char *, int symbol);
};

extern Lexer jsLexer;

int jslex();
int jslex_destroy();
void jserror(const std::string &);
void jserror(const std::string &, const ASTLocation &);
