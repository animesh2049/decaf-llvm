%{

#include <iostream>
#include <fstream>
#include <string.h>

#include "decaf_parser.tab.h"

// GLOBAL DEFINITIONS
#define YY_DECL extern "C" int yylex()

// GLOBALS
std::fstream flex_outfile, bison_outfile;

%}
%option noyywrap
%option yylineno
%option nounput

DIGIT [0-9]
HEX [0-9a-fA-F]
CHAR ([\40-\176]{-}['"\\])|\\n|\\t|\\\\|\\\"|\\\'

%%

"boolean"                   { flex_outfile << "BOOLEAN_DECLARATION" << std::endl; return TYPE_BOOLEAN; }
"break"                     { flex_outfile << "BREAK" << std::endl; return BREAK; }
"callout"                   { flex_outfile << "CALLOUT" << std::endl; return CALLOUT; }
"class"                     { flex_outfile << "CLASS" << std::endl; return CLASS; }
"continue"                  { flex_outfile << "CONTINUE" << std::endl; return CONTINUE; }
"else"                      { flex_outfile << "ELSE" << std::endl; return ELSE; }
"false"                     { flex_outfile << "BOOLEAN: false" << std::endl; return FALSE; }
"for"                       { flex_outfile << "FOR" << std::endl; return FOR; }
"if"                        { flex_outfile << "IF" << std::endl; return IF; }
"int"                       { flex_outfile << "INT_DECLARATION" << std::endl; return TYPE_INTEGER; }
"return"                    { flex_outfile << "RETURN" << std::endl; return RETURN; }
"true"                      { flex_outfile << "BOOLEAN: true" << std::endl; return TRUE; }
"void"                      { flex_outfile << "VOID_DECLARATION" << std::endl; return VOID; }

","                         { flex_outfile << "CONNECTIVE: COMMA" << std::endl; return COMMA; }
"{"     										{ flex_outfile << "CONNECTIVE: O_CUR_BRACE" << std::endl; return O_CUR_BRACE; }
"}"     										{ flex_outfile << "CONNECTIVE: C_CUR_BRACE" << std::endl; return C_CUR_BRACE; }
"("     										{ flex_outfile << "CONNECTIVE: O_PAREN" << std::endl; return O_PAREN; }
")"     										{ flex_outfile << "CONNECTIVE: C_PAREN" << std::endl; return C_PAREN; }
";"       									{ flex_outfile << "CONNECTIVE: SEMICOLON" << std::endl; return SEMICOLON; }
"["													{ flex_outfile << "CONNECTIVE: O_BRACE" << std::endl; return O_BRACE; }
"]"													{ flex_outfile << "CONNECTIVE: C_BRACE" << std::endl; return C_BRACE; }

"+="                        { flex_outfile << "ASSIGNMENT: PLUS_EQ" << std::endl; return PLUS_EQUAL; }
"-="                        { flex_outfile << "ASSIGNMENT: MINUS_EQ" << std::endl; return MINUS_EQUAL; }

"!="                        { flex_outfile << "OP: NEQ" << std::endl; return OP_NOT_EQUAL; }
"<="                        { flex_outfile << "OP: LEQ" << std::endl; return OP_LESS_EQUAL; }
">="                        { flex_outfile << "OP: GEQ" << std::endl; return OP_GREATER_EQUAL; }
"=="                        { flex_outfile << "OP: EEQ" << std::endl; return OP_EQUAL_EQUAL; }

"+"                         { flex_outfile << "REL_OP: PLUS" << std::endl; return OP_PLUS; }
"-"                         { flex_outfile << "REL_OP: MINUS" << std::endl; return OP_MINUS; }
"*"                         { flex_outfile << "REL_OP: MULTIPLY" << std::endl; return OP_MULTIPLY; }
"/"                         { flex_outfile << "REL_OP: DIVIDE" << std::endl; return OP_DIVIDE; }
"%"                         { flex_outfile << "REL_OP: MODULO" << std::endl; return OP_MODULO; }

"!"                         { flex_outfile << "OP: NOT" << std::endl; return OP_NOT; }

"<"                         { flex_outfile << "REL_OP: LT" << std::endl; return OP_LESS_THAN; }
">"                         { flex_outfile << "REL_OP: GT" << std::endl; return OP_GREATER_THAN; }
"="                         { flex_outfile << "ASSIGNMENT: EQ" << std::endl; return EQUAL; }

"||"                        { flex_outfile << "LOGICAL_OP: OR" << std::endl; return OP_LOGICAL_OR; }
"&&"                        { flex_outfile << "LOGICAL_OP: AND" << std::endl; return OP_LOGICAL_AND; }

[a-zA-Z_][a-zA-Z0-9_]*    	{ flex_outfile << "ID: " << yytext << std::endl; yylval.sval = strdup(yytext);				return IDENTIFIER; }
[+-]?{DIGIT}+               { flex_outfile << "INT: " << yytext << std::endl; yylval.sval = strdup(yytext);				return INT_LITERAL; }
0[xX]{HEX}+                 { flex_outfile << "INT: " << yytext << std::endl; yylval.sval = strdup(yytext);				return INT_LITERAL; }
\"{CHAR}*\"                 { flex_outfile << "STRING: " << yytext << std::endl; yylval.sval = strdup(yytext);				return STRING_LITERAL; }
\'{CHAR}\'                  { flex_outfile << "CHARACTER:" << yytext << std::endl; yylval.sval = strdup(yytext);				return CHAR_LITERAL; }

\/\/.*                      /* Ignore one line commments */
[ \t]*											{}
[\n]												{ yylineno++; }

.       									{ std::cout << "Error by lexer" << std::endl; }
%%
