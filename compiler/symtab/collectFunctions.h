#ifndef _COLLECT_FUNCTIONS_H_
#define _COLLECT_FUNCTIONS_H_

#include "symtabTraversal.h"

class CollectFunctions : public SymtabTraversal {
 public:
  Vec<FnSymbol*>* functions;
  CollectFunctions(Vec<FnSymbol*>* init_functions);
  void processSymbol(Symbol* sym);
};

void collect_functions(Vec<FnSymbol*>* functions);

// USAGE:
//   #include "../symtab/collectFunctions.h"
//   Vec<FnSymbol*> all_functions;
//   collect_functions(&all_functions);

#endif
