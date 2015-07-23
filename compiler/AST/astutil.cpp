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

#include "astutil.h"
#include "baseAST.h"
#include "CForLoop.h"
#include "ForLoop.h"
#include "expr.h"
#include "passes.h"
#include "ParamForLoop.h"
#include "stlUtil.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"
#include "WhileStmt.h"

#include "oldCollectors.h" // Deprecated. To be removed.
// After #include "oldCollectors.h" has been removed from all other source
// files, the corresponding collector functions can be removed from this
// implementation file, then this #include, and finally the .h file itself.

#include <vector>

static void pruneUnusedAggregateTypes(Vec<TypeSymbol*>& types);
static void pruneUnusedRefs(Vec<TypeSymbol*>& types);
static void changeDeadTypesToVoid(Vec<TypeSymbol*>& types);


void collectFnCalls(BaseAST* ast, Vec<CallExpr*>& calls) {
  AST_CHILDREN_CALL(ast, collectFnCalls, calls);
  if (CallExpr* call = toCallExpr(ast))
    if (call->isResolved())
      calls.add(call);
}


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

void collect_stmts(BaseAST* ast, Vec<Expr*>& stmts) {
  if (Expr* expr = toExpr(ast)) {
    stmts.add(expr);
    if (isBlockStmt(expr) || isCondStmt(expr)) {
      AST_CHILDREN_CALL(ast, collect_stmts, stmts);
    }
  }
}

void collect_stmts(BaseAST* ast, std::vector<Expr*>& stmts) {
  if (Expr* expr = toExpr(ast)) {
    stmts.push_back(expr);
    if (isBlockStmt(expr) || isCondStmt(expr)) {
      AST_CHILDREN_CALL(ast, collect_stmts, stmts);
    }
  }
}

void collectDefExprs(BaseAST* ast, Vec<DefExpr*>& defExprs) {
  AST_CHILDREN_CALL(ast, collectDefExprs, defExprs);
  if (DefExpr* defExpr = toDefExpr(ast))
    defExprs.add(defExpr);
}

void collectDefExprs(BaseAST* ast, std::vector<DefExpr*>& defExprs) {
  AST_CHILDREN_CALL(ast, collectDefExprs, defExprs);
  if (DefExpr* defExpr = toDefExpr(ast))
    defExprs.push_back(defExpr);
}

void collectCallExprs(BaseAST* ast, Vec<CallExpr*>& callExprs) {
  AST_CHILDREN_CALL(ast, collectCallExprs, callExprs);
  if (CallExpr* callExpr = toCallExpr(ast))
    callExprs.add(callExpr);
}

void collectCallExprs(BaseAST* ast, std::vector<CallExpr*>& callExprs) {
  AST_CHILDREN_CALL(ast, collectCallExprs, callExprs);
  if (CallExpr* callExpr = toCallExpr(ast))
    callExprs.push_back(callExpr);
}

void collectMyCallExprs(BaseAST* ast, Vec<CallExpr*>& callExprs,
                        FnSymbol* parent_fn) {
  AST_CHILDREN_CALL(ast, collectMyCallExprs, callExprs, parent_fn);
  if (CallExpr* callExpr = toCallExpr(ast))
    if (callExpr->parentSymbol == parent_fn)
      callExprs.add(callExpr);
}

void collectMyCallExprs(BaseAST* ast, std::vector<CallExpr*>& callExprs,
                           FnSymbol* parent_fn) {
  AST_CHILDREN_CALL(ast, collectMyCallExprs, callExprs, parent_fn);
  if (CallExpr* callExpr = toCallExpr(ast))
    if (callExpr->parentSymbol == parent_fn)
      callExprs.push_back(callExpr);
}

void collectGotoStmts(BaseAST* ast, Vec<GotoStmt*>& gotoStmts) {
  AST_CHILDREN_CALL(ast, collectGotoStmts, gotoStmts);
  if (GotoStmt* gotoStmt = toGotoStmt(ast))
    gotoStmts.add(gotoStmt);
}

void collectGotoStmts(BaseAST* ast, std::vector<GotoStmt*>& gotoStmts) {
  AST_CHILDREN_CALL(ast, collectGotoStmts, gotoStmts);
  if (GotoStmt* gotoStmt = toGotoStmt(ast))
    gotoStmts.push_back(gotoStmt);
}

void collectSymExprs(BaseAST* ast, Vec<SymExpr*>& symExprs) {
  AST_CHILDREN_CALL(ast, collectSymExprs, symExprs);
  if (SymExpr* symExpr = toSymExpr(ast))
    symExprs.add(symExpr);
}

