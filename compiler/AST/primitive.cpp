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
returnInfoUInt64(CallExpr* call) {
  return dtUInt[INT_SIZE_64];
}

static Type*
returnInfoFloat(CallExpr* call) {
  return dtReal[FLOAT_SIZE_64];
}

static Type*
returnInfoComplexField(CallExpr* call) {  // for get real/imag primitives
  Type *t = call->get(1)->typeInfo();
  if (t == dtComplex[COMPLEX_SIZE_64]) {
    return dtReal[FLOAT_SIZE_32];
  } else if (t == dtComplex[COMPLEX_SIZE_128]) {
    return dtReal[FLOAT_SIZE_64];
  } else if (t == dtComplex[COMPLEX_SIZE_256]) {
    return dtReal[FLOAT_SIZE_128];
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
    if (SymExpr* sym = dynamic_cast<SymExpr*>(call->get(1)))
      if (sym->var->isTypeVariable)
        return dtUnknown;
    USR_FATAL(call, "Illegal cast to generic type");
  }
  return t;
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
  ClassType* ct = dynamic_cast<ClassType*>(call->get(1)->typeInfo());
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
  SymExpr* sym = dynamic_cast<SymExpr*>(call->get(1));
  if (!sym || !sym->var->type->substitutions.n)
    INT_FATAL(call, "bad primitive");
  return dynamic_cast<Type*>(sym->var->type->substitutions.v[0].value);
}

static Type*
returnInfoChplAlloc(CallExpr* call) {
  SymExpr* sym = dynamic_cast<SymExpr*>(call->get(1));
  if (!sym)
    INT_FATAL(call, "bad _chpl_alloc primitive");
  return sym->var->type;
}

static Type*
returnInfoGetMember(CallExpr* call) {
  SymExpr* sym1 = dynamic_cast<SymExpr*>(call->get(1));
  if (!sym1)
    INT_FATAL(call, "bad member primitive");
  ClassType* ct = dynamic_cast<ClassType*>(sym1->var->type);
  if (!ct)
    INT_FATAL(call, "bad member primitive");
  SymExpr* sym = dynamic_cast<SymExpr*>(call->get(2));
  if (!sym)
    INT_FATAL(call, "bad member primitive");
  VarSymbol* var = dynamic_cast<VarSymbol*>(sym->var);
  if (!var)
    INT_FATAL(call, "bad member primitive");
  if (var->immediate) {
    char* name = var->immediate->v_string;
    for_fields(field, ct) {
      if (!strcmp(field->name, name))
        return field->type;
    }
  } else
    return var->type;
  INT_FATAL(call, "bad member primitive");
  return NULL;
}

HashMap<char *, StringHashFns, PrimitiveOp *> primitives_map;

PrimitiveOp* primitives[NUM_KNOWN_PRIMS];

PrimitiveOp::PrimitiveOp(PrimitiveTag atag,
                         char *aname,
                         Type *(*areturnInfo)(CallExpr*)) :
  tag(atag),
  name(aname),
  returnInfo(areturnInfo),
  isReference(false),
  passLineno(false)
{
  primitives_map.put(name, this);
}

static void
prim_def(PrimitiveTag tag, char* name, Type *(*returnInfo)(CallExpr*),
         bool passLineno = false, bool isReference = false) {
  primitives[tag] = new PrimitiveOp(tag, name, returnInfo);
  primitives[tag]->isReference = isReference;
  primitives[tag]->passLineno = passLineno;
}

static void
prim_def(char* name, Type *(*returnInfo)(CallExpr*),
         bool passLineno = false, bool isReference = false) {
  PrimitiveOp* prim = new PrimitiveOp(PRIMITIVE_UNKNOWN, name, returnInfo);
  prim->isReference = isReference;
  prim->passLineno = passLineno;
}


