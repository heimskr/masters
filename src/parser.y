%{
#include <cassert>
#include <cstdarg>
#include <initializer_list>

#define NO_YYPARSE
#include "Lexer.h"
#include "ASTNode.h"
#include "Parser.h"

// Disable PVS-Studio warnings about branches that do the same thing.
//-V::1037

template <typename ...Args>
void D(Args && ...args) {
	(void) std::initializer_list<int> {
		((void) delete args, 0)...
	};
}

using AN = ASTNode;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
%}

%debug
%defines
%define parse.error verbose
%token-table
%verbose

%define api.prefix {js}

%initial-action {
    jsParser.root = new ASTNode(jsParser, JSTOK_ROOT, ASTLocation(), "");
}

%token JSTOK_ROOT JSTOK_NUMBER JSTOK_IDENT JSTOK_STRING
%token JSTOK_LPAREN "("
%token JSTOK_RPAREN ")"
%token JSTOK_PLUS "+"
%token JSTOK_MINUS "-"
%token JSTOK_TIMES "*"
%token JSTOK_DIV "/"
%token JSTOK_XOR "^"
%token JSTOK_TRUE "true"
%token JSTOK_FALSE "false"
%token JSTOK_IF "if"
%token JSTOK_ELSE "else"
%token JSTOK_WHILE "while"
%token JSTOK_SEMI ";"
%token JSTOK_TEQ "==="
%token JSTOK_ASSIGN "="
%token JSTOK_LAND "&&"
%token JSTOK_LOR "||"
%token JSTOK_AND "&"
%token JSTOK_OR "|"
%token JSTOK_LTE "<="
%token JSTOK_GTE ">="
%token JSTOK_LT "<"
%token JSTOK_GT ">"
%token JSTOK_NOT "!"
%token JSTOK_TILDE "~"
%token JSTOK_NTEQ "!=="
%token JSTOK_LBRACE "{"
%token JSTOK_RBRACE "}"
%token JSTOK_LSQUARE "["
%token JSTOK_RSQUARE "]"
%token JSTOK_COMMA ","
%token JSTOK_QUESTION "?"
%token JSTOK_COLON ":"
%token JSTOK_RETURN "return"
%token JSTOK_PERIOD "."
%token JSTOK_VOID "void"
%token JSTOK_LSHIFT "<<"
%token JSTOK_RSHIFTA ">>"
%token JSTOK_RSHIFTL ">>>"
%token JSTOK_META_NAME "#name"
%token JSTOK_META_AUTHOR "#author"
%token JSTOK_META_VERSION "#version"
%token JSTOK_META_ORCID "#orcid"
%token JSTOK_BREAK "break"
%token JSTOK_CONTINUE "continue"
%token JSTOK_FOR "for"
%token JSTOK_MOD "%"
%token JSTOK_PLUSEQ "+="
%token JSTOK_MINUSEQ "-="
%token JSTOK_PLUSPLUS "++"
%token JSTOK_MINUSMINUS "--"
%token JSTOK_DO "do"
%token JSTOK_NULL "null"
%token JSTOK_DIVEQ   "/="
%token JSTOK_TIMESEQ "*="
%token JSTOK_MODEQ "%="
%token JSTOK_SRAEQ ">>="
%token JSTOK_SRLEQ ">>>="
%token JSTOK_SLEQ "<<="
%token JSTOK_ANDEQ "&="
%token JSTOK_LANDEQ "&&="
%token JSTOK_OREQ "|="
%token JSTOK_LOREQ "||="
%token JSTOK_XOREQ "^="
%token JSTOK_EXP "**"
%token JSTOK_EXPEQ "**="
%token JSTOK_NEW "new"
%token JSTOK_DELETE "delete"
%token JSTOK_UNDEFINED "undefined"
%token JSTOK_NAN "NaN"
%token JSTOK_LET "let"
%token JSTOK_CONST "const"
%token JSTOK_INFINITY "Infinity"
%token JSTOK_FUNCTION "function"
%token JSTOK_TYPEOF "typeof"
%token JSTOK_IN "in"
%token JSTOK_INSTANCEOF "instanceof"

%token JS_LIST JS_BLOCK JS_EMPTY JS_POSTPLUS JS_POSTMINUS

