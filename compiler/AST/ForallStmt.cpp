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

#include "ForallStmt.h"
#include "AstVisitor.h"
#include "build.h"
#include "foralls.h"
#include "passes.h"
#include "stringutil.h"

/////////////////////////////////////////////////////////////////////////////
//
// ForallIntent represents one forall intent in a with clause
//
// This is a transitional step on the way to LoopIntentVar.
// We also plan to retire BlockStmt::forallIntents and ForallIntents class
// by switching ALL forall statements (and the likes) to new representation.
//
/////////////////////////////////////////////////////////////////////////////

ForallIntent::ForallIntent(TFITag intent, Expr* var, Expr* reduceExpr):
  Expr(E_ForallIntent),
  fiIntent(intent),
  fiVar(var),
  riSpec(reduceExpr)
{
  gForallIntents.add(this);
}

ForallIntent* ForallIntent::copyInner(SymbolMap* map) {
  ForallIntent* _this = new ForallIntent(fiIntent,
                                         COPY_INT(fiVar),
                                         COPY_INT(riSpec));
  return _this;
}

void ForallIntent::replaceChild(Expr* oldAst, Expr* newAst) {
  if (oldAst == fiVar)
    fiVar = newAst;
  else if (oldAst == riSpec)
    riSpec = newAst;
  else
    INT_FATAL(this, "Unexpected case in ForallIntent::replaceChild");
}

Expr* ForallIntent::getFirstChild() {
  return fiVar;
}

Expr* ForallIntent::getFirstExpr() {
  return fiVar->getFirstExpr();
}

Expr* ForallIntent::getNextExpr(Expr* expr) {
  if (expr == fiVar && riSpec)
    return riSpec;
  else
    return this;
}

void ForallIntent::verify() {
  Expr::verify(E_ForallIntent);
  verifyParent(fiVar);
  verifyParent(riSpec);
  bool isRI = isReduce();
  INT_ASSERT(isRI == (fiIntent == TFI_REDUCE));
  INT_ASSERT(isRI == (riSpec != NULL));
}

GenRet ForallIntent::codegen() {
  INT_ASSERT(false); // this method should not be invoked
  GenRet ret;
  return ret;
}

void ForallIntent::accept(AstVisitor* visitor) {
  if (visitor->enterForallIntent(this)) {
    variable()->accept(visitor);
    if (Expr* ri = reduceExpr())
      ri->accept(visitor);
    visitor->exitForallIntent(this);
  }
}


/////////////////////////////////////////////////////////////////////////////
//
// ForallStmt represents a forall loop statement
//
/////////////////////////////////////////////////////////////////////////////

ForallStmt::ForallStmt(bool zippered, BlockStmt* body):
  Stmt(E_ForallStmt),
  fZippered(zippered),
  fLoopBody(body)
{
  fIterVars.parent = this;
  fIterExprs.parent = this;
  fIntentVars.parent = this;
  fFIntents.parent = this;
  gForallStmts.add(this);
}

ForallStmt* ForallStmt::copyInner(SymbolMap* map) {
  ForallStmt* _this  = new ForallStmt(fZippered,
                                      COPY_INT(fLoopBody));
  for_alist(expr, fIterVars)
    _this->fIterVars.insertAtTail(COPY_INT(expr));
  for_alist(expr, fIterExprs)
    _this->fIterExprs.insertAtTail(COPY_INT(expr));
  for_alist(expr, fIntentVars)
    _this->fIntentVars.insertAtTail(COPY_INT(expr));
  for_alist(expr, fFIntents)
    _this->fFIntents.insertAtTail(COPY_INT(expr));

  return _this;
}

void ForallStmt::replaceChild(Expr* oldAst, Expr* newAst) {
  if (oldAst == fLoopBody) {
    if (!newAst)
      fLoopBody = NULL;
    else if (BlockStmt* newBlock = toBlockStmt(newAst))
      fLoopBody = newBlock;
    else
      // It is caller responsibility to make newAst fit.
      INT_ASSERT(false);

  } else {
    // We did not find oldAst in our ForallStmt.
    INT_ASSERT(false);
  }
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

  INT_ASSERT(fIterVars.length == fIterExprs.length);
  if (fZippered) INT_ASSERT(fIterVars.length > 0);
  else           INT_ASSERT(fIterVars.length == 1);

  verifyList(fIterVars, this);
  verifyList(fIterExprs, this);
  verifyList(fIntentVars, this);
  for_alist(expr, fIterVars)
    INT_ASSERT(isDefExpr(expr));
  for_alist(expr, fIntentVars)
    INT_ASSERT(isDefExpr(expr));
  for_alist(expr, fIntentVars)
    INT_ASSERT(isForallIntent(expr));

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
  INT_ASSERT(!fLoopBody->forallIntents);

  // Currently ForallStmt are gone during resolve().
  INT_ASSERT(!resolved);
}

