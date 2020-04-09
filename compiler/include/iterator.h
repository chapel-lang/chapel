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

#ifndef _ITERATOR_H_
#define _ITERATOR_H_
#include "symbol.h"
#include "vec.h"

class BaseAST;
class AggregateType;
class FnSymbol;
class CallExpr;

class IteratorInfo {
public:
  IteratorInfo();

  FnSymbol*      iterator;
  FnSymbol*      getIterator;
  AggregateType* iclass;
  AggregateType* irecord;
  FnSymbol*      advance;
  FnSymbol*      zip1;
  FnSymbol*      zip2;
  FnSymbol*      zip3;
  FnSymbol*      zip4;
  FnSymbol*      hasMore;
  FnSymbol*      getValue;
  FnSymbol*      init;
  FnSymbol*      incr;

  Type*          yieldedType;
  RetTag         iteratorRetTag;
};

void cleanupIteratorInfo(FnSymbol* host);

class IteratorGroup {
public:
  FnSymbol* serial;
  FnSymbol* standalone;
  FnSymbol* leader;
  FnSymbol* follower;

  // Search for a standalone/leader gave a non-iterator/forwarder.
  bool noniterSA, noniterL;

  IteratorGroup();
};

void resolveAlsoParallelIterators(FnSymbol* serial, Expr* call);
void verifyIteratorGroup(FnSymbol* it);
void cleanupIteratorGroup(FnSymbol* it);

void showIteratorGroup(IteratorGroup* igroup);
void showIteratorGroup(BaseAST* ast);
void showIteratorGroup(int id);

CallExpr* isSingleLoopIterator(FnSymbol* fn, Vec<BaseAST*>& asts);
CondStmt* isIBBCondStmt(BaseAST* ast);
void      createIteratorBreakBlocks();
void      addIteratorBreakBlocksInline(Expr* loopRef, Symbol* IC,
                                       BlockStmt* loopBody, CallExpr* yield,
                                       std::vector<Expr*>* delayedRemoval);
BlockStmt* getAndRemoveIteratorBreakBlockForYield(std::vector<Expr*>* delayedRm,
                                                  CallExpr* yield);
void gatherPrimIRFieldValByFormal();
void cleanupPrimIRFieldValByFormal();

void lowerIterator(FnSymbol* fn);

#endif
