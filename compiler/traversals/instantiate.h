#ifndef _INSTANTIATE_H_
#define _INSTANTIATE_H_

#include "traversal.h"

class Instantiate : public Traversal {
 public:
  void postProcessExpr(Expr* expr);
};

#endif
