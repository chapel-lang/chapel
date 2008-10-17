#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H_

#include "chpl.h"

enum PrimitiveTag {
  PRIMITIVE_UNKNOWN = 0,    // use for any primitives not in this list
  PRIMITIVE_ACTUALS_LIST,
  PRIMITIVE_NOOP,
  PRIMITIVE_MOVE,
  PRIMITIVE_INIT,
  PRIMITIVE_SET_REF,
  PRIMITIVE_GET_REF,
  PRIMITIVE_REF2STR,
  PRIMITIVE_RETURN,
  PRIMITIVE_YIELD,
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
  PRIMITIVE_UNION_GETID,
  PRIMITIVE_UNION_SETID,
  PRIMITIVE_GET_MEMBER,
  PRIMITIVE_GET_MEMBER_VALUE,
  PRIMITIVE_SET_MEMBER,
  PRIMITIVE_CHECK_NIL,
  PRIMITIVE_NEW,                       // new keyword
  PRIMITIVE_GET_REAL,                  // get complex real component
  PRIMITIVE_GET_IMAG,                  // get complex imag component

  PRIMITIVE_LOCAL_CHECK,          // Assert that a wide ref is on this locale
  PRIMITIVE_LOCAL_DEREF,          // Assign a wide ref addr field to a non-wide

  PRIMITIVE_THREAD_INIT,
  PRIMITIVE_THREAD_ID,
  PRIMITIVE_GET_SERIAL,                // get serial state
  PRIMITIVE_SET_SERIAL,                // set serial state to true or false

  PRIMITIVE_SYNC_INIT,                
  PRIMITIVE_SYNC_DESTROY,                
  PRIMITIVE_SYNC_LOCK,                
  PRIMITIVE_SYNC_UNLOCK,
  PRIMITIVE_SYNC_WAIT_FULL,
  PRIMITIVE_SYNC_WAIT_EMPTY,
  PRIMITIVE_SYNC_SIGNAL_FULL,
  PRIMITIVE_SYNC_SIGNAL_EMPTY,
  PRIMITIVE_SINGLE_INIT,
  PRIMITIVE_SINGLE_LOCK,
  PRIMITIVE_SINGLE_UNLOCK,
  PRIMITIVE_SINGLE_WAIT_FULL,
  PRIMITIVE_SINGLE_SIGNAL_FULL,

  PRIMITIVE_WRITEEF,
  PRIMITIVE_WRITEFF,
  PRIMITIVE_WRITEXF,
  PRIMITIVE_SYNC_RESET,
  PRIMITIVE_READFE,
  PRIMITIVE_READFF,
  PRIMITIVE_READXX,
  PRIMITIVE_SYNC_ISFULL,
  PRIMITIVE_SINGLE_WRITEEF,
  PRIMITIVE_SINGLE_RESET,
  PRIMITIVE_SINGLE_READFF,
  PRIMITIVE_SINGLE_READXX,
  PRIMITIVE_SINGLE_ISFULL,

  PRIMITIVE_GET_END_COUNT,
  PRIMITIVE_SET_END_COUNT,

  PRIMITIVE_INIT_TASK_LIST,
  PRIMITIVE_PROCESS_TASK_LIST,
  PRIMITIVE_EXECUTE_TASKS_IN_LIST,
  PRIMITIVE_FREE_TASK_LIST,

  PRIMITIVE_CHPL_ALLOC,
  PRIMITIVE_CHPL_ALLOC_PERMIT_ZERO,  // chpl_alloc wrapper that permits size 0
  PRIMITIVE_CHPL_FREE,               // only for variables on heap?
  PRIMITIVE_INIT_FIELDS, // initialize fields of a temporary record
  PRIMITIVE_PTR_EQUAL,
  PRIMITIVE_PTR_NOTEQUAL,
  PRIMITIVE_CAST,
  PRIMITIVE_DYNAMIC_CAST,
  PRIMITIVE_ISSUBTYPE,
  PRIMITIVE_TYPEOF,
  PRIMITIVE_GET_ITERATOR_RETURN,
  PRIMITIVE_USE,
  PRIMITIVE_USED_MODULES_LIST, // used modules in BlockStmt::modUses
  PRIMITIVE_TUPLE_EXPAND,
  PRIMITIVE_TUPLE_AND_EXPAND,
  PRIMITIVE_ARRAY_FREE,
  PRIMITIVE_ARRAY_FREE_ELTS,
  PRIMITIVE_ARRAY_ALLOC,
  PRIMITIVE_ARRAY_GET,
  PRIMITIVE_ARRAY_GET_VALUE,
  PRIMITIVE_ARRAY_SET,
  PRIMITIVE_ARRAY_SET_FIRST,
  PRIMITIVE_ERROR,
  PRIMITIVE_WARNING,
  PRIMITIVE_WHEN,

