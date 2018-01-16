/*
 * Copyright 2004-2018 Cray Inc.
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

#include "primitive.h"

#include "expr.h"
#include "iterator.h"
#include "stringutil.h"
#include "type.h"
#include "resolution.h"
#include "wellknown.h"

static QualifiedType
returnInfoUnknown(CallExpr* call) {
  return QualifiedType(dtUnknown);
}

static QualifiedType
returnInfoVoid(CallExpr* call) {
  return QualifiedType(dtVoid, QUAL_VAL);
}

static QualifiedType
returnInfoCVoidPtr(CallExpr* call) {
  return QualifiedType(dtCVoidPtr, QUAL_VAL);
}

static QualifiedType
returnInfoBool(CallExpr* call) {
  return QualifiedType(dtBool, QUAL_VAL);
}

static QualifiedType
returnInfoString(CallExpr* call) {
  return QualifiedType(dtString, QUAL_VAL);
}

static QualifiedType
returnInfoStringC(CallExpr* call) {
  return QualifiedType(dtStringC, QUAL_VAL);
}

static QualifiedType
returnInfoWideMake(CallExpr* call) {
  Type* t1 = call->get(1)->typeInfo();
  if (t1->symbol->hasFlag(FLAG_REF))
    INT_FATAL("ref not supported here yet");

  if (wideClassMap.get(t1))
    t1 = wideClassMap.get(t1);

  return QualifiedType(t1, QUAL_VAL);
}


static QualifiedType
returnInfoLocaleID(CallExpr* call) {
  return QualifiedType(dtLocaleID, QUAL_VAL);
}

static QualifiedType
returnInfoNodeID(CallExpr* call) {
  return QualifiedType(NODE_ID_TYPE, QUAL_VAL);
}

static QualifiedType
returnInfoUInt8(CallExpr* call) {
  return QualifiedType(dtUInt[INT_SIZE_8], QUAL_VAL);
}

static QualifiedType
returnInfoInt32(CallExpr* call) {
  return QualifiedType(dtInt[INT_SIZE_32], QUAL_VAL);
}

static QualifiedType
returnInfoInt64(CallExpr* call) {
  return QualifiedType(dtInt[INT_SIZE_64], QUAL_VAL);
}

/*
static QualifiedType
returnInfoUInt64(CallExpr* call) {
  return QualifiedType(dtUInt[INT_SIZE_64], QUAL_VAL);
}
*/

static QualifiedType
returnInfoSizeType(CallExpr* call) {
  return QualifiedType(SIZE_TYPE, QUAL_VAL);
}

//
// Since 'int' is equivalent to 'int(64)' currently, this doesn't do
// anything different than returnInfoInt64 does, but it's intended to
// be used in cases where a primitive returns a type that ought to
// track the default 'int' size rather than being hard-coded to a
// specific bit-width.
//
static QualifiedType
returnInfoDefaultInt(CallExpr* call) {
  return returnInfoInt64(call);
}

/*
static QualifiedType
returnInfoUInt32(CallExpr* call) { // unexecuted none/gasnet on 4/25/08
  return QualifiedType(dtUInt[INT_SIZE_32], QUAL_VAL);
}

static QualifiedType
returnInfoReal32(CallExpr* call) {
  return QualifiedType(dtReal[FLOAT_SIZE_32], QUAL_VAL);
}

static QualifiedType
returnInfoReal64(CallExpr* call) {
  return QualifiedType(dtReal[FLOAT_SIZE_64], QUAL_VAL);
}
*/

static QualifiedType
returnInfoComplexField(CallExpr* call) {  // for get real/imag primitives
  Type *t = call->get(1)->getValType();
  if (t == dtComplex[COMPLEX_SIZE_64]) {
    return QualifiedType(dtReal[FLOAT_SIZE_32]->refType, QUAL_REF);
  } else if (t == dtComplex[COMPLEX_SIZE_128]) {
    return QualifiedType(dtReal[FLOAT_SIZE_64]->refType, QUAL_REF);
  } else {
    INT_FATAL( call, "unsupported complex size");
  }
  return QualifiedType(dtUnknown);
}

static QualifiedType
returnInfoFirst(CallExpr* call) {
  return call->get(1)->qualType();
}

static QualifiedType
returnInfoFirstDeref(CallExpr* call) {
  QualifiedType tmp = call->get(1)->qualType();
  Type* type = tmp.type()->getValType();
  // if it's a tuple, also remove references in the elements
  if (type->symbol->hasFlag(FLAG_TUPLE)) {
    AggregateType* tupleType = toAggregateType(type);
    INT_ASSERT(tupleType);

    type = computeNonRefTuple(tupleType);
  }
  return QualifiedType(type, QUAL_VAL);
}

