#include "astutil.h"
#include "baseAST.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"


void collect_stmts(BaseAST* ast, Vec<Expr*>& stmts) {
  if (Expr* expr = toExpr(ast)) {
    stmts.add(expr);
    if (expr->astTag == STMT_BLOCK || expr->astTag == STMT_COND) {
      AST_CHILDREN_CALL(ast, collect_stmts, stmts);
    }
  }
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
              

void reset_line_info(BaseAST* ast, int lineno) {
  ast->lineno = lineno;
  AST_CHILDREN_CALL(ast, reset_line_info, lineno);
}


void compute_call_sites() {
  forv_Vec(FnSymbol, fn, gFns) {
    if (fn->calledBy)
      fn->calledBy->clear();
    else
      fn->calledBy = new Vec<CallExpr*>();
  }
  forv_Vec(CallExpr, call, gCalls) {
    if (FnSymbol* fn = call->isResolved()) {
      fn->calledBy->add(call);
    }
  }
}


void buildDefUseMaps(Map<Symbol*,Vec<SymExpr*>*>& defMap,
                     Map<Symbol*,Vec<SymExpr*>*>& useMap) {
  Vec<Symbol*> symSet;
  forv_Vec(BaseAST, ast, gAsts) {
    if (DefExpr* def = toDefExpr(ast)) {
      if (def->parentSymbol) {
        if (isVarSymbol(def->sym) || isArgSymbol(def->sym)) {
          symSet.set_add(def->sym);
        }
      }
    }
  }
  buildDefUseMaps(symSet, gAsts, defMap, useMap);
}


void buildDefUseMaps(FnSymbol* fn,
                     Map<Symbol*,Vec<SymExpr*>*>& defMap,
                     Map<Symbol*,Vec<SymExpr*>*>& useMap) {
  Vec<BaseAST*> asts;
  collect_asts(fn, asts);
  Vec<Symbol*> symSet;
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = toDefExpr(ast)) {
      if (isVarSymbol(def->sym) || isArgSymbol(def->sym)) {
        symSet.set_add(def->sym);
      }
    }
  }
  buildDefUseMaps(symSet, asts, defMap, useMap);
}


