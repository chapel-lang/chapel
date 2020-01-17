/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

#include "DecoratedClassType.h"
#include "ForallStmt.h"
#include "passes.h"
#include "resolution.h"
#include "resolveIntents.h"
#include "stringutil.h"
#include "stmt.h"


/////////////////////////////////////////////////////////////////////////////
// needsShadowVar() and helpers
//

// Is 'sym' defined outside 'block'?
static bool isOuterVarNew(Symbol* sym, BlockStmt* block) {
  DefExpr*  defPt = sym->defPoint;
  Expr* parentExp = defPt->parentExpr;

  while (true) {
    if (!parentExp) {
      Symbol* parentSym = defPt->parentSymbol;
      if (isModuleSymbol(parentSym))
        // We reached the outermost level and did not come across 'block'.
        return true;

      defPt     = parentSym->defPoint;
      parentExp = defPt->parentExpr;
      continue;
    }
    if (parentExp == block)
      return false;

    parentExp = parentExp->parentExpr;
  }
  INT_ASSERT(false);
  return false; // dummy
}

// Is 'sym' an index variable of 'fs' ?
static bool isFsIndexVar(ForallStmt* fs, Symbol* sym)
{
  if (!sym->hasFlag(FLAG_INDEX_VAR))
    return false;

  return sym->defPoint->list == &fs->inductionVariables();
}

static bool isFsShadowVar(ForallStmt* fs, Symbol* sym)
{
  if (!isShadowVarSymbol(sym))
    return false;

  return sym->defPoint->list == &fs->shadowVariables();
}

static bool needsShadowVar(ForallStmt* fs, BlockStmt* block, Symbol* sym) {
  return
    isLcnSymbol(sym)             && // include only variable-like things
    sym->type != dtMethodToken   && // not a method token
    sym->defPoint->parentSymbol != rootModule && // not a system symbol
    !sym->isParameter()          && // not a param, including isImmediate()
    !sym->hasFlag(FLAG_INSTANTIATED_PARAM) && // not a param, again
    !sym->hasFlag(FLAG_TYPE_VARIABLE)      && // not a type alias or formal
    !sym->hasFlag(FLAG_TEMP)     && // not a temp
    !isFsIndexVar(fs, sym)       && // not fs's index var
    !isFsShadowVar(fs, sym)      && // not fs's shadow var
    isOuterVarNew(sym, block);      // it must be an outer variable
}

/////////////////////////////////////////////////////////////////////////////
// handle void shadow vars or outer vars
//

//
// We should keep all reduction shadow vars in the AST to preserve
// well-formedness of the sequence (PRP, PAS, RP, AS).
// Even if one of them is void, AS->initBlock()/deinitBlock() may still
// contain meaningful actions. Otherwise we could toss all four of them.
//
bool preserveShadowVar(Symbol* var) {
  if (ShadowVarSymbol* svar = toShadowVarSymbol(var)) {
    switch (svar->intent) {
      case TFI_REDUCE:
      case TFI_REDUCE_OP:
      case TFI_REDUCE_PARENT_AS:
      case TFI_REDUCE_PARENT_OP:
        return true;

      default:
        break; // handle normally
    }
  }

  return false;
}

// If an outer var is void, it was probably pruned.
// Replace it with none.
void adjustVoidShadowVariables() {
  forv_Vec(ShadowVarSymbol, svar, gShadowVarSymbols)
    if (svar->inTree())
      if (Symbol* ovar = svar->outerVarSym())
        if (ovar->type == dtNothing) {
          SET_LINENO(svar);
          svar->outerVarSE->replace(new SymExpr(gNone));
        }
}

/////////////////////////////////////////////////////////////////////////////
// setup support helpers
//

// insertInitialization() flavors:
//  (a1) initialize an arbitrary Symbol, place it in a BlockStmt, or
//  (a2) initialize a ShadowVarSymbol, place it in its initBlock();
//  (b1) initialize from a Symbol, or
//  (b2) initialize from an Expr, computing it into a temp.

static void insertInitialization(BlockStmt* destBlock,
                                 Symbol* destVar, Symbol* srcVar) {
  destBlock->insertAtTail("'init var'(%S,%S)", destVar, srcVar);
}

static void insertInitialization(ShadowVarSymbol* destVar, Symbol* srcVar) {
  insertInitialization(destVar->initBlock(), destVar, srcVar);
}

static void insertInitialization(BlockStmt* destBlock,
                                 Symbol* destVar, Expr* srcExpr) {

  Expr* initCall = new_Expr("'init var'(%S,%E)", destVar, srcExpr);
  destBlock->insertAtTail(initCall);
  normalize(initCall);
}

// insertDeinitialization() flavors: as (a1) or (a2) above

static void insertDeinitialization(BlockStmt* destBlock, Symbol* destVar) {
  // NB if we use PRIM_CALL_DESTRUCTOR, we end up with
  // deinit() calls for shadow variables of class types.
  destBlock->insertAtTail("chpl__autoDestroy(%S)", destVar);
}

static void insertDeinitialization(ShadowVarSymbol* destVar) {
  insertDeinitialization(destVar->deinitBlock(), destVar);
}

static void resolveOneShadowVar(ForallStmt* fs, ShadowVarSymbol* svar) {
  resolveBlockStmt(svar->initBlock());
  resolveBlockStmt(svar->deinitBlock());
}

/////////////////////////////////////////////////////////////////////////////
// setup for a reduce intent
//

