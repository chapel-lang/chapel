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

#include "iterator.h"

#include "astutil.h"
#include "bb.h"
#include "bitVec.h"
#include "CForLoop.h"
#include "driver.h"
#include "expr.h"
#include "ForLoop.h"
#include "oldCollectors.h"
#include "optimizations.h"
#include "passes.h"
#include "preFold.h"
#include "resolution.h"
#include "resolveFunction.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "view.h"
#include "WhileStmt.h"

#include <map>
#include <vector>

//
// This file implements lowerIterator() called by the lowerIterators pass
//
// lowerIterator() rewrites the original iterator function to fill in
// the iterator record and build the various functions needed to build
// the final loop: zip*, hasMore, advance, getValue, and getIterator.
//

//#define DEBUG_LIVE


IteratorInfo::IteratorInfo() :
  iterator(NULL),
  getIterator(NULL),
  iclass(NULL),
  irecord(NULL),
  advance(NULL),
  zip1(NULL),
  zip2(NULL),
  zip3(NULL),
  zip4(NULL),
  hasMore(NULL),
  getValue(NULL),
  init(NULL),
  incr(NULL)
{}

// Actions upon deleting a FnSymbol.
void cleanupIteratorInfo(FnSymbol* host) {
  IteratorInfo* iteratorInfo = host->iteratorInfo;

  if (iteratorInfo && ! host->hasFlag(FLAG_TASK_FN_FROM_ITERATOR_FN)) {
    // Also set iterator class and iterator record iteratorInfo = NULL.
    if (iteratorInfo->iclass)
      iteratorInfo->iclass->iteratorInfo = NULL;

    if (iteratorInfo->irecord)
      iteratorInfo->irecord->iteratorInfo = NULL;

    delete iteratorInfo;
  }
}


//
// Iterator Groups
//

/*
The following properties hold after a call is resolved to an iterator "IT".
Implemented by resolveAlsoParallelIterators(). 

ANY ITERATOR

* Iterator <-> IG is a many-to-one relationship.
  It is stored in Iterator->iteratorGroup for each Iterator in IG.

  Given a group IG, IT2->iteratorGroup==IG for each iterator IT2
  that IG points to: IG.serial, IG.standalone (when non-NULL), etc.

  IG.serial is always non-NULL.
  
* An iterator IT2 (any flavor) is pointed to from a group IG
  if and only if IT2->iteratorGroup == IG.

SERIAL / STANDALONE / LEADER

* If IT is a serial iterator, there is an iterator group IG for it
  such that:
   - IG.serial points to IT.
   - IG.standalone points to the corresponding standalone iterator
     if it is available, NULL otherwise.
   - IG.leader - ditto.
   - IG.follower is currently unused.

* If a serial IT has no corresponding standalone or leader,
  there is still a group IG for IT, such that:
   - IG.serial == IT
   - IG.standalone == IG.leader == NULL.
  This can be used to check availability of standalone/leader.

* If IT is a standalone or leader iterator, there may or may not be
  a group for it. We are interested in a group only when it maps
  from a serial iterator to its parallel counterparts. So when
  a CallExpr invokes a parallel iterator directly, iterator groups
  are not updated.

SERIAL --> PARALLEL

* Each parallel iterators is attempted to be resolved by resolving
  a "representative call". It is created as follows:
   - It seeks a function of the same name as the serial iterator's.
   - The actual arguments are the serial iterator's formals, plus 'tag'.

* That way there is a 1:1 relationship between the serial iterator
  its standalone counterpart; ditto leader. Because the choice
  of the parallel iterator is affected only by the serial iterator.
  This choice is not affected by the "original call" i.e. one that
  led to resolving the serial iterator.

* The "representative call" is placed next to the "original call".
  To strengthen confidence in this 1:1 relationship, it seems like
  the representative call should go next to the definition
  of the serial iterator. However, that would cause visibility issues.

LOGISTICS  

* Only the availability of the standalone and leader iterators
  is detected. Their bodies are not resolved, to avoid encountering
  (and generating) potential compile-time errors when those iterators
  are not actually used in the program.

* The absence of a parallel iterator is not an error. An error may
  be issued later when+if this iterator is required for a parallel
  computation.

* The follower iterator is not sought at this time. To seek it we would
  need to know what the leader iterator yields. For that we may have to
  resolve the **body** of the leader. Which we are not doing here.
*/

IteratorGroup::IteratorGroup() :
  serial(NULL), standalone(NULL), leader(NULL), follower(NULL),
  noniterSA(false), noniterL(false)
{}

static bool isIteratorOrForwarder(FnSymbol* it) {
  // The test 'it->retType->symbol->hasFlag(FLAG_ITERATOR_RECORD)'
  // gives a false negative when it->retType is "unknown"
  // or a false positive for chpl__autoCopy(_iteratorRecord).
  // FLAG_FN_RETURNS_ITERATOR is not a great test either because
  // iteratorIndex() has it whereas it usually doesn't.

  return it->hasFlag(FLAG_ITERATOR_FN) ||
         it->hasFlag(FLAG_FN_RETURNS_ITERATOR);
}

// Look for the iterator for 'iterKindTag' and update the iterator group.
static void checkParallelIterator(FnSymbol* serial, Expr* call,
                                  Symbol* iterKindTag, IteratorGroup* igroup,
                                  FnSymbol*& outParIter, bool& noniterFlag)
{
  // Build a "representative call".
  CallExpr* repCall = new CallExpr(new UnresolvedSymExpr(serial->name));

  // Use the formals of 'serial', for the purposes of resolution.
  for_formals(formal, serial) {
    if (formal->name != astrTag)
      repCall->insertAtTail(
        new NamedExpr(formal->name, createSymExprPropagatingParam(formal)) );
  }

  // Add the tag argument.
  repCall->insertAtTail(new NamedExpr(astrTag, new SymExpr(iterKindTag)));

  // Wrap in a block, to contain possible side-effects of resolving.
  BlockStmt* container = new BlockStmt(repCall);
  call->getStmtExpr()->insertAfter(container);

  if (FnSymbol* parIter = tryResolveCall(repCall)) {
    // Got it.
    if (isIteratorOrForwarder(parIter)) {
      outParIter = parIter;
      parIter->iteratorGroup = igroup;

    } else {
      // If this is not an iterator, do not record it.
      // We may need to raise an error later.
      noniterFlag = true;
    }
  }

  container->remove();
}

// See if any parallel iterators are available.
void resolveAlsoParallelIterators(FnSymbol* serial, Expr* call) {
  if (! isIteratorOrForwarder(serial)) return;  // not of interest
  if (serial->iteratorGroup != NULL) return;  // already taken care of

  if (serial->hasFlag(FLAG_INLINE_ITERATOR))
    // 'serial' is actually a parallel iterator. Since we did not come
    // from a serial iterator, we will not update iterator groups.
    return;

  if (! serial->isIterator())
    // The above "is parallel" check does not fire on iterator forwarders.
    // So check for 'tag' formals instead, like in isIteratorOfType().
    for_formals(formal, serial)
      if (formal->name == astrTag && formal->type == gLeaderTag->type)
        return;

  IteratorGroup* igroup = new IteratorGroup();
  igroup->serial = serial;
  serial->iteratorGroup = igroup;

  checkParallelIterator(serial, call, gStandaloneTag, igroup,
                        igroup->standalone, igroup->noniterSA);

  checkParallelIterator(serial, call, gLeaderTag, igroup,
                        igroup->leader, igroup->noniterL);
}

static inline void verifyIGfunction(IteratorGroup* igroup, FnSymbol* fn) {
  if (fn != NULL)
    INT_ASSERT(fn->iteratorGroup == igroup);
}

void verifyIteratorGroup(FnSymbol* it) {
  IteratorGroup* igroup = it->iteratorGroup;
  if (! igroup) return;

  INT_ASSERT(it == igroup->serial     ||
             it == igroup->standalone ||
             it == igroup->leader     ||
             it == igroup->follower);

  // Independent of 'it'.
  INT_ASSERT(igroup->serial != NULL);
  verifyIGfunction(igroup, igroup->serial);
  verifyIGfunction(igroup, igroup->standalone);
  verifyIGfunction(igroup, igroup->leader);
  verifyIGfunction(igroup, igroup->follower);
}

static inline void cleanupIGfunction(FnSymbol* it, FnSymbol*& parIterInIG) {
  if (it == parIterInIG) {
    parIterInIG = NULL;
    it->iteratorGroup = NULL;
  }
}

// Actions upon deleting a FnSymbol.
void cleanupIteratorGroup(FnSymbol* it) {
  IteratorGroup* igroup = it->iteratorGroup;
  if (! igroup) return;

  it->iteratorGroup = NULL;

  bool deleteIG = false;

  if (it == igroup->serial) {
    // Without the serial iterator, the group is of no interest.
    deleteIG = true;
    if (FnSymbol* SA = igroup->standalone) SA->iteratorGroup = NULL;
    if (FnSymbol* L  = igroup->leader)     L->iteratorGroup  = NULL;
    if (FnSymbol* F  = igroup->follower)   F->iteratorGroup  = NULL;
  } else {
    cleanupIGfunction(it, igroup->standalone);
    cleanupIGfunction(it, igroup->leader);
    cleanupIGfunction(it, igroup->follower);
  }

  if (deleteIG ||  // or if nobody remains in the group:
      (igroup->serial     == NULL &&
       igroup->standalone == NULL &&
       igroup->leader     == NULL &&
       igroup->follower   == NULL )
  ) {
    delete igroup;
  }
}

// showIteratorGroup() - for debugging

