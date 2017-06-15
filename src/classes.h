#ifndef _CLASSES_H
#define _CLASSES_H

#include <vector>
#include <string>

class Program;
class Field_Decl;
class Var_Decl;
class Method_Decl;
class Identifier;
class Var_Identifier;
class Arr_Identifier;
class Type_Identifier;
class Statement;
class Expression;
class Callout_Arg;
class Literal;
class Location;
class Var_Location;
class Arr_Location;
class Method_Call;
class Block;
class Statement;
class For_Statement;
class If_Statement;
class Return_Statement;
class Continue_Statement;
class Break_Statement;
class Normal_Method;
class Callout_Method;


enum class Bin_Op : char {
  op_plus,
  op_minus,
  op_multiply,
  op_divide,
  op_modulo,
  op_greater_than,
  op_less_than,
  op_less_equal,
  op_greater_equal,
  op_logical_or,
  op_logical_and,
  op_equal_equal,
  op_not_equal
};

enum class Data_Type : char {
  int_type,
  void_type,
  bool_type,
};

enum class Assign_Op : char {
  op_plus_equal,
  op_minus_equal,
  op_equal,
};

enum class Unary_Op : char {
  op_minus,
  op_not,
};

class Identifier {
public:
  Identifier(){}
  ~Identifier(){}
  virtual void print()=0;
};

class Var_Identifier : public Identifier {
  std::string id;
public:
  Var_Identifier(std::string id){
    this->id = id;
  }
  std::string get_id(){
    return this->id;
  }
  void print(){
    std::cout << "<identifier name=\"" << this->id << "\">" << std::endl;
  }
  ~Var_Identifier(){}
};

class Arr_Identifier : public Identifier { // Why can't we just inherit Var_Identifier and
  std::string id;                          // overide the consturctor and create new function get_size()
  int size;
public:
  Arr_Identifier(std::string id, int size){
    this->id = id;
    this->size = size;
  }
  std::string get_id() {
    return this->id;
  }
  int get_size(){
    return this->size;
  }
  void print(){
    std::cout << "<arr_identifier name=\"" << this->id << "\" size=\"" << this->size << "\">" << std::endl;
  }
  ~Arr_Identifier(){}
};

class Type_Identifier {
  std::string id;
  Data_Type type;
public:
  Type_Identifier(std::string(id), Data_Type type){
    this->id = id;
    this->type = type;
  }
  std::string get_id(){
    return this->id;
  }
  std::string get_type(){
    switch (this->type) {
      case Data_Type::int_type:
        return "int";
      case Data_Type::void_type:
        return "void";
      case Data_Type::bool_type:
        return "bool";
    }
  }

  void print(){
    std::cout << "<type_identifier type=\"" << this->get_type() << "\" name=\"" << this->id << "\">" << std::endl;
  }

};

class Var_Decl {
  Data_Type type;
  std::vector<Var_Identifier *> *identifier;
  std::vector<Arr_Identifier *>    *arr_identifier;
public:
  Var_Decl(Data_Type type, std::vector<Var_Identifier *> *identifier) {
      if(identifier) this->identifier = identifier;
      else this->identifier = new std::vector<Var_Identifier *>();
      this->arr_identifier = new std::vector<Arr_Identifier *>();
      this->type = type;
    }
  Var_Decl(Data_Type type, std::vector<Arr_Identifier *> *arr_identifier) {
    if(arr_identifier)  this->arr_identifier = arr_identifier;
    else  this->arr_identifier = new std::vector<Arr_Identifier *>();
    this->identifier = new std::vector<Var_Identifier *>();
    this->type = type;
  }
  Var_Decl(Data_Type type, std::vector<Var_Identifier *> *identifier,
    std::vector<Arr_Identifier *> *arr_identifier) {
      if(identifier) this->identifier = identifier;
      else this->identifier = new std::vector<Var_Identifier *>();
      if (arr_identifier) this->arr_identifier = arr_identifier;
      else this->arr_identifier = new std::vector<Arr_Identifier *>();
      this->type = type;
    }
  std::vector<Var_Identifier *> *get_id_list(){
    return this->identifier;
  }
  std::vector<Arr_Identifier *> *get_arr_id_list() {
    return this->arr_identifier;
  }
  std::string get_type(){
    switch (this->type) {
      case Data_Type::int_type:
        return "int";
      case Data_Type::void_type:
        return "void";
      case Data_Type::bool_type:
        return "bool";
      default :
        std::cout << "This data type is not supported" << std::endl;
        exit(3);
    }
  }
  void print(){
    std::cout << "<variable_declaration type=\"" << this->get_type() << "\" normal_count=\"" << this->identifier->size() << "\" arr_count=\"" << this->arr_identifier->size() << "\">" << std::endl;
    std::cout << "<normal_variables>" << std::endl;
    for(auto it : *identifier) {
      it->print();
    }
    std::cout << "</normal_variables>" << std::endl;
    std::cout << "<arr_variables>" << std::endl;
    for(auto it : *arr_identifier) {
       it->print();
    }
    std::cout << "</arr_variables>" << std::endl;
  }
};

