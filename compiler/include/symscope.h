#ifndef _SYMSCOPE_H_
#define _SYMSCOPE_H_

#include "symbol.h"

class SymScope {
 public:
  BaseAST* astParent; // back pointer to AST
  SymScope* parent;
  Map<char*,Vec<FnSymbol*>*> visibleFunctions;
  ChainHashMap<char*, StringHashFns, Symbol*> table;

  SymScope(BaseAST* iastParent, SymScope* iparent);
  ~SymScope();
  
  void define(Symbol* sym);
  void undefine(Symbol* sym);

  Symbol* lookupLocal(char* name, Vec<SymScope*>* alreadyVisited = NULL, bool nestedUse = false);
  Symbol* lookup(char* name);

  void addModuleUse(ModuleSymbol* mod);
  Vec<ModuleSymbol*>* getModuleUses();

  void print();
  void print(bool number, int indent);

  void codegen(FILE* outfile, char* separator);
  void codegenFunctions(FILE* outfile);

  void addVisibleFunction(FnSymbol* fn);
  void removeVisibleFunction(FnSymbol* fn);
  void getVisibleFunctions(Vec<FnSymbol*>* allVisibleFunctions, char* name,
                           bool recursed = false, bool nestedUse = false);
}; 

#endif