void ForallStmt::accept(AstVisitor* visitor) {
  if (visitor->enterForallStmt(this)) {
    for_alist(expr, inductionVariables())
      expr->accept(visitor);
    for_alist(expr, iteratedExpressions())
      expr->accept(visitor);
    for_alist(expr, intentVariables())
      expr->accept(visitor);
    fLoopBody->accept(visitor);
    visitor->exitForallStmt(this);
  }
}

GenRet ForallStmt::codegen() {
  INT_ASSERT(false); // this method should not be invoked
  GenRet ret;
  return ret;
}

Expr* ForallStmt::getFirstChild() {
  return fIterVars.head;
}

Expr* ForallStmt::getFirstExpr() {
  if (Expr* iv = fIterVars.head->getFirstExpr())
    return iv;
  else if (Expr* ie = fIterExprs.head->getFirstExpr())
    return ie;

  // we must have found something
  INT_ASSERT(false);
  return NULL; //dummy
}

Expr* ForallStmt::getNextExpr(Expr* expr) {
  if (expr == fIterVars.tail)
    return fIterExprs.head;

  if (expr == fIterExprs.tail) {
    if (Expr* inv = fIntentVars.head)
      return inv;
    else
      return fLoopBody->getFirstExpr();
  }

  if (expr == fIntentVars.tail)
    return fLoopBody->getFirstExpr();

  return this;
}

/////////////////////////////////////////////////////////////////////////////
// helpers
/////////////////////////////////////////////////////////////////////////////

// Is 'expr' an iterable-expression for 'this' ?
bool ForallStmt::isIteratedExpression(Expr* expr) {
  return expr->list && expr->list == &fIterExprs;
}

// If 'var' is listed in this's with-clause with a reduce intent,
// return its position in the with-clause, otherwise return -1.
// Used in preFold for PRIM_REDUCE_ASSIGN, set up in normalize.
int ForallStmt::reduceIntentIdx(Symbol* var) {
  int idx = 0;
  for_forall_intents(fi, temp, this) {
    idx++;
    if (fi->isReduce())
      if (SymExpr* fiVarSE = toSymExpr(fi->variable()))
        if (fiVarSE->symbol() == var)
          return idx;
  }

  // Did not see 'var' with a reduce intent.
  return -1;
}

// Return the enclosing forall statement for 'expr', or NULL if none.
ForallStmt* enclosingForallStmt(Expr* expr) {
  for (Expr* curr = expr->parentExpr; curr; curr = curr->parentExpr)
    if (ForallStmt* fs = toForallStmt(curr))
      return fs;
  return NULL;
}

// valid after addParIdxVarsAndRestruct()
VarSymbol* parIdxVar(const ForallStmt* fs) {
  DefExpr* def = toDefExpr(fs->loopBody()->body.head);
  VarSymbol* result = toVarSymbol(def->sym);
  INT_ASSERT(result && !strcmp(result->name, "chpl__parIdx"));
  return result;
}

// ditto
VarSymbol* parIdxCopyVar(const ForallStmt* fs) {
  DefExpr* def = toDefExpr(fs->loopBody()->body.head->next);
  VarSymbol* result = toVarSymbol(def->sym);
  INT_ASSERT(result && !strcmp(result->name, "chpl__parIdxCopy"));
  return result;
}

