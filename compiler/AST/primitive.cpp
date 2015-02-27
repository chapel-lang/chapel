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

#include "primitive.h"

#include "expr.h"
#include "iterator.h"
#include "stringutil.h"
#include "type.h"

static Type*
returnInfoUnknown(CallExpr* call) {
  return dtUnknown;
}

static Type*
returnInfoVoid(CallExpr* call) {
  return dtVoid;
}

static Type*
returnInfoOpaque(CallExpr* call) {
  return dtOpaque;
}

static Type*
returnInfoBool(CallExpr* call) {
  return dtBool;
}

static Type*
returnInfoStringC(CallExpr* call) {
  return dtStringC;
}

static Type*
returnInfoStringCopy(CallExpr* call) {
  return dtStringCopy;
}

static Type*
returnInfoLocaleID(CallExpr* call) {
  return dtLocaleID;
}

static Type*
returnInfoNodeID(CallExpr* call) {
  return NODE_ID_TYPE;
}

static Type*
returnInfoInt32(CallExpr* call) {
  return dtInt[INT_SIZE_32];
}

static Type*
returnInfoInt64(CallExpr* call) {
  return dtInt[INT_SIZE_64];
}

static Type*
returnInfoSizeType(CallExpr* call) {
  return SIZE_TYPE;
}

//
// Since 'int' is equivalent to 'int(64)' currently, this doesn't do
// anything different than returnInfoInt64 does, but it's intended to
// be used in cases where a primitive returns a type that ought to
// track the default 'int' size rather than being hard-coded to a
// specific bit-width.
//
static Type*
returnInfoDefaultInt(CallExpr* call) {
  return returnInfoInt64(call);
}

/*
static Type*
returnInfoUInt32(CallExpr* call) { // unexecuted none/gasnet on 4/25/08
  return dtUInt[INT_SIZE_32];
}

static Type*
returnInfoUInt64(CallExpr* call) {
  return dtUInt[INT_SIZE_64];
}


static Type*
returnInfoReal32(CallExpr* call) {
  return dtReal[FLOAT_SIZE_32];
}

static Type*
returnInfoReal64(CallExpr* call) {
  return dtReal[FLOAT_SIZE_64];
}
*/

static Type*
returnInfoComplexField(CallExpr* call) {  // for get real/imag primitives
  Type *t = call->get(1)->getValType();
  if (t == dtComplex[COMPLEX_SIZE_64]) {
    return dtReal[FLOAT_SIZE_32]->refType;
  } else if (t == dtComplex[COMPLEX_SIZE_128]) {
    return dtReal[FLOAT_SIZE_64]->refType;
  } else {
    INT_FATAL( call, "unsupported complex size");
  }
  return dtUnknown;
}

static Type*
returnInfoFirst(CallExpr* call) {
  return call->get(1)->typeInfo();
}

static Type*
returnInfoFirstDeref(CallExpr* call) {
  return call->get(1)->getValType();
}

static Type*
returnInfoCast(CallExpr* call) {
  Type* t1 = call->get(1)->typeInfo();
  Type* t2 = call->get(2)->typeInfo();
  if (t2->symbol->hasFlag(FLAG_WIDE_CLASS))
    if (wideClassMap.get(t1))
      t1 = wideClassMap.get(t1);
  if (t2->symbol->hasFlag(FLAG_WIDE_REF))
    if (wideRefMap.get(t1))
      t1 = wideRefMap.get(t1);
  return t1;
}

static Type*
returnInfoVal(CallExpr* call) {
  AggregateType* ct = toAggregateType(call->get(1)->typeInfo());
  if (!ct || !ct->symbol->hasFlag(FLAG_REF))
    INT_FATAL(call, "attempt to get value type of non-reference type");
  return ct->getField(1)->type;
}

static Type*
returnInfoRef(CallExpr* call) {
  Type* t = call->get(1)->typeInfo();
  if (!t->refType)
    INT_FATAL(call, "invalid attempt to get reference type");
  return t->refType;
}

