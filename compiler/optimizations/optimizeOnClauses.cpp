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

//
// Mark On statements/blocks as "fast" (no communication, small, etc.)
//
// The comm layer can provide a "fast" option, for example, run within
//  the handler (rather than creating a new task).
//

#include <vector>
#include "stlUtil.h"
#include "astutil.h"
#include "expr.h"
#include "stmt.h"
#include "passes.h"

/* Print lots of debugging messages when DEBUG is defined */
#ifdef DEBUG
  #define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
  #define DEBUG_PRINTF(...)
#endif

//
// Return true if this primitive function is safe for fast on optimization
// (e.g., no communication, no sync/single accesses)
//
static bool
isFastPrimitive(CallExpr *call, bool isLocal) {
  INT_ASSERT(call->primitive);
  // Check primitives for communication
  switch (call->primitive->tag) {
  case PRIM_UNKNOWN:
    // TODO: Return true for PRIM_UNKNOWNs that are side-effect free
    return false;

  case PRIM_NOOP:
  case PRIM_REF_TO_STRING:
  case PRIM_RETURN:
  case PRIM_UNARY_MINUS:
  case PRIM_UNARY_PLUS:
  case PRIM_UNARY_NOT:
  case PRIM_UNARY_LNOT:
  case PRIM_ADD:
  case PRIM_SUBTRACT:
  case PRIM_MULT:
  case PRIM_DIV:
  case PRIM_MOD:
  case PRIM_LSH:
  case PRIM_RSH:
  case PRIM_EQUAL:
  case PRIM_NOTEQUAL:
  case PRIM_LESSOREQUAL:
  case PRIM_GREATEROREQUAL:
  case PRIM_LESS:
  case PRIM_GREATER:
  case PRIM_AND:
  case PRIM_OR:
  case PRIM_XOR:
  case PRIM_POW:
  case PRIM_MIN:
  case PRIM_MAX:

  case PRIM_GET_MEMBER:
  case PRIM_GET_SVEC_MEMBER:
  case PRIM_GET_PRIV_CLASS:
  case PRIM_NEW_PRIV_CLASS:

  case PRIM_CHECK_NIL:
  case PRIM_GET_REAL:
  case PRIM_GET_IMAG:

  case PRIM_ADDR_OF:
  case PRIM_LOCAL_CHECK:

  case PRIM_INIT_FIELDS:
  case PRIM_PTR_EQUAL:
  case PRIM_PTR_NOTEQUAL:
  case PRIM_CAST:

  case PRIM_BLOCK_LOCAL:

  case PRIM_ON_LOCALE_NUM:
  case PRIM_GET_SERIAL:
  case PRIM_SET_SERIAL:

  case PRIM_START_RMEM_FENCE:
  case PRIM_FINISH_RMEM_FENCE:

  case PRIM_STRING_COPY:
  case PRIM_CAST_TO_VOID_STAR:
  case PRIM_SIZEOF:

  case PRIM_GET_USER_LINE:
  case PRIM_GET_USER_FILE:
    DEBUG_PRINTF(" *** OK (default): %s\n", call->primitive->name);
    return true;

  case PRIM_MOVE:
  case PRIM_ASSIGN:
  case PRIM_ADD_ASSIGN:
  case PRIM_SUBTRACT_ASSIGN:
  case PRIM_MULT_ASSIGN:
  case PRIM_DIV_ASSIGN:
  case PRIM_MOD_ASSIGN:
  case PRIM_LSH_ASSIGN:
  case PRIM_RSH_ASSIGN:
  case PRIM_AND_ASSIGN:
  case PRIM_OR_ASSIGN:
  case PRIM_XOR_ASSIGN:
    if (!isCallExpr(call->get(2))) { // callExprs checked in calling function
      if (!call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_REF) &&
          !call->get(2)->typeInfo()->symbol->hasFlag(FLAG_WIDE_REF)) {
        DEBUG_PRINTF(" *** OK (PRIM_MOVE 1): %s\n", call->primitive->name);
        return true;
      }
    } else {
      DEBUG_PRINTF(" *** OK (PRIM_MOVE 2): %s\n", call->primitive->name);
      // Not necessarily true, but we return true because
      // the callExpr will be checked in the calling function
      return true;
    }
    break;

// I think these can always return true. <hilde>
// But that works only if the remote get is removed from code generation.
  case PRIM_WIDE_GET_LOCALE:
  case PRIM_WIDE_GET_NODE:
  case PRIM_WIDE_GET_ADDR:
    // If this test is true, a remote get is required.
    if (!(call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_REF) &&
          call->get(1)->getValType()->symbol->hasFlag(FLAG_WIDE_CLASS))) {
      DEBUG_PRINTF(" *** OK (PRIM_WIDE_GET_LOCALE, etc.): %s\n",
                   call->primitive->name);
      return true;
    }
    break;

  case PRIM_SET_UNION_ID:
  case PRIM_GET_UNION_ID:
  case PRIM_GET_MEMBER_VALUE:
  case PRIM_GET_SVEC_MEMBER_VALUE:
    if (!call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_REF)) {
      return true;
      DEBUG_PRINTF(" *** OK (PRIM_SET_UNION_ID, etc.): %s\n",
                   call->primitive->name);
    }
    break;

  case PRIM_ARRAY_SET:
  case PRIM_ARRAY_SET_FIRST:
  case PRIM_SETCID:
  case PRIM_TESTCID:
  case PRIM_GETCID:
  case PRIM_ARRAY_GET:
  case PRIM_ARRAY_GET_VALUE:
  case PRIM_DYNAMIC_CAST:
    if (!call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
      DEBUG_PRINTF(" *** OK (PRIM_ARRAY_SET, etc.): %s\n",
                   call->primitive->name);
      return true;
    }
    break;

  case PRIM_DEREF:
  case PRIM_SET_MEMBER:
  case PRIM_SET_SVEC_MEMBER:
    if (!call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_REF) &&
        !call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
      DEBUG_PRINTF(" *** OK (PRIM_DEREF, etc.): %s\n", call->primitive->name);
      return true;
    }
    break;

  case PRIM_CHPL_COMM_GET:
  case PRIM_CHPL_COMM_PUT:
  case PRIM_CHPL_COMM_REMOTE_PREFETCH:
  case PRIM_CHPL_COMM_GET_STRD:
  case PRIM_CHPL_COMM_PUT_STRD:
    // These may involve communication, so are deemed slow.
    return false;

  case PRIM_SYNC_INIT: // Maybe fast?
  case PRIM_SYNC_DESTROY: // Maybe fast?
  case PRIM_SYNC_LOCK:
  case PRIM_SYNC_UNLOCK:
  case PRIM_SYNC_WAIT_FULL:
  case PRIM_SYNC_WAIT_EMPTY:
  case PRIM_SYNC_SIGNAL_FULL:
  case PRIM_SYNC_SIGNAL_EMPTY:
  case PRIM_SINGLE_INIT: // Maybe fast?
  case PRIM_SINGLE_DESTROY: // Maybe fast?
  case PRIM_SINGLE_LOCK:
  case PRIM_SINGLE_UNLOCK:
  case PRIM_SINGLE_WAIT_FULL:
  case PRIM_SINGLE_SIGNAL_FULL:

  case PRIM_WRITEEF:
  case PRIM_WRITEFF:
  case PRIM_WRITEXF:
  case PRIM_READFE:
  case PRIM_READFF:
  case PRIM_READXX:
  case PRIM_SYNC_IS_FULL:
  case PRIM_SINGLE_WRITEEF:
  case PRIM_SINGLE_READFF:
  case PRIM_SINGLE_READXX:
  case PRIM_SINGLE_IS_FULL:
   // These may block, so are deemed slow.
   return false;

  case PRIM_NEW:
  case PRIM_INIT:
  case PRIM_NO_INIT:
  case PRIM_TYPE_INIT:
  case PRIM_LOGICAL_FOLDER:
  case PRIM_TYPEOF:
  case PRIM_TYPE_TO_STRING:
  case PRIM_ENUM_MIN_BITS:
  case PRIM_ENUM_IS_SIGNED:
  case PRIM_IS_UNION_TYPE:
  case PRIM_IS_ATOMIC_TYPE:
  case PRIM_IS_SYNC_TYPE:
  case PRIM_IS_SINGLE_TYPE:
  case PRIM_IS_TUPLE_TYPE:
  case PRIM_IS_STAR_TUPLE_TYPE:
  case PRIM_IS_SUBTYPE:
  case PRIM_TUPLE_EXPAND:
  case PRIM_TUPLE_AND_EXPAND:
  case PRIM_QUERY:
  case PRIM_QUERY_PARAM_FIELD:
  case PRIM_QUERY_TYPE_FIELD:
  case PRIM_ERROR:
  case PRIM_WARNING:

  case PRIM_BLOCK_PARAM_LOOP:
  case PRIM_BLOCK_BEGIN:
  case PRIM_BLOCK_COBEGIN:
  case PRIM_BLOCK_COFORALL:
  case PRIM_BLOCK_ON:
  case PRIM_BLOCK_BEGIN_ON:
  case PRIM_BLOCK_COBEGIN_ON:
  case PRIM_BLOCK_COFORALL_ON:
  case PRIM_BLOCK_UNLOCAL:

  case PRIM_ACTUALS_LIST:
  case PRIM_YIELD:

  case PRIM_USE:
  case PRIM_USED_MODULES_LIST:

  case PRIM_WHEN:
  case PRIM_INT_ERROR:
  case PRIM_CAPTURE_FN:
  case PRIM_CREATE_FN_TYPE:

  case PRIM_NUM_FIELDS:
  case PRIM_IS_POD:
  case PRIM_FIELD_NUM_TO_NAME:
  case PRIM_FIELD_VALUE_BY_NUM:
  case PRIM_FIELD_ID_BY_NUM:
  case PRIM_FIELD_VALUE_BY_NAME:
    INT_FATAL("This primitive should have been removed from the tree by now.");
    break;

    // By themselves, loops are considered "fast".
  case PRIM_BLOCK_WHILEDO_LOOP:
  case PRIM_BLOCK_DOWHILE_LOOP:
  case PRIM_BLOCK_FOR_LOOP:
  case PRIM_BLOCK_C_FOR_LOOP:
    return true;
 
   // These don't block in the Chapel sense, but they may require a system
    // call so we don't consider them eligible.
    //
  case PRIM_FREE_TASK_LIST:
  case PRIM_ARRAY_ALLOC:
  case PRIM_ARRAY_FREE:
  case PRIM_ARRAY_FREE_ELTS:
    return false;

    // Temporarily unclassified (legacy) cases.
    // These formerly defaulted to false (slow), so we leave them
    // here until they are proven fast.
  case PRIM_GET_END_COUNT:
  case PRIM_SET_END_COUNT:
  case PRIM_PROCESS_TASK_LIST:
  case PRIM_EXECUTE_TASKS_IN_LIST:
  case PRIM_TO_LEADER:
  case PRIM_TO_FOLLOWER:
  case PRIM_DELETE:
  case PRIM_CALL_DESTRUCTOR:
  case PRIM_HEAP_REGISTER_GLOBAL_VAR:
  case PRIM_HEAP_BROADCAST_GLOBAL_VARS:
  case PRIM_PRIVATE_BROADCAST:
  case PRIM_RT_ERROR:
  case PRIM_RT_WARNING:
  case PRIM_FTABLE_CALL:
  case PRIM_VIRTUAL_METHOD_CALL:
    return false;

  default:
    INT_FATAL("Unhandled case.");
    break;
  }

  return isLocal;
}

