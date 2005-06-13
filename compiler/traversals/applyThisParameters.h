/******************************************************************************
 *** Apply This Parameter
 ***
 *** This traversal changes all references to variables and functions
 *** defined in a class within the definitions of bound functions.
 *** They are changed to MemberAccess objects where the base is "this"
 *** and the member is the variable or function.
 ***
 ***/

#ifndef _APPLY_THIS_PARAMETERS_H_
#define _APPLY_THIS_PARAMETERS_H_

#include "traversal.h"
#include "type.h"

class ApplyThisParameters : public Traversal {
 public:
  void preProcessExpr(Expr* expr);
};

#endif
