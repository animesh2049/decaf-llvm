%{
#include <iostream>
#include <fstream>
#include <string.h>

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
extern int yylineno;
extern std::fstream flex_outfile;
extern std::fstream bison_outfile;

void yyerror(const char *s);
%}

%union {
	char* sval;
}


%token 	<sval> BREAK
%token 	<sval> CALLOUT
%token 	<sval> CLASS
%token 	<sval> CONTINUE
%token 	<sval> ELSE
%token 	<sval> FALSE
%token 	<sval> FOR
%token 	<sval> IF
%token 	<sval> RETURN
%token 	<sval> TRUE
%token 	<sval> VOID
%token 	<sval> COMMA

%token 	<sval> TYPE_BOOLEAN
%token 	<sval> TYPE_INTEGER

%token 	<sval> O_CUR_BRACE
%token 	<sval> C_CUR_BRACE
%token 	<sval> O_PAREN
%token 	<sval> C_PAREN
%token 	<sval> SEMICOLON
%token 	<sval> O_BRACE
%token 	<sval> C_BRACE

%left 	<sval> OP_LOGICAL_OR OP_LOGICAL_AND

%token 	<sval> EQUAL PLUS_EQUAL MINUS_EQUAL
%left 	<sval> OP_NOT_EQUAL OP_EQUAL_EQUAL

%nonassoc <sval> OP_LESS_THAN OP_GREATER_THAN OP_LESS_EQUAL OP_GREATER_EQUAL

%left 	<sval> OP_PLUS OP_MINUS

%left 	<sval> OP_MULTIPLY OP_DIVIDE OP_MODULO

%precedence OP_NOT OP_UMINUS

%token 	<sval> IDENTIFIER
%token 	<sval> INT_LITERAL
%token 	<sval> STRING_LITERAL
%token 	<sval> CHAR_LITERAL

%%

program:
	CLASS IDENTIFIER O_CUR_BRACE field_decls method_decls C_CUR_BRACE		{ bison_outfile << "PROGRAM ENCOUNTERED" << std::endl; }
	|
	CLASS IDENTIFIER O_CUR_BRACE field_decls C_CUR_BRACE								{ bison_outfile << "PROGRAM ENCOUNTERED" << std::endl; }
	|
	CLASS IDENTIFIER O_CUR_BRACE method_decls C_CUR_BRACE								{ bison_outfile << "PROGRAM ENCOUNTERED" << std::endl; }
	|
	CLASS IDENTIFIER O_CUR_BRACE C_CUR_BRACE														{ bison_outfile << "PROGRAM ENCOUNTERED" << std::endl; }
	;

field_decls:
	field_decls field_decl
	|
	field_decl
	;

method_decls:
	method_decls method_decl
	|
	method_decl
	;

field_decl:
	type id_all_list SEMICOLON																					{ bison_outfile << "DECLARATION ENCOUNTERED" << std::endl; }
	;

id_all_list:
	id_all_list COMMA IDENTIFIER																				{ bison_outfile << "ID=" << $3 << std::endl; }
	|
	id_all_list COMMA id_array
	|
	IDENTIFIER																													{ bison_outfile << "ID=" << $1 << std::endl; }
	|
	id_array
	;

id_array:
	IDENTIFIER O_BRACE INT_LITERAL C_BRACE															{ bison_outfile << "ID=" << $1 << std::endl << "SIZE=" << $3 << std::endl; }
	;

method_decl:
	type IDENTIFIER O_PAREN method_id_list C_PAREN block
	|
	VOID IDENTIFIER O_PAREN method_id_list C_PAREN block
	;

method_id_list :
	method_id_list COMMA type IDENTIFIER
	|
	type IDENTIFIER
	;

block:
	O_CUR_BRACE var_decls statements C_CUR_BRACE
	|
	O_CUR_BRACE statements C_CUR_BRACE
	|
	O_CUR_BRACE var_decls C_CUR_BRACE
	|
	O_CUR_BRACE C_CUR_BRACE
	;

var_decls:
	var_decls var_decl
	|
	var_decl
	;

var_decl:
	type ids SEMICOLON
	;

