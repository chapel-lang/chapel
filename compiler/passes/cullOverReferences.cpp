#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"
#include "symbol.h"
#include "symscope.h"

static bool
refNecessary(SymExpr* se) {
  if (se->var->defs.n > 1)
    return true;
  forv_Vec(SymExpr, use, se->var->uses) {
    if (CallExpr* call = toCallExpr(use->parentExpr)) {
      if (call->isResolved()) {
        ArgSymbol* formal = actual_to_formal(use);
        if (formal->defPoint->getFunction()->_this == formal)
          return true;
        if (formal->intent == INTENT_INOUT || formal->intent == INTENT_OUT)
          return true;
      } else if (call->isPrimitive(PRIMITIVE_MOVE)) {
        if (refNecessary(toSymExpr(call->get(1))))
          return true;
      } else if (call->isPrimitive(PRIMITIVE_GET_MEMBER)) {
        CallExpr* move = toCallExpr(call->parentExpr);
        INT_ASSERT(move);
        INT_ASSERT(move->isPrimitive(PRIMITIVE_MOVE));
        if (refNecessary(toSymExpr(move->get(1))))
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
  return (type->symbol->hasPragma("array") ||
          type->symbol->hasPragma("domain") ||
          type->symbol->hasPragma("iterator class"));
}


// removes references that are not necessary
void cullOverReferences() {
  //
  // change call of reference function to value function
  //
  compute_sym_uses();
  forv_Vec(BaseAST, ast, gAsts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (FnSymbol* fn = call->isResolved()) {
        if (FnSymbol* copy = fn->valueFunction) {
          if (CallExpr* move = toCallExpr(call->parentExpr)) {
            INT_ASSERT(move->isPrimitive(PRIMITIVE_MOVE));
            SymExpr* se = toSymExpr(move->get(1));
            INT_ASSERT(se);
            if (!refNecessary(se)) {
              VarSymbol* tmp = new VarSymbol("_tmp", copy->retType);
              move->insertBefore(new DefExpr(tmp));
              move->insertAfter(new CallExpr(PRIMITIVE_MOVE, se->var,
                                             new CallExpr(PRIMITIVE_SET_REF, tmp)));
              se->var = tmp;
              SymExpr* base = toSymExpr(call->baseExpr);
              base->var = copy;
            }
          } else {
            SymExpr* base = toSymExpr(call->baseExpr);
            base->var = copy;
          }
        }
      }
    }
  }

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