//
// Incorporate the initial value of the outer variable:
//   globalOp.initialAccumulate(outerVar)
// If this does not resolve, instead call:
//   globalOp.accumulate(outerVar)
//
static void insertAndResolveInitialAccumulate(ForallStmt* fs, BlockStmt* hld,
                                            Symbol* globalOp, Symbol* outerVar)
{
  CallExpr* initAccum = new CallExpr("initialAccumulate", gMethodToken,
                                     globalOp, outerVar);
  if (hld)
    hld->insertAtTail(initAccum);
  else
    fs->insertBefore(initAccum);

  FnSymbol* initAccumOutcome = tryResolveCall(initAccum);

  if (initAccumOutcome) {
    // Yes, use initialAccumulate().

  } else {
    // No, call accumulate() instead.
    CallExpr* accum = new CallExpr("accumulate", gMethodToken,
                                   globalOp, outerVar);
    initAccum->replace(accum);
    initAccum = accum;
    initAccumOutcome = tryResolveCall(initAccum);

    if (initAccumOutcome) {
      // OK, will use accumulate();

    } else {
      // The user must provide at least one of the two.
      USR_FATAL_CONT(initAccum, "the reduction operation of the reduce intent for the variable %s does not provide initialAccumulate() or accumulate() that accepts the initial value of this variable", outerVar->name);

      // We can continue compilation meanwhile.
      initAccum->remove();
      return;
    }
  }

  resolveFnForCall(initAccumOutcome, initAccum);
}

//
// This works around #12497, whereby FLAG_EXPR_TEMP works correctly
// for all cases except when:
// * the result of a reduce expression is detupled in a variable decl, ex.
//     var (val,idx) = minloc reduce ...;
// * the result of the reduce op's generate() is a tuple containing an array.
//
// If so, we get valgrind failure due to double-deletion in these tests:
//   reductions/vass/reproducer-tuple-with-array
//   studies/kmeans/kmeans-blc
//   studies/kmeans/kmeansonepassreduction-minchange
//
// This is invoked only for ForallStmts representing reduce expressions.
//
static void workaroundForReduceIntoDetupleDecl(ForallStmt* fs, Symbol* svar) {
  //
  // The pattern we are looking for is when 'svar' is used twice:
  // * as an outer variable of one of fs's shadow variables, and
  // * in a PRIM_INIT_VAR of a variable with FLAG_NO_COPY
  //   which is created in buildTupleVarDeclStmt().
  //

  // Find the SE using 'svar' as an outer var.
  SymExpr* ovarSE = NULL;
  for_shadow_vars(sv2,temp,fs)
    if (SymExpr* ovar2 = sv2->outerVarSE)
      if (ovar2->symbol() == svar)
        { ovarSE = ovar2; break; }
  if (ovarSE == NULL) return; // not our pattern

  // Find the other use of 'svar'.
  SymExpr* otherUse = NULL;
  for_SymbolUses(useSE, svar)
    if (useSE != ovarSE) {
      if (otherUse != NULL) return; // too many uses - not our pattern
      otherUse = useSE;
    }
  if (otherUse == NULL) return; // not our pattern

  // Find the var initialized from 'otherUse', if any.
  if (CallExpr* initCall = toCallExpr(otherUse->parentExpr))
    if (initCall->isPrimitive(PRIM_INIT_VAR))
      if (SymExpr* initSE = toSymExpr(initCall->get(1))) {
        INT_ASSERT(initSE != otherUse); // otherwise 'otherUse' is not a "use"
        // The final check...
        if (initSE->symbol()->hasFlag(FLAG_NO_COPY))
          // Yes, found the pattern we are looking for!
          // React to it:
          svar->removeFlag(FLAG_EXPR_TEMP);
      }
}

// Finalize the reduction:  outerVar = globalOp.generate()
static void insertFinalGenerate(ForallStmt* fs,
                                Symbol* fiVarSym, Symbol* globalOp)
{
  Expr* next = fs->next; // nicer ordering of the following insertions
  INT_ASSERT(next);
  if (fs->needsInitialAccumulate()) {
    VarSymbol* genTemp = newTemp("chpl_gentemp");
    next->insertBefore(new DefExpr(genTemp));
    next->insertBefore("'move'(%S, generate(%S,%S))",
                     genTemp, gMethodToken, globalOp);
    next->insertBefore(new CallExpr("=", fiVarSym, genTemp));
    // TODO: Should we try to free chpl_gentemp right after the assignment?
    genTemp->addFlag(FLAG_INSERT_AUTO_DESTROY);
  } else {
    // Initialize, not assign. Do everything *after* 'fs'.
    next->insertBefore(fiVarSym->defPoint->remove());
    next->insertBefore("'move'(%S, generate(%S,%S))",
                       fiVarSym, gMethodToken, globalOp);
    fiVarSym->addFlag(FLAG_EXPR_TEMP);
    workaroundForReduceIntoDetupleDecl(fs, fiVarSym);
  }
}

static void moveInstantiationPoint(BlockStmt* to, BlockStmt* from, Type* type) {
  //
  // BHARSH 2018-05-08:
  // Workaround for point of instantiation issue. We resolve some functions
  // within the block 'hld', which sets the point of instantiation for those
  // functions to 'hld'. We then flatten and remove 'hld' from the tree, so the
  // functions have an invalid point of instantiation.
  //
  // This snippet updates the instantiation point of the reduction class and
  // its methods to point to the surviving Block (parent of 'hld').
  //
  // Vass 2018-08-01: Does not look like we can remove this yet. See ex.:
  //   release/examples/benchmarks/isx
  //   parallel/forall/reduce-intents
  AggregateType* reductionClass = toAggregateType(canonicalDecoratedClassType(type));
  if (reductionClass->symbol->instantiationPoint == from) {
    reductionClass->symbol->instantiationPoint = to;
    forv_Vec(FnSymbol, fn, reductionClass->methods) {
      if (fn->instantiationPoint() == from) {
        fn->setInstantiationPoint(to);
      }
    }
  }
}

