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

#include "AstVisitorTraverse.h"
#include "astutil.h"
#include "CForLoop.h"
#include "driver.h"
#include "expr.h"
#include "ForallStmt.h"
#include "ForLoop.h"
#include "optimizations.h"
#include "passes.h"
#include "resolution.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "virtualDispatch.h"
#include "wellknown.h"

#include <stack>

/*
   Optimization to transform certain statements in foralls into
   unordered operations.

   The optimization consists of two parts:
     * an early part that makes use of lifetime checking information
       and expects to be called during lifetime checking.
     * a later part that is called late in compilation (around LICM)
       that does the actual transformation.

   The first makes certain checks on the lifetime of the relevant
   variables to make sure the optimization is valid.

   The overall idea of the optimization is to find statements that
   represent the last thing occurring in a forall iteration. Since the
   forall iterations could run in any order, these last statements could
   also complete in any order.

   This handles PRIM_ASSIGN as well as several chpl_comm_atomic functions
   by converting them to unordered calls within the runtime.

   It could handle PRIM_ARRAY_SET_FIRST as well if that becomes
   important in the future.
 */

// ---- getLastStmts and friends

// getLastStmts returns the last statements, including inside of conditionals or
// normal (non loop) blocks. Only returns statements that, if executed,
// are the last statement executed in the loop block.

static void helpGetLastStmts(Expr* last, std::vector<Expr*>& stmts);

static void getLastStmts(BlockStmt* loop, std::vector<Expr*>& stmts) {

  Expr* last = NULL;

  if (CForLoop* cfor = toCForLoop(loop)) {
    // For a CForLoop, ignore the initBlock, testBlock, incrBlock.
    BlockStmt* init = cfor->initBlockGet();
    BlockStmt* test = cfor->testBlockGet();
    BlockStmt* incr = cfor->incrBlockGet();

    last = loop->body.last();
    while (last == init || last == test || last == incr) {
      last = last->prev;
    }
  } else {
    last = loop->body.last();
  }
  helpGetLastStmts(last, stmts);
}

static Expr* skipIgnoredStmts(Expr* last) {

  while (true) {
    CallExpr* call = toCallExpr(last);
    FnSymbol* calledFn = NULL;
    if (call)
      calledFn = call->resolvedFunction();

    // Ignore calls to chpl_rmem_consist_maybe_acquire that were added
    // by the compiler. We will remove these if we optimize an atomic op.
    if (calledFn && calledFn->hasFlag(FLAG_COMPILER_ADDED_REMOTE_FENCE)) {
      last = last->prev;

    // Ignore calls to PRIM_END_OF_STATEMENT
    } else if (call && call->isPrimitive(PRIM_END_OF_STATEMENT)) {
      last = last->prev;

    // Ignore PRIM_OPTIMIZATION_INFO and related DefExpr
    // (move last before these if they are present)
    } else if (call && call->isPrimitive(PRIM_OPTIMIZATION_INFO)) {
      Symbol* optSym = toSymExpr(call->get(1))->symbol();
      last = last->prev;
      if (DefExpr* def = toDefExpr(last)) {
        if (def->sym == optSym)
          last = last->prev;
      }

    } else {
      break; // stop looking
    }
  }

  return last;
}

static void helpGetLastStmts(Expr* last, std::vector<Expr*>& stmts) {

  if (last == NULL)
    return;

  last = skipIgnoredStmts(last);

  if (CondStmt* cond = toCondStmt(last)) {
    helpGetLastStmts(cond->thenStmt->body.last(), stmts);
    if (cond->elseStmt)
      helpGetLastStmts(cond->elseStmt->body.last(), stmts);
    return;
  }

  if (BlockStmt* nestedBlock = toBlockStmt(last)) {
    if (nestedBlock->isRealBlockStmt()) {
      helpGetLastStmts(nestedBlock->body.last(), stmts);
      return;
    }
  }

  // Ignore PRIM_OPTIMIZATION_INFO and related DefExpr
  // (move last before these if they are present)
  if (CallExpr* call = toCallExpr(last)) {
    if (call->isPrimitive(PRIM_OPTIMIZATION_INFO)) {
      Symbol* optSym = toSymExpr(call->get(1))->symbol();
      last = last->prev;
      if (DefExpr* def = toDefExpr(last)) {
        if (def->sym == optSym)
          last = last->prev;
      }
    }
  }

  last = skipIgnoredStmts(last);
  stmts.push_back(last);
}



// ---- mark optimizable foralls during lifetime checking

