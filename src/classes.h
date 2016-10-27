#ifndef _CLASSES_H
#define _CLASSES_H

#include <vector>
#include <string>


class Program;
class Field_Decl;
class Var_Decl;
class Method_Decl;
class Identifier;
class Statement;
class Expression;
class Data_Type;
class Arr_Identifier;
class Assign_Op;
class Callout_Arg;
class Literal;
class Location;
class Method_Call;
class Block;


class Program {
  std::string id;
  std::vector<Field_Decl *> *field_decl_list;
  std::vector<Method_Decl *> *method_decl_list;
  std::vector<Statement *> *statement_decl_list; // Check ?
public:
  Program(std::string id, std::vector<Field_Decl *> *field_decl_list,
    std::vector<Method_Decl *> *method_decl_list,
    std::vector<Statement *> *statement_decl_list) {
       this->id = id;
       this->method_decl_list = method_decl_list;
       this->field_decl_list = field_decl_list;
       this->statement_decl_list = statement_decl_list;
     }
  std::string get_id(){
    return this->id;
  }
  std::vector<Field_Decl *> get_fdls(){
    return this->field_decl_list;
  }
  std::vector<Method_Decl *> get_mdls(){
    return this->method_decl_list;
  }
  std::vector<Statement *> get_stmts(){
    return this->statement_decl_list;
  }
  ~Program(){}
};

class Field_Decl {
  Data_Type data_type;
  std::vector<Identifier *> *identifier;
  std::vector<Arr_Identifier *> *arr_identifier;
public:
  Field_Decl(Data_Type data_type, std::vector<Identifier *> *identifier){
    this->data_type = data_type;
    this->identifier = identifier;
    this->arr_identifier = NULL;
  }
  Field_Decl(Data_Type data_type, std::vector<Arr_Identifier *>*arr_identifier){
    this->data_type = data_type;
    this->arr_identifier = arr_identifier;
    this->identifier = NULL;
  }
  std::vector<Identifier *> get_idl(){
    return this->identifier;
  }
  std::vector<Arr_Identifier *> get_arr_idl(){
    this->arr_identifier;
  }
  Data_Type get_data_type(){
    this->data_type;
  }
  ~Field_Decl(){}
}

class Method_Decl {
  std::string id;
  Data_Type return_type;
  std::vector<Identifier *> *arguments;
  Block my_block;
public:
  Method_Decl(std::string id, Data_Type return_type,
    std::vector<Identifier *> *arguments, Block my_block){
      this->id = id;
      this->data_type = data_type;
      this->arguments = arguments;
      this->my_block = my_block;
    }
  std::string get_id(){
    return this->id;
  }
  Data_Type get_return_type(){
    return this->return_type;
  }
  std::vector<Identifier *> get_arguments(){
    return this->arguments;
  }
  Block get_block(){
    return this->my_block;
  }
  ~Method_Decl(){}
};

class Var_Decl {
  Data_Type data_type;

}
