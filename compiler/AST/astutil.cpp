/*
 * Copyright 2020-2026 Hewlett Packard Enterprise Development LP
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

#include "astutil.h"

#include "baseAST.h"
#include "CatchStmt.h"
#include "CForLoop.h"
#include "fcf-support.h"
#include "DecoratedClassType.h"
#include "DeferStmt.h"
#include "ForallStmt.h"
#include "ForLoop.h"
#include "IfExpr.h"
#include "ImportStmt.h"
#include "iterator.h"
#include "expr.h"
#include "LoopExpr.h"
#include "passes.h"
#include "ParamForLoop.h"
#include "resolution.h"
#include "stlUtil.h"
#include "stmt.h"
#include "symbol.h"
#include "TemporaryConversionThunk.h"
#include "TryStmt.h"
#include "type.h"
#include "virtualDispatch.h"
#include "wellknown.h"
#include "WhileStmt.h"

#include "global-ast-vecs.h"

#include <set>
#include <vector>

static bool shouldPruneAggregateType(TypeSymbol* ts, Vec<TypeSymbol*>& types);
static bool shouldPruneRefType(TypeSymbol* ts, Vec<TypeSymbol*>& types);
static void pruneUnusedAggregateTypes(Vec<TypeSymbol*>& types);
static void pruneUnusedRefs(Vec<TypeSymbol*>& types);
static void pruneStaleFunctionTypes(Vec<TypeSymbol*>& types);


void collectFnCalls(BaseAST* ast, std::vector<CallExpr*>& calls) {
  AST_CHILDREN_CALL(ast, collectFnCalls, calls);
  if (CallExpr* call = toCallExpr(ast))
    if (call->isResolved())
      calls.push_back(call);
}

void collectVirtualAndFnCalls(BaseAST* ast, std::vector<CallExpr*>& calls) {
  AST_CHILDREN_CALL(ast, collectFnCalls, calls);
  if (CallExpr* call = toCallExpr(ast))
    if (call->resolvedOrVirtualFunction() != NULL)
      calls.push_back(call);
}


void collectExprs(BaseAST* ast, std::vector<Expr*>& exprs) {
  AST_CHILDREN_CALL(ast, collectExprs, exprs);
  if (Expr* expr = toExpr(ast))
    exprs.push_back(expr);
}

void collect_stmts(BaseAST* ast, std::vector<Expr*>& stmts) {
  if (Expr* expr = toExpr(ast)) {
    stmts.push_back(expr);
    if (isBlockStmt(expr) || isCondStmt(expr)) {
      AST_CHILDREN_CALL(ast, collect_stmts, stmts);
    }
  }
}

void collectDefExprs(BaseAST* ast, std::vector<DefExpr*>& defExprs) {
  AST_CHILDREN_CALL(ast, collectDefExprs, defExprs);
  if (DefExpr* defExpr = toDefExpr(ast))
    defExprs.push_back(defExpr);
}

void collectDefExprs(BaseAST* ast, llvm::SmallVectorImpl<DefExpr*>& defExprs) {
  AST_CHILDREN_CALL(ast, collectDefExprs, defExprs);
  if (DefExpr* defExpr = toDefExpr(ast))
    defExprs.push_back(defExpr);
}

void collectForallStmts(BaseAST* ast, std::vector<ForallStmt*>& forallStmts) {
  AST_CHILDREN_CALL(ast, collectForallStmts, forallStmts);
  if (ForallStmt* forall = toForallStmt(ast))
    forallStmts.push_back(forall);
}

void collectCForLoopStmtsPreorder(BaseAST* ast, std::vector<CForLoop*>& cforloopStmts) {
  if (CForLoop* cforloop = toCForLoop(ast))
    cforloopStmts.push_back(cforloop);
  AST_CHILDREN_CALL(ast, collectCForLoopStmtsPreorder, cforloopStmts);
}

void collectCallExprs(BaseAST* ast, std::vector<CallExpr*>& callExprs) {
  AST_CHILDREN_CALL(ast, collectCallExprs, callExprs);
  if (CallExpr* callExpr = toCallExpr(ast))
    callExprs.push_back(callExpr);
}

void collectCallExprsForGpuEligibilityAnalysis(BaseAST* ast, std::vector<CallExpr*>& callExprs) {
  if (auto blk = toBlockStmt(ast)) {
    if (blk->isGpuPrimitivesBlock()) {
      return;
    }
  }

  // Skip code in the "then" branch of a GPU-CPU token, since only the "else"
  // branch will be codegenerated for the GPU.
  if (auto cond = toCondStmt(ast)) {
    if (SymExpr* se = toSymExpr(cond->condExpr)) {
      if (se->symbol() == gCpuVsGpuToken) {
        ast = cond->elseStmt;
      }
    }
  }

  AST_CHILDREN_CALL(ast, collectCallExprsForGpuEligibilityAnalysis, callExprs);
  if (CallExpr* callExpr = toCallExpr(ast))
    callExprs.push_back(callExpr);
}

void collectBlockStmts(BaseAST* ast, std::vector<BlockStmt*>& blockStmts) {
  AST_CHILDREN_CALL(ast, collectBlockStmts, blockStmts);
  if (BlockStmt* blockStmt = toBlockStmt(ast))
    blockStmts.push_back(blockStmt);
}

void collectForLoops(BaseAST* ast, std::vector<ForLoop*>& forLoops) {
  AST_CHILDREN_CALL(ast, collectForLoops, forLoops);
  if (ForLoop* forLoop = toForLoop(ast))
    forLoops.push_back(forLoop);
}

void collectMyCallExprs(BaseAST* ast, std::vector<CallExpr*>& callExprs,
                           FnSymbol* parent_fn) {
  AST_CHILDREN_CALL(ast, collectMyCallExprs, callExprs, parent_fn);
  if (CallExpr* callExpr = toCallExpr(ast))
    if (callExpr->parentSymbol == parent_fn)
      callExprs.push_back(callExpr);
}

void collectGotoStmts(BaseAST* ast, std::vector<GotoStmt*>& gotoStmts) {
  AST_CHILDREN_CALL(ast, collectGotoStmts, gotoStmts);
  if (GotoStmt* gotoStmt = toGotoStmt(ast))
    gotoStmts.push_back(gotoStmt);
}


// This is a specialized helper for lowerIterators.
// Collects the gotos whose target is inTree() into 'GOTOs' and
// the iterator break blocks into 'IBBs'.
void collectTreeBoundGotosAndIteratorBreakBlocks(BaseAST* ast,
                                                 std::vector<GotoStmt*>& GOTOs,
                                                 std::vector<CondStmt*>& IBBs) {
  if (CondStmt* condStmt = isIBBCondStmt(ast)) {
    IBBs.push_back(condStmt);
    // Do not descend into the IBB to avoid its "goto return".
    // We do not expect it to contain nested IBBs.
    return;
  }

  AST_CHILDREN_CALL(ast, collectTreeBoundGotosAndIteratorBreakBlocks, GOTOs,
                                                                      IBBs);
  // Include only the gotos whose target is inTree().
  if (GotoStmt* gt = toGotoStmt(ast))
    if (SymExpr* labelSE = toSymExpr(gt->label))
      if (labelSE->symbol()->inTree())
        GOTOs.push_back(gt);
}

std::set<Symbol*> findAllDetupledComponents(Symbol* sym) {
  std::set<Symbol*> ret;

  if (!sym->typeInfo()->symbol->hasFlag(FLAG_TUPLE) ||
      !sym->hasFlag(FLAG_TEMP)) {
    return ret;
  }

  for_SymbolSymExprs(se1, sym) {
    auto c1 = toCallExpr(se1->parentExpr);
    if (c1 && c1->baseExpr == se1) {
      auto c2 = toCallExpr(c1->parentExpr);
      if (c2 && c2->isPrimitive(PRIM_MOVE)) {
        auto seTemp = toSymExpr(c2->get(1));
        if (seTemp && seTemp->symbol()->hasFlag(FLAG_TEMP)) {
          for_SymbolSymExprs(se2, seTemp->symbol()) {
            auto c3 = toCallExpr(se2->parentExpr);
            if (c3 && c3->isPrimitive(PRIM_INIT_VAR) &&
                c3 != c2 &&
                c3->get(2) == se2) {
              auto seFound = toSymExpr(c3->get(1));
              INT_ASSERT(seFound);
              auto sym = seFound->symbol();
              INT_ASSERT(ret.find(sym) == ret.end());
              ret.insert(sym);
            }
          }
        }
      }
    }
  }
  return ret;
}

//
// This is a specialized helper for lowerForalls.
// Traverse the body of the iterator looking for "top-level" yields.
// Do not descend into calls as they should not contain inner yields.
// Do not descend into a forall loop because any yields in it will be
// handled when lowering the forall and inlining **its** iterator.
// Yields in a parallel construct will not come up upon this traversal
// because parallel constructs are represented with calls to task functions.
// While task functions have been outlined by now, lowerForalls places
// a clone of a task function right next to a call to it, and this
// traversal does not descend into symbols.
//
void computeHasToplevelYields(BaseAST* ast, bool& result) {
  if (result)
    return;
  else if (CallExpr* call = toCallExpr(ast))
    result = call->isPrimitive(PRIM_YIELD); // do not dig further
  else if (isSymbol(ast) || isForallStmt(ast))
    ; // do not descend into these
  else
    AST_CHILDREN_CALL(ast, computeHasToplevelYields, result);
}


void collectSymExprs(BaseAST* ast, std::vector<SymExpr*>& symExprs) {
  AST_CHILDREN_CALL(ast, collectSymExprs, symExprs);
  if (SymExpr* symExpr = toSymExpr(ast))
    symExprs.push_back(symExpr);
}

void collectSymExprs(BaseAST* ast, llvm::SmallVectorImpl<SymExpr*>& symExprs) {
  AST_CHILDREN_CALL(ast, collectSymExprs, symExprs);
  if (SymExpr* symExpr = toSymExpr(ast))
    symExprs.push_back(symExpr);
}

// Same as collectSymExprs(), including only SymExprs for 'sym'.
void collectSymExprsFor(BaseAST* ast, Symbol* sym,
                        std::vector<SymExpr*>& symExprs) {
  AST_CHILDREN_CALL(ast, collectSymExprsFor, sym, symExprs);
  if (SymExpr* symExpr = toSymExpr(ast))
    if (symExpr->symbol() == sym)
      symExprs.push_back(symExpr);
}

// Same as collectSymExprs(), including only SymExprs for 'sym1' and 'sym2'.
void collectSymExprsFor(BaseAST* ast, const Symbol* sym1, const Symbol* sym2,
                        std::vector<SymExpr*>& symExprs) {
  AST_CHILDREN_CALL(ast, collectSymExprsFor, sym1, sym2, symExprs);
  if (SymExpr* symExpr = toSymExpr(ast))
    if (symExpr->symbol() == sym1 || symExpr->symbol() == sym2)
      symExprs.push_back(symExpr);
}

// Same as collectSymExprs(), including only LcnSymbols.
void collectLcnSymExprs(BaseAST* ast, std::vector<SymExpr*>& symExprs) {
  AST_CHILDREN_CALL(ast, collectLcnSymExprs, symExprs);
  if (SymExpr* symExpr = toSymExpr(ast))
    if (isLcnSymbol(symExpr->symbol()))
      symExprs.push_back(symExpr);
}

void collectSymbols(BaseAST* ast, std::vector<Symbol*>& symbols) {
  AST_CHILDREN_CALL(ast, collectSymbols, symbols);
  if (Symbol* symbol = toSymbol(ast))
    symbols.push_back(symbol);
}

void collect_asts_preorder(BaseAST* ast, std::vector<BaseAST*>& asts) {
  asts.push_back(ast);
  AST_CHILDREN_CALL(ast, collect_asts, asts);
}

void collect_asts(BaseAST* ast, std::vector<BaseAST*>& asts) {
  collect_asts_preorder(ast, asts);
}

void collect_asts_unorder(BaseAST* ast, std::vector<BaseAST*>& asts) {
  collect_asts_preorder(ast, asts);
}

void collect_asts_postorder(BaseAST* ast, std::vector<BaseAST*>& asts) {
  AST_CHILDREN_CALL(ast, collect_asts_postorder, asts);
  asts.push_back(ast);
}

static void collect_top_asts_internal(BaseAST*               ast,
                                      std::vector<BaseAST*>& asts) {
  if (!isSymbol(ast) || isArgSymbol(ast)) {
    AST_CHILDREN_CALL(ast, collect_top_asts_internal, asts);
    asts.push_back(ast);
  }
}

//
// Collect only top-level AST nodes, i.e., expressions but not
// symbols, but also includes arg symbols and types (not sure why).
// In fact, current uses of this function seem like they could be
// replaced by more specific traversals implemented in this file.
// Something to check out another day.
//
void collect_top_asts(BaseAST* ast, std::vector<BaseAST*>& asts) {
  AST_CHILDREN_CALL(ast, collect_top_asts_internal, asts);
  asts.push_back(ast);
}

static void do_containsSymExprFor(BaseAST* ast, Symbol* sym, SymExpr** found) {
  AST_CHILDREN_CALL(ast, do_containsSymExprFor, sym, found);
  if (SymExpr* symExpr = toSymExpr(ast))
    if (symExpr->symbol() == sym)
      *found = symExpr;
}

// returns true if the AST contains a SymExpr pointing to sym
SymExpr* findSymExprFor(BaseAST* ast, Symbol* sym) {
  SymExpr* ret = NULL;
  do_containsSymExprFor(ast, sym, &ret);
  return ret;
}

void reset_ast_loc(BaseAST* destNode, BaseAST* sourceNode) {
  reset_ast_loc(destNode, sourceNode->astloc);
}

void reset_ast_loc(BaseAST* destNode, astlocT astlocArg) {
  destNode->astloc = astlocArg;
  AST_CHILDREN_CALL(destNode, reset_ast_loc, astlocArg);
}

// Gather into fn->calledBy all direct calls to 'fn'.
// This is a specialization of computeAllCallSites()
// for use during resolveDynamicDispatches().
void computeNonvirtualCallSites(FnSymbol* fn) {
  if (fn->calledBy == NULL) {
    fn->calledBy = new Vec<CallExpr*>();
  }

  INT_ASSERT(virtualRootsMap.get(fn) == NULL);

  for_SymbolSymExprs(se, fn) {
    if (CallExpr* call = toCallExpr(se->parentExpr)) {
      if (fn == call->resolvedFunction()) {
        fn->calledBy->add(call);

      } else if (call->isPrimitive(PRIM_FTABLE_CALL)) {
        // sjd: do we have to do anything special here?
        //      should this call be added to some function's calledBy list?

      } else if (call->isPrimitive(PRIM_VIRTUAL_METHOD_CALL)) {
        FnSymbol* vFn = toFnSymbol(toSymExpr(call->get(1))->symbol());
        if (vFn == fn) {
          INT_FATAL(call, "unexpected case calling %s", fn->name);
        }
      }
    }
  }
}

// Gather into fn->calledBy all calls to 'fn', regular or virtual,
// and all virtual calls to all its virtual parents, if any.
void computeAllCallSites(FnSymbol* fn) {
  Vec<CallExpr*>* calledBy = fn->calledBy;
  if (calledBy == NULL)
    fn->calledBy = calledBy = new Vec<CallExpr*>();
  else
    calledBy->clear();

  for_SymbolSymExprs(se, fn) {
    if (CallExpr* call = toCallExpr(se->parentExpr)) {
      if (fn == call->resolvedFunction()) {
        calledBy->add(call);

      } else if (call->isPrimitive(PRIM_FTABLE_CALL)) {
        // sjd: do we have to do anything special here?
        //      should this call be added to some function's calledBy list?

      } else if (call->isPrimitive(PRIM_VIRTUAL_METHOD_CALL)) {
        FnSymbol* vFn = toFnSymbol(toSymExpr(call->get(1))->symbol());
        if (vFn == fn)
          calledBy->add(call);
      }
    }
  }

  // Add all virtual calls on parents.
  if (Vec<FnSymbol*>* parents = virtualParentsMap.get(fn))
    forv_Vec(FnSymbol, pfn, *parents)
      for_SymbolSymExprs(pse, pfn)
        if (CallExpr* pcall = toCallExpr(pse->parentExpr))
          if (pcall->isPrimitive(PRIM_VIRTUAL_METHOD_CALL) &&
              pfn == toFnSymbol(toSymExpr(pcall->get(1))->symbol()))
            calledBy->add(pcall);
}

// computeAllCallSites() for all FnSymbols.
void compute_call_sites() {
  for_alive_in_Vec(FnSymbol, fn, gFnSymbols)
    computeAllCallSites(fn);
}

// builds the def and use maps for every variable/argument
// in the entire program.
void buildDefUseMaps(Map<Symbol*,Vec<SymExpr*>*>& defMap,
                     Map<Symbol*,Vec<SymExpr*>*>& useMap) {
  // Collect the set of symbols to track by extracting all var and arg
  // symbols from among all def expressions.
  Vec<Symbol*> symSet;
  forv_Vec(DefExpr, def, gDefExprs) {
    if (def->parentSymbol) {
      if (isLcnSymbol(def->sym)) {
        symSet.set_add(def->sym);
      }
    }
  }

  // The set of uses is the set of all SymExprs.
  buildDefUseMaps(symSet, defMap, useMap);
}


// Within the current AST element, recursively collect
// the set of all symbols appearing in var or arg declarations
// and the set of all uses (i.e. SymExprs).
void collectSymbolSetSymExprVec(BaseAST* ast,
                                Vec<Symbol*>& symSet,
                                Vec<SymExpr*>& symExprs) {
  if (DefExpr* def = toDefExpr(ast)) {
    if (isLcnSymbol(def->sym)) {
      symSet.set_add(def->sym);
    }
  } else if (SymExpr* se = toSymExpr(ast)) {
    symExprs.add(se);
  }
  AST_CHILDREN_CALL(ast, collectSymbolSetSymExprVec, symSet, symExprs);
}

void collectSymbolSet(BaseAST* ast, Vec<Symbol*>& symSet) {
  if (DefExpr* def = toDefExpr(ast)) {
    if (isLcnSymbol(def->sym)) {
      symSet.set_add(def->sym);
    }
  }
  AST_CHILDREN_CALL(ast, collectSymbolSet, symSet);
}

void collectSymbolSet(BaseAST* ast, std::set<Symbol*>& symSet) {
  if (DefExpr* def = toDefExpr(ast)) {
    if (isLcnSymbol(def->sym)) {
      symSet.insert(def->sym);
    }
  }
  AST_CHILDREN_CALL(ast, collectSymbolSet, symSet);
}

void collectSymbolSet(BaseAST* ast, llvm::SmallPtrSetImpl<Symbol*>& symSet) {
  if (DefExpr* def = toDefExpr(ast)) {
    if (isLcnSymbol(def->sym)) {
      symSet.insert(def->sym);
    }
  }
  AST_CHILDREN_CALL(ast, collectSymbolSet, symSet);
}


// builds the vectors for every variable/argument in 'fn' and looks
// for uses and defs only in 'fn'
void buildDefUseMaps(FnSymbol* fn,
                     Map<Symbol*,Vec<SymExpr*>*>& defMap,
                     Map<Symbol*,Vec<SymExpr*>*>& useMap) {
  Vec<Symbol*> symSet;
  // Collect symbols within the given function
  collectSymbolSet(fn, symSet);
  buildDefUseMaps(symSet, defMap, useMap);
}

// builds the vectors for every variable declaration in the given block
// and looks for uses and defs within the same block (scope).
void buildDefUseMaps(BlockStmt* block,
                     Map<Symbol*,Vec<SymExpr*>*>& defMap,
                     Map<Symbol*,Vec<SymExpr*>*>& useMap) {
  Vec<Symbol*> symSet;
  // Collect symbols and sym expressions only within the given block.
  collectSymbolSet(block, symSet);
  buildDefUseMaps(symSet, defMap, useMap);
}

static void addUseOrDef(Map<Symbol*,Vec<SymExpr*>*>& ses, SymExpr* se) {
  Vec<SymExpr*>* sev = ses.get(se->symbol());
  if (sev) {
    sev->add(se);
  } else {
    sev = new Vec<SymExpr*>();
    sev->add(se);
    ses.put(se->symbol(), sev);
  }
}


void addDef(Map<Symbol*,Vec<SymExpr*>*>& defMap, SymExpr* def) {
  addUseOrDef(defMap, def);
}


void addUse(Map<Symbol*,Vec<SymExpr*>*>& useMap, SymExpr* use) {
  addUseOrDef(useMap, use);
}


//
// Checks if a callExpr is one of the op= primitives
// Note, this does not check if a callExpr is an
// op= function call (such as before inlining)
//
bool isOpEqualPrim(CallExpr* call) {
  if (call->isPrimitive(PRIM_ADD_ASSIGN)        ||
      call->isPrimitive(PRIM_SUBTRACT_ASSIGN)   ||
      call->isPrimitive(PRIM_MULT_ASSIGN)       ||
      call->isPrimitive(PRIM_DIV_ASSIGN)        ||
      call->isPrimitive(PRIM_MOD_ASSIGN)        ||
      call->isPrimitive(PRIM_LSH_ASSIGN)        ||
      call->isPrimitive(PRIM_RSH_ASSIGN)        ||
      call->isPrimitive(PRIM_AND_ASSIGN)        ||
      call->isPrimitive(PRIM_OR_ASSIGN)         ||
      call->isPrimitive(PRIM_XOR_ASSIGN)        ||
      call->isPrimitive(PRIM_LOGICALAND_ASSIGN) ||
      call->isPrimitive(PRIM_LOGICALOR_ASSIGN)
      ) {
    return true;
  }
  //otherwise false
  return false;
}

bool isMoveOrAssign(CallExpr* call) {
  return call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN);
}

//
// Returns true if:
// - the call is a PRIM_MOVE or PRIM_ASSIGN
// - The LHS and RHS are both SymExprs
// - The LHS is not a reference
// - the RHS is a reference
//
bool isDerefMove(CallExpr* call) {
  return isMoveOrAssign(call)                    &&
         isSymExpr(call->get(2))                 &&
         call->get(1)->isRefOrWideRef() == false &&
         call->get(2)->isRefOrWideRef() ==  true;
}

bool isNewLike(CallExpr* call) {
  return call->isPrimitive(PRIM_NEW) ||
         call->isPrimitive(PRIM_NEW_WITH_ALLOCATOR);
}


//
// Check if a callExpr is a relational operator primitive
// (==, !=, <=, >=, <, >)
//
bool isRelationalOperator(CallExpr* call) {
  if (call->isPrimitive(PRIM_EQUAL)          ||
      call->isPrimitive(PRIM_NOTEQUAL)       ||
      call->isPrimitive(PRIM_LESSOREQUAL)    ||
      call->isPrimitive(PRIM_GREATEROREQUAL) ||
      call->isPrimitive(PRIM_LESS)           ||
      call->isPrimitive(PRIM_GREATER)) {
    return true;
  }
  //otherwise false
  return false;

}

static constexpr int DEF = 1;
static constexpr int USE = 2;
static constexpr int DEF_USE = 3;

static int computeDefAndOrUseDirectCall(FnSymbol* fn, SymExpr* se) {
  auto ret = USE;

  if (auto arg = actual_to_formal(se)) {
    if (arg->intent == INTENT_REF   ||
        arg->intent == INTENT_INOUT ||
        (fn->name == astrSassign &&
         fn->getFormal(1) == arg &&
         isRecord(arg->type))) {
      // BHARSH TODO: get rid of this 'isRecord' special case
      ret = DEF_USE;
    } else if (arg->intent == INTENT_OUT) {
      ret = DEF;
    }
  }

  // OK to fall through.
  return ret;
}

static int computeDefAndOrUseFromFunctionType(CallExpr* call, SymExpr* se) {
  auto fn = call->resolvedFunction();
  auto ft = call->functionType();
  INT_ASSERT(ft);

  auto ret = USE;

  int zeroBasedFormalIdx = -1;
  if (auto formal = ft->formalByOrdinal(se, &zeroBasedFormalIdx)) {
    if (fn && fn->name == astrSassign && zeroBasedFormalIdx == 0 &&
        isRecord(formal->type())) {
      // BHARSH TODO: get rid of this 'isRecord' special case
      ret = DEF_USE;
    } else if (formal->intent() == INTENT_INOUT ||
               formal->intent() == INTENT_REF) {
      ret = DEF_USE;
    } else if (formal->intent() == INTENT_OUT) {
      ret = DEF;
    }
  }

  // OK to fall through.
  return ret;
}

//
// TODO this should be fixed to include PRIM_SET_MEMBER
// See notes in iterator.cpp and/or loopInvariantCodeMotion.cpp
// TODO this should also be fixed to include the PRIM_SET_SVEC_MEMBER
// which gets inserted from the returnStartTuplesByRefArgs pass
//  an attempt to do so is in the commented-out sections below
//  but would require also fixing a bug in copy-propagation
//  with e.g. functions/deitz/nested/test_nested_var_iterator2.chpl
// TODO handle PRIM_FTABLE_CALL
//
// return & 1 is true if se is a def
// return & 2 is true if se is a use
//
// Note that a DefExpr is where we hang the variable declaration, but after
// normalize, a DefExpr itself does not set a variable, and so it does not
// count as a Def.
int isDefAndOrUse(SymExpr* se) {
  if (CallExpr* call = toCallExpr(se->parentExpr)) {
    bool isFirstActual = (call->numActuals() && call->get(1) == se);
    auto fn = call->resolvedFunction();

    if (se->symbol() == fn && call->baseExpr == se) {
      // The use is of a function that is called.
      return USE;
    }

    // TODO: PRIM_SET_MEMBER, PRIM_SET_SVEC_MEMBER

    if (call->isPrimitive(PRIM_ASSIGN)) {
      if (isFirstActual) {
        if (se->isRef()) {
          return DEF_USE; // *(se) = ?;
        } else {
          return DEF; // se = var;
        }
      } else {
        return USE; // ? = se;
      }
    } else if (call->isPrimitive(PRIM_MOVE)) {
      if (isFirstActual) {
        if (se->isRef()) {
          if (call->get(2)->isRef() == false) {
            return DEF_USE; // *(se) = var;
          } else {
            return DEF; // se = ref; just copying the pointer
          }
        } else {
          return DEF; // se = ?;
        }
      } else {
        return USE; // ? = se;
      }
    } else if (call->isPrimitive(PRIM_END_OF_STATEMENT)) {
      return 0; // neither def nor use
    } else if (isOpEqualPrim(call) && isFirstActual) {
      // Both a def and a use:
      // se   =   se <op> ?
      // ^-def    ^-use
      return DEF_USE;
    } else if (call->isPrimitive(PRIM_VIRTUAL_METHOD_CALL)) {

      if (se == call->get(1) || se == call->get(2)) {
        // actual_to_formal() breaks if passed the cid argument, or the
        // 'FnSymbol' representing the static method type.
        return USE;
      }

      // same as for resolvedFunction()
      ArgSymbol* arg = actual_to_formal(se);
      if (arg->intent == INTENT_REF ||
          arg->intent == INTENT_INOUT)
        return DEF_USE;
      else if (arg->intent == INTENT_OUT)
        return DEF;

    } else if ((fn && fn->isUsedAsValue()) || call->isIndirectCall()) {
      auto ret = computeDefAndOrUseFromFunctionType(call, se);

      if (fVerify && fn) {
        // This is the old code, but we keep it around in a '--verify' branch
        // to make sure that we're computing the correct result above.
        auto check = computeDefAndOrUseDirectCall(fn, se);
        INT_ASSERT(check == ret);
      }

      return ret;

    } else if (fn) {
      return computeDefAndOrUseDirectCall(fn, se);
    }
  }

  return USE;
}


void buildDefUseMaps(Vec<Symbol*>& symSet,
                     Map<Symbol*,Vec<SymExpr*>*>& defMap,
                     Map<Symbol*,Vec<SymExpr*>*>& useMap) {
  forv_Vec(Symbol, sym, symSet) {
    if (sym == NULL) continue;

    for_SymbolSymExprs(se, sym) {
      if (se->inTree() && sym == se->symbol()) {
        int result = isDefAndOrUse(se);

        if (result & 1)
          addDef(defMap, se);

        if (result & 2)
          addUse(useMap, se);
      }
    }
  }
}

typedef Map<Symbol*,Vec<SymExpr*>*> SymbolToVecSymExprMap;
typedef MapElem<Symbol*,Vec<SymExpr*>*> SymbolToVecSymExprMapElem;

void freeDefUseMaps(Map<Symbol*,Vec<SymExpr*>*>& defMap,
                    Map<Symbol*,Vec<SymExpr*>*>& useMap) {
  form_Map(SymbolToVecSymExprMapElem, e, defMap) {
    delete e->value;
  }
  defMap.clear();
  form_Map(SymbolToVecSymExprMapElem, e, useMap) {
    delete e->value;
  }
  useMap.clear();
}


static void
buildDefUseSetsInternal(BaseAST* ast,
                        Vec<Symbol*>& symSet,
                        Vec<SymExpr*>& defSet,
                        Vec<SymExpr*>& useSet) {
  if (SymExpr* se = toSymExpr(ast)) {
    if (se->inTree() && se->symbol() && symSet.set_in(se->symbol())) {
      int result = isDefAndOrUse(se);
      if (result & 1)
        defSet.set_add(se);
      if (result & 2)
        useSet.set_add(se);
    }
  }
  AST_CHILDREN_CALL(ast, buildDefUseSetsInternal, symSet, defSet, useSet);
}


void buildDefUseSets(Vec<Symbol*>& syms,
                     FnSymbol* fn,
                     Vec<SymExpr*>& defSet,
                     Vec<SymExpr*>& useSet) {
  Vec<Symbol*> symSet;
  forv_Vec(Symbol, sym, syms) {
    symSet.set_add(sym);
  }
  buildDefUseSetsInternal(fn, symSet, defSet, useSet);
}


void subSymbol(BaseAST* ast, Symbol* oldSym, Symbol* newSym) {
  if (SymExpr* se = toSymExpr(ast))
    if (se->symbol() == oldSym)
      se->setSymbol(newSym);
  AST_CHILDREN_CALL(ast, subSymbol, oldSym, newSym);
}


void sibling_insert_help(BaseAST* sibling, BaseAST* ast) {
  Expr* parentExpr = NULL;
  Symbol* parentSymbol = NULL;
  if (Expr* expr = toExpr(sibling)) {
    parentExpr = expr->parentExpr;
    parentSymbol = expr->parentSymbol;
  } else if (sibling)
    INT_FATAL(ast, "major error in sibling_insert_help");
  if (parentSymbol)
    insert_help(ast, parentExpr, parentSymbol);
}


void parent_insert_help(BaseAST* parent, Expr* ast) {
  if (!parent || !parent->inTree())
    return;
  Expr* parentExpr = NULL;
  Symbol* parentSymbol = NULL;
  if (Expr* expr = toExpr(parent)) {
    parentExpr = expr;
    parentSymbol = expr->parentSymbol;
  } else if (Symbol* symbol = toSymbol(parent)) {
    parentSymbol = symbol;
  } else if (Type* type = toType(parent)) {
    parentSymbol = type->symbol;
  } else
    INT_FATAL(ast, "major error in parent_insert_help");
  insert_help(ast, parentExpr, parentSymbol);
}


void insert_help(BaseAST* ast,
                 Expr* parentExpr,
                 Symbol* parentSymbol) {
  if (Symbol* sym = toSymbol(ast)) {
    parentSymbol = sym;
    parentExpr = NULL;
  } else if (Expr* expr = toExpr(ast)) {
    if (SymExpr* se = toSymExpr(expr)) {
      Symbol* symbol = se->symbol();
      if (symbol && parentSymbol && !expr->parentSymbol) {
        // Add the SymExpr to the list if the SymExpr is
        // being added to the tree
        symbol->addSymExpr(se);
      }
    }
    expr->parentSymbol = parentSymbol;
    expr->parentExpr = parentExpr;
    parentExpr = expr;
  }
  AST_CHILDREN_CALL(ast, insert_help, parentExpr, parentSymbol);
}


void remove_help(BaseAST* ast, int trace_flag) {
  trace_remove(ast, trace_flag);
  AST_CHILDREN_CALL(ast, remove_help, trace_flag);
  if (Expr* expr = toExpr(ast)) {
    if (SymExpr* se = toSymExpr(expr)) {
      Symbol* symbol = se->symbol();
      if (symbol && expr->parentSymbol) {
        // Remove the SymExpr from the list if the SymExpr is
        // being removed from the tree.
        symbol->removeSymExpr(se);
      }
    }
    expr->parentSymbol = NULL;
    expr->parentExpr = NULL;
  } else if (LabelSymbol* labsym = toLabelSymbol(ast)) {
    if (labsym->iterResumeGoto)
      removedIterResumeLabels.add(labsym);
  }
}


// Return the corresponding Symbol in the formal list of the actual a
ArgSymbol*
actual_to_formal(Expr *a) {
  if (CallExpr *call = toCallExpr(a->parentExpr)) {
    if (call->isResolved()) {
      for_formals_actuals(formal, actual, call) {
        if (a == actual)
          return formal;
      }
    } else if (call->isPrimitive(PRIM_VIRTUAL_METHOD_CALL)) {
      FnSymbol* fn = toFnSymbol(toSymExpr(call->get(1))->symbol());
      int i = 0;
      for_actuals(actual, call) {
        i++;
        if (a == actual)
          return fn->getFormal(i-2);
      }
    }
  }

  return nullptr;
}


Expr* formal_to_actual(CallExpr* call, Symbol* arg) {
  if (call->isResolved()) {
    for_formals_actuals(formal, actual, call) {
      if (arg == formal)
        return actual;
    }
    INT_FATAL(call, "couldn't find arg");
  } else {
    INT_FATAL(call, "formal_to_actual invoked with unresolved call");
  }
  return NULL;
}

bool givesType(Symbol* sym) {
  bool retval = false;

  if (isTypeSymbol(sym) == true) {
    retval = true;

  } else if (sym->hasFlag(FLAG_TYPE_VARIABLE) == true) {
    retval = true;

  } else if (FnSymbol* fn = toFnSymbol(sym)) {
    retval = fn->retTag == RET_TYPE;
  }

  return retval;
}

static bool isNumericTypeSymExpr(Expr* expr) {
  if (SymExpr* se = toSymExpr(expr)) {
    Symbol* sym = se->symbol();
    Type* t = sym->type;
    // if it's the actual type symbol for that type
    if (t->symbol == sym && sym->hasFlag(FLAG_TYPE_VARIABLE))
      return isIntType(t) ||
             isUIntType(t) ||
             isRealType(t) ||
             isImagType(t) ||
             isComplexType(t);
  }

  return false;
}

bool isExternType(Type* t) {
  // narrow references are OK but not wide references
  if (t->isWideRef())
    return false;

  if (isFunctionType(t) && fcfs::usePointerImplementation()) {
    return false;
  }

  ClassTypeDecoratorEnum d = ClassTypeDecorator::UNMANAGED_NONNIL;
  // unmanaged or borrowed classes are OK
  if (isClassLikeOrManaged(t) || isClassLikeOrPtr(t))
    d = removeNilableFromDecorator(classTypeDecorator(t));

  TypeSymbol* ts = t->symbol;

  EnumType* et = toEnumType(t);

  return t->isRef() ||
         d == ClassTypeDecorator::BORROWED ||
         d == ClassTypeDecorator::UNMANAGED ||
         (et && et->isConcrete()) ||
         (ts->hasFlag(FLAG_TUPLE) && ts->hasFlag(FLAG_STAR_TUPLE)) ||
         ts->hasFlag(FLAG_GLOBAL_TYPE_SYMBOL) ||
         ts->hasFlag(FLAG_DATA_CLASS) ||
         ts->hasFlag(FLAG_C_PTR_CLASS) ||
         ts->hasFlag(FLAG_C_ARRAY) ||
         ts->hasFlag(FLAG_EXTERN) ||
         ts->hasFlag(FLAG_EXPORT); // these don't exist yet
}

bool isExportableType(Type* t) {

  // TODO: Exporting will need a different representation of FCF types.
  if (t->symbol->hasFlag(FLAG_FUNCTION_CLASS)) return false;
  if (t == dtString || t == dtBytes) {
    // string/bytes are OK in export functions
    // because they are converted to wrapper
    // functions
    return true;
  }

  return isExternType(t);
}

bool isTypeExpr(Expr* expr) {
  bool retval = false;

  if (SymExpr* sym = toSymExpr(expr)) {
    retval = givesType(sym->symbol());

  } else if (CallExpr* call = toCallExpr(expr)) {
    if (call->isPrimitive(PRIM_TYPEOF) == true) {
      retval = true;

    } else if (call->isPrimitive(PRIM_GET_MEMBER_VALUE) == true ||
               call->isPrimitive(PRIM_GET_MEMBER)       == true) {
      SymExpr*       left = toSymExpr(call->get(1));
      Type*          t    = canonicalDecoratedClassType(left->getValType());
      AggregateType* ct   = toAggregateType(t);

      INT_ASSERT(ct != NULL);

      if (left->symbol()->type->symbol->hasFlag(FLAG_TUPLE) == true &&
          left->symbol()->hasFlag(FLAG_TYPE_VARIABLE)       == true) {
        retval = true;

      } else {
        SymExpr*   right = toSymExpr(call->get(2));
        VarSymbol* var   = toVarSymbol(right->symbol());

        if (var->isType() == true) {
          retval = true;

        } else if (var->immediate != NULL) {
          const char* name = var->immediate->v_string.c_str();
          Symbol*     field = ct->getField(name);

          retval = field->hasFlag(FLAG_TYPE_VARIABLE);
        }
      }

    } else if (call->isPrimitive(PRIM_GET_RUNTIME_TYPE_FIELD)) {
      bool isType = false;
      getPrimGetRuntimeTypeFieldReturnType(call, isType);
      retval = isType;

    } else if (call->isPrimitive(PRIM_COERCE)) {
      retval = isTypeExpr(call->get(1));

    } else if (call->numActuals() == 1 &&
               call->baseExpr &&
               isNumericTypeSymExpr(call->baseExpr)) {
      // e.g. call 'int' 64 is a type expression (resulting in int(64))
      retval = true;

    } else if (FnSymbol* fn = call->resolvedFunction()) {
      retval = fn->retTag == RET_TYPE;

    // TODO: Is it safe to just check if the base expression is a type?
    } else if (isTypeConstructorWithRuntimeTypeActual(call)) {
      retval = true;
    }
  }

  return retval;
}

bool isTypeConstructorWithRuntimeTypeActual(CallExpr* call) {
  if (!call || call->isPrimitive()) return false;

  auto se = toSymExpr(call->baseExpr);
  if (!se || !se->symbol()->hasFlag(FLAG_TYPE_VARIABLE)) return false;

  for_actuals(actual, call)
    if (auto se = toSymExpr(actual))
      if (auto sym = se->symbol()->type->symbol)
        if (sym->hasFlag(FLAG_HAS_RUNTIME_TYPE)) return true;

  return false;
}

static void pruneVisit(TypeSymbol*       ts,
                       Vec<FnSymbol*>&   fns,
                       Vec<TypeSymbol*>& types);

static void pruneVisitFn(FnSymbol*         fn,
                         Vec<FnSymbol*>&   fns,
                         Vec<TypeSymbol*>& types);

static void
pruneVisit(Symbol* sym, Vec<FnSymbol*>& fns, Vec<TypeSymbol*>& types) {
  if (isFnSymbol(sym)) {
    pruneVisitFn(toFnSymbol(sym), fns, types);
  } else {
    pruneVisit(sym->type->symbol, fns, types);
    if (sym->isRef() && !sym->type->symbol->hasFlag(FLAG_REF)) {
      pruneVisit(sym->type->refType->symbol, fns, types);
    }
  }
}


static void
pruneVisit(TypeSymbol* ts, Vec<FnSymbol*>& fns, Vec<TypeSymbol*>& types) {
  if (types.set_in(ts)) return;
  types.set_add(ts);
  llvm::SmallVector<DefExpr*, 32> defExprs;
  collectDefExprs(ts, defExprs);
  for (DefExpr* def : defExprs) {
    if (def->sym->type)
      pruneVisit(def->sym, fns, types);
  }
  if (ts->hasFlag(FLAG_DATA_CLASS))
    pruneVisit(getDataClassType(ts), fns, types);
}


static void
pruneVisitFn(FnSymbol* fn, Vec<FnSymbol*>& fns, Vec<TypeSymbol*>& types) {
  if (fns.set_in(fn)) return;
  fns.set_add(fn);

  if (fn->retType) types.set_add(fn->retType->symbol);

  llvm::SmallVector<SymExpr*, 16> symExprs;
  collectSymExprs(fn, symExprs);
  for(SymExpr* se : symExprs) {
    if (FnSymbol* next = toFnSymbol(se->symbol()))
      if (!fns.set_in(next))
        pruneVisit(next, fns, types);
    if (se->symbol() && se->symbol()->type)
      pruneVisit(se->symbol(), fns, types);
  }
  for_formals(formal, fn) {
    pruneVisit(formal, fns, types);
  }
}


static ModuleSymbol* getToplevelModule(FnSymbol* fn) {
  if (fn->defPoint == nullptr) {
    return nullptr;
  }

  // find the top-level module
  ModuleSymbol* topLevelModule = nullptr;
  for (ModuleSymbol* cur = fn->defPoint->getModule();
       cur && cur->defPoint;
       cur = cur->defPoint->getModule()) {
    if (isModuleSymbol(cur) && cur != theProgram && cur != rootModule) {
      topLevelModule = cur;
    }
  }

  return topLevelModule;
}

static bool separatelyCompilingModule(ModuleSymbol* topLevelModule) {
  return gDynoGenLibModuleNameAstrs.count(topLevelModule->name) > 0;
}

static bool keepFnForSeparateCompilation(FnSymbol* fn) {
  if (!fDynoGenLib) {
    return false;
  }

  ModuleSymbol* mod = getToplevelModule(fn);
  if (mod && separatelyCompilingModule(mod)) {
    // Workaround: don't keep 'iteratorIndex' functions in
    // ChapelIteratorSupport because these can call 'getValue' functions
    // that contain partial and invalid AST.
    if (0 == strcmp(mod->name, "ChapelIteratorSupport") &&
        0 == strcmp(fn->name, "iteratorIndex")) {
      return false;
    }

    // generally, keep functions in modules being separately compiled
    return true;
  }
  return false;
}

// Visit and mark functions (and types) which are reachable from
// externally visible symbols.
static void
visitVisibleFunctions(Vec<FnSymbol*>& fns, Vec<TypeSymbol*>& types)
{
  // chpl_gen_main is always visible (if it exists).
  if (chpl_gen_main)
    pruneVisit(chpl_gen_main, fns, types);

  // printModuleInitOrder function is always visible
  INT_ASSERT(gPrintModuleInitFn);
  pruneVisit(gPrintModuleInitFn, fns, types);

  // Functions appearing the function pointer table are visible.
  // These are blocks that can be started through a forall, coforall,
  // or on statement.
  for (FnSymbol* fn : ftableVec)
    pruneVisit(fn, fns, types);

  // Mark VFT entries as visible.
  for (int i = 0; i < virtualMethodTable.n; i++)
    if (virtualMethodTable.v[i].key)
      for (int j = 0; j < virtualMethodTable.v[i].value->n; j++)
        pruneVisit(virtualMethodTable.v[i].value->v[j], fns, types);

  // Mark things to consider always visible:
  //  * exported symbols
  //  * always-resolve functions
  //  * functions in modules being separately compiled
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_EXPORT) ||
        fn->hasFlag(FLAG_ALWAYS_RESOLVE) ||
        keepFnForSeparateCompilation(fn)) {
      pruneVisit(fn, fns, types);
    }
  }

  // Mark well-known functions as visible
  std::vector<FnSymbol*> wellKnownFns = getWellKnownFunctions();
  for_vector(FnSymbol, fn, wellKnownFns) {
    pruneVisit(fn, fns, types);
  }

  pruneVisitFn(gAddModuleFn, fns, types);
  forv_Vec(ModuleSymbol, mod, gModuleSymbols) {
    if (mod->initFn)
      pruneVisitFn(mod->initFn, fns, types);
    if (mod->deinitFn)
      pruneVisitFn(mod->deinitFn, fns, types);
  }
}


static void
pruneUnusedTypes(Vec<TypeSymbol*>& types)
{
  pruneStaleFunctionTypes(types);
  pruneUnusedAggregateTypes(types);
  pruneUnusedRefs(types);
  cleanupAfterTypeRemoval();
}

static bool shouldPruneAggregateType(TypeSymbol* ts, Vec<TypeSymbol*>& types) {
  // Do not delete reference types.
  // We delete the reference type if the base type is dead (below).
  if (ts->hasFlag(FLAG_REF)) return false;

  // Ignore types flagged as primitive.
  if (ts->hasFlag(FLAG_PRIMITIVE_TYPE)) return false;

  // Ignore types that are not class types
  if (!isAggregateType(ts->type)) return false;

  // Do not delete the object class.  It's pretty crucial.
  if (ts->hasFlag(FLAG_OBJECT_CLASS)) return false;

  // Visit only those types not marked as visible.
  if (types.set_in(ts)) return false;

  return true;
}

static void pruneUnusedAggregateTypes(Vec<TypeSymbol*>& types) {
  //
  // delete unused AggregateType types, only deleting references to such
  // types when the value types are deleted
  //
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (shouldPruneAggregateType(ts, types)) ts->defPoint->remove();
  }
}

static bool shouldPruneRefType(TypeSymbol* ts, Vec<TypeSymbol*>& types) {
  // This pass, we are interested only in ref types.
  if (!ts->hasFlag(FLAG_REF)) return false;

  // Ignore types flagged as primitive.
  if (ts->hasFlag(FLAG_PRIMITIVE_TYPE)) return false;

  // Ignore types that are not class types
  if (!isAggregateType(ts->type)) {
    // hilde sez: Ref types are always class types.
    // So we can't get here.
    INT_ASSERT(false);
    return false;
  }

  // Visit only those types not marked as visible.
  if (types.set_in(ts)) return false;

  if (Type* vt = ts->getValType()) {
    // Don't remove a ref type if it refers to a class type
    if (isAggregateType(vt))
      // and the class type is still alive.
      if (types.set_in(vt->symbol)) return false;

    // Don't delete nil ref as it is used in widening.
    if (vt == dtNil) return false;

    // Ok, we can remove the ref type.
    return true;
  }

  return false;
}

static void pruneUnusedRefs(Vec<TypeSymbol*>& types) {
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (shouldPruneRefType(ts, types)) {
      Type* vt = ts->getValType();
      INT_ASSERT(vt);
      // Unlink reference type from value type.
      vt->refType = NULL;
      ts->defPoint->remove();
    }
  }
}

static void pruneStaleFunctionTypes(Vec<TypeSymbol*>& types) {
  std::set<Type*> removed;

  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    auto ft = toFunctionType(ts->type);
    if (!ft || !ts->inTree()) continue;

    // Visit only those types not marked as visible.
    if (types.set_in(ts)) continue;

    // A function type should not be pruned if it is simply "unused". This is
    // because many function types are "unused" but still exist attached to
    // a 'FnSymbol*'. Instead, it should be pruned if it is _stale_: that is,
    // if its return type or any formal type is unused.
    auto shouldPrune = [&](Type* t) {
      return !t->inTree() ||
        shouldPruneRefType(t->symbol, types) ||
        shouldPruneAggregateType(t->symbol, types);
    };

    bool prune = shouldPrune(ft->returnType());

    if (!prune && ft->numFormals() > 0) {
      for (int i = 0; i < ft->numFormals(); i++) {
        auto t = ft->formal(i)->type();
        prune = shouldPrune(t);
        if (prune) break;
      }
    }

    if (prune) {
      removed.insert(ts->type);
      ts->defPoint->remove();
    }
  }
}

static bool shouldRemoveSymBeforeCodeGeneration(Symbol* sym) {
  if (!sym || !sym->inTree() || !sym->type) return false;

  // Do not remove symbols with at least one use.
  if (sym->firstSymExpr()) return false;

  // Do not remove formals or functions.
  if (isArgSymbol(sym) || isFnSymbol(sym)) return false;

  // Do not remove fields.
  if (isTypeSymbol(sym->defPoint->parentSymbol)) return false;

  if (sym->type == dtUninstantiated &&
      sym != dtUninstantiated->symbol) {
    return true;
  }
  return false;
}

void cleanupAfterTypeRemoval() {
  //
  // change symbols with dead types to void (important for baseline)
  //
  forv_Vec(DefExpr, def, gDefExprs) {
    auto sym = def->sym;
    if (!def->inTree() || !sym || !sym->type) continue;

    bool canChange = !sym->type->symbol->inTree() &&
                     (isAggregateType(sym->type) ||
                      isFunctionType(sym->type)) &&
                     !isTypeSymbol(sym);
    if (canChange) sym->type = dtNothing;

    // Some types must never reach code generation, as they have no runtime
    // representation. E.g., 'uninstantiated' is one of these types.
    // Temporaries can be introduced with this type when resolving methods
    // over any partially-instantiated type.
    if (shouldRemoveSymBeforeCodeGeneration(sym)) def->remove();
  }

  // Clear out any uses of removed types in Type::substitutionsPostResolve
  for_alive_in_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (AggregateType* at = toAggregateType(ts->type)) {
      for (size_t i = 0; i < at->substitutionsPostResolve.size(); i++) {
        NameAndSymbol& ns = at->substitutionsPostResolve[i];
        if (ns.value && !ns.value->inTree()) {
          ns.value = dtNothing->symbol;
        }
      }
    }
  }

  // and in FnSymbol::substitutionsPostResolve
  for_alive_in_Vec(FnSymbol, fn, gFnSymbols) {
    for (size_t i = 0; i < fn->substitutionsPostResolve.size(); i++) {
      NameAndSymbol& ns = fn->substitutionsPostResolve[i];
      if (ns.value && !ns.value->inTree()) {
        ns.value = dtNothing->symbol;
      }
    }
  }
}

static void removeVoidMoves()
{
  forv_Vec(CallExpr, call, gCallExprs)
  {
    if (! call->isPrimitive(PRIM_MOVE))
      continue;

    SymExpr* se = toSymExpr(call->get(1));
    if (se->symbol()->type != dtNothing)
      continue;

    // the RHS of the move could be a function with side effects.
    // So, if it is a call, just remove the move, but leave the call.
    if (CallExpr* rhsCall = toCallExpr(call->get(2))) {
      rhsCall->remove();
      call->replace(rhsCall);
    } else {
      call->remove();
    }
  }
}

static void removeStatementLevelUsesOfNone() {
  for_SymbolSymExprs(se, gNone) {
    if (se == se->getStmtExpr()) se->remove();
  }
}

// Determine sets of used functions and types, and then delete
// functions which are not visible and classes which are not used.
void
prune() {
  Vec<FnSymbol*> fns;     // This receives the set of used functions.
  Vec<TypeSymbol*> types; // This receives the set of used types.

  visitVisibleFunctions(fns, types);

  //
  // delete unused functions
  //
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (!fns.set_in(fn))
      fn->defPoint->remove();
  }

  pruneUnusedTypes(types);

  removeVoidMoves();

  removeStatementLevelUsesOfNone();
}


// Done this way to make the pass name for each pass unique.
void prune2() { prune(); } // Synonym for prune.

/*
 * Takes a call that is a PRIM_SVEC_GET_MEMBER* and returns the symbol of the
 * field. Normally the call is something of the form PRIM_SVEC_GET_MEMBER(p, 0)
 * and what this function gets out is the symbol that is the first field
 * instead of just the number 0.
 */