// This could definitely be implemented in a faster way.
static bool isBlockWithinBlock(BlockStmt* a, BlockStmt* b) {
  Expr* findParent = b;
  for (Expr* cur = a; cur; cur = cur->parentExpr) {
    if (cur == findParent)
      return true;
  }
  return false;
}


// Returns true if the symbol refers to something that will
// outlive the loop.
static
bool symbolOutlivesLoop(BlockStmt* loop, Symbol* sym,
                        LifetimeInformation* lifetimeInfo) {
  BlockStmt* defInBlock = toBlockStmt(sym->defPoint->parentExpr);
  if (defInBlock && isBlockWithinBlock(defInBlock, loop)) {
    return outlivesBlock(lifetimeInfo, sym, loop);
  } else {
    // Sym is defined outside of loop, OK
    return true;
  }
}

static
bool exprIsOptimizable(BlockStmt* loop, Expr* lastStmt,
                        LifetimeInformation* lifetimeInfo) {
  if (CallExpr* call = toCallExpr(lastStmt)) {
    if (call->isPrimitive(PRIM_ASSIGN)) {
      Symbol* lhs = toSymExpr(call->get(1))->symbol();
      Expr* rhs = call->get(2);
      if (lhs->getValType() == rhs->getValType()) // same type
        return true;
    } else if (call->isNamed("=")) {
      Symbol* lhs = toSymExpr(call->get(1))->symbol();
      Symbol* rhs = toSymExpr(call->get(2))->symbol();
      if (lhs->getValType() == rhs->getValType()) // same type
        if (isPOD(lhs->getValType())) // no custom = overloads
          return true;
    } else if (FnSymbol* fn = call->resolvedFunction()) {
      if (fn->_this &&
          fn->_this->getValType()->symbol->hasFlag(FLAG_ATOMIC_TYPE)) {
        Symbol* atomic = toSymExpr(call->get(1))->symbol();
        INT_ASSERT(atomic->getValType()->symbol->hasFlag(FLAG_ATOMIC_TYPE));
        return true;
      }
    }
  }

  return false;
}

static bool forallNoTaskPrivate(ForallStmt* forall) {
  for_shadow_vars (shadow, temp, forall) {
    if (shadow->isReduce()) {
      if (ShadowVarSymbol* op = shadow->ReduceOpForAccumState())
        if (ModuleSymbol* mod = op->type->symbol->defPoint->getModule())
          if (mod->modTag == MOD_INTERNAL)
            continue; // This internal reduction is OK

      // perhaps a custom reduction could leak a task ID to forall
      return false;
    } else if (shadow->isTaskPrivate()) {
      // task private variable could include arbitrary init expr
      // effectively computing a task id
      return false;
    } else if (shadow->intent == TFI_IN || shadow->intent == TFI_CONST_IN) {
      // copy-init or = for in intent could compute a task ID
      if (!isPOD(shadow->type))
        return false;
    }
  }

  return true;
}

class MarkOptimizableForallLastStmts : public AstVisitorTraverse {

  public:
    LifetimeInformation* lifetimeInfo;

    virtual bool enterForallStmt(ForallStmt* forall);
    void markLoopsInForall(ForallStmt* forall);
};

bool MarkOptimizableForallLastStmts::enterForallStmt(ForallStmt* forall) {
  if (fNoOptimizeForallUnordered == false) {
    // If the optimization is enabled, do this
    markLoopsInForall(forall);
  }

  // Either way, add chpl_comm_unordered_task_fence at the end of
  // the forall.
  // TODO: move this to a better place in the compiler.
  SET_LINENO(forall);
  forall->insertAfter(new CallExpr(gChplAfterForallFence));

  return true;
}

