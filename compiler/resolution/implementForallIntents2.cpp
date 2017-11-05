/*
 * Copyright 2004-2017 Cray Inc.
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

#include "implementForallIntents.h"

/*
This file contains Part 2 of the new implementation of forall intents.
"Part 2" is mostly about extending a copy of the parallel iterator in question
with arguments and yield values corresponding to forall intents.

For a reduce intent, the global reduce op instance needs to be
properly allocated, deallocated and connected to the corresponding
user outer variable. This is the job of "Part 1".

For now, implementForallIntents2New() and immediately-adjacent functions
are in implementForallIntents.cpp, for historical reasons.

Even though we don't have to, we keep "New" in function names for now, to
distinguish them from the "old" implementation' in implementForallIntents.cpp.

OUTLINE

extendLeaderNew() extends a copy of the parallel iterator
of the given forall loop to:

* accept additional arguments for outer/shadow variables,
* additionally yield shadow variables,
* propagate these additional formals down to the yields,
  properly handling whatever parallel constructs are on the way.

TOP LEVEL

The extension process starts "at the top level"
by processing the call to the parallel iterator:

  * clone the parallel iterator

  * add actuals to the call to the parallel iterator

  * create + add formals to the parallel iterator
     - they become shadow variables

  * traverse the body of the parallel iterator

For a reduce intent (RI), the actual/formal is the reduce op class;
the corresponding thing to yield is a task-private accumulation state.

During the traversal, act as following.

WHEN ENCOUNTERING A TASK FUNCTION

  * add actuals + formals to the task function and CallExpr
    like we did at the top level to the parallel iterator

  * traverse the body of the task function

WHEN ENCOUNTERING A YIELD

  * augment the yield with the current shadow variables

  * Also, do the following at most once per the parallel iterator
    ("at the top level") and per a task function:

    - for a RI, initialize a task-private reduce op at task
      startup, combine() then delete it at task teardown
      (do not do this "at the top level")

    - for a RI, set up + tear down the shadow variable to yield
      based on the current reduce op

    - for a TPV (task-private variable), set up + tear down the shadow
      variable to yield (no corresponding actual+formal in above steps)

IF THE YIELD IS INSIDE A FORALL

  * augment the ForallStmt with forall intents

  * for a RI, skip certain steps

IF THE YIELD IS INSIDE A FOR OVER A PARALLEL ITERATOR

  * convert to forall loop
    (preserving the semantics)

  * process the forall as above

*/

/////////// helper data structures ///////////

//
// FIinfo aka Forall Intent info
//
// Contains all things pertinent to a single forall intent
// across the call described by the enclosing FIcontext.
//
// RI = Reduce Intent  TPV = Task-Private Variable (upcoming)
//
class FIinfo {
public:
  // The shadow var in the ForallStmt being processed.
  // In most cases it can be used only for info ex. the intent.
  ShadowVarSymbol* svSymbol;

  // The variable to be passed as an actual
  // to the parallel iterator, task function,
  // or ForallStmt within the parallel iterator.
  // NULL for a TPV (todo soon: always NULL in this case?)
  Symbol* fiActual;

  // The corresponding formal or (for a TPV) local variable.
  // For a RI, this is "parentOp"; otherwise the shadow variable.
  // For a TPV, this is a local variable, once created;
  // otherwise the formal of the parallel iterator or task fn.
  // Todo: better name for it?
  Symbol* fiFormal;

  // For a reduce intent - non-NULL once created:
  Symbol* riCurrentOp;
  Symbol* riShadowVar;

  // constructor
  // invoked implicitly upon FIcontext::fiInfos.resize()
  FIinfo() : svSymbol(0), fiActual(0), fiFormal(0),
             riCurrentOp(0), riShadowVar(0) { }
};

typedef std::vector<FIinfo> FIvec;

//
// FIcontext aka Forall Intents contest
//
// Contains things pertinent to the 'call' being traversed
// during extendLeaderNew(). The 'call' can be either:
//
//   when inIterator(): to the parallel iterator, from the ForallStmt,
//
//   when inTaskFn(): to a task function,
//     from within the parallel iterator or its task function.
//
// Multiple yield statements may be handled/extended
// under the same FIcontext.
//
class FIcontext {
public:
  // The forall stmt being processed.
  ForallStmt*  forall;

  // The call in being looked at.
  CallExpr*    call;

  // The iterator or task function being called.
  FnSymbol*    curFn;

  // Calling a function nested in the iterator?
  bool         nested;

  // The iterator's return symbol.
  VarSymbol*   retSym;

  // Where to add task startup/teardown code. It is usually 'curFn'.
  // However, when descending into a task function that does not create
  // a user-level task (and so does not affect the intents),
  // it is an enclosing function.
  FnSymbol*    anchorFn;

