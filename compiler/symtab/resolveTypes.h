#ifndef _RESOLVE_TYPES_H_
#define _RESOLVE_TYPES_H_

#include "symtabTraversal.h"

class ResolveTypes : public SymtabTraversal {
 public:
  void processSymbol(Symbol* sym);
};

void resolveTypes(void);

#endif
