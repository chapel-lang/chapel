#ifndef _COLLECT_FUNCTIONS_H_
#define _COLLECT_FUNCTIONS_H_

#include "symtabTraversal.h"

class CollectFunctions : public SymtabTraversal {
 public:
  Vec<FnSymbol*>* functions;
  CollectFunctions(Vec<FnSymbol*>* init_functions);
  void processSymbol(Symbol* sym);
};

#endif
