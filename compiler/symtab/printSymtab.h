#ifndef _PRINT_SYMTAB_H_
#define _PRINT_SYMTAB_H_

#include "symtabTraversal.h"

class PrintSymtab : public SymtabTraversal {
 public:
  FILE* outfile;

  PrintSymtab(FILE* init_outfile = stderr);
  void run(ModuleSymbol* moduleList);
  
  void preProcessScope(SymScope* scope);
  void postProcessScope(SymScope* scope);
  void processSymbol(Symbol* sym);
};


#endif

