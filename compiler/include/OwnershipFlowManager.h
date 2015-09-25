/*
 * Copyright 2004-2015 Cray Inc.
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

#ifndef _CHPL_OWNERSHIP_FLOW_MANAGER_H_
#define _CHPL_OWNERSHIP_FLOW_MANAGER_H_

#include "AliasVectorMap.h"
#include "bb.h"
#include "bitVec.h"

#include <map>
#include <vector>

#define DEBUG_AMM 1

typedef std::vector<BitVec*> FlowSet;

class OwnershipFlowManager
{
public:
       OwnershipFlowManager(FnSymbol* fn);
      ~OwnershipFlowManager();

  void updateFunction();


private:
  typedef BasicBlock::BasicBlockVector    BasicBlockVector;
  typedef std::map<Symbol*, size_t>       SymbolIndexMap;
  typedef SymbolIndexMap::value_type      SymbolIndexElement;
  typedef std::vector<SymExpr*>           SymExprVector;
  typedef std::vector<DefExpr*>           DefExprVector;

  enum FlowSetFlags
  {
    FlowSet_PROD       = 1 << 0,
    FlowSet_CONS       = 1 << 1,
    FlowSet_USE        = 1 << 2,
    FlowSet_USED_LATER = 1 << 3,
    FlowSet_EXIT       = 1 << 4,
    FlowSet_IN         = 1 << 5,
    FlowSet_OUT        = 1 << 6,
    FlowSet_ALL        = 0xff
  };

       OwnershipFlowManager();
       OwnershipFlowManager(const OwnershipFlowManager&);

  void operator=(const OwnershipFlowManager&);

  void buildBasicBlocks();


  void extractSymbols();


  void populateAliases();
  void populateStmtAliases(SymExprVector& symExprs);
  void createAlias(SymExpr* se);


  void createFlowSets();


  void computeExits();
  void computeScopeMap();
  void addInternalDefs();
  void computeExitBlocks();


  void computeTransitions();
  void computeTransitions(BasicBlock&    bb,
                          BitVec*        prod,
                          BitVec*        live,
                          BitVec*        use,
                          BitVec*        cons);
  void computeTransitions(SymExprVector& symExprs,
                          BitVec*        prod,
                          BitVec*        live,
                          BitVec*        use,
                          BitVec*        cons);


  void backwardFlowUse();


  void insertAutoCopies();
  void insertAutoCopy(CallExpr* call,
                      BitVec*   prod,
                      BitVec*   live,
                      BitVec*   cons,
                      Symbol*   rvv,
                      bool      rvvIsOwned);

  void autoCopyToRvvFromSymExpr(CallExpr* call,
                                BitVec*   prod,
                                BitVec*   live,
                                BitVec*   cons);

  void autoCopyToRvvFromPrimop(CallExpr* call);

  void autoCopyForCallExpr(CallExpr* call,
                           BitVec*   prod,
                           BitVec*   live,
                           BitVec*   cons,
                           Symbol*   rvv);

  void insertAutoCopy(SymExpr* se);

  bool isDestructorFormal(SymExpr* se)                                   const;
  bool isDestructorArg(SymExpr* se)                                      const;

  void iteratorInsertAutoDestroys();
  void iteratorInsertAutoDestroys(BitVec*        toCons,
                                  BitVec*        cons,
                                  BasicBlock*    bb);
  void iteratorInsertAutoDestroys(BitVec*        toCons,
                                  BitVec*        cons,
                                  SymExprVector& symExprs);


  void forwardFlowOwnership();


  void insertAutoDestroys();
  void insertAutoDestroy(BitVec* toCons);
  void insertAutoDestroyAtScopeExit(Symbol* sym);
  void insertAtOtherExitPoints(Symbol*   sym,
                               CallExpr* autoDestroyCall);


  void processCreator(SymExpr* se,
                      BitVec*  prod,
                      BitVec*  live);

  void processUser(SymExpr* se,
                   BitVec*  use);

  void processConsumer(SymExpr* se,
                       BitVec*  live,
                       BitVec*  cons);

  void processBitwiseCopy(SymExpr*  se,
                          BitVec*   prod,
                          BitVec*   live,
                          BitVec*   cons);

  void processBitwiseCopy(CallExpr* call,
                          BitVec*   prod,
                          BitVec*   live,
                          BitVec*   cons);

  void setAliasList  (BitVec*       bits,
                      SymbolVector& aliasList);

  void resetAliasList(BitVec*       bits,
                      SymbolVector& aliasList);

  bool isLocal(SymExpr* expr)                                            const;
  bool isLocal(Symbol*  sym)                                             const;

  // Debug support.
  void printInfo()                                                       const;

  void printSymbols()                                                    const;

  void printBasicBlocks();

  void printFlowSets();

  void printFlowSets(FlowSetFlags flags);

  void printSymbolStats(Symbol* sym);
  void printSymbolStats(size_t index);
  void printSymbolStats(Symbol* sym, size_t index);

  FnSymbol*         _fn;         // Records the function being analyzed
  Symbol*           fnRetSym;    // The return symbol of _fn.

  // The vector of basic blocks is a property of the function being analyzed.
  // It is copied here for convience.  This pointer must be whenever basic
  // block analysis is re-run.
  BasicBlockVector* basicBlocks; // Cached basic block vector (un-owned).
  size_t            nbbs;        // Cached basic block count.
                                 //  Valid after BB analysis is run.

  // A vector of symbols of interest in this function.
  // The bits in each  BitVec in the flow sets correspond to symbols in this
  // vector.
  SymbolVector      symbols;
  size_t            nsyms;       // Cached symbol count.
                                 // Valid after <symbols> is populated.
  SymbolIndexMap    symbolIndex; // Map from a symbol to its index in BitVecs.

  // The map contains one entry for each symbol.
  // Each entry is a list of symbols with which the given symbol is an alias.
  AliasVectorMap    aliases;

  // These fields are described in detail immediately below
  FlowSet           PROD;
  FlowSet           CONS;
  FlowSet           USE;
  FlowSet           USED_LATER;
  FlowSet           EXIT;
  FlowSet           IN;
  FlowSet           OUT;

  // PROD(i,j) is true iff symbol j gains ownership of an object in block i.
  //  PROD is valid after forward flow analysis has converged.
  //  A producer is an insn that assigns ownership of an object to a symbol.
  //  Each symbol should have only one producer on any given path.  This is a
  //  "duh" for user-defined symbols since initialization is associated with
  //  declaration -- meaning that ownership of a symbol is established before
  //  any conditional node; therefore this invariant is obeyed trivially.
  //  Internally, however, at present we break this rule for the return value
  //  variable.  It is declared at the beginning of the function but is
  //  initialized (in routines that do not declare an explicit return type)
  //  at each yield or return statement.
  //  If the invariant is violated, it is an internal error, since the user
  //  cannot write code that will cause a variable to be initialized later in
  //  the flow.  This analysis treats "noinit" initialization as
  //  initialization.  The language itself also obeys this rule, since it
  //  provides no syntax by which a user can call a constructor on an
  //  already-constructed object.

  // CONS(i,j) is true iff symbol j is consumed at least once in block i.
  //  CONS is valid after forward flow anaysis has converged.
  //  A consumer is an insn that receives ownership of the given symbol.
  //  There can be multiple consumers on a given flow path.  A given insn can
  //  consume multple symbols, or even the same symbol multiple times.
  //  A given symbol need not be owned on entry to a given consumer insn.  If
  //  the "owned" bit for that symbol is false, then an autocopy is inserted
  //  so the ownership requirement of the consumer is satisfied.  Multiple
  //  autocopies may be inserted if multiple symbols are consumed or if one
  //  symbol is consumed multiply.
  //  If the "owned" bit for that symbol is true, then the symbol can be
  //  consumed without requiring an autocopy.  This is true only if consumption
  //  of that symbol is coincident with its last use.  Supposing, for example,
  //  that a symbol is consumed twice in one insn (and not consumed later in
  //  the flow), an autocopy would have to be inserted for the first use;
  //  ownership can be transferred in the second use.

  // USE(i,j) is true iff block i contains a read of symbol j.
  //  Currently references are not tracked, but use of symbol j in an "addr_of"
  //  primitive is considered to be a read.  As long as "addr_of" and the use
  //  of the generated address occur within the same block, we can avoid the
  //  extra complexity of reference tracking.

  // USED_LATER(i,j) is true iff symbol j is used in the flow following block
  // i.  In a block with multiple successors, it is true if USE or USED_LATER
  // is true for *any* of its successors.

  // EXIT(i,j) is true iff block i is associated with the end-of-scope of
  // symbol j.  For this to be useful, we may need to insert empty join
  // blocks at the end of scopes containing "interesting" flows.

  // IN(i,j) is true iff symbol j is already owned on entry to block i.
  //  Valid after forward and backward flows have converged.

  // OUT(i,j) is true iff symbol j must be owned on exit from block i.
  //  Valid after forward and backward flows have converged.

#if DEBUG_AMM
  // Debug level: 0 - off; 1 - verbose; 2 - very verbose.
  unsigned debug;
#endif
};

#endif