  // Where in 'anchorFn' to insert startup/teardown code.
  Expr*        anchor1;
  Expr*        anchor2;
  Expr*        anchor2orig;
  Expr*        anchor2prev;

  // Information on each forall intent across this call.
  FIvec        fiInfos;

  bool inIterator() const { return !nested; }
  bool inTaskFn()   const { return nested; }

  int  numShadowVars()   const {
    int result = forall->numIntentVars();
    INT_ASSERT(result == (int)fiInfos.size());
    return result;
  }

  // constructor for the outermodst context
  FIcontext(ForallStmt* fs, CallExpr* iterCall,
            FnSymbol* iterFn, VarSymbol* retSymbol) :
    forall(fs),
    call(iterCall),
    curFn(iterFn),
    nested(false),
    retSym(retSymbol),
    anchorFn(curFn),
    anchor1(0), anchor2(0), anchor2orig(0), anchor2prev(0)
  {
    fiInfos.resize(forall->numIntentVars());
  }

  // constructor for a nested context
  FIcontext(FIcontext& parentCx, CallExpr* taskCall, FnSymbol* taskFn) :
    forall(parentCx.forall),
    call(taskCall),
    curFn(taskFn),
    nested(true),
    retSym(parentCx.retSym),
    anchorFn(curFn),
    anchor1(0), anchor2(0), anchor2orig(0), anchor2prev(0)
  {
    int numSVars = forall->numIntentVars();
    fiInfos.resize(numSVars);
    for (int ix = 0; ix < numSVars; ix++) {
      fiInfos[ix].svSymbol = parentCx.fiInfos[ix].svSymbol;
      fiInfos[ix].fiActual = parentCx.fiInfos[ix].fiFormal;
    }
  }
};


/////////// EFLOPI: Enclosing For Loop Over Parallel Iterator ///////////

// 
// Info we gather while looking for an eflopi or an enclosing forall loop
//
// The contents are valid only after a call to findEnclosingParallelLoop()
// as follows:
//
//  * 'eflopiForall' and 'eflopiLoop' are always set up
//   -  to the corresponding ForallStmt or ForLoop node if one is found,
//   -  to NULL if neither is found.
//
//  * the other fields are set up only if eflopiLoop != NULL.
//
class EflopiInfo {
public:
  ForallStmt* eflopiForall;
  ForLoop*    eflopiLoop;
  CallExpr*   eflopiCall;
  // more details
  // todo soon: remove all except asgnToIter?
  Symbol*    iterSym;
  CallExpr*  asgnToIter;
  Symbol*    iterCallTemp;
  CallExpr*  asgnToCallTemp;
};

//
// Is 'forLoop' a loop over a parallel iterator?
// If so, fill in 'eInfo' with details.
//
// The current pattern-matching implementation is far from ideal.
// We could improve it by using for_SymbolSymExprs()
// or by having ForLoop include the iterator call directly.
//
static bool findCallToParallelIterator(ForLoop* forLoop, EflopiInfo& eInfo)
{
  Symbol* iterSym = forLoop->iteratorGet()->symbol();

  // Find an assignment to 'iterSym'.
  // It is usually located within a short walk before forLoop.
  CallExpr* asgnToIter = NULL;
  for (Expr* curr = forLoop->prev; curr; curr = curr->prev)
    if (CallExpr* call = toCallExpr(curr))
      if (call->isPrimitive(PRIM_MOVE))
        if (SymExpr* lhs1 = toSymExpr(call->get(1)))
          if (lhs1->symbol() == iterSym) {
            asgnToIter = call;
            break;
          }
  INT_ASSERT(asgnToIter);

  // We have:
  //   move( call_tmp call( ITERATOR args... ) )
  //   move( _iterator call( _getIterator call_tmp ) )
  // We need to see if args... contain a leader or standalone tag.
  // 'asgnToIter' is the second of the above moves. Find the first one.
  CallExpr* rhs1 = toCallExpr(asgnToIter->get(2));
  INT_ASSERT(rhs1 && rhs1->isNamed("_getIterator"));
  Symbol* calltemp = toSymExpr(rhs1->get(1))->symbol();
  CallExpr* asgnToCallTemp = NULL;
  for (Expr* curr = asgnToIter->prev; curr; curr = curr->prev)
    if (CallExpr* call = toCallExpr(curr))
      if (call->isPrimitive(PRIM_MOVE))
        if (SymExpr* lhs2 = toSymExpr(call->get(1)))
          if (lhs2->symbol() == calltemp) {
            asgnToCallTemp = call;
            break;
          }

  // Sometimes, e.g. forall over a range - ri-5-c.chpl, _getIterator is invoked
  // on a non-temp. We notice this by the absence of asgnToCallTemp.
  // In such a case, the iterator will be nontemp.these(),
  // which is not a parallel iterator.
  if (!asgnToCallTemp)
    return false;

  CallExpr* iterCall = toCallExpr(asgnToCallTemp->get(2));
  INT_ASSERT(iterCall);

  if (callingParallelIterator(iterCall)) {
    // Found it. Fill in the information before returning.
    eInfo.eflopiForall   = NULL;
    eInfo.eflopiLoop     = forLoop;
    eInfo.eflopiCall     = iterCall;
    eInfo.iterSym        = iterSym;
    eInfo.asgnToIter     = asgnToIter;
    eInfo.iterCallTemp   = calltemp;
    eInfo.asgnToCallTemp = asgnToCallTemp;
    return true;
  }

  // no signs of a parallel iterator
  return false;
}