class Statement {
public:
  Statement() {}
  ~Statement() {}
  virtual void print()=0;
};

class Expression {
public:
  Expression(){}
  ~Expression(){}
  virtual void print() = 0;
};

class Block : public Statement {
	std::vector<Var_Decl *>* identifier_list;
	std::vector<Statement *>* statement_list;
public:
	Block (std::vector<Var_Decl *>* identifier_list, std::vector<Statement *>* statement_list) {
    if(identifier_list)
		  this->identifier_list = identifier_list;
    else
      this->identifier_list = new std::vector<Var_Decl *>();
    if(statement_list)
      this->statement_list = statement_list;
    else
      this->statement_list = new std::vector<Statement *>();
	}
	std::vector<Var_Decl *> *get_identifier_list() {
		return this->identifier_list;
	}
	std::vector<Statement *> *get_statement_list() {
		return this->statement_list;
	}
  void print(){
    std::cout << "<block>" << std::endl;
    std::cout << "<variable_declarations count=\"" << this->identifier_list->size() << "\">" << std::endl;
    for(auto it : *identifier_list) {
      it->print();
    }
    std::cout << "</variable_declarations>" << std::endl;
    std::cout << "<statement_declartation count=\"" << this->statement_list->size() << "\">" << std::endl;
    for(auto it : *statement_list){
      it->print();
    }
    std::cout << "</statement_declartation>" << std::endl;
    std::cout << "</block>" << std::endl;
  }
};

class If_Statement : public Statement {
	Expression* condition;
	Statement* if_block;
	Statement* else_block;
public:
	If_Statement (Expression* condition, Statement* if_block, Statement* else_block) {
		this->condition = condition;
		this->if_block = if_block;
	  this->else_block = else_block;
	}
	Expression* get_condition() {
		return this->condition;
	}

	Statement* get_if_block() {
		return this->if_block;
	}

	Statement* get_else_block() {
		return this->else_block;
	}
  void print(){
    std::cout << "<if_statement condition=\"" << this->condition << "\">" << std::endl;
    this->if_block->print();
    if (else_block) this->else_block->print();
    std::cout << "</if_statement>" << std::endl;
  }
};

class Location : public Expression {
public:
  Location(){}
  ~Location(){}
  virtual void print()=0;
};

class Var_Location : public Location {
  std::string id;
public:
  Var_Location(std::string id) {
    this->id = id;
  }
  std::string get_id(){
    return this->id;
  }
  void print(){
    std::cout << "<location id=\"" << this->id << "\">" << std::endl;
  }
};

class Arr_Location : public Location {
	std::string id;
	Expression* index;
public:
	Arr_Location (std::string id, Expression* index) {
		this->id = id;
		this->index = index;
	}
	std::string get_id() {
		return this->id;
	}
	Expression* get_index() {
		return this->index;
	}
  void print(){
    std::cout << "<location id=\"" << this->id << "\">" << std::endl;
    std::cout << "<position>" << std::endl;
    this->index->print();
    std::cout << "</position>" << std::endl;
    std::cout << "</location>" << std::endl;
  }
};

