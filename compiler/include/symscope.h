#ifndef _SYMSCOPE_H_
#define _SYMSCOPE_H_

#include "symbol.h"

class SymScope {
 public:
  BaseAST* astParent; // back pointer to AST
  SymScope* parent;
  Map<const char*,Vec<FnSymbol*>*> visibleFunctions;
  Map<const char*,Symbol*> table;

  SymScope(BaseAST* iastParent, SymScope* iparent);
  ~SymScope();
  
  void define(Symbol* sym);
  void undefine(Symbol* sym);

  Symbol* lookupLocal(const char* name,
                      Vec<SymScope*>* alreadyVisited = NULL,
                      bool returnModules = true);
  Symbol* lookup(const char* name,
                 Vec<SymScope*>* alreadyVisited = NULL,
                 bool returnModules = true,
                 bool scanModuleUses = true);

  void addModuleUse(ModuleSymbol* mod);
  Vec<ModuleSymbol*>* getModuleUses();

  void print();
  void print(bool number, int indent);

  void codegen(FILE* outfile);
  void codegenFunctions(FILE* outfile);

  void addVisibleFunction(FnSymbol* fn);
  void removeVisibleFunction(FnSymbol* fn);
  void getVisibleFunctions(Vec<FnSymbol*>* allVisibleFunctions, const char* name,
                           bool recursed = false);
}; 

#endif