// Starting with 'curr', look for an enclosing parallel loop, i.e.
// either a forall loop or a for loop over a parallel iterator.
// Set up 'eflopiInfo' as appropriate.
static void findEnclosingParallelLoop(Expr* curr, EflopiInfo& eflopiInfo) {
  while ((curr = curr->parentExpr)) {
    if (ForallStmt* fs = toForallStmt(curr)) {
      eflopiInfo.eflopiForall = fs;
      eflopiInfo.eflopiLoop   = NULL;
      return;
    }
    else if (ForLoop* forLoop = toForLoop(curr)) {
      if (findCallToParallelIterator(forLoop, eflopiInfo))
        // findCallToParallelIterator() filled out eflopiInfo.
        return;
    }
  }

  // Nothing of interest.
  eflopiInfo.eflopiForall = NULL;
  eflopiInfo.eflopiLoop   = NULL;
  return;
}

// We need expr->remove().
// If it is the only thing in its enclosing block, yank the block.
// Ditto the defer statement.
static void removeWithEnclosing(Expr* expr) {
  if (BlockStmt* block = toBlockStmt(expr->parentExpr))
    if (expr == block->body.head && expr == block->body.tail)
      {
        // Remove the entire block. Check for an enclosing defer first.
        if (DeferStmt* defer = toDeferStmt(block->parentExpr)) {
          defer->remove();
          return;
        }
        INT_ASSERT(block->parentExpr != NULL);
        block->remove();
        return;
      }
  expr->remove();
  return;
}

static ForallStmt* replaceEflopiWithForall(EflopiInfo& eInfo)
{
  // the loop to be replaced
  ForLoop* src  = eInfo.eflopiLoop;

  // Otherwise currentAstLoc points to the forall loop
  // that invokes the iterator src->parentSymbol.
  SET_LINENO(src);

  // the replacement loop
  ForallStmt* dest = ForallStmt::fromForLoop(src);

  // convert src index variable to dest index variable
  // todo instead use the corresponding user variable
  Symbol* srcIndex = src->indexGet()->symbol();
  src->indexGet()->remove(); // get it out of the way
  dest->inductionVariables().insertAtHead(srcIndex->defPoint->remove());
  // remove its uses outside the loop
  for_SymbolSymExprs(idxSE, srcIndex) {
    if (Expr* idxStmt = idxSE->getStmtExpr()) {
      // heuristic, instead of checking the full chain of parentExpr
      // todo soon: instead of heuristic, leverage getSingleDef()
      if (idxStmt->parentExpr != src) {
        INT_ASSERT(idxSE == srcIndex->getSingleDef());
        // todo soon: given that the above always holds, use it
        // and simplify the body of this for_SymbolSymExprs
        INT_ASSERT(isBlockStmt(idxStmt->parentExpr));
        INT_ASSERT(toBlockStmt(idxStmt->parentExpr)->body.length == 1);
        removeWithEnclosing(idxStmt);
      } else {
        INT_ASSERT(idxSE == srcIndex->getSingleUse());
      }
    }
  }

  // set the iterator call
  dest->iteratedExpressions().insertAtHead(new SymExpr(eInfo.iterCallTemp));

  // remove _iterator that keeps the iterator class
  Symbol* IC = src->iteratorGet()->symbol();
  src->iteratorGet()->remove(); // get it out of the way
  for_SymbolSymExprs(icSE, IC)
    removeWithEnclosing(icSE->getStmtExpr());
  IC->defPoint->remove();

  // no more uses
  INT_ASSERT(IC->firstSymExpr() == NULL);

  // no with-clause adjustments - the for loop does not have one

  // transfer the loop body
  AList& sourceBody = src->body;
  AList& resultBody = dest->loopBody()->body;
  while (Expr* forStmt = sourceBody.head)
    resultBody.insertAtTail(forStmt->remove());

  // replace the loop statement itself
  src->replace(dest);

  return dest;
}


/////////// assorted helpers ///////////

static void checkNoYieldsIn(FnSymbol* fn) {
  std::vector<CallExpr*> calls;
  collectCallExprs(fn, calls);
  for_vector(CallExpr, call, calls)
    INT_ASSERT(!call->isPrimitive(PRIM_YIELD));
}