static void showIGhelp(IteratorGroup* igroup, FnSymbol* fn, const char* kind)
{
  if (fn == NULL) {
    printf("  %s -\n", kind);
  } else {
    printf("  %s  %s[%d]  ", kind, fn->name, fn->id);
    if (IteratorGroup* fnIG = fn->iteratorGroup) {
      if (fnIG == igroup) printf("+\n");
      else printf("ig ((IteratorGroup*)%p)\n", fnIG);
    } else {
      printf("no ig\n");
    }
  }
}

static void showIGhelp(IteratorGroup* igroup, FnSymbol* fn) {
  if (fn) printf("%s %s[%d]  ",
                 fn->isIterator() ? "iter" : "proc",
                 fn->name, fn->id);
  printf("igroup %p\n", igroup);

  if (igroup != NULL) {
    showIGhelp(igroup, igroup->serial,     "serial");
    showIGhelp(igroup, igroup->standalone, "standalone");
    showIGhelp(igroup, igroup->leader,     "leader");
    if (igroup->follower != NULL)  // currently unexpected
      showIGhelp(igroup, igroup->follower, "??follower");
  }
}

void showIteratorGroup(IteratorGroup* igroup) {
  showIGhelp(igroup, NULL);
}

void showIteratorGroup(BaseAST* ast) {
  if (ast == NULL)
    printf("<showIteratorGroup: ast==NULL>\n");
  else if (FnSymbol* fn = toFnSymbol(ast))
    showIGhelp(fn->iteratorGroup, fn);
  else
    printf("<showIteratorGroup: node %d is a %s, not a FnSymbol>\n",
           ast->id, ast->astTagAsString());
}

void showIteratorGroup(int id) {
  BaseAST* aid(int id);
  showIteratorGroup(aid(id));
}


//
// Helpers
//

static void skipIBBinAlist(Expr*& _alist_next) {
  INT_ASSERT(isIBBCondStmt(_alist_next));
  // If I had a C++-style iterator, I would just say "it++;".
  _alist_next = _alist_next->next;
}

// If this is an IBB, return its CondStmt. Otherwise return NULL.
CondStmt* isIBBCondStmt(BaseAST* ast) {
  if (CondStmt* condStmt = toCondStmt(ast))
    if (SymExpr* condSE = toSymExpr(condStmt->condExpr))
      if (condSE->symbol() == gIteratorBreakToken)
        return condStmt;
  return NULL;
}

// If this is in an IBB, return its CondStmt. Otherwise return NULL.
static CondStmt* isInIBBCondStmt(Expr* expr) {
  for (Expr* parent = expr->parentExpr; parent;
       expr = parent, parent = parent->parentExpr)
    if (CondStmt* IBB = isIBBCondStmt(parent))
      if (expr == IBB->thenStmt)
        return IBB;
  return NULL;
}


// Return the PRIM_YIELD CallExpr* or NULL.
static inline CallExpr* asYieldExpr(BaseAST* e) {
  if (CallExpr* call = toCallExpr(e))
    if (call->isPrimitive(PRIM_YIELD))
      return call;
  return NULL;
}
static inline CallExpr* parentYieldExpr(SymExpr* se) {
  return asYieldExpr(se->parentExpr);
}


//
// Now that we have localized yield symbols, the return symbol
// and the PRIM_RETURN CallExpr are not needed and would cause trouble.
// Returns the type yielded by the iterator. (fn->retType is not it.)
//
static Type*
removeRetSymbolAndUses(FnSymbol* fn) {
  // follows getReturnSymbol()
  CallExpr* ret = toCallExpr(fn->body->body.last());
  INT_ASSERT(ret && ret->isPrimitive(PRIM_RETURN));

  // Since iterator returns might have been already translated to
  // assignments to a return argument, remove those assignments.
  if (CallExpr* assign = toCallExpr(ret->prev))
    if (assign->isPrimitive(PRIM_MOVE) || assign->isPrimitive(PRIM_ASSIGN))
      if (SymExpr* se = toSymExpr(assign->get(1)))
        if (se->symbol()->hasFlag(FLAG_RETARG))
          assign->remove();

  // Yank the return statement.
  ret->remove();

  // We cannot remove rsym's definition, because rsym
  // may also be referenced in an autoDestroy call.

  INT_ASSERT(fn->iteratorInfo != NULL);
  Type* yieldedType = fn->iteratorInfo->yieldedType;

  return yieldedType;
}


//
// Handle the shape of the yielded values.
//

// add "proc ir._fromForExpr_ param return true;"
static void addIteratorFromForExpr(Expr* ref, Symbol* ir) {
  SET_LINENO(ref);
  FnSymbol* fn = new FnSymbol("_fromForExpr_");
  fn->addFlag(FLAG_COMPILER_GENERATED);
  fn->addFlag(FLAG_METHOD);
  fn->addFlag(FLAG_NO_PARENS);
  fn->retTag = RET_PARAM;
  fn->retType = dtBool;
  fn->setMethod(true);
  
  ArgSymbol* mtArg = new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken);
  ArgSymbol* irArg = new ArgSymbol(INTENT_BLANK, "this", ir->type);
  irArg->addFlag(FLAG_ARG_THIS);
  fn->_this = irArg;

  fn->insertFormalAtTail(mtArg);
  fn->insertFormalAtTail(irArg);
  fn->insertAtTail("'return'(%S)", gTrue);

  // Ideally, want to put the DefExpr(fn) next to ir->type->symbol->defPoint.
  // Using theProgram->block instead sidesteps a visibility issue
  // for a for-expression that is the default value for a field. Ex.:
  //   classes/initializers/compilerGenerated/omittedLoopExpr
  theProgram->block->insertAtTail(new DefExpr(fn));
  toAggregateType(ir->type)->methods.add(fn);
  resolveFunction(fn);
}

// return the result of "chpl_iteratorFromForExpr(ir)"
bool checkIteratorFromForExpr(Expr* ref, Symbol* shape) {
  CallExpr* checkCall = new CallExpr("chpl_iteratorFromForExpr", shape);
  BlockStmt* holder = new BlockStmt(BLOCK_SCOPELESS);
  holder->insertAtTail(checkCall);
  ref->insertAfter(holder);

  resolveCall(checkCall);
  FnSymbol* checkFn = checkCall->resolvedFunction();
  resolveFunction(checkFn);
  holder->remove();
  
  Symbol* checkResult = checkFn->getReturnSymbol();

  // chpl_iteratorFromForExpr() is a param boolean function
  return getSymbolImmediate(checkResult)->bool_value();
}

//
// Insert before 'ref':
//   temp = chpl_computeIteratorShape(shapeSpec);
//   ir._shape_ = temp;
//
// Add _shape_ field to ir.type and figure out its type,
// if the field did not exist.
//
CallExpr* setIteratorRecordShape(Expr* ref, Symbol* ir, Symbol* shapeSpec,
                                 bool fromForExpr) {
  // We could skip this if the field already exists and is void.
  // It might be better to insert these anyway for uniformity.
  VarSymbol* value  = newTemp("shapeTemp");
  CallExpr* compute = new CallExpr("chpl_computeIteratorShape", shapeSpec);
  CallExpr* set     = new CallExpr(PRIM_MOVE, value, compute);
  ref->insertBefore(new DefExpr(value));
  ref->insertBefore(set);
  resolveExpr(compute);
  resolveExpr(set);

  AggregateType* iRecord = toAggregateType(ir->type);
  INT_ASSERT(iRecord->symbol->hasFlag(FLAG_ITERATOR_RECORD));
  Symbol* field = iRecord->getField("_shape_", false);
  if (field == NULL) {
    field = new VarSymbol("_shape_", value->type);
    iRecord->fields.insertAtTail(new DefExpr(field));
    // An accessor lets us get _shape_ in Chapel code.
    FnSymbol* accessor = build_accessor(iRecord, field, false, false);
    accessor->setGeneric(false);
    // This sidesteps the visibility issue in the presence of nested
    // LoopExprs. Ex. test/expressions/loop-expr/scoping.chpl
    theProgram->block->insertAtTail(accessor->defPoint->remove());
    if (fromForExpr)
      addIteratorFromForExpr(ref, ir);
  } else {
    INT_ASSERT(field->type == value->type);
  }
  INT_ASSERT(fromForExpr || !checkIteratorFromForExpr(ref, ir));

  return new CallExpr(PRIM_SET_MEMBER, ir, field, value);
}

//
// Replaces the primitive 'call' with the above.
// Ex. test/arrays/return/returnArbitrary*
//
void setIteratorRecordShape(CallExpr* call) {
  INT_ASSERT(call->isPrimitive(PRIM_ITERATOR_RECORD_SET_SHAPE));

  // Keep in sync with the PRIM_ITERATOR_RECORD_SET_SHAPE case in preFold.
  Symbol* ir = toSymExpr(call->get(1))->symbol();
  INT_ASSERT(ir->type->symbol->hasFlag(FLAG_ITERATOR_RECORD));
  Symbol* shapeSpec = toSymExpr(call->get(2))->symbol();
  Symbol* fromForLoop = toSymExpr(call->get(3))->symbol();
  CallExpr* shapeCall = setIteratorRecordShape(call, ir, shapeSpec,
                          getSymbolImmediate(fromForLoop)->bool_value());
  call->replace(shapeCall);
}

// Find the parent block that is either
//  * a loop
//  * or, not a BlockStmt
// Used to ignore non-loop BlockStmts
// (these come up in particular with local and unlocal blocks).
static Expr* loopOrNonBlockParent(Expr* expr) {
  Expr* parent = expr->parentExpr;
  while (parent != NULL) {
    if (!isBlockStmt(parent))
      break;
    if (isLoopStmt(parent))
      break;

    parent = parent->parentExpr;
  }
  return parent;
}

