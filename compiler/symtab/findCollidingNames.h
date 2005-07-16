#ifndef _FIND_COLLIDING_NAMES_H_
#define _FIND_COLLIDING_NAMES_H_
#include "symtabTraversal.h"

extern bool _adhoc_to_uniform_mangling;

class FindCollidingNames : public SymtabTraversal {
  Vec<FnSymbol*> fnsyms;
  Vec<TypeSymbol*> typesyms;
  Vec<VarSymbol*> globalvars;
public:
  void processSymbol(Symbol* sym);
  void run(ModuleList* moduleList = NULL);
};

#endif 