static QualifiedType
returnInfoScalarPromotionType(CallExpr* call) {
  QualifiedType tmp = call->get(1)->qualType();
  Type* type = tmp.type()->getValType();

  if (type->scalarPromotionType)
    type = type->scalarPromotionType;

  return QualifiedType(type, QUAL_VAL);
}


static QualifiedType
returnInfoCast(CallExpr* call) {
  Type* t1 = call->get(1)->typeInfo();
  Type* t2 = call->get(2)->typeInfo();
  if (t2->symbol->hasFlag(FLAG_WIDE_CLASS)) {
    if (wideClassMap.get(t1))
      t1 = wideClassMap.get(t1);
  }
  if (t2->symbol->hasFlag(FLAG_WIDE_REF)) {
    if (wideRefMap.get(t1))
      t1 = wideRefMap.get(t1);
  }
  return QualifiedType(t1); // what should qual be here?
}

static QualifiedType
returnInfoVal(CallExpr* call) {
  AggregateType* ct = toAggregateType(call->get(1)->typeInfo());

  if (call->get(1)->isRefOrWideRef()) {
    return QualifiedType(call->get(1)->getValType(), QUAL_VAL);
  } else if (ct && ct->symbol->hasFlag(FLAG_WIDE_CLASS)) {
    return QualifiedType(ct, QUAL_VAL);
  }

  INT_FATAL(call, "attempt to get value type of non-reference type");
  return QualifiedType(NULL);
}

// BHARSH TODO: merge this with returnInfoAsRef and fix the WIDE_REF case...
static QualifiedType
returnInfoRef(CallExpr* call) {
  Type* t = call->get(1)->getValType();
  if (!t->refType)
    INT_FATAL(call, "invalid attempt to get reference type");
  return QualifiedType(t->refType, QUAL_REF);
}

static QualifiedType
returnInfoAsRef(CallExpr* call) {
  Type* t = call->get(1)->typeInfo();
  if (isReferenceType(t)) {
    return QualifiedType(t, QUAL_REF);
  }
  else if (t->symbol->hasFlag(FLAG_WIDE_REF)) {
    return QualifiedType(t, QUAL_WIDE_REF);
  }
  else {
    if (!t->refType)
      INT_FATAL(call, "invalid attempt to get reference type");
    return QualifiedType(t->refType, QUAL_REF);
  }
}

// NEEDS TO BE FINISHED WHEN PRIMITIVES ARE REDONE
static QualifiedType
returnInfoNumericUp(CallExpr* call) {
  Type* t1 = call->get(1)->typeInfo()->getValType();
  Type* t2 = call->get(2)->typeInfo()->getValType();
  if (is_int_type(t1) && is_real_type(t2))
    return QualifiedType(t2, QUAL_VAL);
  if (is_real_type(t1) && is_int_type(t2))
    return QualifiedType(t1, QUAL_VAL);
  if (is_int_type(t1) && is_bool_type(t2))
    return QualifiedType(t1, QUAL_VAL);
  if (is_bool_type(t1) && is_int_type(t2))
    return QualifiedType(t2, QUAL_VAL);
  return QualifiedType(t1, QUAL_VAL);
}

static QualifiedType
returnInfoArrayIndexValue(CallExpr* call) {
  Type* type = call->get(1)->typeInfo();
  if (type->symbol->hasFlag(FLAG_WIDE_CLASS))
    type = type->getField("addr")->type;
  if (!type->substitutions.n)
    INT_FATAL(call, "bad primitive");
  // Is this conditional necessary?  Can just assume condition is true?
  if (type->symbol->hasFlag(FLAG_DATA_CLASS)) {
    return QualifiedType(toTypeSymbol(getDataClassType(type->symbol))->type, QUAL_VAL);
  }
  else {
    return QualifiedType(toTypeSymbol(type->substitutions.v[0].value)->type, QUAL_VAL);
  }
}

static QualifiedType
returnInfoArrayIndex(CallExpr* call) {
  QualifiedType tmp = returnInfoArrayIndexValue(call);
  return QualifiedType(tmp.type()->refType, QUAL_REF);
}

static QualifiedType
returnInfoGetMember(CallExpr* call) {
  AggregateType* ct = toAggregateType(call->get(1)->typeInfo());
  if (!ct)
    INT_FATAL(call, "bad member primitive");
  if (ct->symbol->hasFlag(FLAG_REF))
    ct = toAggregateType(ct->getValType());
  SymExpr* sym = toSymExpr(call->get(2));
  if (!sym)
    INT_FATAL(call, "bad member primitive");
  VarSymbol* var = toVarSymbol(sym->symbol());
  if (!var)
    INT_FATAL(call, "bad member primitive");
  if (Immediate* imm = var->immediate) {
    Symbol* field = NULL;
    if (imm->const_kind == CONST_KIND_STRING)
    {
      const char* name = var->immediate->v_string;
      field = ct->getField(name);
    }
    if (imm->const_kind == NUM_KIND_INT)
    {
      int64_t i = imm->int_value();
      field = ct->getField(i);
    }
    INT_ASSERT(field);

    return field->qualType();
  } else
    return sym->qualType();
  INT_FATAL(call, "bad member primitive");
  return QualifiedType(NULL);
}

