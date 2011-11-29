#ifndef _BB_H_
#define _BB_H_

#include "chpl.h"

class BitVec;
class Expr;
class SymExpr;
class Stmt;
class Symbol;
class FnSymbol;

// A pseudo-singleton.
// Each basic block contains a list of expressions, in and out edges and an index.
// The goto and label maps persist only between calls to buildBasicBlocks.
class BasicBlock {
 public:
  // Since these four variables persist between calls to 
  // buildBasicBlocks() they may contain invalid data if the AST changes.
  static BasicBlock* basicBlock;  // This is the "current" basic block
  // referenced in BB_START(), BB_ADD(), BB_STOP() macros
  // and directly in buildBasicBlocks().
  static Map<LabelSymbol*,Vec<BasicBlock*>*> gotoMaps;
  static Map<LabelSymbol*,BasicBlock*> labelMaps;
  static int nextid;

  int id;
  Vec<Expr*> exprs;
  Vec<BasicBlock*> ins;
  Vec<BasicBlock*> outs;
  BasicBlock();

  bool isOK();
  static void reset(FnSymbol* fn);

  // This function must be called on each function and each time
  // there is a structural change in the AST.
  static void buildBasicBlocks(FnSymbol* fn, Expr* stmt);
  static BasicBlock* Steal();
};

void buildBasicBlocks(FnSymbol* fn);
bool verifyBasicBlocks(FnSymbol* fn);

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
