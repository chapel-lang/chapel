#ifndef _FILES_TO_AST_H_
#define _FILES_TO_AST_H_

#include "pass.h"

class Stmt;

extern ModuleSymbol* internalPrelude;
extern ModuleSymbol* prelude;
extern Stmt* entryPoint;

class FilesToAST : public Pass {
  void run(ModuleSymbol* moduleList);
};

#endif