Symbol* getSvecSymbol(CallExpr* call) {
  INT_ASSERT(call->isPrimitive(PRIM_GET_SVEC_MEMBER)       ||
             call->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE) ||
             call->isPrimitive(PRIM_SET_SVEC_MEMBER));
  Type* type = call->get(1)->getValType();
  AggregateType* tuple = toAggregateType(type);
  SymExpr* fieldVal = toSymExpr(call->get(2));
  VarSymbol* fieldSym = toVarSymbol(fieldVal->symbol());
  if (fieldSym) {
    int immediateVal = fieldSym->immediate->int_value();

    INT_ASSERT(immediateVal >= 0 && immediateVal < tuple->fields.length);
    return tuple->getField(immediateVal+1);
  } else {
    // GET_SVEC_MEMBER(p, i), where p is a star tuple
    return NULL;
  }
}


static void addToUsedFnSymbols(std::set<FnSymbol*>& fnSymbols,
                               FnSymbol*            newFn) {
  if(fnSymbols.count(newFn) == 0) {
    fnSymbols.insert(newFn);
    AST_CHILDREN_CALL(newFn->body, collectUsedFnSymbols, fnSymbols);
  }
}

/*
* Collect all of the functions in the call graph at and below the function
* call.
*/
void collectUsedFnSymbols(BaseAST* ast, std::set<FnSymbol*>& fnSymbols) {
  AST_CHILDREN_CALL(ast, collectUsedFnSymbols, fnSymbols);

  // if there is a function call, get the FnSymbol associated with it
  // and look through that FnSymbol for other function calls. Do not
  // look through an already visited FnSymbol, or you'll have an infinite
  // loop in the case of recursion.
  if (CallExpr* call = toCallExpr(ast)) {
    if (FnSymbol* fn = call->resolvedFunction()) {
      addToUsedFnSymbols(fnSymbols, fn);

    } else if (call->isPrimitive(PRIM_FTABLE_CALL)) {
      //
      // TODO: We'd like a way to accurately find the set of functions that
      // might be called from a PRIM_FTABLE_CALL. The complicated nature
      // of recursive iterator lowering seems to be too big of an obstacle
      // right now.
      //
      for (FnSymbol* fn : ftableVec) {
        addToUsedFnSymbols(fnSymbols, fn);
      }
    }
  }
}

