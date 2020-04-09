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

#include "astutil.h"
#include "AstVisitorTraverse.h"
#include "CForLoop.h"
#include "ForLoop.h"
#include "ForallStmt.h"
#include "iterator.h"
#include "passes.h"
#include "resolution.h"
#include "stringutil.h"
#include "wellknown.h"
#include <set>

/*

This file provides parts of the implementation of forall loops
and forall intents, specifically:

* Setting up shadow variables and other things - during resolution.

* Lowering forall loops by inlining the corresponding parallel
iterators - during lowerIterators.

During lowerIterators we are mostly not concerned about const-ness,
so we may be blurring const and non-const intents.
We DO need to preserve some const properties to enable optimizations.

---------------------------------

Jargon:

* svar, SV - a shadow variable, or ShadowVarSymbol

* ovar - the outer variable, or ShadowVarSymbol::outerVarSym()

* "current variable" - what a given shadow variable becomes
  in a particular task; it is a VarSymbol or ArgSymbol

---------------------------------

To lower a forall statement:

* The parallel iterator is inlined, like it is a regular call.

* Each yield statement is replaced by a clone of the forall loop body.

  While cloning, the shadow variables in the loop body are replaced
  with "current variables" that are in effect for that yield.
  The mapping from shadow variables to current variables is
  maintained in 'svar2clonevar' - see below.

  The shadow variables themselves do not remain in the AST
  after lowering.

* The inlined body of the parallel iterator is traversed using ExpandVisitor.
  "Current variables" are created and additional code is inserted
  in certain key places.

  The SymbolMap in ExpandVisitor::svar2clonevar records the current variable
  for each shadow variable. This map is created at start of the traversal,
  which corresponds to the start of the iterator body. A new map is created
  and maintained for traversing into each task function and forall loop
  of the cloned body of the parallel iterator.

  Cloning, aka copy(), of the loop body uses whatever svar2clonevar map
  is in effect at the point where the traversal encounters the corresponding
  yield statement.

* The key places during the traversal are:

 - The start and end of the iterator body.
 - A task-parallel construct i.e. a call to a task function.
 - A forall loop.
 - A yield statement.

---------------------------------

The additional code is inserted for each shadow variable SV
in the order of shadow variables on shadowVariables() list.
Deinitialization items are inserted in the reverse order.

*** start of the iterator body ***

includes setting up svar2clonevar - the SymbolMap
  in effect during the traversal, except as indicated

(end) code added after the end of the inlined iterator body

TFI_IN_PARENT
  nothing - it supports IN intents

TFI_IN, TFI_CONST_IN
  def currentVar
  currentVar.init(outerVarSym() of the corresponding TFI_IN_PARENT)
  svar2clonevar: SV --> currentVar

(end)
  currentVar.deinit() // for records

TFI_REF, TFI_CONST_REF
  // use the outer var directly
  svar2clonevar: SV --> SV->outerVarSym()

TFI_REDUCE_OP
  // use the reduce op already set up for us
  svar2clonevar: SV --> SV->outerVarSym()

TFI_REDUCE_PARENT_AS
TFI_REDUCE_PARENT_OP
  // t.b.d.

TFI_REDUCE
  def currentVar
  currentVar.init((the corresponding TFI_REDUCE_OP).identity)
  svar2clonevar: SV --> currentVar

(end)
  (the corresponding TFI_REDUCE_OP).accumulate(currentVar)
  currentVar.deinit() // for records

TFI_TASK_PRIVATE
  def currentVar
  svar2clonevar: SV --> currentVar
  initialize currentVar according to user specification

(end)
  currentVar.deinit() // for records

*** call to task function ***

(actual) an actual is added to the call
and a formal is added to the task function

(in task fn) actions performed there
  includes setting up a new svar2clonevar map
  in effect during the traversal of the task fn, except as indicated

(end) deinit actions at the end of the task function

TFI_IN_PARENT
  nothing - it supports IN intents

TFI_IN, TFI_CONST_IN
(actual) svar2clonevar(SV)

(in task fn) for POD
  svar2clonevar: SV --> the corresponding formal

(in task fn) for records
  def currentVar
  currentVar.init(the corresponding formal)
  svar2clonevar: SV --> currentVar

(end) for records
  currentVar.deinit()

TFI_REF, TFI_CONST_REF
(actual)     svar2clonevar(SV)
(in task fn) svar2clonevar: SV --> the corresponding formal

TFI_REDUCE_OP
(actual) svar2clonevar(SV)

TFI_REDUCE_PARENT_AS
TFI_REDUCE_PARENT_OP
  // t.b.d.

(in task fn)
  def currentVar
  currentVar.init((the corresponding formal).clone())
  svar2clonevar: SV --> currentVar

(end)
  chpl__reduceCombine((the corresponding formal), currentVar)
  chpl__cleanupLocalOp((the corresponding formal), currentVar)

TFI_REDUCE
(actual) none

(in task fn)
  def currentVar
  currentVar.init((the corresponding TFI_REDUCE_OP).identity)
  svar2clonevar: SV --> currentVar

(end)
  (the corresponding TFI_REDUCE_OP).accumulate(currentVar)
  currentVar.deinit() // for records

TFI_TASK_PRIVATE
  def currentVar
  svar2clonevar: SV --> currentVar
  initialize currentVar according to user specification

(end)
  currentVar.deinit() // for records

*** nested forall loop ***

For each SV:
- clone it --> SVclone
- set SVclone->outerVarSym() to svar2clonevar(SV)
  which is typically the currentVar created above
- append SVclone to forall loop's ForallStmt::shadowVariables()
- set svar2clonevar: SV --> SVclone
  in effect during the traversal of the ForallStmt

*** yield statement ***

Replace with a clone of the loop body
(of the ForallStmt being inlined).
Use the current svar2clonevar map when cloning.

---------------------------------

In more detail - for an IN intent at *** start of the iterator body *** :

ForallStmt::shadowVariables() has DefExprs of two ShadowVarSymbols:
 TFI_IN_PARENT comes first,
 TFI_IN or TFI_CONST_IN comes immediately after.

Call them SVO and SVI, respectively.
ForallStmt::loopBody() references SVI.
SVI->initBlock() contains SVI.init(SVO), resolved and lowered appropriately.

'currVar' (created below) is the variable to be used for SVI
when the loop body is executed due to a yield at the top level of the
iterator body, i.e. a yield that is outside any parallel constructs.

Here are the steps performed by expandTopLevel() / expandShadowVarTopLevel():

* svar2clonevar.put(SVO, SVI->outerVarSym())
* create currVar
* svar2clonevar.put(SVI, currVar)
* add DefExpr(currVar) to start of the cloned iterator body
* add SVI->initBlock()->copy(svar2clonevar) immediately after
* add SVI->deinitBlock()->copy(svar2clonevar) at end of iterator body

For a reduce intent, almost the same steps are performed.
The generated code is somewhat different because:
* The ShadowVarSymbols in ForallStmt::shadowVariables() are, in order:
TFI_REDUCE_PARENT_OP, TFI_REDUCE_PARENT_AS, TFI_REDUCE_OP, TFI_REDUCE.
* initBlock() and deinitBlock() contain the sequences needed
to set up/tear down the reduceOp class and the accumulation state.

For a task-private intent, again these are the same steps,
except:
* there is no compiler-introduced "companion" ShadowVarSymbol,
* initBlock() reflects the variable's type and/or initialization
expression as specified by the user in the with-clause.

---------------------------------

Key fields of ShadowVarSymbol used here:

* the outer variable:  ShadowVarSymbol::outerVarSym()
* the init block:      ShadowVarSymbol::initBlock()
* the deinit block:    ShadowVarSymbol::deinitBlock()

The init/deinit blocks specify how the shadow variable
is initialized or destructed. For example:

* [const] in: the shadow variable is copy-initialized
  from its outer variable at start of the task
  and deinitialized at end of the task.
  For a class variable, it is a pointer assignment
  and a no-op, respectively.

* [const] ref: a pointer assignment and a no-op.

* ReduceOp pointer: initialized by clone()
  on the parent task's ReduceOp. At the end of the task,
  it is combine()-ed into the parent ReduceOp, then delete-ed.

* Accumulation State for a reduction: initialized
  by calling identity on the corresponding ReduceOp.
  accumulate()-ed into the ReduceOp before deinitialization.

The init/deinit blocks have to be set up during resolution
because some of their code would not resolve afterwards.
Ditto ForallStmt::fRecIterGetIterator,fRecIterFreeIterator.
This is because unused functions are pruned at the end of resolution.

---------------------------------

Further key todos:

* When a global is passed by [const] ref intent,
replace references to the corresponding shadow variable
in loop body with the global itself.
Note: be aware that, in the loop body, the compiler adds derefs
from such a shadow variable - because it is a "ref".

*/


