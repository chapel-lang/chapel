#ifndef _RESOLVE_TYPES_H_
#define _RESOLVE_TYPES_H_

#include "symtabTraversal.h"

class ResolveTypes : public SymtabTraversal {
 public:
  ResolveTypes::ResolveTypes();
  void processSymbol(Symbol* sym);
};

class ResolveTupleTypes : public SymtabTraversal {
 public:
  ResolveTupleTypes::ResolveTupleTypes();
  void processSymbol(Symbol* sym);
};

#endif
