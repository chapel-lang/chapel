#ifndef _REMOVE_DEAD_SYMBOLS2_H_
#define _REMOVE_DEAD_SYMBOLS2_H_

#include "symtabTraversal.h"

class RemoveDeadSymbols2 : public SymtabTraversal {
 public:
  RemoveDeadSymbols2(void);
  void processSymbol(Symbol* sym);
};

#endif