void collectSymExprs(BaseAST* ast, std::vector<SymExpr*>& symExprs) {
  AST_CHILDREN_CALL(ast, collectSymExprs, symExprs);
  if (SymExpr* symExpr = toSymExpr(ast))
    symExprs.push_back(symExpr);
}

static void collectMySymExprsHelp(BaseAST* ast, Vec<SymExpr*>& symExprs) {
  if (isSymbol(ast)) return; // do not descend into nested symbols
  AST_CHILDREN_CALL(ast, collectMySymExprsHelp, symExprs);
  if (SymExpr* se = toSymExpr(ast))
    symExprs.add(se);
}

static void collectMySymExprsHelp(BaseAST* ast, std::vector<SymExpr*>& symExprs) {
  if (isSymbol(ast)) return; // do not descend into nested symbols
  AST_CHILDREN_CALL(ast, collectMySymExprsHelp, symExprs);
  if (SymExpr* se = toSymExpr(ast))
    symExprs.push_back(se);
}

// Collect the SymExprs only *directly* under 'me'.
// Do not include those in nested functions/other nested symbols.
void collectMySymExprs(Symbol* me, Vec<SymExpr*>& symExprs) {
  // skip the isSymbol(ast) check in collectMySymExprsHelp()
  AST_CHILDREN_CALL(me, collectMySymExprsHelp, symExprs);
}

// The same for std::vector.
void collectMySymExprs(Symbol* me, std::vector<SymExpr*>& symExprs) {
  // skip the isSymbol(ast) check in collectMySymExprsHelp()
  AST_CHILDREN_CALL(me, collectMySymExprsHelp, symExprs);
}

void collectSymbols(BaseAST* ast, Vec<Symbol*>& symbols) {
  AST_CHILDREN_CALL(ast, collectSymbols, symbols);
  if (Symbol* symbol = toSymbol(ast))
    symbols.add(symbol);
}

void collectSymbols(BaseAST* ast, std::vector<Symbol*>& symbols) {
  AST_CHILDREN_CALL(ast, collectSymbols, symbols);
  if (Symbol* symbol = toSymbol(ast))
    symbols.push_back(symbol);
}