void MarkOptimizableForallLastStmts::markLoopsInForall(ForallStmt* forall) {

  bool addNoTaskPrivate = forallNoTaskPrivate(forall);
  std::vector< std::vector<Expr*> > lastStatementsPerBody;

  // Gather the last statements in each loop body
  std::vector<BlockStmt*> bodies = forall->loopBodies();
  for_vector(BlockStmt, block, bodies) {
    std::vector<Expr*> lastStmts;
    getLastStmts(block, lastStmts);
    lastStatementsPerBody.push_back(lastStmts);
  }

  // Compute the number of last statements
  // (expecting it matches across fast-follower/follower bodies)
  int numLastStmts = -1;
  for (size_t loopNum = 0;
       loopNum < lastStatementsPerBody.size();
       loopNum++) {
    int numThisLoop = (int) lastStatementsPerBody[loopNum].size();
    if (numLastStmts == -1)
      numLastStmts = numThisLoop;
    else if (numLastStmts != numThisLoop)
      return; // Give up on optimizing it
  }

  // Consider the last statements
  for (int stmtNum = 0; stmtNum < numLastStmts; stmtNum++) {

    int loopNum;

    // For this last statement, compute the lifetime
    // properties, but do so ignoring the difference between
    // fast-follower/follower loops.
    bool addLhsOutlivesForall = false;
    bool addRhsOutlivesForall = false;

    loopNum = 0;
    for_vector(BlockStmt, block, bodies) {
      Expr* stmt = lastStatementsPerBody[loopNum][stmtNum];
      if (exprIsOptimizable(block, stmt, lifetimeInfo)) {
        SymExpr* lhs = NULL;
        SymExpr* rhs = NULL;
        if (CallExpr* call = toCallExpr(stmt)) {
          if (call->numActuals() >= 1)
            lhs = toSymExpr(call->get(1));
          if (call->numActuals() >= 2)
            rhs = toSymExpr(call->get(2));
        }
        if (lhs && symbolOutlivesLoop(block, lhs->symbol(), lifetimeInfo))
          addLhsOutlivesForall = true;
        if (rhs && symbolOutlivesLoop(block, rhs->symbol(), lifetimeInfo))
          addRhsOutlivesForall = true;
      }

      loopNum++;
    }

    // Now that we have the lifetime properties, mark all follower
    // loop bodies with the appropriate optimization info.
    loopNum = 0;
    for_vector(BlockStmt, block, bodies) {
      Expr* stmt = lastStatementsPerBody[loopNum][stmtNum];
      if (exprIsOptimizable(block, stmt, lifetimeInfo)) {
        SymExpr* lhs = NULL;
        SymExpr* rhs = NULL;
        if (CallExpr* call = toCallExpr(stmt)) {
          if (call->numActuals() >= 1)
            lhs = toSymExpr(call->get(1));
          if (call->numActuals() >= 2)
            rhs = toSymExpr(call->get(2));
        }
        if (lhs && addLhsOutlivesForall)
          addOptimizationFlag(stmt, OPT_INFO_LHS_OUTLIVES_FORALL);
        if (rhs && addRhsOutlivesForall)
          addOptimizationFlag(stmt, OPT_INFO_RHS_OUTLIVES_FORALL);
        if (addNoTaskPrivate)
          addOptimizationFlag(stmt, OPT_INFO_FLAG_NO_TASK_PRIVATE);
      }

      loopNum++;
    }
  }
}

void checkLifetimesForForallUnorderedOps(FnSymbol* fn,
                                         LifetimeInformation* lifetimeInfo) {

  // This runs even for fNoOptimizeForallUnordered
  // because even if the optimization is disabled, we want
  // to mark the ends of foralls with chpl_comm_unordered_task_fence.

  MarkOptimizableForallLastStmts mark;
  mark.lifetimeInfo = lifetimeInfo;
  fn->accept(&mark);
}


// ---- may-block analysis

// the numeric values matter here
typedef enum {
  STATE_UNKNOWN = 0,
  STATE_COMPUTED = 1,
  STATE_RECURSIVE = 2,
  STATE_COMPLETES_TASKS = 4,
  STATE_CREATES_TASKS = 8,
  STATE_YIELDS_TASKS = 16,
  STATE_JOINS_TASKS = 32,
  STATE_USES_ATOMICS = 64,
  STATE_CRITICAL_SECTION = 128,
  STATE_MAYBE_BLOCKING = 256,
  STATE_EXTERN_ANYTHING = 512,
} MayBlockState;

static const char* blockStateString(MayBlockState state) {
  const char* ret = "";

  // STATE_COMPUTED is intentionally not here.
  if (state & STATE_RECURSIVE)
    ret = astr(ret, ",recursive");
  if (state & STATE_COMPLETES_TASKS)
    ret = astr(ret, ",completes-tasks");
  if (state & STATE_CREATES_TASKS)
    ret = astr(ret, ",creates-tasks");
  if (state & STATE_YIELDS_TASKS)
    ret = astr(ret, ",yields-tasks");
  if (state & STATE_JOINS_TASKS)
    ret = astr(ret, ",joins-tasks");
  if (state & STATE_USES_ATOMICS)
    ret = astr(ret, ",atomics");
  if (state & STATE_CRITICAL_SECTION)
    ret = astr(ret, ",critical-section");
  if (state & STATE_MAYBE_BLOCKING)
    ret = astr(ret, ",blocking");
  if (state & STATE_EXTERN_ANYTHING)
    ret = astr(ret, ",anything");

  // Leave out the leading ','
  if (ret[0] == ',')
    ret = astr(&ret[1]);

  // put a word there so the output doesn't look too weird
  if (ret[0] == '\0' && (state & STATE_COMPUTED))
    ret = astr("computed");

  return ret;
}

