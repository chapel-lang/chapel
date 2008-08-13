#ifndef _BB_H_
#define _BB_H_

#include "chpl.h"

class BitVec;
class Expr;
class SymExpr;
class Stmt;
class Symbol;
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

void buildLocalsVectorMap(FnSymbol* fn,
                          Vec<Symbol*>& locals,
                          Map<Symbol*,int>& localMap);

void backwardFlowAnalysis(FnSymbol* fn,
                          Vec<BitVec*>& GEN,
                          Vec<BitVec*>& KILL,
                          Vec<BitVec*>& IN,
                          Vec<BitVec*>& OUT);

void forwardFlowAnalysis(FnSymbol* fn,
                         Vec<BitVec*>& GEN,
                         Vec<BitVec*>& KILL,
                         Vec<BitVec*>& IN,
                         Vec<BitVec*>& OUT,
                         bool intersect = true);

void printBasicBlocks(FnSymbol* fn);

void printLocalsVector(Vec<Symbol*> locals, Map<Symbol*,int>& localMap);
void printDefsVector(Vec<SymExpr*> defs, Map<SymExpr*,int>& defMap);
void printLocalsVectorSets(Vec<BitVec*>& sets, Vec<Symbol*> locals);
void printBitVectorSets(Vec<BitVec*>& sets);

#endif
