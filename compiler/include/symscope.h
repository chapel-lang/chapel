#ifndef _SYMSCOPE_H_
#define _SYMSCOPE_H_

#include "symbol.h"

enum scopeType {
  // builtins at the global level
  SCOPE_INTRINSIC = -1,
  SCOPE_MODULE = 0,           // less is all modules
  SCOPE_ARG,
  SCOPE_LOCAL,
  SCOPE_CLASS,
};

class SymScope : public gc {
 public:
  scopeType type;

  BaseAST* astParent; // back pointer to AST

  SymScope* parent;
  SymScope* child;
  SymScope* sibling;

  Vec<ModuleSymbol*> uses; // modules used by this scope

  Vec<Symbol*> symbols;

  Map<char*,Vec<FnSymbol*>*> visibleFunctions;

  ChainHashMap<char*, StringHashFns, Symbol*> table;

  SymScope(scopeType init_type);
  void setASTParent(BaseAST* ast);
  
  void insertChildScope(SymScope *aScope);
  bool isEmpty(void);

  void define(Symbol* sym);
  void undefine(Symbol* sym);

  void dump_only();
  void dump();

  int parentLength(void);

  void codegen(FILE* outfile, char* separator);
  void codegenFunctions(FILE* outfile);

  void addVisibleFunction(FnSymbol* fn);
  void removeVisibleFunction(FnSymbol* fn);
  void setVisibleFunctions(FnSymbol* fn);
  void printVisibleFunctions();

  void getVisibleFunctions(Vec<FnSymbol*>* allVisibleFunctions, char* name,
                           bool recursed = false);
}; 

void getSymbols(SymScope *scope, Vec<Symbol *> &symbols);

#endif