///////////                                                      ///////////
/////////// Lower ForallStmts by inlining the parallel iterator. ///////////
///////////                                                      ///////////


class ExpandVisitor;
static void expandYield( ExpandVisitor* EV, CallExpr* yield);
static void expandTaskFn(ExpandVisitor* EV, CallExpr* call, FnSymbol* taskFn);
static void expandForall(ExpandVisitor* EV, ForallStmt* fs);


/////////// ExpandVisitor visitor ///////////

class ExpandVisitor : public AstVisitorTraverse {
public:
  ForallStmt* const forall;
  SymbolMap& svar2clonevar;
  std::vector<Expr*> delayedRemoval;

  ExpandVisitor(ForallStmt* fs, SymbolMap& map);
  ExpandVisitor(ExpandVisitor* parentEV, SymbolMap& map);
  ~ExpandVisitor();

  virtual bool enterCallExpr(CallExpr* node) {
    if (node->isPrimitive(PRIM_YIELD)) {
      expandYield(this, node);
    }
    else if (FnSymbol* calledFn = node->resolvedFunction()) {
      if (isTaskFun(calledFn)) {
        expandTaskFn(this, node, calledFn);
      } else if (calledFn == gChplPropagateError) {
        handleChplPropagateErrorCall(node);
      }
    }
    // There shouldn't be anything interesting inside the call.
    // expandTaskFn() takes care of descending into 'taskFn'.
    return false;
  }

  virtual bool enterForallStmt(ForallStmt* node) {

    if (forall->hasVectorizationHazard()) {
      node->setHasVectorizationHazard(true);
    }

    expandForall(this, node);
    // expandForall() takes care of descending into 'node'
    return false;
  }

  virtual bool enterCForLoop(CForLoop* node) {
    if (forall->hasVectorizationHazard()) {
      node->setHasVectorizationHazard(true);
    }
    return true;
  }
  virtual bool enterForLoop(ForLoop* node) {
    if (forall->hasVectorizationHazard()) {
      node->setHasVectorizationHazard(true);
    }
    return true;
  }
};

// constructor for the outer level
ExpandVisitor::ExpandVisitor(ForallStmt* fs, SymbolMap& map) :
  forall(fs),
  svar2clonevar(map),
  delayedRemoval()
{
}

// constructor for a nested situation
ExpandVisitor::ExpandVisitor(ExpandVisitor* parentEV, SymbolMap& map) :
  forall(parentEV->forall),
  svar2clonevar(map),
  delayedRemoval()
{
}

ExpandVisitor::~ExpandVisitor() {
  for_vector(Expr, expr, delayedRemoval)
    expr->remove();
}


/////////// misc helpers ///////////

// Remove the return statement and the def of 'ret'. Return 'ret'.
// See also removeRetSymbolAndUses().
static Symbol* removeParIterReturn(BlockStmt* cloneBody, Symbol* retsym) {
  CallExpr* retexpr = toCallExpr(cloneBody->body.tail);
  INT_ASSERT(retexpr && retexpr->isPrimitive(PRIM_RETURN));
  if (retsym == NULL) {
    retsym = toSymExpr(retexpr->get(1))->symbol();
    INT_ASSERT(retsym->type->symbol->hasFlag(FLAG_ITERATOR_RECORD));

  } else {
    CallExpr* move = toCallExpr(retsym->getSingleDef()->getStmtExpr());
    INT_ASSERT(move->isPrimitive(PRIM_MOVE) || move->isPrimitive(PRIM_ASSIGN));
    retsym = toSymExpr(move->get(2))->symbol();
    move->remove();
  }

  retexpr->remove();
  return retsym;
}

static void removeVoidReturn(BlockStmt* cloneBody) {
  CallExpr* retexpr = toCallExpr(cloneBody->body.tail);
  INT_ASSERT(retexpr && retexpr->isPrimitive(PRIM_RETURN));
  INT_ASSERT(toSymExpr(retexpr->get(1))->symbol() == gVoid);

  retexpr->remove();
}

/////////// standardized svar actions ///////////

// When ForallStmt::loopBody() is inlined upon encountering a yield
// in the parallel iterator, each shadow variable within this loopBody
// is replaced by a newly-created VarSymbol, or possibly other things -
// depending on its TFI_ tag etc.
//
// No ShadowVarSymbols remain in the AST after a ForallStmt
// is lowered by inlining the parallel iterator.
//
// The following functions create the VarSymbol that will replace the
// shadow variable...

