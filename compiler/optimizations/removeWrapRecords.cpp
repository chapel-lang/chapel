/*
 * Copyright 2004-2015 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "astutil.h"
#include "expr.h"
#include "optimizations.h"
#include "passes.h"
#include "resolveIntents.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "type.h"
#include "stlUtil.h"


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
  forv_Vec(AggregateType, ct, gAggregateTypes) {
    for_fields(field, ct) {
      if (!strcmp(field->name, "_valueType"))
        field->defPoint->remove();
    }
  }

  //
  // Remove actuals bound to _valueType formals.
  //
  compute_call_sites();
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    for_formals(formal, fn) {
      if (!strcmp(formal->name, "_valueType")) {
        forv_Vec(CallExpr, call, *fn->calledBy) {
          formal_to_actual(call, formal)->remove();
        }
      }        
    }
  }

  //
  // Remove all uses of _valueType formals, and then the formal itself.
  //
  // We need to complete the above action on all functions/formals first,
  // before proceeding to the following loop.  Otherwise in the following
  // scenario:
  //
  //   proc fun1(..., _valueType) {
  //     ...
  //     tmp = fun2(..., _valueType); <-- need to preserve this call.
  //     ...
  //   }
  //
  // Suppose we process fun1() before fun2().  In that case, the code below
  // would remove the indicated call because _valueType still appears as an
  // argument.  Most likely, that call is a call to the compiler-generated
  // default constructor for a record-wrapped type.  It needs to be preserved
  // in its reduced form, e.g. _construct_array(_value).  If the call is
  // removed entirely, then the array tmp is never initialized, and the program
  // computes garbage.
  // In this revised formulation, all _valueType formals and their
  // corresponding actual arguments are removed first.  Then, the call to
  // fun2() no longer contains a reference to the _valueType argument of
  // fun1(), so it is preserved as desired.
  //
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    for_formals(formal, fn) {
      if (!strcmp(formal->name, "_valueType")) {
        // Remove all uses of _valueType within the body of this function.
        std::vector<SymExpr*> symExprs;
        collectSymExprsSTL(fn->body, symExprs);
        for_vector(SymExpr, se, symExprs) {
          // Ignore dead ones.
          if (se->parentSymbol == NULL)
            continue;
          // Weed out all but the formal we're interested in.
          if (se->var != formal)
            continue;
          // OK, remove the entire statement accessing the _valueType formal.
          Expr* stmt = se->getStmtExpr();
          stmt->remove();
        }
        formal->defPoint->remove();
      }        
    }
  }

  //
  // replace accesses of _value with wrap record
  //
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->parentSymbol == NULL)
      continue;

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
  forv_Vec(AggregateType, ct, gAggregateTypes) {
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