static QualifiedType
returnInfoGetTupleMember(CallExpr* call) {
  AggregateType* ct = toAggregateType(call->get(1)->getValType());
  INT_ASSERT(ct && ct->symbol->hasFlag(FLAG_STAR_TUPLE));
  return ct->getField("x1")->qualType();
}

static QualifiedType
returnInfoGetTupleMemberRef(CallExpr* call) {
  Type* type = returnInfoGetTupleMember(call).type();
  if (type->refType)
    type = type->refType;
  Qualifier q = QUAL_REF;
  if (call->get(1)->isWideRef()) {
    q = QUAL_WIDE_REF;
    if (Type* t = wideRefMap.get(type)) {
      type = t;
    }
  }
  return QualifiedType(type, q);
}

static QualifiedType
returnInfoGetMemberRef(CallExpr* call) {
  AggregateType* ct = toAggregateType(call->get(1)->getValType());
  INT_ASSERT(ct);
  SymExpr* se = toSymExpr(call->get(2));
  INT_ASSERT(se);
  VarSymbol* var = toVarSymbol(se->symbol());
  INT_ASSERT(var);
  Type* retType = NULL;
  if (Immediate* imm = var->immediate)
  {
    Symbol* field = NULL;
    if (imm->const_kind == CONST_KIND_STRING)
    {
      const char* name = var->immediate->v_string;
      field = ct->getField(name);
    }
    if (imm->const_kind == NUM_KIND_INT)
    {
      int64_t i = imm->int_value();
      if (ct->symbol->hasFlag(FLAG_ITERATOR_CLASS)) {
        // Handle a peculiar intra-pass state where we attempt to get the
        // type of an iterator class field when what we really want is the
        // type of the corresponding formal of the iterator function.
        FnSymbol* fn = getTheIteratorFn(ct);
        field = fn->getFormal(i);
      } else {
        field = ct->getField(i);
      }
    }
    INT_ASSERT(field);
    retType = field->type->refType ? field->type->refType : field->type;
  } else {
    retType = var->type->refType ? var->type->refType : var->type;
  }
  Qualifier q = QUAL_REF;
  if (call->get(1)->isWideRef()) {
    q = QUAL_WIDE_REF;
    if (Type* t = wideRefMap.get(retType)) {
      retType = t;
    }
  }
  return QualifiedType(retType, q);
}

static QualifiedType
returnInfoEndCount(CallExpr* call) {
  static Type* endCountType = NULL;
  if (endCountType == NULL) {
    // Look for the type var `_remoteEndCountType` in ChapelBase.
    forv_Vec(VarSymbol, var, gVarSymbols) {
      const char* searchStr = "_remoteEndCountType";
      if (strcmp(var->cname, searchStr) == 0) {
        endCountType = var->type;
        break;
      }
    }
  }
  return QualifiedType(endCountType, QUAL_VAL);
}

static QualifiedType
returnInfoVirtualMethodCall(CallExpr* call) {
  SymExpr* se = toSymExpr(call->get(1));
  INT_ASSERT(se);
  FnSymbol* fn = toFnSymbol(se->symbol());
  INT_ASSERT(fn);
  return fn->getReturnQualType();
}

static QualifiedType
returnInfoSecondType(CallExpr* call) {
  QualifiedType t = call->get(2)->qualType();
  return t;
}

static QualifiedType
returnInfoIteratorRecordFieldValueByFormal(CallExpr* call) {
  QualifiedType t = call->get(2)->qualType();
  return t;
}

// print the number of each type of primitive present in the AST
void printPrimitiveCounts(const char* passName) {
  int primCounts[NUM_KNOWN_PRIMS];
  for(int i=0; i<NUM_KNOWN_PRIMS; i++) {
    primCounts[i] = 0;
  }

  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->baseExpr == NULL) {
      if (call->primitive) {
        primCounts[call->primitive->tag] += 1;
      }
    }
  }

  printf("NUM_KNOWN_PRIMS = %d\n", NUM_KNOWN_PRIMS);
  for(int i=1; i<NUM_KNOWN_PRIMS; i++) {
    if (primitives[i])
      printf("%s prim[%d] %s %d\n", passName, i, primitives[i]->name, primCounts[i]);
    else
      printf("%s prim[%d] *** UNKNOWN *** %d\n", passName, i, primCounts[i]);
  }

}

HashMap<const char *, StringHashFns, PrimitiveOp *> primitives_map;

PrimitiveOp* primitives[NUM_KNOWN_PRIMS];

