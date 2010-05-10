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
  if(se->var->defPoint->parentSymbol == se->parentSymbol)
    return 1;
    // &&  !se->var->hasFlag(FLAG_CONST)) 
  return 0;
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
	  if (strstr(rhs->primitive->name, "string_concat")) 
	    break;
	}
	if (!rhs->primitive->isAtomicSafe) {
	  if (strstr(rhs->primitive->name, "fprintf")) {
	    if (strstr(rhs->getFunction()->name, "tx_clone_halt"))
	      break;
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
	  else
	    call->replace(new CallExpr(PRIM_TX_LOAD_LOCALEID, 
				       tx, lhs->var, se->var));
	} else if (se->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) { 
	  call->replace(new CallExpr(PRIM_TX_GET_LOCALEID, 
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
	  if (valueType == dtString) {
	    USR_WARN(call, "FIXME: string type (FLAG_WIDE GET_REF)");
	  } else {
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
	} else {
	  if(se2->var->hasFlag(FLAG_SUPER_CLASS) &&
	     lhs->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
	    VarSymbol *tmp = newTemp(se2->typeInfo());
	    call->insertBefore(new DefExpr(tmp));
	    call->insertBefore(new CallExpr(PRIM_TX_LOAD_MEMBER_VALUE,
					    tx, tmp, se1->var, se2->var));
	    rhs->replace(new SymExpr(tmp)); 
	  } else {
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
  
	  // generate CHPL_STM_WIDE_GET_FIELD -- requires STM tracking if 
	  // (A) its on the heap (B) its on the stack and passed around.
	  // currently assuming its on the stack and not passed around. 
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
	  // generate CHPL_STM_WIDE_GET_TUPLE_COMPONENT
	  // same reasoning as in CHPL_STM_WIDE_GET_FIELD
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
	  break;
	  USR_FATAL(call, "FIXME: GET_SVEC_MEMBER_VALUE FLAG_WIDE");
	} else {
	  INT_ASSERT(lhs->getValType() == rhs->getValType());
	  if (!isOnStack(se1))
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
	rhs->replace(new CallExpr(PRIM_TX_CHPL_ALLOC_PERMIT_ZERO, 
				  se1->var, tx,
				  newMemDesc(astr("stm ", memDescStr))));
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
	  rhs->isPrimitive(PRIM_TASK_ID)        ||
	  rhs->isPrimitive(PRIM_STRING_COPY)    ||
	  rhs->primitive == NULL) {
        break;
      } 
      // if (rhs->isPrimitive(PRIM_SYNC_ISFULL)) {
      // 	call->remove();
      // 	break;
      // }
      txUnknownMovePrimitive(call, rhs);
    }
    if (lhs->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS) &&
	!call->get(2)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
      INT_ASSERT(call->get(2)->typeInfo() != dtString); 
      if (!isOnStack(lhs)) 
	USR_WARN(call, "Write case 1 not instrumented");
      break;
    }
    if (lhs->typeInfo()->symbol->hasFlag(FLAG_WIDE) &&
	call->get(2)->typeInfo()->symbol->hasFlag(FLAG_REF)) {
      if (!isOnStack(lhs)) 
	USR_WARN(call, "Write case 2 not instrumented");
      break;
    }
    if (lhs->typeInfo()->symbol->hasFlag(FLAG_WIDE) && 
	!call->get(2)->typeInfo()->symbol->hasFlag(FLAG_WIDE) &&
	!call->get(2)->typeInfo()->symbol->hasFlag(FLAG_REF)) {
      SymExpr* rhs = toSymExpr(call->get(2));
      INT_ASSERT(rhs);
      INT_ASSERT(isOnStack(rhs));
      call->replace(new CallExpr(PRIM_TX_PUT, tx, lhs->var, rhs->var));
      break;
    }
    if (lhs->typeInfo()->symbol->hasFlag(FLAG_REF) &&
	call->get(2)->typeInfo()->symbol->hasFlag(FLAG_WIDE)) {
      if (!isOnStack(lhs)) 
	USR_WARN(call, "Write case 4 not instrumented");
      break;
    }
    if (!lhs->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS) &&
	!lhs->typeInfo()->symbol->hasFlag(FLAG_REF) &&
	call->get(2)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
      if (!isOnStack(lhs)) 
	USR_WARN(call, "Write case 5 not instrumented");
      break;
    }   
    if (call->get(2)->typeInfo()->symbol->hasFlag(FLAG_STAR_TUPLE)) {
      if (call->get(1)->typeInfo()->symbol->hasFlag(FLAG_REF)) {
	SymExpr* rhs = toSymExpr(call->get(2));
	INT_ASSERT(rhs);
	INT_ASSERT(isOnStack(rhs));
	call->replace(new CallExpr(PRIM_TX_STORE_REF, tx, lhs->var, rhs->var));
	break;
      } else {
	USR_FATAL(call, "STAR TUPLE in store case");
	SymExpr* rhs = toSymExpr(call->get(2));
	INT_ASSERT(rhs);
	if (isOnStack(rhs) && !isOnStack(lhs)) {
	  call->replace(new CallExpr(PRIM_TX_STORE, tx, lhs->var, rhs->var));
	}
      }
    } 
    if (lhs->typeInfo()->symbol->hasFlag(FLAG_REF) && 
	!call->get(2)->typeInfo()->symbol->hasFlag(FLAG_REF)) {
      SymExpr* rhs = toSymExpr(call->get(2));
      INT_ASSERT(rhs);
      INT_ASSERT(isOnStack(rhs));
      call->replace(new CallExpr(PRIM_TX_STORE_REF, tx, lhs->var, rhs->var));
      break;
    } else {
      SymExpr* rhs = toSymExpr(call->get(2));
      INT_ASSERT(rhs);
      if (isOnStack(rhs) && !isOnStack(lhs)) 
	call->replace(new CallExpr(PRIM_TX_STORE, tx, lhs->var, rhs->var));
    }
    break;
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
    else 
      call->replace(new CallExpr(PRIM_TX_STORE_CID, tx, se->var));
    break;
  }
  case PRIM_SET_SVEC_MEMBER: {
    SymExpr* se1 = toSymExpr(call->get(1));
    SymExpr* se2 = toSymExpr(call->get(2));
    SymExpr* se3 = toSymExpr(call->get(3));
    if (se1->typeInfo()->symbol->hasFlag(FLAG_WIDE))
      call->replace(new CallExpr(PRIM_TX_SET_SVEC_MEMBER,
				 tx, se1->var, se2->var, se3->var));
    else 
      call->replace(new CallExpr(PRIM_TX_STORE_SVEC_MEMBER,
				 tx, se1->var, se2->var, se3->var));
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
      call->replace(new CallExpr(PRIM_TX_STORE_MEMBER,
				 tx, se1->var, se2->var, se3->var));
    }
    break;
  }
  case PRIM_CHECK_NIL:
  case PRIM_LOCAL_CHECK:
    break;
  case PRIM_SYNC_INIT:
  case PRIM_SYNC_DESTROY:
  case PRIM_SYNC_LOCK:
  case PRIM_SYNC_UNLOCK:
  case PRIM_SYNC_WAIT_FULL:
  case PRIM_SYNC_WAIT_EMPTY:
  case PRIM_SYNC_SIGNAL_FULL:
  case PRIM_SYNC_SIGNAL_EMPTY:
  case PRIM_SYNC_ISFULL:
    //    call->remove();
    break;
  case PRIM_CHPL_FREE: {
    SymExpr* se = toSymExpr(call->get(1));
    INT_ASSERT(se);
    call->replace(new CallExpr(PRIM_TX_CHPL_FREE, tx, se->var));
    break;
  }
  case PRIM_CAST:
  case PRIM_BLOCK_LOCAL:
  case PRIM_BLOCK_ATOMIC:
    break;
  case PRIM_BLOCK_WHILEDO_LOOP:
  case PRIM_BLOCK_FOR_LOOP:
    break;
  case PRIM_RT_ERROR:
    call->replace(new CallExpr(PRIM_TX_RT_ERROR));
    break;
  case PRIM_TX_RT_ERROR:
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

