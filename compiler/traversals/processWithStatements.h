#ifndef _PROCESS_WITH_STATEMENTS_H_
#define _PROCESS_WITH_STATEMENTS_H_

#include "baseAST.h"
#include "traversal.h"

class ProcessWithStatements : public Traversal {
 public:
  void preProcessStmt(Stmt* stmt);
};

#endif
