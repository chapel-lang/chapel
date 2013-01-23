#include "astutil.h"
#include "baseAST.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"


void collectFnCalls(BaseAST* ast, Vec<CallExpr*>& calls) {
  AST_CHILDREN_CALL(ast, collectFnCalls, calls);
  if (CallExpr* call = toCallExpr(ast))
    if (call->isResolved())
      calls.add(call);
}


void collect_stmts(BaseAST* ast, Vec<Expr*>& stmts) {
  if (Expr* expr = toExpr(ast)) {
    stmts.add(expr);
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

void collectCallExprs(BaseAST* ast, Vec<CallExpr*>& callExprs) {
  AST_CHILDREN_CALL(ast, collectCallExprs, callExprs);
  if (CallExpr* callExpr = toCallExpr(ast))
    callExprs.add(callExpr);
}

void collectMyCallExprs(BaseAST* ast, Vec<CallExpr*>& callExprs,
                        FnSymbol* parent_fn) {
  AST_CHILDREN_CALL(ast, collectMyCallExprs, callExprs, parent_fn);
  if (CallExpr* callExpr = toCallExpr(ast))
    if (callExpr->parentSymbol == parent_fn)
      callExprs.add(callExpr);
}

void collectGotoStmts(BaseAST* ast, Vec<GotoStmt*>& gotoStmts) {
  AST_CHILDREN_CALL(ast, collectGotoStmts, gotoStmts);
  if (GotoStmt* gotoStmt = toGotoStmt(ast))
    gotoStmts.add(gotoStmt);
}

void collectSymExprs(BaseAST* ast, Vec<SymExpr*>& symExprs) {
  AST_CHILDREN_CALL(ast, collectSymExprs, symExprs);
  if (SymExpr* symExpr = toSymExpr(ast))
    symExprs.add(symExpr);
}

void collectSymbols(BaseAST* ast, Vec<Symbol*>& symbols) {
  AST_CHILDREN_CALL(ast, collectSymbols, symbols);
  if (Symbol* symbol = toSymbol(ast))
    symbols.add(symbol);
}

void collect_asts(BaseAST* ast, Vec<BaseAST*>& asts) {
  asts.add(ast);
  AST_CHILDREN_CALL(ast, collect_asts, asts);
}

void collect_asts_postorder(BaseAST* ast, Vec<BaseAST*>& asts) {
  AST_CHILDREN_CALL(ast, collect_asts_postorder, asts);
  asts.add(ast);
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
    } else if (call->isPrimitive(PRIM_VMT_CALL)) {
      FnSymbol* fn = toFnSymbol(toSymExpr(call->get(1))->var);
      Vec<FnSymbol*>* children = virtualChildrenMap.get(fn);
      fn->calledBy->add(call);
      forv_Vec(FnSymbol, child, *children)
        child->calledBy->add(call);
    }
  }
}


void buildDefUseMaps(Map<Symbol*,Vec<SymExpr*>*>& defMap,
                     Map<Symbol*,Vec<SymExpr*>*>& useMap) {
  Vec<Symbol*> symSet;
  forv_Vec(DefExpr, def, gDefExprs) {
    if (def->parentSymbol) {
      if (isVarSymbol(def->sym) || isArgSymbol(def->sym)) {
        symSet.set_add(def->sym);
      }
    }
  }
  buildDefUseMaps(symSet, gSymExprs, defMap, useMap);
}


void collectSymbolSetSymExprVec(BaseAST* ast,
                                Vec<Symbol*>& symSet,
                                Vec<SymExpr*>& symExprs) {
  if (DefExpr* def = toDefExpr(ast)) {
    if (isVarSymbol(def->sym) || isArgSymbol(def->sym)) {
      symSet.set_add(def->sym);
    }
  } else if (SymExpr* se = toSymExpr(ast)) {
    symExprs.add(se);
  }
  AST_CHILDREN_CALL(ast, collectSymbolSetSymExprVec, symSet, symExprs);
}


