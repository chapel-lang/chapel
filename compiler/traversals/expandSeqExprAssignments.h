#ifndef _EXPAND_SEQ_EXPR_ASSIGNMENTS_H_
#define _EXPAND_SEQ_EXPR_ASSIGNMENTS_H_

#include "traversal.h"

class ExpandSeqExprAssignments : public Traversal {
 public:
  void postProcessStmt(Stmt* stmt);
};


#endif
