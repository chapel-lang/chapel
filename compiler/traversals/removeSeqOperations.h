#ifndef _REMOVE_SEQ_OPERATIONS_H_
#define _REMOVE_SEQ_OPERATIONS_H_

#include "traversal.h"

class RemoveSeqOperations : public Traversal {
  void postProcessExpr(Expr* expr);
};

#endif
