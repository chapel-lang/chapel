#ifndef _ELIMINATE_RETURNS_H_
#define _ELIMINATE_RETURNS_H_

#include "traversal.h"

class EliminateReturns : public Traversal {
 public:
  EliminateReturns(void);

  void preProcessStmt(Stmt* stmt);
};

#endif
