#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"
#include "symbol.h"

//
// returnStarTuplesByRefArgs changes all functions that return star
// tuples into function that take, as arguments, references to these
// star tuples and assign the values into these references
//
void returnStarTuplesByRefArgs() {
  compute_call_sites();

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if ((fn->retType->symbol->hasFlag(FLAG_STAR_TUPLE))) {
      SET_LINENO(fn);

      //
      // change function interface to take a reference
      //
      Symbol* ret = fn->getReturnSymbol();
      //
      // Is it redundant to make this both have ref intent and ref type?
      //
      ArgSymbol* arg = new ArgSymbol(INTENT_REF, "_ret", ret->type->refType);
      fn->insertFormalAtTail(arg);
      fn->retType = dtVoid;
      fn->insertBeforeReturn(new CallExpr(PRIM_MOVE, arg, ret));
      CallExpr* call = toCallExpr(fn->body->body.tail);
      INT_ASSERT(call && call->isPrimitive(PRIM_RETURN));
      call->get(1)->replace(new SymExpr(gVoid));

      //
      // update call sites to new interface
      //
      forv_Vec(CallExpr, call, *fn->calledBy) {
        SET_LINENO(call);
        CallExpr* move = toCallExpr(call->parentExpr);
        if (!move) {
          //
          // insert dummy to capture return
          //
          Symbol* tmp = newTemp(ret->type);
          call->insertBefore(new DefExpr(tmp));
          move = new CallExpr(PRIM_MOVE, tmp, call->remove());
          tmp->defPoint->insertAfter(move);
        }
        SymExpr* actual = toSymExpr(move->get(1));
        actual->remove();
        if (actual->typeInfo() != arg->type) {
          Symbol* tmp = newTemp(arg->type);
          move->insertBefore(new DefExpr(tmp));
          move->insertBefore(new CallExpr(PRIM_MOVE, tmp,
                               new CallExpr(PRIM_ADDR_OF, actual)));
          actual = new SymExpr(tmp);
        }
        move->replace(call->remove());
        call->insertAtTail(actual);
      }
    }
  }

  //
  // replace SET/GET_MEMBER primitives on star tuples with
  // SET/GET_SVEC_MEMBER primitives
  //
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_SET_MEMBER) ||
        call->isPrimitive(PRIM_GET_MEMBER) ||
        call->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
      Type* type = call->get(1)->getValType();
      if (type->symbol->hasFlag(FLAG_STAR_TUPLE)) {
        SET_LINENO(call);
        AggregateType* ct = toAggregateType(type);
        SymExpr* se = toSymExpr(call->get(2));
        int i = atoi(se->var->name+1);
        INT_ASSERT(i >= 1 && i <= ct->fields.length);
        if (call->isPrimitive(PRIM_SET_MEMBER))
          call->primitive = primitives[PRIM_SET_SVEC_MEMBER];
        else if (call->isPrimitive(PRIM_GET_MEMBER))
          call->primitive = primitives[PRIM_GET_SVEC_MEMBER];
        else if (call->isPrimitive(PRIM_GET_MEMBER_VALUE))
          call->primitive = primitives[PRIM_GET_SVEC_MEMBER_VALUE];
        call->get(2)->replace(new SymExpr(new_IntSymbol(i)));
      }
    }
  }
}