// ... of [const] in intent
static VarSymbol* createCurrIN(ShadowVarSymbol* SI) {
  INT_ASSERT(!SI->isRef());
  VarSymbol* currSI = new VarSymbol(SI->name, SI->type);
  currSI->qual = SI->isConstant() ? QUAL_CONST_VAL : QUAL_VAL;
  if (SI->isConstant()) currSI->addFlag(FLAG_CONST);
  return currSI;
}

// ... for the ReduceOp class of a reduce intent
static VarSymbol* createCurrRP(ShadowVarSymbol* RP) {
  VarSymbol* currRP = new VarSymbol(RP->name, RP->type);
  currRP->qual = QUAL_CONST_VAL;
  return currRP;
}

// ... for the Accumulation State of a reduce intent
static VarSymbol* createCurrAS(ShadowVarSymbol* AS) {
  VarSymbol* currAS = new VarSymbol(astr("AS_", AS->name), AS->type);
  currAS->qual = QUAL_VAL;
  return currAS;
 }

// ... for a task-private variable
static VarSymbol* createCurrTPV(ShadowVarSymbol* TPV) {
  VarSymbol* currTPV = new VarSymbol(TPV->name, TPV->type);
  currTPV->qual = TPV->qual;
  if (TPV->hasFlag(FLAG_CONST))   currTPV->addFlag(FLAG_CONST);
  if (TPV->hasFlag(FLAG_REF_VAR)) currTPV->addFlag(FLAG_REF_VAR);
  return currTPV;
}

static void addDefAndMap(Expr* aInit, SymbolMap& map, ShadowVarSymbol* svar,
                         VarSymbol* currVar)
{
  if (currVar->type == dtNothing) {
    INT_ASSERT(currVar->firstSymExpr() == NULL);
    return;
  }
  aInit->insertBefore(new DefExpr(currVar));
  map.put(svar, currVar);
}

static void addCloneOfInitBlock(Expr* aInit, SymbolMap& map, ShadowVarSymbol* svar) {
  //
  // We have to clone IB as a whole. This is to ensure that the uses
  // of any symbols that the original IB defines (via DefExprs
  // within that IB) get converted to uses of these symbols' copies.
  //
  BlockStmt* copyIB = svar->initBlock()->copy(&map);
  aInit->insertBefore(copyIB);
  // Let's drop the BlockStmt wrapper, to simplify the AST.
  copyIB->flattenAndRemove();
}

static void addCloneOfDeinitBlock(Expr* aFini, SymbolMap& map, ShadowVarSymbol* svar) {
  BlockStmt* copyDB = svar->deinitBlock()->copy(&map);
  aFini->insertAfter(copyDB);
  // Let's drop the BlockStmt wrapper, to simplify the AST.
  copyDB->flattenAndRemove();
}


/////////// checkForallsInInitDeinitBlocks ///////////

/*
Consider the following situation:

* We are lowering a ForallStmt 'fs1' by inlining its iterator,
  which invokes a task function. While inlining the task function...

* For each of fs1's ShadowVarSymbol, we add its init block
  to the start of (a clone of) the task function, and its deinit block
  to the end of that clone. Because a ShadowVarSymbol's init and deinit
  blocks exist exactly to contain the task-startup and task-shutdown actions.

* If one of these de/init blocks contains its own ForallStmt 'fs2', then
  we create a copy of it. That copy gets copies of fs1's ShadowVarSymbols.

* Later, it will come time to lower that copy by inlining its iterator.
  If that iterator calls task function(s), they will get copies of fs1's
  ShadowVarSymbols' de/init blocks. Which will create yet another ForallStmt.

* This process will continue ad infinitum.

The below check issues an error if there is a danger of that happening.

We could mitigate the impact by inlining the potentially-offending ForallStmts
first. For that, such a ForallStmt should either not invoke task functions, or
not contain other ForallStmts in its ShadowVarSymbols' de/init blocks, if any.
Leaving this a future work for now.
*/

static std::set<ForallStmt*> forallsAlreadyChecked;

static void checkForallsInShadowVarBlock(ShadowVarSymbol* svar,
                                         BlockStmt* block, bool& gotError) {
  std::vector<ForallStmt*> fss;
  collectForallStmts(block, fss);

  if (! fss.empty()) {
    gotError = true;
    USR_FATAL_CONT(svar,
      "A forall statement with a shadow or task-private var '%s'"
      " containing, in turn, another forall statement is not implemented",
      svar->name);
  }
}

static void checkForallsInInitDeinitBlocks(ForallStmt* forall) {
  if (forallsAlreadyChecked.count(forall)) return;
  forallsAlreadyChecked.insert(forall);
  bool gotError = false;

  for_shadow_vars(svar, temp, forall) {
    checkForallsInShadowVarBlock(svar, svar->initBlock(), gotError);
    checkForallsInShadowVarBlock(svar, svar->deinitBlock(), gotError);
  }

  // Allow other USR_FATAL_CONTs if this ForallStmt is clear.
  if (gotError) USR_STOP();
}


/////////// expandYield ///////////

// Replace 'yield' with a clone of the forall loop body.
static void expandYield(ExpandVisitor* EV, CallExpr* yieldCall)
{
  // Todo: update EV->svar2clonevar in-place, then reset to NULL,
  // to avoid map creation+destruction cost.

  // This will be svar2clonevar (the incoming "current map")
  // plus a clone of the induction variable.
  SymbolMap map;
  map.copy(EV->svar2clonevar);

  // This adds (original idxVar -> cloneIdxVar) to 'map'.
  // There is only one idx var because all zippering has been lowered away.
  VarSymbol* cloneIdxVar = parIdxVar(EV->forall)->copy(&map);
  yieldCall->insertBefore(new DefExpr(cloneIdxVar));

  Expr* yieldExpr = yieldCall->get(1)->remove();
  yieldCall->insertBefore(new CallExpr(PRIM_MOVE, cloneIdxVar, yieldExpr));

  BlockStmt* bodyClone = EV->forall->loopBody()->copy(&map);
  addIteratorBreakBlocksInline(EV->forall->loopBody(), NULL,
                               bodyClone, yieldCall, &EV->delayedRemoval);
  yieldCall->replace(bodyClone);

  // Note that we are not descending into 'bodyClone'.  The only thing
  // that needs to be done there is to lower its ForallStmts, if any.
  // That will be done when lowerForallStmtsInline() gets to them.
  // They are guaranteed to come later because we just created them
  // when doing loopBody->copy(), so added at the end of gForallStmts.
}


/////////// expandTaskFn ///////////

