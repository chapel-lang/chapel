#ifndef _LEGALIZE_C_NAMES_
#define _LEGALIZE_C_NAMES_

#include "symtabTraversal.h"

class LegalizeCNames : public SymtabTraversal {
 public:

  void processSymbol(Symbol* sym);
};

#endif