void
initPrimitive() {
  primitives[PRIMITIVE_UNKNOWN] = NULL;

  prim_def(PRIMITIVE_NOOP, "noop", returnInfoVoid);
  prim_def(PRIMITIVE_MOVE, "move", returnInfoMove);
  prim_def(PRIMITIVE_REF, "ref", returnInfoMove);
  prim_def(PRIMITIVE_RETURN, "return", returnInfoFirst);
  prim_def(PRIMITIVE_YIELD, "yield", returnInfoFirst);
  prim_def(PRIMITIVE_UNARY_MINUS, "u-", returnInfoFirst);
  prim_def(PRIMITIVE_UNARY_PLUS, "u+", returnInfoFirst);
  prim_def(PRIMITIVE_UNARY_NOT, "u~", returnInfoFirst);
  prim_def(PRIMITIVE_UNARY_LNOT, "!", returnInfoBool);
  prim_def(PRIMITIVE_ADD, "+", returnInfoNumericUp);
  prim_def(PRIMITIVE_SUBTRACT, "-", returnInfoNumericUp);
  prim_def(PRIMITIVE_MULT, "*", returnInfoNumericUp);
  prim_def(PRIMITIVE_DIV, "/", returnInfoNumericUp);
  prim_def(PRIMITIVE_MOD, "%", returnInfoFirst);
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

  prim_def(PRIMITIVE_SETCID, "setcid", returnInfoVoid);
  prim_def(PRIMITIVE_GETCID, "getcid", returnInfoBool);
  prim_def(PRIMITIVE_UNION_SETID, "set_union_id", returnInfoVoid);
  prim_def(PRIMITIVE_UNION_GETID, "get_union_id", returnInfoBool);
  prim_def(PRIMITIVE_GET_MEMBER, ".", returnInfoGetMember, false, true);
  prim_def(PRIMITIVE_GET_MEMBER_VALUE, ".v", returnInfoGetMember);
  prim_def(PRIMITIVE_SET_MEMBER, ".=", returnInfoVoid);

  prim_def(PRIMITIVE_GET_MEMBER_REF_TO, ".*", returnInfoVoid);
  prim_def(PRIMITIVE_SET_MEMBER_REF_TO, ".=&", returnInfoVoid);
  prim_def(PRIMITIVE_SET_HEAPVAR, "setheapvar", returnInfoMove);
  prim_def(PRIMITIVE_REFC_INIT, "refc_init", returnInfoVoid);
  prim_def(PRIMITIVE_REFC_TOUCH, "refc_touch", returnInfoVoid);
  prim_def(PRIMITIVE_REFC_RELEASE, "refc_release", returnInfoVoid, true);

  // thread primitives
  prim_def(PRIMITIVE_THREAD_INIT, "thread_init", returnInfoVoid);
  prim_def(PRIMITIVE_THREAD_ID, "thread_id", returnInfoUInt64);  // 64-bit
  prim_def(PRIMITIVE_GET_SERIAL, "thread_get_serial", returnInfoBool);
  prim_def(PRIMITIVE_SET_SERIAL, "thread_set_serial", returnInfoVoid);

  // operations on sync/single vars
  prim_def(PRIMITIVE_SYNC_LOCK, "sync_lock", returnInfoVoid);
  prim_def(PRIMITIVE_SYNC_UNLOCK, "sync_unlock", returnInfoVoid);
  prim_def(PRIMITIVE_SYNC_SIGNAL_FULL, "sync_signal_full", returnInfoVoid);
  prim_def(PRIMITIVE_SYNC_BROADCAST_FULL,"sync_broadcast_full",returnInfoVoid);
  prim_def(PRIMITIVE_SYNC_WAIT_FULL, "sync_wait_full", returnInfoVoid);
  prim_def(PRIMITIVE_SYNC_SIGNAL_EMPTY, "sync_signal_empty", returnInfoVoid);
  prim_def(PRIMITIVE_SYNC_BROADCAST_EMPTY,"sync_broadcast_empty",returnInfoVoid);
  prim_def(PRIMITIVE_SYNC_WAIT_EMPTY, "sync_wait_empty", returnInfoVoid);
  // sync/single var support
  prim_def(PRIMITIVE_MUTEX_NEW, "mutex_new", returnInfoMutexP);
  prim_def(PRIMITIVE_MUTEX_DESTROY, "mutex_destroy", returnInfoVoid);
  prim_def(PRIMITIVE_CONDVAR_NEW, "condvar_new", returnInfoCondVarP);
  prim_def(PRIMITIVE_CONDVAR_DESTROY, "condvar_destroy", returnInfoVoid);

  prim_def(PRIMITIVE_CHPL_ALLOC, "chpl_alloc", returnInfoChplAlloc, true);
  prim_def(PRIMITIVE_CHPL_FREE, "chpl_free", returnInfoVoid, true);
  prim_def(PRIMITIVE_PTR_EQUAL, "ptr_eq", returnInfoBool);
  prim_def(PRIMITIVE_PTR_NOTEQUAL, "ptr_neq", returnInfoBool);
  prim_def(PRIMITIVE_ISSUBTYPE, "is_subtype", returnInfoBool);
  prim_def(PRIMITIVE_CAST, "cast", returnInfoCast);
  prim_def(PRIMITIVE_DYNAMIC_CAST, "dynamic_cast", returnInfoCast);
  prim_def(PRIMITIVE_TYPEOF, "typeof", returnInfoFirst);
  prim_def(PRIMITIVE_USE, "use", returnInfoVoid);
  prim_def(PRIMITIVE_TUPLE_EXPAND, "expand_tuple", returnInfoVoid);
  prim_def(PRIMITIVE_TUPLE_AND_EXPAND, "and_expand_tuple", returnInfoVoid);

  prim_def(PRIMITIVE_ARRAY_INIT, "array_init", returnInfoVoid, true);
  prim_def(PRIMITIVE_ARRAY_FREE, "array_free", returnInfoVoid, true);
  prim_def(PRIMITIVE_ARRAY_FREE_ELTS, "array_free_elts", returnInfoVoid);
  prim_def(PRIMITIVE_ARRAY_GET, "array_get", returnInfoArrayIndex, false, true);
  prim_def(PRIMITIVE_ARRAY_GET_VALUE, "array_get_value", returnInfoArrayIndex, false, true);
  prim_def(PRIMITIVE_ARRAY_SET, "array_set", returnInfoVoid);
  prim_def(PRIMITIVE_ARRAY_SET_FIRST, "array_set_first", returnInfoVoid);

  prim_def(PRIMITIVE_ERROR, "error", returnInfoVoid);
  prim_def(PRIMITIVE_WHEN, "when case expressions", returnInfoVoid);

  prim_def(PRIMITIVE_LOOP_PARAM, "param loop", returnInfoVoid);
  prim_def(PRIMITIVE_LOOP_WHILEDO, "while...do loop", returnInfoVoid);
  prim_def(PRIMITIVE_LOOP_DOWHILE, "do...while loop", returnInfoVoid);
  prim_def(PRIMITIVE_LOOP_FOR, "for loop", returnInfoVoid);

  prim_def(PRIMITIVE_CLASS_NULL, "_refnull", returnInfoBool);
  prim_def(PRIMITIVE_GC_INIT, "_gc_init", returnInfoVoid);
  prim_def(PRIMITIVE_GC_TOUCH, "_refup", returnInfoVoid);
  prim_def(PRIMITIVE_GC_FREE, "_refdown", returnInfoVoid);
  prim_def(PRIMITIVE_GC_ISPOS, "_refgzero", returnInfoBool);
  prim_def(PRIMITIVE_GC_ISNEG, "_reflzero", returnInfoBool);

  prim_def(PRIMITIVE_IS_ENUM, "isEnumType", returnInfoBool);

  prim_def(PRIMITIVE_GET_IC_TYPE, "get_ic_type", returnInfoICType);

  prim_def(PRIMITIVE_INT_ERROR, "_internal_error", returnInfoVoid);

  prim_def("_config_has_value", returnInfoBool);
  prim_def("_config_get_value", returnInfoString);

  prim_def("acos", returnInfoFloat);
  prim_def("acosh", returnInfoFloat);
  prim_def("asin", returnInfoFloat);
  prim_def("asinh", returnInfoFloat);
  prim_def("atan", returnInfoFloat);
  prim_def("atan2", returnInfoFloat);  
  prim_def("atanh", returnInfoFloat);
  prim_def("cbrt", returnInfoFloat);
  prim_def("ceil", returnInfoFloat);
  prim_def("cos", returnInfoFloat);
  prim_def("cosh", returnInfoFloat);
  prim_def("exp", returnInfoFloat);
  prim_def("exp2", returnInfoFloat);
  prim_def("erf", returnInfoFloat);
  prim_def("erfc", returnInfoFloat);
  prim_def("expm1", returnInfoFloat);
  prim_def("fabs", returnInfoFloat);
  prim_def("floor", returnInfoFloat);
  prim_def("lgamma", returnInfoFloat);
  prim_def("log", returnInfoFloat);
  prim_def("log2", returnInfoFloat);
  prim_def("log10", returnInfoFloat);
  prim_def("log1p", returnInfoFloat);
  prim_def("nearbyint", returnInfoFloat);
  prim_def("rint", returnInfoFloat);
  prim_def("round", returnInfoFloat);
  prim_def("sin", returnInfoFloat);
  prim_def("sinh", returnInfoFloat);
  prim_def("sqrt", returnInfoFloat);
  prim_def("tan", returnInfoFloat);
  prim_def("tanh", returnInfoFloat);
  prim_def("tgamma", returnInfoFloat);
  prim_def("trunc", returnInfoFloat);
  prim_def("fopen", returnInfoFile);
  prim_def("fclose", returnInfoInt32);
  prim_def("fprintf", returnInfoInt32);
  prim_def("fscanf", returnInfoInt32);
  prim_def("fflush", returnInfoInt32);
  prim_def("readLit", returnInfoBool);
  prim_def("string_fscanf", returnInfoString, true);
  prim_def("string_contains", returnInfoBool);
  prim_def("string_copy", returnInfoString);
  prim_def("string_index", returnInfoString);
  prim_def("string_concat", returnInfoString);
  prim_def("string_equal", returnInfoBool);
  prim_def("string_select", returnInfoString);
  prim_def("string_strided_select", returnInfoString);
  prim_def("string_length", returnInfoInt32);
  prim_def("ascii", returnInfoInt32);
  prim_def("sleep", returnInfoVoid);
  prim_def("real2int", returnInfoInt64);
  prim_def("object2int", returnInfoInt64);
  prim_def("_chpl_exit", returnInfoVoid);

  prim_def("complex_get_real", returnInfoComplexField, false, true);
  prim_def("complex_get_imag", returnInfoComplexField, false, true);
  prim_def("complex_set_real", returnInfoVoid);
  prim_def("complex_set_imag", returnInfoVoid);

  prim_def("get_stdin", returnInfoFile);
  prim_def("get_stdout", returnInfoFile);
  prim_def("get_stderr", returnInfoFile);
  prim_def("get_nullfile", returnInfoFile);
  prim_def("get_errno", returnInfoString);
  prim_def("get_eof", returnInfoInt32);

  prim_def("_init_timer", returnInfoVoid);
  prim_def("_now_timer", returnInfoTimer);
  prim_def("_seconds_timer", returnInfoFloat);
  prim_def("_microseconds_timer", returnInfoFloat);
  prim_def("_now_year", returnInfoInt32);
  prim_def("_now_month", returnInfoInt32);
  prim_def("_now_day", returnInfoInt32);
  prim_def("_now_time", returnInfoFloat);

  prim_def("_bytesPerLocale", returnInfoUInt64);

  prim_def("_chpl_memtest_printMemTable", returnInfoVoid, true);
  prim_def("_chpl_memtest_printMemStat", returnInfoVoid, true);
  prim_def("_chpl_memtest_resetMemStat", returnInfoVoid, true);
  prim_def("_chpl_memtest_allocAndFree", returnInfoVoid, true);
  prim_def("_chpl_memtest_freedMalloc", returnInfoVoid, true);
  prim_def("_chpl_memtest_freedWithoutMalloc", returnInfoVoid, true);
  prim_def("_chpl_memtest_reallocWithoutMalloc", returnInfoVoid, true);
  prim_def("_chpl_memtest_reallocZeroSize", returnInfoVoid, true);
  prim_def("_chpl_memtest_mallocOutOfMemory", returnInfoVoid, true);
  prim_def("_chpl_memtest_reallocOutOfMemory", returnInfoVoid, true);
  prim_def("startTrackingMem", returnInfoVoid);

  prim_def("_printError", returnInfoVoid, true);

  prim_def("_mem_used", returnInfoUInt64, true);
}