//
// At this point in compilation, in certain cases an in-intent formal is
// represented as a ref-intent formal + copy construction into a "formal temp".
// See insertFormalTemps(). As a result, in-intent formals are treated
// as PODs, with argument passing implemented as memcpy,
//
// So, we need to mimic that here. Otherwise we miss copy-construction
// from the actual into the formal.
//
static void addFormalTempSIifNeeded(FnSymbol* cloneTaskFn, Expr* aInit,
                                    SymbolMap& map, ShadowVarSymbol* SI)
{
  // This record must be present and contain the eFormal - see addArgAndMap().
  SymbolMapElem* e = map.get_record(SI);
  ArgSymbol* eFormal = toArgSymbol(e->value);

  if (!formalRequiresTemp(eFormal, cloneTaskFn))
    return; // not that case

  VarSymbol* currSI = createCurrIN(SI);
  aInit->insertBefore(new DefExpr(currSI));

  // map(SI) = currSI; map(SO) = eFormal
  e->value = currSI;
  map.put(SI->ParentvarForIN(), eFormal);

  eFormal->intent         = INTENT_CONST_REF;
  eFormal->originalIntent = INTENT_CONST_REF;  // (*)
  // non-ref type is ok for eFormal

  // (*) We need this adjustment, otherwise shouldAddFormalTempAtCallSite()
  // in insertSerialization() / remoteValueForwarding will get confused.
  // Check these tests for valgrind and memLeaks under --no-local and numa:
  //   parallel/forall/in-intents/coforall-plus-on
  //   parallel/forall/in-intents/both-arr-dom-const-const
  //   parallel/forall/in-intents/both-arr-dom-var-const

  addCloneOfInitBlock(aInit, map, SI);
}

static IntentTag argIntentForForallIntent(ForallIntentTag tfi) {
  switch (tfi) {
    case TFI_DEFAULT:      return INTENT_BLANK;
    case TFI_CONST:        return INTENT_CONST;
    case TFI_IN:           return INTENT_IN;
    case TFI_CONST_IN:     return INTENT_CONST_IN;
    case TFI_REF:          return INTENT_REF;
    case TFI_CONST_REF:    return INTENT_CONST_REF;
    case TFI_REDUCE_OP:    return INTENT_CONST_IN;

    case TFI_IN_PARENT:
    case TFI_REDUCE:
    case TFI_REDUCE_PARENT_AS:
    case TFI_REDUCE_PARENT_OP:
    case TFI_TASK_PRIVATE:
      // argIntentForForallIntent() is not intended for these
      INT_ASSERT(false); return INTENT_BLANK;
  }
  INT_ASSERT(false); return INTENT_BLANK;
}

static ArgSymbol* newExtraFormal(ShadowVarSymbol* svar, int ix,
                                 Symbol* eActual, bool nested)
{
    Type*     efType = eActual->type;
    IntentTag  efInt = argIntentForForallIntent(svar->intent);
    bool  addFlagImm = false;

    if (efInt & INTENT_FLAG_REF) {
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
      // Or, create a _build_tuple specifically when it is known
      // which formals/components should be by ref.
      //
      efType = efType->getRefType();

      if (eActual->isConstValWillNotChange())
        addFlagImm = true;
    }

    ArgSymbol* eFormal = new ArgSymbol(efInt, svar->name, efType);

    if (eFormal->isRef() &&
        (addFlagImm || eActual->hasFlag(FLAG_REF_TO_IMMUTABLE)))
      eFormal->addFlag(FLAG_REF_TO_IMMUTABLE);

    return eFormal;
}

//
// When eActual is not a ref whereas the formal is a ref, insert
// an explicit address-of. Otherwise create_arg_bundle_class() in parallel.cpp
// will pass our actual by value in the argument bundle. Ex.
//   parallel/forall/vass/default-intent-record-with-int-field
//
static Symbol* eActualOrRef(Expr* ref, ShadowVarSymbol* svar, Symbol* eActual)
{
  if (!svar->isRef() || eActual->isRef())
    return eActual;

  VarSymbol* temp = newTempConst("eaAddrOf", eActual->getRefType());
  ref->insertBefore(new DefExpr(temp));
  ref->insertBefore("'move'(%S,'addr of'(%S))", temp, eActual);
  return temp;
}

static void addArgAndMap(FnSymbol* cloneTaskFn, CallExpr* callToTFn,
                         int numOrigActuals, SymbolMap& iMap,
                         SymbolMap& map, ShadowVarSymbol* svar,
                         int ix, Symbol* mappee = NULL)
{
  Symbol* eActual = iMap.get(svar);   // 'e' for "extra" (i.e. newly added)
  callToTFn->insertAtTail(eActualOrRef(callToTFn, svar, eActual));

  ArgSymbol* eFormal = newExtraFormal(svar, ix, eActual, /*nested:*/true);
  cloneTaskFn->insertFormalAtTail(eFormal);
  map.put(mappee ? mappee : svar, eFormal);
}

static void expandShadowVarTaskFn(FnSymbol* cloneTaskFn, CallExpr* callToTFn,
                                  Expr* aInit, Expr* aFini,
                                  int numOrigAct, SymbolMap& iMap,
                                  SymbolMap& map, ShadowVarSymbol* svar,
                                  int ix) {
  SET_LINENO(svar);
  switch (svar->intent)
  {
    case TFI_IN:
    case TFI_CONST_IN:
      addArgAndMap(cloneTaskFn, callToTFn, numOrigAct, iMap,
                   map, svar, ix);
      addFormalTempSIifNeeded(cloneTaskFn, aInit, map, svar);
      addCloneOfDeinitBlock(aFini, map, svar);
      break;

    case TFI_REF:
    case TFI_CONST_REF:
      addArgAndMap(cloneTaskFn, callToTFn, numOrigAct, iMap,
                   map, svar, ix);
      // no init/deinit
      break;

    case TFI_REDUCE:
    {
      ShadowVarSymbol *PRP=0, *PAS=0, *RP=0, *AS=svar;
      setReduceSVars(PRP, PAS, RP, AS);

      /*
      We have in iMap:
      - RP+AS svars --> current RP+AS vars in cloneTaskFn's caller.

      We add:
      - caller's current RP+AS vars as actuals to callToTFn
      - PRP+PAS formals to cloneTaskFn
      - curr RP+AS vars to cloneTaskFn
      - map:
         PRP+PAS svars --> the PRP+PAS formals
         RP+AS svars --> the curr RP+AS vars

      [Currently there is no PAS/AS formal/actual. They are upcoming.]
      */

      addArgAndMap(cloneTaskFn, callToTFn, numOrigAct, iMap,
                   map, RP, ix, PRP);
      map.get(PRP)->name = PRP->name; // tweak the name
      addDefAndMap(aInit, map, RP, createCurrRP(RP));
      addDefAndMap(aInit, map, AS, createCurrAS(AS));

      // Init/deinit code is all with AS.
      addCloneOfInitBlock(aInit, map, AS);
      addCloneOfDeinitBlock(aFini, map, AS);

      break;
    }

    case TFI_TASK_PRIVATE:
      addDefAndMap(aInit, map, svar, createCurrTPV(svar));
      addCloneOfInitBlock(aInit, map, svar);
      addCloneOfDeinitBlock(aFini, map, svar);
      break;

    case TFI_IN_PARENT:         // handled upon TFI_IN
    case TFI_REDUCE_OP:         // handled upon TFI_REDUCE
    case TFI_REDUCE_PARENT_AS:
    case TFI_REDUCE_PARENT_OP:
      break;

    case TFI_DEFAULT:           // no abstract intents, please
    case TFI_CONST:
      INT_ASSERT(false);
      break;
  }
}

