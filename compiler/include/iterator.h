/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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
  FnSymbol *noniterSA, *noniterL;

  IteratorGroup();
};

void resolveAlsoParallelIterators(FnSymbol* serial, Expr* call);
void verifyIteratorGroup(FnSymbol* it);
void cleanupIteratorGroup(FnSymbol* it);

void showIteratorGroup(IteratorGroup* igroup);
void showIteratorGroup(BaseAST* ast);
void showIteratorGroup(int id);

CallExpr* isSingleLoopIterator(FnSymbol* fn, std::vector<BaseAST*>& asts);
CondStmt* isIBBCondStmt(BaseAST* ast);
void      createIteratorBreakBlocks();
void      addIteratorBreakBlocksInline(Expr* loopRef, Symbol* IC,
                                       BlockStmt* loopBody, CallExpr* yield,
                                       std::vector<Expr*>* delayedRemoval);
BlockStmt* getAndRemoveIteratorBreakBlockForYield(std::vector<Expr*>* delayedRm,
                                                  CallExpr* yield);

/*
 At lowering time, now that we have localized yield symbols, the return symbol
 and the PRIM_RETURN CallExpr are not needed and would cause trouble;
 this function removes them.
 */
void removeRetSymbolAndUses(FnSymbol* fn);
/*
 Creates (and returns) a field to add to an iterator class.
 'type' is used if local==NULL.
 */
Symbol* createICField(int& i, Symbol* local, Type* type,
                      bool isValueField, FnSymbol* fn);
/*
 When converting an iterator that captures outer variables or has state
 between yields, replace the uses of outer variables (captured in formals)
 and state (via local variables) with references to a fields in the given
 class or record.
 */
void replaceLocalUseOrDefWithFieldRef(SymExpr* se,
                                      Symbol* classOrRecord,
                                      std::vector<BaseAST*>& asts,
                                      SymbolMap& local2field,
                                      Vec<SymExpr*>& defSet,
                                      Vec<SymExpr*>& useSet);
/*
 Inserts code to return a value from a function. If the function uses
 an "out parameter" / return arg, uses that, otherwise inserts a real
 return statement.
 */
void insertReturn(FnSymbol* fn, Symbol* toReturn);
/*
 Given a record that contains fields for outer variables captured as
 formals of the builder function, initializes the fields with the corresponding
 formals' values. The outer variable symbols are assumed to be a subset
 of the locals vector.
 */
void initializeRecordFieldWithArgLocals(FnSymbol* fn,
                                        Symbol* rec,
                                        Vec<Symbol*>& locals,
                                        SymbolMap& local2field);

void gatherPrimIRFieldValByFormal();
void cleanupPrimIRFieldValByFormal();

void lowerIterator(FnSymbol* fn);

CallExpr *buildFastFollowerCheckCallForZipOrProm(std::vector<SymExpr *> exprs,
                                                 const char *fnName,
                                                 const char *boolOp,
                                                 Symbol *lead);
CallExpr *generateFastFollowersForZip(CallExpr *iterCall,
                                      Symbol *leadIdxCopy,
                                      SymbolMap *map = NULL,
                                      bool getIterator = true);
CallExpr *generateRegularFollowersForZip(CallExpr *iterCall,
                                         Symbol *leadIdxCopy,
                                         SymbolMap *map = NULL,
                                         bool getIterator = true);
#endif
