#ifndef _RECONSTRUCT_ITERATORS_H_
#define _RECONSTRUCT_ITERATORS_H_

#include "symtabTraversal.h"

class ReconstructIterators : public SymtabTraversal {
 public:
  void processSymbol(Symbol* sym);
};

#endif