// ditto
BlockStmt* userLoop(const ForallStmt* fs) {
  BlockStmt* ul = toBlockStmt(fs->loopBody()->body.tail);
  INT_ASSERT(ul);
  return ul;
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

// when 'name' is known
static VarSymbol* createAndAddIndexVar(AList& fIterVars, const char* name) {
  VarSymbol* idxVar = new VarSymbol(name);
  idxVar->addFlag(FLAG_INDEX_VAR);
  idxVar->addFlag(FLAG_INSERT_AUTO_DESTROY);
  fIterVars.insertAtTail(new DefExpr(idxVar));
  return idxVar;
}

// compile-created, with FLAG_TEMP
static VarSymbol* createAndAddIndexVar(AList& fIterVars, int idxPosition) {
  VarSymbol* result = createAndAddIndexVar(fIterVars,
                                       astr("chpl_idx_", istr(idxPosition)));
  result->addFlag(FLAG_TEMP);
  // to guide resolveTupleIndexing(); see ex. leaderFollowerConstChecking.chpl
  // in test/performance/vectorization/vectorizeOnly/constChecking/
  result->addFlag(FLAG_INDEX_OF_INTEREST);
  return result;
}

static void fsDestructureWhenSingleIdxVar(ForallStmt* fs, AList& fIterVars,
                                 UnresolvedSymExpr* index, int numIterables)
{
  // This is the case like:  forall idx in zip(A,B) ...
  // i.e. zippered with a single index variable.
  // MPF suggests that we still create an index variable per iterable
  // then tuple them up into the index variable.

  CallExpr* bt = new CallExpr("_build_tuple_always_allow_ref"); // to tuple them up

  for (int i = 1; i <= numIterables; i++) {
    VarSymbol* idx_i = createAndAddIndexVar(fIterVars, i);
    bt->insertAtTail(idx_i);
  }

  VarSymbol* idxUser = new VarSymbol(index->unresolved);
  idxUser->addFlag(FLAG_INDEX_VAR);
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

  } else {
    // We need to create an index variable and go from there.
    INT_ASSERT(isCallExpr(index)); // need to implement the other cases

    VarSymbol* idxVar = createAndAddIndexVar(fIterVars, idxNum);
    destructureIndices(fs->loopBody(), index, new SymExpr(idxVar), false);

  }
}


// Fill fs->inductionVariables(): one DefExpr per "iterable".
// If the user supplies a corresponding index variable, use that.
// Otherwise create a compiler var and detuple/aggregate into it.
static void fsDestructureIndices(ForallStmt* fs, Expr* indices) {
  const int numIterables = fs->numIteratedExprs();
  AList& fIterVars = fs->inductionVariables();

  if (numIterables == 1) {
    fsDestructureIndex(fs, fIterVars, indices, 1);
    INT_ASSERT(fIterVars.length == 1);
    return;
  }

  INT_ASSERT(numIterables > 1); // should not have 0 iterables

  if (CallExpr* indicesCall = toCallExpr(indices)) {
    INT_ASSERT(indicesCall->isNamed("_build_tuple")); // ensured by checkIndices()

    if (indicesCall->numActuals() != numIterables) {
      if (indicesCall->numActuals() > numIterables)
        USR_FATAL(fs, "the number of index variables in the tuple (%d)"
                  " exceeds the number of zippered iterable expressions"
                  " (%d)", indicesCall->numActuals(), numIterables);
      else
        USR_FATAL(fs, "not enough index variables in the tuple:"
                  " %d variables vs. %d zippered iterable expressions;"
                  " to keep all iterable expressions, add index variable(s)"
                  " or '_' underscore(s) to the tuple",
                  indicesCall->numActuals(), numIterables);
    }
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
  INT_ASSERT(fs->inductionVariables().length == numIterables);
  INT_ASSERT(fs->iteratedExpressions().length == numIterables);
}

static void adjustReduceOpNames(ForallStmt* fs) {
  for_forall_intents(fi, temp, fs)
   if (Expr* ri = fi->reduceExpr())
    if (UnresolvedSymExpr* sym = toUnresolvedSymExpr(ri)) {
      if (!strcmp(sym->unresolved, "max"))
        sym->unresolved = astr("MaxReduceScanOp");
      else if (!strcmp(sym->unresolved, "min"))
        sym->unresolved = astr("MinReduceScanOp");
    }
}

BlockStmt* ForallStmt::build(Expr* indices, Expr* iterator,
                             ForallIntents* forall_intents,
                             BlockStmt* body, bool zippered)
{
  if (!indices)
    indices = new UnresolvedSymExpr("chpl__elidedIdx");
  checkIndices(indices);

  ForallStmt* fs = new ForallStmt(zippered, body);

  if (forall_intents) {
    // transfer from 'forall_intents' into 'fs'
    // todo: how to make the parser produce AList directly?
    AList& fis = fs->forallIntents();
    for (int i = 0; i < forall_intents->numVars(); i++)
      fis.insertAtTail(new ForallIntent(forall_intents->fIntents[i],
                                        forall_intents->fiVars[i],
                                        forall_intents->riSpecs[i]));
    delete forall_intents;
  }

  fsDestructureIterables(fs, iterator);
  fsDestructureIndices(fs, indices);
  fsVerifyNumIterables(fs);

  adjustReduceOpNames(fs);
  body->blockTag = BLOCK_NORMAL; // do not flatten it in cleanup(), please

  return buildChapelStmt(fs);
}
