#include "primitive.h"
#include "../analysis/analysis_ops.h"
#include "../passes/interpreter_ops.h"

Vec<PrimitiveOp *> primitives;
HashMap<char *, StringHashFns, PrimitiveOp *> primitives_map;

PrimitiveOp* prim_move;

PrimitiveOp::PrimitiveOp(PrimitiveKind akind, char *aname, InterpreterOp *aiop, AnalysisOp *aaop)
  : kind(akind), name(aname), interpreterOp(aiop), analysisOp(aaop)
{
  primitives.add(this);
  primitives_map.put(name, this);
}

void 
initPrimitive() {
  primitives.add(prim_move = new PrimitiveOp(PRIMITIVE_UNKNOWN, "move", unimplemented_interpreter_op, unimplemented_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_INIT, "init", init_interpreter_op, init_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_SIZEOF, "sizeof", unimplemented_interpreter_op, unimplemented_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_FOPEN, "fopen", fopen_interpreter_op, return_int_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_FCLOSE, "fclose", fclose_interpreter_op, return_int_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_STRERROR, "strerror", strerror_interpreter_op, return_string_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_FPRINTF, "fprintf", fprintf_interpreter_op, return_int_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_FSCANF, "fscanf", fscanf_interpreter_op, return_int_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_ARRAY_INDEX, "array_index", array_index_interpreter_op, array_index_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_ARRAY_SET, "array_set", array_set_interpreter_op, array_set_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_UNARY_MINUS, "u-", unary_minus_interpreter_op, unary_minus_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_UNARY_PLUS, "u+", unary_plus_interpreter_op, unary_plus_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_UNARY_BNOT, "u~", unary_bnot_interpreter_op, unary_bnot_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_UNARY_NOT, "not", unary_not_interpreter_op, unary_not_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_ADD, "+", add_interpreter_op, add_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_SUBTRACT, "-", subtract_interpreter_op, subtract_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_MULT, "*", mult_interpreter_op, mult_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_DIV, "/", div_interpreter_op, div_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_MOD, "%", mod_interpreter_op, mod_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_EQUAL, "==", equal_interpreter_op, equal_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_NOTEQUAL, "!=", notequal_interpreter_op, notequal_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_ELSSOREQUAL, "<=", elssorequal_interpreter_op, lessorequal_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_GREATEROREQUAL, ">=", greaterorequal_interpreter_op, greaterorequal_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_LESS, "<", less_interpreter_op, less_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_GREATER, ">", greater_interpreter_op, greater_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_AND, "&", and_interpreter_op, and_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_OR, "|", or_interpreter_op, or_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_XOR, "^", xor_interpreter_op, xor_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_LAND, "&&", land_interpreter_op, land_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_LOR, "||", lor_interpreter_op, lor_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_EXP, "**", exp_interpreter_op, exp_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_GET_MEMBER, ".", get_member_interpreter_op, get_member_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_SET_MEMBER, ".=", set_member_interpreter_op, set_member_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_PTR_EQ, "ptr_eq", ptr_eq_interpreter_op, return_bool_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_PTR_NEQ, "ptr_neq", ptr_neq_interpreter_op, return_bool_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_CAST, "cast", cast_interpreter_op, cast_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_TO_STRING, "to_string", to_string_interpreter_op, return_string_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_COPY_STRING, "copy_string", copy_string_interpreter_op, return_string_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_STRING_INDEX, "string_index", string_index_interpreter_op, return_string_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_STRING_CONCAT, "string_concat", string_concat_interpreter_op, return_string_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_STRING_EQUAL, "string_equal", string_equal_interpreter_op, return_bool_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_STRING_SELECT, "string_select", string_select_interpreter_op, return_string_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_STRING_STRIDED_SELECT, "string_strided_select", string_strided_select_interpreter_op, return_string_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_STRING_LENGTH, "string_length", string_length_interpreter_op, return_int_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_CHPL_ALLOC, "chpl_alloc", alloc_interpreter_op, alloc_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_EXIT, "exit", done_interpreter_op, return_int_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_HALT, "halt", done_interpreter_op, return_int_analysis_op));
  primitives.add(new PrimitiveOp(PRIMITIVE_ASSERT, "assert", done_interpreter_op, return_int_analysis_op));
  // TEMPORARY
  primitives.add(new PrimitiveOp(PRIMITIVE_UNKNOWN, "array_pointwise_op", unimplemented_interpreter_op, array_pointwise_op_analysis_op));
}
