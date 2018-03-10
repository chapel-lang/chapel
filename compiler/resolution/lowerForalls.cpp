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
#include "view.h" //wass
#include "wellknown.h"

static bool verb = false; //wass
static int breakOnExpand = false; //wass

/*
At this point we are mostly not concerned about const-ness,
so we may be blurring const and non-const intents.
We DO need to preserve some const properties to enable optimizations.
*/

typedef Map<FnSymbol*,FnSymbol*> TaskFnCopyMap; //wass need this?


/////////// lowerForallIntentsAtResolution : RP for AS ///////////

//wass
#define lfiResolve 0

static ShadowVarSymbol* createSOforSI(ForallStmt* fs, ShadowVarSymbol* SI)
{
  ShadowVarSymbol* SO = new ShadowVarSymbol(TFI_IN_OVAR,
                                            astr("SO_", SI->name), NULL);
  SO->addFlag(FLAG_CONST);  // make it be like 'const in'
  SO->qual = QUAL_CONST_VAL;
  SO->type = SI->type;

  // It goes on the shadow variable list right before SI.
  SI->defPoint->insertBefore(new DefExpr(SO));
  INT_ASSERT(SI->SOforSI() == SO);  // ensure SOforSI() works

  return SO;
}

static ShadowVarSymbol* createRPforAS(ForallStmt* fs, ShadowVarSymbol* AS)
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
  INT_ASSERT(AS->RPforAS() == RP);  // ensure RPforAS() works

  return RP;
}


/////////// lowerForallIntentsAtResolution : setup IB, DB ///////////

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
//  if (!strncmp(destVar->type->symbol->name, "_array(", 7)) gdbShouldBreakHere(); //wass
// wass: or do it in walkForallBlocks()
/* wass was:
  if (FnSymbol* autoDestroy = getAutoDestroy(destVar->type))
    destBlock->insertAtTail(new CallExpr(autoDestroy, destVar));
*/
  // NB if we use PRIM_CALL_DESTRUCTOR, we end up with
  // deinit() calls for shadow variables of class types.
  destBlock->insertAtTail("chpl__autoDestroy(%S)", destVar);
}

static void setupForIN(ForallStmt* fs, ShadowVarSymbol* SI, Symbol* SO,
                       BlockStmt* IB, BlockStmt* DB) {
  INT_ASSERT(!SI->isRef());

  insertInitialization(IB, SI, SO);
#if lfiResolve
  resolveBlockStmt(IB);
#endif

  insertDeinitialization(DB, SI);
#if lfiResolve
  // no need to resolve the deinit
#endif
}

static void setupForREF(ForallStmt* fs, ShadowVarSymbol* SR, Symbol* gR,
                        BlockStmt* IB, BlockStmt* DB) {
}  // nothing for a REF intent

static void setupForR_OP(ForallStmt* fs, ShadowVarSymbol* RP, Symbol* gOp,
                         BlockStmt* IB, BlockStmt* DB) {
  IB->insertAtTail("'move'(%S, clone(%S,%S))", // initialization
                   RP, gMethodToken, gOp);
#if lfiResolve
  resolveBlockStmt(IB);
#endif

  DB->insertAtTail("chpl__reduceCombine(%S,%S)", gOp, RP);
  DB->insertAtTail("chpl__cleanupLocalOp(%S,%S)", gOp, RP); // deletes RP
#if lfiResolve
  resolveBlockStmt(DB);
#endif
}

static void setupForR_AS(ForallStmt* fs, ShadowVarSymbol* AS, Symbol* ignored,
                         BlockStmt* IB, BlockStmt* DB) {
  ShadowVarSymbol* RP = AS->RPforAS();
  insertInitialization(IB, AS, new_Expr("identity(%S,%S)", gMethodToken, RP));
#if lfiResolve
  resolveBlockStmt(IB);
#endif

  DB->insertAtTail("accumulate(%S,%S,%S)", gMethodToken, RP, AS);
#if lfiResolve
  resolveBlockStmt(DB);
#endif
  insertDeinitialization(DB, AS);
#if lfiResolve
  // no need to resolve the deinit
#endif
}

// This should be static. Making it extern for now while it is unused.
void setupForTPV(ForallStmt* fs, ShadowVarSymbol* PV, Symbol* ignored,
                        BlockStmt* IB, BlockStmt* DB);
void setupForTPV(ForallStmt* fs, ShadowVarSymbol* PV, Symbol* ignored,
                        BlockStmt* IB, BlockStmt* DB) {
  // IB already comes from PV's declaration in the with-clause.
#if lfiResolve
  resolveBlockStmt(IB);
#endif

  insertDeinitialization(DB, PV);
#if lfiResolve
  // no need to resolve the deinit
#endif
}