%start start

%left ";"
%left ","
%right "?"
%right "=" "+=" "-=" "*=" "/=" "%=" "<<=" ">>=" ">>>=" "&=" "|=" "^=" "&&=" "||="
%left "||"
%left "&&"
%left "|"
%left "^"
%left "&"
%left "==" "!="
%left "<" "<=" ">" ">=" "in" "instanceof"
%left "<<" ">>" ">>>"
%left "+" "-"
%left "*" "/" "%"
%right "**"
%nonassoc "!" "~" PREFIX UNARY "typeof" "void" "delete"
%nonassoc POSTFIX
%nonassoc NEW_NO_ARGS
%nonassoc "[" CALL NEW_ARGS
%left "."
%nonassoc "("
%nonassoc "else"

%%

start: program;

program: program statement { $$ = $1->adopt($2); }
       | { $$ = jsParser.root; };

statement: block
         | for_loop
         | while_loop
         | conditional
         | var_def
         | var_decl
         | expr       ";" { D($2); }
         | "return"   ";" { D($2); }
         | "break"    ";" { D($2); }
         | "continue" ";" { D($2); }
         | "return" expr ";" { $$ = $1->adopt($2); D($3); }
         | ";" { $1->symbol = JS_EMPTY; };

var_def: "let"   ident "=" expr ";" { $$ = $1->adopt({$2, $4}); D($3, $5); }
       | "const" ident "=" expr ";" { $$ = $1->adopt({$2, $4}); D($3, $5); }

var_decl: "let"   ident ";" { $$ = $1->adopt($2); D($3); }
        | "const" ident ";" { $$ = $1->adopt($2); D($3); }

function: "function"       "(" arglist_ ")" block { $$ = $1->adopt({$3, $5});     D($2, $4); }
        | "function" ident "(" arglist_ ")" block { $$ = $1->adopt({$4, $6, $2}); D($3, $5); };

block: "{" statements "}" { $$ = $2; D($1, $3); };

statements: statements statement { $$ = $1->adopt($2); }
          |                      { $$ = new ASTNode(jsParser, JS_BLOCK); };

conditional: "if" "(" expr ")" statement "else" statement { $$ = $1->adopt({$3, $5, $7}); D($2, $4, $6); }
           | "if" "(" expr ")" statement                  { $$ = $1->adopt({$3, $5});     D($2, $4);     };

while_loop: "while" "(" expr ")" statement          { $$ = $1->adopt({$3, $5}); D($2, $4);     }
          | "do" statement "while" "(" expr ")" ";" { $$ = $1->adopt({$5, $2}); D($4, $6, $7); };

for_loop: "for" "(" expr ";" expr_ ";" expr_ ")" statement { $$ = $1->adopt({$3, $5, $7, $9}); D($2, $4, $6, $8); };

expr_: expr | { $$ = new ASTNode(jsParser, JS_EMPTY); };

