#ifndef _INSERT_VARIABLE_INITIALIZATIONS_H_
#define _INSERT_VARIABLE_INITIALIZATIONS_H_

#include "baseAST.h"
#include "traversal.h"
#include "symbol.h"
#include "stmt.h"

void insert_default_init_stmt(VarSymbol* var, Stmt* init_stmt);

class InsertVariableInitializations : public Traversal {
 public:
  InsertVariableInitializations(void);
  void postProcessStmt(Stmt* stmt);
};
  
#endif