static std::map<FnSymbol*, MayBlockState> fnMayBlock;

class GatherBlockingFunctions : public AstVisitorTraverse {

  public:
    std::stack<MayBlockState> blockingLoopStack;

    GatherBlockingFunctions();

    void updateState(MayBlockState newState);
    MayBlockState finalState();

    void beginLoop();
    void endLoop();

    virtual bool enterCallExpr(CallExpr* node);
    virtual void exitCallExpr(CallExpr* node);

    virtual bool enterWhileDoStmt(WhileDoStmt* node);
    virtual void exitWhileDoStmt(WhileDoStmt* node);
    virtual bool enterDoWhileStmt(DoWhileStmt* node);
    virtual void exitDoWhileStmt(DoWhileStmt* node);
    virtual bool enterCForLoop(CForLoop* node);
    virtual void exitCForLoop(CForLoop* node);
    virtual bool enterForLoop(ForLoop* node);
    virtual void exitForLoop(ForLoop* node);
};

static bool loopContainsBlocking(BlockStmt* block) {
  // Do the may-block analysis on each statement within the loop.
  GatherBlockingFunctions gather;
  for_alist(stmt, block->body) {
    stmt->accept(&gather);
  }
  MayBlockState state = gather.finalState();

  if (fReportBlocking)
    if (developer || printsUserLocation(block))
      USR_PRINT(block, "loopContainsBlocking = %s",
                blockStateString(state));

  return (state & STATE_MAYBE_BLOCKING) || (state & STATE_EXTERN_ANYTHING);
}

static bool isTaskFunOrWrapper(FnSymbol* fn) {
  return fn->hasFlag(FLAG_ON) ||
         fn->hasFlag(FLAG_ON_BLOCK) ||
         fn->hasFlag(FLAG_NON_BLOCKING) ||
         fn->hasFlag(FLAG_COBEGIN_OR_COFORALL) ||
         fn->hasFlag(FLAG_COBEGIN_OR_COFORALL_BLOCK) ||
         fn->hasFlag(FLAG_BEGIN) ||
         fn->hasFlag(FLAG_BEGIN_BLOCK) ||
         fn->hasFlag(FLAG_LOCAL_ON);
}

static MayBlockState mayBlock(FnSymbol* fn) {
  MayBlockState& state = fnMayBlock[fn];
  if (state == STATE_UNKNOWN) {
    if (fn->hasFlag(FLAG_FN_SYNCHRONIZATION_FREE) ||
        fn->hasFlag(FLAG_FN_UNORDERED_SAFE) ||
        fn->hasFlag(FLAG_COMPILER_ADDED_REMOTE_FENCE)) {
      state = STATE_COMPUTED;
    } else if (fn->hasFlag(FLAG_FUNCTION_TERMINATES_PROGRAM)) {
      // No need for such a function to impede optimization
      // TODO: this could apply also to other code leading to the halt
      state = STATE_COMPUTED;
    } else if (fn->hasFlag(FLAG_TASK_SPAWN_IMPL_FN)) {
      // up end count functions are associated with creating tasks
      state = STATE_CREATES_TASKS;
    } else if (fn->hasFlag(FLAG_TASK_COMPLETE_IMPL_FN)) {
      // associated with completing the current task
      state = STATE_COMPLETES_TASKS;
    } else if (fn->hasFlag(FLAG_TASK_JOIN_IMPL_FN)) {
      // wait end count functions are associated with joining with tasks
      state = STATE_JOINS_TASKS;
    } else if (fn->hasFlag(FLAG_EXTERN)) {
      ModuleSymbol* inModule = fn->defPoint->getModule();
      if (inModule->hasFlag(FLAG_ATOMIC_MODULE))
        state = STATE_USES_ATOMICS;
     else if (0 == strcmp(fn->name, "chpl_task_yield"))
        state = STATE_YIELDS_TASKS;
     //else if (0 == strcmp(fn->name, "chpl_comm_task_end"))
     //   state = STATE_COMPLETES_TASKS;
     else
        state = STATE_EXTERN_ANYTHING;
    } else if (0 == strcmp(fn->name, "dsiAccess")) {
      // Assume array access calls do not block
      // (but may contain a critical section)
      state = STATE_CRITICAL_SECTION;
    /*} else if (0 == strcmp(fn->name, "chpl_save_task_error_owned") ||
               0 == strcmp(fn->name, "chpl_save_task_error")) {
      // These are a normal part of task completion
      state = STATE_COMPLETES_TASKS;*/
    } else {
      int fnstate = STATE_RECURSIVE;
      // Set the state in the map.
      // This is the base case for recursive functions.
      state = (MayBlockState) fnstate;

      GatherBlockingFunctions gather;

      fn->body->accept(&gather);
      fnstate = gather.finalState();

      // STATE_USES_ATOMIC_RECURSIVE just means maybe blocks
      // since the recursion could create a loop
      if ((fnstate & STATE_RECURSIVE) && (fnstate & STATE_USES_ATOMICS))
        fnstate |= STATE_MAYBE_BLOCKING;

      if (fReportBlocking) {
        bool user = (printsUserLocation(fn->defPoint) &&
                     !fn->hasFlag(FLAG_COMPILER_GENERATED) &&
                     !isTaskFunOrWrapper(fn));

        if (user || developer)
          USR_PRINT(fn->defPoint, "%s %s",
                    blockStateString((MayBlockState)fnstate), fn->name);
        if (developer)
          USR_PRINT("fn id %i", fn->id);
      }

      // Remove any recursive portion of the state, since that
      // won't be relevant to callers (we used it above).
      fnstate = fnstate & ~STATE_RECURSIVE;

      // Save it in the map
      state = (MayBlockState) fnstate;
    }
  }
  return state;
}

