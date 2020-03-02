/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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
#include "stlUtil.h"
#include "stmt.h"
#include "symbol.h"
#include "TryStmt.h"
#include "type.h"
#include "virtualDispatch.h"
#include "wellknown.h"
#include "WhileStmt.h"

#include "oldCollectors.h" // Deprecated. To be removed.
// After #include "oldCollectors.h" has been removed from all other source
// files, the corresponding collector functions can be removed from this
// implementation file, then this #include, and finally the .h file itself.

#include <vector>

static void pruneUnusedAggregateTypes(Vec<TypeSymbol*>& types);
static void pruneUnusedRefs(Vec<TypeSymbol*>& types);
static void changeDeadTypesToVoid(Vec<TypeSymbol*>& types);


void collectFnCalls(BaseAST* ast, std::vector<CallExpr*>& calls) {
  AST_CHILDREN_CALL(ast, collectFnCalls, calls);
  if (CallExpr* call = toCallExpr(ast))
    if (call->isResolved())
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

void collectForallStmts(BaseAST* ast, std::vector<ForallStmt*>& forallStmts) {
  AST_CHILDREN_CALL(ast, collectForallStmts, forallStmts);
  if (ForallStmt* forall = toForallStmt(ast))
    forallStmts.push_back(forall);
}

void collectCallExprs(BaseAST* ast, std::vector<CallExpr*>& callExprs) {
  AST_CHILDREN_CALL(ast, collectCallExprs, callExprs);
  if (CallExpr* callExpr = toCallExpr(ast))
    callExprs.push_back(callExpr);
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


void collectSymExprs(BaseAST* ast, std::vector<SymExpr*>& symExprs) {
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

void collect_asts(BaseAST* ast, std::vector<BaseAST*>& asts) {
  asts.push_back(ast);
  AST_CHILDREN_CALL(ast, collect_asts, asts);
}

void collect_asts_postorder(BaseAST* ast, Vec<BaseAST*>& asts) {
  AST_CHILDREN_CALL(ast, collect_asts_postorder, asts);
  asts.add(ast);
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
          INT_FATAL(call, "unexpected case calling %d", fn->name);
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
  if (call->isPrimitive(PRIM_ADD_ASSIGN)      ||
      call->isPrimitive(PRIM_SUBTRACT_ASSIGN) ||
      call->isPrimitive(PRIM_MULT_ASSIGN)     ||
      call->isPrimitive(PRIM_DIV_ASSIGN)      ||
      call->isPrimitive(PRIM_MOD_ASSIGN)      ||
      call->isPrimitive(PRIM_LSH_ASSIGN)      ||
      call->isPrimitive(PRIM_RSH_ASSIGN)      ||
      call->isPrimitive(PRIM_AND_ASSIGN)      ||
      call->isPrimitive(PRIM_OR_ASSIGN)       ||
      call->isPrimitive(PRIM_XOR_ASSIGN)) {
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


//
// TODO this should be fixed to include PRIM_SET_MEMBER
// See notes in iterator.cpp and/or loopInvariantCodeMotion.cpp
// TODO this should also be fixed to include the PRIM_SET_SVEC_MEMBER
//  an attempt to do so is in the commented-out sections below
//  but would require also fixing a bug in copy-propagation
//  with e.g. functions/deitz/nested/test_nested_var_iterator2.chpl
// TODO this should handle PRIM_VIRTUAL_METHOD_CALL and PRIM_FTABLE_CALL
//
// which gets inserted from the returnStartTuplesByRefArgs pass
// return & 1 is true if se is a def
// return & 2 is true if se is a use
//
// Note that a DefExpr is where we hang the variable declaration, but after
// normalize, a DefExpr itself does not set a variable, and so it does not
// count as a Def.
int isDefAndOrUse(SymExpr* se) {
  const int DEF = 1;
  const int USE = 2;
  const int DEF_USE = 3;
  if (CallExpr* call = toCallExpr(se->parentExpr)) {
    bool isFirstActual = (call->get(1) == se);

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
    } else if (FnSymbol* fn = call->resolvedFunction()) {
      ArgSymbol* arg = actual_to_formal(se);

      // BHARSH TODO: get rid of this 'isRecord' special case
      if (arg->intent == INTENT_REF ||
          arg->intent == INTENT_INOUT ||
          (fn->name == astrSassign &&
           fn->getFormal(1) == arg &&
           isRecord(arg->type))) {
        return DEF_USE;
      } else if (arg->intent == INTENT_OUT) {
        return DEF;
      }
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
  INT_FATAL(a, "bad call to actual_to_formal");
  return NULL;
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
      return is_bool_type(t) ||
             is_int_type(t) ||
             is_uint_type(t) ||
             is_real_type(t) ||
             is_imag_type(t) ||
             is_complex_type(t);
  }

  return false;
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
          const char* name = var->immediate->v_string;
          Symbol*     field = ct->getField(name);

          retval = field->hasFlag(FLAG_TYPE_VARIABLE);
        }
      }

    } else if (call->numActuals() == 1 &&
               call->baseExpr &&
               isNumericTypeSymExpr(call->baseExpr)) {
      // e.g. call 'int' 64 is a type expression (resulting in int(64))
      retval = true;

    } else if (FnSymbol* fn = call->resolvedFunction()) {
      retval = fn->retTag == RET_TYPE;
    }
  }

  return retval;
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
  std::vector<DefExpr*> defExprs;
  collectDefExprs(ts, defExprs);
  for_vector(DefExpr, def, defExprs) {
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
  std::vector<SymExpr*> symExprs;
  collectSymExprs(fn, symExprs);
  for_vector(SymExpr, se, symExprs) {
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


// Visit and mark functions (and types) which are reachable from
// externally visible symbols.
static void
visitVisibleFunctions(Vec<FnSymbol*>& fns, Vec<TypeSymbol*>& types)
{
  // chpl_gen_main is always visible (if it exists).
  if (chpl_gen_main)
    pruneVisit(chpl_gen_main, fns, types);

  // When present, the printModuleInitOrder function is always visible;
  // it will be NULL for --minimal-modules compilations
  if (gPrintModuleInitFn) {
    pruneVisit(gPrintModuleInitFn, fns, types);
  }

  // Functions appearing the function pointer table are visible.
  // These are blocks that can be started through a forall, coforall,
  // or on statement.
  forv_Vec(FnSymbol, fn, ftableVec)
    pruneVisit(fn, fns, types);

  // Mark VFT entries as visible.
  for (int i = 0; i < virtualMethodTable.n; i++)
    if (virtualMethodTable.v[i].key)
      for (int j = 0; j < virtualMethodTable.v[i].value->n; j++)
        pruneVisit(virtualMethodTable.v[i].value->v[j], fns, types);

  // Mark exported symbols and module init/deinit functions as visible.
  forv_Vec(FnSymbol, fn, gFnSymbols)
    if (fn->hasFlag(FLAG_EXPORT))
      pruneVisit(fn, fns, types);

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

  pruneUnusedAggregateTypes(types);
  pruneUnusedRefs(types);
  changeDeadTypesToVoid(types);
}


static void pruneUnusedAggregateTypes(Vec<TypeSymbol*>& types)
{
  //
  // delete unused AggregateType types, only deleting references to such
  // types when the value types are deleted
  //
  forv_Vec(TypeSymbol, ts, gTypeSymbols)
  {
    // Do not delete reference types.
    // We delete the reference type if the base type is dead (below).
    if (ts->hasFlag(FLAG_REF))
      continue;

    // Ignore types flagged as primitive.
    if (ts->hasFlag(FLAG_PRIMITIVE_TYPE))
      continue;

    // Ignore types that are not class types
    if (!isAggregateType(ts->type))
      continue;

    // Do not delete the object class.  It's pretty crucial.
    if (ts->hasFlag(FLAG_OBJECT_CLASS))
      continue;

    // Visit only those types not marked as visible.
    if (types.set_in(ts))
      continue;

    ts->defPoint->remove();
  }
}


static void pruneUnusedRefs(Vec<TypeSymbol*>& types)
{
  forv_Vec(TypeSymbol, ts, gTypeSymbols)
  {
    // This pass, we are interested only in ref types.
    if (!ts->hasFlag(FLAG_REF))
      continue;

    // Ignore types flagged as primitive.
    if (ts->hasFlag(FLAG_PRIMITIVE_TYPE))
      continue;

    // Ignore types that are not class types
    if (!isAggregateType(ts->type))
    {
      // hilde sez: Ref types are always class types.
      // So we can't get here.
      INT_ASSERT(false);
      continue;
    }

    // Visit only those types not marked as visible.
    if (types.set_in(ts))
      continue;

    if (Type* vt = ts->getValType())
    {
      // Don't remove a ref type if it refers to a class type
      if (isAggregateType(vt))
        // and the class type is still alive.
        if (types.set_in(vt->symbol))
          continue;

      // Don't delete nil ref as it is used in widening.
      if (vt == dtNil)
        continue;

      // Unlink reference type from value type.
      vt->refType = NULL;
    }

    ts->defPoint->remove();
  }
}


static void changeDeadTypesToVoid(Vec<TypeSymbol*>& types)
{
  //
  // change symbols with dead types to void (important for baseline)
  //
  forv_Vec(DefExpr, def, gDefExprs) {
    if (def->inTree()                             &&
        def->sym->type                   != NULL  &&
        isAggregateType(def->sym->type)  ==  true &&
        isTypeSymbol(def->sym)           == false &&
        !types.set_in(def->sym->type->symbol))
      def->sym->type = dtNothing;
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
}


// Done this way to make the pass name for each pass unique.
void prune2() { prune(); } // Synonym for prune.

/*
 * Takes a call that is a PRIM_SVEC_GET_MEMBER* and returns the symbol of the
 * field. Normally the call is something of the form PRIM_SVEC_GET_MEMBER(p, 1)
 * and what this function gets out is the symbol that is the first field
 * instead of just the number 1.
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

    INT_ASSERT(immediateVal >= 1 && immediateVal <= tuple->fields.length);
    return tuple->getField(immediateVal);
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
      forv_Vec(FnSymbol, fn, ftableVec) {
        addToUsedFnSymbols(fnSymbols, fn);
      }
    }
  }
}

static void setQualRef(Symbol* sym) {
  if (sym->isRefOrWideRef() && sym->type->symbol->hasEitherFlag(FLAG_REF, FLAG_WIDE_REF)) {
    Qualifier q = sym->qualType().getQual();
    const bool isRef = q == QUAL_REF        || q == QUAL_CONST_REF        ||
                       q == QUAL_NARROW_REF || q == QUAL_CONST_NARROW_REF ||
                       q == QUAL_WIDE_REF   || q == QUAL_CONST_WIDE_REF;
    if (isRef == false) {
      if (sym->type->symbol->hasFlag(FLAG_WIDE_REF)) {
        q = QUAL_WIDE_REF;
      } else {
        q = QUAL_REF;
      }
      sym->qual = q;
      if (ArgSymbol* arg = toArgSymbol(sym)) {
        if (arg->intent != INTENT_CONST_REF) {
          arg->intent = INTENT_REF;
        }
      }
    }
    sym->type = sym->getValType();
  }
}

void convertToQualifiedRefs() {
#define fixRefSymbols(SymType) \
  forv_Vec(SymType, sym, g##SymType##s) { \
    setQualRef(sym); \
  }

  fixRefSymbols(VarSymbol);
  fixRefSymbols(ArgSymbol);
  fixRefSymbols(ShadowVarSymbol);

  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_ADDR_OF)) {
      call->primitive = primitives[PRIM_SET_REFERENCE];
    }
  }
#undef fixRefSymbols
}
