#ifndef _INSERT_FUNCTION_TEMPS_H_
#define _INSERT_FUNCTION_TEMPS_H_

#include "traversal.h"
#include "vec.h"
#include "expr.h"

class InsertFunctionTemps : public Traversal {
 public:
  void postProcessStmt(Stmt* stmt);
};

#endif
