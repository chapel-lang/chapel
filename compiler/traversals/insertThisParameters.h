/******************************************************************************
 *** Insert This Parameters
 ***
 *** This traversal inserts "this" as the first parameter in bound
 *** functions.  It also resolves the class that secondary methods are
 *** bound to.  It also mangles the cname of methods.
 ***
 ***/

#ifndef _INSERT_THIS_PARAMETERS_H_
#define _INSERT_THIS_PARAMETERS_H_

#include "traversal.h"

class InsertThisParameters : public Traversal {
 public:
  void preProcessExpr(Expr* expr);
};

void insertThisParameters(void);

#endif
