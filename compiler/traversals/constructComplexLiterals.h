#ifndef _CONSTRUCT_COMPLEX_LITERALS_H_
#define _CONSTRUCT_COMPLEX_LITERALS_H_

#include "traversal.h"

class ConstructComplexLiterals : public Traversal {
 public:
  void postProcessExpr(Expr* expr);
};

#endif
