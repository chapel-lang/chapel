//
// insertTransactions pass:
// replace atomic block with calls to transaction primitives
//

#include "astutil.h"
#include "expr.h"
#include "optimizations.h"
#include "passes.h"
#include "stmt.h"
#include "symbol.h"
#include "stringutil.h"
#include "driver.h"
#include "files.h"

void txUnknownPrimitive(CallExpr *);
void txUnknownMovePrimitive(CallExpr*, CallExpr*);

static Map<FnSymbol*,FnSymbol*> fnCache; // cache of cloned stm functions
static Vec<BlockStmt*> queue;  // queue of blocks with PRIM_BLOCK_ATOMIC set

static
bool isOnStack(SymExpr* se) {
  if (se->var->defPoint->parentSymbol == se->parentSymbol)
    return true;
  return false;
}

//
// Based on expr.cpp:codegen_member
//
static 
bool isBaseOnStack(SymExpr* base) {
  ClassType* ct = toClassType(base->typeInfo());
  INT_ASSERT(ct);
  INT_ASSERT(!isUnion(ct));

  if (ct->symbol->hasFlag(FLAG_REF))
    return false;
  if (isClass(ct))
    return false;
  if (!isClass(ct) && isOnStack(base))
    return true;
  return false; 
}

//
// Based on expr.cpp:codegenTuplemember
// 
static
bool isTupleBaseOnStack(SymExpr* base) {
  if (base->typeInfo()->symbol->hasFlag(FLAG_REF))
    return false;
  if (!isOnStack(base))
    return false;
  return true;
}

void txUnknownPrimitive(CallExpr *call) { /* gdb use only */ }

void txUnknownMovePrimitive(CallExpr* call, CallExpr* rhs) {    
  if (rhs->numActuals() >= 1)
    if (SymExpr* se = toSymExpr(rhs->get(1))) {
      if (!isOnStack(se)) 
        USR_WARN(call, "Heap load not instrumented.");     
    }
  if (rhs->numActuals() >= 2) 
    if (SymExpr* se = toSymExpr(rhs->get(2))) {
      if (!isOnStack(se)) 
        USR_WARN(call, "Heap load not instrumented.");
    }
}

