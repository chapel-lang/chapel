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

//
// complex2record
//
// This pass changes the primitive complex type to a record.  As an
// optimization, unimplemented, it can be turned off to take advantage
// of C99 support of complex types.
//

#include "astutil.h"
#include "build.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"

static AggregateType*
buildComplexRecord(const char* name, Type* real) {
  AggregateType* ct = new AggregateType(AGGREGATE_RECORD);
  TypeSymbol* ts = new TypeSymbol(name, ct);
  ct->fields.insertAtTail(new DefExpr(new VarSymbol("re", real)));
  ct->fields.insertAtTail(new DefExpr(new VarSymbol("im", real)));
  ts->addFlag(FLAG_NO_CODEGEN);
  rootModule->block->insertAtTail(new DefExpr(ts));
  return ct;
}

#define complex2rec(t)                                          \
  ((t == dtComplex[COMPLEX_SIZE_64]) ? complex64 :              \
   ((t == dtComplex[COMPLEX_SIZE_128]) ? complex128 : 0))

#define complex2real(t)                                                 \
  ((t == dtComplex[COMPLEX_SIZE_64]) ? dtReal[FLOAT_SIZE_32] :         \
   ((t == dtComplex[COMPLEX_SIZE_128]) ? dtReal[FLOAT_SIZE_64] : 0))

void
complex2record() {
  SET_LINENO(rootModule);
  return;
  AggregateType* complex64 = buildComplexRecord("_complex64", dtReal[FLOAT_SIZE_32]);
  AggregateType* complex128 = buildComplexRecord("_complex128", dtReal[FLOAT_SIZE_64]);

  complex64->GEPMap.insert(std::pair<std::string, int>("re", 0));
  complex64->GEPMap.insert(std::pair<std::string, int>("im", 1));
  complex128->GEPMap.insert(std::pair<std::string, int>("re", 0));
  complex128->GEPMap.insert(std::pair<std::string, int>("im", 1));

  complex64->refType = dtComplex[COMPLEX_SIZE_64]->refType;
  dtComplex[COMPLEX_SIZE_64]->refType = NULL;
  complex128->refType = dtComplex[COMPLEX_SIZE_128]->refType;
  dtComplex[COMPLEX_SIZE_128]->refType = NULL;

  forv_Vec(SymExpr, se, gSymExprs) {
    if (VarSymbol* var = toVarSymbol(se->var)) {
      if (is_complex_type(var->type)) {
        if (var->immediate) {
          AggregateType* ct = complex2rec(se->var->type);
          VarSymbol* tmp = newTemp(ct);
          se->getStmtExpr()->insertBefore(new DefExpr(tmp));
          se->getStmtExpr()->insertBefore(new CallExpr(PRIM_SET_MEMBER, tmp, ct->getField(1), complex2real(se->var->type)->defaultValue));
          se->getStmtExpr()->insertBefore(new CallExpr(PRIM_SET_MEMBER, tmp, ct->getField(2), complex2real(se->var->type)->defaultValue));
          se->replace(new SymExpr(tmp));
        }
      }
    } else if (TypeSymbol* ts = toTypeSymbol(se->var)) {
      if (is_complex_type(ts->type)) {
        se->var = complex2rec(ts->type)->symbol;
      }
    }
  }

  forv_Vec(DefExpr, def, gDefExprs) {
    if (!isTypeSymbol(def->sym))
      if (is_complex_type(def->sym->type))
        def->sym->type = complex2rec(def->sym->type);
    if (FnSymbol* fn = toFnSymbol(def->sym))
      if (is_complex_type(fn->retType))
        fn->retType = complex2rec(fn->retType);
  }

  // Change definition of PRIM_GET_REAL and PRIM_GET_IMAG
  // to field selection inline.
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_GET_REAL)) {
      call->primitive = primitives[PRIM_GET_MEMBER];
      AggregateType* ct = toAggregateType(call->get(1)->getValType());
      call->insertAtTail(ct->getField(1));
    } else if (call->isPrimitive(PRIM_GET_IMAG)) {
      call->primitive = primitives[PRIM_GET_MEMBER];
      AggregateType* ct = toAggregateType(call->get(1)->getValType());
      call->insertAtTail(ct->getField(2));
    }
  }

  //
  // change arrays of complexes into arrays of new complex records
  //
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (ts->hasFlag(FLAG_DATA_CLASS)) {
      if (TypeSymbol* nt = getDataClassType(ts)) {
        if (is_complex_type(nt->type)) {
          Type* complexType = complex2rec(nt->type);
          TypeSymbol* complexTypeSymbol = (complexType) ? complexType->symbol : 0;
          setDataClassType(ts, complexTypeSymbol);
        }
      }
    }
  }

  // These are updated so is_complex_type() and is_arithmetic_type() still work.
  dtComplex[COMPLEX_SIZE_64] = complex64;
  dtComplex[COMPLEX_SIZE_128] = complex128;

// The compiler should really treat the representation of the complex type opaquely,
// using macros or primitives to set and access the fields within complex numbers.
// That means this whole "optimization" step is moot; it should be left to runtime
// support how to represent and access complex numbers.
}