static void expandTaskFn(ExpandVisitor* EV, CallExpr* callToTFn, FnSymbol* taskFn)
{
  bool addErrorArgToCall = false;
  Expr *aInit=NULL, *aFini=NULL;

  // Follow the cloning steps in expandBodyForIteratorInline().
  // No need for taskFnCopies.

  // This holds because we flatten everything right away.
  // We need it so that we can place the def of 'fcopy' anywhere
  // while preserving correct scoping of its SymExprs.
  INT_ASSERT(isGlobal(taskFn));

  checkForallsInInitDeinitBlocks(EV->forall);

  FnSymbol* cloneTaskFn = taskFn->copy();

  if (!preserveInlinedLineNumbers)
    reset_ast_loc(cloneTaskFn, callToTFn);

  // Note that 'cloneTaskFn' will likely get a copy of forall loop body,
  // so we need to preserve correct scoping of its SymExprs.
  callToTFn->insertBefore(new DefExpr(cloneTaskFn));

  aInit = new CallExpr("aInit"); cloneTaskFn->insertAtHead(aInit);
  aFini = new CallExpr("aFini"); cloneTaskFn->insertIntoEpilogue(aFini);
  // Should aFini go into the epilogue or before epilogue?

  callToTFn->baseExpr->replace(new SymExpr(cloneTaskFn));

  SymbolMap& iMap = EV->svar2clonevar;  // incoming "current map"
  SymbolMap  map;                       // "current map" for cloneTaskFn body

  int numOrigActuals = callToTFn->numActuals();
  int ix = 0;

  for_shadow_vars(svar, temp, EV->forall)
    expandShadowVarTaskFn(cloneTaskFn, callToTFn, aInit, aFini,
                          numOrigActuals, iMap, map, svar, ++ix);

  aInit->remove();
  aFini->remove();

  ExpandVisitor taskFnVis(EV, map);

  // Traverse recursively.
  cloneTaskFn->body->accept(&taskFnVis);

  fixupErrorHandlingExits(cloneTaskFn->body, addErrorArgToCall);
  if (addErrorArgToCall)
    addDummyErrorArgumentToCall(callToTFn);

  // If we don't flatten it right away, we get non-global taskFns
  // in expandTaskFn(). That may cause issues with scoping.
  flattenNestedFunction(cloneTaskFn);
}

/////////// expandForall ///////////

static void expandShadowVarForall(ForallStmt* fs,
                                  SymbolMap& iMap, SymbolMap& map,
                                  ShadowVarSymbol* srcSV)
{
  SET_LINENO(srcSV);

  // copy() also performs map.put(srcSV, newSV)
  ShadowVarSymbol* newSV = srcSV->copy(&map);

  // Set newSV->outerVarSym.
  Symbol* newOvar = NULL;

  switch (newSV->intent) {
    case TFI_IN:
    case TFI_CONST_IN:
    case TFI_REF:
    case TFI_CONST_REF:
    case TFI_REDUCE:
      newOvar = srcSV; break;         // the current var just before 'fs'

    case TFI_IN_PARENT:
      newOvar = srcSV->INforParentvar(); break; // the current var for SI

    case TFI_REDUCE_PARENT_AS:
      newOvar = srcSV->AccumStateForParentAS(); break;  // current AS var

    case TFI_REDUCE_PARENT_OP:
      newOvar = srcSV->ReduceOpForParentRP(); break;    // current RP var

    case TFI_REDUCE_OP:
    case TFI_TASK_PRIVATE:
      break;                                              // no outer var

    case TFI_DEFAULT:
    case TFI_CONST:
      INT_ASSERT(false);                   // no abstract intents, please
      break;
  }

  if (newOvar != NULL)
    newSV->outerVarSE = new SymExpr(iMap.get(newOvar));
  else
    INT_ASSERT(newSV->outerVarSE == NULL);

  fs->shadowVariables().insertAtTail(new DefExpr(newSV));
}

static void expandForall(ExpandVisitor* EV, ForallStmt* fs)
{
  ForallStmt*     pfs  = EV->forall;
  SymbolMap&      iMap = EV->svar2clonevar; // incoming "current map"
  SymbolMap       map;                      // "current map" for fs body
  ExpandVisitor   forallVis(EV, map);

  for_shadow_vars(srcSV, temp, pfs)
    expandShadowVarForall(fs, iMap, map, srcSV);

  // Traverse recursively.
  fs->loopBody()->accept(&forallVis);
}


/////////// outermost visitor ///////////

static void expandShadowVarTopLevel(Expr* aInit, Expr* aFini, SymbolMap& map,
                                    ShadowVarSymbol* svar)
{
  SET_LINENO(svar);
  switch (svar->intent)
  {
    case TFI_IN:
    case TFI_CONST_IN:
    {
      ShadowVarSymbol* INP = svar->ParentvarForIN();
      map.put(INP, INP->outerVarSym());

      addDefAndMap(aInit, map, svar, createCurrIN(svar));
      addCloneOfInitBlock(aInit, map, svar);
      addCloneOfDeinitBlock(aFini, map, svar);

      break;
    }

    case TFI_REF:
    case TFI_CONST_REF:
      // Use the outer variable directly, for simplicity.
      // NB we are not concerned with const checking any more.
      map.put(svar, svar->outerVarSym());
      break;

    case TFI_REDUCE:
    {
      ShadowVarSymbol *PRP=0, *PAS=0, *RP=0, *AS=svar;
      setReduceSVars(PRP, PAS, RP, AS);

      map.put(RP, PRP->outerVarSym());
      map.put(AS, PAS->outerVarSym());
      // We do not need PRP,PAS in 'map' at the top level.

      break;
    }

    case TFI_TASK_PRIVATE:
      addDefAndMap(aInit, map, svar, createCurrTPV(svar));
      addCloneOfInitBlock(aInit, map, svar);
      addCloneOfDeinitBlock(aFini, map, svar);
      break;

    case TFI_IN_PARENT:         // handled upon TFI_IN
    case TFI_REDUCE_OP:         // handled upon TFI_REDUCE
    case TFI_REDUCE_PARENT_AS:
    case TFI_REDUCE_PARENT_OP:
      break;

    case TFI_DEFAULT:           // no abstract intents, please
    case TFI_CONST:
      INT_ASSERT(false);
      break;
  }
}

