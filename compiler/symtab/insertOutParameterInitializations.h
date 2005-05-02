#ifndef _INSERT_OUT_PARAMETER_INITIALIZATIONS_H_
#define _INSERT_OUT_PARAMETER_INITIALIZATIONS_H_

#include "symtabTraversal.h"

class InsertOutParameterInitializations : public SymtabTraversal {
 public:
  InsertOutParameterInitializations::InsertOutParameterInitializations();
  void processSymbol(Symbol* sym);
};

#endif
