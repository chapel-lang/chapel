#include "expr.h"
#include "primitive.h"
#include "type.h"
#include "../passes/interpreter_ops.h"

static Type*
returnInfoBool(CallExpr* call) {
  return dtBool;
}

static Type*
returnInfoVoid(CallExpr* call) {
  return dtVoid;
}

static Type*
returnInfoString(CallExpr* call) {
  return dtString;
}

static Type*
returnInfoInt(CallExpr* call) {
  return dtInt[INT_TYPE_64];
}

static Type*
returnInfoFloat(CallExpr* call) {
  return dtFloat[FLOAT_TYPE_64];
}

static Type*
returnInfoFirst(CallExpr* call) {
  return call->get(1)->typeInfo();
}

static Type*
returnInfoCast(CallExpr* call) {
  Type* t = call->get(1)->typeInfo();
  if (MetaType* mt = dynamic_cast<MetaType*>(t))
    return mt->base;
  else
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

// NEEDS TO BE FINISHED WHEN PRIMITIVES ARE REDONE
static Type*
returnInfoNumericUp(CallExpr* call) {
  Type* t1 = call->get(1)->typeInfo();
  Type* t2 = call->get(2)->typeInfo();
  if (is_int_type(t1) && is_float_type(t2))
    return t2;
  if (is_float_type(t1) && is_int_type(t2))
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
  return dynamic_cast<MetaType*>(sym->var->type)->base;
}

static Type*
returnInfoGetMember(CallExpr* call) {
  SymExpr* sym = dynamic_cast<SymExpr*>(call->get(2));
  if (!sym)
    INT_FATAL(call, "bad member primitive");
  VarSymbol* var = dynamic_cast<VarSymbol*>(sym->var);
  if (!var || !var->immediate)
    INT_FATAL(call, "bad member primitive");
  char* name = var->immediate->v_string;
  SymExpr* sym1 = dynamic_cast<SymExpr*>(call->get(1));
  if (!sym1)
    INT_FATAL(call, "bad member primitive");
  ClassType* ct = dynamic_cast<ClassType*>(sym1->var->type);
  if (!ct)
    INT_FATAL(call, "bad member primitive");
  forv_Vec(Symbol, field, ct->fields) {
    if (!strcmp(field->name, name))
      return field->type;
  }
  INT_FATAL(call, "bad member primitive");
  return NULL;
}

HashMap<char *, StringHashFns, PrimitiveOp *> primitives_map;

PrimitiveOp* primitives[NUM_KNOWN_PRIMS];

PrimitiveOp::PrimitiveOp(PrimitiveTag atag, char *aname, InterpreterOp *aiop, Type *(*areturnInfo)(CallExpr*))
  : tag(atag), name(aname), interpreterOp(aiop), returnInfo(areturnInfo)
{
  primitives_map.put(name, this);
}

static void
prim_def(PrimitiveTag tag, char* name, InterpreterOp* iop, Type *(*returnInfo)(CallExpr*)) {
  primitives[tag] = new PrimitiveOp(tag, name, iop, returnInfo);
}

static void
prim_def(char* name, InterpreterOp* iop, Type *(*returnInfo)(CallExpr*)) {
  new PrimitiveOp(PRIMITIVE_UNKNOWN, name, iop, returnInfo);
}

void
initPrimitive() {
  primitives[PRIMITIVE_UNKNOWN] = NULL;

  prim_def(PRIMITIVE_MOVE, "move", unimplemented_interpreter_op, returnInfoMove);
  prim_def(PRIMITIVE_UNARY_MINUS, "u-", unary_minus_interpreter_op, returnInfoFirst);
  prim_def(PRIMITIVE_UNARY_PLUS, "u+", unary_plus_interpreter_op, returnInfoFirst);
  prim_def(PRIMITIVE_UNARY_NOT, "u~", unary_not_interpreter_op, returnInfoFirst);
  prim_def(PRIMITIVE_UNARY_LNOT, "!", unary_lnot_interpreter_op, returnInfoBool);
  prim_def(PRIMITIVE_ADD, "+", add_interpreter_op, returnInfoNumericUp);
  prim_def(PRIMITIVE_SUBTRACT, "-", subtract_interpreter_op, returnInfoNumericUp);
  prim_def(PRIMITIVE_MULT, "*", mult_interpreter_op, returnInfoNumericUp);
  prim_def(PRIMITIVE_DIV, "/", div_interpreter_op, returnInfoNumericUp);
  prim_def(PRIMITIVE_MOD, "%", mod_interpreter_op, returnInfoFirst);
  prim_def(PRIMITIVE_LSH, "<<", lsh_interpreter_op, returnInfoFirst);
  prim_def(PRIMITIVE_RSH, ">>", rsh_interpreter_op, returnInfoFirst);
  prim_def(PRIMITIVE_EQUAL, "==", equal_interpreter_op, returnInfoBool);
  prim_def(PRIMITIVE_NOTEQUAL, "!=", notequal_interpreter_op, returnInfoBool);
  prim_def(PRIMITIVE_LESSOREQUAL, "<=", elssorequal_interpreter_op, returnInfoBool);
  prim_def(PRIMITIVE_GREATEROREQUAL, ">=", greaterorequal_interpreter_op, returnInfoBool);
  prim_def(PRIMITIVE_LESS, "<", less_interpreter_op, returnInfoBool);
  prim_def(PRIMITIVE_GREATER, ">", greater_interpreter_op, returnInfoBool);
  prim_def(PRIMITIVE_AND, "&", and_interpreter_op, returnInfoFirst);
  prim_def(PRIMITIVE_OR, "|", or_interpreter_op, returnInfoFirst);
  prim_def(PRIMITIVE_XOR, "^", xor_interpreter_op, returnInfoFirst);
  prim_def(PRIMITIVE_LAND, "&&", land_interpreter_op, returnInfoBool);
  prim_def(PRIMITIVE_LOR, "||", lor_interpreter_op, returnInfoBool);
  prim_def(PRIMITIVE_POW, "**", pow_interpreter_op, returnInfoNumericUp);
  prim_def(PRIMITIVE_GET_MEMBER, ".", get_member_interpreter_op, returnInfoGetMember);
  prim_def(PRIMITIVE_SET_MEMBER, ".=", set_member_interpreter_op, returnInfoVoid);
  prim_def(PRIMITIVE_GET_MEMBER_REF_TO, ".*", unimplemented_interpreter_op, returnInfoVoid);
  prim_def(PRIMITIVE_SET_MEMBER_REF_TO, ".=&", unimplemented_interpreter_op, returnInfoVoid);
  prim_def(PRIMITIVE_SET_HEAPVAR, "setheapvar", unimplemented_interpreter_op, returnInfoMove);
  prim_def(PRIMITIVE_REFC_INIT, "refc_init", unimplemented_interpreter_op, returnInfoVoid);
  prim_def(PRIMITIVE_REFC_TOUCH, "refc_touch", unimplemented_interpreter_op, returnInfoVoid);
  prim_def(PRIMITIVE_REFC_RELEASE, "refc_release", unimplemented_interpreter_op, returnInfoVoid);
  prim_def(PRIMITIVE_CHPL_ALLOC, "chpl_alloc", alloc_interpreter_op, returnInfoChplAlloc);
  prim_def(PRIMITIVE_CHPL_FREE, "chpl_free", unimplemented_interpreter_op, returnInfoVoid);
  prim_def(PRIMITIVE_PTR_EQUAL, "ptr_eq", ptr_eq_interpreter_op, returnInfoBool);
  prim_def(PRIMITIVE_PTR_NOTEQUAL, "ptr_neq", ptr_neq_interpreter_op, returnInfoBool);
  prim_def(PRIMITIVE_CAST, "cast", cast_interpreter_op, returnInfoCast);
  prim_def(PRIMITIVE_TYPEOF, "typeof", unimplemented_interpreter_op, returnInfoFirst);
  prim_def(PRIMITIVE_USE, "use", unimplemented_interpreter_op, returnInfoVoid);
  prim_def(PRIMITIVE_TUPLE_EXPAND, "expand_tuple", unimplemented_interpreter_op, returnInfoVoid);
  prim_def("abs", abs_interpreter_op, returnInfoInt);
  prim_def("acos", acos_interpreter_op, returnInfoFloat);
  prim_def("acosh", acosh_interpreter_op, returnInfoFloat);
  prim_def("asin", asin_interpreter_op, returnInfoFloat);
  prim_def("asinh", asinh_interpreter_op, returnInfoFloat);
  prim_def("atan", atan_interpreter_op, returnInfoFloat);
  prim_def("atan2", atan2_interpreter_op, returnInfoFloat);  
  prim_def("atanh", atanh_interpreter_op, returnInfoFloat);
  prim_def("cbrt", cbrt_interpreter_op, returnInfoFloat);
  prim_def("ceil", ceil_interpreter_op, returnInfoFloat);
  prim_def("cos", cos_interpreter_op, returnInfoFloat);
  prim_def("cosh", cosh_interpreter_op, returnInfoFloat);
  prim_def("exp", exp_interpreter_op, returnInfoFloat);
  prim_def("exp2", exp2_interpreter_op, returnInfoFloat);
  prim_def("erf", erf_interpreter_op, returnInfoFloat);
  prim_def("erfc", erfc_interpreter_op, returnInfoFloat);
  prim_def("expm1", expm1_interpreter_op, returnInfoFloat);
  prim_def("fabs", fabs_interpreter_op, returnInfoFloat);
  prim_def("floor", floor_interpreter_op, returnInfoFloat);
  prim_def("lgamma", lgamma_interpreter_op, returnInfoFloat);
  prim_def("log", log_interpreter_op, returnInfoFloat);
  prim_def("log2", log2_interpreter_op, returnInfoFloat);
  prim_def("log10", log10_interpreter_op, returnInfoFloat);
  prim_def("log1p", log1p_interpreter_op, returnInfoFloat);
  prim_def("nearbyint", nearbyint_interpreter_op, returnInfoFloat);
  prim_def("rint", rint_interpreter_op, returnInfoFloat);
  prim_def("round", round_interpreter_op, returnInfoFloat);
  prim_def("sin", sin_interpreter_op, returnInfoFloat);
  prim_def("sinh", sinh_interpreter_op, returnInfoFloat);
  prim_def("sqrt", sqrt_interpreter_op, returnInfoFloat);
  prim_def("tan", tan_interpreter_op, returnInfoFloat);
  prim_def("tanh", tanh_interpreter_op, returnInfoFloat);
  prim_def("tgamma", tgamma_interpreter_op, returnInfoFloat);
  prim_def("trunc", trunc_interpreter_op, returnInfoFloat);
  prim_def("fopen", fopen_interpreter_op, returnInfoInt);
  prim_def("fclose", fclose_interpreter_op, returnInfoInt);
  prim_def("strerror", strerror_interpreter_op, returnInfoString);
  prim_def("fprintf", fprintf_interpreter_op, returnInfoInt);
  prim_def("fscanf", fscanf_interpreter_op, returnInfoInt);
  prim_def("fflush", fflush_interpreter_op, returnInfoInt);
  prim_def("array_init", array_init_interpreter_op, returnInfoVoid);
  prim_def("array_index", array_index_interpreter_op, returnInfoArrayIndex);
  prim_def("array_set", array_set_interpreter_op, returnInfoVoid);
  prim_def("to_string", to_string_interpreter_op, returnInfoString);
  prim_def("string_copy", copy_string_interpreter_op, returnInfoString);
  prim_def("string_index", string_index_interpreter_op, returnInfoString);
  prim_def("string_concat", string_concat_interpreter_op, returnInfoString);
  prim_def("string_equal", string_equal_interpreter_op, returnInfoBool);
  prim_def("string_select", string_select_interpreter_op, returnInfoString);
  prim_def("string_strided_select", string_strided_select_interpreter_op, returnInfoString);
  prim_def("string_length", string_length_interpreter_op, returnInfoInt);
  prim_def("ascii", unimplemented_interpreter_op, returnInfoInt);
  prim_def("exit", done_interpreter_op, returnInfoInt);
}
