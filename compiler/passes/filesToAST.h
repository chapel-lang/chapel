#ifndef _FILES_TO_AST_H_
#define _FILES_TO_AST_H_

#include "pass.h"

class Stmt;

extern ModuleSymbol* prelude;
extern ModuleSymbol* fileModule;

class FilesToAST : public Pass {
  virtual void run(Vec<ModuleSymbol*>* modules);
};

#endif