static MayBlockState combineBlockState(MayBlockState aIn, MayBlockState bIn) {
  int a = aIn;
  int b = bIn;
  int state;

  state = a | b;

  return (MayBlockState) state;
}

GatherBlockingFunctions::GatherBlockingFunctions() {
  blockingLoopStack.push(STATE_COMPUTED);
}

void GatherBlockingFunctions::updateState(MayBlockState newState) {
  INT_ASSERT(!blockingLoopStack.empty());
  MayBlockState& state = blockingLoopStack.top();
  state = combineBlockState(state, newState);
}

MayBlockState GatherBlockingFunctions::finalState() {
  INT_ASSERT(!blockingLoopStack.empty());
  return blockingLoopStack.top();
}

void GatherBlockingFunctions::beginLoop() {
  blockingLoopStack.push(STATE_COMPUTED);
}
void GatherBlockingFunctions::endLoop() {
  INT_ASSERT(!blockingLoopStack.empty());
  int loopState = blockingLoopStack.top();
  blockingLoopStack.pop();

  INT_ASSERT(!blockingLoopStack.empty());

  // Adjust the parent based on what we saw.
  if (loopState & STATE_USES_ATOMICS)
    loopState |= STATE_MAYBE_BLOCKING;

  MayBlockState& state = blockingLoopStack.top();
  state = combineBlockState(state, (MayBlockState)loopState);
}

bool GatherBlockingFunctions::enterCallExpr(CallExpr* node) {
  MayBlockState callstate = STATE_COMPUTED;

  CallExpr* call = node;

  if (call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN)) {
    if (CallExpr* rhsCall = toCallExpr(call->get(2)))
      call = rhsCall;
  }

  if (call->isPrimitive(PRIM_VIRTUAL_METHOD_CALL)) {
    // Consider all of the virtual calls
    // (Note, this assumes whole-program compilation!)

    // Consider whatever was in the call (a base method)
    FnSymbol* fn = call->resolvedOrVirtualFunction();
    callstate = mayBlock(fn);

    // Consider also any methods that could override that one
    if (Vec<FnSymbol*>* childrenFns = virtualChildrenMap.get(fn)) {
      forv_Vec(FnSymbol, childFn, *childrenFns) {
        MayBlockState childstate = mayBlock(childFn);
        callstate = combineBlockState(callstate, childstate);
      }
    }
  } else if (FnSymbol* fn = call->resolvedFunction()) {
    callstate = mayBlock(fn);
  }
  updateState(callstate);
  return false;
}
void GatherBlockingFunctions::exitCallExpr(CallExpr* node) {
}
bool GatherBlockingFunctions::enterWhileDoStmt(WhileDoStmt* node) {
  beginLoop();
  return true;
}
void GatherBlockingFunctions::exitWhileDoStmt(WhileDoStmt* node) {
  endLoop();
}
bool GatherBlockingFunctions::enterDoWhileStmt(DoWhileStmt* node) {
  beginLoop();
  return true;
}
void GatherBlockingFunctions::exitDoWhileStmt(DoWhileStmt* node) {
  endLoop();
}
bool GatherBlockingFunctions::enterCForLoop(CForLoop* node) {
  beginLoop();
  return true;
}
void GatherBlockingFunctions::exitCForLoop(CForLoop* node) {
  endLoop();
}
bool GatherBlockingFunctions::enterForLoop(ForLoop* node) {
  beginLoop();
  return true;
}
void GatherBlockingFunctions::exitForLoop(ForLoop* node) {
  endLoop();
}