/////////// lowerForallIntentsAtResolution ///////////

/*
wass update this comment
Set up shadow variables and task startup/teardown blocks during resolution.
* Create TFI_REDUCE_OP shadow variables.
* Populate task and top startup/teardown blocks.
* Resolve all the operations and the types of the shadow variables.

Note that this is done during resolveForallHeader,
i.e. before resolving the forall loop body.
*/

// Creates TFI_REDUCE_OP svars. Resolves TFI_REDUCE and TFI_REDUCE_OP svar types.
// Invoked from resolveForallHeader().
void lowerForallIntentsAtResolution(ForallStmt* fs); //wass to header
void lowerForallIntentsAtResolution(ForallStmt* fs) {
  for_shadow_vars(svar, temp, fs)
  {
    SET_LINENO(svar);
    Symbol* ovar = svar->outerVarSym();
    BlockStmt* IB = svar->initBlock();
    BlockStmt* DB = svar->deinitBlock();
    
    switch (svar->intent)
    {
      case TFI_IN:
      case TFI_CONST_IN: { ShadowVarSymbol* SO = createSOforSI(fs, svar);
                           setupForIN(fs, svar, SO, IB, DB);      break; }

      case TFI_REF:
      case TFI_CONST_REF:  setupForREF(fs, svar, ovar, IB, DB);   break;

      case TFI_REDUCE: {   ShadowVarSymbol* RP = createRPforAS(fs, svar);
                           setupForR_OP(fs, RP, RP->outerVarSym(),
                                        RP->initBlock(), RP->deinitBlock());
                           setupForR_AS(fs, svar, ovar, IB, DB);  break; }

      // We placed such svar earlier in the list - it should not come up here.
      case TFI_IN_OVAR:
      case TFI_REDUCE_OP:  INT_ASSERT(false);                     break;

      // No abstract intents, please.
      case TFI_DEFAULT:
      case TFI_CONST:      INT_ASSERT(false);                     break;
    }
  }
}

#if !lfiResolve
// Also need to resolve IB, DB after we have set them up above.
void resolveShadowVarsIfNeeded(DefExpr* def); //wass to .h
void resolveShadowVarsIfNeeded(DefExpr* def) {
  ForallStmt* efs = NULL;
  if (AList* list = def->list)
    if (ForallStmt* fs = toForallStmt(list->parent))
      if (list == &fs->shadowVariables())
        if (def == list->tail)
          efs = fs;
  if (efs == NULL) return;

  for_shadow_vars(svar, temp, efs) {
    resolveBlockStmt(svar->initBlock());
    resolveBlockStmt(svar->deinitBlock());
  }
}
#endif


/////////// forwards ///////////

class ExpandVisitor;
static void expandYield( ExpandVisitor* EV, CallExpr* yield);
static void expandTaskFn(ExpandVisitor* EV, CallExpr* call, FnSymbol* taskFn);
static void expandForall(ExpandVisitor* EV, ForallStmt* fs);


/////////// ExpandVisitor visitor ///////////

class ExpandVisitor : public AstVisitorTraverse {
public:
  ForallStmt* const forall;
  FnSymbol* const parIter; // wass needed?
  SymbolMap& svar2clonevar;
  TaskFnCopyMap& taskFnCopies;  // like in expandBodyForIteratorInline()
  ExpandVisitor* parentVis;
  bool breakOnYield; // wass for debugging
  // wass we may want to stash parIdxVar(forall) in a field

  ExpandVisitor(ForallStmt* fs, FnSymbol* parIterArg,
                SymbolMap& map, TaskFnCopyMap& taskFnCopiesArg);

  ExpandVisitor(ExpandVisitor* parentEV,
                SymbolMap& map);