static QualifiedType computeFlattenedRefType(QualifiedType qt) {
  Qualifier retQual = qt.getQual();
  Type* retType = qt.type();

  bool isRefType = qt.type()->symbol->hasEitherFlag(FLAG_REF, FLAG_WIDE_REF);

  if (qt.isRefOrWideRef() && isRefType) {
    Qualifier q = qt.getQual();
    const bool isRefQual = QualifiedType::qualifierIsRef(q);

    if (!isRefQual) {
      if (qt.isWideRefType()) {
        q = QUAL_WIDE_REF;
      } else {
        q = QUAL_REF;
      }
      retQual = q;
    }

    retType = retType->getValType();
  }

  QualifiedType ret = { retQual, retType };

  return ret;
}

static void setQualRef(Symbol* sym) {
  auto qt1 = sym->qualType();
  auto qt2 = computeFlattenedRefType(qt1);

  if (qt1 != qt2) {
    // Only write on a change.
    if (qt1.getQual() != qt2.getQual()) sym->qual = qt2.getQual();

    // Only write on a change.
    if (qt1.type() != qt2.type()) sym->type = qt2.type();

    if (qt2.isRefOrWideRef()) {
      if (ArgSymbol* arg = toArgSymbol(sym)) {
        // Invariant should already be maintained, so just assert.
        if (qt2.isConst()) {
          INT_ASSERT(arg->intent == INTENT_CONST_REF);
        } else {
          // TODO: Tuple types are weird here...
          INT_ASSERT(arg->intent == INTENT_REF_MAYBE_CONST ||
                     arg->intent == INTENT_REF);
        }
      }
    }
  }
}

