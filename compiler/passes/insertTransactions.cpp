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

static Map<FnSymbol*,FnSymbol*> fnCache; // cache of cloned stm functions
static Vec<BlockStmt*> queue;  // queue of blocks with PRIM_BLOCK_ATOMIC set

static
bool isOnStack(SymExpr* se) {
  if (se->var->defPoint->parentSymbol == se->parentSymbol)
    return 1;
  return 0;
}

// static
// bool isFormalArg(SymExpr* se) {
//   FnSymbol *fn = se->getFunction();
//   int i; 
//   for (i = 1; i <= fn->numFormals(); i++) {
//     if (toVarSymbol(se->var) == toVarSymbol(fn->getFormal(i))) 
//       return 1;
//   }
//   return 0;
// }

// static
// bool isConst(SymExpr* se) {
//   if (se->var->hasFlag(FLAG_CONST)) return 1;
//   return 0;
// }


//
// Based on codegen_member in expr.cpp
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
// Based on codegenTuplemember in expr.cpp
// 
static
bool isTupleBaseOnStack(SymExpr* base) {
  if (base->typeInfo()->symbol->hasFlag(FLAG_REF)) {
    USR_WARN(base, "TupleBase has FLAG_REF");
    return false;
  }
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
	    USR_WARN(call, "FIXME: string type (FLAG_WIDE GET_REF)");
	  else {
	    call->replace(new CallExpr(PRIM_TX_GET_REF, 
				       tx, lhs->var, se->var));
	  }
	} else if (se->typeInfo() == dtString) {
	  USR_WARN(call, "FIXME: string type (GET_REF)");
	} else {
	  INT_ASSERT(se->typeInfo()->symbol->hasFlag(FLAG_STAR_TUPLE) ||
		     se->typeInfo()->symbol->hasFlag(FLAG_REF));
	  call->replace(new CallExpr(PRIM_TX_LOAD_REF, tx, lhs->var, se->var));
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
	    USR_FATAL(call, "FIXME: GET_MEMBER_VALUE SUPER_CLASS");
	  else 
	    call->replace(new CallExpr(PRIM_TX_GET_MEMBER_VALUE,
				       tx, lhs->var, se1->var, se2->var));
	} else if (se1->typeInfo()->symbol->hasFlag(FLAG_WIDE)) {
	  INT_ASSERT(!isUnion(se1->getValType()));
	  call->replace(new CallExpr(PRIM_TX_GET_MEMBER_VALUE,
				     tx, lhs->var, se1->var, se2->var));
	} else if (se1->typeInfo()->symbol->hasFlag(FLAG_STAR_TUPLE)) {
	  USR_FATAL(call, "FIXME: GET_MEMBER_VALUE STAR_TUPLE");	  
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
	if (se1->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
	  // Generating the transactional equivalent of CHPL_WIDE_GET_FIELD
	  // is only necessary if se1 is on the heap (or) falls under one
	  // of the uncommon cases we need to track stack references.
	  if (!isOnStack(se1)) 
	    USR_WARN(call, "Heap load not instrumented (GET_MEMBER)");    
	  break;
	} else if (se1->typeInfo()->symbol->hasFlag(FLAG_WIDE)) {
	  if (!isOnStack(se1)) 
	    USR_WARN(call, "Heap load not instrumented (GET_MEMBER)");
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
	    USR_WARN(call, "Heap load not being tracked (GET_SVEC_MEMBER)");
	  break;
	} else {
	  if (!isOnStack(se1))
	    USR_WARN(call, "Heap load not being tracked (GET_SVEC_MEMBER)");
	  break;
	}
      }
      if (rhs->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE)) {
	SymExpr* se1 = toSymExpr(rhs->get(1));
	SymExpr* se2 = toSymExpr(rhs->get(2));
	INT_ASSERT(se1);
	INT_ASSERT(se2);
	if (se1->typeInfo()->symbol->hasFlag(FLAG_WIDE)) {
	  USR_FATAL(call, "FIXME: GET_SVEC_MEMBER_VALUE FLAG_WIDE");
	} else {
	  INT_ASSERT(lhs->getValType() == rhs->getValType());
	  if (!isTupleBaseOnStack(se1))
	    call->replace(new CallExpr(PRIM_TX_LOAD_SVEC_MEMBER_VALUE, 
				       tx, lhs->var, se1->var, se2->var));
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
      if (rhs->isPrimitive(PRIM_CAST)           ||
          rhs->isPrimitive(PRIM_GET_PRIV_CLASS) ||
	  rhs->isPrimitive(PRIM_SET_REF)) {
        // generate CHPL_STM_WIDE_CAST -- same reasoning as 
        // in CHPL_STM_WIDE_GET_FIELD
        break;
      }
      if (rhs->isPrimitive(PRIM_GET_SERIAL)) { 
	// indicates the use of serial clause in atomic
	// thread-local storage so no STM instrumentation required
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
	// Operates on stack variables, no STM instrumentation required
        break;
      } 
      if (rhs->isPrimitive(PRIM_TASK_ID)        ||
	  rhs->isPrimitive(PRIM_GET_SERIAL)) {
	// Reads thread-local storage, no STM instrumentation required
	break;
      }
      if (rhs->isPrimitive(PRIM_STRING_COPY)) {
	USR_WARN(call, "Ignoring STRING_COPY primitive");
	break;
      }
      if (rhs->isPrimitive(PRIM_CHPL_NUMRUNNINGTASKS)) {
	USR_WARN(call, "Ignoring CHPL_NUM_RUNNINGTASKS primitive");
	break;
      }
      if (rhs->isPrimitive(PRIM_SYNC_ISFULL)) {
	USR_WARN(call, "Ignoring PRIM_SYNC_ISFULL primitive");
      	break;
      }
      txUnknownMovePrimitive(call, rhs);
    }
    if (lhs->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS) &&
	!call->get(2)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
      INT_ASSERT(isOnStack(lhs)); 
      // Generates CHPL_WIDEN, no STM instrumentation required
      break;
    }
    if (lhs->typeInfo()->symbol->hasFlag(FLAG_WIDE) &&
	call->get(2)->typeInfo()->symbol->hasFlag(FLAG_REF)) {    
      INT_ASSERT(isOnStack(lhs)); 
      // Generates CHPL_WIDEN, no STM instrumentation required
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
      // Generates CHPL_NARROW, no STM instrumentation required
      break;
    }
    if (!lhs->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS) &&
	!lhs->typeInfo()->symbol->hasFlag(FLAG_REF) &&
	call->get(2)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
      INT_ASSERT(isOnStack(lhs)); 
      // Generates CHPL_NARROW, no STM instrumentation required
      break;
    }   
    if (call->get(2)->typeInfo()->symbol->hasFlag(FLAG_STAR_TUPLE)) {
      SymExpr* rhs = toSymExpr(call->get(2));
      INT_ASSERT(rhs && isOnStack(rhs));
      if (lhs->typeInfo()->symbol->hasFlag(FLAG_REF)) {
	USR_WARN(call, "Instrumenting STORE_REF");
	call->replace(new CallExpr(PRIM_TX_STORE_REF,
				   tx, lhs->var, rhs->var));
	break;
      } else 
	USR_FATAL(call, "STAR TUPLE in store case");
    } 
    if (lhs->typeInfo()->symbol->hasFlag(FLAG_REF) && 
	!call->get(2)->typeInfo()->symbol->hasFlag(FLAG_REF)) {
      SymExpr* rhs = toSymExpr(call->get(2));
      INT_ASSERT(rhs && isOnStack(rhs));
      USR_WARN(call, "Instrumenting STORE_REF");
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
    USR_FATAL(call, "Sync operations are not permitted in atomic.");
    break;
  case PRIM_PROCESS_TASK_LIST:
    USR_WARN("Ignoring PROCESS_TASK_LIST primitive");
    break;
  case PRIM_EXECUTE_TASKS_IN_LIST:
    USR_WARN("Ignoring EXECUTE_TASKS_IN_LIST primitive");
    break;
  case PRIM_FREE_TASK_LIST:
    USR_WARN("Ignoring FREE_TASK_LIST primitive");
    break;
  case PRIM_SET_SERIAL:
    // Writing thread-local storage, no STM instrumentation required
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
static FnSymbol*
createTxFnClone(FnSymbol* fn) {
  FnSymbol* fnTxClone = fn->copy();
  fnTxClone->name = astr("__tx_clone_", fn->name);
  fnTxClone->cname = astr("__tx_clone_", fn->cname);
  ArgSymbol* formalTxDesc = new ArgSymbol(INTENT_BLANK, "tx", dtTransaction);
  fnTxClone->insertFormalAtHead(formalTxDesc);
  return fnTxClone;
}

void handleFunctionCalls(BlockStmt* block, CallExpr* call, FnSymbol* fn, Symbol* tx) {

  if (strstr(fn->name, "halt")) return;
  if (strstr(fn->name, "compilerWarning")) return;
  
  if (strstr(fn->name, "__tx_clone")) {
    USR_FATAL("Recursive cloning of cloned function %s", fn->name, fn);
  }

  FnSymbol* fnTxClone = fnCache.get(fn);
  
  // create clone if one doesn't already exist
  if (!fnTxClone) {
    INT_ASSERT(!strstr(fn->name, "__tx_clone_"));
    fnTxClone = createTxFnClone(fn);
    fnCache.put(fn, fnTxClone);
    fnCache.put(fnTxClone, fnTxClone);
  }
   
  if (fn->hasFlag(FLAG_BEGIN_BLOCK)) {
    USR_FATAL("begin statements not permitted in atomic.");
  } else if (fn->hasFlag(FLAG_COBEGIN_OR_COFORALL_BLOCK)) {
    USR_FATAL("cobegin/coforall statements not permitted in atomic.");
  } else if (fn->hasFlag(FLAG_ON_BLOCK)) {
    fnTxClone->removeFlag(FLAG_ON_BLOCK);
    fnTxClone->addFlag(FLAG_TX_ON_BLOCK);
  } 

  // add clone to queue and insert fnTxClone's function definition
  // this case is required to deal with functions that were cloned in
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

    if (strstr(fn->name, "__tx_clone_")) {
      // fn is already a transactional clone, get the tx descriptor 
      // from its formal arg list. Note, we will be processing
      // the entire function body for cloned functions.
      tx = fn->getFormal(1); 
    } else {
      // fn may be cloned later, but for now we create a clone so 
      // that we have a "fresh" version of the function. We process
      // function clones later as and when they are called. Note,
      // we will be processing only atomic blocks within regular fns.
      INT_ASSERT(block->blockInfo->isPrimitive(PRIM_BLOCK_ATOMIC));
      // USR_PRINT("Creating clone for function %s %p", fn->name, fn);
      FnSymbol* fnTxClone = fnCache.get(fn);
      if (!fnTxClone) {
	fnTxClone = createTxFnClone(fn);
	fnCache.put(fn, fnTxClone);
	fnCache.put(fnTxClone, fnTxClone);
      }
      tx = newTemp("tx", dtTransaction);
      block->insertAtHead(new DefExpr(tx));
      env = newTemp("local_env", dtTxEnv);
      block->insertAtHead(new DefExpr(env));
      block->insertAtHead(new CallExpr(PRIM_TX_BEGIN, tx, env));
      block->insertAtTailBeforeGoto(new CallExpr(PRIM_TX_COMMIT, tx));
    }  

    forv_Vec(CallExpr, call, calls) {
      if (call->primitive) {
	handleMemoryOperations(block, call, tx);
	continue;
      }
      FnSymbol* cloneFn = call->isResolved();
      INT_ASSERT(cloneFn);
      if (strstr(cloneFn->name, "waitEndCount")) 
	gdbShouldBreakHere();
      if (strstr(cloneFn->name, "readFE")) 
	gdbShouldBreakHere();
      handleFunctionCalls(block, call, cloneFn, tx);
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
