%{
#include <iostream>
#include <vector>
#include <string>
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
	int ival;
	char cval;
	Program *program;
	Field_Decl *field_decl;
	std::vector<Field_Decl *> *field_decl_list;
	Method_Decl *method_decl;
	std::vector<Method_Decl *> *method_decl_list;
	Var_Decl *var_decl;
	std::vector<Var_Decl *> *var_decl_list;
	Identifier *identifier;
	std::vector<Identifier *> *identifier_list;
	Statement *statement;
	std::vector<Statement *> *statement_list;
	Expression *expr;
	std::vector<Expression *> *expr_list;
	Data_Type *type;
	Arr_Identifier *arr_identifier;
	std::vector<Arr_Identifier *> *arr_identifier_list;
	Assign_Op *assign_op;
	Callout_Arg *callout_arg;
	std::vector<Callout_Arg *> *callout_arg_list;
	Literal *literal;
	Location *location;
	Method_Call *method_call;
	Block *block;
}


%token	<sval> BREAK
%token	<sval> CALLOUT
%token	<sval> CLASS
%token	<sval> CONTINUE%token	<sval> ELSE
%token	<sval> FALSE
%token	<sval> FOR
%token	<sval> IF
%token	<sval> RETURN
%token	<sval> TRUE
%token	<sval> VOID
%token	<sval> COMMA

%token	<sval> TYPE_BOOLEAN
%token	<sval> TYPE_INTEGER

%token	<sval> O_CUR_BRACE
%token	<sval> C_CUR_BRACE
%token	<sval> O_PAREN
%token	<sval> C_PAREN
%token	<sval> SEMICOLON
%token	<sval> O_BRACE
%token	<sval> C_BRACE

%left 	<sval> OP_LOGICAL_OR OP_LOGICAL_AND

%token 	<sval> EQUAL PLUS_EQUAL MINUS_EQUAL
%left 	<sval> OP_NOT_EQUAL OP_EQUAL_EQUAL

%nonassoc <sval> OP_LESS_THAN OP_GREATER_THAN OP_LESS_EQUAL OP_GREATER_EQUAL

%left 	<sval> OP_PLUS OP_MINUS

%left 	<sval> OP_MULTIPLY OP_DIVIDE OP_MODULO

%precedence OP_NOT OP_UMINUS

%token	<sval> IDENTIFIER
%token	<sval> INT_LITERAL
%token	<sval> STRING_LITERAL
%token	<sval> CHAR_LITERAL

%type	<field_decl> 					field_decl
%type	<field_decl_list>			field_decl_list
%type	<method_decl>					method_decl
%type	<method_decl_list>		method_decl_list
%type	<identifier>					identifier
%type	<identifier_list>			identifier_list
%type	<var_decl>						var_decl
%type	<var_decl_list>				var_decl_list
%type	<statement>						statement
%type	<statement_list>			statement_list
%type	<expr>								expr
%type	<expr_list>						expr_list
%type	<arr_identifier>			arr_identifier
%type	<arr_identifier_list>	arr_identifier_list
%type	<callout_arg>					callout_arg
%type	<callout_arg_list>		callout_arg_list
%type	<block>								block
%type	<assign_op>						assign_op
%type	<program>							program
%type	<type>								type
%type	<literal>							literal
%type	<location>						location
%type	<method_call>					method_call


%%

program:
	CLASS IDENTIFIER O_CUR_BRACE field_decls method_decls C_CUR_BRACE
	|
	CLASS IDENTIFIER O_CUR_BRACE field_decls C_CUR_BRACE
	|
	CLASS IDENTIFIER O_CUR_BRACE method_decls C_CUR_BRACE
	|
	CLASS IDENTIFIER O_CUR_BRACE C_CUR_BRACE
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
	type id_all_list SEMICOLON
	;

id_all_list:
	id_all_list COMMA IDENTIFIER
	|
	id_all_list COMMA id_array
	|
	IDENTIFIER
	|
	id_array
	;

id_array:
	IDENTIFIER O_BRACE INT_LITERAL C_BRACE
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
	location assign_op expr SEMICOLON
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
	IDENTIFIER
	|
	IDENTIFIER O_BRACE expr C_BRACE
	;

method_call:
	IDENTIFIER O_PAREN exprs C_PAREN
	|
	IDENTIFIER O_PAREN C_PAREN
	|
	CALLOUT O_PAREN STRING_LITERAL C_PAREN
	|
	CALLOUT O_PAREN STRING_LITERAL COMMA callout_args C_PAREN

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
	expr OP_PLUS expr
	|
	expr OP_MINUS expr
	|
	expr OP_MULTIPLY expr
	|
	expr OP_DIVIDE expr
	|
	expr OP_MODULO expr
	|
	expr OP_LESS_THAN expr
	|
	expr OP_GREATER_THAN expr
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
	INT_LITERAL
	|
	CHAR_LITERAL
	|
	TRUE
	|
	FALSE
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
