#ifndef _SYMBOLTABLE_H
#define _SYMBOLTABLE_H

#include <map>
#include <list>
#include <stack>
#include <llvm/IR/BasicBlock.h>

class SymbolTableNode {
private:
  llvm::BasicBlock *basic_block;
  std::map<std::string, llvm::Value*> name_value;
public:
  SymbolTableNode(llvm::BasicBlock *block) : basic_block(block) {}

  bool does_exist(std::string name) { // Only to check whether something of this name exists
    return (this->name_value[name] != NULL);
  }

  void add_item(std::string name, llvm::Value* value) {
    name_value[name] = value;
  }

  llvm::Value *get_item(std::string name) {
    return this->name_value[name];
  }

  llvm::BasicBlock *get_block() {
    return this->basic_block;
  }
};

class SymbolTable {
private:
  std::list<SymbolTableNode*> *node_list;
  std::stack< std::pair<llvm::BasicBlock *, llvm::BasicBlock*> *> *break_continue_pair_stack;
public:
  void add_scope(llvm::BasicBlock *block) {
    this->node_list->push_front(new SymbolTableNode(block));
  }

  std::pair<llvm::BasicBlock*, llvm::BasicBlock*> *get_break_continue_stack_scope() {
    if (this->break_continue_pair_stack == nullptr) return nullptr;
    return this->break_continue_pair_stack->top();
  }

  void delete_present_break_continue_stack_scope() {
    if (this->break_continue_pair_stack)
      this->break_continue_pair_stack->pop();
  }

  void add_break_continue_stack_scope(llvm::BasicBlock *break_scope, llvm::BasicBlock *continue_scope) {
    std::pair<llvm::BasicBlock *, llvm::BasicBlock *> temp_pair = std::make_pair(break_scope, continue_scope);
    this->break_continue_pair_stack->push(&temp_pair);
  }

  void delete_scope() {
    if (this->node_list != nullptr)
      this->node_list->pop_front();
  }

  llvm::Value *get_variable(std::string id) {
    for (auto it = this->node_list->rbegin(); it != this->node_list->rend(); it++) {
      if ((*it)->get_item(id) != nullptr) return (*it)->get_item(id);
    }
    return nullptr;
  }

  SymbolTableNode *get_present_scope() {
    if (this->node_list == nullptr) return nullptr;
    return this->node_list->front();
  }
};

#endif