void buildDefUseMaps(Vec<Symbol*>& symSet,
                     Map<Symbol*,Vec<SymExpr*>*>& defMap,
                     Map<Symbol*,Vec<SymExpr*>*>& useMap) {
  buildDefUseMaps(symSet, gAsts, defMap, useMap);
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
static int isDefAndOrUse(SymExpr* se) {
  if (CallExpr* call = toCallExpr(se->parentExpr)) {
    if (call->isPrimitive(PRIMITIVE_MOVE) && call->get(1) == se) {
      return 1;
    } else if (call->isResolved()) {
      ArgSymbol* arg = actual_to_formal(se);
      if (arg->intent == INTENT_REF ||
          arg->intent == INTENT_INOUT ||
          arg->type->symbol->hasFlag(FLAG_ARRAY) || // pass by reference
          arg->type->symbol->hasFlag(FLAG_DOMAIN)) { // pass by reference
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
                     Vec<BaseAST*>& asts,
                     Map<Symbol*,Vec<SymExpr*>*>& defMap,
                     Map<Symbol*,Vec<SymExpr*>*>& useMap) {
  forv_Vec(BaseAST, ast, asts) {
    if (SymExpr* se = toSymExpr(ast)) {
      if (se->parentSymbol && se->var && symSet.set_in(se->var)) {
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


void buildDefUseSets(Vec<Symbol*>& syms,
                     FnSymbol* fn,
                     Vec<SymExpr*>& defSet,
                     Vec<SymExpr*>& useSet) {
  Vec<BaseAST*> asts;
  collect_asts(fn, asts);
  Vec<Symbol*> symSet;
  forv_Vec(Symbol, sym, syms) {
    symSet.set_add(sym);
  }
  forv_Vec(BaseAST, ast, asts) {
    if (SymExpr* se = toSymExpr(ast)) {
      if (se->parentSymbol && se->var && symSet.set_in(se->var)) {
        int result = isDefAndOrUse(se);
        if (result & 1)
          defSet.set_add(se);
        if (result & 2)
          useSet.set_add(se);
      }
    }
  }
}


#define SUB_SYMBOL(x)                                   \
  if (x) {                                              \
    if (Symbol* y = map->get(x)) {                      \
        x = y;                                          \
    }                                                   \
  }

#define SUB_TYPE(x)                                     \
  if (x) {                                              \
    if (Symbol* y = map->get(x->symbol)) {              \
      x = y->type;                                      \
    }                                                   \
  }

void update_symbols(BaseAST* ast, SymbolMap* map) {
  if (SymExpr* sym_expr = toSymExpr(ast)) {
    SUB_SYMBOL(sym_expr->var);
  } else if (DefExpr* defExpr = toDefExpr(ast)) {
    SUB_TYPE(defExpr->sym->type);
  } else if (VarSymbol* ps = toVarSymbol(ast)) {
    SUB_TYPE(ps->type);
  } else if (FnSymbol* ps = toFnSymbol(ast)) {
    SUB_TYPE(ps->type);
    SUB_TYPE(ps->retType);
    SUB_SYMBOL(ps->_this);
    SUB_SYMBOL(ps->_outer);
  } else if (ArgSymbol* ps = toArgSymbol(ast)) {
    SUB_TYPE(ps->type);
  }
  AST_CHILDREN_CALL(ast, update_symbols, map);
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


void remove_help(BaseAST* ast, int dummy) {
  AST_CHILDREN_CALL(ast, remove_help, dummy);
  if (Expr* expr = toExpr(ast)) {
    expr->parentSymbol = NULL;
    expr->parentExpr = NULL;
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
    }
  }
  INT_FATAL(a, "bad call to actual_to_formals");
  return NULL;
}


Expr* formal_to_actual(CallExpr* call, Symbol* formal) {
  for_actuals(expr, call) {
    if (actual_to_formal(expr) == formal)
      return expr;
  }
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
    pruneVisit(toTypeSymbol(ts->type->substitutions.v[0].value), fns, types);
}


static void
pruneVisit(FnSymbol* fn, Vec<FnSymbol*>& fns, Vec<TypeSymbol*>& types, bool pruneDestructors) {
  fns.set_add(fn);
  Vec<BaseAST*> asts;
  collect_asts(fn, asts);
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (FnSymbol* next = call->isResolved())
        if (!fns.set_in(next))
          pruneVisit(next, fns, types, pruneDestructors);
    } else if (SymExpr* se = toSymExpr(ast)) {
      if (se->var && se->var->type && !types.set_in(se->var->type->symbol))
        pruneVisit(se->var->type->symbol, fns, types);
    } else if (!pruneDestructors) {
      // the type of a destructor's argument may not be used anywhere else
      // since calls to destructors may not have been inserted yet
      if (ArgSymbol* as = toArgSymbol(ast))
        if (!types.set_in(as->type->symbol))
          pruneVisit(as->type->symbol, fns, types);
    }
  }
}


static void
reallyPrune(bool pruneDestructors) {
  Vec<FnSymbol*> fns;
  Vec<TypeSymbol*> types;
  pruneVisit(chpl_main, fns, types, pruneDestructors);
  if (fRuntime) {
    forv_Vec(FnSymbol, fn, gFns) {
      if (fn->hasFlag(FLAG_EXPORT))
        pruneVisit(fn, fns, types, pruneDestructors);
    }
  }
  if (!pruneDestructors) {
    forv_Vec(FnSymbol, fn, gFns) {
      if (!strcmp(fn->name, "~chpl_destroy") || !strcmp(fn->name, "chpl__auto_destroy")) {
        pruneVisit(fn, fns, types, pruneDestructors);
      }
    }
  }
  forv_Vec(FnSymbol, fn, gFns) {
    if (!fns.set_in(fn)) {
      fn->defPoint->remove();
    }
  }
  forv_Vec(TypeSymbol, ts, gTypes) {
    if (!types.set_in(ts)) {
      if (toClassType(ts->type)) {
        //
        // do not delete class/record references
        //
        if (ts->hasFlag(FLAG_REF) &&
            toClassType(ts->type->getField("_val")->type))
          continue;

        //
        // delete reference type if type is not used
        //
        if (ts->type->refType)
          ts->type->refType->symbol->defPoint->remove();
        Type *vt = ts->typeInfo()->getValueType();
        // don't remove reference to nil, as it may be used when widening
        if (vt != dtNil) {
          if (vt) {
            INT_ASSERT(!vt->refType || vt->refType == ts->type);
            vt->refType = NULL;
          }
          ts->defPoint->remove();
        }
      }
    }
  }

  //
  // change symbols with dead types to void (important for baseline)
  //
  forv_Vec(BaseAST, ast, gAsts) {
    if (DefExpr* def = toDefExpr(ast))
      if (def->parentSymbol && def->sym->type && isClassType(def->sym->type) && !isTypeSymbol(def->sym) && !types.set_in(def->sym->type->symbol))
        def->sym->type = dtVoid;
  }
}

void
prune() {
  static bool firstTime = true;
  if (firstTime) {
    fixupDestructors();
    reallyPrune(fEnableDestructorCalls ? false : true);
    firstTime = false;
  } else if (fEnableDestructorCalls)
    reallyPrune(true);
}
