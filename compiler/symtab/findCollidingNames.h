#ifndef _FIND_COLLIDING_NAMES_H_
#define _FIND_COLLIDING_NAMES_H_
#include "symtabTraversal.h"
#include "uniqueName.h"

extern bool _adhoc_to_uniform_mangling;

class FindCollidingNames : public SymtabTraversal {
  Vec<Symbol*> fnsyms;
  Vec<Symbol*> typesyms;
  Vec<Symbol*> globalvars;
public:
  void processSymbol(Symbol* sym);
  void run(ModuleList* moduleList = NULL);
};

#endif 
