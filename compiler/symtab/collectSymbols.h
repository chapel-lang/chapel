#ifndef _COLLECT_SYMBOLS_H_
#define _COLLECT_SYMBOLS_H_

#include "symtabTraversal.h"

class CollectSymbols : public SymtabTraversal {
 public:
  Vec<Symbol*>* syms;
  CollectSymbols(Vec<Symbol*>* init_syms);
  void processSymbol(Symbol* sym);
};

#endif