//
// Determines that an iterator has a single loop with a single yield
// in it by checking the following conditions:
//
//   1. There is exactly one for-loop and no other loops.
//   2. The single for-loop is top-level to the function.
//   3. There is exactly one yield.
//   4. The single yield is top-level to the for-loop.
//   5. There are no goto statements.
//
// I believe these conditions can be relaxed.
//
CallExpr*
isSingleLoopIterator(FnSymbol* fn, Vec<BaseAST*>& asts) {
  if (fNoOptimizeLoopIterators)
    return NULL;
  BlockStmt* singleFor = NULL;
  CallExpr* singleYield = NULL;
  forv_Vec(BaseAST, ast, asts) {
    // If a yield statement,
    if (CallExpr* call = toCallExpr(ast)) {
      if (call->isPrimitive(PRIM_YIELD)) {
        if (singleYield) {
          // We already saw a yield stmt.  This is the second one, so fail.
          return NULL;
        }

        // Select yield statements whose parent expression is a loop statement
        // (except for dowhile statements, for some reason....

        // Find the parent block that is either
        //  * a loop
        //  * or, a conditional
        // Ignore non-loop BlockStmts
        // (conditionals could probably be allowed if both sides yielded)
        Expr* parent = loopOrNonBlockParent(call);

        // This test is not logically related to the preceding quick-exit, so
        // putting "else" here would be misleading.
        if (isLoopStmt(parent)) {
          // NOAKES 2014/11/25  It is interesting the DoWhile loops aren't supported
          if (isDoWhileStmt(call->parentExpr))
            return NULL;

          // We expect that ParamForLoops have already been removed from the tree
          if (isParamForLoop(call->parentExpr))
          {
            INT_FATAL(call->parentExpr, "Unexpected ParamForLoop construct.");
          }

          singleYield = call;
        } else {
          return NULL;
        }
      }
    }

    // This clause captures the first loop statement (except for while-do
    // statements, for some reason ...).
    else if (isLoopStmt(ast)) {
      // NOAKES 2014/11/25  It is interesting the DoWhile loops aren't supported
      if (isDoWhileStmt(ast))
        return NULL;

      // We expect that ParamForLoops have already been removed from the tree
      if (isParamForLoop(ast))
      {
        INT_FATAL(ast, "Unexpected ParamForLoop construct.");
      }

      Expr*      expr  = toExpr(ast);
      BlockStmt* block = toBlockStmt(ast);
      Expr*     parent = loopOrNonBlockParent(expr);

      if (parent == NULL && expr->parentSymbol == fn) {
        // This captures the first loop statement, but does not fail if there
        // is more than one.  Compare the test for a single yield above.
        // Is this intentional?
        if (singleFor == NULL)
          singleFor = block;
        // 2015-02-23 hilde: TODO: Uncomment the following, and see what breaks
//        else
//          INT_FATAL(expr, "Iterator contains a second for loop.")
        // I think the existing code works because each loop should contain at
        // least one yield, so the second yield causes the singleYield test
        // above to fail and return NULL preemptively.  Bad news if that
        // assumption fails.
        // This question would not arise if the search were rewritten in a more
        // straightforward fashion: First, look for a single loop; then, within
        // that single loop, look for a single yield.  Simple, obvious,
        // foolproof.
        // Also, probably the redundant code would be removed.
      } else {
        return NULL;
      }
    }

    // If the iterator  contains a goto statement, it is not considered to be a
    // single loop iterator.
    else if (GotoStmt* gt = toGotoStmt(ast)) {
      if (isInIBBCondStmt(gt))
        ; // Gotos in an IBB do not affect our judgment.
          // Should we restrict this case to allow a goto only if it is
          // the last goto in the IBB conditional's then-branch?
      else
        return NULL;
    }
  }

  if (singleFor && singleYield) {
    return singleYield;
  } else {
    return NULL;
  }
}

//  se -- A sym expression which accesses a live local variable.
//  ic -- The iterator class containing the given field.
static void replaceLocalWithFieldTemp(SymExpr*       se,
                                      Symbol*        ic,
                                      Symbol*        field,
                                      bool           is_def,
                                      bool           is_use,
                                      Vec<BaseAST*>& asts)
{
  // Get the expression that sets or uses the symexpr.
  CallExpr* call = toCallExpr(se->parentExpr);

  // Create a new temp and load the field value into it.
  VarSymbol* tmp = newTemp(se->symbol()->qualType());

  // Find the statement containing the symexpr access.
  Expr* stmt = se->getStmtExpr();

  // NOAKES 2014/12/02 This is a brittle way to determine if the
  // SymExpr is in a loop header.  Revisit
  BlockStmt* block = toBlockStmt(stmt);
  BlockStmt* loop  = (block != 0 && block->isLoopStmt()) ? block : 0;

  bool makeRef = false;

  if (call && call->isPrimitive(PRIM_GET_MEMBER)) {
    // Get member returns the address of the member, so we convert the
    // type of the corresponding temp to a reference type.
    makeRef = true;
  }
  if (call && call->isResolved()) {
    // If se is an argument to a function that takes in
    // that argument by a ref concrete intent, make the temporary
    // here a ref to the iterator class field.
    ArgSymbol* arg = actual_to_formal(se);
    INT_ASSERT(arg);
    if (!isReferenceType(tmp->type) &&
        (arg->intent & INTENT_FLAG_REF)) {
      makeRef = true;
    }
  }
  if (ArgSymbol* arg = toArgSymbol(se->symbol())) {
    if (arg->intent & INTENT_FLAG_REF)
      makeRef = true;
  }

  if (makeRef) {
    INT_ASSERT(tmp->type->getRefType());
    tmp->type = tmp->type->getRefType();
  }

  // OK, insert the declaration.
  stmt->insertBefore(new DefExpr(tmp));
  asts.add(tmp->defPoint);  // hilde sez: I don't think this is necessary.

  // If this symexpr is used here,
  if (is_use) {
    // Load the temp from the field before the statement that uses it.
    if (tmp->type == field->type->refType)
      stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER, ic, field)));
    else
      stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, ic, field)));

    // If in a loop, we also have to reload this temp at the bottom of the loop.
    if (loop) {
      if (tmp->type == field->type->refType)
        loop->insertAtTail(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER, ic, field)));
      else
        loop->insertAtTail(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, ic, field)));
    }
  }

  bool add_writeback = false;

  if (is_def ||
      // Currently buildDefUseSets() does not identify PRIM_SET_MEMBER and
      // PRIM_SVEC_SET_MEMBER as defs.
      (call &&
       (call->isPrimitive(PRIM_SET_MEMBER) ||
        call->isPrimitive(PRIM_SET_SVEC_MEMBER)) &&
       call->get(1) == se))
  {
    add_writeback = true;
  }

  // If the temporary is a ref to the iterator class field,
  // there is never a need to add a writeback.
  if (tmp->type == field->type->refType) {
    add_writeback = false;

    // The above code only initializes a reference tmp if is_use is set.
    // This assert will help to identify the issue if that becomes
    // no longer sufficient.
    INT_ASSERT(is_use);
  }

  if (add_writeback) {
    ArgSymbol* arg = toArgSymbol(se->symbol());

    if (arg)
    {
      if (arg->intent == INTENT_INOUT ||
          arg->intent == INTENT_OUT ||
          arg->intent == INTENT_REF) {
        // This is the writeback of a formal temp into its corresponding arg:
        // (move x (= x _formal_tmp_x))
        // Since these arguments have reference types, we use the code
        // (move tmp0 (.v ic field))
        // to load the passed-in pointer-to-actual-arg.  The C dereference
        // operator (*) is applied to variables of ref type during code generation
        // The read of the formal will be replaced by the same code. Thus, we have:
        // (move tmp0 (.v ic field_x))
        // (move tmp1 (.v ic field_x))
        // (move tmp2 (.v ic field_formal_tmp_x))
        // (move tmp0 (= tmp1 tmp2))
        // which is sufficient.
        // The normal code below adds a writeback of the temp result of a call:
        // (= ic field_x tmp0)
        // but that is trivial, since ic->field_x and tmp0 are already equal.
        stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, ic, field)));
      }
      else
      {
        // Arg symbols which do not have a writeback component are not written back.
      }
    } else {
      if (loop) {
        // The writeback (to the class field) occurs at the head of the loop.
        loop->insertAtHead(new CallExpr(PRIM_SET_MEMBER, ic, field, tmp));
        // Why not after as well?
      } else {
        // Otherwise (not a loop)
        // The writeback occurs after the statement.
        stmt->insertAfter(new CallExpr(PRIM_SET_MEMBER, ic, field, tmp));
      }
    }
  }

  // After all of that, the local variable is bridged out and the temp used instead.
  se->setSymbol(tmp);
}


//
// What is oneLocalYS ?
//
// oneLocalYS is set when all PRIM_YIELDs in the iterator
// yield the same Symbol *and* that Symbol is defined locally
// within the iterator.
//
// When oneLocalYS: that Symbol is represented directly by ic.value.
// I.e. the Symbol is mapped directly to valField, in
// local2field, replaceLocalsWithFields, etc.
// ic.value is not updated explicitly upon a yield - because it always
// has the up-to-date value of that Symbol.
//
// When !oneLocalYS: each local Symbol is represented by its own field
// ic.FNN_xxx. Non-local Symbols are not represented in ic's fields.
// Upon each yield, ic.value aka valField is explicitly updated
// with yield's expression.
// E.g. 'yield localvar' is converted to ic.value = ic.FNN_localvar.
//


