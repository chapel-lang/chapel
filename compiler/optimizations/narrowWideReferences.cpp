#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"
#include "view.h"

typedef Map<SymExpr*,Type*> SymExprTypeMap;
typedef MapElem<SymExpr*,Type*> SymExprTypeMapElem;

//#define PRINT_NARROW_EFFECT_SUMMARY
//#define PRINT_NARROW_EFFECT

#ifdef PRINT_NARROW_EFFECT_SUMMARY
  int narrowCount = 0, wideCount = 0;
#endif

static Map<Symbol*,Vec<SymExpr*>*> defMap;
static Map<Symbol*,Vec<SymExpr*>*> useMap;

class WideInfo {
public:
  Symbol* sym;
  bool failToNarrow;
  FnSymbol* fnToNarrow;          // function to narrow if sym is narrowed
  Vec<int> inVec; // if the wide references in this vec are narrowed,
                  // then sym can be narrowed
  Vec<int> outVec; // if sym is not narrowed, then the wide references
                   // in this vec cannot be narrowed

  Vec<SymExpr*> exprsToWiden; // exprs to widen if sym is narrowed
  Vec<CallExpr*> callsToRemove;  // calls to remove if sym is narrowed

  WideInfo(Symbol* isym) : sym(isym), failToNarrow(false), fnToNarrow(NULL) { }
};

static Map<Symbol*,int> idMap;
static Vec<WideInfo*> wideVec;

void
printNode(BaseAST* ast) {
  Symbol* sym = toSymbol(ast);
  int id = idMap.get(sym);
  if (!id) {
    printf("Symbol %d is not in graph", sym->id);
    return;
  }
  WideInfo* wi = wideVec.v[id];
  printf("%d: %s[%d]\n", id, wi->sym->cname, wi->sym->id);
  printf("%s\n", (wi->failToNarrow) ? "WIDE" : "NARROW");
  printf("IN: ");
  for (int i = 0; i < wi->inVec.n; i++)
    printf("%d ", wideVec.v[wi->inVec.v[i]]->sym->id);
  printf("\n");
  printf("OUT: ");
  for (int i = 0; i < wi->outVec.n; i++)
    printf("%d ", wideVec.v[wi->outVec.v[i]]->sym->id);
  printf("\n");
}

void
printNode(int id) {
  printNode(aid(id));
}

static void
forwardPropagateFailToNarrow(WideInfo* wi) {
  for (int i = 0; i < wi->outVec.n; i++) {
    WideInfo* nwi = wideVec.v[wi->outVec.v[i]];
    if (!nwi->failToNarrow) {
      nwi->failToNarrow = true;
      forwardPropagateFailToNarrow(nwi);
    }
  }
}

//
// set of uses that may need to be widened after narrowing
//
static SymExprTypeMap widenMap;


static void
addNarrowDep(Symbol* from, Symbol* to) {
  int fid = idMap.get(from);
  int tid = idMap.get(to);
  INT_ASSERT(fid > 0 && tid > 0);
  wideVec.v[tid]->inVec.add(fid);
  wideVec.v[fid]->outVec.add(tid);
}


static void
narrowField(Symbol* field, WideInfo* wi) {
  TypeSymbol* ts = toTypeSymbol(field->defPoint->parentSymbol);
  if (!strcmp(ts->name, "_class_localson_fn")) {
    wi->failToNarrow = true;
    return;
  }
  if (ts->hasFlag(FLAG_REF) ||
      ts->hasFlag(FLAG_WIDE) ||
      ts->hasFlag(FLAG_WIDE_CLASS)) {
    wi->failToNarrow = true;
    return;
  }

  INT_ASSERT(defMap.get(field) == NULL);

//   for_uses(use, useMap, field) {
//     if (CallExpr* call = toCallExpr(use->parentExpr)) {
//       if (call->isPrimitive(PRIM_SET_MEMBER) && call->get(2) == use) {
//         SymExpr* base = toSymExpr(call->get(1));
//         SymExpr* rhs = toSymExpr(call->get(3));
//         if (base->typeInfo()->symbol->hasFlag(FLAG_WIDE) ||
//             base->getValType()->symbol->hasFlag(FLAG_WIDE_CLASS))
//           addNarrowDep(base->var, field);
//         if (rhs->typeInfo()->symbol->hasFlag(FLAG_WIDE) ||
//             rhs->getValType()->symbol->hasFlag(FLAG_WIDE_CLASS))
//           addNarrowDep(rhs->var, field);
//         continue;
//       }
//       if (call->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
//         continue;
//       }
//     }
    wi->failToNarrow = true;
    return;
//   }
}