  PRIMITIVE_BLOCK_PARAM_LOOP,      // BlockStmt::blockInfo - param for loop
  PRIMITIVE_BLOCK_WHILEDO_LOOP,    // BlockStmt::blockInfo - while do loop
  PRIMITIVE_BLOCK_DOWHILE_LOOP,    // BlockStmt::blockInfo - do while loop
  PRIMITIVE_BLOCK_FOR_LOOP,        // BlockStmt::blockInfo - for loop
  PRIMITIVE_BLOCK_INLINE_FOR_LOOP, // BlockStmt::blockInfo - for loop
  PRIMITIVE_BLOCK_BEGIN,           // BlockStmt::blockInfo - begin block
  PRIMITIVE_BLOCK_COBEGIN,         // BlockStmt::blockInfo - cobegin block
  PRIMITIVE_BLOCK_COFORALL,        // BlockStmt::blockInfo - coforall block
  PRIMITIVE_BLOCK_ON,              // BlockStmt::blockInfo - on block
  PRIMITIVE_BLOCK_LOCAL,           // BlockStmt::blockInfo - local block

  PRIMITIVE_TO_LEADER,
  PRIMITIVE_TO_FOLLOWER,

  PRIMITIVE_DELETE,

  PRIMITIVE_GC_CC_INIT,       // Initialize heap for copy-collecting
  PRIMITIVE_GC_ADD_ROOT,      // Add a root variable for garbage collection
  PRIMITIVE_GC_ADD_NULL_ROOT, // Add a root and point it to NULL
  PRIMITIVE_GC_DELETE_ROOT,   // Remove a root variable for garbage collection
  PRIMITIVE_GC_CLEANUP,       // Free GC heaps

  PRIMITIVE_IS_ENUM,   // True if argument is an enum
  PRIMITIVE_IS_TUPLE,  // True if argument is a tuple

  PRIMITIVE_LOGICAL_FOLDER, // Help fold logical && and ||

  PRIMITIVE_GET_LOCALEID,   // return locale on which an expression exists
  PRIMITIVE_LOCALE_ID,    // return locale id
  PRIMITIVE_NUM_LOCALES,  // return number of locales
  PRIMITIVE_ON_LOCALE_NUM,  // specify a particular locale # for an on clause

  PRIMITIVE_ALLOC_GVR,    // allocate space for global vars registry
  PRIMITIVE_HEAP_REGISTER_GLOBAL_VAR,
  PRIMITIVE_HEAP_BROADCAST_GLOBAL_VARS,
  PRIMITIVE_PRIVATE_BROADCAST,

  PRIMITIVE_INT_ERROR,

  PRIMITIVE_GET_ERRNO,

  PRIMITIVE_RT_ERROR,
  PRIMITIVE_RT_WARNING,

  PRIMITIVE_PRIVATE_SET_CLASS,
  PRIMITIVE_PRIVATE_NUM_CLASSES,
  PRIMITIVE_PRIVATE_GET_CLASS,

  NUM_KNOWN_PRIMS
};

class PrimitiveOp { public:
  PrimitiveTag tag;
  const char *name;
  Type *(*returnInfo)(CallExpr*);
  bool isEssential; // has effects visible outside of the function
  bool passLineno; // pass line number and filename to this primitive

  PrimitiveOp(PrimitiveTag atag, const char *aname, Type *(*areturnInfo)(CallExpr*));
};

extern HashMap<const char *, StringHashFns, PrimitiveOp *> primitives_map;

extern PrimitiveOp* primitives[NUM_KNOWN_PRIMS];

void initPrimitive();

#endif
