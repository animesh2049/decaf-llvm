#ifndef _CODEGEN_H
#define _CODEGEN_H

#include <llvm/ADT/APFloat.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/IRBuilder.h>

#include "classes.h"
#include "symboltable.h"


class Codegen {
private:
  static llvm::IRBuilder<> *Builder;
  static llvm::Module *module;
  /*
  * unique_ptr is taken because it has some special properties
  * like it automatically destorys object when assigned with
  * something else
  */
  SymbolTable *symbol_table;
  SymbolTableNode *global_scope;
  Program *program;
  llvm::Function *main_function;

public:
  Codegen () {
    this->module = new llvm::Module("main_module", llvm::getGlobalContext());
    Builder = new llvm::IRBuilder<>(llvm::getGlobalContext());
    /*this->program = start;
    this->symbol_table->add_scope();
    this->global_scope = this->symbol_table->get_present_scope();

    if (!start->get_main_function()) LogError("No Declaration of main");

    llvm::FunctionType *main_type;
    std::string main_return_type = start->get_main_function()->get_return_type();
    if (main_return_type == "int") {
      main_type = llvm::FunctionType::get(llvm::Type::getInt32Ty(llvm::getGlobalContext()), false);
    }
    else if (main_return_type == "void") {
      main_type = llvm::FunctionType::get(llvm::Type::getVoidTy(llvm::getGlobalContext()), false);
    }
    else if (main_return_type == "bool") {
      main_type = llvm::FunctionType::get(llvm::Type::getInt1Ty(llvm::getGlobalContext()), false);
    }
    else LogError("Unknow return type of main function :(");

    this->main_function = llvm::Function::Create(main_type, llvm::Function::ExternalLinkage, "main_function", module.get()); */
  }

  void generate_code() {
    llvm::BasicBlock *block = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry", this->main_function);
    node_code_gen(this->program);
    llvm::verifyModule(*(this->module));
    llvm::legacy::PassManager PM;
    PM.add(llvm::createPrintModulePass(llvm::outs()));
    PM.run(*(this->module));
  }

  void node_code_gen(Program *my_program) {
    for (auto &it : *(my_program->get_fdls())) node_code_gen(it);

  }

  void node_code_gen(Field_Decl *node) {
    std::string var_type = node->get_data_type();
    for (auto &it : *(node->get_idl())) node_code_gen(node->get_data_type(), it);
    for (auto &it : *(node->get_arr_idl())) node_code_gen(node->get_data_type(), it);
  }

  void node_code_gen(std::string type, Var_Identifier *node) {
    SymbolTableNode *present_scope = this->symbol_table->get_present_scope();
    if (present_scope->get_item(node->get_id()) != NULL) {
      LogError("Redefiniton of " + node->get_id());
      exit(-1);
    }
    llvm::AllocaInst *allocate_instruction = this->Builder->CreateAlloca(this->get_type(type), nullptr, node->get_id());
    present_scope->add_item(node->get_id(), allocate_instruction);
  }

  void node_code_gen(std::string type, Arr_Identifier *node) {
    SymbolTableNode *present_scope = this->symbol_table->get_present_scope();
    if (present_scope->get_item(node->get_id()) != NULL) {
      LogError("Redefiniton of " + node->get_id());
      exit(-1);
    }
    llvm::AllocaInst *allocate_instruction = this->Builder->CreateAlloca(
      this->get_type(type), this->Builder->getInt32(node->get_size()), node->get_id());
    present_scope->add_item(node->get_id(), allocate_instruction);
  }

  void node_code_gen(Block *node) {
    //for (auto &it : *(node->get_identifier_list())) node_code_gen(it);
    for (auto &it : *(node->get_statement_list())) node_code_gen(it);
  }

  llvm::Value *node_code_gen(Expression *node) {
    if (dynamic_cast<Binary_Operation_Expression*>(node)) node_code_gen(dynamic_cast<If_Statement*>(node));
    if (dynamic_cast<Unary_Operation_Expression*>(node)) node_code_gen(dynamic_cast<If_Statement*>(node));
    return nullptr;
  }

  llvm::Value *node_code_gen(Binary_Operation_Expression *node) {
    llvm::Value *l = node_code_gen(node->get_left_expr());
    llvm::Value *r = node_code_gen(node->get_right_expr());
    // Type check should be done before creating instruction
    switch (this->get_binop(node->get_binop())) {
      case Bin_Op::op_plus:
        return this->Builder->CreateFAdd(l, r, "addtmp");  // Change F* to I* as we are not 
      case Bin_Op::op_minus:                               // supporting floating point variables in decaf
        return this->Builder->CreateFSub(l, r, "subtmp");
      case Bin_Op::op_multiply:
        return this->Builder->CreateFMul(l, r, "multmp");
      case Bin_Op::op_divide:
        return this->Builder->CreateFDiv(l, r, "divtmp");
      case Bin_Op::op_modulo:
        return this->Builder->CreateFRem(l, r, "modtmp");
      case Bin_Op::op_less_than:
        return this->Builder->CreateFCmpULT(l, r, "lttmp");
      case Bin_Op::op_greater_than:
        return this->Builder->CreateFCmpUGT(l, r, "gttmp");
      case Bin_Op::op_less_equal:
        return this->Builder->CreateFCmpULE(l, r, "letmp");
      case Bin_Op::op_greater_equal:
        return this->Builder->CreateFCmpUGE(l, r, "getmp");
      case Bin_Op::op_equal_equal:
        return this->Builder->CreateFCmpUEQ(l, r, "eqltmp");
      case Bin_Op::op_logical_and:
        return this->Builder->CreateAnd(l, r, "andtmp");
      case Bin_Op::op_logical_or:
        return this->Builder->CreateOr(l, r, "ortmp");
      case Bin_Op::op_not_equal:
        return this->Builder->CreateFCmpUNE(l, r, "netmp");
    }
  }

