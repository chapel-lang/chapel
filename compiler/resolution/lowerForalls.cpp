/*
 * Copyright 2004-2018 Cray Inc.
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
#include "ForallStmt.h"
#include "implementForallIntents.h"
#include "passes.h"
#include "resolution.h"
#include "stringutil.h"
#include "wellknown.h"

/*

This file provides parts of the implementation of forall loops
and forall intents, specifically:

* Setting up shadow variables and other things - during resolution.
This part 

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
  While cloning, The shadow variables in the loop body are replaced
  with "current variables" that are in effect for that yield.
  The shadow variables themselves do not remain in the AST.

* The inlined body of the parallel iterator is traversed using ExpandVisitor.
  "Current variables" are created and additional code is inserted
  in certain key places.

  The SymbolMap in ExpandVisitor::svar2clonevar records
  the current variables to be used when a yield statement
  or a ForallStmt is encountered.

* The key places during the tranversal are:

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

TFI_IN_OUTERVAR
  nothing - it supports IN intents

TFI_IN, TFI_CONST_IN
  def currentVar
  currentVar.init(outerVarSym() of the corresponding TFI_IN_OUTERVAR)
  svar2clonevar: SV --> currentVar

(end)
  currentVar.deinit() // for records

TFI_REF, TFI_CONST_REF
  // use the outer var directly
  svar2clonevar: SV --> SV->outerVarSym()

TFI_REDUCE_OP
  // use the reduce op already set up for us
  svar2clonevar: SV --> SV->outerVarSym()

TFI_REDUCE
  def currentVar
  currentVar.init((the corresponding TFI_REDUCE_OP).identity)
  svar2clonevar: SV --> currentVar

(end)
  (the corresponding TFI_REDUCE_OP).accumulate(currentVar)
  currentVar.deinit() // for records

TFI_TASK_PRIVATE
  TODO

*** call to task function ***

(actual) an actual is added to the call
and a formal is added to the task function

(in task fn) actions performed there
  includes setting up a new svar2clonevar map
  in effect during the traversal of the task fn, except as indicated

(end) deinit actions at the end of the task function

TFI_IN_OUTERVAR
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
  TODO

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

* Not all forall-like things in Chapel code get transformed
into ForallStmt nodes. Some forall intents are implemented
in implementForallIntents1(), implementForallIntents2().
Todo: switch those to this "modern" implementation.

* When a global is passed by [const] ref intent,
replace references to the corresponding shadow variable
in loop body with the global itself.
Note: be aware that, in the loop body, the compiler adds derefs
from such a shadow variable - because it is a "ref".

*/


///////////                                       ///////////
/////////// Set up shadow variables at resolve(). ///////////
///////////                                       ///////////


/////////// create helper SVars ///////////

static ShadowVarSymbol* create_IN_OUTERVAR(ForallStmt* fs, ShadowVarSymbol* SI)
{
  ShadowVarSymbol* SO = new ShadowVarSymbol(TFI_IN_OUTERVAR,
                                            astr("SO_", SI->name), NULL);
  SO->addFlag(FLAG_CONST);  // make it be like 'const in'
  SO->qual = QUAL_CONST_VAL;
  SO->type = SI->type;

  // It goes on the shadow variable list right before SI.
  SI->defPoint->insertBefore(new DefExpr(SO));
  INT_ASSERT(SI->OutervarForIN() == SO);  // ensure OutervarForIN() works

  return SO;
}

static ShadowVarSymbol* create_REDUCE_OP(ForallStmt* fs, ShadowVarSymbol* AS)
{
  SymExpr* gOpSE = toSymExpr(AS->reduceOpExpr()->remove());
  Symbol*  gOp   = gOpSE->symbol();
  // Handling of the case of gOp being a type should have happened earlier.
  INT_ASSERT(!gOp->hasFlag(FLAG_TYPE_VARIABLE));

  ShadowVarSymbol* RP = new ShadowVarSymbol(TFI_REDUCE_OP,
                                            astr("RP_", AS->name), gOpSE);

  // It always points to the same reduction op class instance.
  RP->addFlag(FLAG_CONST);
  RP->qual = QUAL_CONST_VAL;
  RP->type = gOp->type;

  // It goes on the shadow variable list right before AS.
  AS->defPoint->insertBefore(new DefExpr(RP));
  INT_ASSERT(AS->ReduceOpForAccumState() == RP);  // ensure ReduceOpForAccumState() works

  return RP;
}