static Symbol* setupRiGlobalOp(ForallStmt* fs, Symbol* fiVarSym,
                               Expr* origRiSpec, TypeSymbol* riTypeSym,
                               Expr* eltTypeArg)
{
  BlockStmt* hld = new BlockStmt(); // "holder"
  fs->insertBefore(hld);
      
  VarSymbol* globalOp = newTemp(astr("globalRP_", fiVarSym->name));
  hld->insertAtTail(new DefExpr(globalOp));

  if (!eltTypeArg) {
    Type* eltType   = fiVarSym->qualType().type()->getValType();
    // fiVar should have been resolved already
    INT_ASSERT(eltType != dtUnknown && eltType != dtAny);

    if (eltType->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE)) {
      // arg = typeof(fiVar)
      VarSymbol* typeofTemp = newTemp("chpl__typeofET");
      typeofTemp->addFlag(FLAG_TYPE_VARIABLE);
      hld->insertAtTail(new DefExpr(typeofTemp));
      hld->insertAtTail("'move'(%S,'typeof'(%S))", typeofTemp, fiVarSym);
      eltTypeArg = new SymExpr(typeofTemp);
    } else {
      // Forego typeof(), use the type directly.
      eltTypeArg = new SymExpr(eltType->symbol);
    }
  }


  {
    NamedExpr* newArg = new NamedExpr("eltType", eltTypeArg);
    CallExpr* newCall = new CallExpr(PRIM_NEW, new SymExpr(riTypeSym), newArg,
                                     new NamedExpr(astr_chpl_manager,
                                         new SymExpr(dtUnmanaged->symbol)));
    hld->insertAtTail(new CallExpr(PRIM_MOVE, globalOp, newCall));
  }

  fs->insertAfter("chpl__delete(%S)", globalOp);
  insertFinalGenerate(fs, fiVarSym, globalOp);

  resolveBlockStmt(hld);

  if (fs->needsInitialAccumulate())
    insertAndResolveInitialAccumulate(fs, hld, globalOp, fiVarSym);

  moveInstantiationPoint(toBlockStmt(hld->parentExpr), hld, globalOp->type);

  hld->flattenAndRemove();

  origRiSpec->replace(new SymExpr(globalOp));
  
  return globalOp;
}

static void handleRISpec(ForallStmt* fs, ShadowVarSymbol* svar)
{
  Symbol* globalOp = NULL;
  Symbol* fiVarSym = svar->outerVarSym();
  Expr*   riSpec   = svar->reduceOpExpr();
  SET_LINENO(riSpec);

  if (SymExpr* riSE = toSymExpr(riSpec)) {
    Symbol* riSym = riSE->symbol();

    if (TypeSymbol* riTypeSym = toTypeSymbol(riSym)) {
      globalOp = setupRiGlobalOp(fs, fiVarSym, riSpec, riTypeSym, NULL);

    } else {
      INT_ASSERT(isLcnSymbol(riSym)); // what else can a globalOp be??

      if (fs->needsInitialAccumulate())
        insertAndResolveInitialAccumulate(fs, NULL, riSym, fiVarSym);

      // The user will manage allocation and deallocation of 'riSym'.
      // So we only need to add a call generate().
      insertFinalGenerate(fs, fiVarSym, riSym);
      globalOp = riSym;
    }

  } else if (CallExpr* riSpecCall = toCallExpr(riSpec)) {
    SymExpr* riTypeSE = toSymExpr(riSpecCall->baseExpr);
    INT_ASSERT(riTypeSE); // need to handle baseExpr anyway
    TypeSymbol* riTypeSym = toTypeSymbol(riTypeSE->symbol());
    INT_ASSERT(riTypeSym); // non-types should be handled elsewhere

    if (riSpecCall->numActuals() != 1)
      USR_FATAL(riSpecCall, "for a reduce intent, the 'reduce' keyword must be preceded by the reduction operator or the name of the reduction class with the single optional argument indicating the type of the reduction input");

    riTypeSE->remove();
    Expr* eltTypeArg = riSpecCall->get(1)->remove();
    globalOp = setupRiGlobalOp(fs, fiVarSym, riSpec, riTypeSym, eltTypeArg);

  } else {
    // What else can this be?
    INT_FATAL(fs, "not implemented");
  }
  // Removing 'globalOp' is left as future work.
  if (globalOp) INT_ASSERT(globalOp); // dummy use of 'globalOp'
}

void  setReduceSVars(ShadowVarSymbol*& PRP, ShadowVarSymbol*& PAS,
                     ShadowVarSymbol*& RP, ShadowVarSymbol* AS)
{
  DefExpr* defRP  = toDefExpr(AS->defPoint->prev);
  DefExpr* defPAS = toDefExpr(defRP->prev);
  DefExpr* defPRP = toDefExpr(defPAS->prev);

  RP  = toShadowVarSymbol(defRP->sym);
  PAS = toShadowVarSymbol(defPAS->sym);
  PRP = toShadowVarSymbol(defPRP->sym);

  INT_ASSERT(AS->intent  == TFI_REDUCE);
  INT_ASSERT(RP->intent  == TFI_REDUCE_OP);
  INT_ASSERT(PAS->intent == TFI_REDUCE_PARENT_AS);
  INT_ASSERT(PRP->intent == TFI_REDUCE_PARENT_OP);
}