PrimitiveOp::PrimitiveOp(PrimitiveTag atag,
                         const char *aname,
                         QualifiedType (*areturnInfo)(CallExpr*)) :
  tag(atag),
  name(aname),
  returnInfo(areturnInfo),
  isEssential(false),
  passLineno(false)
{
  primitives_map.put(name, this);
}

static void
prim_def(PrimitiveTag tag, const char* name, QualifiedType (*returnInfo)(CallExpr*),
         bool isEssential = false, bool passLineno = false) {
  primitives[tag] = new PrimitiveOp(tag, name, returnInfo);
  primitives[tag]->isEssential = isEssential;
  primitives[tag]->passLineno = passLineno;
}

static void
prim_def(const char* name, QualifiedType (*returnInfo)(CallExpr*),
         bool isEssential = false, bool passLineno = false) {
  PrimitiveOp* prim = new PrimitiveOp(PRIM_UNKNOWN, name, returnInfo);
  prim->isEssential = isEssential;
  prim->passLineno = passLineno;
}


/*
 * The routine below, using the routines just above, define primitives
 * for use by the compiler.  Each primitive definition takes:
 *
 * - (optionally) the primitive's enum
 * - its string name
 * - a function pointer indicating the type it returns/evaluates to
 * - (optionally) a boolean saying whether or not it's essential (i.e.,
 *   should not be dead code eliminated)
 * - (optionally) a boolean saying whether or not it expects to receive
 *   filename/lineno arguments
 *
 * Primitives may be defined by the compiler directly (e.g., rewritten
 * during function resolution) or implemented in the runtime.  In the
 * latter case, not surprisingly, it is important to make sure that
 * the return type of the primitive as specified here matches that of
 * the runtime function implementing the primitive.
 */

