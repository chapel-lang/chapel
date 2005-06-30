/******************************************************************************
 *** Expand Class Withs
 ***
 *** This traversal expands with-statements that appear in class
 *** definitions; it replaces them with the definitions in the
 *** included class.
 ***
 ***/

#ifndef _EXPAND_CLASS_WITHS_H_
#define _EXPAND_CLASS_WITHS_H_

#include "traversal.h"

class ExpandClassWiths : public Traversal {
public:
  void preProcessExpr(Expr* expr);
};

#endif