static ShadowVarSymbol* create_REDUCE_PRP(ForallStmt* fs, ShadowVarSymbol* AS)
{
  // This is the global reduce op, either provided by user or set up by us.
  // PRP->outerVarSE will point to it.
  SymExpr* gOpSE = toSymExpr(AS->reduceOpExpr()->remove());
  Symbol*  gOp   = gOpSE->symbol();
  // Handling of the case of gOp being a type should have happened earlier.
  INT_ASSERT(!gOp->hasFlag(FLAG_TYPE_VARIABLE));

  ShadowVarSymbol* PRP = new ShadowVarSymbol(TFI_REDUCE_PARENT_OP,
                                             astr("PRP_", AS->name), gOpSE);

  // It always points to the same reduction op class instance.
  PRP->addFlag(FLAG_CONST);
  PRP->qual = QUAL_CONST_VAL;
  PRP->type = gOp->type;

  // It goes on the shadow variable list before AS.
  AS->defPoint->insertBefore(new DefExpr(PRP));

  return PRP;
}

static ShadowVarSymbol* create_REDUCE_PAS(ForallStmt* fs, ShadowVarSymbol* AS)
{
  ShadowVarSymbol* PAS = new ShadowVarSymbol(TFI_REDUCE_PARENT_AS,
                                             astr("PAS_", AS->name), NULL);
  PAS->qual = QUAL_VAL;
  PAS->type = AS->type;

  // It goes on the shadow variable list before AS, after PRP.
  AS->defPoint->insertBefore(new DefExpr(PAS));

  return PAS;
}

static ShadowVarSymbol* create_REDUCE_RP(ForallStmt* fs, ShadowVarSymbol* PRP,
                                         ShadowVarSymbol* AS)
{
  ShadowVarSymbol* RP = new ShadowVarSymbol(TFI_REDUCE_OP,
                                            astr("RP_", AS->name), NULL);

  // It always points to the same reduction op class instance.
  RP->addFlag(FLAG_CONST);
  RP->qual = QUAL_CONST_VAL;
  RP->type = PRP->type;

  // It goes on the shadow variable list right before AS.
  AS->defPoint->insertBefore(new DefExpr(RP));
  INT_ASSERT(AS->ReduceOpForAccumState() == RP); //ReduceOpForAccumState() OK?

  return RP;
}

static void setupForReduce(ForallStmt* fs,
                           ShadowVarSymbol* PRP, ShadowVarSymbol* PAS,
                           ShadowVarSymbol* RP,  ShadowVarSymbol* AS)
{
  // Put everything in AS's init/deinit block. No need to use the others'.
  BlockStmt* IB  = AS->initBlock();
  BlockStmt* DB  = AS->deinitBlock();

  /// at task startup ///
  insertInitialization(IB, RP, new_Expr("clone(%S,%S)", gMethodToken, PRP));
  insertInitialization(IB, AS, new_Expr("identity(%S,%S)", gMethodToken, RP));

  /// at task teardown ///
  DB->insertAtTail("accumulate(%S,%S,%S)", gMethodToken, RP, AS);
  insertDeinitialization(DB, AS);
  DB->insertAtTail("chpl__reduceCombine(%S,%S)", PRP, RP);
  DB->insertAtTail("chpl__cleanupLocalOp(%S,%S)", PRP, RP); // deletes RP

  // At the top level, we have global RP and global AS.
  // The global RP has already been set up. On to the global AS.
  Symbol*    globalRP = PRP->outerVarSym();
  VarSymbol* globalAS = new VarSymbol(astr("globalAS_", AS->name), AS->type);
  globalAS->qual = QUAL_VAL;

  PAS->outerVarSE = new SymExpr(globalAS);
  insert_help(PAS->outerVarSE, NULL, PAS);

  /// before the forall ///
  BlockStmt* holder1 = new BlockStmt();
  fs->insertBefore(holder1);
  holder1->insertAtTail(new DefExpr(globalAS));
  insertInitialization(holder1, globalAS,
                       new_Expr("identity(%S,%S)", gMethodToken, globalRP));
  resolveBlockStmt(holder1);
  PAS->type = globalAS->type; // now that we know it
  holder1->flattenAndRemove();

  /// after the forall ///
  BlockStmt* holder2 = new BlockStmt();
  fs->insertAfter(holder2);
  holder2->insertAtTail("accumulate(%S,%S,%S)",
                        gMethodToken, globalRP, globalAS);
  insertDeinitialization(holder2, globalAS);
  // the contents will be resolved after resolving 'fs'
  holder2->flattenAndRemove();
}

static void handleReduce(ForallStmt* fs, ShadowVarSymbol* AS) {
  handleRISpec(fs, AS);

  Symbol* ASovar = AS->outerVarSym();
  if (ASovar && ASovar->hasFlag(FLAG_CONST)) {
    USR_FATAL_CONT(fs,
      "reduce intent is applied to a 'const' variable %s", ASovar->name);
  }

  // The AS shadow var is a reference to the accumulation state.
  // Cf. RP is the reduce op.  PAS / PRP are like AS / RP.
  AS->addFlag(FLAG_REF_VAR);
  AS->qual = QUAL_REF;
  AS->type = dtUnknown;

  // PRP, PAS, RP precede AS in fs->shadowVariables() in this order.
  ShadowVarSymbol* PRP = create_REDUCE_PRP(fs, AS);
  ShadowVarSymbol* PAS = create_REDUCE_PAS(fs, AS);
  ShadowVarSymbol* RP  = create_REDUCE_RP(fs, PRP, AS);
  setupForReduce(fs, PRP, PAS, RP, AS);

  // Ensure setReduceSVars() works.
  ShadowVarSymbol *PRP1=0, *PAS1=0, *RP1=0;
  setReduceSVars(PRP1, PAS1, RP1, AS);
  INT_ASSERT(PRP1 == PRP);
  INT_ASSERT(PAS1 == PAS);
  INT_ASSERT(RP1  == RP);
}