void
initPrimitive() {
  primitives[PRIM_UNKNOWN] = NULL;

  prim_def(PRIM_ACTUALS_LIST, "actuals list", returnInfoVoid);
  prim_def(PRIM_NOOP, "noop", returnInfoVoid);
  // dst, src. PRIM_MOVE can set a reference.
  prim_def(PRIM_MOVE, "move", returnInfoVoid, false, true);

  prim_def(PRIM_INIT,       "init",       returnInfoFirstDeref);
  prim_def(PRIM_INIT_FIELD, "init field", returnInfoVoid, false, true);
  prim_def(PRIM_INIT_MAYBE_SYNC_SINGLE_FIELD, "init maybe sync/single field",
           returnInfoVoid, false, true);
  prim_def(PRIM_INIT_VAR,   "init var",   returnInfoVoid);
  prim_def(PRIM_NO_INIT,    "no init",    returnInfoFirstDeref);
  prim_def(PRIM_TYPE_INIT,  "type init",  returnInfoFirstDeref);

  prim_def(PRIM_REF_TO_STRING, "ref to string", returnInfoStringC);
  prim_def(PRIM_RETURN, "return", returnInfoFirst, true);
  prim_def(PRIM_THROW, "throw", returnInfoFirst, true, true);
  prim_def(PRIM_TRY_EXPR, "try-expr", returnInfoFirst);
  prim_def(PRIM_TRYBANG_EXPR, "try!-expr", returnInfoFirst);
  prim_def(PRIM_YIELD, "yield", returnInfoFirst, true);
  prim_def(PRIM_UNARY_MINUS, "u-", returnInfoFirstDeref);
  prim_def(PRIM_UNARY_PLUS, "u+", returnInfoFirstDeref);
  prim_def(PRIM_UNARY_NOT, "u~", returnInfoFirstDeref);
  prim_def(PRIM_UNARY_LNOT, "!", returnInfoBool);
  prim_def(PRIM_ADD, "+", returnInfoNumericUp);
  prim_def(PRIM_SUBTRACT, "-", returnInfoNumericUp);
  prim_def(PRIM_MULT, "*", returnInfoNumericUp);
  prim_def(PRIM_DIV, "/", returnInfoNumericUp, true); // div by zero is visible
  prim_def(PRIM_MOD, "%", returnInfoFirstDeref); // mod by zero?
  prim_def(PRIM_LSH, "<<", returnInfoFirstDeref);
  prim_def(PRIM_RSH, ">>", returnInfoFirstDeref);
  prim_def(PRIM_EQUAL, "==", returnInfoBool);
  prim_def(PRIM_NOTEQUAL, "!=", returnInfoBool);
  prim_def(PRIM_LESSOREQUAL, "<=", returnInfoBool);
  prim_def(PRIM_GREATEROREQUAL, ">=", returnInfoBool);
  prim_def(PRIM_LESS, "<", returnInfoBool);
  prim_def(PRIM_GREATER, ">", returnInfoBool);
  prim_def(PRIM_AND, "&", returnInfoFirstDeref);
  prim_def(PRIM_OR, "|", returnInfoFirstDeref);
  prim_def(PRIM_XOR, "^", returnInfoFirstDeref);
  prim_def(PRIM_POW, "**", returnInfoNumericUp);

  // dst, src. PRIM_ASSIGN with reference dst sets *dst
  prim_def(PRIM_ASSIGN, "=", returnInfoVoid, true);
  prim_def(PRIM_ADD_ASSIGN, "+=", returnInfoVoid, true);
  prim_def(PRIM_SUBTRACT_ASSIGN, "-=", returnInfoVoid, true);
  prim_def(PRIM_MULT_ASSIGN, "*=", returnInfoVoid, true);
  prim_def(PRIM_DIV_ASSIGN, "/=", returnInfoVoid, true);
  prim_def(PRIM_MOD_ASSIGN, "%=", returnInfoVoid, true);
  prim_def(PRIM_LSH_ASSIGN, "<<=", returnInfoVoid, true);
  prim_def(PRIM_RSH_ASSIGN, ">>=", returnInfoVoid, true);
  prim_def(PRIM_AND_ASSIGN, "&=", returnInfoVoid, true);
  prim_def(PRIM_OR_ASSIGN, "|=", returnInfoVoid, true);
  prim_def(PRIM_XOR_ASSIGN, "^=", returnInfoVoid, true);
  prim_def(PRIM_REDUCE_ASSIGN, "reduce=", returnInfoVoid, true);

  prim_def(PRIM_MIN, "_min", returnInfoFirst);
  prim_def(PRIM_MAX, "_max", returnInfoFirst);

  prim_def(PRIM_SETCID, "setcid", returnInfoVoid, true, true);
  prim_def(PRIM_TESTCID, "testcid", returnInfoBool, false, true);
  prim_def(PRIM_GETCID, "getcid", returnInfoInt32, false, true);
  prim_def(PRIM_SET_UNION_ID, "set_union_id", returnInfoVoid, true, true);
  prim_def(PRIM_GET_UNION_ID, "get_union_id", returnInfoDefaultInt, false, true);

  // PRIM_GET_MEMBER(_VALUE): aggregate, field
  // if the field is a ref:
  //   GET_MEMBER is invalid AST
  //   GET_MEMBER_VALUE returns the reference
  // if the field is not a ref
  //   GET_MEMBER returns a reference to the field
  //   GET_MEMBER_VALUE returns the field value
  prim_def(PRIM_GET_MEMBER, ".", returnInfoGetMemberRef);
  prim_def(PRIM_GET_MEMBER_VALUE, ".v", returnInfoGetMember, false, true);

  // PRIM_SET_MEMBER: base, field, value
  // if the field is a ref, and the value is a ref, sets the ptr.
  // if the field is a ref, and the value is a not ref, invalid AST
  // if the field is not ref, and the value is a ref, derefs value first
  // if neither are references, sets the field
  prim_def(PRIM_SET_MEMBER, ".=", returnInfoVoid, true, true);

  prim_def(PRIM_CHECK_NIL, "_check_nil", returnInfoVoid, true, true);
  prim_def(PRIM_NEW, "new", returnInfoFirst);
  prim_def(PRIM_GET_REAL, "complex_get_real", returnInfoComplexField);
  prim_def(PRIM_GET_IMAG, "complex_get_imag", returnInfoComplexField);
  prim_def(PRIM_QUERY, "query", returnInfoUnknown);
  prim_def(PRIM_QUERY_PARAM_FIELD, "query param field", returnInfoGetMemberRef);
  prim_def(PRIM_QUERY_TYPE_FIELD, "query type field", returnInfoGetMember);

  prim_def(PRIM_ADDR_OF, "addr of", returnInfoRef);
  prim_def(PRIM_DEREF,   "deref",   returnInfoVal, false, true);
  // If the argument is a reference, simply return it. Otherwise, return a
  // ref to the arg. The result is always a reference.
  prim_def(PRIM_SET_REFERENCE, "set reference", returnInfoAsRef);

  // local block primitives
  prim_def(PRIM_LOCAL_CHECK, "local_check", returnInfoVoid, true, true);

  prim_def(PRIM_GET_END_COUNT, "get end count", returnInfoEndCount);
  prim_def(PRIM_SET_END_COUNT, "set end count", returnInfoVoid, true);

  prim_def(PRIM_GET_DYNAMIC_END_COUNT, "get dynamic end count", returnInfoEndCount);
  prim_def(PRIM_SET_DYNAMIC_END_COUNT, "set dynamic end count", returnInfoVoid, true);


  // task primitives
  prim_def(PRIM_GET_SERIAL, "task_get_serial", returnInfoBool);
  prim_def(PRIM_SET_SERIAL, "task_set_serial", returnInfoVoid, true);

  // These are used for task-aware allocation.
  prim_def(PRIM_SIZEOF, "sizeof", returnInfoSizeType);

  prim_def(PRIM_INIT_FIELDS, "chpl_init_record", returnInfoVoid, true);
  prim_def(PRIM_PTR_EQUAL, "ptr_eq", returnInfoBool);
  prim_def(PRIM_PTR_NOTEQUAL, "ptr_neq", returnInfoBool);
  // PRIM_IS_SUBTYPE arguments are (parent, sub) and it checks
  // if sub is a sub-type of parent.
  prim_def(PRIM_IS_SUBTYPE, "is_subtype", returnInfoBool);
  // PRIM_CAST arguments are (type to cast to, value to cast)
  prim_def(PRIM_CAST, "cast", returnInfoCast, false, true);
  prim_def(PRIM_DYNAMIC_CAST, "dynamic_cast", returnInfoCast, false, true);

  // PRIM_TYPEOF of an array returns a runtime type (containing its domain)
  // For values without a runtime type component, it works the same as
  // PRIM_STATIC_TYPEOF
  prim_def(PRIM_TYPEOF, "typeof", returnInfoFirstDeref);

  // Return the compile-time component of a type (ignoring runtime types)
  // For an array, returns the compile-time type only.
  // (there might be uninitialized memory if the run-time type is used).
  prim_def(PRIM_STATIC_TYPEOF, "static typeof", returnInfoFirstDeref);

  // As with PRIM_STATIC_TYPEOF, returns a compile-time component of
  // a type only. Returns the scalar promotion type (i.e. the type of the
  // elements that iterating over it would yield)
  prim_def(PRIM_SCALAR_PROMOTION_TYPE, "scalar promotion type", returnInfoScalarPromotionType);
  prim_def(PRIM_USED_MODULES_LIST, "used modules list", returnInfoVoid);
  prim_def(PRIM_TUPLE_EXPAND, "expand_tuple", returnInfoVoid);
  prim_def(PRIM_TUPLE_AND_EXPAND, "and_expand_tuple", returnInfoVoid);

  prim_def(PRIM_CHPL_COMM_GET, "chpl_comm_get", returnInfoVoid, true, true);
  prim_def(PRIM_CHPL_COMM_PUT, "chpl_comm_put", returnInfoVoid, true, true);
  prim_def(PRIM_CHPL_COMM_ARRAY_GET, "chpl_comm_array_get", returnInfoVoid, true, true);
  prim_def(PRIM_CHPL_COMM_ARRAY_PUT, "chpl_comm_array_put", returnInfoVoid, true, true);
  prim_def(PRIM_CHPL_COMM_REMOTE_PREFETCH, "chpl_comm_remote_prefetch", returnInfoVoid, true, true);
  prim_def(PRIM_CHPL_COMM_GET_STRD, "chpl_comm_get_strd", returnInfoVoid, true, true);
  prim_def(PRIM_CHPL_COMM_PUT_STRD, "chpl_comm_put_strd", returnInfoVoid, true, true);

  prim_def(PRIM_ARRAY_SHIFT_BASE_POINTER, "shift_base_pointer", returnInfoVoid, true, true);
  prim_def(PRIM_ARRAY_ALLOC, "array_alloc", returnInfoVoid, true, true);
  prim_def(PRIM_ARRAY_FREE, "array_free", returnInfoVoid, true, true);
  prim_def(PRIM_ARRAY_GET, "array_get", returnInfoArrayIndex, false, true);
  prim_def(PRIM_ARRAY_GET_VALUE, "array_get_value", returnInfoArrayIndexValue, false, true);
  // PRIM_ARRAY_SET is unused by compiler, runtime, modules
  prim_def(PRIM_ARRAY_SET, "array_set", returnInfoVoid, true, true);
  prim_def(PRIM_ARRAY_SET_FIRST, "array_set_first", returnInfoVoid, true, true);

  prim_def(PRIM_ERROR, "error", returnInfoVoid, true);
  prim_def(PRIM_WARNING, "warning", returnInfoVoid, true);
  prim_def(PRIM_WHEN, "when case expressions", returnInfoVoid);
  prim_def(PRIM_TYPE_TO_STRING, "typeToString", returnInfoString);

  // These are the block info primitives.
  prim_def(PRIM_BLOCK_PARAM_LOOP, "param loop", returnInfoVoid);
  prim_def(PRIM_BLOCK_WHILEDO_LOOP, "while...do loop", returnInfoVoid);
  prim_def(PRIM_BLOCK_DOWHILE_LOOP, "do...while loop", returnInfoVoid);
  prim_def(PRIM_BLOCK_FOR_LOOP, "for loop", returnInfoVoid);
  prim_def(PRIM_BLOCK_C_FOR_LOOP, "C for loop", returnInfoVoid);
  prim_def(PRIM_BLOCK_BEGIN, "begin block", returnInfoVoid);
  prim_def(PRIM_BLOCK_COBEGIN, "cobegin block", returnInfoVoid);
  prim_def(PRIM_BLOCK_COFORALL, "coforall loop", returnInfoVoid);
  prim_def(PRIM_BLOCK_ON, "on block", returnInfoVoid);
  prim_def(PRIM_BLOCK_BEGIN_ON, "begin on block", returnInfoVoid);
  prim_def(PRIM_BLOCK_COBEGIN_ON, "cobegin on block", returnInfoVoid);
  prim_def(PRIM_BLOCK_COFORALL_ON, "coforall on block", returnInfoVoid);
  prim_def(PRIM_BLOCK_LOCAL, "local block", returnInfoVoid);
  prim_def(PRIM_BLOCK_UNLOCAL, "unlocal block", returnInfoVoid);

  prim_def(PRIM_TO_LEADER, "to leader", returnInfoVoid);
  prim_def(PRIM_TO_FOLLOWER, "to follower", returnInfoVoid);
  prim_def(PRIM_TO_STANDALONE, "to standalone", returnInfoVoid);

  prim_def(PRIM_CALL_DESTRUCTOR, "call destructor", returnInfoVoid, true);

  prim_def(PRIM_LOGICAL_FOLDER, "_paramFoldLogical", returnInfoBool);

  prim_def(PRIM_WIDE_MAKE, "_wide_make", returnInfoWideMake, false, true);
  prim_def(PRIM_WIDE_GET_LOCALE, "_wide_get_locale", returnInfoLocaleID, false, true);
  // MPF - 10/9/2015 - neither _wide_get_node nor _wide_get_addr
  // is used in the module or test code. insertWideReferences uses
  // PRIM_WIDE_GET_NODE. It might make sense to keep both of these
  // functions for debugging.
  prim_def(PRIM_WIDE_GET_NODE, "_wide_get_node", returnInfoNodeID, false, true);
  prim_def(PRIM_WIDE_GET_ADDR, "_wide_get_addr", returnInfoCVoidPtr, false, true);
  prim_def(PRIM_IS_WIDE_PTR, "is wide pointer", returnInfoBool);

  prim_def(PRIM_ON_LOCALE_NUM, "chpl_on_locale_num", returnInfoLocaleID);

  prim_def(PRIM_HEAP_REGISTER_GLOBAL_VAR, "_heap_register_global_var", returnInfoVoid, true, true);
  prim_def(PRIM_HEAP_BROADCAST_GLOBAL_VARS, "_heap_broadcast_global_vars", returnInfoVoid, true, true);
  prim_def(PRIM_PRIVATE_BROADCAST, "_private_broadcast", returnInfoVoid, true, true);

  prim_def(PRIM_INT_ERROR, "_internal_error", returnInfoVoid, true);

  prim_def(PRIM_CAPTURE_FN_FOR_CHPL, "capture fn for chpl", returnInfoVoid);
  prim_def(PRIM_CAPTURE_FN_FOR_C, "capture fn for C", returnInfoVoid);
  prim_def(PRIM_CREATE_FN_TYPE, "create fn type", returnInfoVoid);

  prim_def("string_compare", returnInfoDefaultInt, true);
  prim_def("string_contains", returnInfoBool, true);
  prim_def("string_concat", returnInfoStringC, true, true);
  prim_def("string_length", returnInfoDefaultInt);
  prim_def("ascii", returnInfoUInt8);
  prim_def("string_index", returnInfoStringC, true, true);
  prim_def(PRIM_STRING_COPY, "string_copy", returnInfoStringC, false, true);
  prim_def(PRIM_CAST_TO_VOID_STAR, "cast_to_void_star", returnInfoCVoidPtr, true, false);
  prim_def("string_select", returnInfoStringC, true, true);
  prim_def("sleep", returnInfoVoid, true);
  prim_def("real2int", returnInfoDefaultInt);
  prim_def("object2int", returnInfoDefaultInt);
  prim_def("chpl_exit_any", returnInfoVoid, true);

  prim_def(PRIM_RT_ERROR, "chpl_error", returnInfoVoid, true, true);
  prim_def(PRIM_RT_WARNING, "chpl_warning", returnInfoVoid, true, true);

  prim_def(PRIM_NEW_PRIV_CLASS, "chpl_newPrivatizedClass", returnInfoVoid, true);
  prim_def(PRIM_GET_PRIV_CLASS, "chpl_getPrivatizedClass",  returnInfoFirst);

  prim_def(PRIM_GET_USER_LINE, "_get_user_line", returnInfoDefaultInt, true, true);
  prim_def(PRIM_GET_USER_FILE, "_get_user_file", returnInfoInt32, true, true);

  prim_def(PRIM_FTABLE_CALL, "call ftable function", returnInfoVoid, true);

  prim_def(PRIM_IS_TUPLE_TYPE, "is tuple type", returnInfoBool);
  prim_def(PRIM_IS_STAR_TUPLE_TYPE, "is star tuple type", returnInfoBool);
  // base, index, value
  prim_def(PRIM_SET_SVEC_MEMBER, "set svec member", returnInfoVoid, true, true);
  prim_def(PRIM_GET_SVEC_MEMBER, "get svec member", returnInfoGetTupleMemberRef);
  prim_def(PRIM_GET_SVEC_MEMBER_VALUE, "get svec member value", returnInfoGetTupleMember, false, true);

  prim_def(PRIM_VIRTUAL_METHOD_CALL, "virtual method call", returnInfoVirtualMethodCall, true, true);

  prim_def(PRIM_NUM_FIELDS, "num fields", returnInfoInt32);
  prim_def(PRIM_FIELD_NUM_TO_NAME, "field num to name", returnInfoString);
  prim_def(PRIM_FIELD_NAME_TO_NUM, "field name to num", returnInfoInt32);
  prim_def(PRIM_FIELD_BY_NUM, "field by num", returnInfoUnknown);
  prim_def(PRIM_CLASS_NAME_BY_ID, "class name by id", returnInfoStringC);

  prim_def(PRIM_ITERATOR_RECORD_FIELD_VALUE_BY_FORMAL, "iterator record field value by formal", returnInfoIteratorRecordFieldValueByFormal);
  prim_def(PRIM_IS_EXTERN_CLASS_TYPE, "is extern class type", returnInfoBool);
  prim_def(PRIM_IS_RECORD_TYPE, "is record type", returnInfoBool);
  prim_def(PRIM_IS_UNION_TYPE, "is union type", returnInfoBool);
  prim_def(PRIM_IS_ATOMIC_TYPE, "is atomic type", returnInfoBool);
  prim_def(PRIM_IS_REF_ITER_TYPE, "is ref iter type", returnInfoBool);

  prim_def(PRIM_IS_POD, "is pod type", returnInfoBool);

  // This primitive allows normalize to request function resolution
  // coerce a return value to the declared return type, even though
  // the declared return type is not really known until function
  // resolution.
  // It coerces its first argument to the type stored in the second argument.
  prim_def(PRIM_COERCE, "coerce", returnInfoSecondType);

  prim_def(PRIM_CALL_RESOLVES, "call resolves", returnInfoBool);
  prim_def(PRIM_METHOD_CALL_RESOLVES, "method call resolves", returnInfoBool);

  prim_def(PRIM_ENUM_MIN_BITS, "enum min bits", returnInfoInt32);
  prim_def(PRIM_ENUM_IS_SIGNED, "enum is signed", returnInfoBool);

  prim_def(PRIM_START_RMEM_FENCE, "chpl_rmem_consist_acquire", returnInfoVoid, true, true);
  prim_def(PRIM_FINISH_RMEM_FENCE, "chpl_rmem_consist_release", returnInfoVoid, true, true);

  prim_def(PRIM_LOOKUP_FILENAME, "chpl_lookupFilename", returnInfoStringC, false, false);

  prim_def(PRIM_GET_COMPILER_VAR, "get compiler variable", returnInfoString);

  // Allocate a class instance on the stack (where normally it
  // would be allocated on the heap). The only argument is the class type.
  prim_def(PRIM_STACK_ALLOCATE_CLASS, "stack allocate class", returnInfoFirst);
  prim_def(PRIM_ZIP, "zip", returnInfoVoid, false, false);
  prim_def(PRIM_REQUIRE, "require", returnInfoVoid, false, false);

  prim_def(PRIM_CHECK_ERROR, "check error", returnInfoVoid, false, false);
}