/////////// set up one SVar ///////////

static void insertInitialization(BlockStmt* destBlock,
                                 Symbol* destVar, Symbol* srcVar) {
  destBlock->insertAtTail("'init var'(%S,%S)", destVar, srcVar);
}
static void insertInitialization(BlockStmt* destBlock,
                                 Symbol* destVar, Expr* srcExpr) {
  VarSymbol* initTemp = new VarSymbol("initTemp");
  destBlock->insertAtTail(new DefExpr(initTemp));
  destBlock->insertAtTail(new CallExpr(PRIM_MOVE, initTemp, srcExpr));
  insertInitialization(destBlock, destVar, initTemp);
}
static void insertDeinitialization(BlockStmt* destBlock,
                                   Symbol* destVar) {
  // NB if we use PRIM_CALL_DESTRUCTOR, we end up with
  // deinit() calls for shadow variables of class types.
  destBlock->insertAtTail("chpl__autoDestroy(%S)", destVar);
}

static void setupForIN(ForallStmt* fs, ShadowVarSymbol* SI, Symbol* dum,
                       BlockStmt* IB, BlockStmt* DB) {
  INT_ASSERT(!SI->isRef());

  ShadowVarSymbol* SO = create_IN_OUTERVAR(fs, SI);

  insertInitialization(IB, SI, SO);
  insertDeinitialization(DB, SI);
}

static void setupForREF(ForallStmt* fs, ShadowVarSymbol* SR, Symbol* gR,
                        BlockStmt* IB, BlockStmt* DB) {
}  // nothing for a REF intent

// Set up the SVar for the ReduceOp class.
static void setupForReduce_OP(ForallStmt* fs, ShadowVarSymbol* RP, Symbol* gOp,
                              BlockStmt* IB, BlockStmt* DB) {
  IB->insertAtTail("'move'(%S, clone(%S,%S))", // initialization
                   RP, gMethodToken, gOp);

  DB->insertAtTail("chpl__reduceCombine(%S,%S)", gOp, RP);
  DB->insertAtTail("chpl__cleanupLocalOp(%S,%S)", gOp, RP); // deletes RP
}

// Set up the SVar for the Accumulation State.
static void setupForReduce_AS(ForallStmt* fs, ShadowVarSymbol* AS, Symbol* dum,
                              BlockStmt* IB, BlockStmt* DB) {
  ShadowVarSymbol* RP = AS->ReduceOpForAccumState();
  insertInitialization(IB, AS, new_Expr("identity(%S,%S)", gMethodToken, RP));

  DB->insertAtTail("accumulate(%S,%S,%S)", gMethodToken, RP, AS);
  insertDeinitialization(DB, AS);
}

static void setupForReduce(ForallStmt* fs, ShadowVarSymbol* AS, Symbol* AS_ovar,
                           BlockStmt* IB, BlockStmt* DB) {
  ShadowVarSymbol* RP = create_REDUCE_OP(fs, AS);
  setupForReduce_OP(fs, RP, RP->outerVarSym(),
                    RP->initBlock(), RP->deinitBlock());
  setupForReduce_AS(fs, AS, AS_ovar, IB, DB);
}

/////////// driver function ///////////

