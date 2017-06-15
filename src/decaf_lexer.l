%{

#include <iostream>
#include <fstream>
#include <cstring>
#include "classes.h"
#include "decaf_parser.tab.h"

// GLOBAL DEFINITIONS
#define YY_DECL extern "C" int yylex()

// GLOBALS
extern std::fstream flex_outfile, bison_outfile;

%}
%option noyywrap
%option yylineno
%option nounput

DIGIT [0-9]
HEX [0-9a-fA-F]
CHAR ([\40-\176]{-}['"\\])|\\n|\\t|\\\\|\\\"|\\\'

%%

"boolean"                   { return TYPE_BOOLEAN; }
"break"                     { return BREAK; }
"callout"                   { return CALLOUT; }
"class"                     { return CLASS; }
"continue"                  { return CONTINUE; }
"else"                      { return ELSE; }
"false"                     { return FALSE; }
"for"                       { return FOR; }
"if"                        { return IF; }
"int"                       { return TYPE_INTEGER; }
"return"                    { return RETURN; }
"true"                      { return TRUE; }
"void"                      { return VOID; }

","                         { return COMMA; }
"{"     										{ return O_CUR_BRACE; }
"}"     										{ return C_CUR_BRACE; }
"("     										{ return O_PAREN; }
")"     										{ return C_PAREN; }
";"       									{ return SEMICOLON; }
"["													{ return O_BRACE; }
"]"													{ return C_BRACE; }

"+="                        { return PLUS_EQUAL; }
"-="                        { return MINUS_EQUAL; }

"!="                        { return OP_NOT_EQUAL; }
"<="                        { return OP_LESS_EQUAL; }
">="                        { return OP_GREATER_EQUAL; }
"=="                        { return OP_EQUAL_EQUAL; }

"+"                         { return OP_PLUS; }
"-"                         { return OP_MINUS; }
"*"                         { return OP_MULTIPLY; }
"/"                         { return OP_DIVIDE; }
"%"                         { return OP_MODULO; }

"!"                         { return OP_NOT; }

"<"                         { return OP_LESS_THAN; }
">"                         { return OP_GREATER_THAN; }
"="                         { return EQUAL; }

"||"                        { return OP_LOGICAL_OR; }
"&&"                        { return OP_LOGICAL_AND; }

[a-zA-Z_][a-zA-Z0-9_]*    	{ yylval.sval = strdup(yytext); return IDENTIFIER; }
{DIGIT}+                    { yylval.sval = strdup(yytext); return INT_LITERAL; }
0[xX]{HEX}+                 { yylval.sval = strdup(yytext); return INT_LITERAL; }
\"{CHAR}*\"                 { yylval.sval = strdup(yytext); return STRING_LITERAL; }
\'{CHAR}\'                  { yylval.sval = strdup(yytext); return CHAR_LITERAL; }

\/\/.*                      /* Ignore one line commments */
[ \t\n]*										{}

.       									  {  }
%%