expr: expr "&&"   expr { $$ = $2->adopt({$1, $3}); }
    | expr "||"   expr { $$ = $2->adopt({$1, $3}); }
    | expr "&"    expr { $$ = $2->adopt({$1, $3}); }
    | expr "|"    expr { $$ = $2->adopt({$1, $3}); }
    | expr "^"    expr { $$ = $2->adopt({$1, $3}); }
    | expr "==="  expr { $$ = $2->adopt({$1, $3}); }
    | expr "!=="  expr { $$ = $2->adopt({$1, $3}); }
    | expr "<<"   expr { $$ = $2->adopt({$1, $3}); }
    | expr ">>"   expr { $$ = $2->adopt({$1, $3}); }
    | expr "<"    expr { $$ = $2->adopt({$1, $3}); }
    | expr ">"    expr { $$ = $2->adopt({$1, $3}); }
    | expr "<="   expr { $$ = $2->adopt({$1, $3}); }
    | expr ">="   expr { $$ = $2->adopt({$1, $3}); }
    | expr "+"    expr { $$ = $2->adopt({$1, $3}); }
    | expr "-"    expr { $$ = $2->adopt({$1, $3}); }
    | expr "*"    expr { $$ = $2->adopt({$1, $3}); }
    | expr "**"   expr { $$ = $2->adopt({$1, $3}); }
    | expr "/"    expr { $$ = $2->adopt({$1, $3}); }
    | expr "="    expr { $$ = $2->adopt({$1, $3}); }
    | expr "%"    expr { $$ = $2->adopt({$1, $3}); }
    | expr "+="   expr { $$ = $2->adopt({$1, $3}); }
    | expr "-="   expr { $$ = $2->adopt({$1, $3}); }
    | expr "*="   expr { $$ = $2->adopt({$1, $3}); }
    | expr "**="  expr { $$ = $2->adopt({$1, $3}); }
    | expr "/="   expr { $$ = $2->adopt({$1, $3}); }
    | expr "%="   expr { $$ = $2->adopt({$1, $3}); }
    | expr "<<="  expr { $$ = $2->adopt({$1, $3}); }
    | expr ">>="  expr { $$ = $2->adopt({$1, $3}); }
    | expr ">>>=" expr { $$ = $2->adopt({$1, $3}); }
    | expr "&="   expr { $$ = $2->adopt({$1, $3}); }
    | expr "|="   expr { $$ = $2->adopt({$1, $3}); }
    | expr "||="  expr { $$ = $2->adopt({$1, $3}); }
    | expr "&&="  expr { $$ = $2->adopt({$1, $3}); }
    | expr "^="   expr { $$ = $2->adopt({$1, $3}); }
    | expr "in"   expr { $$ = $2->adopt({$1, $3}); }
    | expr "instanceof" expr { $$ = $2->adopt({$1, $3}); }
    | expr "." JSTOK_IDENT %prec "." { $$ = $2->adopt({$1, $3});        }
    | expr "[" expr "]"    %prec "[" { $$ = $2->adopt({$1, $3}); D($4); }
    | function_call %prec CALL
    | "(" expr ")" %prec "(" { $$ = $2; D($1, $3); }
    | "!" expr { $$ = $1->adopt($2); }
    | "~" expr { $$ = $1->adopt($2); }
    | expr "?" expr ":" expr %prec "?" { $$ = $2->adopt({$1, $3, $5}); D($4); }
    | "+" number %prec UNARY   { $$ = $2->adopt($1); }
    | "-" number %prec UNARY   { $$ = $2->adopt($1); }
    | "++" expr  %prec PREFIX  { $$ = $1->adopt($2); }
    | expr "++"  %prec POSTFIX { $$ = $2->adopt($1); $$->symbol = JS_POSTPLUS; }
    | "--" expr  %prec PREFIX  { $$ = $1->adopt($2); }
    | expr "--"  %prec POSTFIX { $$ = $2->adopt($1); $$->symbol = JS_POSTMINUS; }
    | JSTOK_IDENT
    | boolean
    | string
    | number
    | "new" expr "(" exprlist_ ")" %prec NEW_ARGS { $$ = $1->adopt({$2, $4}); D($3, $5); }
    | "new" expr %prec NEW_NO_ARGS { $$ = $1->adopt($2); }
	| "delete" expr { $$ = $1->adopt($2); }
	| "void"   expr { $$ = $1->adopt($2); }
	| "typeof" expr { $$ = $1->adopt($2); }
    | "null"
    | "undefined";
    | "NaN";
    | "Infinity";

string: JSTOK_STRING;

boolean: "true" | "false";

function_call: expr "(" exprlist_ ")" { $$ = $2->adopt({$1, $3}); D($4); };

exprlist: exprlist "," expr { $$ = $1->adopt($3); D($2); }
        | expr              { $$ = (new ASTNode(jsParser, JS_LIST))->locate($1)->adopt($1); };

exprlist_: exprlist
         | { $$ = new ASTNode(jsParser, JS_LIST); };

arglist: arglist "," ident { $$ = $1->adopt($3); D($2); }
       | ident             { $$ = (new ASTNode(jsParser, JS_LIST))->locate($1)->adopt($1); };

arglist_: arglist
        | { $$ = new ASTNode(jsParser, JS_LIST); };

number: JSTOK_NUMBER;
ident:  JSTOK_IDENT;

%%

#pragma GCC diagnostic pop

const char * Parser::getName(int symbol) {
    return yytname[YYTRANSLATE(symbol)];
}
