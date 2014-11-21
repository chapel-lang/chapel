/*
 * Copyright 2004-2014 Cray Inc.
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

// A pseudo-singleton.
// Each basic block contains a list of expressions, in and out edges and an index.
// The goto and label maps persist only between calls to buildBasicBlocks.
class BasicBlock {
public:
  //
  // Class methods/state for BasicBlock
  //
  static void              clear(FnSymbol* fn);
  static void              reset(FnSymbol* fn);

  static void              buildBasicBlocks(FnSymbol* fn, Expr* stmt);

  static BasicBlock*       steal();

  static Map<LabelSymbol*, std::vector<BasicBlock*>*> gotoMaps;
  static Map<LabelSymbol*, BasicBlock*>               labelMaps;
  static BasicBlock*                                  basicBlock;
  static int                                          nextid;



  //
  // Instance methods/state on BasicBlock
  //
                           BasicBlock();

  bool                     isOK();

  int                      id;
  std::vector<Expr*>       exprs;
  std::vector<bool>        isEssential;
  std::vector<BasicBlock*> ins;
  std::vector<BasicBlock*> outs;
};

void buildBasicBlocks (FnSymbol* fn);
bool verifyBasicBlocks(FnSymbol* fn);

void buildLocalsVectorMap(FnSymbol*         fn,
                          Vec<Symbol*>&     locals,
                          Map<Symbol*,int>& localMap);

void backwardFlowAnalysis(FnSymbol*             fn,
                          std::vector<BitVec*>& GEN,
                          std::vector<BitVec*>& KILL,
                          std::vector<BitVec*>& IN,
                          std::vector<BitVec*>& OUT);

void forwardFlowAnalysis(FnSymbol*             fn,
                         std::vector<BitVec*>& GEN,
                         std::vector<BitVec*>& KILL,
                         std::vector<BitVec*>& IN,
                         std::vector<BitVec*>& OUT,
                         bool                  intersect = true);

void printBasicBlocks(FnSymbol* fn);

void printLocalsVector(Vec<Symbol*> locals, Map<Symbol*,int>& localMap);
void printDefsVector(std::vector<SymExpr*> defs, Map<SymExpr*,int>& defMap);
void printLocalsVectorSets(std::vector<BitVec*>& sets, Vec<Symbol*> locals);
void printBitVectorSets(std::vector<BitVec*>& sets);

#endif
