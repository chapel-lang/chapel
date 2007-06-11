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
    if (CallExpr* call = dynamic_cast<CallExpr*>(use->parentExpr)) {
      if (call->isResolved()) {
        ArgSymbol* formal = actual_to_formal(use);
        if (formal->defPoint->getFunction()->_this == formal)
          return true;
        if (formal->intent == INTENT_INOUT || formal->intent == INTENT_OUT)
          return true;
      } else if (call->isPrimitive(PRIMITIVE_MOVE)) {
        if (refNecessary(dynamic_cast<SymExpr*>(call->get(1))))
          return true;
      } else if (call->isPrimitive(PRIMITIVE_SET_MEMBER)) {
        if (!call->get(2)->typeInfo()->refType)
          return true;
      } else if (call->isPrimitive(PRIMITIVE_RETURN)) {
        return true;
      }
    }
  }
  return false;
}
    

// removes references that are not necessary
void cullOverReferences() {
  Map<FnSymbol*,FnSymbol*> refMap; // reference fun to value fun

  //
  // make value functions from reference functions
  //
  forv_Vec(FnSymbol, fn, gFns) {
    if (fn->retRef) {
      FnSymbol* copy = fn->copy();
      copy->retRef = false;
      fn->defPoint->insertBefore(new DefExpr(copy));
      VarSymbol* ret = new VarSymbol("ret", getValueType(fn->retType));
      assert(ret->type);
      CallExpr* call = dynamic_cast<CallExpr*>(copy->body->body->last());
      if (!call || !call->isPrimitive(PRIMITIVE_RETURN))
        INT_FATAL(fn, "function is not normal");
      SymExpr* se = dynamic_cast<SymExpr*>(call->get(1));
      if (!se)
        INT_FATAL(fn, "function is not normal");
      call->insertBefore(new DefExpr(ret));
      call->insertBefore(new CallExpr(PRIMITIVE_MOVE, ret,
                           new CallExpr(PRIMITIVE_GET_REF, se->var)));
      se->var = ret;
      copy->retType = ret->type;
      refMap.put(fn, copy);
    }
  }

  //
  // change "setter" to true or false depending on whether the symbol
  // appears in reference or value functions
  //
  forv_Vec(BaseAST, ast, gAsts) {
    if (SymExpr* se = dynamic_cast<SymExpr*>(ast)) {
      if (se->var == gSetter) {
        FnSymbol* fn = dynamic_cast<FnSymbol*>(se->parentSymbol);
        if (!fn)
          INT_FATAL(se, "unexpected case");
        se->var = fn->retRef ? gTrue : gFalse;
      }
    }
  }

  compute_sym_uses();

  forv_Vec(BaseAST, ast, gAsts) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
      //
      // change call of reference function to value function
      //
      if (FnSymbol* fn = call->isResolved()) {
        if (FnSymbol* copy = refMap.get(fn)) {
          if (CallExpr* move = dynamic_cast<CallExpr*>(call->parentExpr)) {
            assert(move->isPrimitive(PRIMITIVE_MOVE));
            SymExpr* se = dynamic_cast<SymExpr*>(move->get(1));
            assert(se);
            if (!refNecessary(se)) {
              VarSymbol* tmp = new VarSymbol("_tmp", copy->retType);
              move->insertBefore(new DefExpr(tmp));
              move->insertAfter(new CallExpr(PRIMITIVE_MOVE, se->var,
                                             new CallExpr(PRIMITIVE_SET_REF, tmp)));
              se->var = tmp;
              SymExpr* base = dynamic_cast<SymExpr*>(call->baseExpr);
              base->var = copy;
            }
          } else {
            SymExpr* base = dynamic_cast<SymExpr*>(call->baseExpr);
            base->var = copy;
          }
        }
      }
    }
  }
}
