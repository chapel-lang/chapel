#ifndef _BB_H_
#define _BB_H_

class BitVec;
class Expr;
class FnSymbol;
class LabelSymbol;
class Stmt;
class Symbol;
class SymExpr;

#include "map.h"

#include <vector>

// Each basic block contains a list of expressions, in and out edges and an index.
// The goto and label maps persist only between calls to buildBasicBlocks.
class BasicBlock
{
  //
  // Typedefs
  //
 public:
  typedef std::vector<BasicBlock*> BasicBlockVector;
  typedef std::vector<BitVec*> BitVecVector;

  //
  // Class methods/variables
  //
public:
  static void               clear(FnSymbol* fn);

  static void               buildBasicBlocks(FnSymbol* fn);

  static void               printBasicBlocks(FnSymbol* fn);

  static void               buildLocalsVectorMap(FnSymbol*             fn,
                                                 Vec<Symbol*>&         locals,
                                                 Map<Symbol*,int>&     localMap);

  static void               backwardFlowAnalysis(FnSymbol*             fn,
                                                 BitVecVector& GEN,
                                                 BitVecVector& KILL,
                                                 BitVecVector& IN,
                                                 BitVecVector& OUT);

  static void               forwardFlowAnalysis (FnSymbol*             fn,
                                                 BitVecVector& GEN,
                                                 BitVecVector& KILL,
                                                 BitVecVector& IN,
                                                 BitVecVector& OUT,
                                                 bool                  intersect = true);

  static void               printLocalsVector(Vec<Symbol*>      locals,
                                              Map<Symbol*,int>& localMap);

  static void               printDefsVector(std::vector<SymExpr*> defs,
                                            Map<SymExpr*, int>&   defMap);

  static void               printLocalsVectorSets(BitVecVector& sets,
                                                  Vec<Symbol*>          locals);

  static void               printBitVectorSets(BitVecVector& sets);

  static BasicBlock*        basicBlock;

  static Map<LabelSymbol*,
             BasicBlock*>   labelMaps;

  static Map<LabelSymbol*,
             BasicBlockVector*> gotoMaps;

private:
  static void               buildBasicBlocks(FnSymbol* fn,
                                             Expr*     stmt,
                                             bool      mark);
  static void               restart(FnSymbol* fn);
  static void               append(Expr* expr, bool mark);
  static void               thread(BasicBlock* src, BasicBlock* dst);

  static void               reset(FnSymbol* fn);

  static BasicBlock*        steal();

  static void               removeEmptyBlocks(FnSymbol* fn);
  static bool               verifyBasicBlocks(FnSymbol* fn);

  static int                nextID;

  //
  // Instance methods/variables
  //
public:
                            BasicBlock();

  int                       id;

  std::vector<Expr*>        exprs;
  std::vector<bool>         marks;

  BasicBlockVector          ins;
  BasicBlockVector          outs;

private:
  bool                      isOK();
};

#endif
