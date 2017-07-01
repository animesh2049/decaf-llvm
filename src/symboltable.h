#ifndef _SYMBOLTABLE_H
#define _SYMBOLTABLE_H

#include <map>
#include <list>
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
};

class SymbolTable {
private:
  std::list<SymbolTableNode*> *node_list;
public:
  void add_scope(llvm::BasicBlock *block) {
    this->node_list->push_front(new SymbolTableNode(block));
  }

  void delete_scope() {
    this->node_list->pop_front();
  }

  llvm::Value *get_variable(std::string id) {
    for (auto it = this->node_list->rbegin(); it != this->node_list->rend(); it++) {
      if ((*it)->get_item(id) != NULL) return (*it)->get_item(id);
    }
    return nullptr;
  }

  llvm::Value *get_variable(std::string id, int index) {
    
  }

  SymbolTableNode *get_present_scope() {
    return this->node_list->front();
  }
};

#endif
