#ifndef _INTERVAL_H_
#define _INTERVAL_H_

#include "chpl.h"

class BitVec;
class Expr;
class SymExpr;
class Stmt;
class Symbol;
class FnSymbol;
class BasicBlock;

enum IntervalTag {
  FOR_LOOP,
  FORALL_LOOP,
  BLOCK
};

class Interval {
 public:
  int id;
  IntervalTag intervalTag;
  Vec<BasicBlock*> bb;
  Vec<Interval*> ins;
  Vec<Interval*> outs;
  Vec<BasicBlock*> sourceEdges;
  bool isEnd; // Used to specify if we are the final node
  Symbol *low;
  Symbol *high;
  Symbol *stride;
  Symbol *ILIndex;
  Interval(int init_id);

  void codegen(FILE* outfile);
};

// stmt argument is internal only
void buildIntervals(FnSymbol* fn, Map<BasicBlock*, Interval*>& intervalMaps);

#endif
