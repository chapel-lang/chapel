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
  primitives.add(new PrimitiveOp("init", unimplemented_interpreter_op, init_analysis_op));
  primitives.add(new PrimitiveOp("sizeof", unimplemented_interpreter_op, unimplemented_analysis_op));
  primitives.add(new PrimitiveOp("fopen", unimplemented_interpreter_op, return_int_analysis_op));
  primitives.add(new PrimitiveOp("fclose", unimplemented_interpreter_op, return_int_analysis_op));
  primitives.add(new PrimitiveOp("strerror", unimplemented_interpreter_op, return_string_analysis_op));
  primitives.add(new PrimitiveOp("write", unimplemented_interpreter_op, return_int_analysis_op));
  primitives.add(new PrimitiveOp("read", unimplemented_interpreter_op, return_int_analysis_op));
  primitives.add(new PrimitiveOp("coerce", unimplemented_interpreter_op, coerce_analysis_op));
  primitives.add(new PrimitiveOp("array_index", unimplemented_interpreter_op, array_index_analysis_op));
  primitives.add(new PrimitiveOp("array_set", unimplemented_interpreter_op, array_set_analysis_op));
  primitives.add(new PrimitiveOp("index_object", unimplemented_interpreter_op, index_object_analysis_op));
  primitives.add(new PrimitiveOp("set_index_object", unimplemented_interpreter_op, set_index_object_analysis_op));
  primitives.add(new PrimitiveOp("array_pointwise_op", unimplemented_interpreter_op, array_pointwise_op_analysis_op));
  primitives.add(new PrimitiveOp("u-", unimplemented_interpreter_op, unary_minus_analysis_op));
  primitives.add(new PrimitiveOp("u+", unimplemented_interpreter_op, unary_plus_analysis_op));
  primitives.add(new PrimitiveOp("u~", unimplemented_interpreter_op, unary_bnot_analysis_op));
  primitives.add(new PrimitiveOp("not", unimplemented_interpreter_op, unary_not_analysis_op));
  primitives.add(new PrimitiveOp("+", unimplemented_interpreter_op, add_analysis_op));
  primitives.add(new PrimitiveOp("-", unimplemented_interpreter_op, subtract_analysis_op));
  primitives.add(new PrimitiveOp("*", unimplemented_interpreter_op, mult_analysis_op));
  primitives.add(new PrimitiveOp("/", unimplemented_interpreter_op, div_analysis_op));
  primitives.add(new PrimitiveOp("%", unimplemented_interpreter_op, mod_analysis_op));
  primitives.add(new PrimitiveOp("==", unimplemented_interpreter_op, equal_analysis_op));
  primitives.add(new PrimitiveOp("!=", unimplemented_interpreter_op, notequal_analysis_op));
  primitives.add(new PrimitiveOp("<=", unimplemented_interpreter_op, lessorequal_analysis_op));
  primitives.add(new PrimitiveOp(">=", unimplemented_interpreter_op, greaterorequal_analysis_op));
  primitives.add(new PrimitiveOp("<", unimplemented_interpreter_op, less_analysis_op));
  primitives.add(new PrimitiveOp(">", unimplemented_interpreter_op, greater_analysis_op));
  primitives.add(new PrimitiveOp("&", unimplemented_interpreter_op, and_analysis_op));
  primitives.add(new PrimitiveOp("|", unimplemented_interpreter_op, or_analysis_op));
  primitives.add(new PrimitiveOp("^", unimplemented_interpreter_op, xor_analysis_op));
  primitives.add(new PrimitiveOp("&&", unimplemented_interpreter_op, land_analysis_op));
  primitives.add(new PrimitiveOp("||", unimplemented_interpreter_op, lor_analysis_op));
  primitives.add(new PrimitiveOp("**", unimplemented_interpreter_op, exp_analysis_op));
  // currently not implemented
  // OP_SEQCAT #
  // OP_BY by
  // OP_SUBTYPE :
  // OP_NOTSUBTYPE !:
  primitives.add(new PrimitiveOp("ptr_eq", unimplemented_interpreter_op, return_int_analysis_op));
  primitives.add(new PrimitiveOp("ptr_neq", unimplemented_interpreter_op, return_int_analysis_op));
  primitives.add(new PrimitiveOp("cast", unimplemented_interpreter_op, cast_analysis_op));
  primitives.add(new PrimitiveOp("to_string", unimplemented_interpreter_op, return_string_analysis_op));
  primitives.add(new PrimitiveOp("copy_string", unimplemented_interpreter_op, return_string_analysis_op));
  primitives.add(new PrimitiveOp("string_index", unimplemented_interpreter_op, return_string_analysis_op));
  primitives.add(new PrimitiveOp("string_concat", unimplemented_interpreter_op, return_string_analysis_op));
  primitives.add(new PrimitiveOp("string_equal", unimplemented_interpreter_op, return_int_analysis_op));
  primitives.add(new PrimitiveOp("string_select", unimplemented_interpreter_op, return_string_analysis_op));
  primitives.add(new PrimitiveOp("string_strided_select", unimplemented_interpreter_op, return_string_analysis_op));
  primitives.add(new PrimitiveOp("string_length", unimplemented_interpreter_op, return_int_analysis_op));
  primitives.add(new PrimitiveOp("chpl_alloc", unimplemented_interpreter_op, alloc_analysis_op));
  primitives.add(new PrimitiveOp("halt", unimplemented_interpreter_op, return_int_analysis_op));
  primitives.add(new PrimitiveOp("assert", unimplemented_interpreter_op, return_int_analysis_op));
}