class Binary_Operation_Expression : public Expression {
  Expression *left;
  Expression *right;
  Bin_Op operation;
public:
  Binary_Operation_Expression(Expression *left, Bin_Op operation,
    Expression *right) {
      this->left = left;
      this->right = right;
      this->operation = operation;
    }
    Expression* get_left_expr(){
      return this->left;
    }
    Expression* get_right_expr(){
      return this->right;
    }
    std::string get_binop(){
      switch (this->operation) {
        case Bin_Op::op_plus:
          return "+";
        case Bin_Op::op_minus:
          return "-";
        case Bin_Op::op_multiply:
          return "*";
        case Bin_Op::op_divide:
          return "/";
        case Bin_Op::op_modulo:
          return "%";
        case Bin_Op::op_greater_than:
          return ">";
        case Bin_Op::op_less_than:
          return "<";
        case Bin_Op::op_less_equal:
          return "<=";
        case Bin_Op::op_greater_equal:
          return ">=";
        case Bin_Op::op_logical_or:
          return "||";
        case Bin_Op::op_logical_and:
          return "&&";
        case Bin_Op::op_equal_equal:
          return "==";
        case Bin_Op::op_not_equal:
          return "!=";
      }
    }
    void print(){
      std::string type;
      switch (this->operation) {
        case Bin_Op::op_plus:
          type = "+"; break;
        case Bin_Op::op_minus:
          type = "-"; break;
        case Bin_Op::op_multiply:
          type = "*"; break;
        case Bin_Op::op_divide:
          type = "/"; break;
        case Bin_Op::op_modulo:
          type = "%"; break;
        case Bin_Op::op_greater_than:
          type = ">"; break;
        case Bin_Op::op_less_than:
          type = "<"; break;
        case Bin_Op::op_less_equal:
          type = "<="; break;
        case Bin_Op::op_greater_equal:
          type = ">="; break;
        case Bin_Op::op_logical_or:
          type = "||"; break;
        case Bin_Op::op_logical_and:
          type = "&&"; break;
        case Bin_Op::op_equal_equal:
          type = "=="; break;
        case Bin_Op::op_not_equal:
          type = "!="; break;
      }
      std::cout << "<binary_expression type=\"" << type << "\">" << std::endl;
      left->print();
      right->print();
      std::cout << "</binary_expression>" << std::endl;
    }
};

class Unary_Operation_Expression : public Expression {
	Expression* expr;
	Unary_Op operation;
public:
	Unary_Operation_Expression (Unary_Op operation, Expression* expr) {
		this->expr = expr;
		this->operation = operation;
	}
	Expression* get_expr() {
		return this->expr;
	}
	std::string get_operation() {
    switch (this->operation) {
      case Unary_Op::op_minus:
        return "-";
      case Unary_Op::op_not:
        return "!";
    }
	}
  void print(){
    std::string type;
    switch (this->operation) {
      case Unary_Op::op_minus:
        type = "-"; break;
      case Unary_Op::op_not:
        type = "!"; break;
    }
    std::cout << "unary_expression type=\"" << type << "\">" << std::endl;
    this->expr->print();
    std::cout << "</unary_expression>" << std::endl;
  }
};

class For_Statement : public Statement {
	std::string id;
	Expression* init_condition;
	Expression* end_condition;
	Block* block;
public:
	For_Statement(std::string id, Expression* init_condition,
    Expression* end_condition, Block* block) {
		this->id = id;
		this->init_condition = init_condition;
		this->end_condition = end_condition;
		this->block = block;
  }
	std::string get_id() {
		return this->id;
	}
	Expression* get_init_condition() {
		return this->init_condition;
	}
	Expression* get_end_condition() {
		return this->end_condition;
	}
	Block* get_block() {
		return this->block;
	}
  void print() {
    std::cout << "<for_statement variable=\"" << this->id << "\">" << std::endl;
    this->block->print();
    std::cout << "</for_statement>" << std::endl;
  }
};

class Assign_Statement : public Statement {
  Assign_Op operation;
  Location *location;
  Expression *expression;
public:
  Assign_Statement(Assign_Op operation, Location *location,
     Expression *expression){
       this->operation = operation;
       this->location = location;
       this->expression = expression;
     }
  Assign_Op get_operation(){
    return this->operation;
  }
  Location *get_location(){
    return this->location;
  }
  Expression *get_expression(){
    return this->expression;
  }
  void print() {
    std::cout << "<assignment>" << std::endl;
    std::cout << "<location>" << std::endl;
    this->location->print();
    std::cout << "</assignment>" << std::endl;
  }
};

class Return_Statement : public Statement {
	Expression* expression;
public:
	Return_Statement (Expression* expression) {
		this->expression = expression;
	}
	Expression* get_expression() {
		return this->expression;
	}
  void print(){}
};