// ---- the main optimization that runs around LICM

static
CallExpr* findMarkerNear(Expr* stmt) {
 for (Expr* cur = stmt; cur != NULL; cur = cur->next) {
  if (CallExpr* call = toCallExpr(cur))
    if (call->isPrimitive(PRIM_OPTIMIZATION_INFO))
      return call;
  }
  return NULL;
}

static const char* optimizableFunctionTable[] =
 {
   "chpl_comm_atomic_add_", "chpl_comm_atomic_add_unordered_",
   "chpl_comm_atomic_sub_", "chpl_comm_atomic_sub_unordered_",
   "chpl_comm_atomic_and_", "chpl_comm_atomic_and_unordered_",
   "chpl_comm_atomic_or_", "chpl_comm_atomic_or_unordered_",
   "chpl_comm_atomic_xor_", "chpl_comm_atomic_xor_unordered_",
   // These are for optimization reporting purposes
   "atomic_fetch_add_explicit_", NULL,
   "atomic_fetch_sub_explicit_", NULL,
   "atomic_fetch_and_explicit_", NULL,
   "atomic_fetch_or_explicit_", NULL,
   "atomic_fetch_xor_explicit_", NULL,
   // These indicate to code using this table that the end is reached
   NULL, NULL };

static bool isOptimizableAtomicFunction(const char* cname) {
  for (int i=0; optimizableFunctionTable[i]; i+=2) {
    const char* from = optimizableFunctionTable[i];
    const char* to = optimizableFunctionTable[i+1];
    if (startsWith(cname, from)) {
      // Pass this test for the purpose of optimization testing
      // (we won't actually transform it later)
      if (to == NULL)
        return true;

      // Otherwise, optimize only if it's not already unordered
      if (!startsWith(cname, to))
        return true;
    }
  }

  return false;
}

static bool isOptimizableAtomicStmt(Expr* stmt, BlockStmt* loop) {
  // If this were to change, we'd need conditionals below to check
  // that the call is not within a PRIM_MOVE, since we can't do the
  // optimization if the return value is used.
  INT_ASSERT(stmt == stmt->getStmtExpr());

  Symbol* refAtomic = NULL;
  if (CallExpr* call = toCallExpr(stmt)) {
    if (FnSymbol* fn = call->resolvedFunction()) {
      if (fn->hasFlag(FLAG_EXTERN)) {
        if (isOptimizableAtomicFunction(fn->cname)) {
          if (startsWith(fn->cname, "chpl_comm_atomic_")) {
            refAtomic = toSymExpr(call->get(3))->symbol();
          } else {
            refAtomic = toSymExpr(call->get(1))->symbol();
          }
        }
      }
    }
  }

  if (refAtomic != NULL)
    if (BlockStmt* defInBlock = toBlockStmt(refAtomic->defPoint->parentExpr))
      if (isBlockWithinBlock(defInBlock, loop))
        if (CallExpr* marker = findMarkerNear(stmt))
          if (hasOptimizationFlag(marker, OPT_INFO_LHS_OUTLIVES_FORALL) &&
              hasOptimizationFlag(marker, OPT_INFO_FLAG_NO_TASK_PRIVATE))
            if (loopContainsBlocking(loop) == false)
              return true;

  return false;
}

static const char* getUnorderedAtomicFunction(const char* cname) {

  for (int i=0; optimizableFunctionTable[i]; i+=2) {
    if (startsWith(cname, optimizableFunctionTable[i])) {
      const char* newPrefix = optimizableFunctionTable[i+1];
      if (newPrefix == NULL)
        return NULL;
      size_t len = strlen(optimizableFunctionTable[i]);
      const char* suffix = cname + len;
      return astr(newPrefix, suffix);
    }
  }

  return NULL;
}


// atomic functions
// cname -> FnSymbol*
static std::map<const char*, FnSymbol*> atomicFns;

