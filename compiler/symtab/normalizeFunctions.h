#ifndef _NORMALIZE_FUNCTIONS_H_
#define _NORMALIZE_FUNCTIONS_H_

#include "symtabTraversal.h"

class NormalizeFunctions : public SymtabTraversal {
 public:
  void processSymbol(Symbol* sym);
};

#endif