static bool
inLocalBlock(CallExpr *call) {
  for (Expr* parent = call->parentExpr; parent; parent = parent->parentExpr) {
    if (BlockStmt* blk = toBlockStmt(parent)) {
      // NOAKES 2014/11/25  Transitional. Do not trip over blockInfoGet for a Loop
      if (blk->isLoopStmt() == true)
        ;
      else if (blk->blockInfoGet() && blk->blockInfoGet()->isPrimitive(PRIM_BLOCK_LOCAL))
        return true;
    }
  }
  return false;
}

static bool
markFastSafeFn(FnSymbol *fn, int recurse, Vec<FnSymbol*> *visited) {
  if (fn->hasFlag(FLAG_FAST_ON))
    return true;

  if (fn->hasFlag(FLAG_EXPORT))
    return true;

  if (fn->hasFlag(FLAG_EXTERN)) {
    // consider a pragma to indicate that it would be "fast"
    return false;
  }

  if (fn->hasFlag(FLAG_NON_BLOCKING))
    return false;

  visited->add_exclusive(fn);

  std::vector<CallExpr*> calls;

  collectCallExprs(fn, calls);

  for_vector(CallExpr, call, calls) {
    DEBUG_PRINTF("\tcall %p (id=%d): ", call, call->id);
    bool isLocal = fn->hasFlag(FLAG_LOCAL_FN) || inLocalBlock(call);

    if (!call->primitive) {
      DEBUG_PRINTF("(non-primitive CALL)\n");
      if ((recurse>0) && call->isResolved()) {
        if (call->isResolved()->hasFlag(FLAG_ON_BLOCK)) {
          visited->add_exclusive(call->isResolved());
          call->isResolved()->removeFlag(FLAG_FAST_ON);
          DEBUG_PRINTF("%d: recurse FAILED (nested on block, id=%d).\n",
                       recurse-1, call->id);
          return false;
        }
        if (!visited->in(call->isResolved())) {
          DEBUG_PRINTF("%d: recurse %p (block=%p, id=%d)\n", recurse-1,
                       call->isResolved(), call->isResolved()->body,
                       call->isResolved()->id);
          DEBUG_PRINTF("\tlength=%d\n", call->isResolved()->body->length());
          if (!markFastSafeFn(call->isResolved(), recurse-1, visited)) {
            DEBUG_PRINTF("%d: recurse FAILED (id=%d).\n", recurse-1, call->id);
            return false;
          }
        } else {
          DEBUG_PRINTF("%d: recurse ALREADY VISITED %p (id=%d)\n", recurse-1,
                       call->isResolved(), call->isResolved()->id);
          DEBUG_PRINTF("\tlength=%d\n", call->isResolved()->body->length());
        }
        DEBUG_PRINTF("%d: recurse DONE.\n", recurse-1);
      } else {
        // No function calls allowed
        DEBUG_PRINTF("%d: recurse FAILED (%s, id=%d).\n", recurse-1,
                     recurse == 1 ? "too deep" : "function not resolved",
                     call->id);
        return false;
      }
    } else if (isFastPrimitive(call, isLocal)) {
      DEBUG_PRINTF(" (FAST primitive CALL)\n");
    } else {
      DEBUG_PRINTF("%d: FAILED (non-FAST primitive CALL: %s, id=%d)\n",
                   recurse-1, call->primitive->name, call->id);
      return false;
    }
  }
  return true;
}


