/*
 * Copyright 2004-2015 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _PRIM_H_
#define _PRIM_H_

#include "chpl.h"

class CallExpr;
class Type;
class VarSymbol;

enum PrimitiveTag {
  PRIM_UNKNOWN = 0,    // use for any primitives not in this list

  PRIM_ACTUALS_LIST,
  PRIM_NOOP,
  PRIM_MOVE,
  PRIM_INIT,
  PRIM_NO_INIT,
  PRIM_TYPE_INIT,       // Used in a context where only a type is needed.
                        // Establishes the type of the result without
                        // generating code.
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

  PRIM_ASSIGN,
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
  PRIM_QUERY_PARAM_FIELD,
  PRIM_QUERY_TYPE_FIELD,

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
  PRIM_USE,
  PRIM_USED_MODULES_LIST,       // used modules in BlockStmt::modUses
  PRIM_TUPLE_EXPAND,
  PRIM_TUPLE_AND_EXPAND,

  PRIM_CHPL_COMM_GET,           // Direct calls to the Chapel comm layer
  PRIM_CHPL_COMM_PUT,           // may eventually add others (e.g., non-blocking)
  PRIM_CHPL_COMM_REMOTE_PREFETCH,
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
  PRIM_BLOCK_C_FOR_LOOP,        // BlockStmt::blockInfo -
                                //   C for loop (initExpr, testExpr, incrExpr)

  PRIM_BLOCK_BEGIN,             // BlockStmt::blockInfo - begin block
  PRIM_BLOCK_COBEGIN,           // BlockStmt::blockInfo - cobegin block
  PRIM_BLOCK_COFORALL,          // BlockStmt::blockInfo - coforall block
  PRIM_BLOCK_ON,                // BlockStmt::blockInfo - on block
  PRIM_BLOCK_BEGIN_ON,          // BlockStmt::blockInfo - begin on block
  PRIM_BLOCK_COBEGIN_ON,        // BlockStmt::blockInfo - cobegin on block
  PRIM_BLOCK_COFORALL_ON,       // BlockStmt::blockInfo - coforall on block
  PRIM_BLOCK_LOCAL,             // BlockStmt::blockInfo - local block
  PRIM_BLOCK_UNLOCAL,           // BlockStmt::blockInfo - unlocal local block

  PRIM_FORALL_LOOP,             // BlockStmt::byrefVars - forall loop body
  PRIM_TO_LEADER,
  PRIM_TO_FOLLOWER,
  PRIM_TO_STANDALONE,

  PRIM_DELETE,

  PRIM_CALL_DESTRUCTOR,         // call destructor on type (do not free)

  PRIM_LOGICAL_FOLDER,          // Help fold logical && and ||

  PRIM_WIDE_GET_LOCALE,         // Returns the "locale" portion of a wide pointer.

  PRIM_WIDE_GET_NODE,           // Get just the node portion of a wide pointer.
  PRIM_WIDE_GET_ADDR,           // Get just the address portion of a wide pointer.

  PRIM_ON_LOCALE_NUM,           // specify a particular localeID for an on clause.

  PRIM_HEAP_REGISTER_GLOBAL_VAR,
  PRIM_HEAP_BROADCAST_GLOBAL_VARS,
  PRIM_PRIVATE_BROADCAST,       // ('_private_broadcast' sym)
                                // Later, a structure index is inserted ahead
                                // of the symbol, so it ends up as
                                // ('_private_broadcast' index sym).

  PRIM_INT_ERROR,

  PRIM_CAPTURE_FN,
  PRIM_CREATE_FN_TYPE,

  PRIM_STRING_COPY,
  PRIM_STRING_FROM_C_STRING,
  PRIM_C_STRING_FROM_STRING,
  PRIM_CAST_TO_VOID_STAR,       // Cast the object argument to void*.

  PRIM_RT_ERROR,
  PRIM_RT_WARNING,

  PRIM_NEW_PRIV_CLASS,
  PRIM_GET_PRIV_CLASS,

  PRIM_GET_USER_LINE,
  PRIM_GET_USER_FILE,

  PRIM_FTABLE_CALL,

  PRIM_IS_SYNC_TYPE,
  PRIM_IS_SINGLE_TYPE,
  PRIM_IS_TUPLE_TYPE,
  PRIM_IS_STAR_TUPLE_TYPE,
  PRIM_SET_SVEC_MEMBER,
  PRIM_GET_SVEC_MEMBER,
  PRIM_GET_SVEC_MEMBER_VALUE,

  PRIM_VIRTUAL_METHOD_CALL,

  PRIM_NUM_FIELDS,
  PRIM_FIELD_NUM_TO_NAME,
  PRIM_FIELD_VALUE_BY_NUM,
  PRIM_FIELD_ID_BY_NUM,
  PRIM_FIELD_VALUE_BY_NAME,
  PRIM_IS_UNION_TYPE,
  PRIM_IS_ATOMIC_TYPE,

  PRIM_ENUM_MIN_BITS,
  PRIM_ENUM_IS_SIGNED,

  PRIM_START_RMEM_FENCE,
  PRIM_FINISH_RMEM_FENCE,

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