static Symbol* makeAddrOfIfNeeded(Symbol* origSym, Expr* ref) {
  if (origSym->isRef()) {
    return origSym;
  }
  else {
    // If origSym->type==dtUnknown, should we use PRIM_SET_REFERENCE ?
    Type* tempType = origSym->getRefType();
    if (tempType == NULL) tempType = dtUnknown; // ex. for a reduce intent

    VarSymbol* atemp = newTemp("svarAddrTemp", tempType);
    atemp->qual = QUAL_REF;

    ref->insertBefore(new DefExpr(atemp));
    ref->insertBefore("'move'(%S, 'addr of'(%S))", atemp, origSym);

    return atemp;
  }
}

static void propagateRecursivelyNew(FIcontext& ctx);


/////////// misc / interface to the "old world" implementation ///////////

// A helper for inserting teardown code in reverse order of intents
// and tear down riShadowVar before riCurrentOp.
static void adjustInsertAnchor2(FIcontext& ctx) {
  ctx.anchor2 = ctx.anchor2prev->next;
}

//
// Set up anchor references, so we can add intent-related code
// into ctx.curFn via ref->insertBefore().
//
// ctx.anchor1 is for startup code at the beginning of a task.
// ctx.anchor2 is for teardown code at the end of a task.
//
// If anchors have been set up previously, move anchor2 *above*
// previously-inserted nodes.
//
static void setupAnchors(FIcontext& ctx) {
  if (ctx.anchor2prev != NULL) {
    // We have set this up already. Just update.
    adjustInsertAnchor2(ctx);
    return;
  }
  setupRedRefs(ctx.anchorFn, ctx.inTaskFn(), ctx.anchor1, ctx.anchor2);
  // When the old world goes away, we can switch to better names.
  ctx.anchor2orig = ctx.anchor2;
  ctx.anchor2prev = new CallExpr("redRef2prev");
  ctx.anchor2->insertBefore(ctx.anchor2prev);
}

static void cleanupAnchors(FIcontext& ctx) {
  // Like cleanupRedRefs(ctx.anchor1, ctx.anchor2), plus ctx.anchor2prev.

  if (ctx.anchor1 == NULL)
    // nothing to do
    return;

  ctx.anchor1->remove();      ctx.anchor1 = NULL;
  ctx.anchor2orig->remove();  ctx.anchor2orig = NULL;
  ctx.anchor2prev->remove();  ctx.anchor2prev = NULL;

  // Do not remove() anchor2 - it may result from adjustInsertAnchor2(),
  // not the one we allocated originally.
  ctx.anchor2 = NULL;
}

static void addArgsToToLeaderCallForPromotionWrapper(FIcontext& ctx) {
  // When the old world goes away, todo: eliminate this temporary array.
  int numSVars = ctx.numShadowVars();
  Symbol* fiFormalsTemp[numSVars];
  for (int ix = 0; ix < numSVars; ix++)
    fiFormalsTemp[ix] = ctx.fiInfos[ix].fiFormal;
  addArgsToToLeaderCallForPromotionWrapper(ctx.curFn, numSVars, fiFormalsTemp);
}


/////////// seting up for a reduce intent ///////////

// As we extend the yield expression into a tuple with shadow variables,
// we need to handle a reduce-intent variable specially by setting up
// the current reduction op and initializing the shadow variable from it.
//
// Setup at start of the task function:
//
//   def currentOp = parentOp.clone();
//   def shadowVar = currentOp.identity;
//
// Accumulate and tear down at the end.
//
//   currentOp.accumulate(shadowVar);
//   chpl__reduceCombine(parentOp, currentOp);
//   chpl__cleanupLocalOp(parentOp, currentOp);
//   // rely on later compiler passes to destruct shadowVar
//
// Exception 1: when the yield stmt is in the iterator itself, outside any
// parallel constructs, do not create/tear down the current op.
// Instead use the parent op directly, as there is no concurrent access to it.
//
// Exception 2: when the yield stmt is in a forall loop, we do not need
// the shadow variable created from the current op (or parent op).
// Instead the shadow variable we add to the yielded tuple is one
// we add to the forall loop's intentVariables(). It will be handled
// according to the above scheme when this forall loop's parallel iterator
// will be processed/extended.
// The above scheme is still used for yields outside a forall loop, if any.