static void
insertSTMCalls() {
  Symbol* tx;
  Symbol* env;
  forv_Vec(BlockStmt, block, queue) {
    Vec<CallExpr*> calls;
    collectCallExprs(block, calls);  
 
    if (block->blockInfo && block->blockInfo->isPrimitive(PRIM_BLOCK_ATOMIC)) {
      if (FnSymbol* fn = block->getFunction()) {
	if (strstr(fn->name, "__tx_clone_")) {
	  // fn is a clone, get the tx descriptor from its formal arg list.
	  // Type I functions will appear here since they don't get added 
	  // to the queue until they change to Type III. 
	  INT_ASSERT(queue.in(fn->body));
	  tx = fn->getFormal(1); 
	} else {
	  // fn most probably might be just a Type I function but we are 
	  // being careful and treating it as a Type III function. 
	  // since fn doesn't have a clone yet, so create one and cache it. 
	  // don't insert it into the IR yet. 
	  // also create a new transaction descriptor for this atomic block  
	  FnSymbol* fnTxClone = createTxFnClone(fn);
	  fnCache.put(fn, fnTxClone); 
	  tx = newTemp("tx", dtTransaction);
	  block->insertAtHead(new DefExpr(tx));
	  env = newTemp("local_env", dtTxEnv);
	  block->insertAtHead(new DefExpr(env));
	} 
      } else {
	USR_FATAL(block, "Cannot deal with indirect or external functions");
      }
    }

    if (block->blockInfo && block->blockInfo->isPrimitive(PRIM_BLOCK_ATOMIC)) {
      block->insertAtHead(new CallExpr(PRIM_TX_BEGIN, tx, env));
      block->insertAtTailBeforeGoto(new CallExpr(PRIM_TX_COMMIT, tx));
    }

    forv_Vec(CallExpr, call, calls) {
      if (call->primitive) {
	if (CallExpr* parent = toCallExpr(call->parentExpr)) {
	  if (parent->primitive && parent->primitive->tag == PRIM_MOVE) 
	    continue;
	} else {
	  handleMemoryOperations(block, call, tx);
	  continue;
	}
      } else if (FnSymbol* fn = call->isResolved()) {
	INT_ASSERT(fn);
	
	FnSymbol* fnTxClone = fnCache.get(fn);
	
	if (!fnTxClone) {
	  // create clone only if fn is already not a clone 
	  INT_ASSERT(!strstr(fn->name, "__tx_clone_"));
	  fnTxClone = createTxFnClone(fn);
	  fnCache.put(fn, fnTxClone);
	  fnCache.put(fnTxClone, fnTxClone);
	}
	
	if(fnTxClone->hasFlag(FLAG_NO_ATOMIC_CLONE)) {
	  // function has pragma "no atomic clone" set
	} 

	// in some cases, we might have created a clone, but not yet inserted
	// the clone for further inspection. That is because we are treating
	// ALL Type I functions as Type III functions. So we don't want to 
	// inspect some of those functions that are truly Type I. 
	if (!queue.in(fnTxClone->body)) {
	    // && !(strstr(fnTxClone->name, "tx_clone_halt") ||
	    //   strstr(fnTxClone->name, "tx_clone_readFE") ||
	    //   strstr(fnTxClone->name, "_syncvar") ||
	    //   strstr(fnTxClone->name, "tx_clone_writeEF") ||
	    //   strstr(fnTxClone->name, "tx_clone_initialize") ||
	    //   strstr(fnTxClone->name, "tx_clone_isFull") ||
	    //   strstr(fnTxClone->name, "tx_clone_readXX") ||
	    //   strstr(fnTxClone->name, "tx_clone_halt") ||
	    //   strstr(fnTxClone->name, "tx_clone_writeIt"))) {
	  queue.add(fnTxClone->body);
	  // insert fnTxClone's function definition
	  fn->defPoint->insertBefore(new DefExpr(fnTxClone));
	}
	
	// replace the function call at the callsite 
	call->baseExpr->replace(new SymExpr(fnTxClone));
	// insert 'tx' into actual argument list
	call->insertAtHead(tx);
      } else
	USR_FATAL(call, "Not a primitve, not a function");
    }
  }
}

void
insertTransactions(void) {
  // collect all explicitly marked atomic blocks
  forv_Vec(BlockStmt, block, gBlockStmts) {
    if (block->parentSymbol)
      if (block->blockInfo)
	if (block->blockInfo->isPrimitive(PRIM_BLOCK_ATOMIC)) {
	  queue.add(block);
	}
  }

  insertSTMCalls();

  // FIXME: Can we remove this earlier ? 
  // cannot do it when collecting atomic blocks above because
  // cloned version of the function must have the this attribute
  // we cannot remove it after we create a clone because not all functions
  // will have a clone. what about functions that don't have clones ?
  // is there any reason to just leave it as it is ? 
  forv_Vec(BlockStmt, block, gBlockStmts) {
    if (block->parentSymbol)
      if (block->blockInfo) 
	if (block->blockInfo->isPrimitive(PRIM_BLOCK_ATOMIC))
	  block->blockInfo->remove();
  }
}