ids:
	ids COMMA IDENTIFIER
	|
	IDENTIFIER
	;

statements:
	statements statement
	|
	statement
	;

statement:
	block
	|
	location assign_op expr SEMICOLON																		{ bison_outfile << "ASSIGNMENT OPERATION ENCOUNTERED" << std::endl; }
	|
	method_call SEMICOLON
	|
	IF O_PAREN expr C_PAREN block ELSE block
	|
	IF O_PAREN expr C_PAREN block
	|
	FOR IDENTIFIER EQUAL expr COMMA expr block
	|
	RETURN expr SEMICOLON
	|
	RETURN SEMICOLON
	|
	BREAK SEMICOLON
	|
	CONTINUE SEMICOLON
	;

assign_op:
	EQUAL
	|
	PLUS_EQUAL
	|
	MINUS_EQUAL
	;

location:
	IDENTIFIER																												{ bison_outfile << "LOCATION ENCOUNTERED=" << $1 << std::endl; }
	|
	IDENTIFIER O_BRACE expr C_BRACE																		{ bison_outfile << "LOCATION ENCOUNTERED=" << $1 << std::endl; }
	;

method_call:
	IDENTIFIER O_PAREN exprs C_PAREN
	|
	IDENTIFIER O_PAREN C_PAREN
	|
	CALLOUT O_PAREN STRING_LITERAL C_PAREN														{ bison_outfile << "CALLOUT TO " << $3 << " ENCOUNTERED" << std::endl; }
	|
	CALLOUT O_PAREN STRING_LITERAL COMMA callout_args C_PAREN					{ bison_outfile << "CALLOUT TO " << $3 << " ENCOUNTERED" << std::endl; }

callout_args:
	callout_args COMMA callout_arg
	|
	callout_arg

callout_arg:
	expr
	|
	STRING_LITERAL

exprs:
	exprs COMMA expr
	|
	expr
	;

expr:
	location
	|
	method_call
	|
	literal
	|
	O_PAREN expr C_PAREN
	|
	expr OP_PLUS expr																									{ bison_outfile << "ADDITION ENCOUNTERED" << std::endl; }
	|
	expr OP_MINUS expr																								{ bison_outfile << "SUBTRACTION ENCOUNTERED" << std::endl; }
	|
	expr OP_MULTIPLY expr																							{ bison_outfile << "MULTIPLICATION ENCOUNTERED" << std::endl; }
	|
	expr OP_DIVIDE expr																								{ bison_outfile << "DIVISION ENCOUNTERED" << std::endl; }
	|
	expr OP_MODULO expr																								{ bison_outfile << "MOD ENCOUNTERED" << std::endl; }
	|
	expr OP_LESS_THAN expr																						{ bison_outfile << "LESS THAN ENCOUNTERED" << std::endl; }
	|
	expr OP_GREATER_THAN expr																					{ bison_outfile << "GREATER THAN ENCOUNTERED" << std::endl; }
	|
	expr OP_LESS_EQUAL expr
	|
	expr OP_GREATER_EQUAL expr
	|
	expr OP_EQUAL_EQUAL expr
	|
	expr OP_NOT_EQUAL expr
	|
	expr OP_LOGICAL_OR expr
	|
	expr OP_LOGICAL_AND expr
	|
	OP_NOT expr
	|
	OP_MINUS expr %prec OP_UMINUS
	;

literal:
	INT_LITERAL																												{ bison_outfile << "INT ENCOUNTERED=" << $1 << std::endl; }
	|
	CHAR_LITERAL																											{ bison_outfile << "CHAR ENCOUNTERED=" << $1 << std::endl; }
	|
	TRUE																															{ bison_outfile << "BOOLEAN ENCOUNTERED=true" << std::endl; }
	|
	FALSE																															{ bison_outfile << "BOOLEAN ENCOUNTERED=false" << std::endl; }
	;

type:
	TYPE_INTEGER
	|
	TYPE_BOOLEAN
	;

%%

void yyerror(const char *s){
	std::cerr << s << std::endl;
	std::cerr << "Syntax error at " << yylineno << std::endl;
	exit(-1);
}
