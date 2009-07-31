#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"
#include "symbol.h"

static bool
refNecessary(SymExpr* se,
             Map<Symbol*,Vec<SymExpr*>*>& defMap,
             Map<Symbol*,Vec<SymExpr*>*>& useMap) {
  Vec<SymExpr*>* defs = defMap.get(se->var);
  if (defs && defs->n > 1)
    return true;
  for_uses(use, useMap, se->var) {
    if (CallExpr* call = toCallExpr(use->parentExpr)) {
      if (FnSymbol* fn = call->isResolved()) {
        ArgSymbol* formal = actual_to_formal(use);
        if (formal->defPoint->getFunction()->_this == formal)
          return true;
        if (formal->intent == INTENT_INOUT || formal->intent == INTENT_OUT)
          return true;
        if (formal->type->symbol->hasFlag(FLAG_REF) &&
            (fn->hasFlag(FLAG_ALLOW_REF) ||
             formal->hasFlag(FLAG_WRAP_OUT_INTENT)))
          return true;
      } else if (call->isPrimitive(PRIM_MOVE)) {
        if (refNecessary(toSymExpr(call->get(1)), defMap, useMap))
          return true;
      } else if (call->isPrimitive(PRIM_GET_MEMBER)) {
        CallExpr* move = toCallExpr(call->parentExpr);
        INT_ASSERT(move);
        INT_ASSERT(move->isPrimitive(PRIM_MOVE));
        if (refNecessary(toSymExpr(move->get(1)), defMap, useMap))
          return true;
      } else if (call->isPrimitive(PRIM_SET_MEMBER)) {
        if (!call->get(2)->typeInfo()->refType)
          return true;
      } else if (call->isPrimitive(PRIM_RETURN)) {
        return true;
      } else if (call->isPrimitive(PRIM_GET_LOCALEID)) {
        return true;
      }
    }
  }
  return false;
}


// removes references that are not necessary
void cullOverReferences() {
  //
  // change call of reference function to value function
  //
  Map<Symbol*,Vec<SymExpr*>*> defMap;
  Map<Symbol*,Vec<SymExpr*>*> useMap;
  buildDefUseMaps(defMap, useMap);
  forv_Vec(CallExpr, call, gCallExprs) {
    if (FnSymbol* fn = call->isResolved()) {
      if (FnSymbol* copy = fn->valueFunction) {
        if (CallExpr* move = toCallExpr(call->parentExpr)) {
          INT_ASSERT(move->isPrimitive(PRIM_MOVE));
          SymExpr* se = toSymExpr(move->get(1));
          INT_ASSERT(se);
          if (!refNecessary(se, defMap, useMap) &&
              !fn->retType->symbol->hasFlag(FLAG_ITERATOR_RECORD)) {
            VarSymbol* tmp = newTemp(copy->retType);
            move->insertBefore(new DefExpr(tmp));
            move->insertAfter(new CallExpr(PRIM_MOVE, se->var,
                                new CallExpr(PRIM_SET_REF, tmp)));
            se->var = tmp;
            SymExpr* base = toSymExpr(call->baseExpr);
            base->var = copy;
          }
        } else
          INT_FATAL("unexpected case");
      }
    }
  }
  freeDefUseMaps(defMap, useMap);

  //
  // change functions that return references to arrays and domains
  // into functions that return arrays and domains
  //
  compute_call_sites();
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (Type* vt = fn->retType->getValueType()) {
      if (vt->symbol->hasFlag(FLAG_ARRAY) || vt->symbol->hasFlag(FLAG_DOMAIN)) {
        Symbol* tmp = newTemp(vt);
        CallExpr* ret = toCallExpr(fn->body->body.tail);
        INT_ASSERT(ret->isPrimitive(PRIM_RETURN));
        ret->insertBefore(new DefExpr(tmp));
        ret->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_REF, ret->get(1)->remove())));
        ret->insertAtTail(tmp);
        fn->retType = vt;
        forv_Vec(CallExpr, call, *fn->calledBy) {
          CallExpr* move = toCallExpr(call->parentExpr);
          INT_ASSERT(move && move->isPrimitive(PRIM_MOVE));
          Symbol* tmp = newTemp(vt);
          move->insertBefore(new DefExpr(tmp));
          move->insertAfter(new CallExpr(PRIM_MOVE, move->get(1)->remove(), new CallExpr(PRIM_SET_REF, tmp)));
          move->insertAtHead(tmp);
        }
      }
    }
  }
}
