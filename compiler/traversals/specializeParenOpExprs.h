/******************************************************************************
 *** Specialize Paren Op Exprs
 ***
 *** This traversal changes all ParenOpExpr that are arrays to
 *** ArrayRef and changes all ParenOpExpr that are write, writeln, and
 *** read to IOCall.  Also, this traversal corrects constructors.
 ***
 ***/

#ifndef _SPECIALIZE_PAREN_OP_EXPRS_H_
#define _SPECIALIZE_PAREN_OP_EXPRS_H_

#include "traversal.h"

class SpecializeParenOpExprs : public Traversal {
 public:
  void postProcessExpr(Expr* expr);
};

#endif
