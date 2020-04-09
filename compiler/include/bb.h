/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
#include <set>

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
  typedef std::set<BasicBlock*> BasicBlockSet;

  //
  // Class methods/variables
  //
public:
  static void        clear(FnSymbol* fn);

  static void        buildBasicBlocks(FnSymbol* fn);

  static void        getReachableBlocks(FnSymbol*      fn,
                                        BasicBlockSet& reachable);

  static void        ignoreUnreachableBlocks(FnSymbol* fn);

  static void        printBasicBlocks(FnSymbol* fn);

  static void        buildLocalsVectorMap(FnSymbol*          fn,
                                          Vec<Symbol*>&      locals,
                                          Map<Symbol*, int>& localMap);

  // Stores the basic block ids in order in according to reverse postorder.
  // Reverse postorder is useful for forward flow analysis
  // to visit a node before any successors
  static void        computeForwardOrder(FnSymbol* fn,
                                         std::vector<int> & order);

  // Stores the basic block ids in order according to reverse preorder.
  // Reverse preorder is useful for backward flow analysis
  // to visit a node after any successors
  static void        computeBackwardOrder(FnSymbol* fn,
                                          std::vector<int> & order);

  static void        backwardFlowAnalysis(FnSymbol*     fn,
                                          BitVecVector& GEN,
                                          BitVecVector& KILL,
                                          BitVecVector& IN,
                                          BitVecVector& OUT);

  static void        forwardFlowAnalysis (FnSymbol*     fn,
                                          BitVecVector& GEN,
                                          BitVecVector& KILL,
                                          BitVecVector& IN,
                                          BitVecVector& OUT,
                                          bool          intersect = true);

  static void        printLocalsVector(Vec<Symbol*>       locals,
                                       Map<Symbol*, int>& localMap);

  static void        printDefsVector(std::vector<SymExpr*> defs,
                                     Map<SymExpr*, int>&   defMap);

  static void        printLocalsVectorSets(BitVecVector& sets,
                                           Vec<Symbol*>  locals);

  static void        printBitVectorSets(BitVecVector& sets);


  static BasicBlock*                          basicBlock;
  static Map<LabelSymbol*, BasicBlock*>       labelMaps;
  static Map<LabelSymbol*, BasicBlockVector*> gotoMaps;

private:
  static void        buildBasicBlocks(FnSymbol* fn,
                                      Expr*     stmt,
                                      bool      mark);
  static void        restart(FnSymbol* fn);
  static void        append(Expr* expr, bool mark);
  static void        thread(BasicBlock* src, BasicBlock* dst);

  static void        reset(FnSymbol* fn);

  static BasicBlock* steal();

  static void        removeEmptyBlocks(FnSymbol* fn);
  static bool        verifyBasicBlocks(FnSymbol* fn);

  static int         nextID;

  //
  // Instance methods/variables
  //
public:
                     BasicBlock();

  void               remove();

  int                id;

  std::vector<Expr*> exprs;
  std::vector<bool>  marks;

  BasicBlockVector   ins;
  BasicBlockVector   outs;

private:
  bool               isOK();
};

#endif