static void transformAtomicStmt(Expr* stmt) {
  CallExpr* call = toCallExpr(stmt);
  FnSymbol* oldFn = call->resolvedFunction();
  const char* newFnCName = getUnorderedAtomicFunction(oldFn->cname);
  FnSymbol* newFn = NULL;

  if (fReportOptimizeForallUnordered)
    USR_PRINT(call, "Optimized atomic call to be unordered");

  // We might have newFnCName == NULL if we just wanted
  // to test the compiler optimization but there is no
  // runtime support / value in the optimization
  if (newFnCName == NULL)
    return;

  // Now lookup up newFn in the map
  if (atomicFns.count(newFnCName) > 0) {
    newFn = atomicFns[newFnCName];
  } else {
    SET_LINENO(oldFn);
    // Duplicate oldFn and change its cname
    INT_ASSERT(oldFn->hasFlag(FLAG_EXTERN));
    newFn = oldFn->copy();
    newFn->cname = newFnCName;
    newFn->name = newFnCName;
    oldFn->defPoint->insertAfter(new DefExpr(newFn));

    // Remove a memory_order argument if present
    ArgSymbol* orderFormal = NULL;
    for_formals(formal, newFn) {
      if (formal->typeInfo()->symbol->hasFlag(FLAG_C_MEMORY_ORDER_TYPE))
        orderFormal = formal;
    }
    INT_ASSERT(orderFormal);
    orderFormal->defPoint->remove();

    // Add it to our map
    atomicFns[newFnCName] = newFn;
  }

  SET_LINENO(call);

  // Remove redundant remote-memory fences from --cache-remote.
  if (fCacheRemote) {
    // remove the chpl_rmem_consist_maybe_release added before the atomic
    // This fence is redundant because we add a full fence immediately below.
    for (Expr* cur = call->getStmtExpr()->prev; cur != NULL; cur = cur->prev) {
      if (CallExpr* call = toCallExpr(cur)) {
        if (FnSymbol* fn = call->resolvedFunction()) {
          if (fn->hasFlag(FLAG_COMPILER_ADDED_REMOTE_FENCE))
            cur->remove();
          // if we encountered a function call, including the added fence, stop
          break;
        }
      }
    }
    // remove the chpl_rmem_consist_maybe_acquire added after the atomic
    // This fence is redundant because we add a full fence immediately below
    // *and* because the optimization fired, we know that the atomic op
    // we are making unordered is not being used to communicate to the current
    // task.
    for (Expr* cur = call->getStmtExpr()->next; cur != NULL; cur = cur->next) {
      if (CallExpr* call = toCallExpr(cur)) {
        if (FnSymbol* fn = call->resolvedFunction()) {
          if (fn->hasFlag(FLAG_COMPILER_ADDED_REMOTE_FENCE))
            cur->remove();
          // if we encountered a function call, including the added fence, stop
          break;
        }
      }
    }
  }

  // TODO: This could just be a release fence (and then we could do an
  // acquire fence at the end of the forall loop in addition to just
  // completing the unordered ops). That could help in 2 cases:
  //  1. If --cache-remote is used, we can still use cached GETs
  //     during the unordered loop
  //  2. If we improve the unordered ops the compiler is targeting to use
  //     other methods that aggregate, within-thread performance might
  //     matter & optimizing the fences could allow better compiler opt.

  // Add a fence call before the call.
  // First, gather the memory order argument from the call.
  // The loop below finds the last argument that is a memory_order.
  Expr* orderActual = NULL;
  for_actuals(actual, call) {
    if (actual->typeInfo()->symbol->hasFlag(FLAG_C_MEMORY_ORDER_TYPE))
      orderActual = actual;
  }
  INT_ASSERT(orderActual);

  call->insertBefore(new CallExpr(gAtomicFenceFn, orderActual->remove()));

  // Change the call's base expression to refer to newFn
  SymExpr* se = toSymExpr(call->baseExpr);
  INT_ASSERT(se && se->symbol() == oldFn);
  se->setSymbol(newFn);

}

static bool isOptimizableAssignStmt(Expr* stmt, BlockStmt* loop) {
  Symbol* lhs = NULL;
  if (CallExpr* call = toCallExpr(stmt))
    if (call->isPrimitive(PRIM_ASSIGN))
      if (SymExpr* lhsSe = toSymExpr(call->get(1)))
        lhs = lhsSe->symbol();

  if (lhs)
    if (BlockStmt* defInBlock = toBlockStmt(lhs->defPoint->parentExpr))
      if (isBlockWithinBlock(defInBlock, loop))
        if (CallExpr* marker = findMarkerNear(stmt))
          if (hasOptimizationFlag(marker, OPT_INFO_LHS_OUTLIVES_FORALL) &&
              hasOptimizationFlag(marker, OPT_INFO_FLAG_NO_TASK_PRIVATE))
            return true;

  return false;
}


