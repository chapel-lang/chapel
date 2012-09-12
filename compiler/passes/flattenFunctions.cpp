#include "alist.h"
#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"


//
// returns true if the symbol is defined in an outer function to fn
// third argument not used at call site
//
static bool
isOuterVar(Symbol* sym, FnSymbol* fn, Symbol* parent = NULL) {
  if (!parent)
    parent = fn->defPoint->parentSymbol;
  if (!isFnSymbol(parent))
    return false;
  else if (sym->defPoint->parentSymbol == parent)
    return true;
  else
    return isOuterVar(sym, fn, parent->defPoint->parentSymbol);
}


//
// finds outer vars directly used in a function
//
static void
findOuterVars(FnSymbol* fn, SymbolMap* uses) {
  Vec<BaseAST*> asts;
  collect_asts(fn, asts);
  forv_Vec(BaseAST, ast, asts) {
    if (SymExpr* symExpr = toSymExpr(ast)) {
      Symbol* sym = symExpr->var;
      if (toVarSymbol(sym) || toArgSymbol(sym))
        if (isOuterVar(sym, fn))
          uses->put(sym,gNil);
    }
  }
}


static void
addVarsToFormals(FnSymbol* fn, SymbolMap* vars) {
  form_Map(SymbolMapElem, e, *vars) {
    if (Symbol* sym = e->key) {
      Type* type = sym->type;
      if (type->refType)
        type = type->refType;
      ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, sym->name, type);
      if (!strcmp(sym->name, "this"))
        arg->addFlag(FLAG_ARG_THIS);
      fn->insertFormalAtTail(new DefExpr(arg));
      vars->put(sym, arg);
    }
  }
}

static void
replaceVarUsesWithFormals(FnSymbol* fn, SymbolMap* vars) {
  Vec<BaseAST*> asts;
  collect_asts(fn->body, asts);
  form_Map(SymbolMapElem, e, *vars) {
    if (Symbol* sym = e->key) {
      ArgSymbol* arg = toArgSymbol(e->value);
      Type* type = arg->type;
      forv_Vec(BaseAST, ast, asts) {
        if (SymExpr* se = toSymExpr(ast)) {
          if (se->var == sym) {
            if (type == sym->type) {
              se->var = arg;
            } else {
              CallExpr* call = toCallExpr(se->parentExpr);
              INT_ASSERT(call);
              FnSymbol* fn = call->isResolved();
              if ((call->isPrimitive(PRIM_MOVE) && call->get(1) == se) ||
                  (call->isPrimitive(PRIM_SET_MEMBER) && call->get(1) == se) ||
                  (call->isPrimitive(PRIM_GET_MEMBER)) ||
                  (call->isPrimitive(PRIM_GET_MEMBER_VALUE)) ||
                  (call->isPrimitive(PRIM_WIDE_GET_LOCALE)) ||
                  (fn && arg->type == actual_to_formal(se)->type)) {
                se->var = arg; // do not dereference argument in these cases
              } else if (call->isPrimitive(PRIM_ADDR_OF)) {
                SET_LINENO(se);
                call->replace(new SymExpr(arg));
              } else {
                SET_LINENO(se);
                VarSymbol* tmp = newTemp(sym->type);
                se->getStmtExpr()->insertBefore(new DefExpr(tmp));
                se->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_DEREF, arg)));
                se->var = tmp;
              }
            }
          }
        }
      }
    }
  }
}


static void
addVarsToActuals(CallExpr* call, SymbolMap* vars, bool outerCall) {
  form_Map(SymbolMapElem, e, *vars) {
    if (Symbol* sym = e->key) {
      if (!outerCall && sym->type->refType) {
        VarSymbol* tmp = newTemp(sym->type->refType);
        call->getStmtExpr()->insertBefore(new DefExpr(tmp));
        call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_ADDR_OF, sym)));
        call->insertAtTail(tmp);
      } else {
        call->insertAtTail(sym);
      }
    }
  }
}