// TODO: At this point I've written this pattern enough that we could use a
//       more general purpose component-transformer sort of utility...
static FunctionType* flattenRefsForFunctionTypes(FunctionType* ft) {
  FunctionType* ret = ft;

  std::vector<FunctionType::Formal> newFormals;
  bool changed = false;

  for (auto& formal : ft->formals()) {
    auto qt1 = formal.qualType();
    auto qt2 = computeFlattenedRefType(qt1);

    newFormals.push_back({ qt2.getQual(), qt2.type(),
                           formal.intent(),
                           formal.name(), formal.flags() });
    changed = changed || qt1 != qt2;
  }

  if (changed) {
    // If any formal type changed, then recompute the function type.
    SET_LINENO(ft->symbol);
    auto newFt = FunctionType::get(ft->kind(), ft->width(), ft->linkage(),
                                   std::move(newFormals),
                                   ft->returnIntent(),
                                   ft->returnType(),
                                   ft->throws());
    INT_ASSERT(ft != newFt);
    ret = newFt;
  }

  return ret;
}

class FlattenRefsInProcPtrTypes : public AdjustSymbolTypes {
 public:
  Type* computeAdjustedType(Type* t) const override {
    if (auto ft = toFunctionType(t->getValType())) {
      auto ret = flattenRefsForFunctionTypes(ft);
      return ret;
    }
    return t;
  };
};