  virtual bool enterCallExpr(CallExpr* node) {
    if (node->isPrimitive(PRIM_YIELD)) {
      expandYield(this, node);
    }
    else if (FnSymbol* calledFn = node->resolvedFunction()) {
      if (isTaskFun(calledFn)) {
        expandTaskFn(this, node, calledFn);
      } else if (calledFn == gChplPropagateError) {
        void handleChplPropagateErrorCall(CallExpr* call); //wass - in .h
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
ExpandVisitor::ExpandVisitor(ForallStmt* fs, FnSymbol* parIterFn,
                             SymbolMap& map, TaskFnCopyMap& taskFnCps) :
  forall(fs),
  parIter(parIterFn),
  svar2clonevar(map),
  taskFnCopies(taskFnCps),
  parentVis(NULL),
  breakOnYield(false)
{
}

// constructor for a nested situation
ExpandVisitor::ExpandVisitor(ExpandVisitor* parentEV,
                             SymbolMap& map) :
  forall(parentEV->forall),
  parIter(parentEV->parIter),
  svar2clonevar(map),
  taskFnCopies(parentEV->taskFnCopies),
  parentVis(NULL),
  breakOnYield(false)
{
}


/////////// misc helpers ///////////

//wass
static void showLOFS(ForallStmt* fs, ExpandVisitor* parentVis,
                     const char* msg, bool showParent) {
  if (!verb) return;
  printf("%s  %d", msg, fs->id);
  if (showParent) {
    printf("  %s", debugLoc(fs));
//    if (parentVis) printf("   parentVis %d", parentVis->forall->id);
//    else           printf("   -parentVis");
  }
  printf("\n");
}

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

#if 0 //wass - replaced with copyBody()
// Cf. copyBody() for inlineFunctions().
// 'anchor' is the AST that the new body will replace.
static BlockStmt* copyParIterBody(FnSymbol* iterFn, CallExpr* iterCall,
                                  Expr* anchor)
{
  SET_LINENO(iterCall);

  SymbolMap  map;

  BlockStmt* retval = NULL;

  for_formals_actuals(formal, actual, iterCall) {
    Symbol* sym = toSymExpr(actual)->symbol();

    // Replace an immediate actual with a temp var "just in case"
    if (sym->isImmediate() == true) {
      VarSymbol* tmp  = newTemp("inlineImm", sym->type);

      actual->replace(new SymExpr(tmp));

      anchor->insertBefore(new DefExpr(tmp));
      anchor->insertBefore(new CallExpr(PRIM_MOVE, tmp, actual));

      sym = tmp;
    }
    if (formal->isRef() && !sym->isRef()) {
      // When passing a value to a reference argument,
      // create a reference temporary so that nothing in the
      // inlined code changes meaning.

      VarSymbol* tmp  = newTemp(astr("i_", formal->name),
                                formal->type);
      DefExpr*   def  = new DefExpr(tmp);
      CallExpr*  move = NULL;

      tmp->qual = QUAL_REF;
      move      = new CallExpr(PRIM_MOVE,
                               tmp,
                               new CallExpr(PRIM_SET_REFERENCE, sym));

      anchor->insertBefore(def);
      anchor->insertBefore(move);

      sym = tmp;
    }

    map.put(formal, sym);
  }

  retval = iterFn->body->copy(&map);

  if (preserveInlinedLineNumbers == false) {
    reset_ast_loc(retval, iterCall);
  }

  removeParIterReturn(retval, false);

  return retval;
}
#endif
#if 0 //wass need this?

/* Do we want to make this available to all compiler code?
MF says "It should:
 * check isUserDefinedRecord and possibly is POD
 * error if a user-defined record has no auto copy
 * use PRIM_MOVE for non-user defined records
 * use autoCopy if one is defined, even for if isUserDefinedRecord returned false
Todo: should it support the case of initializing a 'ref' variable?
Todo: what if 'valType' is a ref type, ex. for RI accumulation state?
*/
//
// Initialize 'dest' from 'init', which can be either a symbol or an expr.
// Insert the initialization code before 'anchor'.
//
static void insertCopyInitialization(Symbol* dest, Type* valType,
                                     BaseAST* init, Expr* anchor)
{
  if (Expr* initExpr = toExpr(init))
    INT_ASSERT(!initExpr->inTree()); // caller responsibility

  if (FnSymbol* autoCopyFn = getAutoCopy(valType)) {    
    Symbol* initSym = toSymbol(init);
    if (initSym == NULL) {
      VarSymbol* actemp = new VarSymbol("actemp", valType);
      anchor->insertBefore(new DefExpr(actemp));
      anchor->insertBefore(new CallExpr(PRIM_MOVE, actemp, init));
      initSym = actemp;
    }
    anchor->insertBefore(new CallExpr(PRIM_MOVE, dest,
                           new CallExpr(autoCopyFn, initSym)));
  } else {
    anchor->insertBefore(new CallExpr(PRIM_MOVE, dest, init));
  }
}

//
// Insert deinit() if needed, after 'anchor'.
//
static void insertDeinitialization(Symbol* dest, Type* valType, Expr* anchor)
{
  if (FnSymbol* autoDestroy = getAutoDestroy(valType))
    anchor->insertAfter(new CallExpr(autoDestroy, dest));
}
#endif


/////////// standardized svar actions ///////////

static VarSymbol* createCurrSI(ShadowVarSymbol* SI) {
  INT_ASSERT(!SI->isRef());
  VarSymbol* currSI = new VarSymbol(SI->name, SI->type);
  currSI->qual = SI->isConstant() ? QUAL_CONST_VAL : QUAL_VAL;
  return currSI;

#if 0 //wass was
          aInit->insertBefore(new DefExpr(bodyvar));

          // Initialize it from the outer var.
          insertCopyInitialization(bodyvar, bodyvar->type,
                                   svar->outerVarSym(), aInit);

          // Deinitialize it at the end.
          insertDeinitialization(bodyvar, bodyvar->type, aFini);
#endif
}

static VarSymbol* createCurrRP(ShadowVarSymbol* RP) {
  VarSymbol* currRP = new VarSymbol(astr("RP_", RP->name), RP->type);
  currRP->qual = QUAL_CONST_VAL;
  return currRP;
}

static VarSymbol* createCurrAS(ShadowVarSymbol* AS) {
  VarSymbol* currAS = new VarSymbol(astr("AS_", AS->name), AS->type);
  currAS->qual = QUAL_VAL;
  return currAS;

#if 0 //wass - was:
  aInit->insertBefore(new DefExpr(acState));
  insertDeinitialization(acState, acState->type, aFini);

  map.put(svar, acState);

  aInit->insertBefore(svar->initBlock()->copy(&map));
  aFini->insertAfter(svar->deinitBlock()->copy(&map));
#endif

#if 0 //wass - was:
  ShadowVarSymbol* rOp = toShadowVarSymbol(svar->outerVarSym());
  Symbol*     globalOp = rOp->outerVarSym(); // aka map.get(rOp)

  // acState.init(globalOp.identity);  (paren-less function)
  // TODO stash away the FnSymbol for identity at resolution.
  VarSymbol*  stemp = newTempConst("rsvTemp", svar->type);
  aInit->insertBefore(new DefExpr(stemp));
  aInit->insertBefore("'move'(%S, identity(%S,%S))",
                      stemp, gMethodToken, globalOp);
  // What if this is a ref type?
  insertCopyInitialization(acState, acState->type, stemp, aInit);

  // globalOp.accumulate(acState); acState.deinit();
  aFini->insertAfter("accumulate(%S,%S,%S)",
                     gMethodToken, globalOp, acState);
#endif
 }

static void addDefAndMap(Expr* aInit, SymbolMap& map, ShadowVarSymbol* svar,
                         VarSymbol* currVar)
{
  aInit->insertBefore(new DefExpr(currVar));
  map.put(svar, currVar);
}

static void addCloneOfIB(Expr* aInit, SymbolMap& map, ShadowVarSymbol* svar) {
  //
  // We have to clone IB as a whole. This is to ensure that the uses
  // of any symbols that the original IB defines (via DefExprs
  // within that IB) get converted to uses of these symbols' copies.
  //
  BlockStmt* copyIB = svar->initBlock()->copy(&map);
  aInit->insertBefore(copyIB);
  // Let's drop the BlockStmt wrapper, to simplify the AST.
  copyIB->flattenAndRemove();
/*wass was:  
  for_alist(stmt, IB->body)
    aInit->insertBefore(stmt->copy(&map));
*/
}

static void addCloneOfDB(Expr* aFini, SymbolMap& map, ShadowVarSymbol* svar) {
  BlockStmt* copyDB = svar->deinitBlock()->copy(&map);
  aFini->insertAfter(copyDB);
  // Let's drop the BlockStmt wrapper, to simplify the AST.
  copyDB->flattenAndRemove();
/*wass was:
  BlockStmt* DB = svar->deinitBlock();
  for_alist_backward(stmt, DB->body)
    aFini->insertAfter(stmt->copy(&map));
*/
}


/////////// expandYield ///////////

// Clone and return the index variable for use in the cloned body.
// Wass after the body is simplified, can inline it back into expandYield.
//
static VarSymbol* setupCloneIdxVar(ExpandVisitor* EV, CallExpr* yieldCall,
                                   SymbolMap& map)
{
  // There is only one idx var because all zippering has been lowered away.
  VarSymbol* origIdxVar = parIdxVar(EV->forall);
  INT_ASSERT(origIdxVar && map.get(origIdxVar) == NULL); //wass remove at end

  // copy() also performs map.put(origIdxVar, cloneIdxVar).
  VarSymbol* cloneIdxVar = origIdxVar->copy(&map);
  INT_ASSERT(map.get(origIdxVar) == cloneIdxVar); //wass remove at end
  // wass confirming: this does not affect EV->svar2clonevar

  return cloneIdxVar;
}

// Replace 'yield' with a clone of the forall loop body.
static void expandYield(ExpandVisitor* EV, CallExpr* yieldCall)
{
if (verb) //wass
  printf("   expandYield %d %s   fs %d\n", yieldCall->id, debugLoc(yieldCall),
         EV->forall->id);
if (EV->forall->id == breakOnResolveID || breakOnExpand) gdbShouldBreakHere();

  // Todo: update EV->svar2clonevar in-place, then reset to NULL,
  // to avoid map creation+destruction cost.

  // This will be svar2clonevar (the incoming "current map")
  // plus a clone of the induction variable.
  SymbolMap map;
  map.copy(EV->svar2clonevar);

  // This adds (original idxVar -> cloneIdxVar) to 'map'.
  VarSymbol* cloneIdxVar = setupCloneIdxVar(EV, yieldCall, map);
  yieldCall->insertBefore(new DefExpr(cloneIdxVar));

  Expr* yieldExpr = yieldCall->get(1)->remove();
  yieldCall->insertBefore(new CallExpr(PRIM_MOVE, cloneIdxVar, yieldExpr));

  BlockStmt* bodyClone = EV->forall->loopBody()->copy(&map);
  yieldCall->replace(bodyClone);

  // Note that we are not descending into 'bodyClone'.  The only thing
  // that needs to be done there is to lower its ForallStmts, if any.
  // That will be done when lowerForallStmtsInline() gets to them.
  // They are guaranteed to come later because we just created them
  // when doing loopBody->copy().
}


/////////// expandTaskFn ///////////

// wass was: static Vec<FnSymbol*> taskFnClonesToFlatten;
//wass static Vec<FnSymbol*> inlinedIteratorsToRemove;

/*
At this point in compilation, in certain cases an in-intent formal is
represented as a ref-intent formal + copy construction into a "formal temp".
See insertFormalTemps(). As a result, in-intent formals are treated
as PODs, with argument passing implemented as memcopy,

So we need to mimick that here. Otherwise we miss copy-construction
from the actual into the formal.
*/
static void addFormalTempSIifNeeded(FnSymbol* cloneTaskFn, Expr* aInit,
                                    SymbolMap& map, ShadowVarSymbol* SI,
                                    bool expandClone) {
  if (!expandClone)
    return; // not modifying cloneTaskFn

  // This record must be present and contain the eFormal - see addArgAndMap().
  SymbolMapElem* e = map.get_record(SI);
  ArgSymbol* eFormal = toArgSymbol(e->value);

  if (!formalRequiresTemp(eFormal, cloneTaskFn))
    return; // not that case

  VarSymbol* currSI = createCurrSI(SI);
  aInit->insertBefore(new DefExpr(currSI));

  // map(SI) = currSI; map(SO) = eFormal
  e->value = currSI;
  map.put(SI->SOforSI(), eFormal);

  eFormal->intent = INTENT_CONST_REF;
  // non-ref type is ok for eFormal

  addCloneOfIB(aInit, map, SI);
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
                         bool expandClone, int ix, Symbol* mappee = NULL)
{
  Symbol* eActual = iMap.get(svar);   // 'e' for "extra" (i.e. newly added)
  callToTFn->insertAtTail(eActualOrRef(callToTFn, svar, eActual));

  if (expandClone) {
    ArgSymbol* eFormal = newExtraFormal(svar, ix, eActual, /*nested:*/false);
    cloneTaskFn->insertFormalAtTail(eFormal);
    map.put(mappee ? mappee : svar, eFormal);
  }
  else {
    ArgSymbol* eFormal = cloneTaskFn->getFormal(numOrigActuals+ix);
    if (eFormal->hasFlag(FLAG_REF_TO_IMMUTABLE))
      // Ensure this flag is correct for all calls. See newExtraFormal().
      INT_ASSERT(eActual->isConstValWillNotChange() ||
                 eActual->hasFlag(FLAG_REF_TO_IMMUTABLE));
  }
}

static void expandShadowVarTaskFn(FnSymbol* cloneTaskFn, CallExpr* callToTFn,
                                  Expr* aInit, Expr* aFini,
                                  int numOrigAct, SymbolMap& iMap,
                                  SymbolMap& map, ShadowVarSymbol* svar,
                                  bool expandClone, int ix) {
  SET_LINENO(svar);
  switch (svar->intent)
  {
    case TFI_IN_OVAR:     // helper svar - nothing to do
      break;

    case TFI_IN:          // in intents
    case TFI_CONST_IN:
      addArgAndMap(cloneTaskFn, callToTFn, numOrigAct, iMap,
                   map, svar, expandClone, ix);
      addFormalTempSIifNeeded(cloneTaskFn, aInit, map, svar, expandClone);
      addCloneOfDB(aFini, map, svar);
      break;

    case TFI_REF:         // ref intents
    case TFI_CONST_REF:
      addArgAndMap(cloneTaskFn, callToTFn, numOrigAct, iMap,
                   map, svar, expandClone, ix);
      // no init/deinit
      break;

    case TFI_REDUCE_OP:   // reduction op class
      addArgAndMap(cloneTaskFn, callToTFn, numOrigAct, iMap,
                   map, svar, expandClone, ix, svar->outerVarSym());
      addDefAndMap(aInit, map, svar, createCurrRP(svar));
      addCloneOfIB(aInit, map, svar);
      addCloneOfDB(aFini, map, svar);
      break;

    case TFI_REDUCE:      // accumulation state
      addDefAndMap(aInit, map, svar, createCurrAS(svar));
      addCloneOfIB(aInit, map, svar);
      addCloneOfDB(aFini, map, svar);
      break;

    case TFI_DEFAULT:    // no abstract intents, please
    case TFI_CONST:
      INT_ASSERT(false);
      break;
  }
}

static void expandTaskFn(ExpandVisitor* EV, CallExpr* callToTFn, FnSymbol* taskFn)
{
if (EV->forall->id == breakOnResolveID || breakOnExpand) gdbShouldBreakHere();

  FnSymbol* cloneTaskFn = EV->taskFnCopies.get(taskFn);
  bool expandClone = false;
  bool addErrorArgToCall = false;
  Expr *aInit=NULL, *aFini=NULL;

  if (cloneTaskFn == NULL) {
    // Follow the cloning steps in expandBodyForIteratorInline().

    // This holds because we flatten everything right away.
    // We need it so that we can place the def of 'fcopy' anywhere
    // while preserving correct scoping of its SymExprs.
    INT_ASSERT(isGlobal(taskFn));

    cloneTaskFn = taskFn->copy();
    EV->taskFnCopies.put(taskFn, cloneTaskFn);

    if (!preserveInlinedLineNumbers)
      reset_ast_loc(cloneTaskFn, callToTFn);

    // Note that 'fcopy' will likely get a copy of 'body',
    // so we need to preserve correct scoping of its SymExprs.
    callToTFn->insertBefore(new DefExpr(cloneTaskFn));

    // We will need to process the clone.
    expandClone = true;
    aInit = new CallExpr("aInit"); cloneTaskFn->insertAtHead(aInit);
    aFini = new CallExpr("aFini"); cloneTaskFn->insertIntoEpilogue(aFini);
    // Should aFini go into the epilogue or before epilogue?
  }
  else {
    INT_ASSERT(false); // wass if we never hit the cache, remove the complexity
  }

  callToTFn->baseExpr->replace(new SymExpr(cloneTaskFn));

  SymbolMap& iMap = EV->svar2clonevar;  // incoming "current map"
  SymbolMap  map;                       // "current map" for cloneTaskFn body

  int numOrigActuals = callToTFn->numActuals();
  int ix = 0;

  for_shadow_vars(svar, temp, EV->forall)
    expandShadowVarTaskFn(cloneTaskFn, callToTFn, aInit, aFini,
                          numOrigActuals, iMap, map, svar, expandClone, ++ix);

  if (expandClone) {
    aInit->remove();
    aFini->remove();
    // wass was: taskFnClonesToFlatten.add(cloneTaskFn);

    ExpandVisitor taskFnVis(EV, map);
    taskFnVis.parentVis = EV->parentVis;

    // Traverse recursively.
    cloneTaskFn->body->accept(&taskFnVis);

    fixupErrorHandlingExits(cloneTaskFn->body, addErrorArgToCall);

    if (addErrorArgToCall)
      addDummyErrorArgumentToCall(callToTFn);

    // If we don't flatten it right away, we get non-global taskFns
    // in expandTaskFn(). That may cause issues with scoping.
    flattenNestedFunction(cloneTaskFn);
  }
}

/////////// expandForall ///////////

static void expandForall(ExpandVisitor* EV, ForallStmt* fs)
{
  showLOFS(fs, EV, " { expandForall", true);
if (EV->forall->id == breakOnResolveID || breakOnExpand) gdbShouldBreakHere();

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

    if (Symbol* newOVar = newSV->outerVarSym()) // wass remove after testing
      INT_ASSERT(newOVar == iMap.get(srcSV));

    fs->shadowVariables().insertAtTail(new DefExpr(newSV));
  }

  // Traverse recursively.
  fs->loopBody()->accept(&forallVis);

  showLOFS(fs, EV, " } expandForall", false);
}


/////////// outermost visitor ///////////

static void expandShadowVarTopLevel(Expr* aInit, Expr* aFini, SymbolMap& map, ShadowVarSymbol* svar) {
  SET_LINENO(svar);
  switch (svar->intent)
  {
    case TFI_IN_OVAR:
      // The outer var for IB of the corresponding in-intent svar.
      map.put(svar, svar->SIforSO()->outerVarSym());
      break;

    case TFI_IN:
    case TFI_CONST_IN:
      addDefAndMap(aInit, map, svar, createCurrSI(svar));
      addCloneOfIB(aInit, map, svar);
      addCloneOfDB(aFini, map, svar);
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
      addCloneOfIB(aInit, map, svar);
      addCloneOfDB(aFini, map, svar);
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

/*
If 'fs' has only ref intents, or none at all,
revert to old iterator-record-based implementation.
If so, do what the original buildStandaloneForallLoopStmt()
did during parsing, with modifications.

Remove 'fs' and replace it with a ForLoop.
*/
static void handleRecursiveIter(ForallStmt* fs,
                                FnSymbol* parIterFn,  CallExpr* parIterCall)
{
  SET_LINENO(parIterCall);

  // Check for non-ref intents.
  SymbolMap sv2ov; // wass empty: sv2ov.size() NB - not a real size; which is sv2ov.i
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

  BlockStmt* PARBlock = fs->iterRecSetup();
  // Keep 'fs' in the tree for now, for debugging convenience.
  fs->insertAfter(PARBlock->remove());

  // These come from populateIterRecSetup().
  DefExpr* iterRecDef = toDefExpr(PARBlock->body.head);
  DefExpr* parIterDef = toDefExpr(iterRecDef->next);
  Expr*  callGetIter  = parIterDef->next;
  Expr*  callFreeIter = callGetIter->next;

  VarSymbol* iterRec = toVarSymbol(iterRecDef->sym);
  VarSymbol* parIter = toVarSymbol(parIterDef->sym);
  VarSymbol* parIdx  = parIdxVar(fs);
  DefExpr*   parIdxDef = parIdx->defPoint;

  // Just in case.
  INT_ASSERT(!strcmp(iterRec->name, "chpl__iterPAR"));
  INT_ASSERT(!strcmp(parIter->name, "chpl__parIter"));
  INT_ASSERT(parIdxDef == fs->inductionVariables().head);

  parIterDef->insertAfter(parIdxDef->remove());
  parIdxDef->insertAfter(new CallExpr(PRIM_MOVE, iterRec, parIterCall->remove()));

/*wass was:
  PARBlock->insertAtTail("'move'(%S, _getIterator(%S))", parIter, iterRec);
  PARBlock->insertAtTail(new DeferStmt(new CallExpr("_freeIterator", parIter)));
  PARBlock->insertAtTail("{TYPE 'move'(%S, iteratorIndex(%S)) }", parIdx, parIter);
*/

  ForLoop* PARBody = new ForLoop(parIdx, parIter, NULL, /* zippered */ false, /*forall*/ true);
  // not parIterCall, ex.
  //  library/standard/FileSystem/filerator/bradc/walk-par.chpl
  PARBody->astloc = fs->astloc;

  PARBlock->insertAtTail(PARBody);

/* wass not needed
  resolveBlockStmt(PARBlock);
  PARBlock->flattenAndRemove(); // into where 'fs' used to be
*/

  BlockStmt* userBody = userLoop(fs);
  if (sv2ov.size() > 0)  // wass the size of a Map is given by sv2ov.i
    update_symbols(userBody, &sv2ov);

#if 1 //wass - induction variables no more!
  INT_ASSERT(fs->inductionVariables().length == 0);
#else
  while (Expr* def = fs->inductionVariables().tail)
    userBody->insertAtHead(def->remove());
#endif

#if 0 //wass - no more shadow variables in loopBody!
  while (Expr* svdef = fs->shadowVariables().tail)
    fs->loopBody()->insertAtHead(svdef->remove());
#endif

  // wass improve clarity of what is flattened into what
  // relying on this assertion:
  INT_ASSERT(userBody == fs->loopBody());

  // Transfer the loop body from fs to PARBody.
  while (Expr* stmt = userBody->body.head)
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

/* wass
// If this calls a parallel iterator, return that.
// Otherwise find the parallel iterator being invoked,
// modify the callee to reference it, and return it.
static FnSymbol* getParIterAndConvertForwarders(CallExpr* parIterCall) {
  FnSymbol* destFn = parIterCall->resolvedFunction();
  if (destFn->hasFlag(FLAG_INLINE_ITERATOR))
    return destFn;

  // Other things may behave differently.
  INT_ASSERT(!strncmp(destFn->name, "_iterator_for_loopexpr", 22));
  destFn = getTheIteratorFnFromIR(destFn->retType);
  INT_ASSERT(destFn->hasFlag(FLAG_INLINE_ITERATOR));

  // Update parIterCall.
  SET_LINENO(parIterCall);
  parIterCall->baseExpr->replace(new SymExpr(destFn));
  return destFn;
}
*/


/////////// main driver ///////////

// Remove supporting references in ShadowVarSymbols.
// Otherwise flattenNestedFunction() will try to propagate them.
static void clearUpRefsInShadowVars() {
  forv_Vec(ShadowVarSymbol, svar, gShadowVarSymbols)
    if (svar->inTree())
      svar->removeSupportingReferences();
}

/*
We have created some nested task functions in expandTaskFn().
Flatten them because the compiler will crash otherwise.

Implementation considerations:
* For some task functions, we create them explicitly in expandTaskFn().
* Some others are created implicitly when we clone iterators that
  contain such task functions.
* The way flattenNestedFunctions() works as of this writing,
  it is faster to flatten all task fns at once, rather than one by one.
*/
// wass see if this is no longer needed
static void removeDeadAndFlatten() {
  Vec<FnSymbol*> taskFnsToFlatten;

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (!fn->inTree()) continue;

    if (fn->firstSymExpr() == NULL)
    {
      if (fn->hasFlag(FLAG_INLINE_ITERATOR) || isTaskFun(fn))
        // Got a parallel iterator or task function with no uses. Remove.
        // We have no business dealing with the other "unused" case.
        fn->defPoint->remove();
    }
    else if (!isGlobal(fn))
    {
      INT_ASSERT(isTaskFun(fn));
      taskFnsToFlatten.add(fn);
    }
  }

  INT_ASSERT(taskFnsToFlatten.n == 0); // wass - we flatten them right away now
  if (taskFnsToFlatten.n > 0)
    flattenNestedFunctions(taskFnsToFlatten);
}

// wass - need 'parentVis' ?
static void lowerOneForallStmt(ForallStmt* fs) {
  ExpandVisitor* parentVis = NULL; //wass - dummy
  showLOFS(fs, parentVis, "{ lonfs", true);
  if (fs->id == breakOnResolveID) gdbShouldBreakHere(); //wass // optionally: set breakOnExpand = true

  // If this fails, need to filter out those FSes.
  INT_ASSERT(fs->inTree() && fs->getFunction()->isResolved());

  // We convert zippering, if any, to a follower loop during resolution.
  INT_ASSERT(fs->numIteratedExprs() == 1);

  CallExpr* parIterCall = toCallExpr(fs->firstIteratedExpr());
  FnSymbol* parIterFn = parIterCall->resolvedFunction();

  // Make sure it is a parallel iterator.
  if (!parIterFn->hasFlag(FLAG_INLINE_ITERATOR))
    // This updates parIterCall, parIterFn.
    handleIteratorForwarders(fs, parIterCall, parIterFn);

  INT_ASSERT(parIterFn->hasFlag(FLAG_INLINE_ITERATOR));

  if (parIterFn->hasFlag(FLAG_RECURSIVE_ITERATOR)) {
    handleRecursiveIter(fs, parIterFn, parIterCall);
    // It is probably OK to lower other foralls meanwhile.
    return;
  }

  // Place to put pre- and post- code.
  SET_LINENO(fs); // wass what should it be?
  CallExpr*  ianch = new CallExpr("anchor");
  BlockStmt* iwrap = new BlockStmt(ianch);

  // Clone the iterator body.
  // Cf. expandIteratorInline() and inlineCall().
  BlockStmt* ibody = copyFnBodyForInlining(parIterCall, parIterFn, ianch);
  removeParIterReturn(ibody, false);

  // Let us remove 'fs' later, for debugging convenience.
  fs->insertAfter(iwrap);
  ianch->replace(ibody);

  TaskFnCopyMap   taskFnCopies;
  SymbolMap       map;
  ExpandVisitor   outerVis(fs, parIterFn, map, taskFnCopies);
  expandTopLevel(&outerVis, iwrap, ibody);
  outerVis.parentVis = parentVis;

  // Traverse recursively.
  ibody->accept(&outerVis);

  fs->remove();
  // We could also do {iwrap,ibody}->flattenAndRemove().

  if (parIterFn->firstSymExpr() == NULL)
    // We have inlined all uses. So, remove the iterator as well.
    parIterFn->defPoint->remove();

//wass
  showLOFS(fs, parentVis, "} lonfs", false);
}

///////////

void lowerForallStmtsInline()
{
  gdbShouldBreakHere(); //wass

  forv_Vec(ForallStmt, fs, gForallStmts)
    if (fs->inTree())
      lowerOneForallStmt(fs);

  USR_STOP();

  clearUpRefsInShadowVars();

  removeDeadAndFlatten();

  gdbShouldBreakHere(); //wass
}
