#include "primitive.h"
#include "../analysis/analysis_ops.h"
#include "../passes/interpreter_ops.h"

HashMap<char *, StringHashFns, PrimitiveOp *> primitives_map;

PrimitiveOp* primitives[NUM_KNOWN_PRIMS];

PrimitiveOp::PrimitiveOp(PrimitiveTag atag, char *aname, InterpreterOp *aiop, AnalysisOp *aaop)
  : tag(atag), name(aname), interpreterOp(aiop), analysisOp(aaop)
{
  primitives_map.put(name, this);
}

static void
prim_def(PrimitiveTag tag, char* name, InterpreterOp* iop, AnalysisOp* aop) {
  primitives[tag] = new PrimitiveOp(tag, name, iop, aop);
}

static void
prim_def(char* name, InterpreterOp* iop, AnalysisOp* aop) {
  new PrimitiveOp(PRIMITIVE_UNKNOWN, name, iop, aop);
}

void
initPrimitive() {
  primitives[PRIMITIVE_UNKNOWN] = NULL;

  prim_def(PRIMITIVE_MOVE, "move", unimplemented_interpreter_op, unimplemented_analysis_op);
  prim_def(PRIMITIVE_INIT, "init", init_interpreter_op, init_analysis_op);
  prim_def(PRIMITIVE_UNARY_MINUS, "u-", unary_minus_interpreter_op, unary_minus_analysis_op);
  prim_def(PRIMITIVE_UNARY_PLUS, "u+", unary_plus_interpreter_op, unary_plus_analysis_op);
  prim_def(PRIMITIVE_UNARY_NOT, "u~", unary_not_interpreter_op, unary_not_analysis_op);
  prim_def(PRIMITIVE_UNARY_LNOT, "!", unary_lnot_interpreter_op, unary_lnot_analysis_op);
  prim_def(PRIMITIVE_ADD, "+", add_interpreter_op, add_analysis_op);
  prim_def(PRIMITIVE_SUBTRACT, "-", subtract_interpreter_op, subtract_analysis_op);
  prim_def(PRIMITIVE_MULT, "*", mult_interpreter_op, mult_analysis_op);
  prim_def(PRIMITIVE_DIV, "/", div_interpreter_op, div_analysis_op);
  prim_def(PRIMITIVE_MOD, "%", mod_interpreter_op, mod_analysis_op);
  prim_def(PRIMITIVE_LSH, "<<", lsh_interpreter_op, lsh_analysis_op);
  prim_def(PRIMITIVE_RSH, ">>", rsh_interpreter_op, rsh_analysis_op);
  prim_def(PRIMITIVE_EQUAL, "==", equal_interpreter_op, equal_analysis_op);
  prim_def(PRIMITIVE_NOTEQUAL, "!=", notequal_interpreter_op, notequal_analysis_op);
  prim_def(PRIMITIVE_LESSOREQUAL, "<=", elssorequal_interpreter_op, lessorequal_analysis_op);
  prim_def(PRIMITIVE_GREATEROREQUAL, ">=", greaterorequal_interpreter_op, greaterorequal_analysis_op);
  prim_def(PRIMITIVE_LESS, "<", less_interpreter_op, less_analysis_op);
  prim_def(PRIMITIVE_GREATER, ">", greater_interpreter_op, greater_analysis_op);
  prim_def(PRIMITIVE_AND, "&", and_interpreter_op, and_analysis_op);
  prim_def(PRIMITIVE_OR, "|", or_interpreter_op, or_analysis_op);
  prim_def(PRIMITIVE_XOR, "^", xor_interpreter_op, xor_analysis_op);
  prim_def(PRIMITIVE_LAND, "&&", land_interpreter_op, land_analysis_op);
  prim_def(PRIMITIVE_LOR, "||", lor_interpreter_op, lor_analysis_op);
  prim_def(PRIMITIVE_EXP, "**", exp_interpreter_op, exp_analysis_op);
  prim_def(PRIMITIVE_GET_MEMBER, ".", get_member_interpreter_op, get_member_analysis_op);
  prim_def(PRIMITIVE_SET_MEMBER, ".=", set_member_interpreter_op, set_member_analysis_op);
  prim_def(PRIMITIVE_TYPE_EQUAL, "type_equal", type_equal_interpreter_op, type_equal_analysis_op);
  prim_def(PRIMITIVE_CHPL_ALLOC, "chpl_alloc", alloc_interpreter_op, alloc_analysis_op);
  prim_def(PRIMITIVE_PTR_EQUAL, "ptr_eq", ptr_eq_interpreter_op, return_bool_analysis_op);
  prim_def(PRIMITIVE_PTR_NOTEQUAL, "ptr_neq", ptr_neq_interpreter_op, return_bool_analysis_op);

  prim_def("fopen", fopen_interpreter_op, return_int_analysis_op);
  prim_def("fclose", fclose_interpreter_op, return_int_analysis_op);
  prim_def("strerror", strerror_interpreter_op, return_string_analysis_op);
  prim_def("fprintf", fprintf_interpreter_op, return_int_analysis_op);
  prim_def("fscanf", fscanf_interpreter_op, return_int_analysis_op);
  prim_def("array_init", array_init_interpreter_op, array_init_analysis_op);
  prim_def("array_index", array_index_interpreter_op, array_index_analysis_op);
  prim_def("array_set", array_set_interpreter_op, array_set_analysis_op);
  prim_def("cast", cast_interpreter_op, cast_analysis_op);
  prim_def("to_string", to_string_interpreter_op, return_string_analysis_op);
  prim_def("copy_string", copy_string_interpreter_op, return_string_analysis_op);
  prim_def("string_index", string_index_interpreter_op, return_string_analysis_op);
  prim_def("string_concat", string_concat_interpreter_op, return_string_analysis_op);
  prim_def("string_equal", string_equal_interpreter_op, return_bool_analysis_op);
  prim_def("string_select", string_select_interpreter_op, return_string_analysis_op);
  prim_def("string_strided_select", string_strided_select_interpreter_op, return_string_analysis_op);
  prim_def("string_length", string_length_interpreter_op, return_int_analysis_op);
  prim_def("exit", done_interpreter_op, return_int_analysis_op);
  prim_def("halt", done_interpreter_op, return_int_analysis_op);
  prim_def("assert", done_interpreter_op, return_int_analysis_op);

  // TEMPORARY
  prim_def("array_pointwise_op", unimplemented_interpreter_op, array_pointwise_op_analysis_op);
}