//
// Set up fiInfo.riCurrentOp, if not already.
//
static void ensureCurrentReduceOpForReduceIntent(FIcontext& ctx,
                                                 FIinfo& fiInfo, int ix)
{
  if (fiInfo.riCurrentOp != NULL)
    return;

  INT_ASSERT(fiInfo.riShadowVar == NULL); // Can't have it without current op.

  Symbol* parentOp = fiInfo.fiFormal;

  if (ctx.inIterator()) {
    // We are in the parallel iterator outside any parallel constructs.
    // Use the parent op directly, as there is no concurrent access to it.
    fiInfo.riCurrentOp = parentOp;
    return;
  }

  Symbol* currOp = new VarSymbol(intentArgName(ix, "reduceCurrOp"));
  // 'currOp' always points to the same reduction op class instance.
  currOp->addFlag(FLAG_CONST);
  currOp->qual = QUAL_CONST_VAL;

  setupAnchors(ctx);

  ctx.anchor1->insertBefore(new DefExpr(currOp));
  ctx.anchor1->insertBefore("'move'(%S, clone(%S,%S))", // initialization
                            currOp, gMethodToken, parentOp);

  ctx.anchor2->insertBefore("chpl__reduceCombine(%S,%S)", parentOp, currOp);
  ctx.anchor2->insertBefore("chpl__cleanupLocalOp(%S,%S)", parentOp, currOp);

  fiInfo.riCurrentOp = currOp;
}

//
// Set up (if not already) and return fiInfo.riShadowVar.
//
static Symbol* shadowVarForReduceIntent(FIcontext& ctx,
                                        FIinfo& fiInfo, int ix)
{
  if (fiInfo.riShadowVar != NULL)
    return fiInfo.riShadowVar;

  Symbol* currOp = fiInfo.riCurrentOp;
  INT_ASSERT(currOp != NULL); // caller responsibility

  setupAnchors(ctx);

  // Create the shadow variable and set it up.
  VarSymbol* rsvar = new VarSymbol(intentArgName(ix, "reduceShadowVar"));
  rsvar->addFlag(FLAG_INSERT_AUTO_DESTROY);

  VarSymbol* stemp = newTemp("rsvTemp");
  ctx.anchor1->insertBefore(new DefExpr(rsvar));
  ctx.anchor1->insertBefore(new DefExpr(stemp));
  ctx.anchor1->insertBefore("'move'(%S, identity(%S,%S))",
                            stemp, gMethodToken, currOp);
  ctx.anchor1->insertBefore("'move'(%S, chpl__autoCopy(%S))",
                            rsvar, stemp);

  // Wrap it up at the end.
  ctx.anchor2->insertBefore("accumulate(%S,%S,%S)",
                            gMethodToken, currOp, rsvar);

  fiInfo.riShadowVar = rsvar;
  return rsvar;
}


/////////// when yield is inside a forall loop ///////////

/*
General Context
---------------

Let's say we are extending the parallel iterator with variables
that corresponds to forall intents. For example, from this:

  iter myIter(param tag == standalone)
  {
    ...
    yield 555;
    ...
  }

to this:

  iter myIter(param tag == standalone,
              x1_reduceParent:SumReduceScanOp, // reduce intent
              x2_foo:int)                      // in intent

 {
    var x1_reduceShadowVar = x1_reduceParent.identity;
    ...
    yield (555, addr-of(x1_reduceShadowVar), addr-of(x2_foo));
    ...
    x1_reduceParent.accumulate(x1_reduceShadowVar);
  }

Particular Concern
------------------

If the above yield is actually within a forall:

  iter myIter(param tag == standalone)
  {
    forall ...
    { ...
      yield 555;
      ... }
  }

then we need to create a new set of shadow variables under this forall
and yield those instead:

  iter myIter(param tag == standalone,
              x1_reduceParent:SumReduceScanOp, // reduce intent
              x2_foo:int)                      // in intent

 {
    forall ... with (reduce x1_reduceParent, in x2_foo)
    { ...
      yield (555, addr-of(x1_reduceShadowVar), addr-of(x2_foo));
      ... }
  }


Notice that 'x1_reduceShadowVar' is not needed in this case.
It is still needed if there are yields outside foralls and task constructs.

If the forall is inside a task function
---------------------------------------

The above approach applies, except instead of x1_reduceParent
we need to create and use a current reduce op.
The difference is the same as in the case
where the yield is not in any forall loops.

Look Out For...
---------------

Need to handle these cases:

* The yield is in >1 nested foralls --> create a new shadow var for each:

  iter myIter(param tag == standalone, x1_reduceParent:SumReduceScanOp) {
    var x1_reduceShadowVar = x1_reduceParent.identity;

    forall ... with (x1_reduceParent reduce x1_reduceShadowVar) {
      // shadow var: x1_reduceShadowVar'

      forall ... with (x1_reduceParent reduce x1_reduceShadowVar') {
        // shadow var: x1_reduceShadowVar''

        ... yield (555, addr-of(x1_reduceShadowVar'')); ...
      }
    }

    x1_reduceParent.accumulate(x1_reduceShadowVar);
  }

* Multiple yields within the same forall(s) --> use the same shadow var:

    forall ... with (x1_reduceParent reduce x1_reduceShadowVar) {
      // shadow var: x1_reduceShadowVar'

      ... yield (555, addr-of(x1_reduceShadowVar'')); ...
      ... yield (666, addr-of(x1_reduceShadowVar'')); ...
    }
*/

