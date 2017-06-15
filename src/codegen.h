#ifndef _CODEGEN_H
#define _CODEGEN_H

#include "classes.h"
#include "symboltable.h"

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


class Codegen {
private:
  std::unique_ptr<llvm::Module> my_module;
  /*
  * unique_ptr is taken because it has some special properties
  * like it automatically destorys object when assigned with
  * something else
  */

  //llvm::Function *my_main_function;
  Program *my_program;

public:
  Codegen (Program *start) {
    my_module = llvm::make_unique<llvm::Module>("main_module", llvm::getGlobalContext());
    my_program = start;
    generate_code();
  }

  void generate_code() {

  }

  llvm::Value *node_code_gen(Program *my_program) {
    for (auto &it : *(my_program->get_fdls())) node_code_gen(it);

  }

  llvm::Value *node_code_gen(Field_Decl *node) {
    std::string var_type = node->get_data_type();
    for (auto &it : *(node->get_idl())) node_code_gen(it);
  }

  llvm::Value *node_code_gen(Var_Identifier *node) {

  }

  llvm::Value *LogErrorV(const char *err_msg) { // Why separate function when LogError is already there
    LogError(err_msg);
    return nullptr;
  }

  llvm::Value *LogError(const char *err_msg) {
    std::cerr << err_msg << std::endl;
    return nullptr;
  }
};

#endif