/////////////////////////////////////////////////////////////////////////////
// handleOneShadowVar()
//

static ShadowVarSymbol* create_IN_Parentvar(ForallStmt* fs,
                                            ShadowVarSymbol* SI,
                                            Symbol* userOuterVar)
{
  ShadowVarSymbol* INP = new ShadowVarSymbol(TFI_IN_PARENT,
                                             astr("INP_", SI->name), NULL);
  INP->addFlag(FLAG_CONST);  // make it be like 'const in'
  INP->qual = QUAL_CONST_VAL;
  INP->type = SI->type;

  // INP goes on the shadow variable list right before SI.
  SI->defPoint->insertBefore(new DefExpr(INP));
  INT_ASSERT(SI->ParentvarForIN() == INP);  // ensure ParentvarForIN() works

  Symbol* INPovar = userOuterVar;

  if (userOuterVar->getValType() != SI->type) {
    // For example, when userOuterVar is "owned" and SI is "borrowed".
    BlockStmt* holder  = new BlockStmt();
    CallExpr*  cast    = createCast(userOuterVar, SI->type->symbol);
    VarSymbol* inptemp = newTempConst("INPtemp", SI->type);
    inptemp->qual      = QUAL_CONST_VAL;

    fs->insertBefore(holder);
    holder->insertAtTail(new DefExpr(inptemp));
    insertInitialization(holder, inptemp, cast);
    resolveBlockStmt(holder);
    holder->flattenAndRemove();

    INPovar = inptemp;
  }

  // Insert a move in the init block for this shadow variable
  // to enable other passes to understand the data flow. This
  // move is not needed in forall lowering.
  CallExpr* move = new CallExpr(PRIM_MOVE, INP, INPovar);
  INP->initBlock()->insertAtTail(move);

  INP->outerVarSE = new SymExpr(INPovar);
  insert_help(INP->outerVarSE, NULL, INP);

  return INP;
}

static void constDueToTFI(ShadowVarSymbol* svar, Symbol* ovar) {
  if (!ovar->isConstant())
    svar->addFlag(FLAG_CONST_DUE_TO_TASK_FORALL_INTENT);
}

static void handleIn(ForallStmt* fs, ShadowVarSymbol* SI, bool isConst) {
  Symbol* ovar = SI->outerVarSym();

  if (isConst) {
    SI->addFlag(FLAG_CONST);
    SI->qual = QUAL_CONST_VAL;
    constDueToTFI(SI, ovar);
  } else {
    SI->qual = QUAL_VAL;
  }
  INT_ASSERT(!SI->isRef());

  ShadowVarSymbol* INP = create_IN_Parentvar(fs, SI, ovar);
  insertInitialization(SI, INP);
  insertDeinitialization(SI);

  resolveOneShadowVar(fs, INP);
}

static void handleRef(ForallStmt* fs, ShadowVarSymbol* SR, bool isConst) {
  Symbol* ovar = SR->outerVarSym();
  if (isConst) {
    SR->addFlag(FLAG_CONST);
    SR->qual = QUAL_CONST_REF;
    constDueToTFI(SR, ovar);
  } else {
    SR->qual = QUAL_REF;
  }
  INT_ASSERT(SR->isRef());
  if (ovar->isConstValWillNotChange())
    SR->addFlag(FLAG_REF_TO_IMMUTABLE);
}

static void handleTaskPrivate(ForallStmt* fs, ShadowVarSymbol* TPV) {
  // initBlock() already comes from TPV's declaration in the with-clause,
  // so nothing to do with it.

  // Need deinitialization - for 'var'/'const' only. No deinit for refs.
  if (!TPV->isRef())
    insertDeinitialization(TPV);
}

static void handleOneShadowVar(ForallStmt* fs, ShadowVarSymbol* svar)
{
  if (svar->id == breakOnResolveID) gdbShouldBreakHere();

  switch (svar->intent) {
    case TFI_IN:           handleIn(fs, svar, false);   break;
    case TFI_CONST_IN:     handleIn(fs, svar, true);    break;

    case TFI_REF:          handleRef(fs, svar, false);  break;
    case TFI_CONST_REF:    handleRef(fs, svar, true);   break;

    case TFI_REDUCE:       handleReduce(fs, svar);      break;
    case TFI_TASK_PRIVATE: handleTaskPrivate(fs, svar); break;

    case TFI_DEFAULT:            // these should have been converted
    case TFI_CONST:              INT_ASSERT(false);     break;

    case TFI_IN_PARENT:          // these have not been created yet
    case TFI_REDUCE_OP:
    case TFI_REDUCE_PARENT_AS:
    case TFI_REDUCE_PARENT_OP:   INT_ASSERT(false);     break;
  }

  resolveOneShadowVar(fs, svar);
}

/////////////////////////////////////////////////////////////////////////////
// determine concrete intent and whether to prune
//

static ForallIntentTag forallIntentForArgIntent(IntentTag intent) {
  switch (intent)
  {
    case INTENT_IN:        return TFI_IN;
    case INTENT_CONST:     return TFI_CONST;
    case INTENT_CONST_IN:  return TFI_CONST_IN;
    case INTENT_REF:       return TFI_REF;
    case INTENT_CONST_REF: return TFI_CONST_REF;
    case INTENT_BLANK:     return TFI_DEFAULT;
    case INTENT_REF_MAYBE_CONST: return TFI_REF; //todo: TFI_REF_MAYBE_CONST ?

    case INTENT_OUT:
    case INTENT_INOUT:
    case INTENT_PARAM:
    case INTENT_TYPE:     INT_ASSERT(false);   // don't know what to return
                          return TFI_DEFAULT;  // dummy
  }
  INT_ASSERT(false);   // unexpected IntentTag; 'intent' contains garbage?
  return TFI_DEFAULT;  // dummy
}

