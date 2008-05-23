#include "astutil.h"
#include "baseAST.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"


void collect_stmts(Vec<Expr*>* exprs, Expr* expr) {
  exprs->add(expr);
  if (expr->astTag == STMT_BLOCK || expr->astTag == STMT_COND) {
    Vec<BaseAST*> next_asts;
    get_ast_children(expr, next_asts);
    forv_Vec(BaseAST, next_ast, next_asts) {
      if (Expr* expr = toExpr(next_ast))
        collect_stmts(exprs, expr);
    }
  }
}

void collect_asts(Vec<BaseAST*>* asts, BaseAST* ast) {
  asts->add(ast);
  Vec<BaseAST*> next_asts;
  get_ast_children(ast, next_asts);
  forv_Vec(BaseAST, next_ast, next_asts) {
    collect_asts(asts, next_ast);
  }
}

void collect_asts(Vec<BaseAST*>* asts) {
  collect_asts(asts, rootModule);
}

void collect_asts_postorder(Vec<BaseAST*>* asts, BaseAST* ast) {
  Vec<BaseAST*> next_asts;
  get_ast_children(ast, next_asts);
  forv_Vec(BaseAST, next_ast, next_asts) {
    collect_asts_postorder(asts, next_ast);
  }
  asts->add(ast);
}

void collect_asts_postorder(Vec<BaseAST*>* asts) {
  collect_asts_postorder(asts, rootModule);
}


void collect_top_asts(Vec<BaseAST*>* asts, BaseAST* ast) {
  Vec<BaseAST*> next_asts;
  get_ast_children(ast, next_asts);
  forv_Vec(BaseAST, next_ast, next_asts) {
    if (!isSymbol(next_ast) || isArgSymbol(next_ast))
      collect_top_asts(asts, next_ast);
  }
  asts->add(ast);
}


void reset_line_info(BaseAST* baseAST, int lineno) {
  Vec<BaseAST*> asts;
  collect_asts(&asts, baseAST);
  forv_Vec(BaseAST, ast, asts) {
    ast->lineno = lineno;
  }
}


void clear_line_info(BaseAST* baseAST) {
  reset_line_info(baseAST, -1);
}


void compute_call_sites() {
  forv_Vec(FnSymbol, fn, gFns) {
    if (fn->calledBy)
      fn->calledBy->clear();
    else
      fn->calledBy = new Vec<CallExpr*>();
  }
  forv_Vec(BaseAST, ast, gAsts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (FnSymbol* fn = call->isResolved()) {
        if (fn->calledBy) { // yuck, got some functions being called
                            // that are no longer in the tree, e.g.,
                            // _INIT_CONFIG
          fn->calledBy->add(call);
        }
      }
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
  collect_asts(&asts, fn);
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
          arg->type->symbol->hasPragma("array") || // pass by reference
          arg->type->symbol->hasPragma("domain")) { // pass by reference
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
  collect_asts(&asts, fn);
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


#define XSUB(_x, _t)                               \
  if (_x) {                                        \
    if (BaseAST *b = map->get(_x)) {               \
      if (_t* _y = to##_t(b)) {                    \
        _x = _y;                                   \
      } else {                                     \
        INT_FATAL("Major error in update_symbols"); \
      }                                            \
    }                                              \
  }

void update_symbols(BaseAST* ast, ASTMap* map) {
  Vec<BaseAST*> asts;
  collect_asts(&asts, ast);
  forv_Vec(BaseAST, ast, asts) {
    if (SymExpr* sym_expr = toSymExpr(ast)) {
      XSUB(sym_expr->var, Symbol);
    } else if (DefExpr* defExpr = toDefExpr(ast)) {
      XSUB(defExpr->sym->type, Type);
    } else if (VarSymbol* ps = toVarSymbol(ast)) {
      XSUB(ps->type, Type);
    } else if (FnSymbol* ps = toFnSymbol(ast)) {
      XSUB(ps->type, Type);
      XSUB(ps->retType, Type);
      XSUB(ps->_this, Symbol);
      XSUB(ps->_outer, Symbol);
    } else if (ArgSymbol* ps = toArgSymbol(ast)) {
      XSUB(ps->type, Type);
    }
  }
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

  Vec<BaseAST*> asts;
  get_ast_children(ast, asts);
  forv_Vec(BaseAST, ast, asts)
    insert_help(ast, parentExpr, parentSymbol);
}


void remove_help(BaseAST* ast) {
  Vec<BaseAST*> asts;
  get_ast_children(ast, asts);
  forv_Vec(BaseAST, a, asts)
    remove_help(a);

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


static void
pruneVisit(TypeSymbol* ts, Vec<FnSymbol*>& fns, Vec<TypeSymbol*>& types) {
  types.set_add(ts);
  Vec<BaseAST*> asts;
  collect_asts(&asts, ts);
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = toDefExpr(ast))
      if (def->sym->type && !types.set_in(def->sym->type->symbol))
        pruneVisit(def->sym->type->symbol, fns, types);
  }
  if (ts->hasPragma("data class"))
    pruneVisit(toType(ts->type->substitutions.v[0].value)->symbol, fns, types);
}


static void
pruneVisit(FnSymbol* fn, Vec<FnSymbol*>& fns, Vec<TypeSymbol*>& types) {
  fns.set_add(fn);
  Vec<BaseAST*> asts;
  collect_asts(&asts, fn);
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (FnSymbol* next = call->isResolved())
        if (!fns.set_in(next))
          pruneVisit(next, fns, types);
    } else if (SymExpr* se = toSymExpr(ast)) {
      if (se->var && se->var->type && !types.set_in(se->var->type->symbol))
        pruneVisit(se->var->type->symbol, fns, types);
    }
  }
}


void
prune() {
  Vec<FnSymbol*> fns;
  Vec<TypeSymbol*> types;
  pruneVisit(chpl_main, fns, types);
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
        if (ts->hasPragma("ref") &&
            toClassType(ts->type->getField("_val")->type))
          continue;

        //
        // delete reference type only if type is not used
        //
        if (ts->type->refType)
          ts->type->refType->symbol->defPoint->remove();
        ts->defPoint->remove();
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