void
flattenNestedFunctions(Vec<FnSymbol*>& nestedFunctions) {
  compute_call_sites();

  Vec<FnSymbol*> nestedFunctionSet;
  forv_Vec(FnSymbol, fn, nestedFunctions)
    nestedFunctionSet.set_add(fn);

  Map<FnSymbol*,SymbolMap*> args_map;
  forv_Vec(FnSymbol, fn, nestedFunctions) {
    SymbolMap* uses = new SymbolMap();
    findOuterVars(fn, uses);
    args_map.put(fn, uses);
  }

  // iterate to get outer vars in a function based on outer vars in
  // functions it calls
  bool change;
  do {
    change = false;
    forv_Vec(FnSymbol, fn, nestedFunctions) {
      Vec<BaseAST*> asts;
      collect_top_asts(fn, asts);
      SymbolMap* uses = args_map.get(fn);
      forv_Vec(BaseAST, ast, asts) {
        if (CallExpr* call = toCallExpr(ast)) {
          if (call->isResolved()) {
            if (FnSymbol* fcall = call->findFnSymbol()) {
              SymbolMap* call_uses = args_map.get(fcall);
              if (call_uses) {
                form_Map(SymbolMapElem, e, *call_uses) {
                  if (isOuterVar(e->key, fn) && !uses->get(e->key)) {
                    uses->put(e->key, gNil);
                    change = true;
                  }
                }
              }
            }
          }
        }
      }
    }
  } while (change);

  // update all call sites of nested functions this must be done
  // before updating the function so that the outer var actuals can be
  // updated with the outer var functions when the formals are updated
  // (in nested functions that call one another)
  forv_Vec(FnSymbol, fn, nestedFunctions) {
    SymbolMap* uses = args_map.get(fn);
    forv_Vec(CallExpr, call, *fn->calledBy) {

      //
      // call not in a nested function; handle the toFollower/toLeader cases
      //
      bool outerCall = false;
      if (FnSymbol* parent = toFnSymbol(call->parentSymbol)) {
        if (!nestedFunctionSet.set_in(parent)) {
          form_Map(SymbolMapElem, use, *uses) {
            if (use->key->defPoint->parentSymbol != parent &&
                !isOuterVar(use->key, parent))
              outerCall = true;
          }
          if (outerCall) {
            nestedFunctionSet.set_add(parent);
            nestedFunctions.add(parent);
            SymbolMap* usesCopy = new SymbolMap();
            form_Map(SymbolMapElem, use, *uses) {
              usesCopy->put(use->key, gNil);
            }
            args_map.put(parent, usesCopy);
          }
        }
      }

      addVarsToActuals(call, uses, outerCall);
    }
  }

  // move nested functions to module level
  forv_Vec(FnSymbol, fn, nestedFunctions) {
    if (isFnSymbol(fn->defPoint->parentSymbol)) {
      ModuleSymbol* mod = fn->getModule();
      Expr* def = fn->defPoint;
      def->remove();
      mod->block->insertAtTail(def);
    }
  }

  // add extra formals to nested functions
  forv_Vec(FnSymbol, fn, nestedFunctions)
    addVarsToFormals(fn, args_map.get(fn));

  // replace outer variable uses with added formals
  forv_Vec(FnSymbol, fn, nestedFunctions)
    replaceVarUsesWithFormals(fn, args_map.get(fn));

  //
  // remove types from functions
  //
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (FnSymbol* fn = toFnSymbol(ts->defPoint->parentSymbol))
      fn->defPoint->insertBefore(ts->defPoint->remove());
  }
}


void flattenFunctions(void) {
  Vec<FnSymbol*> nestedFunctions;
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (isFnSymbol(fn->defPoint->parentSymbol))
      nestedFunctions.add(fn);
  }

  flattenNestedFunctions(nestedFunctions);
}
