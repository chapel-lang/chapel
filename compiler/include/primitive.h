#ifndef _PRIM_H_
#define _PRIM_H_

#include "chpl.h"

enum PrimitiveTag {
  PRIM_UNKNOWN = 0,    // use for any primitives not in this list

  PRIM_ACTUALS_LIST,
  PRIM_NOOP,
  PRIM_MOVE,
  PRIM_INIT,
  PRIM_REF_TO_STRING,
  PRIM_RETURN,
  PRIM_YIELD,
  PRIM_UNARY_MINUS,
  PRIM_UNARY_PLUS,
  PRIM_UNARY_NOT,
  PRIM_UNARY_LNOT,
  PRIM_ADD,
  PRIM_SUBTRACT,
  PRIM_MULT,
  PRIM_DIV,
  PRIM_MOD,
  PRIM_LSH,
  PRIM_RSH,
  PRIM_EQUAL,
  PRIM_NOTEQUAL,
  PRIM_LESSOREQUAL,
  PRIM_GREATEROREQUAL,
  PRIM_LESS,
  PRIM_GREATER,
  PRIM_AND,
  PRIM_OR,
  PRIM_XOR,
  PRIM_POW,

  PRIM_ADD_ASSIGN,
  PRIM_SUBTRACT_ASSIGN,
  PRIM_MULT_ASSIGN,
  PRIM_DIV_ASSIGN,
  PRIM_MOD_ASSIGN,
  PRIM_LSH_ASSIGN,
  PRIM_RSH_ASSIGN,
  PRIM_AND_ASSIGN,
  PRIM_OR_ASSIGN,
  PRIM_XOR_ASSIGN,

  PRIM_MIN,
  PRIM_MAX,

  PRIM_SETCID,
  PRIM_TESTCID,
  PRIM_GETCID,
  PRIM_SET_UNION_ID,
  PRIM_GET_UNION_ID,
  PRIM_GET_MEMBER,
  PRIM_GET_MEMBER_VALUE,
  PRIM_SET_MEMBER,
  PRIM_CHECK_NIL,
  PRIM_NEW,                 // new keyword
  PRIM_GET_REAL,            // get complex real component
  PRIM_GET_IMAG,            // get complex imag component
  PRIM_QUERY,               // query expression primitive

  PRIM_ADDR_OF,             // set a reference to a value
  PRIM_DEREF,               // dereference a reference

  PRIM_LOCAL_CHECK,         // assert that a wide ref is on this locale

  PRIM_SYNC_INIT,
  PRIM_SYNC_DESTROY,
  PRIM_SYNC_LOCK,
  PRIM_SYNC_UNLOCK,
  PRIM_SYNC_WAIT_FULL,
  PRIM_SYNC_WAIT_EMPTY,
  PRIM_SYNC_SIGNAL_FULL,
  PRIM_SYNC_SIGNAL_EMPTY,
  PRIM_SINGLE_INIT,
  PRIM_SINGLE_DESTROY,
  PRIM_SINGLE_LOCK,
  PRIM_SINGLE_UNLOCK,
  PRIM_SINGLE_WAIT_FULL,
  PRIM_SINGLE_SIGNAL_FULL,

  PRIM_WRITEEF,
  PRIM_WRITEFF,
  PRIM_WRITEXF,
  PRIM_READFE,
  PRIM_READFF,
  PRIM_READXX,
  PRIM_SYNC_IS_FULL,
  PRIM_SINGLE_WRITEEF,
  PRIM_SINGLE_READFF,
  PRIM_SINGLE_READXX,
  PRIM_SINGLE_IS_FULL,

  PRIM_GET_END_COUNT,
  PRIM_SET_END_COUNT,

  PRIM_PROCESS_TASK_LIST,
  PRIM_EXECUTE_TASKS_IN_LIST,
  PRIM_FREE_TASK_LIST,

  PRIM_GET_SERIAL,              // get serial state
  PRIM_SET_SERIAL,              // set serial state to true or false

  PRIM_SIZEOF,

  PRIM_INIT_FIELDS,             // initialize fields of a temporary record
  PRIM_PTR_EQUAL,
  PRIM_PTR_NOTEQUAL,
  PRIM_IS_SUBTYPE,
  PRIM_CAST,
  PRIM_DYNAMIC_CAST,
  PRIM_TYPEOF,
  PRIM_GET_ITERATOR_RETURN,
  PRIM_USE,
  PRIM_USED_MODULES_LIST,       // used modules in BlockStmt::modUses
  PRIM_TUPLE_EXPAND,
  PRIM_TUPLE_AND_EXPAND,

  PRIM_CHPL_COMM_GET,           // Direct calls to the Chapel comm layer
  PRIM_CHPL_COMM_PUT,           // may eventually add others (e.g., non-blocking)
  PRIM_CHPL_COMM_GET_STRD,      // Direct calls to the Chapel comm layer for strided comm
  PRIM_CHPL_COMM_PUT_STRD,      //  may eventually add others (e.g., non-blocking)

  PRIM_ARRAY_ALLOC,
  PRIM_ARRAY_FREE,
  PRIM_ARRAY_FREE_ELTS,
  PRIM_ARRAY_GET,
  PRIM_ARRAY_GET_VALUE,
  PRIM_ARRAY_SHIFT_BASE_POINTER,

