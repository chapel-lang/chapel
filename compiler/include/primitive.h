#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H_

#include "chpl.h"

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
  PRIMITIVE_MIN,
  PRIMITIVE_MAX,
  PRIMITIVE_PROD_ID,                   // product identity operand
  PRIMITIVE_BAND_ID,                   // bit-wise AND identity operand
  PRIMITIVE_BOR_ID,                    // bit-wise OR identity operand
  PRIMITIVE_BXOR_ID,                   // bit-wise XOR identity operand
  PRIMITIVE_LAND_ID,                   // logical AND identity operand
  PRIMITIVE_LOR_ID,                    // logical OR identity operand
  PRIMITIVE_LXOR_ID,                   // logical XOR identity operand
  PRIMITIVE_GETCID,
  PRIMITIVE_SETCID,
  PRIMITIVE_GET_MEMBER,
  PRIMITIVE_SET_MEMBER,
  PRIMITIVE_GET_MEMBER_REF_TO,         // get the value of the reference
  PRIMITIVE_SET_MEMBER_REF_TO,         // set the reference to a var
  PRIMITIVE_SET_HEAPVAR,               // used to alloc on_heap vars
  PRIMITIVE_REFC_INIT,                 // initialize ref-counted heap var
  PRIMITIVE_REFC_TOUCH,                // touch reference-counted var
  PRIMITIVE_REFC_RELEASE,              // dec ref-counter and maybe free var
  PRIMITIVE_THREAD_ID,

  PRIMITIVE_SYNCVAR_LOCK,                
  PRIMITIVE_SYNCVAR_UNLOCK,
  PRIMITIVE_SYNCVAR_SIGNAL_FULL,
  PRIMITIVE_SYNCVAR_WAIT_FULL,
  PRIMITIVE_SYNCVAR_SIGNAL_EMPTY,
  PRIMITIVE_SYNCVAR_WAIT_EMPTY,
  PRIMITIVE_MUTEX_NEW,
  PRIMITIVE_MUTEX_DESTROY,
  PRIMITIVE_CONDVAR_NEW,
  PRIMITIVE_CONDVAR_DESTROY,

  PRIMITIVE_CHPL_ALLOC,
  PRIMITIVE_CHPL_FREE,                 // only for on_heap vars?
  PRIMITIVE_PTR_EQUAL,
  PRIMITIVE_PTR_NOTEQUAL,
  PRIMITIVE_TOSTRING,
  PRIMITIVE_CAST,
  PRIMITIVE_ISSUBTYPE,
  PRIMITIVE_TYPEOF,
  PRIMITIVE_USE,
  PRIMITIVE_TUPLE_EXPAND,
  PRIMITIVE_ARRAY_INIT,
  PRIMITIVE_ARRAY_GET,
  PRIMITIVE_ARRAY_SET,
  NUM_KNOWN_PRIMS
};

class PrimitiveOp : public gc { public:
  PrimitiveTag tag;
  char *name;
  Type *(*returnInfo)(CallExpr*);
  bool isReference;

  PrimitiveOp(PrimitiveTag atag, char *aname, Type *(*areturnInfo)(CallExpr*), bool aIsReference);
};

extern HashMap<char *, StringHashFns, PrimitiveOp *> primitives_map;

extern PrimitiveOp* primitives[NUM_KNOWN_PRIMS];

void initPrimitive();

#endif
