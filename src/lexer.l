%{
#include "Lexer.h"
#ifdef YY_USER_ACTION
#error "YY_USER_ACTION is already defined"
#endif
#define YY_USER_ACTION { jsLexer.advance(jstext); }
#define RTOKEN(x) return jsLexer.token(jstext, JSTOK_##x);

// Disable PVS-Studio warnings about branches that do the same thing.
//-V::1037
%}

%option 8bit
%option debug
%option nobackup
%option nodefault
%option noinput
%option nounput
%option noyywrap
%option warn

JS_DECIMAL      ([0-9]+)
JS_HEX          (0x[0-9a-fA-F]+)
JS_BINARY       (0b[01]+)
JS_FLOATING     ("-"?([0-9]+"."[0-9]+|[0-9]+"."|"."[0-9]+)("e"[+\-]?[0-9]+)?)
JS_IDENT        ([a-zA-Z_$][$a-zA-Z_0-9]*)
JS_DSTRING      (\"([^\\"\n]|\\([\\"nratbe0]|x[0-9a-fA-F][0-9a-fA-F]))*\")
JS_SSTRING      (\'([^\\'\n]|\\([\\'nratbe0]|x[0-9a-fA-F][0-9a-fA-F]))*\')
JS_MLCOMMENT    ("/*"([^*]|"*"[^/])*"*/")
JS_SLCOMMENT    ("//"[^\n]*)
JS_SHEBANG      ("#!"[^\n]*)

%%

{JS_MLCOMMENT}  {}
{JS_SLCOMMENT}  {}
{JS_SHEBANG}    {}
[ \t]+          {}
\n              {jsLexer.newline();}

"instanceof"    {RTOKEN(INSTANCEOF)}
"undefined"     {RTOKEN(UNDEFINED)}
"arguments"     {RTOKEN(ARGUMENTS)}
"continue"      {RTOKEN(CONTINUE)}
"Infinity"      {RTOKEN(INFINITY)}
"function"      {RTOKEN(FUNCTION)}
"return"        {RTOKEN(RETURN)}
"typeof"        {RTOKEN(TYPEOF)}
"delete"        {RTOKEN(DELETE)}
"false"         {RTOKEN(FALSE)}
"while"         {RTOKEN(WHILE)}
"break"         {RTOKEN(BREAK)}
"const"         {RTOKEN(CONST)}
"null"          {RTOKEN(NULL)}
"true"          {RTOKEN(TRUE)}
"else"          {RTOKEN(ELSE)}
"void"          {RTOKEN(VOID)}
">>>="          {RTOKEN(SRLEQ)}
"for"           {RTOKEN(FOR)}
"NaN"           {RTOKEN(NAN)}
"new"           {RTOKEN(NEW)}
"let"           {RTOKEN(LET)}
"var"           {RTOKEN(VAR)}
">>="           {RTOKEN(SRAEQ)}
"<<="           {RTOKEN(SLEQ)}
">>>"           {RTOKEN(RSHIFTL)}
"**="           {RTOKEN(EXPEQ)}
"!=="           {RTOKEN(NTEQ)}
"==="           {RTOKEN(TEQ)}
"in"            {RTOKEN(IN)}
"if"            {RTOKEN(IF)}
"do"            {RTOKEN(DO)}
"**"            {RTOKEN(EXP)}
"+="            {RTOKEN(PLUSEQ)}
"-="            {RTOKEN(MINUSEQ)}
"/="            {RTOKEN(DIVEQ)}
"*="            {RTOKEN(TIMESEQ)}
"%="            {RTOKEN(MODEQ)}
"&="            {RTOKEN(ANDEQ)}
"|="            {RTOKEN(OREQ)}
"^="            {RTOKEN(XOREQ)}
"++"            {RTOKEN(PLUSPLUS)}
"--"            {RTOKEN(MINUSMINUS)}
"&&"            {RTOKEN(LAND)}
"||"            {RTOKEN(LOR)}
"<<"            {RTOKEN(LSHIFT)}
">>"            {RTOKEN(RSHIFTA)}
"<="            {RTOKEN(LTE)}
">="            {RTOKEN(GTE)}
"("             {RTOKEN(LPAREN)}
")"             {RTOKEN(RPAREN)}
"+"             {RTOKEN(PLUS)}
"-"             {RTOKEN(MINUS)}
"*"             {RTOKEN(TIMES)}
"/"             {RTOKEN(DIV)}
"^"             {RTOKEN(XOR)}
";"             {RTOKEN(SEMI)}
"&"             {RTOKEN(AND)}
"|"             {RTOKEN(OR)}
"!"             {RTOKEN(NOT)}
"<"             {RTOKEN(LT)}
">"             {RTOKEN(GT)}
"="             {RTOKEN(ASSIGN)}
"{"             {RTOKEN(LBRACE)}
"}"             {RTOKEN(RBRACE)}
"["             {RTOKEN(LSQUARE)}
"]"             {RTOKEN(RSQUARE)}
","             {RTOKEN(COMMA)}
"?"             {RTOKEN(QUESTION)}
":"             {RTOKEN(COLON)}
"."             {RTOKEN(PERIOD)}
"~"             {RTOKEN(TILDE)}
"%"             {RTOKEN(MOD)}

{JS_DSTRING}    {RTOKEN(STRING)}
{JS_SSTRING}    {RTOKEN(STRING)}
{JS_DECIMAL}    {RTOKEN(NUMBER)}
{JS_HEX}        {RTOKEN(NUMBER)}
{JS_BINARY}     {RTOKEN(NUMBER)}
{JS_FLOATING}   {RTOKEN(NUMBER)}
{JS_IDENT}      {RTOKEN(IDENT)}
.               {jsLexer.badchar(*yytext);}

%%

#undef RTOKEN