// In the body of an iterator function, replace references to local variables
// with references to fields in the iterator class instead.
static void
replaceLocalsWithFields(FnSymbol* fn,           // the iterator function
                        Vec<BaseAST*>& asts,    // the asts in that function, listed postorder.
                        SymbolMap& local2field, // Map: local symbol --> class field
                        Vec<Symbol*>& yldSymSet,// The set of locals that appear in yields.
                        Symbol* valField,       // ic.value field - value being yielded.
                        bool oneLocalYS,        // fn's yields has exactly one yield symbol, and it is local.
                        Vec<Symbol*>& locals) { // The list of live locals in the iterator body.

  IteratorInfo* ii = fn->iteratorInfo;
  Symbol* ic = ii->advance->getFormal(1);   // The iterator class

  Vec<SymExpr*> defSet;
  Vec<SymExpr*> useSet;
  buildDefUseSets(locals, fn, defSet, useSet);

  // Traverse the asts in the iterator and select just the sym expressions.
  forv_Vec(BaseAST, ast, asts) {
    if (SymExpr* se = toSymExpr(ast)) {

      // Ignore symexprs that are not in the tree.
      if (!se->parentSymbol)
        continue;

      if (CallExpr* pc = parentYieldExpr(se)) {
        // SymExpr is in a yield.

        Symbol* ySym = se->symbol();
        if (oneLocalYS) {
          // ySym is already tracked in ic.value. No need to do anything.

          // While here, sanity-check a couple of things.
          INT_ASSERT(ySym->defPoint->parentSymbol == fn);
          INT_ASSERT(yldSymSet.set_in(ySym));
          INT_ASSERT(local2field.get(ySym) == valField);

        } else {
          // Update ic.value.
          // Unless it's yielding something of type dtNothing.
          if (ySym->type != dtNothing) {
            SymExpr* upd = new SymExpr(ySym);
            pc->insertBefore(new CallExpr(PRIM_SET_MEMBER, ic, valField, upd));
            if (ySym->defPoint->parentSymbol == fn) {
              // Need to convert 'upd' itself, too.
              Symbol* field = local2field.get(se->symbol());
              INT_ASSERT(field && field != valField);
              replaceLocalWithFieldTemp(upd, ic, field, false, true, asts);
            }
          }
        }
      } else if (useSet.set_in(se) || defSet.set_in(se)) {
        // SymExpr is among those we are interested in: def or use of a live local.

        // Get the corresponding field in the iterator class.
        Symbol* field = local2field.get(se->symbol());

        // Get the expression that sets or uses the symexpr.
        CallExpr* call = toCallExpr(se->parentExpr);

        if (call && call->isPrimitive(PRIM_ADDR_OF)) {

          // Convert (addr of var) to (. _ic field).
          // Note, GET_MEMBER is not valid on a ref field;
          // in that event, GET_MEMBER_VALUE returns the ref.
          if (field->isRef())
            call->primitive = primitives[PRIM_GET_MEMBER_VALUE];
          else
            call->primitive = primitives[PRIM_GET_MEMBER];

          call->insertAtHead(ic);
          se->setSymbol(field);
        } else {
          replaceLocalWithFieldTemp(se, ic, field,
                                    defSet.set_in(se), useSet.set_in(se), asts);
        }
      }
    }
  }
}

//
// Build zip functions for the single loop iterator optimization
//
// The single iterator loop optimization is applied to loops
// recognized as above in isSingleLoopIterator().  Such loops can be
// generated in a specialized form with simplified control flow.  When
// the optimization does not apply, zip1 and zip3 simply call advance,
// and zip2 and zip4 are empty (see below in lowerIterator()).
//

// Build the zip1 function, copying expressions out of the iterator
// body that occur *before* the start of the loop proper (i.e. before the
// singleLoop construct).
static void
buildZip1(IteratorInfo* ii, Vec<BaseAST*>& asts, BlockStmt* singleLoop) {

  // Expects to be called inside a clause that already tests singleLoop !=
  // NULL.  This restriction can be removed if the != NULL test is pushed down
  // into this routine.
  INT_ASSERT(singleLoop != NULL);

  BlockStmt* zip1body = new BlockStmt();

  // See Note #1.
  SymbolMap map;
  Symbol* advance_this = ii->advance->getFormal(1);
  Symbol* my_this = ii->zip1->getFormal(1);
  INT_ASSERT(my_this == ii->zip1->_this);
  map.put(advance_this, my_this);

  // Copy non-arg def expressions from the original iterator
  // 2015-02-23 hilde: TODO #1:
  // This is sloppy.  We only need local variables that are actually used
  // within the zip1 body.  So we can probably gen the DefExprs we need by
  // scanning the exprs to be copied and just populating the map with the
  // symbols we actually use.  This utility can be factored out of all of the
  // buildZip functions.
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = toDefExpr(ast))
      if (!isArgSymbol(def->sym))
        zip1body->insertAtTail(def->copy(&map));
  }

  // Copy all iterator body expressions before singleLoop that are not
  // declarations
  for_alist(expr, ii->iterator->body->body) {
    // Quit when we reach singleLoop
    if (expr == singleLoop)
      break;

    if (!isDefExpr(expr))
      zip1body->insertAtTail(expr->copy(&map));
  }

  if (WhileStmt* stmt = toWhileStmt(singleLoop)) {
    // By the time we get here, the condExpr has been passed through _cond_test
    // and the result stored in a temp, so condExprForTmpVariableGet just
    // returns the temp result.  So we can simply say:  (.= this more tmp)
    // This simplification depends on the current interpretation of more
    // in a single-loop, single-yield iterator (i.e. 1 = more; 0 = done).
    SymExpr* condExpr = stmt->condExprForTmpVariableGet()->copy(&map);
    Type* moreType = ii->iclass->getField("more")->type;
    VarSymbol* condTemp = newTemp("cond_tmp", moreType);
    zip1body->insertAtTail(new DefExpr(condTemp));
    zip1body->insertAtTail(new CallExpr(PRIM_MOVE, condTemp,
                                        new CallExpr(PRIM_CAST, moreType->symbol, condExpr)));
    zip1body->insertAtTail(new CallExpr(PRIM_SET_MEMBER, my_this,
                                        ii->iclass->getField("more"), condTemp));
  } else if (singleLoop->isCForLoop()) {
    // CForLoops do not use the "more" field in their loop termination test.
    // See the code for that special case in buildHasMore().
  } else {
    // ParamForLoops should have been removed during resolution.
    // DoWhileLoops are not treated as singleLoop iterators.
    // ForLoops should have been replaced in expandForLoop().
    INT_FATAL(singleLoop, "Unexpected singleLoop iterator type");
  }

  zip1body->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));

  ii->zip1->body->replace(zip1body);
}


// Build the zip2 function, copying expressions out of the singleLoop
// body that are *before* the yield
static void
buildZip2(IteratorInfo* ii, Vec<BaseAST*>& asts, BlockStmt* singleLoop) {

  // Expects to be called inside a clause that already tests singleLoop !=
  // NULL.  This restriction can be removed if the != NULL test is pushed down
  // into this routine.
  INT_ASSERT(singleLoop != NULL);

  // In copied expressions, replace _ic with zip2->_this .
  // See Note #1.
  SymbolMap map;
  Symbol* advance_this = ii->advance->getFormal(1);
  Symbol* my_this = ii->zip2->getFormal(1);
  INT_ASSERT(my_this == ii->zip2->_this);
  map.put(advance_this, my_this);

  // This block will replace the body of the zip2 function stubbed in
  // during resolution.
  BlockStmt* zip2body = new BlockStmt();

  // Copy non-arg def expressions from the original iterator
  // See TODO #1 above.
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = toDefExpr(ast))
      if (!isArgSymbol(def->sym))
        zip2body->insertAtTail(def->copy(&map));
  }

  if (singleLoop->isForLoop())
  {
    INT_FATAL(singleLoop, "Unexpected singleLoop iterator type.");
  }

  // Copy all non-defs in singleLoop before the yield
  for_alist(expr, singleLoop->body) {
    if (CallExpr* call = toCallExpr(expr))
      if (call->isPrimitive(PRIM_YIELD))
        break;
    if (!isDefExpr(expr))
      zip2body->insertAtTail(expr->copy(&map));
  }

  // Since this passes isSingleLoopIterator(), there is a single yield,
  // so 'more' is always the first value after initialization, i.e. 2.
  zip2body->insertAtTail(new CallExpr(PRIM_SET_MEMBER, my_this, ii->iclass->getField("more"), new_IntSymbol(2)));

  zip2body->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));

  ii->zip2->body->replace(zip2body);
}


