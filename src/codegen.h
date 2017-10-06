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

/*
 * Add support for simple blocks which are without any functions.
 * Do check for main function and its prototype.
 * Check if arrays are declared only in global scope.
 * Initialization is must with declaration of variables.
 * Changing in function shouldn't change the actual value.
 * Functions returning something can be an expresssion,
 * whereas void functions can't be expression, they can only be statement.
 */

class Codegen {
private:
  static llvm::IRBuilder<> *Builder;
  static llvm::Module *module;
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
    for (auto &it : *(my_program->get_mdls())) node_code_gen(it);
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
    if (dynamic_cast<Binary_Operation_Expression*>(node)) return node_code_gen(dynamic_cast<Binary_Operation_Expression*>(node));
    if (dynamic_cast<Unary_Operation_Expression*>(node)) return node_code_gen(dynamic_cast<Unary_Operation_Expression*>(node));
    return nullptr;
  }

  llvm::Value *node_code_gen(Binary_Operation_Expression *node) {
    llvm::Value *l = node_code_gen(node->get_left_expr());
    llvm::Value *r = node_code_gen(node->get_right_expr());
    if (l->getType() != r->getType()) LogError("Mismatch in type of operands"), exit(-1);

    switch (node->get_binop()) {
      case Bin_Op::op_plus:
        return this->Builder->CreateAdd(l, r, "addtmp");  // Change F* to I* as we are not
      case Bin_Op::op_minus:                               // supporting floating point variables in decaf
        return this->Builder->CreateSub(l, r, "subtmp");  // Check if this also gives the values of the expresssion
      case Bin_Op::op_multiply:
        return this->Builder->CreateMul(l, r, "multmp");
      case Bin_Op::op_divide:
        return this->Builder->CreateUDiv(l, r, "divtmp");
      case Bin_Op::op_modulo:
        return this->Builder->CreateURem(l, r, "modtmp");
      case Bin_Op::op_less_than:
        return this->Builder->CreateICmpULT(l, r, "lttmp");
      case Bin_Op::op_greater_than:
        return this->Builder->CreateICmpUGT(l, r, "gttmp");
      case Bin_Op::op_less_equal:
        return this->Builder->CreateICmpULE(l, r, "letmp");
      case Bin_Op::op_greater_equal:
        return this->Builder->CreateICmpUGE(l, r, "getmp");
      case Bin_Op::op_equal_equal:
        return this->Builder->CreateICmpEQ(l, r, "eqltmp");
      case Bin_Op::op_logical_and:
        return this->Builder->CreateAnd(l, r, "andtmp");
      case Bin_Op::op_logical_or:
        return this->Builder->CreateOr(l, r, "ortmp");
      case Bin_Op::op_not_equal:
        return this->Builder->CreateICmpNE(l, r, "netmp");
    }
  }

  llvm::Value *node_code_gen(Unary_Operation_Expression *node) {
    llvm::Value *expr = node_code_gen(node->get_expr());

    switch (node->get_operation()) {
      case Unary_Op::op_minus:
        return this->Builder->CreateNeg(expr, "minustmp");
      case Unary_Op::op_not:
        return this->Builder->CreateNot(expr, "nottmp");
    }
  }

  void node_code_gen(Assign_Statement *node) {
    llvm::Value *expr = node_code_gen(node->get_expression());
    llvm::Value *storage_location = nullptr;
    llvm::Value *prev_val = nullptr;
    llvm::Value *new_val = nullptr;
    if (dynamic_cast<Var_Location*>(node->get_location())) {
      Var_Location *variable_location = dynamic_cast<Var_Location*>(node->get_location());
      llvm::Value *storage_location = this->symbol_table->get_variable(variable_location->get_id());
      if (storage_location == nullptr) LogError("Undefined variable referenced"), exit(-1);
      llvm::PointerType *ptr_type = expr->getType()->getPointerTo();
      if (ptr_type != storage_location->getType()) LogError("Type of lvalue and rvalue doesn't match"), exit(-1);
    }
    else if (dynamic_cast<Arr_Location*>(node->get_location())) {
      Arr_Location *array_location = dynamic_cast<Arr_Location*>(node->get_location());
      llvm::Value *arr_location = this->symbol_table->get_variable(array_location->get_id());
      llvm::Value *index_expression = node_code_gen(array_location->get_index());
      if (index_expression->getType() != this->Builder->getInt32Ty()) LogError("Array Location is not interger type :()"), exit(-1);
      llvm::Value *storage_location = this->Builder->CreateGEP(arr_location, index_expression);
      if (storage_location == nullptr) LogError("Undefined variable referenced"), exit(-1);
      llvm::PointerType *ptr_type = expr->getType()->getPointerTo();
      if (ptr_type != storage_location->getType()) LogError("Type of lvalue and rvalue doesn't match"), exit(-1);
    }
    else {
      LogError("Unknown kind of variable :(");
      exit(-1);
    }

    switch (node->get_operation()) {
      case Assign_Op::op_plus_equal:
        prev_val = this->Builder->CreateLoad(storage_location);
        new_val = this->Builder->CreateFAdd(expr, prev_val, "tmppluseql");
        break;
      case Assign_Op::op_minus_equal:
        prev_val = this->Builder->CreateLoad(storage_location);
        new_val = this->Builder->CreateFSub(prev_val, expr, "tmpminuseql");
        break;
      case Assign_Op::op_equal:
        new_val = expr;
        break;
    }

    this->Builder->CreateStore(new_val, storage_location);
  }

  void node_code_gen(Return_Statement *node) {
    llvm::Value *ret_expr = node_code_gen(node->get_expression());
    llvm::Function *func = this->Builder->GetInsertBlock()->getParent();
    if (func == nullptr) LogError("return statement without function"), exit(-1);
    if (ret_expr->getType() != func->getReturnType()) LogError("Wrong return type of function"), exit(-1);
    this->Builder->CreateRet(ret_expr);
  }

  void node_code_gen(For_Statement *node) {
    llvm::Value *loop_var = this->symbol_table->get_variable(node->get_id());
    llvm::Value *loop_var_val = nullptr;
    llvm::Value *cond_comp = nullptr;
    llvm::Value *end_condition = nullptr;
    llvm::BasicBlock *curr_block = this->Builder->GetInsertBlock();
    llvm::BasicBlock *start_loop_block = llvm::BasicBlock::Create(llvm::getGlobalContext(), "loopstart", curr_block->getParent());
    llvm::BasicBlock *after_loop_block = llvm::BasicBlock::Create(llvm::getGlobalContext(), "afterloop", curr_block->getParent());
    llvm::BasicBlock *for_block = llvm::BasicBlock::Create(llvm::getGlobalContext(), "forblock", curr_block->getParent());

    this->Builder->SetInsertPoint(start_loop_block);
    this->symbol_table->add_scope(start_loop_block);
    llvm::Value *loop_var_initialization = node_code_gen(node->get_init_condition());
    this->Builder->CreateStore(loop_var_initialization, loop_var);
    end_condition = node_code_gen(node->get_end_condition());
    loop_var_val = this->Builder->CreateLoad(loop_var);
    cond_comp = this->Builder->CreateICmpEQ(loop_var_val, end_condition, "loop_constraint");
    this->Builder->CreateCondBr(cond_comp, curr_block, for_block);

    this->Builder->SetInsertPoint(after_loop_block);
    this->symbol_table->add_scope(after_loop_block);
    loop_var_val = this->Builder->CreateLoad(loop_var);
    end_condition = node_code_gen(node->get_end_condition());
    cond_comp = this->Builder->CreateICmpEQ(loop_var_val, end_condition, "loop_constraint");
    this->Builder->CreateCondBr(cond_comp, curr_block, for_block);

    this->Builder->SetInsertPoint(for_block);
    this->symbol_table->add_scope(for_block);
    this->symbol_table->add_break_continue_stack_scope(curr_block, for_block);
    node_code_gen(node->get_block());
    loop_var_val = this->Builder->CreateLoad(loop_var);
    llvm::Value *increment_loop_var = this->Builder->CreateAdd(loop_var_val, this->Builder->getInt32(1), "increment_loop_var");
    this->Builder->CreateBr(after_loop_block);

    this->symbol_table->delete_scope();
    this->symbol_table->delete_scope();
    this->symbol_table->delete_scope();
    this->symbol_table->delete_present_break_continue_stack_scope();
    this->Builder->SetInsertPoint(curr_block);
  }

  void node_code_gen(If_Statement *node) {
    llvm::BasicBlock *curr_block = this->Builder->GetInsertBlock();
    llvm::BasicBlock *if_block = nullptr;
    llvm::BasicBlock *else_block = nullptr;
    llvm::Value *if_cond = node_code_gen(node->get_condition());
    llvm::Value *true_cond = this->Builder->getInt1(1);
    llvm::Value *cond_comp = this->Builder->CreateICmpEQ(if_cond, cond_comp, "if_condition");
    this->Builder->CreateCondBr(cond_comp, if_block, else_block);

    this->Builder->SetInsertPoint(if_block);
    this->symbol_table->add_scope(if_block);
    node_code_gen(node->get_if_block());
    this->symbol_table->delete_scope();
    this->Builder->CreateBr(curr_block);

    this->Builder->SetInsertPoint(else_block);
    this->symbol_table->add_scope(else_block);
    node_code_gen(node->get_else_block());
    this->symbol_table->delete_scope();
    this->Builder->CreateBr(curr_block);

    this->Builder->SetInsertPoint(curr_block);
  }

  void node_code_gen(Break_Statement *node) {
    std::pair<llvm::BasicBlock *, llvm::BasicBlock*> *present_scope_pair = this->symbol_table->get_break_continue_stack_scope();
    if (present_scope_pair == nullptr)
      LogError("Use of break statement without for block :("), exit(-1);
    this->symbol_table->delete_present_break_continue_stack_scope();
    this->Builder->CreateBr(present_scope_pair->first);
  }

  void node_code_gen(Continue_Statement *node) {
    std::pair<llvm::BasicBlock *, llvm::BasicBlock*> *present_scope_pair = this->symbol_table->get_break_continue_stack_scope();
    if (present_scope_pair == nullptr)
      LogError("Use of break statement without for block :("), exit(-1);
    this->symbol_table->delete_present_break_continue_stack_scope();
    this->Builder->CreateBr(present_scope_pair->second);
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

  /*Bin_Op get_binop(std::string op) {
    if (op == "+") return Bin_Op::op_plus;  // Instead of returning string from change
    if (op == "-") return Bin_Op::op_minus; // the return method to return binop in classes.h
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
  }*/

  void LogError(std::string err_msg) {
    std::cerr << err_msg << std::endl;
  }
};

#endif