static Map<const char*, VarSymbol*> memDescsMap;
static Map<int, VarSymbol*> memDescsNodeMap;  // key is the Type node's ID
Vec<const char*> memDescsVec;
static int64_t memDescInt = 0;

VarSymbol* newMemDesc(const char* str) {
  const char* s = astr(str);
  if (VarSymbol* v = memDescsMap.get(s))
    return v;
  VarSymbol* memDescVar = new_IntSymbol(memDescInt++, INT_SIZE_16);
  memDescsMap.put(s, memDescVar);
  memDescsVec.add(s);
  return memDescVar;
}

VarSymbol* newMemDesc(Type* type) {
  if (VarSymbol* v = memDescsNodeMap.get(type->id))
    return v;
  VarSymbol* memDescVar = new_IntSymbol(memDescInt++, INT_SIZE_16);
  memDescsNodeMap.put(type->id, memDescVar);
  memDescsVec.add(type->symbol->name);
  return memDescVar;
}

bool getSettingPrimitiveDstSrc(CallExpr* call, Expr** dest, Expr** src)
{
  if (call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN)) {
    // dest, src
    *dest = call->get(1);
    *src = call->get(2);
    return true;
  }

  if (call->isPrimitive(PRIM_SET_MEMBER) ||
      call->isPrimitive(PRIM_SET_SVEC_MEMBER)) {
    // base, field/index, value
    *dest = call->get(1);
    *src = call->get(3);
    return true;
  }

  return false;
}
