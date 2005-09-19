#ifndef _COPY_PROPAGATION_H_
#define _COPY_PROPAGATION_H_

#include "symtabTraversal.h"

class CopyPropagation : public SymtabTraversal {
public:
  CopyPropagation();
  void processSymbol(Symbol* sym);
};

#endif