static void
narrowSym(Symbol* sym, WideInfo* wi) {
  bool isWideObj = sym->type->symbol->hasFlag(FLAG_WIDE_CLASS);
  bool isWideRef = sym->type->symbol->hasFlag(FLAG_WIDE);
  INT_ASSERT(isWideObj ^ isWideRef);

  for_defs(def, defMap, sym) {
    if (CallExpr* call = toCallExpr(def->parentExpr)) {
      if (call->isPrimitive(PRIM_MOVE)) {
        if (CallExpr* rhs = toCallExpr(call->get(2))) {
          if (rhs->isPrimitive(PRIM_CHPL_ALLOC) ||
              rhs->isPrimitive(PRIM_GET_PRIV_CLASS) ||
              rhs->isPrimitive(PRIM_SET_REF))
            continue;
          if (rhs->isPrimitive(PRIM_GET_MEMBER)) {
            INT_ASSERT(isWideRef);
            SymExpr* base = toSymExpr(rhs->get(1));
            if (base->typeInfo()->symbol->hasFlag(FLAG_WIDE) ||
                base->getValType()->symbol->hasFlag(FLAG_WIDE_CLASS))
              addNarrowDep(base->var, sym);
            continue;
          }
          if (rhs->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
            SymExpr* base = toSymExpr(rhs->get(1));
            SymExpr* member = toSymExpr(rhs->get(2));
            if (base->typeInfo()->symbol->hasFlag(FLAG_WIDE) ||
                base->getValType()->symbol->hasFlag(FLAG_WIDE_CLASS))
              addNarrowDep(base->var, sym);
            if (member->typeInfo()->symbol->hasFlag(FLAG_WIDE) ||
                member->getValType()->symbol->hasFlag(FLAG_WIDE_CLASS))
              addNarrowDep(member->var, sym);
            continue;
          }
          if (rhs->isPrimitive(PRIM_STRING_COPY)) {
            SymExpr* se = toSymExpr(rhs->get(1));
            if (se->typeInfo()->symbol->hasFlag(FLAG_WIDE) ||
                se->getValType()->symbol->hasFlag(FLAG_WIDE_CLASS))
              addNarrowDep(se->var, sym);
            continue;
          }
          if (rhs->isPrimitive(PRIM_CAST)) {
            SymExpr* se = toSymExpr(rhs->get(2));
            if (se->typeInfo()->symbol->hasFlag(FLAG_WIDE) ||
                se->getValType()->symbol->hasFlag(FLAG_WIDE_CLASS))
              addNarrowDep(se->var, sym);
            continue;
          }
          if (FnSymbol* fn = rhs->isResolved()) {
            if ((isWideRef && fn->retType->symbol->hasFlag(FLAG_WIDE)) ||
                (isWideObj && fn->retType->symbol->hasFlag(FLAG_WIDE_CLASS)))
              addNarrowDep(fn->getReturnSymbol(), sym);
            continue;
          }
        }
        if (SymExpr* rhs = toSymExpr(call->get(2))) {
          if ((isWideRef && rhs->var->type->symbol->hasFlag(FLAG_WIDE)) ||
              (isWideObj && rhs->var->type->symbol->hasFlag(FLAG_WIDE_CLASS)))
            addNarrowDep(rhs->var, sym);
          continue;
        }
      }
    }
#ifdef PRINT_NARROW_EFFECT
    printf("%d %s def fail to narrow ", sym->id, sym->cname);
    print_view(def->getStmtExpr());
#endif
    wi->failToNarrow = true;
    return;
  }

  for_uses(use, useMap, sym) {
    if (CallExpr* call = toCallExpr(use->parentExpr)) {
      if ((call->isPrimitive(PRIM_MOVE) && call->get(2) == use) ||
          (call->isPrimitive(PRIM_SET_MEMBER) && call->get(1) == use) ||
          (call->isPrimitive(PRIM_GET_MEMBER) && call->get(1) == use) ||
          (call->isPrimitive(PRIM_GET_MEMBER_VALUE) && call->get(1) == use) ||
          (call->isPrimitive(PRIM_SET_SVEC_MEMBER) && call->get(1) == use) ||
          (call->isPrimitive(PRIM_GET_SVEC_MEMBER) && call->get(1) == use) ||
          (call->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE) && call->get(1) == use) ||
          (call->isPrimitive(PRIM_CAST) && call->get(2) == use) ||
          (call->isPrimitive(PRIM_GET_REF)) ||
          //          (call->isPrimitive(PRIM_GET_LOCALEID)) ||
          (call->isPrimitive(PRIM_SYNC_INIT)) ||
          (call->isPrimitive(PRIM_SYNC_LOCK)) ||
          (call->isPrimitive(PRIM_SYNC_DESTROY)) ||
          (call->isPrimitive(PRIM_NEW_PRIV_CLASS)) ||
          (call->isPrimitive(PRIM_PROCESS_TASK_LIST)) ||
          (call->isPrimitive(PRIM_STRING_COPY)) ||
          (call->isPrimitive(PRIM_SETCID)) ||
          (call->isPrimitive(PRIM_CHPL_ALLOC) && call->get(1) == use) ||
          (call->isPrimitive(PRIM_CHPL_FREE) && call->get(1) == use))
        continue;
      if (call->isResolved() ||
          (call->isPrimitive(PRIM_SET_MEMBER) && call->get(3) == use) ||
          (call->isPrimitive(PRIM_SET_SVEC_MEMBER) && call->get(3) == use)) {
        wi->exprsToWiden.add(use);
        continue;
      }
      if (call->isPrimitive(PRIM_LOCAL_CHECK)) {
        wi->callsToRemove.add(call);
        continue;
      }
      if (call->isPrimitive(PRIM_RETURN)) {
        wi->fnToNarrow = toFnSymbol(call->parentSymbol);
        INT_ASSERT(wi->fnToNarrow);
        forv_Vec(CallExpr*, call, *wi->fnToNarrow->calledBy) {
          if (call->isPrimitive(PRIM_VMT_CALL)) {
            wi->failToNarrow = true;
            return;
          }
        }
        continue;
      }
    }
#ifdef PRINT_NARROW_EFFECT
    printf("%d %s use fail to narrow ", sym->id, sym->cname);
    print_view(use->getStmtExpr());
#endif
    wi->failToNarrow = true;
    return;
  }
}