void convertToQualifiedRefs() {
#define fixRefSymbols(SymType) \
  forv_Vec(SymType, sym, g##SymType##s) { \
    setQualRef(sym); \
  }

  fixRefSymbols(VarSymbol);
  fixRefSymbols(ArgSymbol);
  fixRefSymbols(ShadowVarSymbol);

  // TODO: Merge the above traversal with this pass to save a walk.
  PassManager pm;
  runPassOverAllSymbols(pm, FlattenRefsInProcPtrTypes());

  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_ADDR_OF)) {
      call->primitive = primitives[PRIM_SET_REFERENCE];
    }
  }
#undef fixRefSymbols
}

bool shouldWarnUnstableFor(BaseAST* ast) {
  if (auto mod = ast->getModule()) {
    if (mod->modTag == MOD_INTERNAL) return fWarnUnstableInternal;
    else if (mod->modTag == MOD_STANDARD) return fWarnUnstableStandard;
  }
  return fWarnUnstable;
}

// this is `symExprIsUsedAsConstRef` when `constRef=true`
bool symExprIsUsedAsRef(
  SymExpr* use,
  bool constRef,
  std::function<bool(SymExpr*, CallExpr*)> checkForMove) {
   if (CallExpr* call = toCallExpr(use->parentExpr)) {
    if (FnSymbol* calledFn = call->resolvedFunction()) {
      ArgSymbol* formal = actual_to_formal(use);

      if(constRef) {
        // generally, use const-ref-return if passing to const ref formal
        if (formal->intent == INTENT_CONST_REF) {
          // but make an exception for initCopy calls
          if (calledFn->hasFlag(FLAG_INIT_COPY_FN))
            return false;

          // TODO: tuples of types with blank intent
          // being 'in' should perhaps use the value version.
          return true;
        }
      } else {
        if (formal->intent == INTENT_REF ||
          formal->intent == INTENT_OUT ||
          formal->intent == INTENT_INOUT) {
          return true;
        }
      }

    } else if (call->isPrimitive(PRIM_RETURN) ||
               call->isPrimitive(PRIM_YIELD)) {
      FnSymbol* inFn = toFnSymbol(call->parentSymbol);

      auto retTag = constRef ? RET_CONST_REF : RET_REF;

      // use const-ref-return if returning by const ref intent
      if (inFn->retTag == retTag)
        return true;

    } else if (call->isPrimitive(PRIM_WIDE_GET_LOCALE) ||
               call->isPrimitive(PRIM_WIDE_GET_NODE)) {
      // If we are extracting a field from the wide pointer,
      // we need to keep it as a pointer.

      // use const-ref-return if querying locale
      return true;

    } else {
      // Check for the case that sym is moved to a compiler-introduced
      // variable, possibly with PRIM_MOVE tmp, PRIM_ADDR_OF sym
      if (call->isPrimitive(PRIM_ADDR_OF) ||
          call->isPrimitive(PRIM_SET_REFERENCE) ||
          call->isPrimitive(PRIM_GET_MEMBER) ||
          call->isPrimitive(PRIM_GET_SVEC_MEMBER))
        call = toCallExpr(call->parentExpr);

      if (call->isPrimitive(PRIM_MOVE) && checkForMove(use, call)) {
        return true;
      }
    }
  }
  return false;
}