/*
Why prune?

Historically, we have avoided using shadow variables in certain cases
because of the complications that we would need to handle downstream
in the compiler otherwise. As of this writing, only the ref-intents
case remains.

Examples of tests that fail without pruning under --local:
 arrays/deitz/jacobi5
 studies/hpcc/HPL/vass/bl.dim
 studies/shootout/submitted/knucleotide
 users/shetag/unboundslice

Potential culprits:
- choosing ref vs. const ref for INTENT_REF_MAYBE_CONST
- more efficient access to the outer variable if it is a global
  (for multilocale performance)
- others?
*/

//
// * If 'intent' is abstract, convert it to a concrete intent.
// * 
// It is done on an already-existing, explicit shadow variable
// or before an implicit shadow variable is to be created.
//
static void resolveShadowVarTypeIntent(Type*& type, ForallIntentTag& intent,
                                       bool& prune)
{
  switch (intent) {
    case TFI_DEFAULT:
    case TFI_CONST:
    // Convert to a concrete intent.
    {
      Type* valType = type->getValType();

      // For managed pointers, don't do ownership transfer by default.
      // Instead, use the borrow type for the shadow variable.
      if (isManagedPtrType(valType)) {
        type    = getManagedPtrBorrowType(valType);
        valType = type->getValType();
      }

      IntentTag argInt = (intent == TFI_DEFAULT) ? INTENT_BLANK : INTENT_CONST;
      intent = forallIntentForArgIntent(concreteIntent(argInt, valType));

      break;
    }

    case TFI_IN:               // Nothing to do for now.
    case TFI_CONST_IN:
    case TFI_REF:
    case TFI_CONST_REF:
    case TFI_REDUCE:
    case TFI_TASK_PRIVATE:      break;

    case TFI_IN_PARENT:         // These have not been created yet.
    case TFI_REDUCE_OP:
    case TFI_REDUCE_PARENT_AS:
    case TFI_REDUCE_PARENT_OP:  INT_ASSERT(false);  break;
  }

  // Enforce val type for in- and ref-intents.
  switch (intent) {
    case TFI_IN:
    case TFI_CONST_IN:
    case TFI_REF:
    case TFI_CONST_REF:         type = type->getValType(); break;

    case TFI_REDUCE:            // Nothing to do for now.
    case TFI_TASK_PRIVATE:      break;

    case TFI_DEFAULT:           // We just eliminated these.
    case TFI_CONST:             INT_ASSERT(false); break;

    case TFI_IN_PARENT:         // These have not been created yet.
    case TFI_REDUCE_OP:
    case TFI_REDUCE_PARENT_AS:
    case TFI_REDUCE_PARENT_OP:  INT_ASSERT(false);  break;
  }

  // Prune, as discussed in the above comment.
  if (intent == TFI_REF)
    prune = true;
}

/////////////////////////////////////////////////////////////////////////////
// handle implicit vs. explicit shadow variables and fields of 'this'
//

// Helper: if this is a 'this' that needs our attention, return its type.
static AggregateType* isRecordReceiver(Symbol* sym) {
  if (sym->hasFlag(FLAG_ARG_THIS))
    if (Type* type = sym->type->getValType())
      if (isUserRecord(type))
        return toAggregateType(type);
  return NULL;
}

//
// Helper: ensure that the receiver formal 'this' has a shadow variable
// when it may be needed for convertFieldsOfRecordReceiver().
//
static void assertNotRecordReceiver(Symbol* ovar, Expr* ref) {
  if (isRecordReceiver(ovar))
    INT_FATAL(ref, "removing shadow variable for 'this'");
}

//
// A forall-intents variation on findOuterVars() in createTaskFunctions.cpp:
// Find all symbols used in 'block' and defined outside of it.
//
// For each found symbol, see if it needs a ShadowVarSymbol.
// If so, create one, add it to fs->shadowVariables() and to 'outer2shadow'.
// Otherwise, map it to markPruned in 'outer2shadow'.
//
// At the same time, perform the substitutions already in 'outer2shadow'
// except markPruned.
//
static void doImplicitShadowVars(ForallStmt* fs, BlockStmt* block,
                                 SymbolMap& outer2shadow)
{
  std::vector<SymExpr*> symExprs;
  collectLcnSymExprs(block, symExprs);

  for_vector(SymExpr, se, symExprs) {
    Symbol* sym = se->symbol();

    if (Symbol* sub = outer2shadow.get(sym)) { // already know how to handle?
      if (sub != markPruned)
        se->setSymbol(sub); // substitute
      continue;
    }

    if (!needsShadowVar(fs, block, sym)) { // do not convert to shadow var
      assertNotRecordReceiver(sym, se);
      outer2shadow.put(sym, markPruned);
      continue;
    }

    ForallIntentTag intent = TFI_DEFAULT;
    Type* type  = sym->type;
    bool  prune = false;
    resolveShadowVarTypeIntent(type, intent, prune); // updates the args

    if (prune) {                      // do not convert to shadow var
      assertNotRecordReceiver(sym, se);
      outer2shadow.put(sym, markPruned);
      continue;
    }

    // Yes, convert to a shadow variable.
    ShadowVarSymbol* svar = new ShadowVarSymbol(intent, sym->name,
                                                new SymExpr(sym));
    svar->type = type;
    fs->shadowVariables().insertAtTail(new DefExpr(svar));

    handleOneShadowVar(fs, svar);

    outer2shadow.put(sym, svar); // mark it for substitution
    se->setSymbol(svar);
  }
}

