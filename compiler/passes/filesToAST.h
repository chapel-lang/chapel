#ifndef _FILES_TO_AST_H_
#define _FILES_TO_AST_H_

#include "pass.h"

extern Stmt* internalPreludeStmts;
extern Stmt* preludeStmts;
extern Stmt* entryPoint;

class FilesToAST : public Pass {
  void run(Module* moduleList);
};

#endif