static Type*
computeNewSymbolType(std::unordered_map<Type*, Type*>& alreadyAdjusted,
                     AdjustTypeFn adjustTypeFn,
                     bool preserveRefLevels,
                     Type* t) {
  // This is a helper function that memoizes the result of performing the
  // caller's type adjustment function. Additionally, if the type returned
  // by the caller's function is a function type, this function will
  // attempt to adjust the types embedded in the function type as well.

  // Memoization - return the already computed type if it is stored.
  auto it = alreadyAdjusted.find(t);
  if (it != alreadyAdjusted.end()) return it->second;

  Type* ret = t;
  if (preserveRefLevels && t != t->getValType()) {
    // For reference types, unpack and adjust the value type instead.
    auto vt1 = t->getValType();
    auto vt2 = computeNewSymbolType(alreadyAdjusted, adjustTypeFn,
                                    preserveRefLevels, vt1);
    INT_ASSERT(vt2);

    // TODO: Do wide classes need any sort of special treatment?
    ret = matchRefLevel(vt2, t);

  } else {
    // Otherwise, just perform the caller-given adjustment function.
    ret = adjustTypeFn(t);
  }

  INT_ASSERT(ret);

  // The type may still need adjustment if it embeds other 'Type*' without
  // use of AST (e.g., not 'VarSymbol' fields in 'AggregateType').
  // TODO: Do other subclass of Type that embed AST need special help here?
  if (auto ft = toFunctionType(ret)) {
    std::vector<FunctionType::Formal> newFormals;
    bool anyChanged = false;

    for (int i = 0; i < ft->numFormals(); i++) {
      auto f = ft->formal(i);
      auto newT = computeNewSymbolType(alreadyAdjusted, adjustTypeFn,
                                       preserveRefLevels,
                                       f->type());
      if (newT != f->type()) {
        newFormals.push_back({ f->qual(), newT, f->intent(), f->name(), f->flags() });
        anyChanged = true;
      } else {
        newFormals.push_back(*f);
      }
    }

    auto newRetType = computeNewSymbolType(alreadyAdjusted, adjustTypeFn,
                                           preserveRefLevels,
                                           ft->returnType());
    anyChanged = anyChanged || newRetType != ft->returnType();

    if (anyChanged) {
      // If any types changed, then recompute the function type.
      SET_LINENO(ft->symbol);
      auto newFt = FunctionType::get(ft->kind(), ft->width(), ft->linkage(),
                                     std::move(newFormals),
                                     ft->returnIntent(),
                                     newRetType,
                                     ft->throws());
      INT_ASSERT(ft != newFt);
      ret = newFt;
    }
  }

  // Emplace the result.
  alreadyAdjusted.emplace_hint(it, t, ret);

  return ret;
}