static
void handleMemoryOperations(BlockStmt* block, CallExpr* call, Symbol* tx) {
  // For now deal with PRIM_MOVE and not internal primitives directly
  if (CallExpr* parent = toCallExpr(call->parentExpr)) {
    if (parent->primitive && parent->primitive->tag == PRIM_MOVE) 
      return;
  }

  switch (call->primitive->tag) {
  case PRIM_ARRAY_SET:
  case PRIM_ARRAY_SET_FIRST: {
    SymExpr *se1 = toSymExpr(call->get(1));
    SymExpr *se2 = toSymExpr(call->get(2));
    SymExpr *se3 = toSymExpr(call->get(3));
    call->replace(new CallExpr(PRIM_TX_ARRAY_SET,
                               tx, se1->var, se2->var, se3->var));
    break;
  }
  case PRIM_ARRAY_ALLOC: {
    SymExpr *se1 = toSymExpr(call->get(1));
    SymExpr *se2 = toSymExpr(call->get(2));
    SymExpr *se3 = toSymExpr(call->get(3));
    call->replace(new CallExpr(PRIM_TX_ARRAY_ALLOC,
                               tx, se1->var, se2->var, se3->var));
    break;
  }
  case PRIM_ARRAY_FREE: {
    SymExpr *se = toSymExpr(call->get(1));
    call->replace(new CallExpr(PRIM_TX_ARRAY_FREE, tx, se->var));
    break;
  }
  case PRIM_MOVE: {
    SymExpr* lhs = toSymExpr(call->get(1));
    INT_ASSERT(lhs);  
    if (CallExpr* rhs = toCallExpr(call->get(2))) { 
      if (rhs->isPrimitive(PRIM_UNKNOWN)) {
        if (rhs->primitive->isAtomicSafe) {
          if (strstr(rhs->primitive->name, "string_concat")) {
            USR_WARN("Ignoring string_concat primitive, leaks memory");
            break;
          }
        }
        if (!rhs->primitive->isAtomicSafe) {
          if (strstr(rhs->primitive->name, "fprintf")) {
            USR_FATAL("I/O operations are not permitted in atomic.");
          }
        }
      } 
      if (rhs->isPrimitive(PRIM_GET_LOCALEID)) {
        SymExpr *se = toSymExpr(rhs->get(1));
        INT_ASSERT(se);
        if (se->typeInfo()->symbol->hasFlag(FLAG_WIDE)) {
          if (se->getValType()->symbol->hasFlag(FLAG_WIDE_CLASS)) 
            call->replace(new CallExpr(PRIM_TX_GET_LOCALEID, 
                                       tx, lhs->var, se->var));
          else {
            if (!isOnStack(se))
              call->replace(new CallExpr(PRIM_TX_LOAD_LOCALEID, 
                                         tx, lhs->var, se->var));
          }
        } else if (se->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) { 
          if (!isOnStack(se))
            call->replace(new CallExpr(PRIM_TX_LOAD_LOCALEID, 
                                       tx, lhs->var, se->var));
        }
        break;
      }
      if (rhs->isPrimitive(PRIM_GET_REF)) {
        SymExpr* se = toSymExpr(rhs->get(1));
        INT_ASSERT(se);
        if (se->typeInfo()->symbol->hasFlag(FLAG_WIDE) ||
            se->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
          Type* valueType;
          if (se->typeInfo()->symbol->hasFlag(FLAG_WIDE))
            valueType = se->getValType();
          else
            valueType = se->typeInfo()->getField("addr")->type;
          INT_ASSERT(valueType == lhs->typeInfo());
          if (valueType == dtString)
            INT_FATAL(call, "FIXME: string type (FLAG_WIDE GET_REF)");
          else {
            call->replace(new CallExpr(PRIM_TX_GET_REF, 
                                       tx, lhs->var, se->var));
          }
        } else if (se->typeInfo() == dtString ||
		   se->typeInfo()->symbol->hasFlag(FLAG_FIXED_STRING)) {
          INT_FATAL(call, "FIXME: string type (GET_REF)");
        } else {
	  INT_ASSERT(se->typeInfo()->symbol->hasFlag(FLAG_STAR_TUPLE) ||
                     se->typeInfo()->symbol->hasFlag(FLAG_REF));
          call->replace(new CallExpr(PRIM_TX_LOAD_REF, 
				     tx, lhs->var, se->var));
	}
        break;
      } 
      if (rhs->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
        SymExpr* se1 = toSymExpr(rhs->get(1));
        SymExpr* se2 = toSymExpr(rhs->get(2));
        INT_ASSERT(se1);
        INT_ASSERT(se2);
        if (se1->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
          if (se2->var->hasFlag(FLAG_SUPER_CLASS)) 
            INT_FATAL(call, "FIXME: GET_MEMBER_VALUE SUPER_CLASS");
          else 
            call->replace(new CallExpr(PRIM_TX_GET_MEMBER_VALUE,
                                       tx, lhs->var, se1->var, se2->var));
        } else if (se1->typeInfo()->symbol->hasFlag(FLAG_WIDE)) {
          INT_ASSERT(!isUnion(se1->getValType()));
          call->replace(new CallExpr(PRIM_TX_GET_MEMBER_VALUE,
                                     tx, lhs->var, se1->var, se2->var));
        } else if (rhs->typeInfo()->symbol->hasFlag(FLAG_STAR_TUPLE)) {
          if (!isBaseOnStack(se1))
            call->replace(new CallExpr(PRIM_TX_LOAD_MEMBER_VALUE_SVEC, 
                                       tx, lhs->var, se1->var, se2->var));  
        } else {
          if (se2->var->hasFlag(FLAG_SUPER_CLASS) &&
             lhs->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
            // Ignore -- CHPL_WIDEN(lhs, &(se1...))
            break;
          } else if (se2->var->hasFlag(FLAG_SUPER_CLASS)) {
            // Ignore -- lhs = (&(se1...))
            break; 
          } else if (!isBaseOnStack(se1)) {
            call->replace(new CallExpr(PRIM_TX_LOAD_MEMBER_VALUE, 
                                       tx, lhs->var, se1->var, se2->var));
          }
        }
        break;
      }
      if (rhs->isPrimitive(PRIM_GET_MEMBER)) {
        SymExpr* se1 = toSymExpr(rhs->get(1));
        SymExpr* se2 = toSymExpr(rhs->get(2));
        INT_ASSERT(se1);
        INT_ASSERT(se2);
        INT_ASSERT(lhs->getValType() == se2->getValType());
        // Generating the transactional equivalent of GET_MEMBER
        // is only necessary if se1 is on the heap (or) falls under one
        // of the uncommon cases we need to track stack references.
        if (se1->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
          if (!isOnStack(se1)) 
            USR_WARN(call, "Heap get not instrumented (GET_MEMBER)");    
          break;
        } else if (se1->typeInfo()->symbol->hasFlag(FLAG_WIDE)) {
          if (!isOnStack(se1)) 
            USR_WARN(call, "Heap get not instrumented (GET_MEMBER)");
          break;
        } else {
          if (!isOnStack(se1))
            USR_WARN(call, "Heap load not instrumented (GET_MEMBER)"); 
          break;
        }
      }
      if (rhs->isPrimitive(PRIM_GET_SVEC_MEMBER)) {
        SymExpr* se1 = toSymExpr(rhs->get(1));
        SymExpr* se2 = toSymExpr(rhs->get(2));
        INT_ASSERT(se1);
        INT_ASSERT(se2);
        if (se1->typeInfo()->symbol->hasFlag(FLAG_WIDE)) {
          // same reasoning as PRIM_GET_MEMBER
          if (!isOnStack(se1)) 
            USR_WARN(call, "Heap get not instrumented (GET_SVEC_MEMBER)");
          break;
        } else {
          if (!isOnStack(se1))
            USR_WARN(call, "Heap load not instrumented (GET_SVEC_MEMBER)");
          break;
        }
      }
      if (rhs->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE)) {
        SymExpr* se1 = toSymExpr(rhs->get(1));
        SymExpr* se2 = toSymExpr(rhs->get(2));
        INT_ASSERT(se1);
        INT_ASSERT(se2);
        if (se1->typeInfo()->symbol->hasFlag(FLAG_WIDE)) {
          call->replace(new CallExpr(PRIM_TX_GET_SVEC_MEMBER_VALUE,
                                     tx, lhs->var, se1->var, se2->var));
        } else {
          Type* tupleType = se1->getValType();
          INT_ASSERT(lhs->getValType() == rhs->getValType());
          if (!isTupleBaseOnStack(se1)) {
            if (tupleType->getField("x1")->type->symbol->hasFlag(FLAG_STAR_TUPLE))
              call->replace(new CallExpr(PRIM_TX_LOAD_SVEC_MEMBER_VALUE_SVEC, 
                                         tx, lhs->var, se1->var, se2->var));
            else
              call->replace(new CallExpr(PRIM_TX_LOAD_SVEC_MEMBER_VALUE, 
                                         tx, lhs->var, se1->var, se2->var));
          }
        }
        break;
      }
      if (rhs->isPrimitive(PRIM_ARRAY_GET)) {
        SymExpr* se1 = toSymExpr(rhs->get(1));
        SymExpr* se2 = toSymExpr(rhs->get(2));
        INT_ASSERT(se1);
        INT_ASSERT(se2);
        if (se1->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
          INT_ASSERT(lhs->getValType() == rhs->getValType());
          call->replace(new CallExpr(PRIM_TX_ARRAY_GET, tx, 
                                     lhs->var, se1->var, se2->var));
        } else {
          INT_ASSERT(lhs->getValType() == rhs->getValType());
          call->replace(new CallExpr(PRIM_TX_ARRAY_LOAD, tx, 
                                     lhs->var, se1->var, se2->var)); 
        }
        break;
      }
      if (rhs->isPrimitive(PRIM_ARRAY_GET_VALUE)) {
         SymExpr* se1 = toSymExpr(rhs->get(1));
         SymExpr* se2 = toSymExpr(rhs->get(2));
         INT_ASSERT(se1);
         INT_ASSERT(se2);
         if (se1->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
           call->replace(new CallExpr(PRIM_TX_ARRAY_GET_VALUE,
                                      tx, lhs->var, se1->var, se2->var)); 
         } else {
           call->replace(new CallExpr(PRIM_TX_ARRAY_LOAD_VALUE,
                                      tx, lhs->var, se1->var, se2->var)); 
         }
        break;
      }
      if (rhs->isPrimitive(PRIM_TESTCID)) {
        SymExpr* se1 = toSymExpr(rhs->get(1));
        SymExpr* se2 = toSymExpr(rhs->get(2));
        INT_ASSERT(se1);
        INT_ASSERT(se2);
        if (se1->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
          call->replace(new CallExpr(PRIM_TX_GET_TEST_CID, 
                                     tx, lhs->var, se1->var, se2->var));
        } else {
          call->replace(new CallExpr(PRIM_TX_LOAD_TEST_CID, 
                                     tx, lhs->var, se1->var, se2->var));
        }
        break;
      }
      if (rhs->isPrimitive(PRIM_GETCID)) {
	INT_FATAL(call, "FIXME: GETCID primitive");
	break;
      }
      if (rhs->isPrimitive(PRIM_CAST)           ||
          rhs->isPrimitive(PRIM_GET_PRIV_CLASS) ||
          rhs->isPrimitive(PRIM_SET_REF)) {
        // generate CHPL_STM_WIDE_CAST -- same reasoning as 
        // in CHPL_STM_WIDE_GET_FIELD
        break;
      }
      if (rhs->isPrimitive(PRIM_GET_SERIAL)) { 
        // indicates the use of serial clause in atomic
        // thread-local storage so STM not required
        break;
      }
      if (rhs->isPrimitive(PRIM_CHPL_ALLOC)) {
        SymExpr* se1 = toSymExpr(rhs->get(1));
        SymExpr* se2 = toSymExpr(rhs->get(2));
        INT_ASSERT(se1);
        INT_ASSERT(se2);
        INT_ASSERT(toVarSymbol(se2->var));
        const char* memDescStr = memDescsNameMap.get(toVarSymbol(se2->var));
        rhs->replace(new CallExpr(PRIM_TX_CHPL_ALLOC, se1->var, tx, 
                                  newMemDesc(astr("stm ", memDescStr))));
        break;
      }
      if (rhs->isPrimitive(PRIM_CHPL_ALLOC_PERMIT_ZERO)) {
        SymExpr* se1 = toSymExpr(rhs->get(1));
        SymExpr* se2 = toSymExpr(rhs->get(2));
        INT_ASSERT(se1);
        INT_ASSERT(se2);
        INT_ASSERT(toVarSymbol(se2->var));
        const char* memDescStr = memDescsNameMap.get(toVarSymbol(se2->var));
        rhs->replace(new CallExpr(PRIM_TX_CHPL_ALLOC_PERMIT_ZERO, se1->var, 
                                  tx, newMemDesc(astr("stm ", memDescStr))));
        break;
      }
      if (rhs->isPrimitive(PRIM_UNARY_MINUS)    ||
          rhs->isPrimitive(PRIM_UNARY_PLUS)     ||
          rhs->isPrimitive(PRIM_UNARY_NOT)      ||
          rhs->isPrimitive(PRIM_UNARY_LNOT)     ||
          rhs->isPrimitive(PRIM_ADD)            || 
          rhs->isPrimitive(PRIM_SUBTRACT)       ||
          rhs->isPrimitive(PRIM_MULT)           ||
          rhs->isPrimitive(PRIM_DIV)            ||
          rhs->isPrimitive(PRIM_MOD)            ||
          rhs->isPrimitive(PRIM_LSH)            ||
          rhs->isPrimitive(PRIM_RSH)            ||
          rhs->isPrimitive(PRIM_PTR_EQUAL)      ||
          rhs->isPrimitive(PRIM_EQUAL)          ||
          rhs->isPrimitive(PRIM_PTR_NOTEQUAL)   ||
          rhs->isPrimitive(PRIM_NOTEQUAL)       ||
          rhs->isPrimitive(PRIM_LESSOREQUAL)    ||
          rhs->isPrimitive(PRIM_GREATEROREQUAL) ||
          rhs->isPrimitive(PRIM_LESS)           ||
          rhs->isPrimitive(PRIM_GREATER)        ||
          rhs->isPrimitive(PRIM_AND)            ||
          rhs->isPrimitive(PRIM_OR)             ||
          rhs->isPrimitive(PRIM_XOR)            ||
          rhs->isPrimitive(PRIM_POW)            ||
          rhs->isPrimitive(PRIM_MIN)            ||
          rhs->isPrimitive(PRIM_MAX)            ||
          rhs->primitive == NULL) {
        // Operates on stack variables, STM not required
        break;
      } 
      if (rhs->isPrimitive(PRIM_TASK_ID)        ||
          rhs->isPrimitive(PRIM_GET_SERIAL)) {
        // Operates on thread-local storage, STM not required
        break;
      }
      if (rhs->isPrimitive(PRIM_STRING_COPY)) {
        USR_WARN(call, "Ignoring STRING_COPY primitive");
        break;
      }
      if (rhs->isPrimitive(PRIM_SYNC_ISFULL))
	USR_FATAL(call, "Sync operations are not permitted inside atomic transactions.");
      txUnknownMovePrimitive(call, rhs);
    }
    if (lhs->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS) &&
        !call->get(2)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
      INT_ASSERT(isOnStack(lhs)); 
      // Generates CHPL_WIDEN, STM not required
      break;
    }
    if (lhs->typeInfo()->symbol->hasFlag(FLAG_WIDE) &&
        call->get(2)->typeInfo()->symbol->hasFlag(FLAG_REF)) {    
      INT_ASSERT(isOnStack(lhs)); 
      // Generates CHPL_WIDEN, STM not required
      break;
    }
    if (lhs->typeInfo()->symbol->hasFlag(FLAG_WIDE) && 
        !call->get(2)->typeInfo()->symbol->hasFlag(FLAG_WIDE) &&
        !call->get(2)->typeInfo()->symbol->hasFlag(FLAG_REF)) {
      SymExpr* rhs = toSymExpr(call->get(2));
      INT_ASSERT(rhs);
      call->replace(new CallExpr(PRIM_TX_PUT, tx, lhs->var, rhs->var));
      break;
    }
    if (lhs->typeInfo()->symbol->hasFlag(FLAG_REF) &&
        call->get(2)->typeInfo()->symbol->hasFlag(FLAG_WIDE)) {
      INT_ASSERT(isOnStack(lhs)); 
      // Generates CHPL_NARROW, STM not required
      break;
    }
    if (!lhs->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS) &&
        !lhs->typeInfo()->symbol->hasFlag(FLAG_REF) &&
        call->get(2)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
      INT_ASSERT(isOnStack(lhs)); 
      // Generates CHPL_NARROW, STM not required
      break;
    }   
    if (call->get(2)->typeInfo()->symbol->hasFlag(FLAG_STAR_TUPLE)) {
      SymExpr* rhs = toSymExpr(call->get(2));
      INT_ASSERT(rhs && isOnStack(rhs));
      if (lhs->typeInfo()->symbol->hasFlag(FLAG_REF)) {
        call->replace(new CallExpr(PRIM_TX_STORE_REF,
                                   tx, lhs->var, rhs->var));
        break;
      } else {
        if (!isOnStack(lhs)) {
          call->replace(new CallExpr(PRIM_TX_STORE, 
                                     tx, lhs->var, rhs->var));
          break;
        }
      } 
    }
    if (lhs->typeInfo()->symbol->hasFlag(FLAG_REF) && 
        !call->get(2)->typeInfo()->symbol->hasFlag(FLAG_REF)) {
      SymExpr* rhs = toSymExpr(call->get(2));
      INT_ASSERT(rhs && isOnStack(rhs));
      call->replace(new CallExpr(PRIM_TX_STORE_REF, tx, lhs->var, rhs->var));
      break;
    } else {
      SymExpr* rhs = toSymExpr(call->get(2));
      INT_ASSERT(rhs);
      if (!isOnStack(lhs)) 
        call->replace(new CallExpr(PRIM_TX_STORE, tx, lhs->var, rhs->var));
      break;
    }
    txUnknownPrimitive(call);    
  }
  case PRIM_ARRAY_GET:
    // access to 1D arrays without the enclosing PRIM_MOVE can be
    // ignored since they don't have any side-effects. Ideally,
    // dead-code elimintation should handle this case.
    break;
  case PRIM_RETURN:
    // FIXME: make sure we are doing transactional scoping right
    //    call->insertBefore(new CallExpr(PRIM_TX_COMMIT, tx));
    break;
  case PRIM_SETCID: {
    SymExpr* se = toSymExpr(call->get(1));
    if (se->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) 
      call->replace(new CallExpr(PRIM_TX_SET_CID, tx, se->var));
    else {
      if (!isOnStack(se)) 
        call->replace(new CallExpr(PRIM_TX_STORE_CID, tx, se->var));
    }
    break;
  }
  case PRIM_SET_SVEC_MEMBER: {
    SymExpr* se1 = toSymExpr(call->get(1));
    SymExpr* se2 = toSymExpr(call->get(2));
    SymExpr* se3 = toSymExpr(call->get(3));
    if (se1->typeInfo()->symbol->hasFlag(FLAG_WIDE))
      call->replace(new CallExpr(PRIM_TX_SET_SVEC_MEMBER,
                                 tx, se1->var, se2->var, se3->var));
    else {
      //      if (!isTupleBaseOnStack(se1)) {
      if (!isOnStack(se1)) {
        call->replace(new CallExpr(PRIM_TX_STORE_SVEC_MEMBER,
                                   tx, se1->var, se2->var, se3->var));
      }
    }
    break;
  }
  case PRIM_GET_MEMBER:
  case PRIM_GET_SVEC_MEMBER:
    break;
  case PRIM_SET_MEMBER: {
    SymExpr* se1 = toSymExpr(call->get(1));
    SymExpr* se2 = toSymExpr(call->get(2));
    SymExpr* se3 = toSymExpr(call->get(3));
    INT_ASSERT(se1);
    INT_ASSERT(se2);
    INT_ASSERT(se3);
    if (se1->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS) ||
        se1->typeInfo()->symbol->hasFlag(FLAG_WIDE)) {    
      call->replace(new CallExpr(PRIM_TX_SET_MEMBER,
                                 tx, se1->var, se2->var, se3->var));      
    } else {
      //      if (!isBaseOnStack(se1)) { 
      if (!isOnStack(se1)) {
        call->replace(new CallExpr(PRIM_TX_STORE_MEMBER,
                                   tx, se1->var, se2->var, se3->var));
      }
    }
    break;
  }
  case PRIM_CHECK_NIL:
  case PRIM_LOCAL_CHECK:
    break;
  case PRIM_SYNC_DESTROY:
  case PRIM_SYNC_INIT:
  case PRIM_SYNC_WAIT_FULL:
  case PRIM_SYNC_SIGNAL_EMPTY:
  case PRIM_SYNC_WAIT_EMPTY:
  case PRIM_SYNC_SIGNAL_FULL:
  case PRIM_SYNC_LOCK:
  case PRIM_SYNC_UNLOCK:
  case PRIM_SYNC_ISFULL:
    USR_FATAL(call, "Sync operations are not permitted inside atomic transactions.");
    break;
  case PRIM_PROCESS_TASK_LIST:
    INT_FATAL(call, "FIXME: Ignoring PROCESS_TASK_LIST primitive");
    break;
  case PRIM_EXECUTE_TASKS_IN_LIST:
    INT_FATAL(call, "FIXME: EXECUTE_TASKS_IN_LIST primitive");
    break;
  case PRIM_FREE_TASK_LIST:
    INT_FATAL(call, "FIXME: FREE_TASK_LIST primitive");
    break;
  case PRIM_VMT_CALL: 
    INT_FATAL(call, "FIXME: VMT_CALL primitive");
    break;
  case PRIM_SET_SERIAL:
    // Writing thread-local storage, STM not required
    break;
  case PRIM_CHPL_FREE: {
    SymExpr* se = toSymExpr(call->get(1));
    INT_ASSERT(se);
    call->replace(new CallExpr(PRIM_TX_CHPL_FREE, tx, se->var));
    break;
  }
  case PRIM_CAST:
    break;
  case PRIM_BLOCK_LOCAL:
    break;
  case PRIM_BLOCK_ATOMIC:
    // In original functions that have an atomic block: This has the
    // straight forward effect of simply skipping the primitive. 
    // In transactional clones that have an atomic block: We simply
    // chose to ignore this primitive completeley since the whole
    // function body has to be processed. In other words, when we
    // choose to process cloned functions, we add its entire function
    // body to the queue and not just the atomic blocks within the clone
  case PRIM_BLOCK_WHILEDO_LOOP:
  case PRIM_BLOCK_FOR_LOOP:
  case PRIM_RT_ERROR:
    break;
  default:
    txUnknownPrimitive(call);
  }  
}

