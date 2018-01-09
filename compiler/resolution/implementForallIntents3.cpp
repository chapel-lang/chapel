#include "AstVisitorTraverse.h"
#include "ForallStmt.h"
#include "implementForallIntents.h"

/////////// forwards ///////////

class ExpandVisitor;
static void expandYield( ExpandVisitor* EV, CallExpr* yield);
static void expandTaskFn(ExpandVisitor* EV, CallExpr* call, FnSymbol* taskFn);
static void expandForall(ExpandVisitor* EV, ForallStmt* fs);


/////////// ExpandVisitor visitor ///////////

class ExpandVisitor : public AstVisitorTraverse {
public:
  ForallStmt* const forall;
  FnSymbol* const parIter; // vass needed?
  SymbolMap& svar2clonevar;
  TaskFnCopyMap& taskFnCopies;  // like in expandBodyForIteratorInline()
  bool breakOnYield; // vass for debugging

  ExpandVisitor(ForallStmt* fs, FnSymbol* parIterArg,
                SymbolMap& map, TaskFnCopyMap& taskFnCopiesArg);

  ExpandVisitor(ExpandVisitor* parentEV,
                SymbolMap& map);

  virtual bool enterCallExpr(CallExpr* node) {
    if (node->isPrimitive(PRIM_YIELD)) {
      expandYield(this, node);
    }
    else if (FnSymbol* taskFn = resolvedToTaskFun(node)) {
      expandTaskFn(this, node, taskFn);
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

ExpandVisitor::ExpandVisitor(ForallStmt* fs, FnSymbol* parIterFn,
                             SymbolMap& map, TaskFnCopyMap& taskFnCps) :
  forall(fs),
  parIter(parIterFn),
  svar2clonevar(map),
  taskFnCopies(taskFnCps)
{
}

ExpandVisitor::ExpandVisitor(ExpandVisitor* parentEV,
                             SymbolMap& map) :
  forall(parentEV->forall),
  parIter(parentEV->parIter),
  svar2clonevar(map),
  taskFnCopies(parentEV->taskFnCopies)
{
}


/////////// misc helpers ///////////

// Cf. copyBody() for inlineFunctions().
static BlockStmt* copyBodyPI(FnSymbol* iterFn, CallExpr* iterCall,
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

  return retval;
}

/* Do we want to make this available to all compiler code?
MF says "It should:
 * check isUserDefinedRecord and possibly is POD
 * error if a user-defined record has no auto copy
 * use PRIM_MOVE for non-user defined records
 * use autoCopy if one is defined, even for if isUserDefinedRecord returned false
Todo: should it support the case of initializing a 'ref' variable?
*/
//
// Initialize 'dest' from 'init', which can be either a symbol or an expr.
// Insert the initialization code before 'anchor'.
//
static void insertInitialization(Symbol* dest, Type* valType,
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

/* vass - need this?
// same as intentArgName()
static const char* shadowVarName(int ix, const char* base) {
  return astr("_x", istr(ix+1), "_", base);
}
*/


/////////// expandYield ///////////

// Replace 'yield' with a clone of the forall loop body.
// Recurse into the cloned body.
static void expandYield(ExpandVisitor* EV, CallExpr* yieldCall)
{
  if (EV->breakOnYield) gdbShouldBreakHere(); 

  // This is svar2clonevar plus a clone of the induction variable.
  SymbolMap map;
  map.copy(EV->svar2clonevar);  // vass is this the right way to copy a map?

  // Clone the index variable for use in the cloned body.
  // There is only one idx var because all zippering has been lowered away.
  VarSymbol* origIdxVar = EV->forall->singleInductionVar();
  INT_ASSERT(map.get(origIdxVar) == NULL); //vass remove at end
  VarSymbol* cloneIdxVar = origIdxVar->copy(&map);
  INT_ASSERT(map.get(origIdxVar) == cloneIdxVar); //vass remove at end
  yieldCall->insertBefore(new DefExpr(cloneIdxVar));

  // Todo should insertInitialization() handle both cases?
  Expr* yieldExpr = yieldCall->get(1)->remove();
  if (cloneIdxVar->isRef())
    yieldCall->insertBefore(new CallExpr(PRIM_SET_REFERENCE,
                                         cloneIdxVar, yieldExpr));
  else
    insertInitialization(cloneIdxVar, cloneIdxVar->type, yieldExpr, yieldCall);

/* vass remove me:
Move the yielded value to a clone of the index variable.
Also need to map the index variable to the yield value.
  SymExpr* yieldSE = toSymExpr(yieldCall->get(1));
  INT_ASSERT(yieldSE != NULL); // need more work otherwise
  // There should not be any zippering.

  map.put(idxVar, yieldSE->symbol());
*/

  BlockStmt* bodyClone = EV->forall->loopBody()->copy(&map);
  yieldCall->replace(bodyClone);

  // A yield stmt does not create any parallelism, so use the same visitor.
  bool breakOnYieldPrev = EV->breakOnYield;
  EV->breakOnYield = true;
  bodyClone->accept(EV);
  EV->breakOnYield = breakOnYieldPrev;
}


/////////// expandTaskFn ///////////

static void expandTaskFn(ExpandVisitor* EV, CallExpr* call, FnSymbol* taskFn)
{
  FnSymbol* cloneTaskFn = EV->taskFnCopies.get(taskFn);
  bool expandClone = false;

  if (cloneTaskFn == NULL) {
    // Follow the cloning steps in expandBodyForIteratorInline().

    // This holds because we flatten everything right away.
    // We need it so that we can place the def of 'fcopy' anywhere
    // while preserving correct scoping of its SymExprs.
    INT_ASSERT(isGlobal(taskFn));

    cloneTaskFn = taskFn->copy();
    EV->taskFnCopies.put(taskFn, cloneTaskFn);

    if (!preserveInlinedLineNumbers)
      reset_ast_loc(cloneTaskFn, call);

    // Note that 'fcopy' will likely get a copy of 'body',
    // so we need to preserve correct scoping of its SymExprs.
    call->insertBefore(new DefExpr(cloneTaskFn));

    // We will need to process the clone.
    expandClone = true;
  }

  call->baseExpr->replace(new SymExpr(cloneTaskFn));

  // These are the variables in effect within the cloned taskFn, if expandClone.
  SymbolMap map;

  int numOrigActuals = call->numActuals();
  int idx = 0;
  for_shadow_vars(svar, temp1, EV->forall) {
    idx++;
    Symbol* eActual = svar->outerVarSym();
    call->insertAtTail(eActual);

    if (expandClone) {
      // vass todo handle reduce intent
      ArgSymbol* eFormal = newExtraFormal(svar, idx, eActual,
                                          false/*vass-nested*/);
      cloneTaskFn->insertFormalAtTail(eFormal);
      map.put(svar, eFormal);
    }
    else {
      ArgSymbol* eFormal = cloneTaskFn->getFormal(numOrigActuals+idx);
      if (eFormal->hasFlag(FLAG_REF_TO_IMMUTABLE))
        // Ensure this flag is correct for all calls. See newExtraFormal().
        INT_ASSERT(eActual->isConstValWillNotChange() ||
                   eActual->hasFlag(FLAG_REF_TO_IMMUTABLE));
    }
  }

  if (expandClone) {
    ExpandVisitor taskFnV(EV, map);
    cloneTaskFn->body->accept(&taskFnV);

    flattenNestedFunction(cloneTaskFn);
  }

  // todo: addDummyErrorArgumentToCall() ?
}

/////////// expandForall ///////////

// Todo implement this when the startup/teardown feature comes in.
static bool inTaskStartupTeardownCode(ForallStmt* fs) { return false; }

static void expandForall(ExpandVisitor* EV, ForallStmt* fs)
{
  if (ForallStmt* efs = enclosingForallStmt(EV->forall))
    // If there are enclosing forall loop(s),
    // we need to handle their intents as well. Currently we don't.
    // It is OK for a FS to be in task startup/teardown code.
    INT_ASSERT(inTaskStartupTeardownCode(efs));
  
}


/////////// outermost visitor ///////////

// Adds code to init+deinit the local op and the reduction state.
// Updates 'map' with svar -> reduction state var.
static void setupForReduceIntent(ForallStmt* fs, ShadowVarSymbol* svar,
                                 int ix, SymbolMap& map,
                                 Expr* aInit, Expr* aFini)
{
  INT_FATAL("vass TODO");

/*
NEW INSIGHT: just put everything in taskInit/Deinit blocks
at resolution, so all we need to do now is to clone those blocks.
*/

/* vass TODO

  Symbol* parentOp = svar->outerVarSym();

* add computation of reduceCurrOp and reduceShadowVar before aInit
* add tear-down after aFini
  // See ensureCurrentReduceOpForReduceIntent(), shadowVarForReduceIntent().
  // Todo optimize: reuse 'parentOp' if we are outside task fns, foralls.

For that, add fields to ShadowVarSymbol during resolution,
fill them out, update as they are being cloned.

* map.put(svar, reduceShadowVar)

*/
}

// 'ibody' is a clone of the parallel iterator body
// We are replacing the ForallStmt with this clone.
static void setupOuterMap(ExpandVisitor* outerVis,
                          BlockStmt* iwrap, BlockStmt* ibody)
{
  INT_ASSERT(ibody->inTree()); //fyi

/* vass - use these?
  // Place initialization and finalization code before these anchors.
  CallExpr* aInit = new CallExpr("anchorInit");
  CallExpr* aFini = new CallExpr("anchorFini");
  iwrap->insertAtHead(aInit);
  iwrap->insertAtTail(aFini);
*/

  // Place initialization code before aInit, finalization code after aFini.
  Expr* aInit = ibody;
  Expr* aFini = ibody;

  // When cloning the loop body of 'fs', replace each shadow variable
  // with the variable given by 'map'.
  SymbolMap& map = outerVis->svar2clonevar;
  int idx = 0;
  for_shadow_vars(svar, temp1, outerVis->forall) {
    idx++;
    switch (svar->intent) {
      case TFI_DEFAULT:
      case TFI_CONST:
        INT_ASSERT(false);   // don't give me an abstract intent
        break;

      case TFI_IN:
      case TFI_CONST_IN:
        {
          bool isconst = svar->intent == TFI_CONST_IN;
          VarSymbol* bodyvar = new VarSymbol(svar->name);
          bodyvar->type = svar->type->getValType();
          bodyvar->qual = isconst ? QUAL_CONST_VAL : QUAL_VAL;
          aInit->insertBefore(new DefExpr(bodyvar));

          // Initialize it from the outer var.
          insertInitialization(bodyvar, bodyvar->type,
                               svar->outerVarSym(), aInit);

          // Deinitialize it at the end.
          if (FnSymbol* autoDestroy = getAutoDestroy(bodyvar->type))
            aFini->insertAfter(new CallExpr(autoDestroy, bodyvar));

          map.put(svar, bodyvar);
        }
        break;

      case TFI_REF:
      case TFI_CONST_REF:
        // Let us reference the outer variable directly, for simplicity.
        // NB we are not concerned with const checking any more.
        // vass todo this does not transfer across (outlined) task functions.
        map.put(svar, svar->outerVarSym());
        break;

      case TFI_REDUCE:
        setupForReduceIntent(outerVis->forall, svar, idx, map, aInit, aFini);
        break;
    }
  }
}


/////////// main driver ///////////

static void lowerForallStmtsInline() {
  forv_Vec(ForallStmt, fs, gForallStmts)
  {
    if (fs->id == breakOnResolveID) gdbShouldBreakHere(); //vass

    // If this fails, need to filter out those FSes.
    // We lower and remove each FS from the tree *after* we see it here.
    INT_ASSERT(fs->inTree() && fs->getFunction()->isResolved());

    // We have converted zippering, if any, to a follower loop.
    INT_ASSERT(fs->numIteratedExprs() == 1);
    CallExpr* parIterCall = toCallExpr(fs->firstIteratedExpr());
    
    FnSymbol* parIterFn = parIterCall->resolvedFunction();
    // Make sure it is a parallel iterator.
    INT_ASSERT(parIterFn->hasFlag(FLAG_INLINE_ITERATOR));
    // We don't know yet what to do with these.
    INT_ASSERT(!parIterFn->hasFlag(FLAG_RECURSIVE_ITERATOR));

    // Place to put pre- and post- code.
    SET_LINENO(fs); // vass what should it be?
    CallExpr*  ianch = new CallExpr("anchor");
    BlockStmt* iwrap = new BlockStmt(ianch);

    // Clone the iterator body.
    // Cf. expandIteratorInline() and inlineCall().
    BlockStmt* ibody = copyBodyPI(parIterFn, parIterCall, ianch);
    
    // Let us remove 'fs' later, for debugging convenience.
    fs->insertAfter(iwrap);
    ianch->replace(ibody);

    TaskFnCopyMap   taskFnCopies;
    SymbolMap       map;
    ExpandVisitor   outerVis(fs, parIterFn, map, taskFnCopies);
    setupOuterMap(&outerVis, iwrap, ibody);
    ibody->accept(&outerVis);

    fs->remove();
    // We could also do {iwrap,ibody}->flattenAndRemove().
  }

  //vass todo remove parallel iterators themselves
  // now that we have inlined them.
}
