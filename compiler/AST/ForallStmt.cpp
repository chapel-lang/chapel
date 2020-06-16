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

#include "ForallStmt.h"
#include "AstVisitor.h"
#include "build.h"
#include "ForLoop.h"
#include "passes.h"
#include "stringutil.h"

/////////////////////////////////////////////////////////////////////////////
//
// ForallStmt represents a forall loop statement
//
/////////////////////////////////////////////////////////////////////////////

ForallStmt::ForallStmt(BlockStmt* body):
  Stmt(E_ForallStmt),
  fLoopBody(body),
  fZippered(false),
  fFromForLoop(false),
  fFromReduce(false),
  fOverTupleExpand(false),
  fAllowSerialIterator(false),
  fRequireSerialIterator(false),
  fVectorizationHazard(false),
  fIsForallExpr(false),
  fContinueLabel(NULL),
  fErrorHandlerLabel(NULL),
  fRecIterIRdef(NULL),
  fRecIterICdef(NULL),
  fRecIterGetIterator(NULL),
  fRecIterFreeIterator(NULL)
{
  fIterVars.parent = this;
  fIterExprs.parent = this;
  fShadowVars.parent = this;
  INT_ASSERT(fLoopBody != NULL);


  optInfo.autoLocalAccessChecked = false;

  gForallStmts.add(this);
}

ForallStmt* ForallStmt::copyInner(SymbolMap* map) {
  ForallStmt* _this  = new ForallStmt(COPY_INT(fLoopBody));
  for_alist(expr, fIterVars)
    _this->fIterVars.insertAtTail(COPY_INT(expr));
  for_alist(expr, fIterExprs)
    _this->fIterExprs.insertAtTail(COPY_INT(expr));
  for_alist(expr, fShadowVars)
    _this->fShadowVars.insertAtTail(COPY_INT(expr));

  _this->fZippered    = fZippered;
  _this->fFromForLoop = fFromForLoop;
  _this->fFromReduce  = fFromReduce;
  _this->fOverTupleExpand       = fOverTupleExpand;
  _this->fAllowSerialIterator   = fAllowSerialIterator;
  _this->fRequireSerialIterator = fRequireSerialIterator;
  _this->fVectorizationHazard   = fVectorizationHazard;
  _this->fIsForallExpr          = fIsForallExpr;
  // todo: fContinueLabel, fErrorHandlerLabel

  _this->fRecIterIRdef        = COPY_INT(fRecIterIRdef);
  _this->fRecIterICdef        = COPY_INT(fRecIterICdef);
  _this->fRecIterGetIterator  = COPY_INT(fRecIterGetIterator);
  _this->fRecIterFreeIterator = COPY_INT(fRecIterFreeIterator);

  return _this;
}

void ForallStmt::replaceChild(Expr* oldAst, Expr* newAst) {
  if (oldAst == fLoopBody)
    fLoopBody = toBlockStmt(newAst);

  else if (oldAst == fRecIterIRdef)
    fRecIterIRdef = toDefExpr(newAst);
  else if (oldAst == fRecIterICdef)
    fRecIterICdef = toDefExpr(newAst);
  else if (oldAst == fRecIterGetIterator)
    fRecIterGetIterator = toCallExpr(newAst);
  else if (oldAst == fRecIterFreeIterator)
    fRecIterFreeIterator = toCallExpr(newAst);

  else
    INT_ASSERT(false);
}

// Todo: are these checks done elsewhere?
static void verifyList(AList& list, Expr* parent) {
  INT_ASSERT(list.parent == parent);
  for_alist(expr, list) {
    INT_ASSERT(expr->list == &list);
    INT_ASSERT(expr->parentExpr == parent);
  }
}

