#ifndef _INSERT_VARIABLE_INITIALIZATIONS_H_
#define _INSERT_VARIABLE_INITIALIZATIONS_H_

#include "baseAST.h"
#include "traversal.h"
#include "symbol.h"
#include "stmt.h"

class InsertVariableInitializations : public Traversal {
 public:
  InsertVariableInitializations(void);
  void postProcessStmt(Stmt* stmt);
};

void insertVariableInitializations(void);
  
#endif
