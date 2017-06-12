%{
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <string.h>
#include <vector>
#include "classes.h"

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
extern int yylineno;
extern std::fstream flex_outfile;
extern std::fstream bison_outfile;
extern Program *start;

void yyerror(const char *s);
%}

%union {
	char* sval;
	int ival;
	char cval;
	Program *program;
	Field_Decl *field_decl;
	std::vector<Field_Decl *> *field_decls;
	Method_Decl *method_decl;
	std::vector<Method_Decl *> *method_decls;
	Var_Decl *var_decl;
	std::vector<Var_Decl *> *var_decls;
	std::vector<Var_Identifier *> *identifiers;
	Statement *statement;
	std::vector<Statement *> *statements;
	Expression *expr;
	std::vector<Expression *> *expressions;
	Data_Type type;
	Arr_Identifier *arr_identifier;
	std::vector<Arr_Identifier *> *arr_identifiers;
	Assign_Op assign_op;
	Callout_Arg *callout_arg;
	std::vector<Callout_Arg *> *callout_args;
	Type_Identifier *type_identifier;
	std::vector<Type_Identifier *> *type_identifiers;
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
%token	<sval> TYPE_VOID

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

%precedence <sval> OP_NOT OP_UMINUS

%token	<sval> IDENTIFIER
%token	<ival> INT_LITERAL
%token	<sval> STRING_LITERAL
%token	<cval> CHAR_LITERAL

%type	<field_decl> 					field_decl
%type	<field_decls>					field_decls
%type	<method_decl>					method_decl
%type	<method_decls>				method_decls
%type	<identifiers>					identifiers
%type	<arr_identifier>			arr_identifier
%type	<arr_identifiers>			arr_identifiers
%type	<type_identifier>			type_identifier
%type <type_identifiers>		type_identifiers
%type	<var_decl>						var_decl
%type	<var_decls>						var_decls
%type	<statement>						statement
%type	<statements>					statements
%type	<expr>								expr
%type	<expressions>					expressions
%type	<callout_arg>					callout_arg
%type	<callout_args>				callout_args
%type	<block>								block
%type	<assign_op>						assign_op
%type	<program>							program
%type	<type>								type
%type	<literal>							literal
%type	<location>						location
%type	<method_call>					method_call

%%

program:
	CLASS IDENTIFIER O_CUR_BRACE field_decls method_decls C_CUR_BRACE	{$$ = new Program(std::string($2), $4, $5); start = $$;}
	|
	CLASS IDENTIFIER O_CUR_BRACE field_decls C_CUR_BRACE	{$$ = new Program(std::string($2), $4, NULL); start = $$;}
	|
	CLASS IDENTIFIER O_CUR_BRACE method_decls C_CUR_BRACE	{$$ = new Program(std::string($2), NULL, $4); start = $$;}
	|
	CLASS IDENTIFIER O_CUR_BRACE C_CUR_BRACE	{$$ = new Program(std::string($2), NULL, NULL); start = $$;}
	;

field_decls:
	field_decl	{$$ = new std::vector<Field_Decl *>(); $$->push_back($1);}
	|
	field_decls field_decl	{$1->push_back($2); $$ = $1;}
	;

field_decl:
	type identifiers SEMICOLON	{$$ = new Field_Decl($1, $2);}
	|
	type arr_identifiers SEMICOLON	{$$ = new Field_Decl($1, $2);}
	|
	type identifiers COMMA arr_identifiers SEMICOLON	{$$ = new Field_Decl($1, $2, $4);}
	|
	type arr_identifiers COMMA identifiers SEMICOLON	{$$ = new Field_Decl($1, $4, $2);}
	;

identifiers:
	IDENTIFIER	{$$ = new std::vector<Var_Identifier *>(); $$->push_back(new Var_Identifier(std::string($1)));}
	|
	identifiers COMMA IDENTIFIER	{$1->push_back(new Var_Identifier(std::string($3))); $$ = $1;}
	;

arr_identifiers:
	arr_identifier	{$$ = new std::vector<Arr_Identifier *>(); $$->push_back($1);}
	|
	arr_identifiers COMMA arr_identifier	{$1->push_back($3); $$ = $1;}
	;

arr_identifier:
	IDENTIFIER O_BRACE INT_LITERAL C_BRACE	{$$ = new Arr_Identifier(std::string($1), $3);}
	;

method_decls:
	method_decl	{$$ = new std::vector<Method_Decl *>(); $$->push_back($1);}
	|
	method_decls method_decl	{$1->push_back($2); $$ = $1;}
	;

method_decl:
	type IDENTIFIER O_PAREN type_identifiers C_PAREN block	{$$ = new Method_Decl(std::string($2), $1, $4, $6);}
	|
	type IDENTIFIER O_PAREN C_PAREN block	{$$ = new Method_Decl(std::string($2), $1, NULL, $5);}
	;

var_decls:
	var_decl	{$$ = new std::vector<Var_Decl *>(); $$->push_back($1);}
	|
	var_decls var_decl	{$1->push_back($2); $$ = $1;}
	;

var_decl:
	type identifiers SEMICOLON	{$$ = new Var_Decl($1, $2);}
	|
	type arr_identifiers SEMICOLON	{$$ = new Var_Decl($1, $2);}
	;

type_identifiers:
	type_identifier	{$$ = new std::vector<Type_Identifier *>(); $$->push_back($1);}
	|
	type_identifiers COMMA type_identifier	{$1->push_back($3); $$ = $1;}
	;

type_identifier :
	type IDENTIFIER	{$$ = new Type_Identifier(std::string($2), $1);}
	;

block:
	O_CUR_BRACE var_decls statements C_CUR_BRACE	{$$ = new Block($2, $3);}
	|
	O_CUR_BRACE statements C_CUR_BRACE	{$$ = new Block(NULL, $2);}
	|
	O_CUR_BRACE var_decls C_CUR_BRACE	{$$ = new Block($2, NULL);}
	|
	O_CUR_BRACE C_CUR_BRACE	{$$ = new Block(NULL, NULL);}
	;

statements:
	statement	{$$ = new std::vector<Statement *>(); $$->push_back($1);}
	|
	statements statement	{$1->push_back($2); $$ = $1;}
	;

statement:
	block	{$$ = $1;}
	|

	location assign_op expr SEMICOLON	{$$ = new Assign_Statement($2, $1, $3);}
	|
	method_call SEMICOLON	{$$ = $1;}
	|
	IF O_PAREN expr C_PAREN block ELSE block	{$$ = new If_Statement($3, $5, $7);}
	|
	IF O_PAREN expr C_PAREN block	{$$ = new If_Statement($3, $5, NULL);}
	|
	FOR IDENTIFIER EQUAL expr COMMA expr block	{$$ = new For_Statement(std::string($2), $4, $6, $7);}
	|
	RETURN expr SEMICOLON	{$$ = new Return_Statement($2);}
	|
	RETURN SEMICOLON	{$$ = new Return_Statement(NULL);}
	|
	BREAK SEMICOLON	{$$ = new Break_Statement();}
	|
	CONTINUE SEMICOLON	{$$ = new Continue_Statement();}
	|
	SEMICOLON	{$$ = NULL;}
	;

assign_op:
	EQUAL	{$$ = Assign_Op::op_equal;}
	|
	PLUS_EQUAL	{$$ = Assign_Op::op_plus_equal;}
	|
	MINUS_EQUAL	{$$ = Assign_Op::op_minus_equal;}
	;

location:
	IDENTIFIER	{$$ = new Var_Location(std::string($1));}
	|
	IDENTIFIER O_BRACE expr C_BRACE	{$$ = new Arr_Location(std::string($1), $3);}
	;

method_call:
	IDENTIFIER O_PAREN expressions C_PAREN	{$$ = new Normal_Method(std::string($1), $3);}
	|
	IDENTIFIER O_PAREN C_PAREN	{$$ = new Normal_Method(std::string($1), NULL);}
	|

	CALLOUT O_PAREN STRING_LITERAL C_PAREN	{$$ = new Callout_Method(std::string($3), NULL);}
	|
	CALLOUT O_PAREN STRING_LITERAL COMMA callout_args C_PAREN	{$$ = new Callout_Method(std::string($3), $5);}

callout_args:
	callout_arg	{$$ = new std::vector<Callout_Arg *>(); $$->push_back($1);}
	|
	callout_args COMMA callout_arg	{$1->push_back($3); $$ = $1;}
	;

callout_arg:
	expr	{$$ = new Expression_Callout_Arg($1);}
	|
	STRING_LITERAL	{$$ = new String_Callout_Arg(std::string($1));}

expressions:
	expr	{$$ = new std::vector<Expression *>(); $$->push_back($1);}
	|
	expressions COMMA expr	{$1->push_back($3); $$ = $1;}
	;

expr:
	location	{$$ = $1;}
	|
	method_call	{$$ = $1;}
	|
	literal	{$$ = $1;}
	|
	O_PAREN expr C_PAREN	{$$ = $2;}
	|
	expr OP_PLUS expr	{$$ = new Binary_Operation_Expression($1, Bin_Op::op_plus, $3);}
	|
	expr OP_MINUS expr	{$$ = new Binary_Operation_Expression($1, Bin_Op::op_minus, $3);}
	|
	expr OP_MULTIPLY expr	{$$ = new Binary_Operation_Expression($1, Bin_Op::op_multiply, $3);}
	|
	expr OP_DIVIDE expr	{$$ = new Binary_Operation_Expression($1, Bin_Op::op_divide, $3);}
	|
	expr OP_MODULO expr	{$$ = new Binary_Operation_Expression($1, Bin_Op::op_modulo, $3);}
	|
	expr OP_LESS_THAN expr	{$$ = new Binary_Operation_Expression($1, Bin_Op::op_less_than, $3);}
	|
	expr OP_GREATER_THAN expr	{$$ = new Binary_Operation_Expression($1, Bin_Op::op_greater_than, $3);}
	|
	expr OP_LESS_EQUAL expr	{$$ = new Binary_Operation_Expression($1, Bin_Op::op_less_equal, $3);}
	|
	expr OP_GREATER_EQUAL expr	{$$ = new Binary_Operation_Expression($1, Bin_Op::op_greater_equal, $3);}
	|
	expr OP_EQUAL_EQUAL expr	{$$ = new Binary_Operation_Expression($1, Bin_Op::op_equal_equal, $3);}
	|
	expr OP_NOT_EQUAL expr	{$$ = new Binary_Operation_Expression($1, Bin_Op::op_not_equal, $3);}
	|
	expr OP_LOGICAL_OR expr	{$$ = new Binary_Operation_Expression($1, Bin_Op::op_logical_or, $3);}
	|
	expr OP_LOGICAL_AND expr	{$$ = new Binary_Operation_Expression($1, Bin_Op::op_logical_and, $3);}
	|
	OP_NOT expr	{$$ = new Unary_Operation_Expression(Unary_Op::op_not, $2);}
	|
	OP_MINUS expr %prec OP_UMINUS	{$$ = new Unary_Operation_Expression(Unary_Op::op_minus, $2);}
	;

literal:
	INT_LITERAL	{$$ = new Int_Literal($1);}
	|
	CHAR_LITERAL	{$$ = new Char_Literal($1);}
	|
	TRUE	{$$ = new Bool_Literal($1);}
	|
	FALSE	{$$ = new Bool_Literal($1);}
	;

type:
	TYPE_INTEGER	{$$ = Data_Type::int_type;}
	|
	TYPE_BOOLEAN	{$$ = Data_Type::bool_type;}
	|
	TYPE_VOID	{$$ = Data_Type::void_type;}
	;

%%

void yyerror(const char *s){
	std::cerr << s << std::endl;
	std::cerr << "Syntax error at " << yylineno << std::endl;
	exit(-1);
}
