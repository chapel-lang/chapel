#ifndef _FIND_UNRESOLVED_SYMBOLS_H_
#define _FIND_UNRESOLVED_SYMBOLS_H_

#include "traversal.h"

class FindUnresolvedSymbols : public Traversal {
 public:
  void preProcessSymbol(Symbol* sym);

};

#endif