// Build the zip3 function, copying expressions out of the singleLoop
// body that are *after* the yield
static void
buildZip3(IteratorInfo* ii, Vec<BaseAST*>& asts, BlockStmt* singleLoop) {

  // Expects to be called inside a clause that already tests singleLoop !=
  // NULL.  This restriction can be removed if the != NULL test is pushed down
  // into this routine.
  INT_ASSERT(singleLoop != NULL);

  BlockStmt* zip3body = new BlockStmt();

  // In copied expressions, replace _ic with zip3->_this .
  // See Note #1.
  SymbolMap map;
  Symbol* advance_this = ii->advance->getFormal(1);
  Symbol* my_this = ii->zip3->getFormal(1);
  INT_ASSERT(my_this == ii->zip3->_this);
  map.put(advance_this, my_this);

  // Copy non-arg def expressions from the original iterator
  // See TODO #1 above.
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = toDefExpr(ast))
      if (!isArgSymbol(def->sym))
        zip3body->insertAtTail(def->copy(&map));
  }

  // Copy all non-defs in singleLoop after the yield
  bool beforeYield = true;
  for_alist(expr, singleLoop->body) {
    // Skip everything before the yield
    if (beforeYield) {
      if (CallExpr* call = toCallExpr(expr))
        if (call->isPrimitive(PRIM_YIELD)) {
          beforeYield = false;
          skipIBBinAlist(_alist_next);
        }
    } else {
      // after yield
      if (!isDefExpr(expr))
        zip3body->insertAtTail(expr->copy(&map));
    }
  }

  if (WhileStmt* stmt = toWhileStmt(singleLoop)) {
    SymExpr* condExpr = stmt->condExprForTmpVariableGet()->copy(&map);
    Type* moreType = ii->iclass->getField("more")->type;
    VarSymbol* condTemp = newTemp("cond_tmp", moreType);
    zip3body->insertAtTail(new DefExpr(condTemp));
    zip3body->insertAtTail(new CallExpr(PRIM_MOVE, condTemp,
                                        new CallExpr(PRIM_CAST, moreType->symbol, condExpr)));
    zip3body->insertAtTail(new CallExpr(PRIM_SET_MEMBER, my_this,
                                        ii->iclass->getField("more"), condTemp));
  } else if (isCForLoop(singleLoop)) {
    // CForLoops do not use the "more" field in their loop termination test.
    // See the code for that special case in buildHasMore().
  } else {
    // ParamForLoops should have been removed during resolution.
    // DoWhileLoops are not treated as singleLoop iterators.
    // ForLoops should have been replaced in expandForLoop().
    INT_FATAL(singleLoop, "Unexpected singleLoop iterator type");
  }

  zip3body->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));

  ii->zip3->body->replace(zip3body);
}


// Build the zip4 function, copy expressions out of the iterator body
// that are *after* the singleLoop
static void
buildZip4(IteratorInfo* ii, Vec<BaseAST*>& asts, BlockStmt* singleLoop) {

  // Expects to be called inside a clause that already tests singleLoop !=
  // NULL.  This restriction can be removed if the != NULL test is pushed down
  // into this routine.
  INT_ASSERT(singleLoop != NULL);

  // In copied expressions, replace _ic with zip4->_this .
  // See Note #1.
  SymbolMap map;
  Symbol* advance_this = ii->advance->getFormal(1);
  Symbol* my_this = ii->zip4->getFormal(1);
  INT_ASSERT(my_this == ii->zip4->_this);
  map.put(advance_this, my_this);

  // This block will replace the body of the zip4 function stubbed in
  // during resolution.
  BlockStmt* zip4body = new BlockStmt();

  // Copy non-arg def expressions from the original iterator
  // See TODO #1 above.
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = toDefExpr(ast))
      if (!isArgSymbol(def->sym))
        zip4body->insertAtTail(def->copy(&map));
  }

  if (singleLoop->isForLoop())
  {
    INT_FATAL(singleLoop, "Unexpected singleLoop iterator type.");
  }

  // Copy all iterator body expressions after singleLoop that are not
  // declarations
  bool flag = true;

  for_alist(expr, ii->iterator->body->body) {
    // Skip everything before the singleLoop
    if (flag) {
      if (expr == singleLoop)
        flag = false;

    } else if (!isDefExpr(expr)) {
      zip4body->insertAtTail(expr->copy(&map));
    }
  }

  zip4body->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));

  ii->zip4->body->replace(zip4body);
}


//
// Handle IBB - Iterator Break Block.
//
// An IBB contains the defer actions to be taken when breaking
// out of the enclosing loop. It is constructed like this:
//
// (a) createIteratorBreakBlocks() adds a conditional after each "yield",
// so they look like this:
//
//   yield ...;
//   if gIteratorBreakToken {
//     return;
//   }
//
// (b) This "return" causes callDestructors to insert, before the "return",
// the defer actions that are appropriate at this point in the iterator.
//
// See also isIBBCondStmt() and the PR message for #12963.
//
void createIteratorBreakBlocks() {
  for_alive_in_Vec(CallExpr, yield, gCallExprs)
    if (yield->isPrimitive(PRIM_YIELD))
      if (FnSymbol* parent = toFnSymbol(yield->parentSymbol)) {
        SET_LINENO(yield);
        // An empty IBB is: if gIteratorBreakToken then return;
        Symbol* epLab = parent->getOrCreateEpilogueLabel();
        yield->insertAfter(new CondStmt(new SymExpr(gIteratorBreakToken),
                             new BlockStmt(new GotoStmt(GOTO_RETURN, epLab))));
      }
}

// Find and return the IBB for the given yield stmt.
// Remove the enclosing conditional from the tree if delayedRm==NULL,
// otherwise add it to delayedRm.
BlockStmt* getAndRemoveIteratorBreakBlockForYield(std::vector<Expr*>* delayedRm,
                                                  CallExpr* yield)
{
  // Right now the CondStmt follows a yield immediately.
  // If this changes, trace the 'next' pointers until found.
  CondStmt* IBBcond = isIBBCondStmt(yield->next);
  BlockStmt* result = IBBcond->thenStmt;

  if (delayedRm) {
    // This happens when lowering a ForallStmt. A few callers up from here
    // there is a for_alist in visitor code. It would exit right after IBBcond
    // if we remove IBBcond here, so the remaining alist stmts would not
    // get processed.
    delayedRm->push_back(IBBcond);
    result->remove();
  } else {
    IBBcond->remove();
  }
  return result;
}


static void handleYieldInAdvance(Vec<LabelSymbol*>& labels,
                                 Vec<LabelSymbol*>& breakLbls,
                                 IteratorInfo* ii, Symbol* ic, Symbol* end,
                                 CallExpr* call, int idx)
{
  BlockStmt* breakBlock = getAndRemoveIteratorBreakBlockForYield(NULL, call);

  call->insertBefore(new CallExpr(PRIM_SET_MEMBER,
                                  ic, ii->iclass->getField("more"),
                                  new_IntSymbol(idx)));
  call->insertBefore(new GotoStmt(GOTO_ITER_END, end));

  LabelSymbol* label = new LabelSymbol(astr("_jump_", istr(idx)));
  labels.add(label);
  call->insertAfter(new DefExpr(label));

  LabelSymbol* breakLab = new LabelSymbol(astr("_jump_break_", istr(idx)));
  breakLbls.add(breakLab);
  breakBlock->insertAtHead(new DefExpr(breakLab));
  // breakBlock already ends with "goto end"
  call->insertAfter(breakBlock);
  // "goto label" needs to skip 'breakBlock'
  INT_ASSERT(breakBlock->next == label->defPoint);
  breakBlock->flattenAndRemove();

  call->remove();
}

static void buildJumpTables(BlockStmt* advanceBody,
                            Vec<LabelSymbol*>& labels,
                            Vec<LabelSymbol*>& breakLbls,
                            Symbol* more)
{
  int i = 2;
  Expr* anchor = advanceBody->body.head;

  forv_Vec(LabelSymbol, label, labels) {
    GotoStmt* igs = new GotoStmt(GOTO_ITER_RESUME, label);
    label->iterResumeGoto = igs;
    Symbol* tmp = newTemp(dtBool);
    // Todo: switch to anchor->insertBefore().
    // Then, the order of comparisons will change. Will it affect performance?
    advanceBody->insertAtHead(new CondStmt(new SymExpr(tmp), igs));
    advanceBody->insertAtHead(new CallExpr(PRIM_MOVE, tmp,
                      new CallExpr(PRIM_EQUAL, more, new_IntSymbol(i++))));
    advanceBody->insertAtHead(new DefExpr(tmp));
  }

  // add jumps to handle 'break's from the loops over this iterator
  // todo: skip this when the loops do not have breaks
  i = 2;
  forv_Vec(LabelSymbol, label, breakLbls) {
    GotoStmt* igs = new GotoStmt(GOTO_ITER_RESUME, label);
    label->iterResumeGoto = igs;
    Symbol* tmp = newTemp(dtBool);
    anchor->insertBefore(new DefExpr(tmp));
    anchor->insertBefore(new CallExpr(PRIM_MOVE, tmp,
                      new CallExpr(PRIM_EQUAL, more, new_IntSymbol(-(i++)))));
    anchor->insertBefore(new CondStmt(new SymExpr(tmp), igs));
  }
}

