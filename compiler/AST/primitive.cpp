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

static Type*
returnInfoComplexField(CallExpr* call) {  // for get real/imag primitives
  Type *t = call->get(1)->typeInfo();
  if (t->symbol->hasPragma("ref"))
    t = getValueType(t);
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
  Type *t = call->get(1)->typeInfo();
  if (t->symbol->hasPragma("ref"))
    t = getValueType(t);
  return t;
}

static Type*
returnIteratorType(CallExpr* call) {
  Type* ict = call->get(1)->typeInfo();
  INT_ASSERT(ict->symbol->hasPragma("iterator class"));
  return ict->defaultConstructor->getReturnSymbol()->type;
}

static Type*
returnInfoCast(CallExpr* call) {
  Type* t = call->get(1)->typeInfo();
  if (t->isGeneric) {
    // unexecuted none/gasnet on 4/25/08
    if (SymExpr* sym = toSymExpr(call->get(1)))
      if (sym->var->isTypeVariable)
        return dtUnknown;
    USR_FATAL(call, "Illegal cast to generic type");
  }
  Type* t2 = call->get(2)->typeInfo();
  if (t2->symbol->hasPragma("wide class"))
    if (wideClassMap.get(t))
      t = wideClassMap.get(t);
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
  // unexecuted none/gasnet on 4/25/08
  Type* t1 = call->get(1)->typeInfo();
  Type* t2 = call->get(2)->typeInfo();
  if (t1 == dtUnknown || t1 == dtNil)
    return t2;
  return t1;
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
  INT_ASSERT(sym);
  Type* type = sym->var->type;
  if (type->symbol->hasPragma("wide class"))
    type = type->getField("addr")->type;
  if (!type->substitutions.n)
    INT_FATAL(call, "bad primitive");
  return toType(type->substitutions.v[0].value)->refType;
}