static Symbol* tupcomForYieldInForall(FIcontext& ctx, ForallStmt* efs, int ix)
{
  FIinfo&          fii = ctx.fiInfos[ix];
  ShadowVarSymbol* osv = fii.svSymbol;
  bool        isReduce = osv->isReduce();

  // Check whether we have already created a shadow variable
  // because we have processed another yield within this forall.
  // If so, reuse it.
  //
  // TODO need to look up in outer enclosing forall statements, if applicable.
  // If it is a parallel for, should convert that to a forall first, perhaps.
  // Without that lookup, in intent and reduce intent may result in races.
  // See also checkFor2ndEnclosingParallelLoop().
  //
  for_shadow_vars(efsShadVar, temp, efs) // linear search, for simplicity
    if (isReduce ?
        (efsShadVar->reduceGlobalOp != NULL &&
         efsShadVar->reduceGlobalOp == fii.riCurrentOp) :
        efsShadVar->outerVarSym()  == fii.fiFormal)
      return efsShadVar;

  // Here is the new shadow variable.
  ShadowVarSymbol* esv = new ShadowVarSymbol(osv->intent, osv->name, NULL);
  efs->intentVariables().insertAtTail(new DefExpr(esv));

  if (isReduce) {
    ensureCurrentReduceOpForReduceIntent(ctx, fii, ix);
    esv->reduceGlobalOp = fii.riCurrentOp;
    esv->outerVarRep    = NULL;  // no user-level outer var
  } else {
    esv->outerVarRep    = new SymExpr(fii.fiFormal);
  }

  return esv;
}

// Currently doubly-nested parallel loops around a yield
// in a parallel iterator do not work smoothly.
// So instead we issue a nice, albeit fatal, error.
//
// One need is for tupcomForYieldInForall() to look up the shadow variables
// in the second enclosing forall loop when applicable - see a todo item there.
//
static void checkFor2ndEnclosingParallelLoop(FIcontext& ctx,
                                      CallExpr* yieldCall, ForallStmt* efs) {
  EflopiInfo eInfo;
  findEnclosingParallelLoop(efs, eInfo);
  Expr* enclPar = eInfo.eflopiForall ? (Expr*)eInfo.eflopiForall
                                     : (Expr*)eInfo.eflopiLoop;
  if (enclPar == NULL)
    // No concern.
    return;

  USR_FATAL_CONT(yieldCall,
     "this yield is nested in two or more enclosing parallel loops");
  USR_PRINT(yieldCall,
     "(a parallel loop is a for-loop over parallel iterator or a forall-loop)");
  USR_PRINT(yieldCall,
     "this is currently not implemented within parallel iterators");

  USR_PRINT(efs, "the immediately enclosing parallel loop is here");
  USR_PRINT(enclPar, "the second enclosing parallel loop is here");
  USR_PRINT(ctx.forall, "the forall loop that invokes the iterator is here");

  // If we continue, we may get a const violation on x*_reduceShadowVar.
  USR_STOP();
}


/////////// extendYieldNew, simple case ///////////

static Symbol* tupcomForSerialYield(FIcontext& ctx, int ix)
{
  FIinfo& fii = ctx.fiInfos[ix];

  switch (fii.svSymbol->intent) {
    case  TFI_DEFAULT:
    {
      INT_ASSERT(false); // it is better to have a concrete intent here
      return NULL;
    }

    case TFI_CONST:
    case TFI_IN:
    case TFI_CONST_IN:
    case TFI_REF:
    case TFI_CONST_REF:
    {
      Symbol* component = fii.fiFormal;
      // When the type is dtUnknown, we still need to ensure yield by ref.
      // Ex. have makeAddrOfIfNeeded() insert a PRIM_SET_REFERENCE.
      INT_ASSERT(component->type != dtUnknown &&
                 component->type != dtAny);
      return component;
    }

    case TFI_REDUCE:
    {
      ensureCurrentReduceOpForReduceIntent(ctx, fii, ix);
      return shadowVarForReduceIntent(ctx, fii, ix);
    }
  }

  INT_ASSERT(false); // garbage intent
  return NULL;
}

