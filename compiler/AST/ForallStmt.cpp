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
#include "passes.h"

/////////////////////////////////////////////////////////////////////////////
//
// ForallStmt represents a forall loop statement
//
/////////////////////////////////////////////////////////////////////////////

ForallStmt::ForallStmt(bool zippered, BlockStmt* body, ForallIntents* with):
  Stmt(E_ForallStmt),
  fZippered(zippered),
  fLoopBody(body),
  fWith(with)
{
  fIterVars.parent = this;
  fIterExprs.parent = this;
  fIntentVars.parent = this;
  gForallStmts.add(this);
}

ForallStmt::~ForallStmt() {
  delete fWith;
}

ForallStmt* ForallStmt::copyInner(SymbolMap* map) {
  ForallStmt* _this  = new ForallStmt(fZippered,
                                      COPY_INT(fLoopBody),
                                      COPY_INT(fWith));
  for_alist(expr, fIterVars)
    _this->fIterVars.insertAtTail(COPY_INT(expr));
  for_alist(expr, fIterExprs)
    _this->fIterExprs.insertAtTail(COPY_INT(expr));
  for_alist(expr, fIntentVars)
    _this->fIntentVars.insertAtTail(COPY_INT(expr));

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

  } else if (fWith->replaceChildFI(oldAst, newAst)) {
      // replaceChildFI took care of it

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

  INT_ASSERT(fWith);

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
    withClause()->acceptFI(visitor); // aka visitor->visitForallIntents(fWith);
    fLoopBody->accept(visitor);
    visitor->exitForallStmt(this);
  }
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

  // Should we also descend into fWith?

  if (expr == fIntentVars.tail)
    return fLoopBody->getFirstExpr();

  return this;
}

// Is 'expr' an iterable-expression for 'this' ?
bool ForallStmt::isIteratedExpression(Expr* expr) {
  return expr->list && expr->list == &fIterExprs;
}

// Return the enclosing forall statement for 'expr', or NULL if none.
ForallStmt* enclosingForallStmt(Expr* expr) {
  for (Expr* curr = expr->parentExpr; curr; curr = curr->parentExpr)
    if (ForallStmt* fs = toForallStmt(curr))
      return fs;
  return NULL;
}

// If 'var' is listed in this's with-clause with a reduce intent,
// return its position in the with-clause, otherwise return -1.
int ForallStmt::reduceIntentIdx(Symbol* var) {
  ForallIntents* fi = withClause();
  int nv = fi->numVars();
  for (int i = 0; i < nv; i++)
    if (fi->isReduce(i))
      if (SymExpr* varSE = toSymExpr(fi->fiVars[i]))
        if (varSE->symbol() == var)
          return i;

  // Did not see 'var' with a reduce intent.
  return -1;
}

GenRet ForallStmt::codegen() {
  INT_ASSERT(false); // should not be invoked
  GenRet ret;
  return ret;
}

BlockStmt* ForallStmt::build(Expr* indices, Expr* iterator, ForallIntents* fi,
                             BlockStmt* body, bool zippered) {
  // TODO
  return (BlockStmt*)NULL;
}
