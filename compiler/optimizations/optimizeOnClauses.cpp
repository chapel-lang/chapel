/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#include "passes.h"

#include "astutil.h"
#include "driver.h"
#include "expr.h"
#include "stlUtil.h"
#include "stmt.h"
#include "wellknown.h"

#include <vector>


// There are two questions:
// 1- is the primitive/function eligible to run in a fast block?
//    any blocking or system call disqualifies it since a fast
//    AM handler can be run in a signal handler
// 2- is the primitive/function communication free?

// Any function containing communication can't run in a fast block.

enum {
  // The primitive is ineligible for a fast (e.g. uses a lock or allocator)
  // AND it causes communication
  NOT_FAST_NOT_LOCAL,
  // Is the primitive ineligible for a fast (e.g. uses a lock or allocator)
  // but communication free?
  LOCAL_NOT_FAST,
  // Does the primitive communicate?
  // This implies NOT_FAST, unless it is in a local block
  // if it is in a local block, this means IS_FAST.
  FAST_NOT_LOCAL,
  // Is the primitive function fast (ie, could it be run in a signal handler)
  // IS_FAST implies IS_LOCAL.
  FAST_AND_LOCAL
};

//
// Return NOT_FAST, NOT_LOCAL, IS_LOCAL, or IS_FAST.
//
static int
classifyPrimitive(CallExpr *call) {
  INT_ASSERT(call->primitive);
  // Check primitives for suitability for executeOnFast and for communication
  switch (call->primitive->tag) {


  case NUM_KNOWN_PRIMS:
  // This snippet creates a single case statement for all the primitives
  // that should not exist at code-generation time. That way, most of them
  // can simply cause an assertion.
  //
  // case PRIM_NEW:
  // ...
  #define PRIMITIVE_G(NAME)
  #define PRIMITIVE_R(NAME) case NAME:
  #include "primitive_list.h"
  #undef PRIMITIVE_R
  #undef PRIMITIVE_G
    switch (call->primitive->tag) {
      case PRIM_GET_USER_LINE:
      case PRIM_GET_USER_FILE:
      case PRIM_BLOCK_LOCAL:
        return FAST_AND_LOCAL;

      // By themselves, loops are considered "fast".
      case PRIM_BLOCK_WHILEDO_LOOP:
      case PRIM_BLOCK_DOWHILE_LOOP:
      case PRIM_BLOCK_FOR_LOOP:
      case PRIM_BLOCK_C_FOR_LOOP:
        return FAST_AND_LOCAL;

      default:
        INT_FATAL("primitive should have been removed from the tree by now.");
    }
    break;

  case PRIM_UNKNOWN:
    // TODO: Return FAST_AND_LOCAL for PRIM_UNKNOWNs that are side-effect free
    return NOT_FAST_NOT_LOCAL;

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
  case PRIM_NEW_PRIV_CLASS:

  case PRIM_CHECK_NIL:
  case PRIM_GET_REAL:
  case PRIM_GET_IMAG:

  case PRIM_ADDR_OF:
  case PRIM_SET_REFERENCE:
  case PRIM_LOCAL_CHECK:

  case PRIM_PTR_EQUAL:
  case PRIM_PTR_NOTEQUAL:
  case PRIM_CAST:

  case PRIM_ON_LOCALE_NUM:
  case PRIM_GET_SERIAL:
  case PRIM_SET_SERIAL:

  case PRIM_START_RMEM_FENCE:
  case PRIM_FINISH_RMEM_FENCE:

  case PRIM_CAST_TO_VOID_STAR:
  case PRIM_SIZEOF_BUNDLE:
  case PRIM_SIZEOF_DDATA_ELEMENT:

  case PRIM_LOOKUP_FILENAME:

  case PRIM_STACK_ALLOCATE_CLASS:

  case PRIM_CLASS_NAME_BY_ID:

  case PRIM_INVARIANT_START:
  case PRIM_NO_ALIAS_SET:
  case PRIM_COPIES_NO_ALIAS_SET:
  case PRIM_OPTIMIZATION_INFO:
    return FAST_AND_LOCAL;

  case PRIM_MOVE:
  case PRIM_ASSIGN:
  case PRIM_UNORDERED_ASSIGN:
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
    if (isCallExpr(call->get(2))) { // callExprs checked in calling function
      // Not necessarily true, but we return true because
      // the callExpr will be checked in the calling function
      return FAST_AND_LOCAL;
    } else {
      bool arg1wide = call->get(1)->isWideRef();
      bool arg2wide = call->get(2)->isWideRef();

      // If neither argument is a wide reference, OK: no communication
      if (!arg1wide && !arg2wide) {
        return FAST_AND_LOCAL;
      }

      if (call->isPrimitive(PRIM_MOVE)) {
        bool arg1ref = call->get(1)->isRef();
        bool arg2ref = call->get(2)->isRef();
        // Handle (move tmp:ref, other_tmp:wide_ref)
        // and    (move tmp:wide_ref, other_tmp:ref)
        // these does not require communication and merely adjust
        // the wideness of the ref.
        if ((arg1wide && arg2ref) || (arg1ref && arg2wide)) {
          return FAST_AND_LOCAL;
        }
      }

      // Otherwise, communication is required if we're not in a local block
      return FAST_NOT_LOCAL;
    }

  case PRIM_WIDE_MAKE:
    return FAST_NOT_LOCAL;

// I think these can always return true. <hilde>
// But that works only if the remote get is removed from code generation.
  case PRIM_WIDE_GET_LOCALE:
  case PRIM_WIDE_GET_NODE:
  case PRIM_WIDE_GET_ADDR:
    // If this test is true, a remote get is required.
    if (!(call->get(1)->isWideRef() &&
          call->get(1)->getValType()->symbol->hasFlag(FLAG_WIDE_CLASS))) {
      return FAST_AND_LOCAL;
    }
    return FAST_NOT_LOCAL;

  case PRIM_ARRAY_SHIFT_BASE_POINTER:
    // SHIFT_BASE_POINTER is fast as long as none of the
    // arguments are wide references.
    if (call->get(1)->isWideRef() ||
        call->get(2)->isWideRef() ||
        call->get(3)->isWideRef())
      return FAST_NOT_LOCAL;
    else
      return FAST_AND_LOCAL;

  case PRIM_SET_UNION_ID:
  case PRIM_GET_UNION_ID:
  case PRIM_GET_MEMBER_VALUE:
  case PRIM_GET_SVEC_MEMBER_VALUE:
    if (!call->get(1)->isWideRef()) {
      return FAST_AND_LOCAL;
    }
    return FAST_NOT_LOCAL;

  case PRIM_ARRAY_SET:
  case PRIM_ARRAY_SET_FIRST:
  case PRIM_SETCID:
  case PRIM_TESTCID:
  case PRIM_GETCID:
  case PRIM_ARRAY_GET:
  case PRIM_DYNAMIC_CAST:
    if (!call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
      return FAST_AND_LOCAL;
    }
    return FAST_NOT_LOCAL;

  case PRIM_DEREF:
  case PRIM_SET_MEMBER:
  case PRIM_SET_SVEC_MEMBER:
    if (!call->get(1)->isWideRef() &&
        !call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
      return FAST_AND_LOCAL;
    }
    return FAST_NOT_LOCAL;

  case PRIM_CHPL_COMM_GET:
  case PRIM_CHPL_COMM_PUT:
  case PRIM_CHPL_COMM_ARRAY_GET:
  case PRIM_CHPL_COMM_ARRAY_PUT:
  case PRIM_CHPL_COMM_REMOTE_PREFETCH:
  case PRIM_CHPL_COMM_GET_STRD:
  case PRIM_CHPL_COMM_PUT_STRD:
    // These involve communication
    // MPF: Couldn't these be fast if in a local block?
    // Shouldn't this be return FAST_NOT_LOCAL ?
    return NOT_FAST_NOT_LOCAL;

    // These don't block in the Chapel sense, but they may require a system
    // call so we don't consider them fast-eligible.
    // However, they are communication free.
    //
  case PRIM_STRING_COPY:
    return LOCAL_NOT_FAST;

  case PRIM_GET_DYNAMIC_END_COUNT:
  case PRIM_SET_DYNAMIC_END_COUNT:
    return FAST_AND_LOCAL;

    // Temporarily unclassified (legacy) cases.
    // These formerly defaulted to false (slow), so we leave them
    // here until they are proven fast.
  case PRIM_REGISTER_GLOBAL_VAR:
  case PRIM_BROADCAST_GLOBAL_VARS:
  case PRIM_PRIVATE_BROADCAST:
  case PRIM_RT_ERROR:
  case PRIM_RT_WARNING:
  case PRIM_FTABLE_CALL:
  case PRIM_VIRTUAL_METHOD_CALL:
  case PRIM_INT_ERROR:
    return NOT_FAST_NOT_LOCAL;

  // no default, so that it is usually a C compilation
  // error when a primitive is added but not included here.
  }

  // At the end of the switch statement.
  // We get here if there is an unhandled primitive.
  INT_FATAL("Unhandled case.");
  return NOT_FAST_NOT_LOCAL;
}