static void collectAndResolveImplicitShadowVars(ForallStmt* fs)
{
  if (!fs->needToHandleOuterVars())  // no shadow vars, please
    return;

  SET_LINENO(fs);
  SymbolMap outer2shadow;

  // These are the additional blocks to look into for outer/shadow variables.
  // This is specific to the start-of-resolution-of-ForallStmt point,
  // where most initBlock() and deinitBlock() are irrelevant/empty.
  for_shadow_vars(svar, temp, fs)
    if (svar->isTaskPrivate())
      if (BlockStmt* initB = svar->initBlock())
        if (!initB->body.empty())
          doImplicitShadowVars(fs, initB, outer2shadow);

  // The bulk of the work is the loop body.
  doImplicitShadowVars(fs, fs->loopBody(), outer2shadow);
}

static Type* ovarOrSvarType(ShadowVarSymbol* svar) {
  if (Symbol* ovar = svar->outerVarSym())
    return ovar->type;
  else
    return svar->type;
}

static void removeUsesOfShadowVar(ShadowVarSymbol* svar) {
  // Substitute its uses with the outer var.
  Symbol* ovar = svar->outerVarSym();
  for_SymbolSymExprs(se, svar)
    se->setSymbol(ovar);

  // Remove it from the tree.
  svar->defPoint->remove();
}

static void resolveAndPruneExplicitShadowVars(ForallStmt* fs,
                                              Expr* lastExplicitSVarDef)
{
  if (lastExplicitSVarDef == NULL)
    return;  // there were no explicit shadow vars

  for_shadow_vars_and_defs(svar, svdef, temp, fs)
  {
    Type* type  = ovarOrSvarType(svar);
    bool  prune = false;
    resolveShadowVarTypeIntent(type, svar->intent, prune); // updates the args

    if (prune) {
      assertNotRecordReceiver(svar->outerVarSym(), svar->outerVarSE);
      removeUsesOfShadowVar(svar);
    }
    else {
      SET_LINENO(svar);
      svar->type = type;
      handleOneShadowVar(fs, svar);
    }

    if (svdef == lastExplicitSVarDef) {
      return; // we have processed implicit shadow vars elsewhere
    }
  }
}

// If it is a CallExpr(<something>, _mt, arg), and <something>
// names a field in 'recType', return that field's Symbol
static Symbol* isFieldAccess(AggregateType* recType, Expr* arg) {
  if (CallExpr* call = toCallExpr(arg->parentExpr))
    if (call->numActuals() == 2)
      if (call->get(1)->getValType() == dtMethodToken)
        // Field accesses are unresolved method calls at this point.
        // A call to a user-defined paren-less method that shadows a field
        // is considered a call to a field accessor and is handled like
        // field access.
        if (UnresolvedSymExpr* base = toUnresolvedSymExpr(call->baseExpr))
          if (Symbol* fieldSym = recType->getField(base->unresolved, false))
            if (! fieldSym->hasFlag(FLAG_PARAM) &&
                ! fieldSym->hasFlag(FLAG_TYPE_VARIABLE))
              return fieldSym;
  return NULL;
}

static VarSymbol* createFieldRef(Expr* anchor, Symbol* thisSym,
                                 Symbol* fieldSym, bool isConst)
{
  VarSymbol* fieldRef = newTemp(astr(fieldSym->name, "_ref"),
                                fieldSym->type->getRefType());
  fieldRef->qual = isConst ? QUAL_CONST_REF : QUAL_REF;
  if (isConst) fieldRef->addFlag(FLAG_CONST);
  anchor->insertBefore(new DefExpr(fieldRef));

  BlockStmt* holder = new BlockStmt();
  anchor->insertBefore(holder);
  // This is how a ref variable is currently initialized.
  CallExpr* init = new CallExpr(fieldSym->name, gMethodToken, thisSym);
  holder->insertAtTail(new CallExpr(PRIM_MOVE, fieldRef, init));
  resolveBlockStmt(holder);
  holder->flattenAndRemove();

  return fieldRef;
}

// Given a field "myField", create a shadow variable myField_SV
// whose outer variable "myField_ref" is set up prior to 'fs':
//   ref myField_ref = ovar.myField;
static ShadowVarSymbol* createSVforFieldAccess(ForallStmt* fs, Symbol* ovar,
                                               Symbol* field)
{
  bool isConst = ovar->isConstant() || field->isConstant();
  VarSymbol* fieldRef = createFieldRef(fs, ovar, field, isConst);

  // Now create the shadow variable.
  Type*           svarType   = field->type;
  ForallIntentTag svarIntent = isConst ? TFI_CONST : TFI_DEFAULT;
  bool            pruneDummy = false;
  resolveShadowVarTypeIntent(svarType, svarIntent, pruneDummy);

  ShadowVarSymbol* svar = new ShadowVarSymbol(svarIntent,
                                              astr(field->name, "_svar"),
                                              new SymExpr(fieldRef));
  svar->type = svarType;
  fs->shadowVariables().insertAtTail(new DefExpr(svar));
  handleOneShadowVar(fs, svar);

  return svar;
}

static void doConvertFieldsOfThis(ForallStmt* fs, AggregateType* recType,
                                  ShadowVarSymbol* svar, Symbol* ovar)
{
  std::map<Symbol*, ShadowVarSymbol*> fieldVars;

  // This traversal covers the loop body as well as IB/DB.
  for_SymbolSymExprs(se, svar) {
    if (Symbol* fieldSym = isFieldAccess(recType, se)) {
      ShadowVarSymbol*& fieldSV = fieldVars[fieldSym];
      if (fieldSV == NULL)
        fieldSV = createSVforFieldAccess(fs, ovar, fieldSym);
      se->parentExpr->replace(new SymExpr(fieldSV));
    }
  }

  // Remove 'svar' if it does not have any uses remaining.
  if (svar->firstSymExpr() == NULL)
    svar->defPoint->remove();
}

