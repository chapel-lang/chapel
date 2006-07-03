#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H_

#include "chpl.h"

class InterpreterOp;

enum PrimitiveTag {
  PRIMITIVE_UNKNOWN = 0,    // use for any primitives not in this list
  PRIMITIVE_MOVE,
  PRIMITIVE_UNARY_MINUS,
  PRIMITIVE_UNARY_PLUS,
  PRIMITIVE_UNARY_NOT,
  PRIMITIVE_UNARY_LNOT,
  PRIMITIVE_ADD,
  PRIMITIVE_SUBTRACT,
  PRIMITIVE_MULT,
  PRIMITIVE_DIV,
  PRIMITIVE_MOD,
  PRIMITIVE_LSH,
  PRIMITIVE_RSH,
  PRIMITIVE_EQUAL,
  PRIMITIVE_NOTEQUAL,
  PRIMITIVE_LESSOREQUAL,
  PRIMITIVE_GREATEROREQUAL,
  PRIMITIVE_LESS,
  PRIMITIVE_GREATER,
  PRIMITIVE_AND,
  PRIMITIVE_OR,
  PRIMITIVE_XOR,
  PRIMITIVE_LAND,
  PRIMITIVE_LOR,
  PRIMITIVE_POW,
  PRIMITIVE_GET_MEMBER,
  PRIMITIVE_SET_MEMBER,
  PRIMITIVE_GET_MEMBER_REF_TO,         // get the value of the reference
  PRIMITIVE_SET_MEMBER_REF_TO,         // set the reference to a var
  PRIMITIVE_SET_HEAPVAR,               // used to alloc on_heap vars
  PRIMITIVE_CHPL_ALLOC,
  PRIMITIVE_CHPL_FREE,                 // only for on_heap vars?
  PRIMITIVE_TYPE_EQUAL,
  PRIMITIVE_PTR_EQUAL,
  PRIMITIVE_PTR_NOTEQUAL,
  NUM_KNOWN_PRIMS
};

class PrimitiveOp : public gc { public:
  PrimitiveTag tag;
  char *name;
  InterpreterOp *interpreterOp;
  Type *(*returnInfo)(CallExpr*);

  PrimitiveOp(PrimitiveTag atag, char *aname, InterpreterOp *aiop, Type *(*areturnInfo)(CallExpr*));
};

extern HashMap<char *, StringHashFns, PrimitiveOp *> primitives_map;

extern PrimitiveOp* primitives[NUM_KNOWN_PRIMS];

void initPrimitive();

#endif
