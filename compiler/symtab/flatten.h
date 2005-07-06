#ifndef _FLATTEN_H_
#define _FLATTEN_H_

#include "symtabTraversal.h"

class Flatten : public SymtabTraversal {
 public:
  void processSymbol(Symbol* sym);
};

#endif
