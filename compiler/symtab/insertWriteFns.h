#ifndef _INSERT_WRITE_FNS
#define _INSERT_WRITE_FNS

#include "symtabTraversal.h"

class InsertWriteFns : public SymtabTraversal {
 public:
  InsertWriteFns(void);
  void processSymbol(Symbol* sym);
};

#endif
