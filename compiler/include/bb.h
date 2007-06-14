#ifndef _BB_H_
#define _BB_H_

#include "chpl.h"

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

void buildDefsVectorMap(FnSymbol* fn,
                        Vec<Symbol*>& locals,
                        Vec<SymExpr*>& defs,
                        Map<SymExpr*,int>& defMap);

void buildDefUseSets(FnSymbol* fn,
                     Vec<Symbol*>& locals,
                     Vec<SymExpr*>& useSet,
                     Vec<SymExpr*>& defSet);

void backwardFlowAnalysis(FnSymbol* fn,
                          Vec<Vec<bool>*>& GEN,
                          Vec<Vec<bool>*>& KILL,
                          Vec<Vec<bool>*>& IN,
                          Vec<Vec<bool>*>& OUT);

void forwardFlowAnalysis(FnSymbol* fn,
                         Vec<Vec<bool>*>& GEN,
                         Vec<Vec<bool>*>& KILL,
                         Vec<Vec<bool>*>& IN,
                         Vec<Vec<bool>*>& OUT,
                         bool intersect = true);

void printBasicBlocks(FnSymbol* fn);

void printLocalsVector(Vec<Symbol*> locals, Map<Symbol*,int>& localMap);
void printDefsVector(Vec<SymExpr*> defs, Map<SymExpr*,int>& defMap);
void printLocalsVectorSets(Vec<Vec<bool>*>& sets, Vec<Symbol*> locals);
void printBitVectorSets(Vec<Vec<bool>*>& sets);

#endif
