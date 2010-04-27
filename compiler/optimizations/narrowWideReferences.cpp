#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"


//#define PRINT_NARROW_EFFECT


Map<Symbol*,Vec<SymExpr*>*> defMap;
Map<Symbol*,Vec<SymExpr*>*> useMap;


//
// set of symbols to narrow
//
Vec<Symbol*> narrowSet;


//
// set of functions to narrow
//
Vec<FnSymbol*> narrowFnSet;


//
// set of uses that will need to be widened after narrowing
//
Map<SymExpr*,Type*> widenMap;


Map<Symbol*,Vec<Symbol*>*> narrowDependenceGraph;


static void narrowArg(ArgSymbol*);


static void
addNarrowDependence(Symbol* from, Symbol* to) {
  Vec<Symbol*>* v = narrowDependenceGraph.get(from);
  if (!v) {
    v = new Vec<Symbol*>();
    narrowDependenceGraph.put(from, v);
  }
  v->add(to);
}


static bool
narrowSym(Symbol* sym) {
  bool verdict = true;

  for_defs(def, defMap, sym) {
    if (CallExpr* call = toCallExpr(def->parentExpr)) {
      if (call->isPrimitive(PRIM_MOVE)) {
        if (CallExpr* rhs = toCallExpr(call->get(2))) {
          if (rhs->isPrimitive(PRIM_CHPL_ALLOC) ||
              rhs->isPrimitive(PRIM_SET_REF))
            continue;
          if (FnSymbol* fn = rhs->isResolved()) {
            if ((!fn->retType->symbol->hasFlag(FLAG_WIDE) &&
                 !fn->retType->symbol->hasFlag(FLAG_WIDE_CLASS)) ||
                narrowFnSet.set_in(fn))
              continue;
            addNarrowDependence(fn, sym);
          }
        }
        if (SymExpr* rhs = toSymExpr(call->get(2))) {
          if ((!rhs->var->type->symbol->hasFlag(FLAG_WIDE) &&
               !rhs->var->type->symbol->hasFlag(FLAG_WIDE_CLASS)) ||
              narrowSet.set_in(rhs->var))
            continue;
          addNarrowDependence(rhs->var, sym);
        }
      }
    }
    verdict = false;
    break;
  }

  //
  // vector of actuals that will need to be added to the widenMap if
  // this symbol is narrowed
  //
  Vec<SymExpr*> actuals;

  //
  // if symbol is returned, mark function to update return type
  //
  FnSymbol* narrowedFn = NULL;

  for_uses(use, useMap, sym) {
    if (CallExpr* call = toCallExpr(use->parentExpr)) {
      if ((call->isPrimitive(PRIM_MOVE) && call->get(2) == use) ||
          (call->isPrimitive(PRIM_SET_MEMBER) && call->get(1) == use) ||
          (call->isPrimitive(PRIM_GET_MEMBER) && call->get(1) == use) ||
          (call->isPrimitive(PRIM_GET_MEMBER_VALUE) && call->get(1) == use) ||
          (call->isPrimitive(PRIM_SET_SVEC_MEMBER) && call->get(1) == use) ||
          (call->isPrimitive(PRIM_GET_SVEC_MEMBER) && call->get(1) == use) ||
          (call->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE) && call->get(1) == use) ||
          (call->isPrimitive(PRIM_PROCESS_TASK_LIST)) ||
          (call->isPrimitive(PRIM_STRING_COPY)) ||
          (call->isPrimitive(PRIM_CHPL_ALLOC) && call->get(1) == use) ||
          (call->isPrimitive(PRIM_CHPL_FREE) && call->get(1) == use))
        continue;
      if (call->isResolved() ||
          (call->isPrimitive(PRIM_SET_MEMBER) && call->get(3) == use) ||
          (call->isPrimitive(PRIM_SET_SVEC_MEMBER) && call->get(3) == use)) {
        actuals.add(use);
        continue;
      }
      if (call->isPrimitive(PRIM_RETURN)) {
        narrowedFn = toFnSymbol(call->parentSymbol);
        INT_ASSERT(narrowedFn);
        forv_Vec(CallExpr*, call, *narrowedFn->calledBy) {
          if (call->isPrimitive(PRIM_VMT_CALL))
            verdict = false;
        }
        continue;
      }
    }
    verdict = false;
    break;
  }

  if (verdict) {
    //
    // if this symbol can be narrowed, add this actual
    //
    forv_Vec(SymExpr, actual, actuals) {
      widenMap.put(actual, sym->type);
    }

    if (narrowedFn) {
      narrowFnSet.set_add(narrowedFn);
#ifdef PRINT_NARROW_EFFECT
      printf("Narrowed %s(%d)\n", narrowedFn->cname, narrowedFn->id);
#endif

      if (Vec<Symbol*>* v = narrowDependenceGraph.get(narrowedFn)) {
        forv_Vec(Symbol, vv, *v) {
          if (ArgSymbol* arg = toArgSymbol(vv))
            narrowArg(arg);
          else
            narrowSym(vv);
        }
      }
    }

    narrowSet.set_add(sym);

    if (Vec<Symbol*>* v = narrowDependenceGraph.get(sym)) {
      forv_Vec(Symbol, vv, *v) {
        if (ArgSymbol* arg = toArgSymbol(vv))
          narrowArg(arg);
        else
          narrowSym(vv);
      }
    }
  }

  return verdict;
}