// create a clone copy of the function fn
// prefix _tx_clone_ to both its name and cname
// add tx descriptor to the head of the formal argument list
static 
FnSymbol* createTxFnClone(FnSymbol* fn) {
  FnSymbol* fnTxClone = fn->copy();
  fnTxClone->name = astr("__tx_clone_", fn->name);
  fnTxClone->cname = astr("__tx_clone_", fn->cname);
  ArgSymbol* formalTxDesc = new ArgSymbol(INTENT_BLANK, "tx", dtTransaction);
  fnTxClone->insertFormalAtHead(formalTxDesc);
  return fnTxClone;
}

static
bool isBadFunction(FnSymbol* fn) {
  if (strstr(fn->name, "waitEndCount") || 
      strstr(fn->name, "readFE")) {
    gdbShouldBreakHere();
    return true;
  }

  if (strstr(fn->name, "halt") || 
      strstr(fn->name, "compilerWarning")) 
    return true;
  
  if (strstr(fn->name, "writeln") || strcmp(fn->name, "write") == 0)
    USR_FATAL("I/O operations are not permitted inside atomic transactions.");

  return false;
} 

static
void handleFunctionCalls(BlockStmt* block, CallExpr* call, FnSymbol* fn, Symbol* tx) {
  FnSymbol* fnTxClone = fnCache.get(fn);
  
  // create clone if one doesn't already exist
  if (!fnTxClone) {
    INT_ASSERT(!strstr(fn->name, "__tx_clone_"));
    fnTxClone = createTxFnClone(fn);
    fnCache.put(fn, fnTxClone);
    fnCache.put(fnTxClone, fnTxClone);
  }
   
  if (fn->hasFlag(FLAG_BEGIN_BLOCK)) {
    USR_FATAL("begin statements are not permitted inside atomic transactions.");
  } else if (fn->hasFlag(FLAG_COBEGIN_OR_COFORALL_BLOCK)) {
    USR_FATAL("cobegin/coforall statements are not permitted inside atomic transactions.");
  } else if (fn->hasFlag(FLAG_ON_BLOCK)) {
    fnTxClone->removeFlag(FLAG_ON_BLOCK);
    fnTxClone->addFlag(FLAG_TX_ON_BLOCK);
  }

  // add clone to queue and insert fnTxClone's function definition
  // this case is required to deal with functions that were cloned
  // but did not get added to the queue since we did not have enough 
  // information at that time to even determine in such a clone will 
  // actually be required. 
  if (!queue.in(fnTxClone->body)) { 
    queue.add(fnTxClone->body);
    fn->defPoint->insertBefore(new DefExpr(fnTxClone));
  }

  // replace the function call at the callsite 
  call->baseExpr->replace(new SymExpr(fnTxClone));
  // insert 'tx' into actual argument list
  call->insertAtHead(tx);
}