static Type*
returnInfoChplAlloc(CallExpr* call) {
  SymExpr* sym = toSymExpr(call->get(1));
  INT_ASSERT(sym);
  Type* type = sym->var->type;
  if (type->symbol->hasPragma("wide class"))
    type = type->getField("addr")->type;
  return type;
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

static Type*
returnInfoEndCount(CallExpr* call) {
  static Type* endCountType = NULL;
  if (endCountType == NULL) {
    forv_Vec(TypeSymbol, ts, gTypes) {
      if (!strcmp(ts->name, "_EndCount")) {
        endCountType = ts->type;
        break;
      }
    }
  }
  return endCountType;
}

static Type*
returnInfoTaskList(CallExpr* call) {
  return dtTaskList;
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

  prim_def(PRIMITIVE_ACTUALS_LIST, "actuals list", returnInfoVoid);
  prim_def(PRIMITIVE_NOOP, "noop", returnInfoVoid);
  prim_def(PRIMITIVE_MOVE, "move", returnInfoMove);
  prim_def(PRIMITIVE_INIT, "init", returnInfoFirstDeref);
  prim_def(PRIMITIVE_SET_REF, "set ref", returnInfoRef);
  prim_def(PRIMITIVE_GET_REF, "get ref", returnInfoVal);
  prim_def(PRIMITIVE_REF2STR, "ref to string", returnInfoString);
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
  prim_def(PRIMITIVE_UNION_GETID, "get_union_id", returnInfoInt64);
  prim_def(PRIMITIVE_GET_MEMBER, ".", returnInfoGetMemberRef);
  prim_def(PRIMITIVE_GET_MEMBER_VALUE, ".v", returnInfoGetMember);
  prim_def(PRIMITIVE_SET_MEMBER, ".=", returnInfoVoid, true);
  prim_def(PRIMITIVE_CHECK_NIL, "_check_nil", returnInfoVoid, true, true);
  prim_def(PRIMITIVE_NEW, "new", returnInfoFirst);
  prim_def(PRIMITIVE_GET_REAL, "complex_get_real", returnInfoComplexField);
  prim_def(PRIMITIVE_GET_IMAG, "complex_get_imag", returnInfoComplexField);

  // local block primitives
  prim_def(PRIMITIVE_LOCAL_CHECK, "local_check", returnInfoVoid, true, true);
  prim_def(PRIMITIVE_LOCAL_DEREF, "local_deref", returnInfoVoid, true);

  // thread primitives
  prim_def(PRIMITIVE_THREAD_INIT, "thread_init", returnInfoVoid, true);
  prim_def(PRIMITIVE_THREAD_ID, "thread_id", returnInfoUInt64, true);  // 64-bit
  prim_def(PRIMITIVE_GET_SERIAL, "thread_get_serial", returnInfoBool);
  prim_def(PRIMITIVE_SET_SERIAL, "thread_set_serial", returnInfoVoid, true);

  // operations on sync/single vars
  prim_def(PRIMITIVE_SYNC_INIT, "init_sync_aux", returnInfoVoid, true);
  prim_def(PRIMITIVE_SYNC_LOCK, "sync_lock", returnInfoVoid, true);
  prim_def(PRIMITIVE_SYNC_UNLOCK, "sync_unlock", returnInfoVoid, true);
  prim_def(PRIMITIVE_SYNC_WAIT_FULL, "sync_wait_full_and_lock", returnInfoVoid, true, true);
  prim_def(PRIMITIVE_SYNC_WAIT_EMPTY, "sync_wait_empty_and_lock", returnInfoVoid, true, true);
  prim_def(PRIMITIVE_SYNC_SIGNAL_FULL, "sync_mark_and_signal_full", returnInfoVoid, true);
  prim_def(PRIMITIVE_SYNC_SIGNAL_EMPTY, "sync_mark_and_signal_empty", returnInfoVoid, true);
  prim_def(PRIMITIVE_SINGLE_INIT, "init_single_aux", returnInfoVoid, true);
  prim_def(PRIMITIVE_SINGLE_LOCK, "single_lock", returnInfoVoid, true);
  prim_def(PRIMITIVE_SINGLE_UNLOCK, "single_unlock", returnInfoVoid, true);
  prim_def(PRIMITIVE_SINGLE_WAIT_FULL, "single_wait_full", returnInfoVoid, true, true);
  prim_def(PRIMITIVE_SINGLE_SIGNAL_FULL, "single_mark_and_signal_full", returnInfoVoid, true);

  // sync/single var support
  prim_def(PRIMITIVE_WRITEEF, "write_EF", returnInfoVoid, true);
  prim_def(PRIMITIVE_WRITEFF, "write_FF", returnInfoVoid, true);
  prim_def(PRIMITIVE_WRITEXF, "write_XF", returnInfoVoid, true);
  prim_def(PRIMITIVE_SYNC_RESET, "sync_reset", returnInfoVoid, true);
  prim_def(PRIMITIVE_READFE, "read_FE", returnInfoFirst, true);
  prim_def(PRIMITIVE_READFF, "read_FF", returnInfoFirst, true);
  prim_def(PRIMITIVE_READXX, "read_XX", returnInfoFirst, true);
  prim_def(PRIMITIVE_SYNC_ISFULL, "sync_is_full", returnInfoBool, true);
  prim_def(PRIMITIVE_SINGLE_WRITEEF, "single_write_EF", returnInfoVoid, true);
  prim_def(PRIMITIVE_SINGLE_RESET, "single_reset", returnInfoVoid, true);
  prim_def(PRIMITIVE_SINGLE_READFF, "single_read_FF", returnInfoFirst, true);
  prim_def(PRIMITIVE_SINGLE_READXX, "single_read_XX", returnInfoFirst, true);
  prim_def(PRIMITIVE_SINGLE_ISFULL, "single_is_full", returnInfoBool, true);

  prim_def(PRIMITIVE_GET_END_COUNT, "get end count", returnInfoEndCount);
  prim_def(PRIMITIVE_SET_END_COUNT, "set end count", returnInfoVoid, true);

  prim_def(PRIMITIVE_INIT_TASK_LIST, "init to NULL", returnInfoTaskList);
  prim_def(PRIMITIVE_PROCESS_TASK_LIST, "process task list", returnInfoVoid, true);
  prim_def(PRIMITIVE_EXECUTE_TASKS_IN_LIST, "execute tasks in list", returnInfoVoid, true);
  prim_def(PRIMITIVE_FREE_TASK_LIST, "free task list", returnInfoVoid, true);

  prim_def(PRIMITIVE_CHPL_ALLOC, "chpl_alloc", returnInfoChplAlloc, true, true);
  prim_def(PRIMITIVE_CHPL_ALLOC_PERMIT_ZERO, "chpl_alloc_permit_zero",
           returnInfoChplAlloc, true, true);
  prim_def(PRIMITIVE_CHPL_FREE, "chpl_free", returnInfoVoid, true, true);
  prim_def(PRIMITIVE_INIT_FIELDS, "chpl_init_record", returnInfoVoid, true);
  prim_def(PRIMITIVE_PTR_EQUAL, "ptr_eq", returnInfoBool);
  prim_def(PRIMITIVE_PTR_NOTEQUAL, "ptr_neq", returnInfoBool);
  prim_def(PRIMITIVE_ISSUBTYPE, "is_subtype", returnInfoBool);
  prim_def(PRIMITIVE_CAST, "cast", returnInfoCast, false, true);
  prim_def(PRIMITIVE_DYNAMIC_CAST, "dynamic_cast", returnInfoCast);
  prim_def(PRIMITIVE_TYPEOF, "typeof", returnInfoFirstDeref);
  prim_def(PRIMITIVE_GET_ITERATOR_RETURN, "get iterator return", returnIteratorType);
  prim_def(PRIMITIVE_USE, "use", returnInfoVoid, true);
  prim_def(PRIMITIVE_TUPLE_EXPAND, "expand_tuple", returnInfoVoid);
  prim_def(PRIMITIVE_TUPLE_AND_EXPAND, "and_expand_tuple", returnInfoVoid);

  prim_def(PRIMITIVE_ARRAY_ALLOC, "array_alloc", returnInfoVoid, true, true);
  prim_def(PRIMITIVE_ARRAY_FREE, "array_free", returnInfoVoid, true, true);
  prim_def(PRIMITIVE_ARRAY_FREE_ELTS, "array_free_elts", returnInfoVoid, true);
  prim_def(PRIMITIVE_ARRAY_GET, "array_get", returnInfoArrayIndex);
  prim_def(PRIMITIVE_ARRAY_GET_VALUE, "array_get_value", returnInfoArrayIndex);
  // PRIMITIVE_ARRAY_SET is unused by compiler, runtime, modules
  prim_def(PRIMITIVE_ARRAY_SET, "array_set", returnInfoVoid, true);
  prim_def(PRIMITIVE_ARRAY_SET_FIRST, "array_set_first", returnInfoVoid, true);

  prim_def(PRIMITIVE_ERROR, "error", returnInfoVoid, true);
  prim_def(PRIMITIVE_WARNING, "warning", returnInfoVoid, true);
  prim_def(PRIMITIVE_WHEN, "when case expressions", returnInfoVoid);

  prim_def(PRIMITIVE_LOOP_PARAM, "param loop", returnInfoVoid);
  prim_def(PRIMITIVE_LOOP_WHILEDO, "while...do loop", returnInfoVoid);
  prim_def(PRIMITIVE_LOOP_DOWHILE, "do...while loop", returnInfoVoid);
  prim_def(PRIMITIVE_LOOP_FOR, "for loop", returnInfoVoid);
  prim_def(PRIMITIVE_LOOP_INLINE, "inline loop", returnInfoVoid);

  prim_def(PRIMITIVE_GC_CC_INIT, "_chpl_gc_init", returnInfoVoid);
  prim_def(PRIMITIVE_GC_ADD_ROOT, "_addRoot", returnInfoVoid);
  prim_def(PRIMITIVE_GC_ADD_NULL_ROOT, "_addNullRoot", returnInfoVoid);
  prim_def(PRIMITIVE_GC_DELETE_ROOT, "_deleteRoot", returnInfoVoid);
  prim_def(PRIMITIVE_GC_CLEANUP, "_chpl_gc_cleanup", returnInfoVoid);

  prim_def(PRIMITIVE_IS_ENUM, "isEnumType", returnInfoBool);
  prim_def(PRIMITIVE_IS_OPAQUE, "isOpaqueType", returnInfoBool);
  prim_def(PRIMITIVE_IS_TUPLE, "isTupleType", returnInfoBool);

  prim_def(PRIMITIVE_LOGICAL_FOLDER, "_paramFoldLogical", returnInfoBool);

  prim_def(PRIMITIVE_NUM_LOCALES, "_chpl_comm_default_num_locales", returnInfoInt32);
  prim_def(PRIMITIVE_GET_LOCALEID, "_get_locale", returnInfoInt32);
  prim_def(PRIMITIVE_LOCALE_ID, "_localeID", returnInfoInt32);
  prim_def(PRIMITIVE_ON_LOCALE_NUM, "chpl_on_locale_num", returnInfoInt32);

  prim_def(PRIMITIVE_ALLOC_GVR, "alloc_global_vars_registry", returnInfoVoid);
  prim_def(PRIMITIVE_HEAP_REGISTER_GLOBAL_VAR, "_heap_register_global_var", returnInfoVoid, true, true);
  prim_def(PRIMITIVE_HEAP_BROADCAST_GLOBAL_VARS, "_heap_broadcast_global_vars", returnInfoVoid, true, true);
  prim_def(PRIMITIVE_PRIVATE_BROADCAST, "_private_broadcast", returnInfoVoid, true, true);

  prim_def(PRIMITIVE_INT_ERROR, "_internal_error", returnInfoVoid, true);

  prim_def(PRIMITIVE_BUILD_ARRAY, "build_array", returnInfoFirst);

  prim_def("_config_has_value", returnInfoBool);
  prim_def("_config_get_value", returnInfoString);

  prim_def("isnan", returnInfoBool);
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

  prim_def("acosf", returnInfoReal32, true);
  prim_def("acoshf", returnInfoReal32, true);
  prim_def("asinf", returnInfoReal32, true);
  prim_def("asinhf", returnInfoReal32, true);
  prim_def("atanf", returnInfoReal32, true);
  prim_def("atan2f", returnInfoReal32, true);  
  prim_def("atanhf", returnInfoReal32, true);
  prim_def("cbrtf", returnInfoReal32, true);
  prim_def("ceilf", returnInfoReal32, true);
  prim_def("cosf", returnInfoReal32, true);
  prim_def("coshf", returnInfoReal32, true);
  prim_def("expf", returnInfoReal32, true);
  prim_def("exp2f", returnInfoReal32, true);
  prim_def("erff", returnInfoReal32, true);
  prim_def("erfcf", returnInfoReal32, true);
  prim_def("expm1f", returnInfoReal32, true);
  prim_def("fabsf", returnInfoReal32, true);
  prim_def("floorf", returnInfoReal32, true);
  prim_def("lgammaf", returnInfoReal32, true);
  prim_def("logf", returnInfoReal32, true);
  prim_def("log2f", returnInfoReal32, true);
  prim_def("log10f", returnInfoReal32, true);
  prim_def("log1pf", returnInfoReal32, true);
  prim_def("nearbyintf", returnInfoReal32, true);
  prim_def("rintf", returnInfoReal32, true);
  prim_def("roundf", returnInfoReal32, true);
  prim_def("sinf", returnInfoReal32, true);
  prim_def("sinhf", returnInfoReal32, true);
  prim_def("sqrtf", returnInfoReal32, true);
  prim_def("tanf", returnInfoReal32, true);
  prim_def("tanhf", returnInfoReal32, true);
  prim_def("tgammaf", returnInfoReal32, true);
  prim_def("truncf", returnInfoReal32, true);

  prim_def("fopen", returnInfoFile, true);
  prim_def("fclose", returnInfoInt32, true);
  prim_def("fprintf", returnInfoInt32, true);
  prim_def("fflush", returnInfoInt32, true);
  prim_def("_fscan_literal", returnInfoBool, true, true);
  prim_def("_fscan_string", returnInfoString, true, true);
  prim_def("_fscan_int32", returnInfoInt32, true, true);
  prim_def("_fscan_uint32", returnInfoUInt32, true, true);
  prim_def("_fscan_real64", returnInfoReal64, true, true);
  prim_def("_readToEndOfLine", returnInfoVoid, true);
  prim_def("_format", returnInfoString);
  prim_def("_string_compare", returnInfoInt32, true);
  prim_def("string_contains", returnInfoBool, true);
  prim_def("string_concat", returnInfoString, true, true);
  prim_def("string_length", returnInfoInt32);
  prim_def("ascii", returnInfoInt32);
  prim_def("string_index", returnInfoString, true, true);
  prim_def("string_copy", returnInfoString, true, true);
  prim_def("string_select", returnInfoString, true, true);
  prim_def("string_strided_select", returnInfoString, true, true);
  prim_def("_string_get_imag_part", returnInfoString, true);
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
  prim_def(PRIMITIVE_GET_ERRNO, "get_errno", returnInfoString);

  prim_def("_init_timer", returnInfoVoid, true);
  prim_def("_now_timer", returnInfoTimer, true);
  prim_def("_seconds_timer", returnInfoReal64, true);
  prim_def("_microseconds_timer", returnInfoReal64, true);
  prim_def("_now_year", returnInfoInt32, true);
  prim_def("_now_month", returnInfoInt32, true);
  prim_def("_now_day", returnInfoInt32, true);
  prim_def("_now_dow", returnInfoInt32, true);
  prim_def("_now_time", returnInfoReal64, true);

  prim_def("chpl_bytesPerLocale", returnInfoUInt64);
  prim_def("chpl_coresPerLocale", returnInfoInt32);
  prim_def("chpl_maxThreads", returnInfoInt32);
  prim_def("chpl_maxThreadsLimit", returnInfoInt32);

  prim_def("startTrackingMem", returnInfoVoid, true);

  prim_def(PRIMITIVE_RT_ERROR, "chpl_error", returnInfoVoid, true, true);
  prim_def(PRIMITIVE_RT_WARNING, "chpl_warning", returnInfoVoid, true, true);

  prim_def("mem_used", returnInfoUInt64, false, true);
}