static int
setLocal(int is, bool inLocal)
{
  // If it's in a local block, it's always local.
  if (inLocal) {
    if (is == NOT_FAST_NOT_LOCAL ) is = LOCAL_NOT_FAST;
    if (is == FAST_NOT_LOCAL )     is = FAST_AND_LOCAL;
  }

  return is;
}

static bool
isFast(int is)
{
  if (is == FAST_AND_LOCAL || is == FAST_NOT_LOCAL) return true;
  return false;
}

static bool
isLocal(int is)
{
  if (is == FAST_AND_LOCAL || is == LOCAL_NOT_FAST) return true;
  return false;
}



static int
classifyPrimitive(CallExpr *call, bool inLocal)
{
  int is = classifyPrimitive(call);

  // If it's in a local block, it's always local.
  is = setLocal(is, inLocal);

  return is;
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

static int
markFastSafeFn(FnSymbol *fn, int recurse, std::set<FnSymbol*>& visited) {

  // First, handle functions we've already visited.
  if (visited.count(fn) != 0) {
    if (fn->hasFlag(FLAG_FAST_ON))
      return FAST_AND_LOCAL;
    if (fn->hasFlag(FLAG_LOCAL_FN))
      return LOCAL_NOT_FAST;
    return NOT_FAST_NOT_LOCAL;
  }

  // Now, add fn to the set of visited functions,
  // since we will categorize it now.
  visited.insert(fn);

  // Next, classify extern functions
  if (fn->hasFlag(FLAG_EXTERN)) {
    if (fn->hasFlag(FLAG_FAST_ON_SAFE_EXTERN)) {
      // Make sure the FAST_ON and LOCAL_FN flags are set.
      fn->addFlag(FLAG_FAST_ON);
      fn->addFlag(FLAG_LOCAL_FN);
      return FAST_AND_LOCAL;
    } else if(fn->hasFlag(FLAG_LOCAL_FN)) {
      return LOCAL_NOT_FAST;
    } else {
      // Other extern functions are not fast or local.
      return NOT_FAST_NOT_LOCAL;
    }
  }

  // Next, go through function bodies.
  // We will set maybefast=false if we see something in the function
  //  that is local but not suitable for a signal handler
  //  (mostly allocation or locking).
  // We will return NOT_FAST_NOT_LOCAL immediately if we see something
  // in the function that is not local.
  bool maybefast = true;

  if (fn->hasFlag(FLAG_NON_BLOCKING))
    maybefast = false;

  std::vector<CallExpr*> calls;

  collectCallExprs(fn, calls);

  for_vector(CallExpr, call, calls) {
    bool inLocal = fn->hasFlag(FLAG_LOCAL_FN) || inLocalBlock(call);

    if (call->primitive) {
      int is = classifyPrimitive(call, inLocal);

      if (!isLocal(is)) {
        // FAST_NOT_LOCAL or NOT_FAST_NOT_LOCAL
        return NOT_FAST_NOT_LOCAL;
      }

      // is == FAST_AND_LOCAL requires no action
      if (is == LOCAL_NOT_FAST) {
        maybefast = false;
      }

    } else {
      if (recurse<=0 || !call->isResolved()) {
        // didn't resolve or past too much recursion.
        // No function calls allowed
        return NOT_FAST_NOT_LOCAL;

      } else {
        // Handle nested 'on' statements
        if (call->resolvedFunction()->hasFlag(FLAG_ON_BLOCK)) {
          if (inLocal) {
            maybefast = false;
          } else {
            return NOT_FAST_NOT_LOCAL;
          }
        }

        // is the call to a fast/local function?
        int is = markFastSafeFn(call->resolvedFunction(),
                                recurse - 1,
                                visited);

        // Remove NOT_LOCAL parts if it's in a local block
        is = setLocal(is, inLocal);

        if (!isLocal(is)) {
          return NOT_FAST_NOT_LOCAL;
        }

        if (is == LOCAL_NOT_FAST) {
          maybefast = false;
        }
        // otherwise, possibly still fast.
      }
    }
  }

  // At this point we've considered all of the function body
  // so if maybefast is still true, we can consider this function fast.

  // We only get to this point if the function is local
  // (otherwise we would return above)
  fn->addFlag(FLAG_LOCAL_FN);

  if (maybefast) {
    fn->addFlag(FLAG_FAST_ON);

    return FAST_AND_LOCAL;
  } else {
    return LOCAL_NOT_FAST;
  }
}

// Removes PRIM_START_RMEM_FENCE and PRIM_FINISH_RMEM_FENCE
// from the passed function.
// For reporting purposes, returns true if the function actually
// changed the function.
static bool
removeUnnecessaryFences(FnSymbol* fn)
{
  bool ret = false;

  // These fences are only present if one of these flags
  // is set. This is an optimization.
  if (fn->hasEitherFlag(FLAG_WRAPPER_NEEDS_START_FENCE,
                        FLAG_WRAPPER_NEEDS_FINISH_FENCE)) {

    // If the function is marked local, remove
    // PRIM_START_RMEM_FENCE / PRIM_FINISH_RMEM_FENCE
    // from the wrapper.
    std::vector<CallExpr*> calls;

    collectCallExprs(fn, calls);

    for_vector(CallExpr, call, calls) {
      if (call->isPrimitive(PRIM_START_RMEM_FENCE) ||
          call->isPrimitive(PRIM_FINISH_RMEM_FENCE)) {
        call->remove();
        ret = true;
      }
    }
  }

  return ret;
}

static CallExpr* findRealOnCall(FnSymbol* wrapperFn) {
  std::vector<CallExpr*> calls;
  collectFnCalls(wrapperFn, calls);
  for_vector(CallExpr, call, calls) {
    if (call->resolvedFunction()->hasFlag(FLAG_ON)) {
      return call;
    }
  }
  INT_ASSERT(false);
  return NULL;
}

// Insert runningTaskCounter increment and decrement calls for on-stmts.
// Adjust the callsite to decrement before migrating a task and adjust the
// wrapper function to increment before calling the body of the moved task.
//
// TODO move this into its own pass
static void addRunningTaskModifiers(void) {
  compute_call_sites();

  std::set<CallExpr*> visited;
  forv_Vec(CallExpr, taskMigrationCall, gCallExprs) {
    FnSymbol* fn = taskMigrationCall->resolvedFunction();
    if (fn && fn->hasFlag(FLAG_ON_BLOCK)) {
      // Adjust runningTaskCounter in wrapper: For non-fast on's, increment the
      // runningTaskCounter in the wrapper function before executing the body.
      // Fast on's run directly in the comm-handler and will not spawn a task.
      if (fn->hasFlag(FLAG_FAST_ON) == false) {
        CallExpr* onFnCall = findRealOnCall(fn);
        SET_LINENO(onFnCall);
        if (visited.count(onFnCall) == 0) {
          visited.insert(onFnCall);
          onFnCall->insertBefore(new CallExpr(gChplIncRunningTask));
          onFnCall->insertAfter(new CallExpr(gChplDecRunningTask));
        }
      }

      // Adjust runningTaskCounter at the callsite: Before initiating on-stmts
      // that aren't fast or non-blocking, decrement the runningTaskCounter
      // before migrating to a new locale
      if (fn->hasEitherFlag(FLAG_NON_BLOCKING, FLAG_FAST_ON) == false) {
        SET_LINENO(taskMigrationCall);
        taskMigrationCall->insertBefore(new CallExpr(gChplDecRunningTask));
        taskMigrationCall->insertAfter(new CallExpr(gChplIncRunningTask));
      }
    }
  }
}

void
optimizeOnClauses(void) {
  if (fNoOptimizeOnClauses) {
    addRunningTaskModifiers();
    return;
  }

  compute_call_sites();

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    std::set<FnSymbol*> visited;

    int is = markFastSafeFn(fn, optimize_on_clause_limit, visited);

    bool fastFork = isFast(is);
    bool removeRmemFences = isLocal(is);

    if (!fn->hasFlag(FLAG_ON_BLOCK))
      fastFork = false;

    if (fastFork) {
      // Code generation will use executeOnFast because
      // the function will have been marked with FLAG_FAST_ON
      // in markFastSafeFn.
      // No other action is necessary at this point.
    }
    if (removeRmemFences) {
      // Compiling with --cache-remote adds fences for the start
      // and end of a on-statement wrapper function. These fences
      // generally cause communication and will lead to deadlock
      // if they are left in an on-body that is run in an AM
      // handler (ie with executeOnFast). Additionally, if there
      // is no communication in the on body, the fence can be
      // removed without harm (since that portion of code will
      // not interact at all with this cache for *remote* data).
      removeRmemFences = removeUnnecessaryFences(fn);
    }

    if ( (fastFork || removeRmemFences) && fReportOptimizedOn) {
      ModuleSymbol *mod = toModuleSymbol(fn->defPoint->parentSymbol);
      INT_ASSERT(mod);
      if (developer ||
          ((mod->modTag != MOD_INTERNAL) && (mod->modTag != MOD_STANDARD))) {
        if (fastFork) {
          printf("Optimized on clause (%s) in module %s (%s:%d)\n",
               fn->cname, mod->name, fn->fname(), fn->linenum());
        }
        if (removeRmemFences) {
          printf("Optimized rmem fence (%s) in module %s (%s:%d)\n",
               fn->cname, mod->name, fn->fname(), fn->linenum());
        }
        if (developer) printf("(id %i)\n", fn->id);
      }
    }
  }
  addRunningTaskModifiers();
}
