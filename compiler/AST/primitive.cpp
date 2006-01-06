#include "primitive.h"
#include "../analysis/analysis_ops.h"
#include "../passes/interpreter_ops.h"

Vec<PrimitiveOp *> primitives;
HashMap<char *, StringHashFns, PrimitiveOp *> primitives_map;

static int prim_index = 0;

PrimitiveOp::PrimitiveOp(char *aname, InterpreterOp *aiop, AnalysisOp *aaop)
  : name(aname), interpreterOp(aiop), analysisOp(aaop)
{
  index = prim_index++;
  primitives.add(this);
  primitives_map.put(name, this);
}

void 
initPrimitive() {
  primitives.add(new PrimitiveOp("init", init_interpreter_op, init_analysis_op));
  primitives.add(new PrimitiveOp("sizeof", unimplemented_interpreter_op, unimplemented_analysis_op));
  primitives.add(new PrimitiveOp("fopen", fopen_interpreter_op, return_int_analysis_op));
  primitives.add(new PrimitiveOp("fclose", fclose_interpreter_op, return_int_analysis_op));
  primitives.add(new PrimitiveOp("strerror", strerror_interpreter_op, return_string_analysis_op));
  primitives.add(new PrimitiveOp("fprintf", fprintf_interpreter_op, return_int_analysis_op));
  primitives.add(new PrimitiveOp("fscanf", fscanf_interpreter_op, return_int_analysis_op));
  primitives.add(new PrimitiveOp("array_index", array_index_interpreter_op, array_index_analysis_op));
  primitives.add(new PrimitiveOp("array_set", array_set_interpreter_op, array_set_analysis_op));
  primitives.add(new PrimitiveOp("array_pointwise_op", unimplemented_interpreter_op, array_pointwise_op_analysis_op));
  primitives.add(new PrimitiveOp("u-", unary_minus_interpreter_op, unary_minus_analysis_op));
  primitives.add(new PrimitiveOp("u+", unary_plus_interpreter_op, unary_plus_analysis_op));
  primitives.add(new PrimitiveOp("u~", unary_bnot_interpreter_op, unary_bnot_analysis_op));
  primitives.add(new PrimitiveOp("not", unary_not_interpreter_op, unary_not_analysis_op));
  primitives.add(new PrimitiveOp("+", add_interpreter_op, add_analysis_op));
  primitives.add(new PrimitiveOp("-", subtract_interpreter_op, subtract_analysis_op));
  primitives.add(new PrimitiveOp("*", mult_interpreter_op, mult_analysis_op));
  primitives.add(new PrimitiveOp("/", div_interpreter_op, div_analysis_op));
  primitives.add(new PrimitiveOp("%", mod_interpreter_op, mod_analysis_op));
  primitives.add(new PrimitiveOp("==", equal_interpreter_op, equal_analysis_op));
  primitives.add(new PrimitiveOp("!=", notequal_interpreter_op, notequal_analysis_op));
  primitives.add(new PrimitiveOp("<=", elssorequal_interpreter_op, lessorequal_analysis_op));
  primitives.add(new PrimitiveOp(">=", greaterorequal_interpreter_op, greaterorequal_analysis_op));
  primitives.add(new PrimitiveOp("<", less_interpreter_op, less_analysis_op));
  primitives.add(new PrimitiveOp(">", greater_interpreter_op, greater_analysis_op));
  primitives.add(new PrimitiveOp("&", and_interpreter_op, and_analysis_op));
  primitives.add(new PrimitiveOp("|", or_interpreter_op, or_analysis_op));
  primitives.add(new PrimitiveOp("^", xor_interpreter_op, xor_analysis_op));
  primitives.add(new PrimitiveOp("&&", land_interpreter_op, land_analysis_op));
  primitives.add(new PrimitiveOp("||", lor_interpreter_op, lor_analysis_op));
  primitives.add(new PrimitiveOp("**", exp_interpreter_op, exp_analysis_op));
  // currently not implemented
  // OP_SEQCAT #
  // OP_BY by
  // OP_SUBTYPE :
  // OP_NOTSUBTYPE !:
  primitives.add(new PrimitiveOp("ptr_eq", ptr_eq_interpreter_op, return_bool_analysis_op));
  primitives.add(new PrimitiveOp("ptr_neq", ptr_neq_interpreter_op, return_bool_analysis_op));
  primitives.add(new PrimitiveOp("cast", cast_interpreter_op, cast_analysis_op));
  primitives.add(new PrimitiveOp("to_string", to_string_interpreter_op, return_string_analysis_op));
  primitives.add(new PrimitiveOp("copy_string", copy_string_interpreter_op, return_string_analysis_op));
  primitives.add(new PrimitiveOp("string_index", string_index_interpreter_op, return_string_analysis_op));
  primitives.add(new PrimitiveOp("string_concat", string_concat_interpreter_op, return_string_analysis_op));
  primitives.add(new PrimitiveOp("string_equal", string_equal_interpreter_op, return_bool_analysis_op));
  primitives.add(new PrimitiveOp("string_select", string_select_interpreter_op, return_string_analysis_op));
  primitives.add(new PrimitiveOp("string_strided_select", string_strided_select_interpreter_op, return_string_analysis_op));
  primitives.add(new PrimitiveOp("string_length", string_length_interpreter_op, return_int_analysis_op));
  primitives.add(new PrimitiveOp("chpl_alloc", alloc_interpreter_op, alloc_analysis_op));
  primitives.add(new PrimitiveOp("exit", done_interpreter_op, return_int_analysis_op));
  primitives.add(new PrimitiveOp("halt", done_interpreter_op, return_int_analysis_op));
  primitives.add(new PrimitiveOp("assert", done_interpreter_op, return_int_analysis_op));
}
