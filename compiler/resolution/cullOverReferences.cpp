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
        if (formal->type->symbol->hasPragma(PRAG_REF) &&
            fn->hasPragma(PRAG_ALLOW_REF))
          return true;
      } else if (call->isPrimitive(PRIMITIVE_MOVE)) {
        if (refNecessary(toSymExpr(call->get(1)), defMap, useMap))
          return true;
      } else if (call->isPrimitive(PRIMITIVE_GET_MEMBER)) {
        CallExpr* move = toCallExpr(call->parentExpr);
        INT_ASSERT(move);
        INT_ASSERT(move->isPrimitive(PRIMITIVE_MOVE));
        if (refNecessary(toSymExpr(move->get(1)), defMap, useMap))
          return true;
      } else if (call->isPrimitive(PRIMITIVE_SET_MEMBER)) {
        if (!call->get(2)->typeInfo()->refType)
          return true;
      } else if (call->isPrimitive(PRIMITIVE_RETURN)) {
        return true;
      } else if (call->isPrimitive(PRIMITIVE_GET_LOCALEID)) {
        return true;
      }
    }
  }
  return false;
}


static bool
isDerefType(Type* type) {
  return (type->symbol->hasPragma(PRAG_ARRAY) ||
          type->symbol->hasPragma(PRAG_DOMAIN) ||
          type->symbol->hasPragma(PRAG_ITERATOR_CLASS));
}


// removes references that are not necessary
void cullOverReferences() {
  //
  // change call of reference function to value function
  //
  Map<Symbol*,Vec<SymExpr*>*> defMap;
  Map<Symbol*,Vec<SymExpr*>*> useMap;
  buildDefUseMaps(defMap, useMap);
  forv_Vec(BaseAST, ast, gAsts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (FnSymbol* fn = call->isResolved()) {
        if (FnSymbol* copy = fn->valueFunction) {
          if (CallExpr* move = toCallExpr(call->parentExpr)) {
            INT_ASSERT(move->isPrimitive(PRIMITIVE_MOVE));
            SymExpr* se = toSymExpr(move->get(1));
            INT_ASSERT(se);
            if (!refNecessary(se, defMap, useMap)) {
              VarSymbol* tmp = new VarSymbol("_tmp", copy->retType);
              move->insertBefore(new DefExpr(tmp));
              move->insertAfter(new CallExpr(PRIMITIVE_MOVE, se->var,
                                             new CallExpr(PRIMITIVE_SET_REF, tmp)));
              se->var = tmp;
              SymExpr* base = toSymExpr(call->baseExpr);
              base->var = copy;
            }
          } else
            INT_FATAL("unexpected case");
        }
      }
    }
  }
  freeDefUseMaps(defMap, useMap);

  //
  // remove references to array and domain wrapper records
  //   this is essential for handling the valid var pragma
  //   and may be worthwhile/necessary otherwise
  //
  forv_Vec(BaseAST, ast, gAsts) {
    if (Symbol* sym = toSymbol(ast)) {
      if (isTypeSymbol(sym))
        continue;
      if (Type* vt = getValueType(sym->type)) {
        if (isDerefType(vt)) {
          sym->type = vt;
        }
      }
    }
    if (FnSymbol* fn = toFnSymbol(ast)) {
      if (Type* vt = getValueType(fn->retType)) {
        if (isDerefType(vt)) {
          fn->retType = vt;
          fn->retTag = RET_VALUE;
        }
      }
    }
    if (CallExpr* call = toCallExpr(ast)) {
      if (call->isPrimitive(PRIMITIVE_GET_REF) ||
          call->isPrimitive(PRIMITIVE_SET_REF)) {
        Type* vt = call->get(1)->typeInfo();
        if (isReference(vt))
          vt = getValueType(vt);
        if (isDerefType(vt)) {
          call->replace(call->get(1)->remove());
        }
      }
      if (call->isPrimitive(PRIMITIVE_GET_MEMBER)) {
        Type* vt = call->get(2)->typeInfo();
        if (isReference(vt))
          vt = getValueType(vt);
        if (isDerefType(vt)) {
          call->primitive = primitives[PRIMITIVE_GET_MEMBER_VALUE];
        }
      }
      if (call->isPrimitive(PRIMITIVE_ARRAY_GET)) {
        Type* vt = call->typeInfo();
        if (isReference(vt))
          vt = getValueType(vt);
        if (isDerefType(vt)) {
          call->primitive = primitives[PRIMITIVE_ARRAY_GET_VALUE];
        }
      }
    }
  }
}