//
// Switch from yielding a user value to yielding the tuple
//   (user value, shadow variable1, shadow variable2...)
//
// 'efs' is the enclosing ForallStmt if any, otherwise NULL.
//
static void extendYieldNew(FIcontext& ctx, CallExpr* yieldCall,
                           ForallStmt* efs)
{
  // Make a tuple that includes the extra args.
  Expr*      origRetArg = yieldCall->get(1)->remove();
  VarSymbol* newOrigRet = localizeYieldForExtendLeader(origRetArg, yieldCall);
  CallExpr*  buildTuple = new CallExpr("_build_tuple_always_allow_ref",
                                       newOrigRet);
  int numSVars   = ctx.numShadowVars();

  if (efs != NULL)
    checkFor2ndEnclosingParallelLoop(ctx, yieldCall, efs);

  for (int ix = 0; ix < numSVars; ix++) {
    Symbol* tupComponent = (efs == NULL) ?
      tupcomForSerialYield(ctx, ix) :
      tupcomForYieldInForall(ctx, efs, ix);

    // shadow variables are always yielded by reference
    tupComponent = makeAddrOfIfNeeded(tupComponent, yieldCall);
    buildTuple->insertAtTail(new SymExpr(tupComponent));
  }

  yieldCall->insertBefore("'move'(%S,%E)", ctx.retSym, buildTuple);
  yieldCall->insertAtTail(new SymExpr(ctx.retSym));
}


/////////// propagateExtraLeaderArgsNew ///////////

static ArgSymbol* newExtraFormal(FIcontext& ctx, FIinfo& fii, int ix,
                                 Symbol* eActual)
{
    bool     isReduce = fii.svSymbol->isReduce();
    const char* eName;

    if      (isReduce)
      eName = intentArgName(ix, "reduceParent");
    else if (ctx.nested)
      eName = eActual->name;  // name is already handled at the outer level
    else if (!strcmp(eActual->name, "_tuple_expand_tmp_"))
      eName = intentArgName(ix, "tet");
    else
      eName = intentArgName(ix, eActual->name);  // uniquify user arg name

    Type* efType    = eActual->type;
    IntentTag efInt = isReduce ? INTENT_CONST_IN :
      concreteIntent(argIntentForForallIntent(fii.svSymbol->intent), efType);
    bool addFlagImm = false;

    if (efInt & INTENT_FLAG_REF) {
      INT_ASSERT(!isReduce); // otherwise efType may be unknown
      INT_ASSERT(efType != dtUnknown && efType != dtAny);

      //
      // For ref intents, we need to make it a ref type.
      // Because eFormal will be passed to _build_tuple_always_allow_ref()
      // call that's created in extendYieldNew().
      // If _build_tuple_always_allow_ref's actual has a non-ref type,
      // even if it is an ArgSymbol with a ref intent, the corresponding
      // component of the resulting tuple will be non-ref, which will break
      // SSCA2 and test/parallel/forall/vass/intents-all-int.chpl.
      // Todo: fix resolution of _build_tuple_always_allow_ref.
      //
      efType = efType->getRefType();

      if (eActual->isConstValWillNotChange())
        addFlagImm = true;
    }
    
    ArgSymbol*  eFormal = new ArgSymbol(efInt, eName, efType);

    if (addFlagImm || eActual->hasFlag(FLAG_REF_TO_IMMUTABLE))
      eFormal->addFlag(FLAG_REF_TO_IMMUTABLE);

    return eFormal;
}

static void propagateExtraLeaderArgsNew(FIcontext& ctx)
{
  if (ctx.curFn->hasFlag(FLAG_WRAPPER))
  {
    // We are not handling void-returning wrappers at the moment.
    INT_ASSERT(!(ctx.curFn->getReturnSymbol() == gVoid ||
                 ctx.curFn->retType == dtVoid));
  }

  const int numSVars = ctx.numShadowVars();

  for (int ix = 0; ix < numSVars; ix++)
  {
    FIinfo&    fii     = ctx.fiInfos[ix];
    Symbol*    eActual = fii.fiActual; // 'e' for "Extra"
    ArgSymbol* eFormal = newExtraFormal(ctx, fii, ix, eActual);
    fii.fiFormal       = eFormal;

    // Use named args to disambiguate from user's iterator formals, if any.
    ctx.call->insertAtTail(new NamedExpr(eFormal->name,
                                         new SymExpr(eActual)));
    ctx.curFn->insertFormalAtTail(eFormal);
  }

  if (ctx.inIterator() && ctx.curFn->hasFlag(FLAG_PROMOTION_WRAPPER))
  {
    INT_ASSERT(!ctx.anchor1); // no need to clean them up
    addArgsToToLeaderCallForPromotionWrapper(ctx);
  }
  else
  {
    propagateRecursivelyNew(ctx);
    cleanupAnchors(ctx);
  }
}


/////////// propagateRecursivelyNew ///////////

