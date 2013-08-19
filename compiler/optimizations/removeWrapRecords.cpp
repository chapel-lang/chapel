#include "astutil.h"
#include "expr.h"
#include "optimizations.h"
#include "passes.h"
#include "resolveIntents.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "type.h"


static Type* getWrapRecordBaseType(Type* type);


//
// removes _array and _domain wrapper records
//
void
removeWrapRecords() {
  //
  // do not remove wrap records if dead code elimination is disabled
  // (or weakened because inlining or copy propagation is disabled)
  // because code associated with accesses to the removed
  // _valueType field will remain
  //
  if (fNoDeadCodeElimination || fNoInline || fNoCopyPropagation)
    return;

  //
  // replace use of _valueType field with type
  //
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_GET_PRIV_CLASS)) {
      SET_LINENO(call);
      call->get(1)->replace(new SymExpr(call->get(1)->typeInfo()->symbol));
    }
  }

  //
  // remove defs of _valueType field
  //
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_SET_MEMBER) ||
        call->isPrimitive(PRIM_GET_MEMBER) ||
        call->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
      if (SymExpr* se = toSymExpr(call->get(2))) {
        if (!strcmp(se->var->name, "_valueType")) {
          se->getStmtExpr()->remove();
        }
      }
    }
  }

  //
  // remove _valueType fields
  //
  forv_Vec(ClassType, ct, gClassTypes) {
    for_fields(field, ct) {
      if (!strcmp(field->name, "_valueType"))
        field->defPoint->remove();
    }
  }

  //
  // remove formals for _valueType fields in constructors
  //
  compute_call_sites();
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    for_formals(formal, fn) {
      if (!strcmp(formal->name, "_valueType")) {
        forv_Vec(CallExpr, call, *fn->calledBy) {
          formal_to_actual(call, formal)->remove();
        }
        formal->defPoint->remove();
      }        
    }
  }

  //
  // replace accesses of _value with wrap record
  //
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_SET_MEMBER)) {
      if (SymExpr* se = toSymExpr(call->get(1))) {
        if (isRecordWrappedType(se->var->type)) {
          call->primitive = primitives[PRIM_MOVE];
          call->get(2)->remove();
        }
      }
    } else if (call->isPrimitive(PRIM_GET_MEMBER)) {
      if (SymExpr* se = toSymExpr(call->get(1))) {
        if (isRecordWrappedType(se->var->type)) {
          call->primitive = primitives[PRIM_ADDR_OF];
          call->get(2)->remove();
        }
      }
    } else if (call->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
      if (SymExpr* se = toSymExpr(call->get(1))) {
        if (isRecordWrappedType(se->var->type)) {
          call->replace(se->remove());
        }
        if (se->var->type->symbol->hasFlag(FLAG_REF)) {
          Type* vt = se->getValType();
          if (isRecordWrappedType(vt)) {
            SET_LINENO(call);
            call->replace(new CallExpr(PRIM_DEREF, se->remove()));
          }
        }
      }
    }
  }

  //
  // scalar replace wrap records
  //
  forv_Vec(VarSymbol, var, gVarSymbols) {
    if (Type* type = getWrapRecordBaseType(var->type))
      if (!var->defPoint->parentSymbol->hasFlag(FLAG_REF))
        var->type = type;
  }
  forv_Vec(ArgSymbol, arg, gArgSymbols) {
    if (Type* type = getWrapRecordBaseType(arg->type)) {
      arg->intent = blankIntentForType(type); // see test/arrays/deitz/part7/test_out_array
      arg->type = type;
    }
  }
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (Type* type = getWrapRecordBaseType(fn->retType))
      fn->retType = type;
  }

  //
  // fix array element type for arrays of arrays and arrays of domains
  //
  forv_Vec(ClassType, ct, gClassTypes) {
    if (ct->symbol->hasFlag(FLAG_DATA_CLASS)) {
      if (TypeSymbol* ts = getDataClassType(ct->symbol)) {
        if (isRecordWrappedType(ts->typeInfo())) {
          setDataClassType(ct->symbol, ts->type->getField("_value")->type->symbol);
        }
      }
    }
  }
}


static Type*
getWrapRecordBaseType(Type* type) {
  if (isRecordWrappedType(type)) {
    return type->getField("_value")->type;
  } else if (type->symbol->hasFlag(FLAG_REF)) {
    Type* vt = type->getValType();
    if (isRecordWrappedType(vt)) {
      return vt->getField("_value")->type->refType;
    }
  }
  return NULL;
}

