#ifndef _REMOVE_LITERALS_H_
#define _REMOVE_LITERALS_H_

#include "traversal.h"

class RemoveLiterals : public Traversal {
 public:
  void postProcessExpr(Expr* expr);
};

#endif