static void
narrowArg(ArgSymbol* arg) {
  FnSymbol* fn = toFnSymbol(arg->defPoint->parentSymbol);
  INT_ASSERT(fn);
  bool candidate = true;
  forv_Vec(FnSymbol, indirectlyCalledFn, ftableVec) {
    if (fn == indirectlyCalledFn)
      candidate = false;
  }
  forv_Vec(CallExpr, call, *fn->calledBy) {
    if (call->isPrimitive(PRIM_VMT_CALL)) {
      candidate = false;
    } else {
      SymExpr* actual = toSymExpr(formal_to_actual(call, arg));
      INT_ASSERT(actual);
      if (!narrowSet.set_in(actual->var))
        candidate = false;
    }
  }
  if (candidate) {
    bool narrowed = narrowSym(arg);
    if (narrowed) {
      forv_Vec(CallExpr, call, *fn->calledBy) {
        SymExpr* actual = toSymExpr(formal_to_actual(call, arg));
        widenMap.put(actual, NULL);
      }
    }
  }
}


void
narrowWideReferences() {
  buildDefUseMaps(defMap, useMap);

  compute_call_sites();

  forv_Vec(VarSymbol, var, gVarSymbols) {
    if (isFnSymbol(var->defPoint->parentSymbol) &&
        (var->type->symbol->hasFlag(FLAG_WIDE) ||
         var->type->symbol->hasFlag(FLAG_WIDE_CLASS)))
      narrowSym(var);
  }

  forv_Vec(ArgSymbol, arg, gArgSymbols) {
    if (arg->type->symbol->hasFlag(FLAG_WIDE) ||
        arg->type->symbol->hasFlag(FLAG_WIDE_CLASS))
      narrowArg(arg);
  }

#ifdef PRINT_NARROW_EFFECT
  int narrowCount = 0, wideCount = 0;

  forv_Vec(VarSymbol, var, gVarSymbols) {
    if (var->type->symbol->hasFlag(FLAG_WIDE) ||
        var->type->symbol->hasFlag(FLAG_WIDE_CLASS)) {
      if (narrowSet.set_in(var))
        narrowCount++;
      else
        wideCount++;
    }
  }

  forv_Vec(ArgSymbol, arg, gArgSymbols) {
    if (arg->type->symbol->hasFlag(FLAG_WIDE) ||
        arg->type->symbol->hasFlag(FLAG_WIDE_CLASS)) {
      if (narrowSet.set_in(arg))
        narrowCount++;
      else
        wideCount++;
    }
  }
  
  printf("Narrow count: %d\nWide count: %d\n", narrowCount, wideCount);
#endif

  forv_Vec(Symbol, sym, narrowSet) if (sym) {
    sym->type = sym->type->getField("addr")->type;
  }

  forv_Vec(FnSymbol, fn, narrowFnSet) if (fn) {
    fn->retType = fn->retType->getField("addr")->type;
  }

  Vec<SymExpr*> keys;
  widenMap.get_keys(keys);

  forv_Vec(SymExpr, key, keys) {
    if (Type* type = widenMap.get(key)) {
      Symbol* tmp = newTemp(type);
      Expr* stmt = key->getStmtExpr();
      stmt->insertBefore(new DefExpr(tmp));
      key->replace(new SymExpr(tmp));
      stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp, key));
    }
  }

  freeDefUseMaps(defMap, useMap);
}
