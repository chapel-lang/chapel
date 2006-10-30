#ifndef _BB_H_
#define _BB_H_

#include "chpl.h"

class Expr;
class Stmt;
class FnSymbol;

class BasicBlock {
 public:
  int id;
  Vec<Expr*> exprs;
  Vec<BasicBlock*> ins;
  Vec<BasicBlock*> outs;
  BasicBlock(int init_id);
};

// stmt argument is internal only
void buildBasicBlocks(FnSymbol* fn, Expr* stmt = NULL);

void printBasicBlocks(FnSymbol* fn);

#endif