void collect_asts(BaseAST* ast, Vec<BaseAST*>& asts) {
  asts.add(ast);
  AST_CHILDREN_CALL(ast, collect_asts, asts);
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

static void collect_top_asts_internal(BaseAST* ast, Vec<BaseAST*>& asts) {
  if (!isSymbol(ast) || isArgSymbol(ast)) {
    AST_CHILDREN_CALL(ast, collect_top_asts_internal, asts);
    asts.add(ast);
  }
}

void collect_top_asts(BaseAST* ast, Vec<BaseAST*>& asts) {
  AST_CHILDREN_CALL(ast, collect_top_asts_internal, asts);
  asts.add(ast);
}

static void collect_top_asts_internal(BaseAST* ast, std::vector<BaseAST*>& asts) {
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

void reset_ast_loc(BaseAST* destNode, BaseAST* sourceNode) {
  reset_ast_loc(destNode, sourceNode->astloc);
}

void reset_ast_loc(BaseAST* destNode, astlocT astlocArg) {
  destNode->astloc = astlocArg;
  AST_CHILDREN_CALL(destNode, reset_ast_loc, astlocArg);
}


void compute_call_sites() {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->calledBy)
      fn->calledBy->clear();
    else
      fn->calledBy = new Vec<CallExpr*>();
  }
  forv_Vec(CallExpr, call, gCallExprs) {
    if (FnSymbol* fn = call->isResolved()) {
      fn->calledBy->add(call);
    } else if (call->isPrimitive(PRIM_FTABLE_CALL)) {
      // sjd: do we have to do anything special here?
      //      should this call be added to some function's calledBy list?
    } else if (call->isPrimitive(PRIM_VIRTUAL_METHOD_CALL)) {
      FnSymbol* fn = toFnSymbol(toSymExpr(call->get(1))->var);
      Vec<FnSymbol*>* children = virtualChildrenMap.get(fn);
      fn->calledBy->add(call);
      forv_Vec(FnSymbol, child, *children)
        child->calledBy->add(call);
    }
  }
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
  buildDefUseMaps(symSet, gSymExprs, defMap, useMap);
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

// builds the vectors for every variable/argument in 'fn' and looks
// for uses and defs only in 'fn'
void buildDefUseMaps(FnSymbol* fn,
                     Map<Symbol*,Vec<SymExpr*>*>& defMap,
                     Map<Symbol*,Vec<SymExpr*>*>& useMap) {
  Vec<Symbol*> symSet;
  Vec<SymExpr*> symExprs;
  // Collect symbols and sym expressions on within the given function
  collectSymbolSetSymExprVec(fn, symSet, symExprs);
  buildDefUseMaps(symSet, symExprs, defMap, useMap);
}

// builds the vectors for every variable declaration in the given block
// and looks for uses and defs within the same block (scope).
void buildDefUseMaps(BlockStmt* block,
                     Map<Symbol*,Vec<SymExpr*>*>& defMap,
                     Map<Symbol*,Vec<SymExpr*>*>& useMap) {
  Vec<Symbol*> symSet;
  Vec<SymExpr*> symExprs;
  // Collect symbols and sym expressions only within the given block.
  collectSymbolSetSymExprVec(block, symSet, symExprs);
  buildDefUseMaps(symSet, symExprs, defMap, useMap);
}

static void addUseOrDef(Map<Symbol*,Vec<SymExpr*>*>& ses, SymExpr* se) {
  Vec<SymExpr*>* sev = ses.get(se->var);
  if (sev) {
    sev->add(se);
  } else {
    sev = new Vec<SymExpr*>();
    sev->add(se);
    ses.put(se->var, sev);
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


//
// Check if a callExpr is a relational operator primitive (==, !=, <=, >=, <, >)
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
// TODO this should also be fixed to include the PRIM_SVEC_SET_MEMBER
// which gets inserted from the returnStartTuplesByRefArgs pass
// return & 1 is true if se is a def
// return & 2 is true if se is a use
//
int isDefAndOrUse(SymExpr* se) {
  if (CallExpr* call = toCallExpr(se->parentExpr)) {
    if ((call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN)) &&
        call->get(1) == se) {
      return 1;
    } else if (isOpEqualPrim(call) && call->get(1) == se) {
      return 3;
    } else if (FnSymbol* fn = call->isResolved()) {
      ArgSymbol* arg = actual_to_formal(se);
      if (arg->intent == INTENT_REF ||
          arg->intent == INTENT_INOUT ||
          (!strcmp(fn->name, "=") && fn->getFormal(1) == arg && isRecord(arg->type)) ||
          isRecordWrappedType(arg->type)) { // pass by reference
        return 3;
        // also use; do not "continue"
      } else if (arg->intent == INTENT_OUT) {
        return 1;
      }
    }
  }
  return 2;
}


void buildDefUseMaps(Vec<Symbol*>& symSet,
                     Vec<SymExpr*>& symExprs,
                     Map<Symbol*,Vec<SymExpr*>*>& defMap,
                     Map<Symbol*,Vec<SymExpr*>*>& useMap) {
  forv_Vec(SymExpr, se, symExprs) {
    if (se->parentSymbol && symSet.set_in(se->var)) {
      int result = isDefAndOrUse(se);
      if (result & 1)
        addDef(defMap, se);
      if (result & 2)
        addUse(useMap, se);
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
    if (se->parentSymbol && se->var && symSet.set_in(se->var)) {
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
    if (se->var == oldSym)
      se->var = newSym;
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
      FnSymbol* fn = toFnSymbol(toSymExpr(call->get(1))->var);
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
  }
  INT_FATAL(call, "bad call to formal_to_actual");
  return NULL;
}


bool isTypeExpr(Expr* expr)
{
  if (SymExpr* sym = toSymExpr(expr))
  {
    if (isTypeSymbol(sym->var))
      return true;

    if (sym->var->hasFlag(FLAG_TYPE_VARIABLE))
      return true;

    if (FnSymbol* fn = toFnSymbol(sym->var))
      if (fn->retTag == RET_TYPE)
        return true;
  }

  if (CallExpr* call = toCallExpr(expr))
  {
    if (call->isPrimitive(PRIM_TYPEOF))
      return true;

    if (call->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
        call->isPrimitive(PRIM_GET_MEMBER))
    {
      AggregateType* ct = toAggregateType(call->get(1)->typeInfo());
      INT_ASSERT(ct);

      if (ct->symbol->hasFlag(FLAG_REF))
        ct = toAggregateType(ct->getValType());

      SymExpr* left = toSymExpr(call->get(1));

      if (left->var->type->symbol->hasFlag(FLAG_TUPLE) &&
          left->var->hasFlag(FLAG_TYPE_VARIABLE))
        return true;

      SymExpr* right = toSymExpr(call->get(2));
      VarSymbol* var = toVarSymbol(right->var);

      if (var->isType())
        return true;

      if (var->immediate)
      {
        const char* name = var->immediate->v_string;
        for_fields(field, ct) {
          if (!strcmp(field->name, name))
            if (field->hasFlag(FLAG_TYPE_VARIABLE))
              return true;
        }
      }
    }

    if (FnSymbol* fn = call->isResolved())
      if (fn->retTag == RET_TYPE)
        return true;
  }

  return false;
}


static void
pruneVisit(TypeSymbol* ts, Vec<FnSymbol*>& fns, Vec<TypeSymbol*>& types) {
  types.set_add(ts);
  std::vector<DefExpr*> defExprs;
  collectDefExprs(ts, defExprs);
  for_vector(DefExpr, def, defExprs) {
    if (def->sym->type && !types.set_in(def->sym->type->symbol))
      pruneVisit(def->sym->type->symbol, fns, types);
  }
  if (ts->hasFlag(FLAG_DATA_CLASS))
    pruneVisit(getDataClassType(ts), fns, types);
}


static void
pruneVisit(FnSymbol* fn, Vec<FnSymbol*>& fns, Vec<TypeSymbol*>& types) {
  fns.set_add(fn);
  std::vector<SymExpr*> symExprs;
  collectSymExprs(fn, symExprs);
  for_vector(SymExpr, se, symExprs) {
    if (FnSymbol* next = toFnSymbol(se->var))
      if (!fns.set_in(next))
        pruneVisit(next, fns, types);
    if (se->var && se->var->type && !types.set_in(se->var->type->symbol))
      pruneVisit(se->var->type->symbol, fns, types);
  }
  for_formals(formal, fn) {
    if (!types.set_in(formal->type->symbol))
      pruneVisit(formal->type->symbol, fns, types);
  }
}


// Visit and mark functions (and types) which are reachable from
// externally visible symbols.
static void
visitVisibleFunctions(Vec<FnSymbol*>& fns, Vec<TypeSymbol*>& types)
{
  // chpl_gen_main is always visible (if it exists).
  // --ipe does not build chpl_gen_main
  if (chpl_gen_main)
    pruneVisit(chpl_gen_main, fns, types);

  // When present, the printModuleInitOrder function is always visible;
  // it will be NULL for --minimal-modules compilations
  if (gPrintModuleInitFn) {
    pruneVisit(gPrintModuleInitFn, fns, types);
  }

  // Functions appearing the function pointer table are visible.
  // These are blocks that can be started through a forall, coforall or on statement.
  forv_Vec(FnSymbol, fn, ftableVec)
    pruneVisit(fn, fns, types);

  // Mark VFT entries as visible.
  for (int i = 0; i < virtualMethodTable.n; i++)
    if (virtualMethodTable.v[i].key)
      for (int j = 0; j < virtualMethodTable.v[i].value->n; j++)
        pruneVisit(virtualMethodTable.v[i].value->v[j], fns, types);

  // Mark exported symbols as visible.
  // All module initialization functions should be exported,
  // but for now we treat them as a separate class. <hilde>
  forv_Vec(FnSymbol, fn, gFnSymbols)
    if (fn->hasFlag(FLAG_EXPORT) ||
        fn->hasFlag(FLAG_MODULE_INIT))
      pruneVisit(fn, fns, types);
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
    if (def->parentSymbol && def->sym->type && isAggregateType(def->sym->type) && !isTypeSymbol(def->sym) && !types.set_in(def->sym->type->symbol))
      def->sym->type = dtVoid;
  }
}


static void removeVoidMoves()
{
  forv_Vec(CallExpr, call, gCallExprs)
  {
    if (! call->isPrimitive(PRIM_MOVE))
      continue;

    SymExpr* se = toSymExpr(call->get(1));
    if (se->var->type != dtVoid)
      continue;

    call->remove();
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


// Done this way because the log letter and hence the pass name for
// each pass must be unique.  See initLogFlags() in runpasses.cpp.
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
  VarSymbol* fieldSym = toVarSymbol(fieldVal->var);
  if (fieldSym) {
    int immediateVal = fieldSym->immediate->int_value();

    INT_ASSERT(immediateVal >= 1 && immediateVal <= tuple->fields.length);
    return tuple->getField(immediateVal);
  } else {
    // GET_SVEC_MEMBER(p, i), where p is a star tuple
    return NULL;
  }
}

/*
* Collect all of the functions in the call graph at and below the function
* call.
*/
void collectUsedFnSymbols(BaseAST* ast, std::set<FnSymbol*>& fnSymbols) {
  AST_CHILDREN_CALL(ast, collectUsedFnSymbols, fnSymbols);
  //if there is a function call, get the FnSymbol associated with it
  //and look through that FnSymbol for other function calls. Do not
  //look through an already visited FnSymbol, or you'll have an infinite
  //loop in the case of recursion.
  if (CallExpr* call = toCallExpr(ast)) {
    if (FnSymbol* fnSymbol = call->isResolved()) {
      if(fnSymbols.count(fnSymbol) == 0) {
        fnSymbols.insert(fnSymbol);
        AST_CHILDREN_CALL(fnSymbol->body, collectUsedFnSymbols, fnSymbols);
      }
    }
  }
}