// 'ibody' is a clone of the parallel iterator body
// We are replacing the ForallStmt with this clone.
static void expandTopLevel(ExpandVisitor* outerVis,
                           BlockStmt* iwrap, BlockStmt* ibody)
{
  INT_ASSERT(ibody->inTree()); //fyi

  // Place initialization code before ibody, finalization code after ibody.
  Expr* aInit = ibody;
  Expr* aFini = ibody;

  // The initial "current map".
  SymbolMap& map = outerVis->svar2clonevar;

  for_shadow_vars(svar, temp, outerVis->forall)
    expandShadowVarTopLevel(aInit, aFini, map, svar);
}


/////////// reorder shadow variables ///////////

// "Proper" shadow variables i.e. not TPVs should go first,
// except those for reduce intents.
static bool shouldGoFirst(ShadowVarSymbol* sv) {
  switch (sv->intent)
  {
  case TFI_DEFAULT:
  case TFI_CONST:
  case TFI_IN_PARENT:
  case TFI_IN:
  case TFI_CONST_IN:
  case TFI_REF:
  case TFI_CONST_REF:
    return true;

  case TFI_REDUCE:
  case TFI_REDUCE_OP:
  case TFI_REDUCE_PARENT_AS:
  case TFI_REDUCE_PARENT_OP:
  case TFI_TASK_PRIVATE:
    return false;
  }

  return false; // dummy
}

//
// We want all proper shadow variables to be ahead of task-private variables.
// Explanation: (a) The latter may reference the former - in TPVs initBlocks.
// (b) We compute+apply SymbolMaps incrementally. (c) When we map a TPV,
// the proper shadow variables that it references must already be in the map.
//
// For example:
//   forall ... with (in n, var m = n) { ... }
// To initialize 'm' by cloning its initBlock, we need to have 'n'
// in the svar2clonevar map.
// By contrast, the only thing that 'n'-the-ShadowVarSymbol references
// is its outer variable, never another shadow- or task-private variable.
//
// We do not hoist reduce-intent shadow variables. This is because they
// should not reference proper shadow variables (right?). So mapping
// is not an issue. The issue is that we want to preserve the order of
// initialization of reduction shadow variables w.r.t. task-private variables
// - because initialization actions may have visible side-effects.
//
static void reorderShadowVsTaskPrivateVars(ForallStmt* fs) {
  AList& svars = fs->shadowVariables();
  if (svars.empty()) return; // nothing to reorder

  // If the following code proves hard to maintain, it may be easier to
  // reimplement it by introducing a temporary AList and moving SVs
  // that shouldGoFirst() from 'svars' to that temp list.

  Expr* currDef = svars.head;
  Expr* lastReorderedDef = NULL;

  do {
    Expr* nextDef = currDef->next;
    ShadowVarSymbol* currSV = toShadowVarSymbol(toDefExpr(currDef)->sym);

    if (shouldGoFirst(currSV)) {
      if (lastReorderedDef == NULL) {
        // This is the first time we are seeing a "shouldGoFirst" SV.
        if (currDef->prev != NULL)
          // We got some defs before, which are not "shouldGoFirst".
          svars.insertAtHead(currDef->remove());
        else
          ;// This def is the first one, nothing to reorder.
      }
      else {
        if (lastReorderedDef == currDef->prev)
          ; // Already properly ordered.
        else
          lastReorderedDef->insertAfter(currDef->remove());
      }
      // In any case, update the "last" pointer.
      lastReorderedDef = currDef;
    }
    else {
      // We have !shouldGoFirst(currSV) - nothing to do.
    }

    currDef = nextDef;
  } while (currDef != NULL);

  if (fVerify) {
    bool inSVs = true;
    for_shadow_vars(sv, temp, fs) {
      if (shouldGoFirst(sv)) INT_ASSERT(inSVs);
      else                   inSVs = false;
    }
  }
}


/////////// recursive iterators ///////////

//
// If 'fs' has only ref intents, or none at all,
// revert to old iterator-record-based implementation.
// Remove 'fs' and replace it with a ForLoop.
//
static void handleRecursiveIter(ForallStmt* fs,
                                FnSymbol* parIterFn,  CallExpr* parIterCall)
{
  SET_LINENO(parIterCall);

  // Check for non-ref intents.
  SymbolMap sv2ov;
  bool gotNonRefs = false;
  for_shadow_vars(svar, temp, fs) {
    if (svar->intent == TFI_REF || svar->intent == TFI_CONST_REF)
      sv2ov.put(svar, svar->outerVarSym());
    else
      { gotNonRefs = true; break; }
  }

  if (gotNonRefs) {
    USR_FATAL_CONT(fs, "forall loops over recursive parallel iterators are currently not implemented");
    USR_PRINT(fs, "in the presence of non-ref intents");
    USR_PRINT(parIterFn, "the parallel iterator is here");
    return;
  }

  // We assume these in the foregoing.
  INT_ASSERT(parIterCall == fs->firstIteratedExpr());
  INT_ASSERT(parIterCall && !parIterCall->next);

  BlockStmt* PARBlock = new BlockStmt();
  // Keep 'fs' in the tree for now, for debugging convenience.
  fs->insertAfter(PARBlock);

  // These were set in setupRecIterFields().
  DefExpr* iterRecDef  = fs->fRecIterIRdef;
  DefExpr* parIterDef  = fs->fRecIterICdef;
  Expr*   callGetIter  = fs->fRecIterGetIterator;
  Expr*   callFreeIter = fs->fRecIterFreeIterator;

  VarSymbol* iterRec   = toVarSymbol(iterRecDef->sym);
  VarSymbol* parIter   = toVarSymbol(parIterDef->sym);
  VarSymbol* parIdx    = parIdxVar(fs);
  DefExpr*   parIdxDef = parIdx->defPoint;

  // Just in case.
  INT_ASSERT(!strcmp(iterRec->name, "chpl__iterPAR"));
  INT_ASSERT(!strcmp(parIter->name, "chpl__parIter"));
  INT_ASSERT(parIdxDef == fs->inductionVariables().head);

  PARBlock->insertAtTail(iterRecDef->remove());
  PARBlock->insertAtTail(parIterDef->remove());
  PARBlock->insertAtTail(parIdxDef->remove());
  INT_ASSERT(toSymExpr(parIterCall->argList.tail)->symbol() == gDummyRef);
  parIterCall->argList.tail->replace(new SymExpr(iterRec)); // ret by ref
  PARBlock->insertAtTail(parIterCall->remove());
  PARBlock->insertAtTail(new CallExpr(PRIM_MOVE, parIter, callGetIter->remove()));

  ForLoop* PARBody = new ForLoop(parIdx, parIter, NULL,
                                 /* zippered */ false,
                                 /*forall*/ true,
                                 /*isForExpr*/ fs->isForallExpr());
  // not parIterCall, ex.
  //  library/standard/FileSystem/filerator/bradc/walk-par.chpl
  PARBody->astloc = fs->astloc;

  PARBlock->insertAtTail(PARBody);

  if (sv2ov.size() > 0)  // actually, the size of a Map is given by sv2ov.i
    update_symbols(fs->loopBody(), &sv2ov);

  // Transfer the loop body from fs to PARBody.
  while (Expr* stmt = fs->loopBody()->body.head)
    PARBody->insertAtTail(stmt->remove());

  // Todo: what to do with this in the presence of error handling?
  // Move callFreeIter and parIterDef outside of PARBlock?
  PARBlock->insertAtTail(callFreeIter->remove());

  fs->remove();
}


