#ifndef _REMOVE_DEAD_SYMBOLS_H_
#define _REMOVE_DEAD_SYMBOLS_H_

#include "traversal.h"

class RemoveDeadSymbols : public Traversal {
  void postProcessStmt(Stmt* stmt);
};

#endif
