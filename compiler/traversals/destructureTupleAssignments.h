#ifndef _DESTRUCTURE_TUPLE_ASSIGNMENTS_H_
#define _DESTRUCTURE_TUPLE_ASSIGNMENTS_H_

#include "baseAST.h"
#include "traversal.h"

class DestructureTupleAssignments : public Traversal {
 public:
  void postProcessStmt(Stmt* stmt);
};

#endif