  llvm::Value *node_code_gen(Unary_Operation_Expression *node) {
    llvm::Value *expr = node_code_gen(node->get_expr());

    switch (this->get_unop(node->get_operation())) {
      case Unary_Op::op_minus:
        return this->Builder->CreateNeg(expr, "minustmp");
      case Unary_Op::op_not:
        return this->Builder->CreateNot(expr, "nottmp");
    }
  }

  void node_code_gen(Assign_Statement *node) {
    llvm::Value *expr = node_code_gen(node->get_expression());

    if (dynamic_cast<Var_Location*>(node->get_location())) {
      Var_Location *variable_location = dynamic_cast<Var_Location*>(node->get_location());
      llvm::Value *location = this->symbol_table->get_variable(variable_location->get_id());
      if (location == nullptr) LogError("Undefined variable referenced"), exit(-1);
      llvm::PointerType *ptr_type = expr->getType()->getPointerTo();
      if (ptr_type != location->getType()) LogError("Type of lvalue and rvalue doesn't match"), exit(-1);
      this->Builder->CreateStore(expr, dynamic_cast<llvm::AllocaInst*>(location));
    }
    else if (dynamic_cast<Arr_Location*>(node->get_location())) {
      Arr_Location *array_location = dynamic_cast<Arr_Location*>(node->get_location());
      llvm::Value *location = this->symbol_table->get_variable(array_location->get_id(), array_location->get_index());
      if (location == nullptr) LogError("Undefined variable referenced"), exit(-1);
      llvm::PointerType *ptr_type = expr->getType()->getPointerTo();
      if (ptr_type != location->getType()) LogError("Type of lvalue and rvalue doesn't match"), exit(-1);
      this->Builder->CreateStore(expr, dynamic_cast<llvm::AllocaInst*>(location));
    }
    else {
      LogError("Unknown kind of variable :(");
      exit(-1);
    }
  }

  void node_code_gen(Statement *node) {
    if (dynamic_cast<If_Statement*>(node)) node_code_gen(dynamic_cast<If_Statement*>(node));
    if (dynamic_cast<For_Statement*>(node)) node_code_gen(dynamic_cast<For_Statement*>(node));
    if (dynamic_cast<Assign_Statement*>(node)) node_code_gen(dynamic_cast<Assign_Statement*>(node));
    if (dynamic_cast<Return_Statement*>(node)) node_code_gen(dynamic_cast<Return_Statement*>(node));
    if (dynamic_cast<Continue_Statement*>(node)) node_code_gen(dynamic_cast<Continue_Statement*>(node));
    if (dynamic_cast<Break_Statement*>(node)) node_code_gen(dynamic_cast<Break_Statement*>(node));
  }

  void node_code_gen(Method_Decl *node) {
    llvm::Type *ret_type = this->get_type(node->get_return_type());

    std::vector<llvm::Type *> args;
    for (auto &it : *(node->get_arguments())) {
      args.push_back(this->get_type(it->get_type()));
    }

    llvm::Function *func = llvm::Function::Create(
      llvm::FunctionType::get(ret_type, args, false),
      llvm::Function::ExternalLinkage, node->get_id(), this->module);

    llvm::BasicBlock *func_basic_block = llvm::BasicBlock::Create(
      llvm::getGlobalContext(), "", func);

    this->symbol_table->add_scope(func_basic_block);
    this->Builder->SetInsertPoint(func_basic_block);

    for (auto &it : *(node->get_arguments())) {
      llvm::AllocaInst *allocate_instruction = this->Builder->CreateAlloca(
        this->get_type(it->get_type()), nullptr, it->get_id());
      this->symbol_table->get_present_scope()->add_item(it->get_id(), allocate_instruction);
    }

    //node_code_gen(node->get_block());
    this->symbol_table->delete_scope();
  }

  llvm::Type *get_type(std::string type) {
    if (type == "int") return this->Builder->getInt32Ty();
    else if (type == "bool") return this->Builder->getInt1Ty();
    else return this->Builder->getVoidTy();
  }

  Bin_Op get_binop(std::string op) {
    if (op == "+") return Bin_Op::op_plus;
    if (op == "-") return Bin_Op::op_minus;
    if (op == "*") return Bin_Op::op_multiply;
    if (op == "/") return Bin_Op::op_divide;
    if (op == "%") return Bin_Op::op_modulo;
    if (op == ">") return Bin_Op::op_greater_than;
    if (op == "<") return Bin_Op::op_less_than;
    if (op == "<=") return Bin_Op::op_less_equal;
    if (op == ">=") return Bin_Op::op_greater_equal;
    if (op == "||") return Bin_Op::op_logical_or;
    if (op == "&&") return Bin_Op::op_logical_and;
    if (op == "==") return Bin_Op::op_equal_equal;
    if (op == "!=") return Bin_Op::op_not_equal;
  }

  Unary_Op get_unop(std::string op) {
    if (op == "-") return Unary_Op::op_minus;
    if (op == "!") return Unary_Op::op_not;
  }

  void LogError(std::string err_msg) {
    std::cerr << err_msg << std::endl;
  }
};

#endif
