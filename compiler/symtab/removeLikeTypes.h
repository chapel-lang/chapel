#ifndef _REMOVE_LIKE_TYPES_H_
#define _REMOVE_LIKE_TYPES_H_

#include "symtabTraversal.h"

class RemoveLikeTypes : public SymtabTraversal {
 public:
  void processSymbol(Symbol* sym);
};

#endif