// See also convertFieldsOfRecordThis().
static void convertFieldsOfRecordReceiver(ForallStmt* fs) {
  // Each access to the receiver's field will reference the ArgSymbol
  // for 'this'. Which will force us to have a shadow variable for 'this'
  // and replace those ArgSymbol references with the shadow variable.
  //
  // So, the simplest way to find whether we need to do the conversion
  // is to look for the shadow variable whose outer var is 'this'.
  //
  // As a bonus, this will automatically handle the case where there are
  // multiple 'this', ex. if the enclosing method is lexically nested
  // within another method. This is perhaps useless, however, because
  // scopeResolve/insertFieldAccess() prevents that from happening.

  for_shadow_vars(svar, temp, fs)
    if (Symbol* ovar = svar->outerVarSym())
      if (AggregateType* recType = isRecordReceiver(ovar))
        doConvertFieldsOfThis(fs, recType, svar, ovar);
}

/////////////////////////////////////////////////////////////////////////////
// Do the same for task functions.
// This code is invoked from elsewhere.

// If 'fn' is nested in a method on a record, return its 'this' formal.
static ArgSymbol* enclosingRecordThisArg(FnSymbol* fn)
{
  while (FnSymbol* parent = toFnSymbol(fn->defPoint->parentSymbol))
  {
    if (ArgSymbol* thisArg = toArgSymbol(parent->_this))
      if (isRecordReceiver(thisArg))
        return thisArg;

    if (parent->hasFlag(FLAG_METHOD))
      // A method on a non-record or a weird-o lacking _this.
      return NULL;

    fn = parent;
  }

  return NULL; // not in a method
}

// This adds a new formal to 'fn' and augments all calls to 'fn'
// to pass a field access to that formal.
static ArgSymbol* createArgForFieldAccess(ArgSymbol* thisArg, FnSymbol* fn,
                                          Symbol* fieldSym)
{
  bool isConst = thisArg->isConstant() || fieldSym->isConstant();
  // Create the formal.
  IntentTag intent = isConst ? INTENT_CONST : INTENT_BLANK;
  ArgSymbol* fieldArg = new ArgSymbol(intent, astr(fieldSym->name, "_arg"),
                                      fieldSym->type);
  fn->insertFormalAtTail(fieldArg);

  // Pass an actual correspondingly.
  // Btw at this point there should be just one call to 'fn'.
  // Because it is a task function representing a syntactic task construct.
  for_SymbolSymExprs(fnse, fn) {
    CallExpr* fnCall = toCallExpr(fnse->parentExpr);
    INT_ASSERT(fnCall->baseExpr == fnse); // expect a simple call to task fn

    VarSymbol* fieldRef = createFieldRef(fnCall->getStmtExpr(), thisArg,
                                         fieldSym, isConst);
    fnCall->insertAtTail(fieldRef);
  }
  
  return fieldArg;
}

// Caller to ensure that 'fn' needs task intents.
// See also convertFieldsOfRecordReceiver().
void convertFieldsOfRecordThis(FnSymbol* fn) {
  // If we are past resolution, we might be adding task intents
  // to something that is created by the compiler, rather than
  // something that corresponds to a task construct in user code.
  // Future work: do this during createTaskFunctions().
  INT_ASSERT(! resolved);

  ArgSymbol* thisArg = enclosingRecordThisArg(fn);
  if (! thisArg) return; // not in a method on a record
  AggregateType* thisType = toAggregateType(thisArg->type->getValType());
  std::map<Symbol*, ArgSymbol*> fieldArgs;

  std::vector<SymExpr*> symExprs;
  collectSymExprsFor(fn, thisArg, symExprs);
  for_vector(SymExpr, se, symExprs)
    if (Symbol* fieldSym = isFieldAccess(thisType, se))
     {
       ArgSymbol*& fieldArg = fieldArgs[fieldSym];
       if (fieldArg == NULL)
         fieldArg = createArgForFieldAccess(thisArg, fn, fieldSym);
       se->parentExpr->replace(new SymExpr(fieldArg));
     }
}

/////////////////////////////////////////////////////////////////////////////
// setupAndResolveShadowVars() - the driver
//

void setupAndResolveShadowVars(ForallStmt* fs)
{
  // Remember the last explicit shadow variable on the list, so that
  // resolveAndPruneExplicitShadowVars() stops there and does not deal
  // with the ones added by collectAndResolveImplicitShadowVars().
  Expr* lastExplicitSVarDef = fs->shadowVariables().tail;

  //
  // Detect, create, set up and resolve implicit shadow vars.
  // Add them to fs->shadowVariables() and substitute them
  // for the outer vars in the loop body.
  //
  collectAndResolveImplicitShadowVars(fs);

  //
  // Resolve the explicit shadow variables i.e. the ones that 
  // the user wrote explicitly in the with-clause.
  // Remove the ones we want to prune.
  //
  // Note: collectAndResolveImplicitShadowVars() needs to go earlier so that
  // it does not come across the explicit shadow vars that have been pruned.
  //
  resolveAndPruneExplicitShadowVars(fs, lastExplicitSVarDef);

  //
  // In a method on a record, treat the record fields like outer variables
  // passed by the default intent.
  //
  convertFieldsOfRecordReceiver(fs);
}
