#ifndef _NORMALIZE_FUNCTIONS_H_
#define _NORMALIZE_FUNCTIONS_H_

#include "symtabTraversal.h"

class NormalizeFunctions : public SymtabTraversal {
 public:
  void processSymbol(Symbol* sym);
};

void insert_formal_temps(FnSymbol* fn);

void normalizeFunctions(void);

#endif
