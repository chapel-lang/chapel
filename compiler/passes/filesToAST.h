#ifndef _FILES_TO_AST_H_
#define _FILES_TO_AST_H_

#include "pass.h"

class FilesToAST : public Pass {
  void run(Stmt* program);
};

#endif

