#ifndef _CODEGEN_ONE_H_
#define _CODEGEN_ONE_H_

#include "symtabTraversal.h"

class CodegenOne : public SymtabTraversal {
 public:
  ChainHashMap<char*, StringHashFns, int> cnames;
  Vec<TypeSymbol*> typeSymbols;
  Vec<FnSymbol*> fnSymbols;
  Vec<VarSymbol*> varSymbols;
  CodegenOne();
  void processSymbol(Symbol* sym);
  virtual void run(Vec<ModuleSymbol*>* modules = NULL);
};

void codegenOne(void);

#endif