// NEEDS TO BE FINISHED WHEN PRIMITIVES ARE REDONE
static Type*
returnInfoNumericUp(CallExpr* call) {
  Type* t1 = call->get(1)->typeInfo();
  Type* t2 = call->get(2)->typeInfo();
  if (is_int_type(t1) && is_real_type(t2))
    return t2;
  if (is_real_type(t1) && is_int_type(t2))
    return t1;
  if (is_int_type(t1) && is_bool_type(t2))
    return t1;
  if (is_bool_type(t1) && is_int_type(t2))
    return t2;
  return t1;
}

static Type*
returnInfoArrayIndexValue(CallExpr* call) {
  SymExpr* sym = toSymExpr(call->get(1));
  INT_ASSERT(sym);
  Type* type = sym->var->type;
  if (type->symbol->hasFlag(FLAG_WIDE_CLASS))
    type = type->getField("addr")->type;
  if (!type->substitutions.n)
    INT_FATAL(call, "bad primitive");
  // Is this conditional necessary?  Can just assume condition is true?
  if (type->symbol->hasFlag(FLAG_DATA_CLASS)) {
    return toTypeSymbol(getDataClassType(type->symbol))->type;
  }
  else {
    return toTypeSymbol(type->substitutions.v[0].value)->type;
  }
}

static Type*
returnInfoArrayIndex(CallExpr* call) {
  return returnInfoArrayIndexValue(call)->refType;
}

static Type*
returnInfoGetMember(CallExpr* call) {
  SymExpr* sym1 = toSymExpr(call->get(1));
  if (!sym1)
    INT_FATAL(call, "bad member primitive");
  AggregateType* ct = toAggregateType(sym1->var->type);
  if (ct->symbol->hasFlag(FLAG_REF))
    ct = toAggregateType(ct->getValType());
  if (!ct)
    INT_FATAL(call, "bad member primitive");
  SymExpr* sym = toSymExpr(call->get(2));
  if (!sym)
    INT_FATAL(call, "bad member primitive");
  VarSymbol* var = toVarSymbol(sym->var);
  if (!var)
    INT_FATAL(call, "bad member primitive");
  if (var->immediate) {
    const char* name = var->immediate->v_string;
    for_fields(field, ct) {
      if (!strcmp(field->name, name))
        return field->type;
    }
  } else
    return var->type;
  INT_FATAL(call, "bad member primitive");
  return NULL;
}

static Type*
returnInfoGetTupleMember(CallExpr* call) {
  AggregateType* ct = toAggregateType(call->get(1)->getValType());
  INT_ASSERT(ct && ct->symbol->hasFlag(FLAG_STAR_TUPLE));
  return ct->getField("x1")->type;
}

static Type*
returnInfoGetTupleMemberRef(CallExpr* call) {
  Type* type = returnInfoGetTupleMember(call);
  return (type->refType) ? type->refType : type;
}

static Type*
returnInfoGetMemberRef(CallExpr* call) {
  AggregateType* ct = toAggregateType(call->get(1)->getValType());
  INT_ASSERT(ct);
  SymExpr* se = toSymExpr(call->get(2));
  INT_ASSERT(se);
  VarSymbol* var = toVarSymbol(se->var);
  INT_ASSERT(var);
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
      field = ct->getField(i);
    }
    INT_ASSERT(field);
    return field->type->refType ? field->type->refType : field->type;
  } else
    return var->type->refType ? var->type->refType : var->type;
}

static Type*
returnInfoEndCount(CallExpr* call) {
  static Type* endCountType = NULL;
  if (endCountType == NULL) {
    forv_Vec(TypeSymbol, ts, gTypeSymbols) {
      if (!strcmp(ts->name, "_EndCount")) {
        endCountType = ts->type;
        break;
      }
    }
  }
  return endCountType;
}

static Type*
returnInfoVirtualMethodCall(CallExpr* call) {
  SymExpr* se = toSymExpr(call->get(1));
  INT_ASSERT(se);
  FnSymbol* fn = toFnSymbol(se->var);
  INT_ASSERT(fn);
  return fn->retType;
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
                         Type *(*areturnInfo)(CallExpr*)) :
  tag(atag),
  name(aname),
  returnInfo(areturnInfo),
  isEssential(false),
  passLineno(false)
{
  primitives_map.put(name, this);
}