// Builds the standard (non-optimized) iterator body by replacing yields with
// statements that update the "more" field in the iterator class, gotos and
// labels.  Then adds a jump table at the beginning of advance, so execution
// will continue from the next label the next time the iterator is entered.
static void
buildAdvance(FnSymbol* fn,
             Vec<BaseAST*>& asts,
             SymbolMap& local2field,
             Vec<Symbol*>& locals) {
  IteratorInfo* ii = fn->iteratorInfo;
  Symbol* ic = ii->advance->getFormal(1);

  //
  // build standard iterator advance method (advance)
  //
  // This block will replace the stubbed-in body.
  BlockStmt* advanceBody = new BlockStmt();

  for_alist(expr, fn->body->body)
    advanceBody->insertAtTail(expr->remove());

  Symbol* end = new LabelSymbol("_end");
  advanceBody->insertAtTail(new DefExpr(end));

  // change yields to labels and gotos
  int i = 2; // 1 = not started, 0 = finished
  Vec<LabelSymbol*> labels, breakLbls;

  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (call->isPrimitive(PRIM_YIELD)) {
        handleYieldInAdvance(labels, breakLbls, ii, ic, end, call, i++);
      } else if (call->isPrimitive(PRIM_RETURN)) {
        INT_FATAL(call, "should have been removed with removeRetSymbolAndUses()");
      }
    } else if (LoopStmt* loop = toLoopStmt(ast)) {
      loop->orderIndependentSet(false);
    }
  }

  // iterator is done ==> more=0
  end->defPoint->insertBefore(new CallExpr(PRIM_SET_MEMBER, ic, ii->iclass->getField("more"), new_IntSymbol(0)));

  // insert jump table at head of advance
  Symbol* more = new VarSymbol("more", dtInt[INT_SIZE_DEFAULT]);
  buildJumpTables(advanceBody, labels, breakLbls, more);

  advanceBody->insertAtHead(new CallExpr(PRIM_MOVE, more, new CallExpr(PRIM_GET_MEMBER_VALUE, ic, ii->iclass->getField("more"))));
  advanceBody->insertAtHead(new DefExpr(more));
  advanceBody->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));

  ii->advance->body->replace(advanceBody);
}


// Build hasMore function.
static void
buildHasMore(IteratorInfo* ii, BlockStmt* singleLoop) {
  // Create an empty body for hasMore()
  BlockStmt* hasMoreBody = new BlockStmt();

  // Stick in a declaration for the return value temp at the top.
  VarSymbol* tmp         = newTemp(ii->hasMore->retType);
  hasMoreBody->insertAtTail(new DefExpr(tmp));

  // See Notes #1 and #2.
  SymbolMap map;
  Symbol* advance_this = ii->advance->getFormal(1);
  Symbol* my_this = ii->hasMore->getFormal(1);
  INT_ASSERT(my_this == ii->hasMore->_this);
  map.put(advance_this, my_this);

  if (singleLoop != NULL) {
    if (singleLoop->isCForLoop() == true) {
      CForLoop*  cforLoop  = toCForLoop(singleLoop);

      // Inline the contents of the test block in the hasMore function,
      BlockStmt* testBlock = cforLoop->testBlockGet();
      for_alist(expr, testBlock->body) {
        if (expr != testBlock->body.tail) {
          hasMoreBody->insertAtTail(expr->copy(&map));
        }
      }

      hasMoreBody->insertAtTail(new CallExpr(PRIM_MOVE, tmp,
                                             testBlock->body.tail->copy(&map)));
    }
    else if (isWhileStmt(singleLoop))
    {
      // Just returns the current "more" field.
      // This is duplicate code as for the non-singleLoop case below, which is
      // unfortunate....
      hasMoreBody->insertAtTail(new CallExpr(PRIM_MOVE,
                                             tmp,
                                             new CallExpr(PRIM_GET_MEMBER_VALUE,
                                                          my_this,
                                                          ii->iclass->getField("more"))));
    }
    else
      INT_FATAL(singleLoop, "Unhandled singleLoop iterator type");
  } else {
    hasMoreBody->insertAtTail(new CallExpr(PRIM_MOVE,
                                           tmp,
                                           new CallExpr(PRIM_GET_MEMBER_VALUE,
                                                        my_this,
                                                        ii->iclass->getField("more"))));
  }

  hasMoreBody->insertAtTail(new CallExpr(PRIM_RETURN, tmp));

  ii->hasMore->body->replace(hasMoreBody);
}

// Build getValue function.
static void
buildGetValue(IteratorInfo* ii, BlockStmt* singleLoop) {
  BlockStmt* getValueBody = new BlockStmt();

  // See Note #1.
  SymbolMap map;
  Symbol* advance_this = ii->advance->getFormal(1);
  Symbol* my_this = ii->getValue->getFormal(1);
  INT_ASSERT(my_this == ii->getValue->_this);
  map.put(advance_this, my_this);


  if (singleLoop && singleLoop->isForLoop())
  {
    INT_FATAL(singleLoop, "Unexpected singleLoop iterator type");
  }
  else if (ii->getValue->retType != dtNothing)
  {
    VarSymbol* tmp = newTemp(ii->getValue->retType);
    getValueBody->insertAtTail(new DefExpr(tmp));
    getValueBody->insertAtTail(new CallExpr(PRIM_MOVE, tmp,
                                            new CallExpr(PRIM_GET_MEMBER_VALUE,
                                                         my_this, ii->iclass->getField("value"))));
    getValueBody->insertAtTail(new CallExpr(PRIM_RETURN, tmp));
  } else {
    getValueBody->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
  }

  ii->getValue->body->replace(getValueBody);
}

static void
buildInit(IteratorInfo* ii, BlockStmt* singleLoop) {
  BlockStmt* initBody = new BlockStmt();

  // See Notes #1 and #2.
  SymbolMap  map;
  Symbol* advance_this = ii->advance->getFormal(1);
  Symbol* my_this = ii->init->getFormal(1);
  INT_ASSERT(ii->init->getFormal(1) == ii->init->_this);
  map.put(advance_this, my_this);

  if (singleLoop != NULL)
  {
    if (singleLoop->isCForLoop() == true) {
      CForLoop*  cforLoop  = toCForLoop(singleLoop);
      BlockStmt* initBlock = NULL;

      initBlock = cforLoop->initBlockGet();

      for_alist(expr, initBlock->body) {
        initBody->insertAtTail(expr->copy(&map));
      }
    }
    else if (singleLoop->isWhileStmt())
    {
      // No init code for a while loop.
    }
    else
      INT_FATAL(singleLoop, "Unhandled singleLoop type");
  }

  initBody->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));

  ii->init->body->replace(initBody);
}

static void
buildIncr(IteratorInfo* ii, BlockStmt* singleLoop) {
  BlockStmt* incrBody = new BlockStmt();

  // See Notes #1 and #2.
  SymbolMap map;
  Symbol* advance_this = ii->advance->getFormal(1);
  Symbol* my_this = ii->incr->getFormal(1);
  INT_ASSERT(my_this == ii->incr->_this);
  map.put(advance_this, my_this);

  if (singleLoop != NULL) {
    if (singleLoop->isCForLoop() == true) {
      CForLoop*  cforLoop  = toCForLoop(singleLoop);
      BlockStmt* incrBlock = NULL;

      incrBlock = cforLoop->incrBlockGet();

      for_alist(expr, incrBlock->body) {
        incrBody->insertAtTail(expr->copy(&map));
      }
    }
    else if (singleLoop->isWhileStmt())
    {
      // No incr clause in a while statement.
    }
    else
      INT_FATAL(singleLoop, "Unhandled singleLoop case.");
  }

  incrBody->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));

  ii->incr->body->replace(incrBody);
}



// Collect local variables that are live at the point of any yield.
static void collectLiveLocalVariables(Vec<Symbol*>& syms, FnSymbol* fn, BlockStmt* singleLoop)
{
  Vec<Symbol*> locals;
  Map<Symbol*,int> localMap;
  Vec<SymExpr*> useSet;
  Vec<SymExpr*> defSet;
  std::vector<BitVec*> OUT;

  liveVariableAnalysis(fn, locals, localMap, useSet, defSet, OUT);

  int block = 0;

  for_vector(BasicBlock, bb, *fn->basicBlocks) {
    bool collect = false;

    for_vector(Expr, expr, bb->exprs) {
      CallExpr* call = toCallExpr(expr);

      if (call && call->isPrimitive(PRIM_YIELD))
        collect = true;

      if (singleLoop && expr == singleLoop->next)
        collect = true;

      if (singleLoop && expr == singleLoop->body.head)
        collect = true;
    }

    if (collect) {
      BitVec live(locals.n);

      for (int j = 0; j < locals.n; j++) {
        if (OUT[block]->get(j))
          live.set(j);
      }

      for (int k = bb->exprs.size() - 1; k >= 0; k--) {
        CallExpr*             call = toCallExpr(bb->exprs[k]);
        std::vector<SymExpr*> symExprs;

        if ((call && call->isPrimitive(PRIM_YIELD)) ||
            (singleLoop && bb->exprs[k] == singleLoop->next) ||
            (singleLoop && bb->exprs[k] == singleLoop->body.head)) {
          for (int j = 0; j < locals.n; j++) {
            if (live.get(j)) {
              syms.add_exclusive(locals.v[j]);
            }
          }
        }

        collectSymExprs(bb->exprs[k], symExprs);

        for_vector(SymExpr, se, symExprs) {
          if (defSet.set_in(se))
            live.unset(localMap.get(se->symbol()));

          if (useSet.set_in(se))
            live.set(localMap.get(se->symbol()));
        }
      }
    }

    block++;
  }

  for_vector(BitVec, out, OUT)
    delete out;

  // C_FOR_LOOP needs to ensure the for-loop init variables are also
  // converted to fields.  The test/incr fields are handled correctly
  // as a result of being inserted in to the body of the loop
  if (singleLoop != NULL && singleLoop->isCForLoop() == true) {
    std::vector<SymExpr*> symExprs;
    CForLoop*             cforLoop = toCForLoop(singleLoop);

    collectSymExprs(cforLoop->initBlockGet(), symExprs);

    for_vector(SymExpr, se, symExprs) {
      if (useSet.set_in(se)) {
        syms.add_exclusive(se->symbol());
      }
    }
  }
}


static bool containsRefVar(Vec<Symbol*>& syms,
                           FnSymbol*     fn,
                           Vec<Symbol*>& yldSymSet)
{
  forv_Vec(Symbol, sym, syms)
    if (!isArgSymbol(sym) && sym->type->symbol->hasFlag(FLAG_REF) &&
        !yldSymSet.set_in(sym))
      return true;

  return false;
}


