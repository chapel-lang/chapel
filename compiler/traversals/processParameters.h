#ifndef _PROCESS_PARAMETERS_H_
#define _PROCESS_PARAMETERS_H_

#include "traversal.h"

class ProcessParameters : public Traversal {
 public:
  ProcessParameters(void);

  void postProcessExpr(Expr* &expr);
};

#endif