/////////// iterator forwarders ///////////

// Indicate whether the function is considered an iterator, NOT a forwarder.
// This used to admit only parallel iterators,
// before we started also allowing serial iterators in ForallStmt loops.
static bool isProperIterator(FnSymbol* fn) {
  return fn->isIterator();
}

// Remove the retArg temp and its initialization as the address of currSym.
static void cleanupRetArg(Symbol* retArg, Symbol* currSym) {
  retArg->defPoint->remove();

  // Ensure we are removing the set-ref of currSym.
  SymExpr* def = retArg->firstSymExpr();
  CallExpr* move = toCallExpr(def->parentExpr);
  INT_ASSERT(move && move->isPrimitive(PRIM_MOVE));
  CallExpr* setref = toCallExpr(move->get(2));
  INT_ASSERT(setref && setref->isPrimitive(PRIM_SET_REFERENCE));
  SymExpr* refee = toSymExpr(setref->get(1));
  INT_ASSERT(refee && refee->symbol() == currSym);
  move->remove();

  // Ensure no other uses.
  INT_ASSERT(retArg->firstSymExpr() == NULL);
}

// Remove the autoDestroy of 'currSym', if present.
static void removeAutoDestroyCallIfPresent(Symbol* currSym) {
  for_SymbolSymExprs(curSE, currSym)
    if (CallExpr* curCall = toCallExpr(curSE->parentExpr))
      if (FnSymbol* curCallFn = curCall->resolvedFunction())
        if (curCallFn->hasFlag(FLAG_AUTO_DESTROY_FN)) {
          curCall->remove();
          break;
        }
}

/*
Handle the case where 'currSym' in stripReturnScaffolding()
is defined by calling a retArg-ified function, for example:

  call _toLeader(ic,currSym)

If the callee looks like:

  proc _toLeader(ic, ref retArg) {
    doSomething;
    retArg = someExpr;
  }

Then replace the call _toLeader(ic,currSym) with:

  doSomething;
  currSym = someExpr;

then stripReturnScaffolding() can continue, using the same currSym.

*/
static Symbol* inlineRetArgFunction(CallExpr* defCall, FnSymbol* defFn,
                                    Symbol* currSym)
{
  BlockStmt* defBody = copyFnBodyForInlining(defCall, defFn,
                                             defCall->getStmtExpr());

  // Expect the last statement to be "return void".
  CallExpr* retexpr = toCallExpr(defBody->body.tail);
  INT_ASSERT(retexpr && retexpr->isPrimitive(PRIM_RETURN));
  INT_ASSERT(toSymExpr(retexpr->get(1))->symbol() == gVoid);

  // Expect the last statement before 'return' to assign into the retarg.
  CallExpr* prev      = toCallExpr(retexpr->prev);
  CallExpr* retAssign = prev;

  if (FnSymbol* fn = prev->resolvedFunction()) {
    // ... or an autoDestroy, which we need to remove.
    // The assignment, then, comes right before.
    INT_ASSERT(fn->hasFlag(FLAG_AUTO_DESTROY_FN));
    retAssign = toCallExpr(prev->prev);
    prev->remove();
  }    

  INT_ASSERT(retAssign && retAssign->isPrimitive(PRIM_ASSIGN));

  SET_LINENO(defCall);
  SymExpr* retArgSE = toSymExpr(retAssign->get(1));

  // Assign into currSym instead of the retarg.
  retArgSE->replace(new SymExpr(currSym));

  // Or - ask copyFnBodyForInlining() to assign directly into the actual,
  // avoiding the temp for retarg.
  cleanupRetArg(retArgSE->symbol(), currSym);
  retexpr->remove();

  // Otherwise how do we replace it?
  INT_ASSERT(defCall == defCall->getStmtExpr());
  defCall->replace(defBody);

  // Continue with the same symbol.
  return currSym;
}

//
// For 'block' the body of a function, return the single CallExpr*
// that computes the return value. Fail if it does not exist.
// Remove the return symbol and the temps that propagate this value
// into the return statement.
//
static CallExpr* stripReturnScaffolding(BlockStmt* block, Symbol* currSym) {
  currSym  = removeParIterReturn(block, currSym);

  while (true) {
    if (SymExpr* defSE = currSym->getSingleDef())
      if (CallExpr* defMove = toCallExpr(defSE->parentExpr)) {
        if (defMove->isPrimitive(PRIM_MOVE)  ||
            defMove->isPrimitive(PRIM_ASSIGN)) {
          INT_ASSERT(defSE == defMove->get(1));
          currSym->defPoint->remove();
          Expr* defSrc = defMove->get(2);

          if (SymExpr* srcSE = toSymExpr(defSrc)) {
            defMove->remove();
            // The autoCopy may be deleted in callDestructors, with autoDestroy
            // still around. Ex. test/functions/promotion/forallPromotes.chpl
            removeAutoDestroyCallIfPresent(currSym);
            INT_ASSERT(currSym->firstSymExpr() == NULL); // no other refs to it
            currSym = srcSE->symbol();
            continue;
          }
          if (CallExpr* srcCall = toCallExpr(defSrc)) {
            if (srcCall->resolvedFunction()->hasFlag(FLAG_AUTO_COPY_FN)) {
              // This should be unnecessary once we get rid of _toLeader fns.
              defMove->remove();
              currSym = toSymExpr(srcCall->get(1))->symbol();
              removeAutoDestroyCallIfPresent(currSym);
              INT_ASSERT(currSym->firstSymExpr() == NULL); // no other refs to it
              continue;
            }
            // Found it. Place it where our ForallStmt will go.
            defMove->replace(srcCall->remove());
            INT_ASSERT(currSym->firstSymExpr() == NULL); // no other refs to it
            return srcCall;
          }
        } else if (FnSymbol* defFn = defMove->resolvedFunction()) {
          if (defFn->hasFlag(FLAG_FN_RETARG)) {
            if (isProperIterator(defFn)) {
              // 'defMove' is the iterator call that we want.
              return defMove;
            } else {
              currSym = inlineRetArgFunction(defMove, defFn, currSym);
              continue;
            }
          }
        }
      }

    // The AST is beyond our expectations. Bail out.
    USR_FATAL(currSym, "only simple control flow is allowed in a procedure that returns an iterator for use in a forall loop");
    break;
  }

  return NULL; //dummy
}