void ForallStmt::verify() {
  Expr::verify(E_ForallStmt);

  if (!fOverTupleExpand || resolved)
    INT_ASSERT(fIterVars.length == fIterExprs.length);

  if (fZippered) INT_ASSERT(fIterVars.length > 0);
  else           INT_ASSERT(fIterVars.length == 1);

  verifyList(fIterVars, this);
  for_alist(expr, fIterVars)
    INT_ASSERT(isDefExpr(expr));

  verifyList(fIterExprs, this);

  verifyList(fShadowVars, this);
  for_alist(expr, fShadowVars) {
    DefExpr* svDef = toDefExpr(expr);
    INT_ASSERT(svDef);
    INT_ASSERT(isShadowVarSymbol(svDef->sym));
  }

  // Either all four are present or none.
  if (fRecIterIRdef != NULL) {
    INT_ASSERT(fRecIterICdef        != NULL);
    INT_ASSERT(fRecIterGetIterator  != NULL);
    INT_ASSERT(fRecIterFreeIterator != NULL);
    verifyParent(fRecIterIRdef);        verifyNotOnList(fRecIterIRdef);
    verifyParent(fRecIterICdef);        verifyNotOnList(fRecIterICdef);
    verifyParent(fRecIterGetIterator);  verifyNotOnList(fRecIterGetIterator);
    verifyParent(fRecIterFreeIterator); verifyNotOnList(fRecIterFreeIterator);
  } else {
    INT_ASSERT(fRecIterICdef        == NULL);
    INT_ASSERT(fRecIterGetIterator  == NULL);
    INT_ASSERT(fRecIterFreeIterator == NULL);
  }

  INT_ASSERT(fLoopBody);
  verifyParent(fLoopBody);
  verifyNotOnList(fLoopBody);
  // should be a normal block
  INT_ASSERT(fLoopBody->blockTag == BLOCK_NORMAL);
  INT_ASSERT(!fLoopBody->blockInfoGet());
  INT_ASSERT(!fLoopBody->isLoopStmt());
  INT_ASSERT(!fLoopBody->useList);
  INT_ASSERT(!fLoopBody->userLabel);
  INT_ASSERT(!fLoopBody->byrefVars);

  // ForallStmts are lowered away during lowerIterators().
  INT_ASSERT(!iteratorsLowered);
}

void ForallStmt::accept(AstVisitor* visitor) {
  if (visitor->enterForallStmt(this))
  {
    for_alist(expr, inductionVariables())  expr->accept(visitor);
    for_alist(expr, iteratedExpressions()) expr->accept(visitor);
    for_alist(expr, shadowVariables())     expr->accept(visitor);

    if (fRecIterIRdef)        fRecIterIRdef->accept(visitor);
    if (fRecIterICdef)        fRecIterICdef->accept(visitor);
    if (fRecIterGetIterator)  fRecIterGetIterator->accept(visitor);
    if (fRecIterFreeIterator) fRecIterFreeIterator->accept(visitor);
    
    fLoopBody->accept(visitor);

    visitor->exitForallStmt(this);
  }
}

GenRet ForallStmt::codegen() {
  INT_ASSERT(false); // this method should not be invoked
  GenRet ret;
  return ret;
}

Expr* ForallStmt::getFirstExpr() {
  return fIterVars.head->getFirstExpr();
}