static void propagateRecursivelyNew(FIcontext& ctx)
{
  std::vector<CallExpr*> rCalls;
  collectMyCallExprs(ctx.curFn, rCalls, ctx.curFn);

  for_vector(CallExpr, rcall, rCalls) {
    if (rcall->isPrimitive(PRIM_YIELD)) {

      EflopiInfo eflopiInfo;
      findEnclosingParallelLoop(rcall, eflopiInfo);

      if (ForallStmt* efs = eflopiInfo.eflopiForall)
      {
        // There is an enclosing forall loop.
        extendYieldNew(ctx, rcall, efs);
      }
      else if (eflopiInfo.eflopiLoop != NULL)
      {
        // There is an enclosing parallel for loop.
        // See also "if (eflopiCall) {" and "USR_WARN(shadowvar,"
        // in former propagateThroughYieldNew().

        ForallStmt* efs = replaceEflopiWithForall(eflopiInfo);
        extendYieldNew(ctx, rcall, efs);
      }
      else
      {
        // No enclosing forall or parallel for.
        extendYieldNew(ctx, rcall, NULL);
      }

    } else if (FnSymbol* tfn = resolvedToTaskFun(rcall)) {

      // We will be extending 'tfn' specifically to this situation.
      // So there better be no other users of it.
      // todo soon: INT_ASSERT(rcall == tfn->singleInvocation());

      if (needsCapture(tfn)) {

        if (tfn->hasFlag(FLAG_BEGIN)) {
          // A 'begin' block should not have yields,
          // so nothing to do here. Just check it.
          if (fVerify) checkNoYieldsIn(tfn);
        }
        else {
          // Ideally, skip this if 'tfn' has no yields.
          // However, that is rare, and checking for that is expensive.
          // So for now we do not check and always process the tfn.

          // Propagate the extra args recursively into 'tfn'.
          FIcontext taskCntx(ctx, rcall, tfn);
          propagateExtraLeaderArgsNew(taskCntx);
        }

      } else {
        // !needsCapture(tfn) => descend into 'tfn' without argument intents.

        // FIcontext remains the same except for curFn.
        FnSymbol* saveCurFn = ctx.curFn;
        ctx.curFn = tfn;

        propagateRecursivelyNew(ctx);

        // Restore curFn. anchorFn can stay the same.
        ctx.curFn = saveCurFn;
      }
    }
  }
}


/////////// extendLeaderNew ///////////

void extendLeaderNew(ForallStmt* fs, 
                     FnSymbol* origIterFn, CallExpr* iterCall)
{
  if (!pristineLeaderIterators.get(origIterFn))
    stashPristineCopyOfLeaderIter(origIterFn, false);

  int numSVars = fs->numIntentVars();
  if (numSVars == 0)
    // no outer variables in the loop body - nothing to do
    return;

  // Replace the callee with a clone.
  if (origIterFn->isResolved()) {
    // ... using a pristine copy if the callee is already resolved.
    // See the comment on an assert in copyLeaderFn().
    origIterFn = pristineLeaderIterators.get(origIterFn);
    INT_ASSERT(origIterFn);
  }

  FnSymbol* iterFn = copyLeaderFn(origIterFn, /*ignore_isResolved:*/false);
  iterFn->instantiationPoint = getVisibilityBlock(iterCall);
  iterCall->baseExpr->replace(new SymExpr(iterFn));

  // Setup the new return/yield symbol.
  VarSymbol* retSym  = NULL;
  Symbol* origRetSym = NULL;

  if (!iterFn->hasFlag(FLAG_PROMOTION_WRAPPER)) {
    retSym  = newTemp("ret"); // its type is to be inferred
    origRetSym = iterFn->replaceReturnSymbol(retSym, /*newRetType*/NULL);
    origRetSym->defPoint->insertBefore(new DefExpr(retSym));
    origRetSym->name = "origRet";
  }

  // Data for the call from the ForallStmt to the parallel iterator.
  FIcontext ctx(fs, iterCall, iterFn, retSym);

  // Initialize it corresponding to the shadow vars from 'fs'.
  // Remove the corresponding actuals from 'iterCall'
  // so that propagateExtraLeaderArgsNew() can uniformly add them in
  // wrapping in NamedExprs.
  // They are added in addActualsToParCallNew().
  //
  // Note that here we may have already disconnected from ForallStmt
  // and its shadow variables' outerVarRep. Because 'iterCall' may be
  // inside a wrapper or iterator forwarder, so iterCall's actuals
  // are not necessarily the same as outerVarReps.

  Expr* origArg = iterCall->get(iterCall->numActuals() - numSVars + 1);
  int ix = 0;

  for_shadow_vars(svar, temp, fs) {
    FIinfo& fii = ctx.fiInfos[ix];
    fii.svSymbol = svar;

    Expr*   nextArg = origArg->next;
    SymExpr* origSE = toSymExpr(origArg->remove());
    INT_ASSERT(origSE);
    fii.fiActual = origSE->symbol();
    // as noted above, fiActual may differ from svar->outerVarSym()

    origArg = nextArg;
    ix++;
  }

  INT_ASSERT(!origArg);        // we should have processed all args
  INT_ASSERT(ix == numSVars);  // ... and all SVars

  propagateExtraLeaderArgsNew(ctx);

  if (origRetSym) {
    checkAndRemoveOrigRetSym(origRetSym, iterFn);
  }
}