static void
prim_def(PrimitiveTag tag, const char* name, Type *(*returnInfo)(CallExpr*),
         bool isEssential = false, bool passLineno = false) {
  primitives[tag] = new PrimitiveOp(tag, name, returnInfo);
  primitives[tag]->isEssential = isEssential;
  primitives[tag]->passLineno = passLineno;
}

static void
prim_def(const char* name, Type *(*returnInfo)(CallExpr*),
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
  prim_def(PRIM_MOVE, "move", returnInfoVoid, false, true);
  prim_def(PRIM_INIT, "init", returnInfoFirstDeref);
  prim_def(PRIM_NO_INIT, "no init", returnInfoFirstDeref);
  prim_def(PRIM_TYPE_INIT, "type init", returnInfoFirstDeref);
  prim_def(PRIM_RETURN, "return", returnInfoFirst, true);
  prim_def(PRIM_YIELD, "yield", returnInfoFirst, true);
  prim_def(PRIM_UNARY_MINUS, "u-", returnInfoFirst);
  prim_def(PRIM_UNARY_PLUS, "u+", returnInfoFirst);
  prim_def(PRIM_UNARY_NOT, "u~", returnInfoFirst);
  prim_def(PRIM_UNARY_LNOT, "!", returnInfoBool);
  prim_def(PRIM_ADD, "+", returnInfoNumericUp);
  prim_def(PRIM_SUBTRACT, "-", returnInfoNumericUp);
  prim_def(PRIM_MULT, "*", returnInfoNumericUp);
  prim_def(PRIM_DIV, "/", returnInfoNumericUp, true); // div by zero is visible
  prim_def(PRIM_MOD, "%", returnInfoFirst); // mod by zero?
  prim_def(PRIM_LSH, "<<", returnInfoFirst);
  prim_def(PRIM_RSH, ">>", returnInfoFirst);
  prim_def(PRIM_EQUAL, "==", returnInfoBool);
  prim_def(PRIM_NOTEQUAL, "!=", returnInfoBool);
  prim_def(PRIM_LESSOREQUAL, "<=", returnInfoBool);
  prim_def(PRIM_GREATEROREQUAL, ">=", returnInfoBool);
  prim_def(PRIM_LESS, "<", returnInfoBool);
  prim_def(PRIM_GREATER, ">", returnInfoBool);
  prim_def(PRIM_AND, "&", returnInfoFirst);
  prim_def(PRIM_OR, "|", returnInfoFirst);
  prim_def(PRIM_XOR, "^", returnInfoFirst);
  prim_def(PRIM_POW, "**", returnInfoNumericUp);

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

  prim_def(PRIM_MIN, "_min", returnInfoFirst);
  prim_def(PRIM_MAX, "_max", returnInfoFirst);

  prim_def(PRIM_SETCID, "setcid", returnInfoVoid, true, true);
  prim_def(PRIM_TESTCID, "testcid", returnInfoBool, false, true);
  prim_def(PRIM_GETCID, "getcid", returnInfoBool, false, true);
  prim_def(PRIM_SET_UNION_ID, "set_union_id", returnInfoVoid, true, true);
  prim_def(PRIM_GET_UNION_ID, "get_union_id", returnInfoDefaultInt, false, true);
  prim_def(PRIM_GET_MEMBER, ".", returnInfoGetMemberRef);
  prim_def(PRIM_GET_MEMBER_VALUE, ".v", returnInfoGetMember, false, true);
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

  // local block primitives
  prim_def(PRIM_LOCAL_CHECK, "local_check", returnInfoVoid, true, true);

  // operations on sync/single vars
  prim_def(PRIM_SYNC_INIT, "sync_init", returnInfoVoid, true);
  prim_def(PRIM_SYNC_DESTROY, "sync_destroy", returnInfoVoid, true);
  prim_def(PRIM_SYNC_LOCK, "sync_lock", returnInfoVoid, true);
  prim_def(PRIM_SYNC_UNLOCK, "sync_unlock", returnInfoVoid, true);
  prim_def(PRIM_SYNC_WAIT_FULL, "sync_wait_full_and_lock", returnInfoVoid, true, true);
  prim_def(PRIM_SYNC_WAIT_EMPTY, "sync_wait_empty_and_lock", returnInfoVoid, true, true);
  prim_def(PRIM_SYNC_SIGNAL_FULL, "sync_mark_and_signal_full", returnInfoVoid, true);
  prim_def(PRIM_SYNC_SIGNAL_EMPTY, "sync_mark_and_signal_empty", returnInfoVoid, true);
  prim_def(PRIM_SINGLE_INIT, "single_init", returnInfoVoid, true);
  prim_def(PRIM_SINGLE_DESTROY, "single_destroy", returnInfoVoid, true);
  prim_def(PRIM_SINGLE_LOCK, "single_lock", returnInfoVoid, true);
  prim_def(PRIM_SINGLE_UNLOCK, "single_unlock", returnInfoVoid, true);
  prim_def(PRIM_SINGLE_WAIT_FULL, "single_wait_full", returnInfoVoid, true, true);
  prim_def(PRIM_SINGLE_SIGNAL_FULL, "single_mark_and_signal_full", returnInfoVoid, true);

  // sync/single var support
  prim_def(PRIM_WRITEEF, "write_EF", returnInfoVoid, true);
  prim_def(PRIM_WRITEFF, "write_FF", returnInfoVoid, true);
  prim_def(PRIM_WRITEXF, "write_XF", returnInfoVoid, true);
  prim_def(PRIM_READFE, "read_FE", returnInfoFirst, true);
  prim_def(PRIM_READFF, "read_FF", returnInfoFirst, true);
  prim_def(PRIM_READXX, "read_XX", returnInfoFirst, true);
  prim_def(PRIM_SYNC_IS_FULL, "sync_is_full", returnInfoBool, true);
  prim_def(PRIM_SINGLE_WRITEEF, "single_write_EF", returnInfoVoid, true);
  prim_def(PRIM_SINGLE_READFF, "single_read_FF", returnInfoFirst, true);
  prim_def(PRIM_SINGLE_READXX, "single_read_XX", returnInfoFirst, true);
  prim_def(PRIM_SINGLE_IS_FULL, "single_is_full", returnInfoBool, true);

  prim_def(PRIM_GET_END_COUNT, "get end count", returnInfoEndCount);
  prim_def(PRIM_SET_END_COUNT, "set end count", returnInfoVoid, true);

  prim_def(PRIM_PROCESS_TASK_LIST, "process task list", returnInfoVoid, true, true);
  prim_def(PRIM_EXECUTE_TASKS_IN_LIST, "execute tasks in list", returnInfoVoid, true, true);
  prim_def(PRIM_FREE_TASK_LIST, "free task list", returnInfoVoid, true, true);

  // task primitives
  prim_def(PRIM_GET_SERIAL, "task_get_serial", returnInfoBool);
  prim_def(PRIM_SET_SERIAL, "task_set_serial", returnInfoVoid, true);

  // These are used for task-aware allocation.
  prim_def(PRIM_SIZEOF, "sizeof", returnInfoSizeType);

  prim_def(PRIM_INIT_FIELDS, "chpl_init_record", returnInfoVoid, true);
  prim_def(PRIM_PTR_EQUAL, "ptr_eq", returnInfoBool);
  prim_def(PRIM_PTR_NOTEQUAL, "ptr_neq", returnInfoBool);
  prim_def(PRIM_IS_SUBTYPE, "is_subtype", returnInfoBool);
  prim_def(PRIM_CAST, "cast", returnInfoCast, false, true);
  prim_def(PRIM_DYNAMIC_CAST, "dynamic_cast", returnInfoCast, false, true);
  prim_def(PRIM_TYPEOF, "typeof", returnInfoFirstDeref);
  prim_def(PRIM_USE, "use", returnInfoVoid, true);
  prim_def(PRIM_USED_MODULES_LIST, "used modules list", returnInfoVoid);
  prim_def(PRIM_TUPLE_EXPAND, "expand_tuple", returnInfoVoid);
  prim_def(PRIM_TUPLE_AND_EXPAND, "and_expand_tuple", returnInfoVoid);

  prim_def(PRIM_CHPL_COMM_GET, "chpl_comm_get", returnInfoVoid, true, true);
  prim_def(PRIM_CHPL_COMM_PUT, "chpl_comm_put", returnInfoVoid, true, true);
  prim_def(PRIM_CHPL_COMM_REMOTE_PREFETCH, "chpl_comm_remote_prefetch", returnInfoVoid, true, true);
  prim_def(PRIM_CHPL_COMM_GET_STRD, "chpl_comm_get_strd", returnInfoVoid, true, true);
  prim_def(PRIM_CHPL_COMM_PUT_STRD, "chpl_comm_put_strd", returnInfoVoid, true, true);

  prim_def(PRIM_ARRAY_SHIFT_BASE_POINTER, "shift_base_pointer", returnInfoVoid, true, true);
  prim_def(PRIM_ARRAY_ALLOC, "array_alloc", returnInfoVoid, true, true);
  prim_def(PRIM_ARRAY_FREE, "array_free", returnInfoVoid, true, true);
  prim_def(PRIM_ARRAY_FREE_ELTS, "array_free_elts", returnInfoVoid, true);
  prim_def(PRIM_ARRAY_GET, "array_get", returnInfoArrayIndex, false, true);
  prim_def(PRIM_ARRAY_GET_VALUE, "array_get_value", returnInfoArrayIndexValue, false, true);
  // PRIM_ARRAY_SET is unused by compiler, runtime, modules
  prim_def(PRIM_ARRAY_SET, "array_set", returnInfoVoid, true, true);
  prim_def(PRIM_ARRAY_SET_FIRST, "array_set_first", returnInfoVoid, true, true);

  prim_def(PRIM_ERROR, "error", returnInfoVoid, true);
  prim_def(PRIM_WARNING, "warning", returnInfoVoid, true);
  prim_def(PRIM_WHEN, "when case expressions", returnInfoVoid);
  prim_def(PRIM_TYPE_TO_STRING, "typeToString", returnInfoStringC);

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

  prim_def(PRIM_FORALL_LOOP, "forall loop", returnInfoVoid);
  prim_def(PRIM_TO_LEADER, "to leader", returnInfoVoid);
  prim_def(PRIM_TO_FOLLOWER, "to follower", returnInfoVoid);
  prim_def(PRIM_TO_STANDALONE, "to standalone", returnInfoVoid);

  prim_def(PRIM_DELETE, "delete", returnInfoVoid);

  prim_def(PRIM_CALL_DESTRUCTOR, "call destructor", returnInfoVoid, true);

  prim_def(PRIM_LOGICAL_FOLDER, "_paramFoldLogical", returnInfoBool);

  prim_def(PRIM_WIDE_GET_LOCALE, "_wide_get_locale", returnInfoLocaleID, false, true);
  // This will be unnecessary once the module code calls the corresponding
  // function directly.
  prim_def(PRIM_WIDE_GET_NODE, "_wide_get_node", returnInfoNodeID, false, true);
  prim_def(PRIM_WIDE_GET_ADDR, "_wide_get_addr", returnInfoInt64, false, true);

  prim_def(PRIM_ON_LOCALE_NUM, "chpl_on_locale_num", returnInfoLocaleID);

  prim_def(PRIM_HEAP_REGISTER_GLOBAL_VAR, "_heap_register_global_var", returnInfoVoid, true, true);
  prim_def(PRIM_HEAP_BROADCAST_GLOBAL_VARS, "_heap_broadcast_global_vars", returnInfoVoid, true, true);
  prim_def(PRIM_PRIVATE_BROADCAST, "_private_broadcast", returnInfoVoid, true, true);

  prim_def(PRIM_INT_ERROR, "_internal_error", returnInfoVoid, true);

  prim_def(PRIM_CAPTURE_FN, "capture fn", returnInfoVoid);
  prim_def(PRIM_CREATE_FN_TYPE, "create fn type", returnInfoVoid);

  prim_def("string_compare", returnInfoDefaultInt, true);
  prim_def("string_contains", returnInfoBool, true);
  prim_def("string_concat", returnInfoStringCopy, true, true);
  prim_def("string_length", returnInfoDefaultInt);
  prim_def("ascii", returnInfoInt32);
  prim_def("string_index", returnInfoStringCopy, true, true);
  prim_def(PRIM_STRING_COPY, "string_copy", returnInfoStringCopy, false, true);
  prim_def(PRIM_C_STRING_FROM_STRING, "c_string_from_string", returnInfoStringC, false, true);
  prim_def(PRIM_CAST_TO_VOID_STAR, "cast_to_void_star", returnInfoOpaque, true, false);
  prim_def("string_select", returnInfoStringCopy, true, true);
  prim_def("sleep", returnInfoVoid, true);
  prim_def("real2int", returnInfoDefaultInt);
  prim_def("object2int", returnInfoDefaultInt);
  prim_def("chpl_exit_any", returnInfoVoid, true);

  prim_def(PRIM_RT_ERROR, "chpl_error", returnInfoVoid, true, true);
  prim_def(PRIM_RT_WARNING, "chpl_warning", returnInfoVoid, true, true);

  prim_def(PRIM_NEW_PRIV_CLASS, "chpl_newPrivatizedClass", returnInfoVoid, true);
  prim_def(PRIM_GET_PRIV_CLASS, "chpl_getPrivatizedClass",  returnInfoFirst);
  
  prim_def(PRIM_GET_USER_LINE, "_get_user_line", returnInfoDefaultInt, true, true);
  prim_def(PRIM_GET_USER_FILE, "_get_user_file", returnInfoStringC, true, true);

  prim_def(PRIM_FTABLE_CALL, "call ftable function", returnInfoVoid, true);

  prim_def(PRIM_IS_SYNC_TYPE, "is sync type", returnInfoBool);
  prim_def(PRIM_IS_SINGLE_TYPE, "is single type", returnInfoBool);
  prim_def(PRIM_IS_TUPLE_TYPE, "is tuple type", returnInfoBool);
  prim_def(PRIM_IS_STAR_TUPLE_TYPE, "is star tuple type", returnInfoBool);
  prim_def(PRIM_SET_SVEC_MEMBER, "set svec member", returnInfoVoid, true, true);
  prim_def(PRIM_GET_SVEC_MEMBER, "get svec member", returnInfoGetTupleMemberRef);
  prim_def(PRIM_GET_SVEC_MEMBER_VALUE, "get svec member value", returnInfoGetTupleMember, false, true);

  prim_def(PRIM_VIRTUAL_METHOD_CALL, "virtual method call", returnInfoVirtualMethodCall, true, true);

  prim_def(PRIM_NUM_FIELDS, "num fields", returnInfoInt32);
  prim_def(PRIM_FIELD_NUM_TO_NAME, "field num to name", returnInfoStringC);
  prim_def(PRIM_FIELD_VALUE_BY_NUM, "field value by num", returnInfoUnknown);
  prim_def(PRIM_FIELD_ID_BY_NUM, "field id by num", returnInfoInt32);
  prim_def(PRIM_FIELD_VALUE_BY_NAME, "field value by name", returnInfoUnknown);
  prim_def(PRIM_IS_UNION_TYPE, "is union type", returnInfoBool);
  prim_def(PRIM_IS_ATOMIC_TYPE, "is atomic type", returnInfoBool);

  prim_def(PRIM_ENUM_MIN_BITS, "enum min bits", returnInfoInt32);
  prim_def(PRIM_ENUM_IS_SIGNED, "enum is signed", returnInfoBool);

  prim_def(PRIM_START_RMEM_FENCE, "chpl_rmem_consist_acquire", returnInfoVoid, true, true);
  prim_def(PRIM_FINISH_RMEM_FENCE, "chpl_rmem_consist_release", returnInfoVoid, true, true);
}

Map<const char*, VarSymbol*> memDescsMap;
Vec<const char*> memDescsVec;

VarSymbol* newMemDesc(const char* str) {
  static int64_t memDescInt = 0;
  const char* s = astr(str);
  if (VarSymbol* v = memDescsMap.get(s))
    return v;
  VarSymbol* memDescVar = new_IntSymbol(memDescInt++, INT_SIZE_16);
  memDescsMap.put(s, memDescVar);
  memDescsVec.add(s);
  return memDescVar;
}