static SymbolNameVec* ptrToSubsOrNull(Symbol* sym) {
  if (auto ts = toTypeSymbol(sym)) {
    return &ts->type->substitutionsPostResolve;
  } else if (auto fn = toFnSymbol(sym)) {
    return &fn->substitutionsPostResolve;
  }
  return nullptr;
}

static
Type* doAdjustSymbolType(std::unordered_map<Type*, Type*>& alreadyAdjusted,
                         Symbol* sym,
                         AdjustTypeFn adjustTypeFn,
                         bool preserveRefLevels) {
  // Wrapper that computes what the new type of the symbol should be.
  auto doComputeType = [&](Type* t) {
    return computeNewSymbolType(alreadyAdjusted, adjustTypeFn,
                                preserveRefLevels, t);
  };

  Type* oldType = sym->type;
  Type* ret = doComputeType(sym->type);
  bool canModifySubstitutions = true;
  bool setType = true;

  if (isTypeSymbol(sym)) {
    // Do not set the 'type' field of a type symbol.
    setType = false;
  }

  if (ret->symbol->hasFlag(FLAG_WIDE_REF) ||
      ret->symbol->hasFlag(FLAG_REF)) {
    // Do not modify substitutions in reference types. Compute a new type.
    canModifySubstitutions = false;
  }

  if (auto vs = toVarSymbol(sym)) {
    auto parent = vs->defPoint->parentSymbol;
    if (parent && isTypeSymbol(parent)) {
      if (parent->hasFlag(FLAG_REF) || parent->hasFlag(FLAG_WIDE_REF)) {
        // Do not modify fields in 'ref' or 'wide-ref' formals.
        //
        // TODO: In general, modifying fields and substitutions ends up making
        //       the AST super wonky and hard to follow. The reason is because
        //       we do not recompute certain properties such as the 'name' of
        //       aggregates after we change things in them. For 'ref' types
        //       this is extra egregious because they are used everywhere.
        setType = false;
      }
    }
  }

  if (auto fn = toFnSymbol(sym)) {
    // Recompute the return type.
    Type* newRetT = doComputeType(fn->retType);
    if (newRetT != fn->retType) fn->retType = newRetT;

    if (fn->iteratorInfo && !fn->hasFlag(FLAG_TASK_FN_FROM_ITERATOR_FN)) {
      // Recompute the yielded type, but only if this is a 'host' function
      // that owns its 'IteratorInfo' and not a e.g., 'on_fn' that inherits
      // the info from a parent function.
      Type* newYieldT = doComputeType(fn->iteratorInfo->yieldedType);
      if (newYieldT != fn->iteratorInfo->yieldedType)
        fn->iteratorInfo->yieldedType = newYieldT;
    }

    if (!fn->isUsedAsValue()) {
      // Clear types for functions not used as values.
      ret = dtUnknown;
    }
  }

  if (canModifySubstitutions) {
    // TODO: Either we shouldn't be modifying subs at all, or we should be
    //       recomputing certain properties of a type like the name after
    //       we do so, because this leads to desync. E.g., for a tuple type,
    //       if we change the component type 2*int -> 2*real, the name will
    //       still print out at '2*int'. This is confusing when debugging
    //       and from a structural perspective. I think we ought to make
    //       entirely new, immutable types and deal with the consequences,
    //       or do a better job of using (recomputable) properties e.g.,
    //       '->name()' instead of just precomputing.
    if (auto subsPtr = ptrToSubsOrNull(sym)) {
      auto& subs = *subsPtr;

      size_t n = subs.size();
      for (size_t i = 0; i < n; i++) {
        auto& ns = subs[i];
        if (TypeSymbol* ets = toTypeSymbol(ns.value)) {
          Type* newT = doComputeType(ets->type);
          if (newT != ets->type) {
            TypeSymbol* newTS = newT->symbol;
            ns.value = newTS;
          }
        }
      }
    }
  }

  // Skip unnecessary writes to avoid triggering watchpoints.
  if (setType && oldType != ret) sym->type = ret;

  return ret;
}