static void insertLocalsForRefs(Vec<Symbol*>& syms,
                                FnSymbol*     fn,
                                Vec<Symbol*>& yldSymSet)
{
  // Walk the variables in this (iterator) function
  // which are not the return symbol nor argument, and are ref symbols.
  forv_Vec(Symbol, sym, syms) {
    if (isArgSymbol(sym) || yldSymSet.set_in(sym))
      continue;

    if (sym->type->symbol->hasFlag(FLAG_REF)) {
      CallExpr* move = NULL;
      if (!sym->isDefined()) {
        INT_FATAL(sym, "Expected sym to have at least one definition");
      }

      // Ignores reference actuals passed to reference formals
      for_SymbolDefs(def, sym) {
        CallExpr* parent = toCallExpr(def->parentExpr);
        INT_ASSERT(parent);
        if (parent->isPrimitive(PRIM_MOVE)) {
          if (move == NULL) {
            move = parent;
          } else {
            INT_FATAL(sym, "Expected sym to have exactly one move-definition");
          }
        }
      }

      INT_ASSERT(move && move->isPrimitive(PRIM_MOVE));

      if (SymExpr* se = toSymExpr(move->get(2)))
      {
        // The symbol is defined through a bitwise (pointer) copy.
        INT_ASSERT(se->symbol()->type->symbol->hasFlag(FLAG_REF));

        if (se->symbol()->defPoint->parentSymbol == fn) {
          syms.add_exclusive(se->symbol());
        }
      }
      else if (CallExpr* call = toCallExpr(move->get(2)))
      {
        // The RHS is a function call.
        if (FnSymbol* fn = call->resolvedFunction()) {
          for_actuals(actual, call) {
            SymExpr* se = toSymExpr(actual);

            if (se->symbol()->defPoint->parentSymbol == fn) {
              syms.add_exclusive(se->symbol());
            }
          }
        }
        else
        {
          // The RHS is not a function call: it must be a primitive instead.
          if (call->isPrimitive(PRIM_ADDR_OF)    ||
              call->isPrimitive(PRIM_GET_MEMBER) ||
              // If we are reading a reference out of a field, I'm not sure
              // we capture the right rhs below.  (The actual target of the
              // ref lies outside the struct that contains the ref.)
              call->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
            SymExpr* rhs = toSymExpr(call->get(1));

            syms.add_exclusive(rhs->symbol());
          }
          else
          {
            INT_FATAL(sym,
                      "Unhandled case: Ref returned by a primitive "
                      "from which we did not expect one.");
          }
        }
      }
      else
      {
        INT_FATAL(move, "RHS of a move is neither a SymExpr nor a CallExpr.");
      }
    }
  }
}


static void
addLiveLocalVariables(Vec<Symbol*>& syms, FnSymbol* fn, BlockStmt* singleLoop,
                      Vec<Symbol*>& yldSymSet)
{
  BasicBlock::buildBasicBlocks(fn);

#ifdef DEBUG_LIVE
  printf("Iterator\n");
  list_view(fn);
#endif

#ifdef DEBUG_LIVE
  printf("Basic Blocks\n");
  BasicBlock::printBasicBlocks(fn);
#endif

  collectLiveLocalVariables(syms, fn, singleLoop);

#ifdef DEBUG_LIVE
  printf("LIVE at Yield Points\n");

  forv_Vec(Symbol, sym, syms) {
    printf("%s[%d]\n", sym->name, sym->id);
  }

  printf("\n");
#endif

  //
  // If we have live references to local variables, then we need to
  // make sure that we add these local variables to the iterator class
  // along with the reference.
  //
  // See test/functions/deitz/iterators/test_fock_simplification.chpl
  // and make sure that the iterator foo is not inlined even though
  // the iterators for the loops inside this one are inlined.  At the
  // time of this comment, the two yields kept foo from being
  // inlined.
  //
  if (containsRefVar(syms, fn, yldSymSet))
    insertLocalsForRefs(syms, fn, yldSymSet);
}


// Collect all local variables
// (Called only if live variable analysis is turned off.)
static void
addAllLocalVariables(Vec<Symbol*>& syms, Vec<BaseAST*>& asts) {
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = toDefExpr(ast))
      if (VarSymbol* var = toVarSymbol(def->sym))
        if (!var->type->symbol->hasFlag(FLAG_REF) ||
             var->hasFlag(FLAG_INDEX_VAR) ||
             var->hasFlag(FLAG_REF_VAR)) {
          syms.add(var);
        }
  }
}


// Preceding calls to the various build...() functions have copied out
// interesting parts of the iterator function.
//
// This function rips the guts out of the original iterator function and
// replaces them with a simple function that just initializes the fields
// in the iterator record with formal arguments of the original iterator
// that are live at yield sites within it.
static void
rebuildIterator(IteratorInfo* ii,
                SymbolMap&    local2field,
                Vec<Symbol*>& locals) {
  // Remove the original iterator function.
  FnSymbol*              fn = ii->iterator;
  std::vector<CallExpr*> icalls;

  collectCallExprs(fn, icalls);

  // ... and the task functions that it calls.
  for_vector(CallExpr, call, icalls)
    if (FnSymbol* taskFn = resolvedToTaskFun(call))
      // ... except those with multiple calls to them.
      if (taskFn->singleInvocation() != NULL)
        taskFn->defPoint->remove();

  for_alist(expr, fn->body->body)
    expr->remove();

  fn->retSymbol = NULL;
  fn->defPoint->remove();

  // Now the iterator creates and returns a copy of the iterator record.
  if (!fn->hasFlag(FLAG_FN_RETARG))
    fn->retType = ii->irecord;

  Symbol* iterator = newTemp("_ir", ii->irecord);

  fn->insertAtTail(new DefExpr(iterator));

  // For each live argument
  forv_Vec(Symbol, local, locals) {
    if (!toArgSymbol(local))
      continue;

    // Get the corresponding field in the iterator class
    Symbol* field = local2field.get(local);
    Symbol* value = local;

    if (local->type == field->type->refType) {
      // If a ref var, load the local in to a temp and
      // then set the value of the corresponding field.
      Symbol* tmp = newTemp(field->type);

      fn->insertAtTail(new DefExpr(tmp));

      fn->insertAtTail(new CallExpr(PRIM_MOVE,
                                    tmp,
                                    new CallExpr(PRIM_DEREF, local)));

      value = tmp;
    }

    fn->insertAtTail(new CallExpr(PRIM_SET_MEMBER, iterator, field, value));
  }

  // Return the filled-in iterator record.
  if (fn->hasFlag(FLAG_FN_RETARG)) {
    ArgSymbol* retArg = NULL;
    for_formals(formal, fn) {
      if (formal->hasFlag(FLAG_RETARG))
        retArg = formal;
    }
    fn->insertAtTail(new CallExpr(PRIM_ASSIGN, retArg, iterator));
    fn->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
  } else {
    fn->insertAtTail(new CallExpr(PRIM_RETURN, iterator));
  }

  ii->getValue->defPoint->insertAfter(new DefExpr(fn));

  fn->addFlag(FLAG_INLINE);
}


// Fills in the body of the getIterator function.
static void
rebuildGetIterator(IteratorInfo* ii) {
  FnSymbol*  getIterator = ii->getIterator;
  Symbol*    ret         = getIterator->getReturnSymbol();

  // This is the iterator record instance.
  ArgSymbol* arg         = getIterator->getFormal(1);

  // Set the iterator class (state object) so that it
  // initially signals that more elements available.
  getIterator->insertBeforeEpilogue(new CallExpr(PRIM_SET_MEMBER,
                                                 ret,
                                                 ii->iclass->getField("more"),
                                                 new_IntSymbol(1)));

  // Enumerate the fields in the iterator record (argument).
  for_fields(field, ii->irecord) {
    if (!strcmp(field->name, "_shape_")) continue;
    // Load the record field into a temp,
    // and then use that to set the corresponding class field.
    VarSymbol* fieldReadTmp  = newTemp(field->qualType());

    CallExpr*  fieldRead     = new CallExpr(PRIM_GET_MEMBER_VALUE, arg, field);

    getIterator->insertBeforeEpilogue(new DefExpr(fieldReadTmp));

    getIterator->insertBeforeEpilogue(new CallExpr(PRIM_MOVE,
                                                 fieldReadTmp,
                                                 fieldRead));

    getIterator->insertBeforeEpilogue(
                             new CallExpr(PRIM_SET_MEMBER,
                                          ret,
                                          ii->iclass->getField(field->name),
                                          fieldReadTmp));
  }

  // The return is supplied in the shell function created during
  // functionResolution. (See protoIteratorClass().)
}


// All "newRet" symbols used in yield expressions will need special handling,
// so we collect them into a set.
static void
collectYieldSymbols(FnSymbol* fn, Vec<BaseAST*>& asts, Vec<Symbol*>& yldSymSet,
                    bool* oneLocalYSRef)
{
  bool gotNonlocalYS = false;
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* yCall = asYieldExpr(ast)) {
      SymExpr* ySymExpr = toSymExpr(yCall->get(1));
      INT_ASSERT(ySymExpr);
      Symbol* ySym = ySymExpr->symbol();
      if (ySym->defPoint->parentSymbol == fn) {
        // It is a local symbol, add it.
        yldSymSet.set_add(ySym);
      } else {
        // Non-local symbols are not included in yldSymSet.
        gotNonlocalYS = true;
        // FYI.
        INT_ASSERT(ySym->isConstant() || ySym->isParameter());
      }
    }
  }
  *oneLocalYSRef = !gotNonlocalYS && (yldSymSet.count() == 1);
}


