#ifndef _INSERT_ELIDED_LOOPS_H_
#define _INSERT_ELIDED_LOOPS_H_

#include "traversal.h"

class InsertElidedLoops : public Traversal {
 public:
  void postProcessStmt(Stmt* stmt);
};

#endif
