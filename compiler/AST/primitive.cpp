#include "expr.h"
#include "primitive.h"
#include "type.h"

static Type*
returnInfoBool(CallExpr* call) {
  return dtBool;
}

static Type*
returnInfoVoid(CallExpr* call) {
  return dtVoid;
}

static Type*
returnInfoFile(CallExpr* call) {
  return dtFile;
}

static Type*
returnInfoTimer(CallExpr* call) {
  return dtTimer;
}

static Type*
returnInfoString(CallExpr* call) {
  return dtString;
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
returnInfoUInt32(CallExpr* call) {
  return dtUInt[INT_SIZE_32];
}

static Type*
returnInfoUInt64(CallExpr* call) {
  return dtUInt[INT_SIZE_64];
}

static Type*
returnInfoReal64(CallExpr* call) {
  return dtReal[FLOAT_SIZE_64];
}

static Type*
returnInfoComplexField(CallExpr* call) {  // for get real/imag primitives
  Type *t = call->get(1)->typeInfo();
  if (t->symbol->hasPragma("ref"))
    t = getValueType(t);
  if (t == dtComplex[COMPLEX_SIZE_64]) {
    return dtReal[FLOAT_SIZE_32]->refType;
  } else if (t == dtComplex[COMPLEX_SIZE_128]) {
    return dtReal[FLOAT_SIZE_64]->refType;
  } else if (t == dtComplex[COMPLEX_SIZE_256]) {
    return dtReal[FLOAT_SIZE_128]->refType;
  } else {
    INT_FATAL( call, "unsupported complex size");
  }
  return dtUnknown;
}

static Type*
returnInfoMutexP( CallExpr* call) {
  return dtMutex_p;
}

static Type*
returnInfoCondVarP( CallExpr* call) {
  return dtCondVar_p;
}

static Type*
returnInfoFirst(CallExpr* call) {
  return call->get(1)->typeInfo();
}

static Type*
returnInfoCast(CallExpr* call) {
  Type* t = call->get(1)->typeInfo();
  if (t->isGeneric) {
    if (SymExpr* sym = toSymExpr(call->get(1)))
      if (sym->var->isTypeVariable)
        return dtUnknown;
    USR_FATAL(call, "Illegal cast to generic type");
  }
  return t;
}

static Type*
returnInfoVal(CallExpr* call) {
  ClassType* ct = toClassType(call->get(1)->typeInfo());
  if (!ct || !ct->symbol->hasPragma("ref"))
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

static Type*
returnInfoMove(CallExpr* call) {
  Type* t1 = call->get(1)->typeInfo();
  Type* t2 = call->get(2)->typeInfo();
  if (t1 == dtUnknown || t1 == dtNil)
    return t2;
  return t1;
}

static Type*
returnInfoICType(CallExpr* call) {
  ClassType* ct = toClassType(call->get(1)->typeInfo());
  return ct->scalarPromotionType; // this is the element type
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
  if (is_int_type(t1) && t2 == dtBool)
    return t1;
  if (t1 == dtBool && is_int_type(t2))
    return t2;
  return t1;
}

static Type*
returnInfoArrayIndex(CallExpr* call) {
  SymExpr* sym = toSymExpr(call->get(1));
  if (!sym || !sym->var->type->substitutions.n)
    INT_FATAL(call, "bad primitive");
  return toType(sym->var->type->substitutions.v[0].value)->refType;
}

static Type*
returnInfoChplAlloc(CallExpr* call) {
  SymExpr* sym = toSymExpr(call->get(1));
  if (!sym)
    INT_FATAL(call, "bad _chpl_alloc primitive");
  return sym->var->type;
}

static Type*
returnInfoGetMember(CallExpr* call) {
  SymExpr* sym1 = toSymExpr(call->get(1));
  if (!sym1)
    INT_FATAL(call, "bad member primitive");
  ClassType* ct = toClassType(sym1->var->type);
  if (ct->symbol->hasPragma("ref"))
    ct = toClassType(getValueType(ct));
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
returnInfoGetMemberRef(CallExpr* call) {
  SymExpr* sym1 = toSymExpr(call->get(1));
  if (!sym1)
    INT_FATAL(call, "bad member primitive");
  ClassType* ct = toClassType(sym1->var->type);
  if (ct->symbol->hasPragma("ref"))
    ct = toClassType(getValueType(ct));
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
        return field->type->refType ? field->type->refType : field->type;
    }
  } else
    return var->type->refType ? var->type->refType : var->type;
  INT_FATAL(call, "bad member primitive");
  return NULL;
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
  PrimitiveOp* prim = new PrimitiveOp(PRIMITIVE_UNKNOWN, name, returnInfo);
  prim->isEssential = isEssential;
  prim->passLineno = passLineno;
}


void
initPrimitive() {
  primitives[PRIMITIVE_UNKNOWN] = NULL;

  prim_def(PRIMITIVE_NOOP, "noop", returnInfoVoid);
  prim_def(PRIMITIVE_MOVE, "move", returnInfoMove);
  prim_def(PRIMITIVE_SET_REF, "set ref", returnInfoRef);
  prim_def(PRIMITIVE_GET_REF, "get ref", returnInfoVal);
  prim_def(PRIMITIVE_RETURN, "return", returnInfoFirst, true);
  prim_def(PRIMITIVE_YIELD, "yield", returnInfoFirst, true);
  prim_def(PRIMITIVE_UNARY_MINUS, "u-", returnInfoFirst);
  prim_def(PRIMITIVE_UNARY_PLUS, "u+", returnInfoFirst);
  prim_def(PRIMITIVE_UNARY_NOT, "u~", returnInfoFirst);
  prim_def(PRIMITIVE_UNARY_LNOT, "!", returnInfoBool);
  prim_def(PRIMITIVE_ADD, "+", returnInfoNumericUp);
  prim_def(PRIMITIVE_SUBTRACT, "-", returnInfoNumericUp);
  prim_def(PRIMITIVE_MULT, "*", returnInfoNumericUp);
  prim_def(PRIMITIVE_DIV, "/", returnInfoNumericUp, true); // div by zero is visible
  prim_def(PRIMITIVE_MOD, "%", returnInfoFirst); // mod by zero?
  prim_def(PRIMITIVE_LSH, "<<", returnInfoFirst);
  prim_def(PRIMITIVE_RSH, ">>", returnInfoFirst);
  prim_def(PRIMITIVE_EQUAL, "==", returnInfoBool);
  prim_def(PRIMITIVE_NOTEQUAL, "!=", returnInfoBool);
  prim_def(PRIMITIVE_LESSOREQUAL, "<=", returnInfoBool);
  prim_def(PRIMITIVE_GREATEROREQUAL, ">=", returnInfoBool);
  prim_def(PRIMITIVE_LESS, "<", returnInfoBool);
  prim_def(PRIMITIVE_GREATER, ">", returnInfoBool);
  prim_def(PRIMITIVE_AND, "&", returnInfoFirst);
  prim_def(PRIMITIVE_OR, "|", returnInfoFirst);
  prim_def(PRIMITIVE_XOR, "^", returnInfoFirst);
  prim_def(PRIMITIVE_POW, "**", returnInfoNumericUp);

  prim_def(PRIMITIVE_MIN, "_min", returnInfoFirst);
  prim_def(PRIMITIVE_MAX, "_max", returnInfoFirst);
  prim_def(PRIMITIVE_PROD_ID, "_prod_id", returnInfoFirst);
  prim_def(PRIMITIVE_LAND_ID, "_land_id", returnInfoBool);
  prim_def(PRIMITIVE_LOR_ID, "_lor_id", returnInfoBool);
  prim_def(PRIMITIVE_LXOR_ID, "_lxor_id", returnInfoBool);
  prim_def(PRIMITIVE_BAND_ID, "_band_id", returnInfoFirst);
  prim_def(PRIMITIVE_BOR_ID, "_bor_id", returnInfoFirst);
  prim_def(PRIMITIVE_BXOR_ID, "_bxor_id", returnInfoFirst);

  prim_def(PRIMITIVE_SETCID, "setcid", returnInfoVoid, true);
  prim_def(PRIMITIVE_GETCID, "getcid", returnInfoBool);
  prim_def(PRIMITIVE_UNION_SETID, "set_union_id", returnInfoVoid, true);
  prim_def(PRIMITIVE_UNION_GETID, "get_union_id", returnInfoBool);
  prim_def(PRIMITIVE_GET_MEMBER, ".", returnInfoGetMemberRef);
  prim_def(PRIMITIVE_GET_MEMBER_VALUE, ".v", returnInfoGetMember);
  prim_def(PRIMITIVE_SET_MEMBER, ".=", returnInfoVoid, true);

  prim_def(PRIMITIVE_GET_REAL, "complex_get_real", returnInfoComplexField);
  prim_def(PRIMITIVE_GET_IMAG, "complex_get_imag", returnInfoComplexField);

  prim_def(PRIMITIVE_GET_MEMBER_REF_TO, ".*", returnInfoVoid);
  prim_def(PRIMITIVE_SET_MEMBER_REF_TO, ".=&", returnInfoVoid, true);
  prim_def(PRIMITIVE_SET_HEAPVAR, "setheapvar", returnInfoMove, true);
  prim_def(PRIMITIVE_REFC_INIT, "refc_init", returnInfoVoid, true);
  prim_def(PRIMITIVE_REFC_TOUCH, "refc_touch", returnInfoVoid, true);
  prim_def(PRIMITIVE_REFC_RELEASE, "refc_release", returnInfoVoid, true, true);

  // thread primitives
  prim_def(PRIMITIVE_THREAD_INIT, "thread_init", returnInfoVoid, true);
  prim_def(PRIMITIVE_THREAD_ID, "thread_id", returnInfoUInt64, true);  // 64-bit
  prim_def(PRIMITIVE_GET_SERIAL, "thread_get_serial", returnInfoBool);
  prim_def(PRIMITIVE_SET_SERIAL, "thread_set_serial", returnInfoVoid, true);

  // operations on sync/single vars
  prim_def(PRIMITIVE_SYNC_LOCK, "sync_lock", returnInfoVoid, true);
  prim_def(PRIMITIVE_SYNC_UNLOCK, "sync_unlock", returnInfoVoid, true);
  prim_def(PRIMITIVE_SYNC_SIGNAL_FULL, "sync_signal_full", returnInfoVoid, true);
  prim_def(PRIMITIVE_SYNC_BROADCAST_FULL,"sync_broadcast_full",returnInfoVoid, true);
  prim_def(PRIMITIVE_SYNC_WAIT_FULL, "sync_wait_full", returnInfoVoid, true);
  prim_def(PRIMITIVE_SYNC_SIGNAL_EMPTY, "sync_signal_empty", returnInfoVoid, true);
  prim_def(PRIMITIVE_SYNC_BROADCAST_EMPTY,"sync_broadcast_empty",returnInfoVoid, true);
  prim_def(PRIMITIVE_SYNC_WAIT_EMPTY, "sync_wait_empty", returnInfoVoid, true);
  // sync/single var support
  prim_def(PRIMITIVE_MUTEX_NEW, "mutex_new", returnInfoMutexP, true);
  prim_def(PRIMITIVE_MUTEX_DESTROY, "mutex_destroy", returnInfoVoid, true);
  prim_def(PRIMITIVE_CONDVAR_NEW, "condvar_new", returnInfoCondVarP, true);
  prim_def(PRIMITIVE_CONDVAR_DESTROY, "condvar_destroy", returnInfoVoid, true);

  prim_def(PRIMITIVE_CHPL_ALLOC, "chpl_alloc", returnInfoChplAlloc, true, true);
  prim_def(PRIMITIVE_CHPL_FREE, "chpl_free", returnInfoVoid, true, true);
  prim_def(PRIMITIVE_PTR_EQUAL, "ptr_eq", returnInfoBool);
  prim_def(PRIMITIVE_PTR_NOTEQUAL, "ptr_neq", returnInfoBool);
  prim_def(PRIMITIVE_ISSUBTYPE, "is_subtype", returnInfoBool);
  prim_def(PRIMITIVE_CAST, "cast", returnInfoCast);
  prim_def(PRIMITIVE_DYNAMIC_CAST, "dynamic_cast", returnInfoCast);
  prim_def(PRIMITIVE_TYPEOF, "typeof", returnInfoFirst);
  prim_def(PRIMITIVE_USE, "use", returnInfoVoid, true);
  prim_def(PRIMITIVE_TUPLE_EXPAND, "expand_tuple", returnInfoVoid);
  prim_def(PRIMITIVE_TUPLE_AND_EXPAND, "and_expand_tuple", returnInfoVoid);

  prim_def(PRIMITIVE_ARRAY_INIT, "array_init", returnInfoVoid, true, true);
  prim_def(PRIMITIVE_ARRAY_FREE, "array_free", returnInfoVoid, true, true);
  prim_def(PRIMITIVE_ARRAY_FREE_ELTS, "array_free_elts", returnInfoVoid, true);
  prim_def(PRIMITIVE_ARRAY_GET, "array_get", returnInfoArrayIndex);
  prim_def(PRIMITIVE_ARRAY_GET_VALUE, "array_get_value", returnInfoArrayIndex);
  prim_def(PRIMITIVE_ARRAY_SET, "array_set", returnInfoVoid, true);
  prim_def(PRIMITIVE_ARRAY_SET_FIRST, "array_set_first", returnInfoVoid, true);

  prim_def(PRIMITIVE_ERROR, "error", returnInfoVoid, true);
  prim_def(PRIMITIVE_WHEN, "when case expressions", returnInfoVoid);

  prim_def(PRIMITIVE_LOOP_PARAM, "param loop", returnInfoVoid);
  prim_def(PRIMITIVE_LOOP_WHILEDO, "while...do loop", returnInfoVoid);
  prim_def(PRIMITIVE_LOOP_DOWHILE, "do...while loop", returnInfoVoid);
  prim_def(PRIMITIVE_LOOP_FOR, "for loop", returnInfoVoid);
  prim_def(PRIMITIVE_LOOP_C_FOR, "c for loop", returnInfoVoid);
  prim_def(PRIMITIVE_LOOP_INLINE, "inline loop", returnInfoVoid);

  prim_def(PRIMITIVE_GC_CC_INIT, "_chpl_gc_init", returnInfoVoid);
  prim_def(PRIMITIVE_GC_ADD_ROOT, "_addRoot", returnInfoVoid);
  prim_def(PRIMITIVE_GC_ADD_NULL_ROOT, "_addNullRoot", returnInfoVoid);
  prim_def(PRIMITIVE_GC_DELETE_ROOT, "_deleteRoot", returnInfoVoid);

  prim_def(PRIMITIVE_CLASS_NULL, "_refnull", returnInfoBool);
  prim_def(PRIMITIVE_GC_INIT, "_gc_init", returnInfoVoid);
  prim_def(PRIMITIVE_GC_TOUCH, "_refup", returnInfoVoid);
  prim_def(PRIMITIVE_GC_FREE, "_refdown", returnInfoVoid);
  prim_def(PRIMITIVE_GC_ISPOS, "_refgzero", returnInfoBool);
  prim_def(PRIMITIVE_GC_ISNEG, "_reflzero", returnInfoBool);

  prim_def(PRIMITIVE_IS_ENUM, "isEnumType", returnInfoBool);
  prim_def(PRIMITIVE_IS_TUPLE, "isTupleType", returnInfoBool);

  prim_def(PRIMITIVE_LOGICAL_FOLDER, "_paramFoldLogical", returnInfoBool);
  prim_def(PRIMITIVE_GET_IC_TYPE, "get_ic_type", returnInfoICType);

  prim_def(PRIMITIVE_ON, "on", returnInfoVoid, true);
  prim_def(PRIMITIVE_NUM_LOCALES, "_chpl_comm_default_num_locales", returnInfoInt32, false, false);
  prim_def(PRIMITIVE_LOCALE_ID, "_localeID", returnInfoInt32, false, false);
  prim_def(PRIMITIVE_COMM_GET, "_chpl_comm_get", returnInfoVoid, true);
  prim_def(PRIMITIVE_COMM_PUT, "_chpl_comm_put", returnInfoVoid, true);
  prim_def(PRIMITIVE_COMM_GET_OFF, "_chpl_comm_get_off", returnInfoVoid, true);
  prim_def(PRIMITIVE_COMM_PUT_OFF, "_chpl_comm_put_off", returnInfoVoid, true);

  prim_def(PRIMITIVE_INT_ERROR, "_internal_error", returnInfoVoid, true);

  prim_def("_config_has_value", returnInfoBool);
  prim_def("_config_get_value", returnInfoString);

  prim_def("acos", returnInfoReal64, true);
  prim_def("acosh", returnInfoReal64, true);
  prim_def("asin", returnInfoReal64, true);
  prim_def("asinh", returnInfoReal64, true);
  prim_def("atan", returnInfoReal64, true);
  prim_def("atan2", returnInfoReal64, true);  
  prim_def("atanh", returnInfoReal64, true);
  prim_def("cbrt", returnInfoReal64, true);
  prim_def("ceil", returnInfoReal64, true);
  prim_def("cos", returnInfoReal64, true);
  prim_def("cosh", returnInfoReal64, true);
  prim_def("exp", returnInfoReal64, true);
  prim_def("exp2", returnInfoReal64, true);
  prim_def("erf", returnInfoReal64, true);
  prim_def("erfc", returnInfoReal64, true);
  prim_def("expm1", returnInfoReal64, true);
  prim_def("fabs", returnInfoReal64, true);
  prim_def("floor", returnInfoReal64, true);
  prim_def("lgamma", returnInfoReal64, true);
  prim_def("log", returnInfoReal64, true);
  prim_def("log2", returnInfoReal64, true);
  prim_def("log10", returnInfoReal64, true);
  prim_def("log1p", returnInfoReal64, true);
  prim_def("nearbyint", returnInfoReal64, true);
  prim_def("rint", returnInfoReal64, true);
  prim_def("round", returnInfoReal64, true);
  prim_def("sin", returnInfoReal64, true);
  prim_def("sinh", returnInfoReal64, true);
  prim_def("sqrt", returnInfoReal64, true);
  prim_def("tan", returnInfoReal64, true);
  prim_def("tanh", returnInfoReal64, true);
  prim_def("tgamma", returnInfoReal64, true);
  prim_def("trunc", returnInfoReal64, true);
  prim_def("fopen", returnInfoFile, true);
  prim_def("fclose", returnInfoInt32, true);
  prim_def("fprintf", returnInfoInt32, true);
  prim_def("fflush", returnInfoInt32, true);
  prim_def("_fscan_literal", returnInfoBool, true, true);
  prim_def("_fscan_string", returnInfoString, true, true);
  prim_def("_fscan_int32", returnInfoInt32, true, true);
  prim_def("_fscan_uint32", returnInfoUInt32, true, true);
  prim_def("_fscan_real64", returnInfoReal64, true, true);
  prim_def("string_contains", returnInfoBool, true);
  prim_def("string_copy", returnInfoString, true);
  prim_def("string_index", returnInfoString, true);
  prim_def("string_concat", returnInfoString, true);
  prim_def("string_equal", returnInfoBool, true);
  prim_def("string_select", returnInfoString, true);
  prim_def("string_strided_select", returnInfoString, true);
  prim_def("string_length", returnInfoInt32);
  prim_def("ascii", returnInfoInt32);
  prim_def("sleep", returnInfoVoid, true);
  prim_def("real2int", returnInfoInt64);
  prim_def("object2int", returnInfoInt64);
  prim_def("_chpl_exit_any", returnInfoVoid, true);

  prim_def("complex_set_real", returnInfoVoid, true);
  prim_def("complex_set_imag", returnInfoVoid, true);

  prim_def("get_stdin", returnInfoFile);
  prim_def("get_stdout", returnInfoFile);
  prim_def("get_stderr", returnInfoFile);
  prim_def("get_nullfile", returnInfoFile);
  prim_def("get_errno", returnInfoString);
  prim_def("get_eof", returnInfoInt32);

  prim_def("_init_timer", returnInfoVoid, true);
  prim_def("_now_timer", returnInfoTimer, true);
  prim_def("_seconds_timer", returnInfoReal64, true);
  prim_def("_microseconds_timer", returnInfoReal64, true);
  prim_def("_now_year", returnInfoInt32, true);
  prim_def("_now_month", returnInfoInt32, true);
  prim_def("_now_day", returnInfoInt32, true);
  prim_def("_now_time", returnInfoReal64, true);

  prim_def("_bytesPerLocale", returnInfoUInt64);
  prim_def("_coresPerLocale", returnInfoInt32);

  prim_def("_memtest_printMemTable", returnInfoVoid, true, true);
  prim_def("_memtest_printMemStat", returnInfoVoid, true, true);
  prim_def("_memtest_resetMemStat", returnInfoVoid, true, true);
  prim_def("_memtest_allocAndFree", returnInfoVoid, true, true);
  prim_def("_memtest_freedMalloc", returnInfoVoid, true, true);
  prim_def("_memtest_freedWithoutMalloc", returnInfoVoid, true, true);
  prim_def("_memtest_reallocWithoutMalloc", returnInfoVoid, true, true);
  prim_def("_memtest_reallocZeroSize", returnInfoVoid, true, true);
  prim_def("_memtest_mallocOutOfMemory", returnInfoVoid, true, true);
  prim_def("_memtest_reallocOutOfMemory", returnInfoVoid, true, true);
  prim_def("startTrackingMem", returnInfoVoid, true);

  prim_def("_printError", returnInfoVoid, true, true);

  prim_def("_mem_used", returnInfoUInt64, false, true);

}
