#ifndef _PRINTAST_H_
#define _PRINTAST_H_

#include "traversal.h"

class PrintAST : public Traversal {
 public:
  PrintAST(void);

  void preProcessStmt(Stmt* stmt);
};

void printAST(void);

#endif
