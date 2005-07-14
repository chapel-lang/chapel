#ifndef _REMOVE_NESTED_FUNCTIONS_
#define _REMOVE_NESTED_FUNCTIONS_

#include "traversal.h"
#include "stmt.h"
#include "symbol.h"

class RemoveNestedFunctions : public Traversal {

public :
  void postProcessStmt(Stmt* stmt);
  FnSymbol* hasEnclosingFunction(DefExpr* fn_def);
};

#endif
