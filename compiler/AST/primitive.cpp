#include "primitive.h"


Vec<PrimitiveOp *> primitives;

PrimitiveOp::PrimitiveOp(char *aname) : name(aname) {
}


void 
initPrimitive() {
  primitives.add(new PrimitiveOp("init"));
  primitives.add(new PrimitiveOp("sizeof"));
  primitives.add(new PrimitiveOp("fopen"));
  primitives.add(new PrimitiveOp("fclose"));
  primitives.add(new PrimitiveOp("strerr"));
  primitives.add(new PrimitiveOp("write"));
  primitives.add(new PrimitiveOp("read"));
  primitives.add(new PrimitiveOp("coerce"));
  primitives.add(new PrimitiveOp("index_object"));
  primitives.add(new PrimitiveOp("set_index_object"));
  primitives.add(new PrimitiveOp("array_pointwise_op"));
  primitives.add(new PrimitiveOp("u-"));
  primitives.add(new PrimitiveOp("u+"));
  primitives.add(new PrimitiveOp("u~"));
  primitives.add(new PrimitiveOp("not"));
  primitives.add(new PrimitiveOp("+"));
  primitives.add(new PrimitiveOp("-"));
  primitives.add(new PrimitiveOp("*"));
  primitives.add(new PrimitiveOp("/"));
  primitives.add(new PrimitiveOp("%"));
  primitives.add(new PrimitiveOp("=="));
  primitives.add(new PrimitiveOp("!="));
  primitives.add(new PrimitiveOp("<="));
  primitives.add(new PrimitiveOp(">="));
  primitives.add(new PrimitiveOp("<"));
  primitives.add(new PrimitiveOp(">"));
  primitives.add(new PrimitiveOp("&"));
  primitives.add(new PrimitiveOp("|"));
  primitives.add(new PrimitiveOp("^"));
  primitives.add(new PrimitiveOp("&&"));
  primitives.add(new PrimitiveOp("||"));
  primitives.add(new PrimitiveOp("**"));
  primitives.add(new PrimitiveOp("ptr_eq"));
  primitives.add(new PrimitiveOp("ptr_neq"));
  primitives.add(new PrimitiveOp("cast"));
  primitives.add(new PrimitiveOp("copy_string"));
  primitives.add(new PrimitiveOp("string_index"));
  primitives.add(new PrimitiveOp("string_select"));
}