class Continue_Statement : public Statement {
public:
	Continue_Statement () {}
  ~Continue_Statement() {}
  void print(){}
};

class Break_Statement : public Statement {
public:
	Break_Statement() {}
  ~Break_Statement() {}
  void print(){}
};

class Literal : public Expression {
public:
	Literal () {}
  void print(){}
};

class Int_Literal : public Literal {
	int value;
public:
	Int_Literal (int value) {
		this->value = value;
	}
	int get_value() {
		return this->value;
	}
  void print(){
    std::cout << "<integer value=\"" << this->value << "\">" << std::endl;
  }
};

class Char_Literal : public Literal {
	char value;
public:
	Char_Literal (char value) {
		this->value = value;
	}
	char get_value() {
		return this->value;
	}
  void print(){
    std::cout << "<character value=\"" << this->value << "\">" << std::endl;
  }
};

class Bool_Literal : public Literal {
	bool value;
public:
	Bool_Literal (bool value) {
		this->value = value;
	}
	bool get_value() {
		return this->value;
	}
  void print(){
    std::cout << "<boolean value=\"" << this->value << "\">" << std::endl;
  }
};

class Callout_Arg {
public:
	Callout_Arg () {
	}
  virtual void print()=0;
};

class String_Callout_Arg : public Callout_Arg {
	std::string argument;
public:
	String_Callout_Arg (std::string argument) {
		this->argument = argument;
	}
	std::string get_argument() {
		return this->argument;
	}
  void print() {
    std::cout << "<callout_argument name=\"" << this->argument << "\">" << std::endl;
  }
};

class Expression_Callout_Arg : public Callout_Arg {
	Expression *argument;
public:
	Expression_Callout_Arg (Expression *argument) {
		this->argument = argument;
	}
	Expression *get_argument() {
		return this->argument;
	}
  void print(){
    this->argument->print();
  }
};

class Method_Call : public Statement, public Expression {
public:
  Method_Call(){}
  ~Method_Call(){}
  virtual void print()=0;
};

class Normal_Method : public Method_Call {
  std::string id;
  std::vector<Expression *> * arguments;
public:
  Normal_Method(std::string id, std::vector<Expression *> * arguments) {
    this->id = id;
    this->arguments = arguments;
  }
  std::string getId() {
    return this->id;
  }
  std::vector<Expression *> * get_arguments() {
    return this->arguments;
  }
  void print(){
    std::cout << "<method_call id=\"" << this->id << "\" argument_count=\"" << this->arguments->size() << "\">" << std::endl;
    for(auto it : *arguments){
      it->print();
    }
    std::cout << "</method_call>" << std::endl;
  }
};

class Callout_Method : public Method_Call {
  std::string id;
  std::vector<Callout_Arg *> * arguments;
public:
  Callout_Method(std::string id, std::vector<Callout_Arg *> * arguments) {
    this->id = id;
    this->arguments = arguments;
  }
  std::string getId() {
    return this->id;
  }
  std::vector<Callout_Arg *> * get_arguments() {
    return this->arguments;
  }
  void print() {
    std::cout << "<callout id=\"" << this->id << "\" argument_count=\"" << this->arguments->size() << "\">" << std::endl;
    for(auto it : *arguments){
      it->print();
    }
    std::cout << "</callout>" << std::endl;
  }
};

class Method_Decl {
  std::string id;
  Data_Type return_type;
  std::vector<Type_Identifier *> *arguments;
  Block *my_block;
public:
  Method_Decl(std::string id, Data_Type return_type,
    std::vector<Type_Identifier *> *arguments, Block *my_block){
      this->id = id;
      this->return_type = return_type;
      if(arguments)
        this->arguments = arguments;
      else
        this->arguments = new std::vector<Type_Identifier *>();
      this->my_block = my_block;
    }
  std::string get_id(){
    return this->id;
  }
  std::string get_return_type(){
    switch (this->return_type) {
      case Data_Type::int_type:
        return "int";
      case Data_Type::void_type:
        return "void";
      case Data_Type::bool_type:
        return "bool";
    }
  }
  std::vector<Type_Identifier *> *get_arguments(){
    return this->arguments;
  }
  Block *get_block(){
    return this->my_block;
  }
  void print() {
    std::string test;
    switch (this->return_type) {
      case Data_Type::int_type:
        test = "int"; break;
      case Data_Type::void_type:
        test = "void"; break;
      case Data_Type::bool_type:
        test = "bool"; break;
    }
    std::cout << "<method_delcaration name=\"" << this->id << "\" return_type=\"" << test << "\" args_count=\"" << this->arguments->size() << "\">" << std::endl;
    this->my_block->print();
    std::cout << "</method_delcaration>" << std::endl;
  }
  ~Method_Decl(){}
};