static void
narrowArg(ArgSymbol* arg, WideInfo* wi) {
  FnSymbol* fn = toFnSymbol(arg->defPoint->parentSymbol);
  INT_ASSERT(fn);
  forv_Vec(FnSymbol, indirectlyCalledFn, ftableVec) {
    if (fn == indirectlyCalledFn) {
      wi->failToNarrow = true;
      return;
    }
  }
  forv_Vec(CallExpr, call, *fn->calledBy) {
    if (call->isPrimitive(PRIM_VMT_CALL)) {
      wi->failToNarrow = true;
      return;
    } else {
      SymExpr* actual = toSymExpr(formal_to_actual(call, arg));
      INT_ASSERT(actual);
      if (actual->var->type->symbol->hasFlag(FLAG_WIDE) ||
          actual->var->type->symbol->hasFlag(FLAG_WIDE_CLASS)) {
        addNarrowDep(actual->var, arg);
      }
    }
  }
  narrowSym(arg, wi);
}


void
narrowWideReferences() {
  buildDefUseMaps(defMap, useMap);

  compute_call_sites();

  wideVec.add(NULL);

  forv_Vec(VarSymbol, var, gVarSymbols) {
    if (var->type->symbol->hasFlag(FLAG_WIDE) ||
        var->type->symbol->hasFlag(FLAG_WIDE_CLASS)) {
      wideVec.add(new WideInfo(var));
      idMap.put(var, wideVec.n-1);
    }
  }

  forv_Vec(ArgSymbol, arg, gArgSymbols) {
    if (arg->type->symbol->hasFlag(FLAG_WIDE) ||
        arg->type->symbol->hasFlag(FLAG_WIDE_CLASS)) {
      wideVec.add(new WideInfo(arg));
      idMap.put(arg, wideVec.n-1);
    }
  }

  forv_Vec(WideInfo, wi, wideVec) if (wi) {
    if (ArgSymbol* arg = toArgSymbol(wi->sym)) {
      narrowArg(arg, wi);
    } else if (VarSymbol* var = toVarSymbol(wi->sym)) {
      if (isFnSymbol(var->defPoint->parentSymbol))
        narrowSym(var, wi);
      else if (isTypeSymbol(var->defPoint->parentSymbol))
        narrowField(var, wi);
      else
        wi->failToNarrow = true;
    }
  }

  forv_Vec(WideInfo, wi, wideVec) if (wi) {
    if (wi->failToNarrow)
      forwardPropagateFailToNarrow(wi);
  }

  forv_Vec(WideInfo, wi, wideVec) if (wi) {
    if (!wi->failToNarrow) {// && wi->inVec.n == 0) {

      //
      // add exprs to widen to the widen map
      //
      forv_Vec(SymExpr, actual, wi->exprsToWiden) {
        widenMap.put(actual, wi->sym->type);
      }

      //
      // remove calls to remove
      //
      forv_Vec(CallExpr, call, wi->callsToRemove) {
        call->remove();
      }

      if (wi->fnToNarrow) {
        wi->fnToNarrow->retType = wi->fnToNarrow->retType->getField("addr")->type;
#ifdef PRINT_NARROW_EFFECT
        printf("%d %s function narrowed\n", wi->fnToNarrow->id, wi->fnToNarrow->cname);
#endif
      }

#ifdef PRINT_NARROW_EFFECT_SUMMARY
      narrowCount++;
#endif
#ifdef PRINT_NARROW_EFFECT
      printf("%d %s narrowed\n", wi->sym->id, wi->sym->cname);
#endif
      wi->sym->type = wi->sym->type->getField("addr")->type;
    }
  }

  forv_Vec(WideInfo, wi, wideVec) if (wi) {
    if (!wi->failToNarrow) { // && wi->inVec.n == 0) {
      if (ArgSymbol* arg = toArgSymbol(wi->sym)) {
        FnSymbol* fn = toFnSymbol(arg->defPoint->parentSymbol);
        forv_Vec(CallExpr, call, *fn->calledBy) {
          SymExpr* actual = toSymExpr(formal_to_actual(call, arg));
          widenMap.put(actual, NULL);
        }
      }
    }
  }

#ifdef PRINT_NARROW_EFFECT_SUMMARY
  forv_Vec(VarSymbol, var, gVarSymbols) {
    if (var->type->symbol->hasFlag(FLAG_WIDE) ||
        var->type->symbol->hasFlag(FLAG_WIDE_CLASS)) {
      wideCount++;
    }
  }

  forv_Vec(ArgSymbol, arg, gArgSymbols) {
    if (arg->type->symbol->hasFlag(FLAG_WIDE) ||
        arg->type->symbol->hasFlag(FLAG_WIDE_CLASS)) {
      wideCount++;
    }
  }
  
  printf("Narrow count: %d\nWide count: %d\n", narrowCount, wideCount);
#endif

  form_Map(SymExprTypeMapElem, e, widenMap) {
    SymExpr* key = e->key;
    Type* value = e->value;
    if (value && key->var->type != value) {
      Symbol* tmp = newTemp(value);
      Expr* stmt = key->getStmtExpr();
      stmt->insertBefore(new DefExpr(tmp));
      key->replace(new SymExpr(tmp));
      stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp, key));
    }
  }

  freeDefUseMaps(defMap, useMap);
}