  PRIM_ARRAY_SET,
  PRIM_ARRAY_SET_FIRST,

  PRIM_ERROR,
  PRIM_WARNING,
  PRIM_WHEN,
  PRIM_TYPE_TO_STRING,

  PRIM_BLOCK_PARAM_LOOP,        // BlockStmt::blockInfo - param for loop (index,
                                // low, high, stride)
  PRIM_BLOCK_WHILEDO_LOOP,      // BlockStmt::blockInfo - while do loop (cond)
  PRIM_BLOCK_DOWHILE_LOOP,      // BlockStmt::blockInfo - do while loop (cond)
  PRIM_BLOCK_FOR_LOOP,          // BlockStmt::blockInfo - for loop (index, iterator)
  PRIM_BLOCK_BEGIN,             // BlockStmt::blockInfo - begin block
  PRIM_BLOCK_COBEGIN,           // BlockStmt::blockInfo - cobegin block
  PRIM_BLOCK_COFORALL,          // BlockStmt::blockInfo - coforall block
  PRIM_BLOCK_ON,                // BlockStmt::blockInfo - on block
  PRIM_BLOCK_ON_NB,             // BlockStmt::blockInfo - non-blocking on block
  PRIM_BLOCK_LOCAL,             // BlockStmt::blockInfo - local block
  PRIM_BLOCK_UNLOCAL,           // BlockStmt::blockInfo - unlocal local block

  PRIM_TO_LEADER,
  PRIM_TO_FOLLOWER,

  PRIM_DELETE,

  PRIM_GC_CC_INIT,              // Initialize heap for copy-collecting
  PRIM_GC_ADD_ROOT,             // Add a root variable for garbage collection
  PRIM_GC_ADD_NULL_ROOT,        // Add a root and point it to NULL
  PRIM_GC_DELETE_ROOT,          // Remove a root variable for garbage collection
  PRIM_GC_CLEANUP,              // Free GC heaps

  PRIM_CALL_DESTRUCTOR,         // call destructor on type (do not free)

  PRIM_LOGICAL_FOLDER,          // Help fold logical && and ||

  PRIM_WIDE_GET_LOCALE,         // Returns the "locale" portion of a wide pointer.

  PRIM_WIDE_GET_NODE,           // Get just the node portion of a wide pointer.
  PRIM_WIDE_GET_ADDR,           // Get just the address portion of a wide pointer.

  PRIM_ON_LOCALE_NUM,           // specify a particular localeID for an on clause.

  PRIM_ALLOC_GVR,               // allocate space for global vars registry
  PRIM_HEAP_REGISTER_GLOBAL_VAR,
  PRIM_HEAP_BROADCAST_GLOBAL_VARS,
  PRIM_PRIVATE_BROADCAST,

  PRIM_INT_ERROR,

  PRIM_CAPTURE_FN,
  PRIM_CREATE_FN_TYPE,

  PRIM_STRING_COPY,
  PRIM_STRING_NORMALIZE,        // Set the size field in a (wide) string.
  PRIM_STRING_FROM_C_STRING,
  PRIM_C_STRING_FROM_STRING,
  PRIM_CAST_TO_VOID_STAR,       // Cast the object argument to void*.

  PRIM_RT_ERROR,
  PRIM_RT_WARNING,

  PRIM_NEW_PRIV_CLASS,
  PRIM_NUM_PRIV_CLASSES,
  PRIM_GET_PRIV_CLASS,

  PRIM_NEXT_UINT32,
  PRIM_GET_USER_LINE,
  PRIM_GET_USER_FILE,

  PRIM_FTABLE_CALL,

  PRIM_IS_STAR_TUPLE_TYPE,
  PRIM_SET_SVEC_MEMBER,
  PRIM_GET_SVEC_MEMBER,
  PRIM_GET_SVEC_MEMBER_VALUE,

  PRIM_VMT_CALL,        // virtual method call

  PRIM_NUM_FIELDS,
  PRIM_FIELD_NUM_TO_NAME,
  PRIM_FIELD_VALUE_BY_NUM,
  PRIM_FIELD_ID_BY_NUM,
  PRIM_FIELD_VALUE_BY_NAME,
  PRIM_IS_UNION_TYPE,

  PRIM_ENUM_MIN_BITS,
  PRIM_ENUM_IS_SIGNED,

  NUM_KNOWN_PRIMS
};

class PrimitiveOp { public:
  PrimitiveTag tag;
  const char *name;
  Type *(*returnInfo)(CallExpr*);
  bool isEssential; // has effects visible outside of the function
  bool passLineno;  // pass line number and filename to this primitive

  PrimitiveOp(PrimitiveTag atag, const char *aname, Type *(*areturnInfo)(CallExpr*));
};

extern HashMap<const char *, StringHashFns, PrimitiveOp *> primitives_map;

extern PrimitiveOp* primitives[NUM_KNOWN_PRIMS];

void printPrimitiveCounts(const char* passName);
void initPrimitive();

extern Map<const char*, VarSymbol*> memDescsMap;
extern Vec<const char*> memDescsVec;

VarSymbol* newMemDesc(const char* str);

#endif