static void transformAssignStmt(Expr* stmt) {
  SET_LINENO(stmt);

  CallExpr* call = toCallExpr(stmt);

  INT_ASSERT(call->isPrimitive(PRIM_ASSIGN));

  Symbol* lhs = toSymExpr(call->get(1))->symbol();
  Expr* rhs = call->get(2);
  CallExpr* callToRemove = NULL;

  if (isSymExpr(rhs) && rhs->isRef() == false) {
    // Find a pattern like
    //
    // move rhs PRIM_DEREF rhsRef
    // PRIM_ASSIGN lhs rhs
    //
    // in that event, we'll fold the PRIM_DEREF into the getput call
    // so conceptually do this pre-transformation to
    //
    // PRIM_ASSIGN lhs rhsRef
    //
    Symbol* rhsSym = toSymExpr(rhs)->symbol();
    Symbol* rhsRef = NULL;
    CallExpr* prevCall = toCallExpr(call->prev);
    if (prevCall != NULL) {
      if (prevCall->isPrimitive(PRIM_MOVE) ||
          prevCall->isPrimitive(PRIM_ASSIGN)) {
        Symbol* prevLhs = toSymExpr(prevCall->get(1))->symbol();
        if (prevLhs == rhsSym) {
          if (CallExpr* rhsCall = toCallExpr(prevCall->get(2))) {
            if (rhsCall->isPrimitive(PRIM_DEREF))
              rhsRef = toSymExpr(rhsCall->get(1))->symbol();
          } else if (SymExpr* rhsSe = toSymExpr(prevCall->get(2))) {
            if (rhsSe->symbol()->isRef() && prevCall->isPrimitive(PRIM_ASSIGN))
              rhsRef = rhsSe->symbol();
          }
        }
      }
    }

    if (rhsRef != NULL && prevCall != NULL) {
      callToRemove = prevCall;
      rhs = new SymExpr(rhsRef);
    }
  }

  if (lhs->isRef() && rhs->isRef()) {
    // add the call to getput
    if (fReportOptimizeForallUnordered) {
      if (developer || printsUserLocation(call)) {
        USR_PRINT(call, "Optimized assign to be unordered");
      }
    }

    call->insertBefore(new CallExpr(PRIM_UNORDERED_ASSIGN, lhs, rhs->copy()));
    call->remove();
    if (callToRemove)
      callToRemove->remove();
  }
}


void optimizeForallUnorderedOps() {

  if (fReportBlocking) {
    // Do blocking analysis on all user functions
    // (analysis will print out result as it is computed)
    forv_Vec(FnSymbol, fn, gFnSymbols) {
      ModuleSymbol* mod = fn->defPoint->getModule();
      if (printsUserLocation(fn->defPoint) &&
          !fn->hasFlag(FLAG_COMPILER_GENERATED) &&
          fn != mod->initFn &&
          fn != mod->deinitFn) {
        mayBlock(fn);
      }
    }
  }

  if (fNoOptimizeForallUnordered)
    return;

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    // Does it start with chpl_comm_atomic_? Add to map.
    if (fn->hasFlag(FLAG_EXTERN) &&
        startsWith(fn->cname, "chpl_comm_atomic_")) {
      atomicFns[fn->cname] = fn;
    }
  }

  std::vector<Expr*> atomicsToOptimize;
  std::vector<Expr*> assignsToOptimize;

  // Gather expressions to optimize. This is done separately from
  // doing the transformation so that the transformation itself does
  // not interfere with the legality analysis.
  // (It would be OK to do this analyze/transform per function or
  //  per module.)
  forv_Vec(BlockStmt, block, gBlockStmts) {
    if (block->isLoopStmt()) {
      LoopStmt* loop = toLoopStmt(block);

      if (loop->isOrderIndependent()) {
        std::vector<Expr*> lastStmts;
        getLastStmts(loop, lastStmts);
        for_vector(Expr, lastStmt, lastStmts) {
          if (isOptimizableAtomicStmt(lastStmt, loop))
            atomicsToOptimize.push_back(lastStmt);
          else if (isOptimizableAssignStmt(lastStmt, loop))
            assignsToOptimize.push_back(lastStmt);
        }
      }
    }
  }

  // Now apply the transformation
  for_vector(Expr, atomic, atomicsToOptimize) {
    transformAtomicStmt(atomic);
  }
  for_vector(Expr, assign, assignsToOptimize) {
    transformAssignStmt(assign);
  }
}