//
// Set up shadow variables during resolution:
// * Create helper shadow variables: TFI_REDUCE_OP, TFI_IN_OUTERVAR.
// * Populate task startup/teardown blocks.
//
// Note that this is done during resolveForallHeader,
// i.e. before resolving the forall loop body.
//
void setupShadowVariables(ForallStmt* fs)
{
  for_shadow_vars(svar, temp, fs)
  {
    SET_LINENO(svar);
    Symbol* ovar = svar->outerVarSym();
    BlockStmt* IB = svar->initBlock();
    BlockStmt* DB = svar->deinitBlock();
    
    switch (svar->intent)
    {
      case TFI_IN:
      case TFI_CONST_IN:     setupForIN(fs, svar, ovar, IB, DB);      break;

      case TFI_REF:
      case TFI_CONST_REF:    setupForREF(fs, svar, ovar, IB, DB);     break;

      case TFI_REDUCE:       setupForReduce(fs, svar, ovar, IB, DB);  break;

      case TFI_TASK_PRIVATE: INT_ASSERT(false); break; // TODO

      // We place such svars earlier in the list.
      // They should not come up here.
      case TFI_IN_OUTERVAR:
      case TFI_REDUCE_OP:    INT_ASSERT(false);  break;

      // No abstract intents, please.
      case TFI_DEFAULT:
      case TFI_CONST:        INT_ASSERT(false);  break;
    }
  }
}


/////////// Resolve shadow variables at resolve(). ///////////

//
// Resolve svars' IB, DB after we have set them up above.
//
static void resolveShadowVarsForForallStmt(ForallStmt* fs)
{
  for_shadow_vars(svar, temp, fs) {
    resolveBlockStmt(svar->initBlock());
    resolveBlockStmt(svar->deinitBlock());
  }
}

//
// Resolve shadow variables of a ForallStmt.
//
// Do this only if 'def' is the ForallStmt's last shadow variable def.
// This complication is due to the resolution order guided by getNextExpr().
//
void resolveShadowVarsIfNeeded(DefExpr* def)
{
  if (AList* list = def->list)
    if (ForallStmt* fs = toForallStmt(list->parent))
      if (list == &fs->shadowVariables())
        if (def == list->tail)
          resolveShadowVarsForForallStmt(fs);
}


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

  ExpandVisitor(ForallStmt* fs, SymbolMap& map);

  ExpandVisitor(ExpandVisitor* parentEV, SymbolMap& map);

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
    expandForall(this, node);
    // expandForall() takes care of descending into 'node'
    return false;
  }
};

// constructor for the outer level
ExpandVisitor::ExpandVisitor(ForallStmt* fs, SymbolMap& map) :
  forall(fs),
  svar2clonevar(map)
{
}

// constructor for a nested situation
ExpandVisitor::ExpandVisitor(ExpandVisitor* parentEV, SymbolMap& map) :
  forall(parentEV->forall),
  svar2clonevar(map)
{
}


/////////// misc helpers ///////////

// Remove the return statement and the def of 'ret'. Return 'ret'.
// See also removeRetSymbolAndUses().
static Symbol* removeParIterReturn(BlockStmt* cloneBody, bool moreRefs) {
  CallExpr* retexpr = toCallExpr(cloneBody->body.tail);
  INT_ASSERT(retexpr && retexpr->isPrimitive(PRIM_RETURN));
  Symbol* retsym = toSymExpr(retexpr->get(1))->symbol();
  INT_ASSERT(retsym->type->symbol->hasFlag(FLAG_ITERATOR_RECORD));
  
  retexpr->remove();
  if (!moreRefs) retsym->defPoint->remove();
  // There should not be any references left to 'ret', unless moreRefs.
  INT_ASSERT(moreRefs || retsym->firstSymExpr() == NULL);

  return retsym;
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
  return currSI;
}

// ... for the ReduceOp class of a reduce intent
static VarSymbol* createCurrROp(ShadowVarSymbol* RP) {
  VarSymbol* currRP = new VarSymbol(astr("RP_", RP->name), RP->type);
  currRP->qual = QUAL_CONST_VAL;
  return currRP;
}

// ... for the Accumulation State of a reduce intent
static VarSymbol* createCurrAS(ShadowVarSymbol* AS) {
  VarSymbol* currAS = new VarSymbol(astr("AS_", AS->name), AS->type);
  currAS->qual = QUAL_VAL;
  return currAS;
 }