class Field_Decl {
  Data_Type data_type;
  std::vector<Var_Identifier *> *identifier;
  std::vector<Arr_Identifier *> *arr_identifier;
public:
  Field_Decl(Data_Type data_type, std::vector<Var_Identifier *> *identifier){
    this->data_type = data_type;
    if(identifier) this->identifier = identifier;
    else  this->identifier = new std::vector<Var_Identifier *>();
    if(arr_identifier) this->arr_identifier = arr_identifier;
    else  this->arr_identifier = new std::vector<Arr_Identifier *>();
  }
  Field_Decl(Data_Type data_type, std::vector<Arr_Identifier *> *arr_identifier){
    this->data_type = data_type;
    if(arr_identifier) this->arr_identifier = arr_identifier;
    else  this->arr_identifier = new std::vector<Arr_Identifier *>();
    if(identifier) this->identifier = identifier;
    else  this->identifier = new std::vector<Var_Identifier *>();
  }
  Field_Decl(Data_Type data_type, std::vector<Var_Identifier *> *identifier,
    std::vector<Arr_Identifier *> *arr_identifier){
      this->data_type = data_type;
      if(identifier) this->identifier = identifier;
      else  this->identifier = new std::vector<Var_Identifier *>();
      if(arr_identifier) this->arr_identifier = arr_identifier;
      else  this->arr_identifier = new std::vector<Arr_Identifier *>();
    }
  std::vector<Var_Identifier *> *get_idl(){
    return this->identifier;
  }
  std::vector<Arr_Identifier *> *get_arr_idl(){
    return this->arr_identifier;
  }
  std::string get_data_type(){
    switch (this->data_type) {
      case Data_Type::int_type:
        return "int";
      case Data_Type::void_type:
        return "void";
      case Data_Type::bool_type:
        return "bool";
    }
  }
  void print() {
    for(auto it : *identifier) {
      std::cout << "<declaration name=\"" << it->get_id() << "\" type=\"" << this->get_data_type() << "/>" << std::endl;
    }
    for(auto it : *arr_identifier){
      std::cout << "<declaration name=\"" << it->get_id() << "\" count=\"" << it->get_size() << "\" type=\"" << this->get_data_type() << "/>" << std::endl;
    }
  }
  ~Field_Decl(){}
};

class Program {
  std::string id;
  std::vector<Field_Decl *> *field_decl_list;
  std::vector<Method_Decl *> *method_decl_list;
public:
  Program(std::string id, std::vector<Field_Decl *> *field_decl_list,
    std::vector<Method_Decl *> *method_decl_list) {
       this->id = id;
       if(method_decl_list)
        this->method_decl_list = method_decl_list;
      else
        this->method_decl_list = new std::vector<Method_Decl *>();
      if(field_decl_list)
        this->field_decl_list = field_decl_list;
      else
        this->field_decl_list = new std::vector<Field_Decl *>();
     }
  std::string get_id(){
    return this->id;
  }
  std::vector<Field_Decl *> *get_fdls(){
    return this->field_decl_list;
  }
  std::vector<Method_Decl *> *get_mdls(){
    return this->method_decl_list;
  }
  void print(){
    std::cout << "<program>" << std::endl;
    std::cout << "<field_declaration count=\"" << this->field_decl_list->size() << "\">" << std::endl;
    for(auto it : *field_decl_list) {
      it->print();
    }
    std::cout << "</field_declarations>" << std::endl;
    std::cout << "<method_declarations count=\"" << (int)this->method_decl_list->size() << "\">" << std::endl;
    for(auto it: *method_decl_list) {
      it->print();
    }
    std::cout << "</method_declarations>" << std::endl;
    std::cout << "</program>" << std::endl;
  }
  ~Program(){}
};

#endif
