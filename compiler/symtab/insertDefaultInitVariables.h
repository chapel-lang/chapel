#ifndef _INSERT_DEFAULT_INIT_VARIABLES_H_
#define _INSERT_DEFAULT_INIT_VARIABLES_H_

#include "symtabTraversal.h"

class InsertDefaultInitVariables : public SymtabTraversal {
 public:
  InsertDefaultInitVariables::InsertDefaultInitVariables();
  void processSymbol(Symbol* sym);
};

void insertDefaultInitVariables(void);

#endif
