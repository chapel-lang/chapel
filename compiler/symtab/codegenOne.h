#ifndef _CODEGEN_ONE_H_
#define _CODEGEN_ONE_H_

#include "symtabTraversal.h"

class CodegenOne : public SymtabTraversal {
 public:
  Vec<TypeSymbol*> typeSymbols;
  Vec<FnSymbol*> fnSymbols;
  Vec<VarSymbol*> varSymbols;
  CodegenOne();
  void processSymbol(Symbol* sym);
  void run(ModuleList* moduleList = NULL);
};

#endif