void
optimizeOnClauses(void) {
  if (fNoOptimizeOnClauses)
    return;

  compute_call_sites();

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (!fn->hasFlag(FLAG_ON_BLOCK))
      continue;
    DEBUG_PRINTF("%p (%s in %s:%d): FLAG_ON_BLOCK (block=%p, id=%d)\n",
                 fn, fn->cname,
                 toModuleSymbol(fn->defPoint->parentSymbol)->filename,
                 fn->linenum(), fn->body, fn->id);
    DEBUG_PRINTF("\tlength=%d\n", fn->body->length());
    Vec<FnSymbol*> visited;

    if (markFastSafeFn(fn, optimize_on_clause_limit, &visited)) {
      DEBUG_PRINTF("\t[CANDIDATE FOR FAST FORK]\n");
      fn->addFlag(FLAG_FAST_ON);

      if (fReportOptimizedOn) {
        ModuleSymbol *mod = toModuleSymbol(fn->defPoint->parentSymbol);
        INT_ASSERT(mod);
        if (developer ||
            ((mod->modTag != MOD_INTERNAL) && (mod->modTag != MOD_STANDARD))) {
          printf("Optimized on clause (%s) in module %s (%s:%d)\n",
                 fn->cname, mod->name, fn->fname(), fn->linenum());
        }
      }
    }
  }
}
