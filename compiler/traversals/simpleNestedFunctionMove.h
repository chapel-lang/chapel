#ifndef _SIMPLE_NESTED_FUNCTION_MOVE_
#define _SIMPLE_NESTED_FUNCTION_MOVE_

#include "traversal.h"
#include "expr.h"

class SimpleNestedFunctionMove : public Traversal {
public:
  void postProcessStmt(Stmt* stmt);
};
#endif 