void
insertTransactions(void) {
  Symbol* tx;
  Symbol* env;

  // collect all explicitly marked atomic blocks
  forv_Vec(BlockStmt, block, gBlockStmts) 
    if (block->parentSymbol &&
        block->blockInfo &&
        block->blockInfo->isPrimitive(PRIM_BLOCK_ATOMIC)) 
      queue.add(block);

  // iteratively process each block in the queue
  forv_Vec(BlockStmt, block, queue) {
    Vec<CallExpr*> calls;
    collectCallExprs(block, calls);  
 
    FnSymbol* fn = block->getFunction();
    INT_ASSERT(fn);
    // USR_PRINT("Processing function %s %p", fn->name, fn); 

    if (!strstr(fn->name, "__tx_clone_")) {
      // fn is a regular function, i.e. has an atomic block. however,
      // we create a clone of fn as a precaution in case it may be 
      // called from inside an atomic block. For clone functions,
      // we need to process the entire function body, but for
      // regular functions we only need to process the atomic block
      INT_ASSERT(block->blockInfo->isPrimitive(PRIM_BLOCK_ATOMIC));
      FnSymbol* fnTxClone = fnCache.get(fn);
      if (!fnTxClone) {
        // USR_PRINT("Creating clone for function %s %p", fn->name, fn);
        fnTxClone = createTxFnClone(fn);
        fnCache.put(fn, fnTxClone);
        fnCache.put(fnTxClone, fnTxClone);
      }

      // For regular functions, we need to create the tx descriptor
      // and tx environment variable that holds the state. We also
      // insert TX_BEGIN and TX_COMMIT calls to mark each atomic block
      tx = newTemp("tx", dtTransaction);
      block->insertAtHead(new DefExpr(tx));
      env = newTemp("local_env", dtTxEnv);
      block->insertAtHead(new DefExpr(env));
      block->insertAtHead(new CallExpr(PRIM_TX_BEGIN, tx, env));
      block->insertAtTailBeforeGoto(new CallExpr(PRIM_TX_COMMIT, tx));
    } else {
      // fn is already a transactional clone, get the tx descriptor 
      // from its formal arg list. Note, we will be processing
      // the entire function body for cloned functions.
      tx = fn->getFormal(1); 
      // USR_PRINT("Getting tx arg from definition %s %p", fn->cname, fn); 
    }  

    forv_Vec(CallExpr, call, calls) {
      if (call->primitive) {
        if (!(strstr(fn->name, "tx_clone_wrapon"))) 
          handleMemoryOperations(block, call, tx);
        continue;
      }
      FnSymbol* cloneFn = call->isResolved();
      INT_ASSERT(cloneFn);
      if (!isBadFunction(cloneFn)) {
        handleFunctionCalls(block, call, cloneFn, tx);
      }
    }
  }

  // TODO: Remove functions that are never invoked directly but only
  // through their clones

  // remove all explicitly marked atomic blocks
  forv_Vec(BlockStmt, block, gBlockStmts) 
    if (block->parentSymbol &&
        block->blockInfo &&
        block->blockInfo->isPrimitive(PRIM_BLOCK_ATOMIC)) 
      block->blockInfo->remove();
}