void buildDefUseMaps(FnSymbol* fn,
                     Map<Symbol*,Vec<SymExpr*>*>& defMap,
                     Map<Symbol*,Vec<SymExpr*>*>& useMap) {
  Vec<Symbol*> symSet;
  Vec<SymExpr*> symExprs;
  collectSymbolSetSymExprVec(fn, symSet, symExprs);
  buildDefUseMaps(symSet, symExprs, defMap, useMap);
}


void buildDefUseMaps(Vec<Symbol*>& symSet,
                     Map<Symbol*,Vec<SymExpr*>*>& defMap,
                     Map<Symbol*,Vec<SymExpr*>*>& useMap) {
  buildDefUseMaps(symSet, gSymExprs, defMap, useMap);
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
// return & 1 is true if se is a def
// return & 2 is true if se is a use
//
int isDefAndOrUse(SymExpr* se) {
  if (CallExpr* call = toCallExpr(se->parentExpr)) {
    if (call->isPrimitive(PRIM_MOVE) && call->get(1) == se) {
      return 1;
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
    } else if (call->isPrimitive(PRIM_VMT_CALL)) {
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


static void
pruneVisit(TypeSymbol* ts, Vec<FnSymbol*>& fns, Vec<TypeSymbol*>& types) {
  types.set_add(ts);
  Vec<BaseAST*> asts;
  collect_asts(ts, asts);
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = toDefExpr(ast))
      if (def->sym->type && !types.set_in(def->sym->type->symbol))
        pruneVisit(def->sym->type->symbol, fns, types);
  }
  if (ts->hasFlag(FLAG_DATA_CLASS))
    pruneVisit(getDataClassType(ts), fns, types);
}


static void
pruneVisit(FnSymbol* fn, Vec<FnSymbol*>& fns, Vec<TypeSymbol*>& types) {
  fns.set_add(fn);
  Vec<BaseAST*> asts;
  collect_asts(fn, asts);
  forv_Vec(BaseAST, ast, asts) {
    if (SymExpr* se = toSymExpr(ast)) {
      if (FnSymbol* next = toFnSymbol(se->var))
        if (!fns.set_in(next))
          pruneVisit(next, fns, types);
      if (se->var && se->var->type && !types.set_in(se->var->type->symbol))
        pruneVisit(se->var->type->symbol, fns, types);
    }
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
  pruneVisit(chpl_gen_main, fns, types);

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
  //
  // delete unused ClassType types, only deleting references to such
  // types when the value types are deleted
  //
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    // Ignore types flagged as primitive.
    if (ts->hasFlag(FLAG_PRIMITIVE_TYPE))
      continue;

    // Visit only those types not marked as visible.
    if (types.set_in(ts))
      continue;

    if (isClassType(ts->type)) {
      //
      // delete reference types for classes/records/unions only if
      // deleting value types
      //
      if (ts->hasFlag(FLAG_REF) && isClassType(ts->getValType()))
        continue;

      //
      // delete reference type if type is not used
      //
      if (!ts->hasFlag(FLAG_REF))
        if (Type* refType = ts->getRefType())
          refType->symbol->defPoint->remove();

      //
      // unlink reference type from value type
      //
      if (ts->hasFlag(FLAG_REF)) {
        if (Type* vt = ts->getValType()) {
          if (vt == dtNil) // don't delete nil ref as it is used when widening
            continue;
          vt->refType = NULL;
        }
      }

      ts->defPoint->remove();
    }
  }

  //
  // change symbols with dead types to void (important for baseline)
  //
  forv_Vec(DefExpr, def, gDefExprs) {
    if (def->parentSymbol && def->sym->type && isClassType(def->sym->type) && !isTypeSymbol(def->sym) && !types.set_in(def->sym->type->symbol))
      def->sym->type = dtVoid;
  }
}

// Done this way because the log letter and hence the pass name for
// each pass must be unique.  See initLogFlags() in runpasses.cpp.
void prune2() { prune(); } // Synonym for prune.

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
}