Expr* ForallStmt::getNextExpr(Expr* expr) {
  if (expr == fIterVars.tail)
    return fIterExprs.head;

  if (expr == fIterExprs.tail) {
    if (Expr* sv1 = fShadowVars.head)
      return sv1->getFirstExpr();
    else if (fRecIterIRdef != NULL)
      return fRecIterIRdef->getFirstExpr();
    else
      return fLoopBody->getFirstExpr();
  }

  if (expr == fShadowVars.tail) {
    if (fRecIterIRdef != NULL)
      return fRecIterIRdef->getFirstExpr();
    else
      return fLoopBody->getFirstExpr();
  }

  // Out of these four fields, either all are present or none:
  //  fRecIterIRdef, fRecIterICdef, fRecIterGetIterator, fRecIterFreeIterator

  if (expr == fRecIterIRdef)
    return fRecIterICdef->getFirstExpr();

  if (expr == fRecIterICdef)
    return fRecIterGetIterator->getFirstExpr();

  if (expr ==fRecIterGetIterator)
    return fRecIterFreeIterator->getFirstExpr();

  if (expr == fRecIterFreeIterator)
    return fLoopBody->getFirstExpr();

  if (expr == fLoopBody)
    return this;

  INT_ASSERT(false); // should have done one of the above
  return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// helpers
/////////////////////////////////////////////////////////////////////////////

// Is 'var' listed in this's with-clause with a reduce intent?
// Used in normalize for PRIM_REDUCE_ASSIGN.
bool ForallStmt:: isReduceIntent(Symbol* var) const {
  if (ShadowVarSymbol* svar = toShadowVarSymbol(var))
    if (svar->defPoint->list == &fShadowVars)
      if (svar->isReduce())
        return true;

  return false;
}

// If there is only one induction var, treat this forall as non-zippered
// This is for the rare case that the zip tuple in "forall ... in zip() ..."
// is a 1-tuple.
// We do this before "lowering" the ForallStmt to leader+follower
// because after lowering there is always just one induction var.
void ForallStmt::setNotZippered() {
  fZippered = false;
}

// Return the nearest enclosing forall statement for 'expr', or NULL if none.
ForallStmt* enclosingForallStmt(Expr* expr) {
  for (Expr* curr = expr->parentExpr; curr; curr = curr->parentExpr)
    if (ForallStmt* fs = toForallStmt(curr))
      return fs;
  return NULL;
}

// Return a ForallStmt* if 'expr' is the DefExpr of its induction variable.
ForallStmt* isForallIterVarDef(Expr* expr) {
  if (expr->list != NULL)
    if (ForallStmt* pfs = toForallStmt(expr->parentExpr))
      if (expr->list == &pfs->inductionVariables())
        return pfs;
  return NULL;
}

// Return a ForallStmt* if 'expr' is its iterable-expression.
ForallStmt* isForallIterExpr(Expr* expr) {
  if (expr->list != NULL)
    if (ForallStmt* pfs = toForallStmt(expr->parentExpr))
      if (expr->list == &pfs->iteratedExpressions())
        return pfs;
  return NULL;
}

// Return a ForallStmt* if 'expr' is its loopBody.
ForallStmt* isForallLoopBody(Expr* expr) {
  if (ForallStmt* pfs = toForallStmt(expr->parentExpr))
    if (expr == pfs->loopBody())
      return pfs;
  return NULL;
}

// Return a const ForallStmt* if 'expr' is its loopBody.
const ForallStmt* isConstForallLoopBody(const Expr* expr) {
  if (const ForallStmt* pfs = toConstForallStmt(expr->parentExpr))
    if (expr == pfs->loopBody())
      return pfs;
  return NULL;
}

// Return a ForallStmt* if 'expr' is one of its fRecIter* helpers.
ForallStmt* isForallRecIterHelper(Expr* expr) {
  if (expr->list == NULL)
    if (ForallStmt* pfs = toForallStmt(expr->parentExpr))
      if (expr == pfs->fRecIterIRdef ||
          expr == pfs->fRecIterICdef ||
          expr == pfs->fRecIterGetIterator ||
          expr == pfs->fRecIterFreeIterator)
        return pfs;
  return NULL;
}

// Return the index variable of the parallel loop.
// Valid after addParIdxVarsAndRestruct().
VarSymbol* parIdxVar(ForallStmt* fs) {
  INT_ASSERT(fs->inductionVariables().length == 1);
  DefExpr* ivdef = fs->firstInductionVarDef();
  VarSymbol* ivsym = toVarSymbol(ivdef->sym);
  INT_ASSERT(ivsym != NULL);
  return ivsym;
}

LabelSymbol* ForallStmt::continueLabel() {
  if (fContinueLabel == NULL) {
    // We are extra-cautious here, to guard against the potential
    // that we have added code that must execute at the end of fLoopBody.
    // If this presents hardship, we can switch to always creating
    // fContinueLabel, right when the ForallStmt is created.
    INT_ASSERT(!normalized);
    fContinueLabel = new LabelSymbol("_continueLabel");
    fLoopBody->insertAtTail(new DefExpr(fContinueLabel));
  }
  return fContinueLabel;
}

/////////////////////////////////////////////////////////////////////////////
// parser support: ForallStmt::build()
/////////////////////////////////////////////////////////////////////////////

// Fill fs->iteratedExpressions(): if zippered, one element per "iterable",
// i.e. per tuple component, otherwise a single element.
static void fsDestructureIterables(ForallStmt* fs, Expr* iterator) {
  AList& fIterExprs = fs->iteratedExpressions();

  if (fs->zippered()) {
    CallExpr* itCall = toCallExpr(iterator);
    INT_ASSERT(itCall->isPrimitive(PRIM_ZIP));

    for_actuals(itActual, itCall)
      fIterExprs.insertAtTail(itActual->remove());

    // drop 'iterator' itself on the floor

  } else {
    fIterExprs.insertAtTail(iterator);
  }
}

// Is there a tuple expansion expression among fs's iterables?
static bool fsContainsTupleExpandIterables(ForallStmt* fs) {
  for_alist(it, fs->iteratedExpressions()) {
    if (CallExpr* itCall = toCallExpr(it))
      if (itCall->isPrimitive(PRIM_TUPLE_EXPAND))
        return true;
  }
  return false;
}

static void checkZipWhenOverTupleExpand(ForallStmt* fs) {
  if (fs->overTupleExpand() && !fs->zippered())
    USR_FATAL_CONT(fs, "A forall loop over tuple expansion expression(s)"
                       " must be zippered");
}

// How many iterables should we pretend that 'fs' has?
// Return 0 for a forall over tuple expansion expression(s).
// In this case, we do not know before resolution how many items the tuple
// expansion(s) will produce. So, go with however many the user provided.
static int numIterablesToDestructure(ForallStmt* fs) {
  if (fs->overTupleExpand())
    return 0;
  else
    return fs->numIteratedExprs();
}

// when there is already a Symbol
static void createAndAddIndexVar(AList& fIterVars, Symbol* idxVar) {
  idxVar->addFlag(FLAG_INDEX_VAR);
  idxVar->addFlag(FLAG_INSERT_AUTO_DESTROY);
  INT_ASSERT(idxVar->defPoint == NULL); // ensure we do not overwrite it
  fIterVars.insertAtTail(new DefExpr(idxVar));
}

// when 'name' is known
static VarSymbol* createAndAddIndexVar(AList& fIterVars, const char* name) {
  VarSymbol* idxVar = new VarSymbol(name);
  createAndAddIndexVar(fIterVars, idxVar);
  return idxVar;
}

// compiler-created, with FLAG_TEMP
static VarSymbol* createAndAddIndexVar(AList& fIterVars, int idxPosition) {
  VarSymbol* result = createAndAddIndexVar(fIterVars,
                                       astr("chpl_idx_", istr(idxPosition)));
  result->addFlag(FLAG_TEMP);
  // to guide resolveTupleIndexing(); see ex. leaderFollowerConstChecking.chpl
  // in test/performance/vectorization/vectorizeOnly/constChecking/
  result->addFlag(FLAG_INDEX_OF_INTEREST);
  return result;
}

// Support different users of fsDestructureWhenSingleIdxVar().
static inline VarSymbol* indexExprToVarSymbol(BaseAST* index) {
  if (UnresolvedSymExpr* USE = toUnresolvedSymExpr(index))
    return new VarSymbol(USE->unresolved);
  if (VarSymbol* VS = toVarSymbol(index))
    return VS;
  // Caller responsibility.
  return NULL;
}

static void fsDestructureWhenSingleIdxVar(ForallStmt* fs, AList& fIterVars,
                                          BaseAST* index, int numIterables)
{
  // This is the case like:  forall idx in zip(A,B) ...
  // i.e. zippered with a single index variable.
  // MPF suggests that we still create an index variable per iterable
  // then tuple them up into the index variable.

  // Make an exception for this case:
  if (fs->overTupleExpand()) {
    createAndAddIndexVar(fs->inductionVariables(),
                         indexExprToVarSymbol(index));
    return;
  }

  CallExpr* bt = new CallExpr("_build_tuple_always_allow_ref"); // to tuple them up

  for (int i = 1; i <= numIterables; i++) {
    VarSymbol* idx_i = createAndAddIndexVar(fIterVars, i);
    bt->insertAtTail(idx_i);
  }

  VarSymbol* idxUser = indexExprToVarSymbol(index);
  idxUser->addFlag(FLAG_INDEX_VAR);
  idxUser->addFlag(FLAG_INSERT_AUTO_DESTROY);
  DefExpr* idxDef = new DefExpr(idxUser);
  fs->loopBody()->insertAtHead(idxDef);
  idxDef->insertAfter("'move'(%S,%E)", idxUser, bt);
}

static void fsDestructureIndex(ForallStmt* fs, AList& fIterVars,
                               Expr* index, int idxNum)
{
  // Handle index expression for a single iterable.
  // Broadly, this is the same as destructureIndices() in build.cpp except:
  //  * put DefExprs on fIterVars list,
  //  * if the user specified a single index variable, that will be it
  //    rather than creating a compiler temp.

  if (UnresolvedSymExpr* indexUSE = toUnresolvedSymExpr(index)) {
    // User specified the index variable - use it.
    createAndAddIndexVar(fIterVars, indexUSE->unresolved);

  } else if (SymExpr* indexSE = toSymExpr(index)) {
    // There is already a Symbol for it - use it.
    createAndAddIndexVar(fIterVars, indexSE->symbol());

  } else {
    // We need to create an index variable and go from there.
    INT_ASSERT(isCallExpr(index)); // need to implement the other cases

    VarSymbol* idxVar = createAndAddIndexVar(fIterVars, idxNum);
    destructureIndices(fs->loopBody(), index, new SymExpr(idxVar), false);

  }
}


void fsCheckNumIdxVarsVsIterables(ForallStmt* fs, int numIdx, int numIter) {
  if (numIdx != numIter) {
    if (numIdx > numIter)
      USR_FATAL(fs, "the number of index variables in the tuple (%d)"
                " exceeds the number of zippered iterable expressions"
                " (%d)", numIdx, numIter);
    else
      USR_FATAL(fs, "not enough index variables in the tuple:"
                " %d variables vs. %d zippered iterable expressions\n"
                "  To keep all iterable expressions, add index variable(s)"
                " or '_' underscore(s) to the tuple.", numIdx, numIter);
  }
}

// Fill fs->inductionVariables(): one DefExpr per "iterable".
// If the user supplies a corresponding index variable, use that.
// Otherwise create a compiler var and detuple/aggregate into it.
static void fsDestructureIndices(ForallStmt* fs, Expr* indices) {
  const int numIterables = numIterablesToDestructure(fs);
  AList& fIterVars = fs->inductionVariables();

  if (numIterables == 1) {
    fsDestructureIndex(fs, fIterVars, indices, 1);
    INT_ASSERT(fIterVars.length == 1);
    return;
  }

  if (CallExpr* indicesCall = toCallExpr(indices)) {
    INT_ASSERT(indicesCall->isNamed("_build_tuple")); // ensured by checkIndices()

    if (numIterables == 0)
      ; // If overTupleExpand(), we will check this later during resolution.
    else
      fsCheckNumIdxVarsVsIterables(fs, indicesCall->numActuals(), numIterables);

    int idxNum = 0;
    for_actuals(index, indicesCall)
      fsDestructureIndex(fs, fIterVars, index->remove(), ++idxNum);

    // 'indicesCall' itself is dropped on the floor to be gc'ed

  } else if (UnresolvedSymExpr* indicesUSE = toUnresolvedSymExpr(indices)) {
    fsDestructureWhenSingleIdxVar(fs, fIterVars, indicesUSE, numIterables);

  } else {
    INT_ASSERT(false); // This case has not been considered.

  }
}

static void fsVerifyNumIterables(ForallStmt* fs) {
  int numIterables = fs->numIteratedExprs();
  INT_ASSERT(fs->iteratedExpressions().length == numIterables);
  if (fs->overTupleExpand()) return; // the follow condition may not hold
  INT_ASSERT(fs->inductionVariables().length == numIterables);
}

static void adjustReduceOpNames(ForallStmt* fs) {
  for_shadow_vars(sv, temp, fs)
   if (Expr* ri = sv->reduceOpExpr())
    if (UnresolvedSymExpr* sym = toUnresolvedSymExpr(ri)) {
      if (!strcmp(sym->unresolved, "max"))
        sym->unresolved = astr("MaxReduceScanOp");
      else if (!strcmp(sym->unresolved, "min"))
        sym->unresolved = astr("MinReduceScanOp");
    }
}

ForallStmt* ForallStmt::buildHelper(Expr* indices, Expr* iterator,
                                    CallExpr* intents, BlockStmt* body,
                                    bool zippered, bool fromForLoop)
{
  ForallStmt* fs = new ForallStmt(body);
  fs->fZippered    = zippered;
  fs->fFromForLoop = fromForLoop;
  body->blockTag   = BLOCK_NORMAL; // do not flatten it in cleanup(), please

  // Transfer the DefExprs of the intent variables (ShadowVarSymbols).
  if (intents) {
    while (Expr* src = intents->argList.head)
      fs->shadowVariables().insertAtTail(src->remove());
  }

  fsDestructureIterables(fs, iterator);
  fs->fOverTupleExpand = fsContainsTupleExpandIterables(fs);
  checkZipWhenOverTupleExpand(fs);
  fsDestructureIndices(fs, indices);
  fsVerifyNumIterables(fs);
  adjustReduceOpNames(fs);

  return fs;
}

BlockStmt* ForallStmt::build(Expr* indices, Expr* iterator, CallExpr* intents,
                             BlockStmt* body, bool zippered, bool serialOK)
{
  checkControlFlow(body, "forall statement");

  if (!indices)
    indices = new UnresolvedSymExpr("chpl__elidedIdx");
  checkIndices(indices);

  ForallStmt* fs = ForallStmt::buildHelper(indices, iterator, intents, body,
                                           zippered, false);
  fs->fAllowSerialIterator = serialOK;

  return buildChapelStmt(fs);
}

/////////////////////////////////////////////////////////////////////////////
// support for converting from a for loop etc.
/////////////////////////////////////////////////////////////////////////////

ForallStmt* ForallStmt::fromForLoop(ForLoop* forLoop) {
  // intended only for for-loops
  INT_ASSERT(forLoop->isForLoop());
  // conversion from zippered is not implemented
  INT_ASSERT(forLoop->zipperedGet() == false);

  ForallStmt* result = new ForallStmt(new BlockStmt());
  result->fFromForLoop = true;

  return result;
}

// "Undo" the tuple for a zippered 'fs'.
static void destructZipperedIterables(ForallStmt* fs, SymExpr* dataExpr) {
  CallExpr* zipcall = toCallExpr(getDefOfTemp(dataExpr));
  INT_ASSERT(zipcall->isPrimitive(PRIM_ZIP));
  for_actuals(actual, zipcall)
    fs->iteratedExpressions().insertAtTail(actual->remove());

  // Ensure removing zipcall is OK.
  CallExpr* move = toCallExpr(zipcall->parentExpr);
  INT_ASSERT(move->isPrimitive(PRIM_MOVE));
  move->remove();
}

ForallStmt* ForallStmt::fromReduceExpr(VarSymbol* idx, SymExpr* dataExpr,
                                       ShadowVarSymbol* svar,
                                       bool zippered, bool requireSerial)
{
  CallExpr* reduceAssign = new CallExpr(PRIM_REDUCE_ASSIGN, svar, idx);
  BlockStmt*  fsBody = new BlockStmt(reduceAssign);
  ForallStmt* result = new ForallStmt(fsBody);

  result->fZippered   = zippered;
  result->fFromReduce = true;
  result->fAllowSerialIterator = true;
  result->fIsForallExpr = true;
  result->fRequireSerialIterator = requireSerial;
  result->shadowVariables().insertAtTail(new DefExpr(svar));

  if (zippered) {
    destructZipperedIterables(result, dataExpr);
    fsDestructureWhenSingleIdxVar(result, result->inductionVariables(),
                                  idx, result->numIteratedExprs());
  } else {
    idx->addFlag(FLAG_INDEX_VAR);
    idx->addFlag(FLAG_INSERT_AUTO_DESTROY); // how about the zippered case?
    result->inductionVariables().insertAtTail(new DefExpr(idx));
    result->iteratedExpressions().insertAtTail(dataExpr);
  }

  return result;
}


// vectorization
bool ForallStmt::hasVectorizationHazard() const {
  return fVectorizationHazard;
}

void ForallStmt::setHasVectorizationHazard(bool v) {
  fVectorizationHazard = v;
}

bool ForallStmt::isForallExpr() const {
  return fIsForallExpr;
}

static void gatherFollowerLoopBodies(BlockStmt* block,
                                     std::vector<BlockStmt*>& bodies) {
  for_alist(stmt, block->body) {
    if (ForLoop* forLoop = toForLoop(stmt)) {
      if (SymExpr* indexSe = forLoop->indexGet())
        if (indexSe->symbol()->hasFlag(FLAG_FOLLOWER_INDEX))
          bodies.push_back(forLoop);
    } else if (BlockStmt* inner = toBlockStmt(stmt)) {
      if (inner->isRealBlockStmt())
        gatherFollowerLoopBodies(inner, bodies);
    }
  }
}

std::vector<BlockStmt*> ForallStmt::loopBodies() const {
  std::vector<BlockStmt*> bodies;

  // First, check for follower loops with fast follower check.
  for_alist(stmt, fLoopBody->body) {
    if (CondStmt* cond = toCondStmt(stmt)) {
      gatherFollowerLoopBodies(cond->thenStmt, bodies);
      if (cond->elseStmt)
        gatherFollowerLoopBodies(cond->elseStmt, bodies);
    }
  }
  if (bodies.size() == 0) {
    // No fast follower check. Try a fast follower loop.
    gatherFollowerLoopBodies(fLoopBody, bodies);
  }

  // With the fast follower check, we might have 2 bodies,
  // but it's probably an error if we find more than that.
  INT_ASSERT(bodies.size() <= 2);

  if (bodies.size() == 0) {
    // No follower loops, must be standalone pattern, just use normal body
    bodies.push_back(fLoopBody);
  }
  return bodies;
}
