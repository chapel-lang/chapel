#ifndef _CLONE_ALL_FNS_H_
#define _CLONE_ALL_FNS_H_

#include "traversal.h"

class CloneAllFns : public Traversal {
  void preProcessStmt(Stmt* &stmt);
};

#endif

