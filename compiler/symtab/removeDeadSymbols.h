#ifndef _REMOVE_DEAD_SYMBOLS_H_
#define _REMOVE_DEAD_SYMBOLS_H_

#include "symtabTraversal.h"

class RemoveDeadSymbols : public SymtabTraversal {
 public:
  RemoveDeadSymbols(void);
  void processSymbol(Symbol* sym);
};

#endif