static void addDefAndMap(Expr* aInit, SymbolMap& map, ShadowVarSymbol* svar,
                         VarSymbol* currVar)
{
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
// as PODs, with argument passing implemented as memcopy,
//
// So, we need to mimick that here. Otherwise we miss copy-construction
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
  map.put(SI->OutervarForIN(), eFormal);

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

  ArgSymbol* eFormal = newExtraFormal(svar, ix, eActual, /*nested:*/false);
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
    case TFI_IN_OUTERVAR: // helper svar - nothing to do
      break;

    case TFI_IN:          // in intents
    case TFI_CONST_IN:
      addArgAndMap(cloneTaskFn, callToTFn, numOrigAct, iMap,
                   map, svar, ix);
      addFormalTempSIifNeeded(cloneTaskFn, aInit, map, svar);
      addCloneOfDeinitBlock(aFini, map, svar);
      break;

    case TFI_REF:         // ref intents
    case TFI_CONST_REF:
      addArgAndMap(cloneTaskFn, callToTFn, numOrigAct, iMap,
                   map, svar, ix);
      // no init/deinit
      break;

    case TFI_REDUCE_OP:   // reduction op class
      addArgAndMap(cloneTaskFn, callToTFn, numOrigAct, iMap,
                   map, svar, ix, svar->outerVarSym());
      addDefAndMap(aInit, map, svar, createCurrROp(svar));
      addCloneOfInitBlock(aInit, map, svar);
      addCloneOfDeinitBlock(aFini, map, svar);
      break;

    case TFI_REDUCE:      // accumulation state
      addDefAndMap(aInit, map, svar, createCurrAS(svar));
      addCloneOfInitBlock(aInit, map, svar);
      addCloneOfDeinitBlock(aFini, map, svar);
      break;

    case TFI_TASK_PRIVATE:
      INT_ASSERT(false); // TODO
      break;

    case TFI_DEFAULT:    // no abstract intents, please
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

static void expandForall(ExpandVisitor* EV, ForallStmt* fs)
{
  ForallStmt*     pfs  = EV->forall;
  SymbolMap&      iMap = EV->svar2clonevar; // incoming "current map"
  SymbolMap       map;                      // "current map" for fs body
  ExpandVisitor   forallVis(EV, map);

  for_shadow_vars(srcSV, temp, pfs)
  {
    SET_LINENO(srcSV);

    // Redirect the original outer vars to their current counterparts.
    if (Symbol* srcOVar = srcSV->outerVarSym())
      map.put(srcOVar, iMap.get(srcSV));

    // copy() also performs map.put(srcSV, newSV)
    ShadowVarSymbol* newSV = srcSV->copy(&map);

    fs->shadowVariables().insertAtTail(new DefExpr(newSV));
  }

  // Traverse recursively.
  fs->loopBody()->accept(&forallVis);
}


/////////// outermost visitor ///////////

static void expandShadowVarTopLevel(Expr* aInit, Expr* aFini, SymbolMap& map, ShadowVarSymbol* svar) {
  SET_LINENO(svar);
  switch (svar->intent)
  {
    case TFI_IN_OUTERVAR:
      // The outer var for IB of the corresponding in-intent svar.
      map.put(svar, svar->INforOutervar()->outerVarSym());
      break;

    case TFI_IN:
    case TFI_CONST_IN:
      addDefAndMap(aInit, map, svar, createCurrIN(svar));
      addCloneOfInitBlock(aInit, map, svar);
      addCloneOfDeinitBlock(aFini, map, svar);
      break;

    case TFI_REF:
    case TFI_CONST_REF:
      // Let us reference the outer variable directly, for simplicity.
      // NB we are not concerned with const checking any more.
      map.put(svar, svar->outerVarSym());
      // no code to add
      break;

    case TFI_REDUCE_OP:
      // Let us use the global op directly.
      map.put(svar, svar->outerVarSym());
      break;

    case TFI_REDUCE:
      addDefAndMap(aInit, map, svar, createCurrAS(svar));
      addCloneOfInitBlock(aInit, map, svar);
      addCloneOfDeinitBlock(aFini, map, svar);
      break;

    case TFI_TASK_PRIVATE:
      INT_ASSERT(false); // TODO
      break;

    // No abstract intents, please.
    case TFI_DEFAULT:
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


/////////// recursive iterators ///////////

//
// If 'fs' has only ref intents, or none at all,
// revert to old iterator-record-based implementation.
// If so, do what the original buildStandaloneForallLoopStmt()
// did during parsing, with modifications.
//
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
  PARBlock->insertAtTail(new CallExpr(PRIM_MOVE, iterRec, parIterCall->remove()));
  PARBlock->insertAtTail(new CallExpr(PRIM_MOVE, parIter, callGetIter->remove()));

  ForLoop* PARBody = new ForLoop(parIdx, parIter, NULL, /* zippered */ false, /*forall*/ true);
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

//
// For 'block' the body of a function, return the single CallExpr*
// that computes the return value. Fail if it does not exist.
// Remove the return symbol and the temps that propagate this value
// into the return statement.
//
static CallExpr* stripReturnScaffolding(BlockStmt* block) {
  Symbol* currSym  = removeParIterReturn(block, true); // 'ret'

  while (true) {
    if (SymExpr* defSE = currSym->getSingleDef())
      if (CallExpr* defMove = toCallExpr(defSE->parentExpr))
        if (defMove->isPrimitive(PRIM_MOVE)) {
          currSym->defPoint->remove();
          Expr* defSrc = defMove->get(2);

          if (SymExpr* srcSE = toSymExpr(defSrc)) {
            defMove->remove();
            INT_ASSERT(currSym->firstSymExpr() == NULL); // no other refs to it
            currSym = srcSE->symbol();
            continue;
          }
          if (CallExpr* srcCall = toCallExpr(defSrc)) {
            // Found it. Place it where our ForallStmt will go.
            defMove->replace(srcCall->remove());
            INT_ASSERT(currSym->firstSymExpr() == NULL); // no other refs to it
            return srcCall;
          }
        }

    // The AST is beyond our expectations. Bail out.
    USR_FATAL(currSym, "only simple control flow is allowed in a procedure that returns an iterator for use in a forall loop");
    break;
  }

  return NULL; //dummy
}

//
// If the iterable expression calls something that is NOT
// a parallel iterator proper, pre-process it and update
// 'iterCall' and 'iterFn' to be the new iterable expression.
//
// Tests:
//   library/packages/Collection/CollectionCounter.chpl
//   library/standard/Random/deitz/test1D2D.chpl
//   reductions/deitz/test_maxloc_reduce_wmikanik_bug2.chpl
//
static void handleIteratorForwarders(ForallStmt* fs,
                                     CallExpr*& iterCall, FnSymbol*& iterFn)
{
  // These should have been replaced away in convertIteratorForLoopexpr().
  INT_ASSERT(strncmp(iterFn->name, "_iterator_for_loopexpr", 22));

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

  CallExpr* forwardee = stripReturnScaffolding(fBody);

  forwardee->replace(fs);
  iterCall->replace(forwardee);

  iterCall = forwardee;
  iterFn   = iterCall->resolvedFunction();

  // Todo: handle the case where the new 'iterFn' is yet
  // another forwarder. If so, repeat the same steps
  // until we reach an iterFn that is a parallel iterator.
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

  if (isVirtualIterator(parIterFn->retType->symbol)) {
    USR_FATAL_CONT(fs, "virtual parallel iterators are not yet supported (see issue #6998)");
    return;
  }

  // Make sure it is a parallel iterator, not a forwarder.
  if (!parIterFn->hasFlag(FLAG_INLINE_ITERATOR))
    // This updates parIterCall, parIterFn.
    handleIteratorForwarders(fs, parIterCall, parIterFn);

  INT_ASSERT(parIterFn->hasFlag(FLAG_INLINE_ITERATOR));

  if (parIterFn->hasFlag(FLAG_RECURSIVE_ITERATOR)) {
    handleRecursiveIter(fs, parIterFn, parIterCall);

    // It is probably OK to lower other foralls even if we can't this one.
    return;
  }

  // Place to put pre- and post- code.
  SET_LINENO(fs);
  CallExpr*  ianch = new CallExpr("anchor");
  BlockStmt* iwrap = new BlockStmt(ianch);

  // Clone the iterator body.
  // Cf. expandIteratorInline() and inlineCall().
  BlockStmt* ibody = copyFnBodyForInlining(parIterCall, parIterFn, ianch);
  removeParIterReturn(ibody, false);

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
}