// Apparently the yield symbols are not considered "live" at yield points.
// So have to add them manually.
static void
addYieldSymbols(Vec<Symbol*>& locals, Vec<Symbol*>& yldSymSet) {
  forv_Vec(Symbol, ySym, yldSymSet) {
    if (ySym)
      locals.add_exclusive(ySym);
  }
}


// After replaceLocalsWithFields() all locals are replaced with field accesses.
static void
removeLocals(Vec<Symbol*>& locals, Vec<BaseAST*>& asts, Vec<Symbol*>& yldSymSet, FnSymbol* fn) {
  forv_Vec(Symbol, l, locals) {
    INT_ASSERT(l->defPoint->parentSymbol == fn);
    if (!isArgSymbol(l))
      l->defPoint->remove();
  }
}


// Creates (and returns) an iterator class field.
// 'type' is used if local==NULL.
static inline Symbol* createICField(int& i, Symbol* local, Type* type,
                                    bool isValueField, FnSymbol* fn) {
  // The field name is "value" for the return value of the iterator,
  // or F<int>_<local->name> otherwise.
  const char* fieldName = isValueField
    ? "value"
    : astr("F", istr(i++), "_", local->name);

  // Add a field to the class

  // Propagate the Qualifier (e.g. field is ref if local is ref)
  // This is especially important if local is an ArgSymbol
  QualifiedType qt(QUAL_VAL, type);
  if (local)
    qt = local->qualType();
  // Workaround: use a ref type here
  // In the future, the Qualifier should be sufficient
  qt = qt.refToRefType();

  INT_ASSERT(qt.type() != dtUnknown);
  Symbol* field = new VarSymbol(fieldName, qt);

  fn->iteratorInfo->iclass->fields.insertAtTail(new DefExpr(field));

  return field;
}

static std::map<Symbol*, std::vector<CallExpr*> > formalToPrimMap;

void gatherPrimIRFieldValByFormal() {
  for_alive_in_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_ITERATOR_RECORD_FIELD_VALUE_BY_FORMAL)) {
      Symbol* formal = toSymExpr(call->get(2))->symbol();
      formalToPrimMap[formal].push_back(call);
    }
  }
}

void cleanupPrimIRFieldValByFormal() {
  formalToPrimMap.clear();
}

// Fills in the iterator class and record types with fields corresponding to the
// local variables defined in the iterator function (or its static context)
// and live at any yield.
static void addLocalsToClassAndRecord(Vec<Symbol*>& locals, FnSymbol* fn,
                                      Vec<Symbol*>& yldSymSet, Type* yieldedType,
                                      Symbol** valFieldRef, bool oneLocalYS,
                                      SymbolMap& local2field, SymbolMap& local2rfield)
{
  IteratorInfo* ii = fn->iteratorInfo;
  Symbol* valField = NULL;

  int i = 0;    // This numbers the fields.
  forv_Vec(Symbol, local, locals) {
    bool isYieldSym = yldSymSet.set_in(local);
    Symbol* field = createICField(i, local, NULL, isYieldSym && oneLocalYS, fn);
    local2field.put(local, field);
    if (isYieldSym) {
      INT_ASSERT(local->type == yieldedType);
      if (oneLocalYS) {
        INT_ASSERT(valField == NULL); // there is exactly 1 yield symbol
        valField = field;
      }
    }

    // Only (live) arguments are added to the record.
    if (isArgSymbol(local)) {
      VarSymbol* rfield = new VarSymbol(field->name, field->type);
      rfield->qual = field->qual;
      local2rfield.put(local, rfield);
      ii->irecord->fields.insertAtTail(new DefExpr(rfield));

      // while we're creating the iterator record fields based on the original
      // iterator function arguments, replace the primitive that gets the value
      // based on the formal with prim_get_member_value of the actual value.
      std::map<Symbol*, std::vector<CallExpr*> >::iterator localIt =
        formalToPrimMap.find(local);
      if (localIt != formalToPrimMap.end()) {
        for_vector(CallExpr, call, localIt->second) {
          INT_ASSERT(ii->irecord == call->get(1)->getValType());
          call->get(2)->replace(new SymExpr(rfield));
          call->primitive = primitives[PRIM_GET_MEMBER_VALUE];
        }
      }
    }
  }

  if (!valField) {
    valField = createICField(i, NULL, yieldedType, true, fn);
  }
  *valFieldRef = valField;
}


// Function resolution just adds a shell for the iterator records and classes,
// and stubs in implementations for the advance, zip1, zip2, zip3, zip4,
// hasMore, getValue, init and incr methods.
// (see protoIteratorClass())
// This function takes a pointer to an iterator and fills in those types.
void lowerIterator(FnSymbol* fn) {
  INT_ASSERT(! iteratorsLowered);  // ensure formalToPrimMap is valid
  SET_LINENO(fn);
  Vec<BaseAST*> asts;
  Type* yieldedType = removeRetSymbolAndUses(fn);
  collect_asts_postorder(fn, asts);

  BlockStmt* singleLoop = NULL;
  if (CallExpr* singleLoopYield = isSingleLoopIterator(fn, asts)) {
    // If the iterator contains a single loop statement containing a single
    // yield, singleLoop is that loop statement; otherwise, it is NULL.
    singleLoop = toBlockStmt(singleLoopYield->parentExpr);
  }

  //
  // create fields for all local variables and arguments; however, if
  // live variable analysis is enabled, make fields only for local
  // variables that are live at places where the iterator methods will
  // yield control back to the loop body.
  //
  SymbolMap local2field;  // map from arg/local to class field
  SymbolMap local2rfield; // map from arg to record field
  Vec<Symbol*> locals;
  Vec<Symbol*> yldSymSet; // local symbols appearing in yield(s)
  Symbol* valField;
  bool oneLocalYS;  // see comment earlier

  // Add all formals to the set of local symbols.
  for_formals(formal, fn)
    if (!formal->hasFlag(FLAG_RETARG))
      locals.add(formal);

  collectYieldSymbols(fn, asts, yldSymSet, &oneLocalYS);

  if (fNoLiveAnalysis)
    addAllLocalVariables(locals, asts);
  else
    addLiveLocalVariables(locals, fn, singleLoop, yldSymSet);

  addYieldSymbols(locals, yldSymSet);

  addLocalsToClassAndRecord(locals, fn, yldSymSet, yieldedType, &valField,
                            oneLocalYS, local2field, local2rfield);

  replaceLocalsWithFields(fn, asts, local2field, yldSymSet, valField,
                          oneLocalYS, locals);

  removeLocals(locals, asts, yldSymSet, fn);

  //------------------------------------------------------------------------
  // The above should probably be factored out.
  //------------------------------------------------------------------------

  IteratorInfo* ii = fn->iteratorInfo;
  if (!fn->hasFlag(FLAG_INLINE_ITERATOR)) {
    // Does this force iterators marked as inline to be inlined
    // even if --no-inline-iterators is specified?
    // Isn't that bad?
    if (singleLoop) {
      if (fReportOptimizedLoopIterators) {
        ModuleSymbol *mod = fn->getModule();

        if (developer || mod->modTag == MOD_USER) {
          printf("Optimized single yield/loop iterator (%s) in module %s (%s:%d)\n",
                 fn->cname, mod->name, fn->fname(), fn->linenum());
        }
      }

      buildZip1(ii, asts, singleLoop);
      buildZip2(ii, asts, singleLoop);
      buildZip3(ii, asts, singleLoop);
      buildZip4(ii, asts, singleLoop);
    } else {
      // For iterators that are not single-loop/single-yield, zip2 and zip4 are
      // no-ops and zip1 and zip3 simply call advance().
      ii->zip1->insertAtHead(new CallExpr(ii->advance, ii->zip1->_this));
      ii->zip3->insertAtHead(new CallExpr(ii->advance, ii->zip3->_this));
    }
    buildAdvance(fn, asts, local2field, locals);
    // Note that buildInit and buildIncr are essentially no-ops for non c for
    // loops and that the behavior of buildHasMore changes for C for loops.
    // For c for loops these basically just takes the init, test, and incr
    // expressions from the c for loop primitives and put them in the iterator
    // functions.
    buildHasMore(ii, singleLoop);
    buildGetValue(ii, singleLoop);
    buildInit(ii, singleLoop);
    buildIncr(ii, singleLoop);
  }
  rebuildIterator(ii, local2rfield, locals);
  rebuildGetIterator(ii);
}

////////////////////////////////////////////////////////////////////////////////
// NOTES
//
// Note #1:
//  In the single-loop optimization, parts of the original advance()
//  function are copied into the zip?, init, incr, hasMore and getValue
//  functions.  In those functions, "this" will refer to the "this" argument
//  passed into the original advance functions (== ii->advance->getFormal(1)).
//  When expressions are copied out of parts of the original advance function,
//  they must be updated to refer to the "this" argument of the function in
//  which they appear (== ii->????->getFormal(1)).  That is why the symbol map
//  is constructed and used in many of the build???? routines.  It turns out
//  that ii->????->_this is an alias for ii->????->getFormal(1).  I chose to
//  use the getFormal(1) variant, because IMO it is more obvious: Most of these
//  routines do not establish a "this" variable, but they all have the same
//  formal argument.
//
// Note #2:
//  In some of the build routines, the SymbolMap map is not used at the outer
//  scope.  I left its declaration and initialization at the outer scope in all
//  build????() functions, for consistency.
//
////////////////////////////////////////////////////////////////////////////////
