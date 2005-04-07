#ifndef _INSERT_TEMPS_H_
#define _INSERT_TEMPS_H_

#include "traversal.h"

class InsertTemps : public Traversal {
 public:
  void postProcessExpr(Expr* expr);
};

#endif