//
// If the iterable expression calls something that is NOT a proper iterator,
// pre-process it, then point 'iterCall' and 'iterFn' to the new callee.
//
// Tests:
//   library/packages/Collection/CollectionCounter.chpl
//   library/standard/Random/deitz/test1D2D.chpl
//   reductions/deitz/test_maxloc_reduce_wmikanik_bug2.chpl
//
static void handleIteratorForwarders(ForallStmt* fs,
                                     CallExpr*& iterCall, FnSymbol*& iterFn)
{
 do {  // Repeat this until the new iterFn is a proper iterator.

  // These should have been replaced away in convertIteratorForLoopexpr().
  INT_ASSERT(iterFn->hasFlag(FLAG_FN_RETURNS_ITERATOR));

  // Handle a return by reference. Ex.:
  //  distributions/bradc/assoc/userAssoc-domain-stress
  Symbol* retRefSym = NULL;
  if (iterFn->hasFlag(FLAG_FN_RETARG)) {
    SymExpr* retRefSE     = toSymExpr(iterCall->argList.tail);
    Symbol*  retRefFormal = toDefExpr(iterFn->formals.tail)->sym;
    INT_ASSERT(retRefSE->symbol() == gDummyRef);
    INT_ASSERT(retRefFormal->hasFlag(FLAG_RETARG));
    SET_LINENO(retRefSE);
    retRefSym = newTemp("retRef", retRefFormal->type);
    retRefSym->qual = QUAL_REF;
    retRefSE->replace(new SymExpr(retRefSym));
  }

  // Inline the forwarder, i.e. 'iterFn', like so:
  //
  //   forall x in iter1() do BODY;
  //   proc iter1() { doSomething; return iter2(); }
  //
  //  ==>
  //
  //   doSomething;
  //   forall x in iter2() do BODY;
  //
  // Correspondingly, update:
  //   iterFn   <- iter2
  //   iterCall <- the call iter2() in the forall

  BlockStmt* fBody = copyFnBodyForInlining(iterCall, iterFn, fs);
  fs->replace(fBody);

  CallExpr* forwardee = stripReturnScaffolding(fBody, retRefSym);

  forwardee->replace(fs);
  iterCall->replace(forwardee);

  iterCall = forwardee;
  iterFn   = iterCall->resolvedFunction();

 } while (! isProperIterator(iterFn));
}


/////////// handle one ForallStmt ///////////

static void lowerOneForallStmt(ForallStmt* fs) {
  if (fs->id == breakOnResolveID) gdbShouldBreakHere();

  // If this fails, need to filter out those FSes.
  INT_ASSERT(fs->inTree() && fs->getFunction()->isResolved());

  // We convert zippering, if any, to a follower loop during resolution.
  INT_ASSERT(fs->numIteratedExprs() == 1);

  CallExpr* parIterCall = toCallExpr(fs->firstIteratedExpr());
  FnSymbol* parIterFn = parIterCall->resolvedFunction();

  if (isVirtualIterator(parIterFn)) {
    USR_FATAL_CONT(fs, "virtual parallel iterators are not yet supported (see issue #6998)");
    return;
  }

  // Make sure it is a parallel iterator, not a forwarder.
  if (! isProperIterator(parIterFn))
    // This updates parIterCall, parIterFn.
    handleIteratorForwarders(fs, parIterCall, parIterFn);

  INT_ASSERT(isProperIterator(parIterFn));

  if (parIterFn->hasFlag(FLAG_RECURSIVE_ITERATOR)) {
    handleRecursiveIter(fs, parIterFn, parIterCall);

    // It is probably OK to lower other foralls even if we can't this one.
    return;
  }

  reorderShadowVsTaskPrivateVars(fs);

  // Place to put pre- and post- code.
  SET_LINENO(fs);
  CallExpr*  ianch = new CallExpr("anchor");
  BlockStmt* iwrap = new BlockStmt(ianch);

  // Clone the iterator body.
  // Cf. expandIteratorInline() and inlineCall().
  BlockStmt* ibody = copyFnBodyForInlining(parIterCall, parIterFn, ianch);
  removeVoidReturn(ibody);

  // Let us remove 'fs' later, for debugging convenience.
  fs->insertAfter(iwrap);
  ianch->replace(ibody);

  SymbolMap       map;
  ExpandVisitor   outerVis(fs, map);
  expandTopLevel(&outerVis, iwrap, ibody);

  // Traverse recursively.
  ibody->accept(&outerVis);

  fs->remove();
  // We could also do {iwrap,ibody}->flattenAndRemove().

  if (parIterFn->firstSymExpr() == NULL)
    // We have inlined all uses. So, remove the iterator as well.
    parIterFn->defPoint->remove();
}


/////////// main driver ///////////

static void removeDeadIters() {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (!fn->inTree()) continue;

    if (fn->firstSymExpr() == NULL)
    {
      if (fn->hasFlag(FLAG_INLINE_ITERATOR) || isTaskFun(fn))
        // Got a parallel iterator or task function with no uses. Remove.
        // TODO: do the same for non-parallel iterators.
        fn->defPoint->remove();
    }
    else
      INT_ASSERT(isGlobal(fn)); // We flatten all task functions right away.
  }
}

void lowerForallStmtsInline()
{
  forv_Vec(ForallStmt, fs, gForallStmts)
    if (fs->inTree())
      lowerOneForallStmt(fs);

  USR_STOP();

  removeDeadIters();

  // Ensure gDummyRef is no longer used.
  INT_ASSERT(gDummyRef->firstSymExpr() == NULL);
}
