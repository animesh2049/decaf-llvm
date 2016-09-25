%{
#include <bits/stdc++.h>
using namespace std;

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
extern int yylineno;
extern fstream flex_outfile;
extern fstream bison_outfile;

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

%left 	<sval> OP_NOT

%token 	<sval> IDENTIFIER
%token 	<sval> INT_LITERAL
%token 	<sval> STRING_LITERAL
%token 	<sval> CHAR_LITERAL

%%

program:
	CLASS IDENTIFIER O_CUR_BRACE field_decls method_decls C_CUR_BRACE
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
	type IDENTIFIER O_PAREN method_id_List C_PAREN block
	|
	VOID IDENTIFIER O_PAREN method_id_List C_PAREN block
	;

method_id_List:
	method_id_List COMMA type IDENTIFIER
	|
	type IDENTIFIER
	;

block:
	O_CUR_BRACE var_decls statements C_CUR_BRACE
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
	RETURN SEMICOLON
	|
	RETURN expr SEMICOLON
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
	O_PAREN expr C_PAREN
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

int main(int argc, char *argv[]){
	if(argc != 2){
		yyerror("Usage: parser inputfilepath\n");
	}
	FILE *myfile = fopen(argv[1], "r");
	if(!myfile){
		cout << "Unable to open file" << endl;
		return -1;
	}
	flex_outfile.open("flex_outfile.txt",std::ios_base::out);
	bison_outfile.open("bison_outfile.txt",std::ios_base::out);
	while(!feof(yyin)){
		yyparse();
	}
	cout << "Success" << endl;
}

void yyerror(const char *s){
	cout << "Syntax error at " << yylineno << endl;
	exit(-1);
}