Type* maybeAdjustSymbolType(Symbol* sym, AdjustTypeFn adjustTypeFn,
                            bool preserveRefLevels) {
  std::unordered_map<Type*, Type*> empty;
  auto ret = doAdjustSymbolType(empty, sym, adjustTypeFn, preserveRefLevels);
  return ret;
}

void adjustAllSymbolTypes(AdjustTypeFn adjustTypeFn, bool preserveRefLevels) {
  std::unordered_map<Type*, Type*> alreadyAdjusted;

  auto adjust = [&](Symbol* sym) {
    return doAdjustSymbolType(alreadyAdjusted, sym, adjustTypeFn,
                              preserveRefLevels);
  };

  forv_Vec(VarSymbol, var, gVarSymbols) {
    adjust(var);
  }

  forv_Vec(ArgSymbol, arg, gArgSymbols) {
    adjust(arg);
  }

  forv_Vec(ShadowVarSymbol, sv, gShadowVarSymbols) {
    adjust(sv);
  }

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    adjust(fn);
  }

  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    Type* newType = adjust(ts);

    // NOTE: Type symbols are the only class of symbol for which 'sym->type'
    //       is not actually changed (it doesn't really make sense). Instead,
    //       we loop and re-target all uses of the old type to point to the
    //       new type instead.
    if (newType != ts->type) {
      for_SymbolSymExprs(se, ts) se->setSymbol(newType->symbol);
    }
  }

  for (auto [kt, vt] : alreadyAdjusted) {
    // No change took place, so there is nothing to remove.
    if (kt == vt) continue;

    if (auto ts = kt->symbol) {
      // Otherwise, try to remove the key type from the tree.
      if (!ts->inTree()) continue;
      if (!ts->isUsed()) {
        ts->defPoint->remove();
      }
    }
  }
}

bool isUseOfProcedureAsValue(SymExpr* se) {
  auto fn = toFnSymbol(se->symbol());

  if (!fn) return false;

  if (auto call = toCallExpr(se->parentExpr)) {
    // TODO: Detect/reject virtual calls if needed.

    if (call->baseExpr != se) {
      auto formal = call->resolvedFunction() ? actual_to_formal(se) : nullptr;
      bool isMoveOrAssign = call->isPrimitive(PRIM_ASSIGN) ||
                            call->isPrimitive(PRIM_MOVE);

      if (isMoveOrAssign && call->get(2) == se) {
        // Ok, being moved or assigned into a variable.
        return true;

      } else if (formal) {
        // Ok, being passed (assume that the type matches).
        return true;
      }
    }
  }

  return false;
}

Type* matchRefLevel(Type* t, Type* match) {
  auto vt = t->getValType();
  auto ret = t;

  if (match->isRef() && !t->isRef()) {
    if (!vt->refType) makeRefType(vt);
    ret = vt->refType;

  } else if (match->isWideRef() && !t->isWideRef()) {
    ret = vt->getWideRefType();
  }

  INT_ASSERT(ret);

  return ret;
}
