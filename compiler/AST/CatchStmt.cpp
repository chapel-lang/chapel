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

#include "CatchStmt.h"
#include "AstVisitor.h"

CatchStmt* CatchStmt::build(BlockStmt* body) {
  return new CatchStmt(NULL, body);
}

CatchStmt* CatchStmt::build(Expr* expr, BlockStmt* body) {
  return new CatchStmt(expr, body);
}

CatchStmt::CatchStmt(Expr* expr, BlockStmt* body) : Stmt(E_CatchStmt) {
  _body = new BlockStmt();

  if (expr != NULL)
    _body->insertAtTail(expr);

  _body->insertAtTail(body);

  gCatchStmts.add(this);
}

CatchStmt::~CatchStmt() {

}

// returns null if this is a catch-all
DefExpr* CatchStmt::expr() const {
  return toDefExpr(_body->body.first());
}

BlockStmt* CatchStmt::body() const {
  return toBlockStmt(_body->body.last());
}

void CatchStmt::accept(AstVisitor* visitor) {
  if (visitor->enterCatchStmt(this)) {
    if (DefExpr* e = expr())
      e->accept(visitor);

    if (BlockStmt* b = body())
      b->accept(visitor);

    visitor->exitCatchStmt(this);
  }
}

CatchStmt* CatchStmt::copyInner(SymbolMap* map) {
  return new CatchStmt(COPY_INT(expr()), COPY_INT(body()));
}

void CatchStmt::replaceChild(Expr* old_ast, Expr* new_ast) {
  if (_body == old_ast) {
    _body = toBlockStmt(new_ast);
  }
}

Expr* CatchStmt::getFirstChild() {
  return _body;
}

Expr* CatchStmt::getFirstExpr() {
  if (_body) {
    return _body->getFirstExpr();
  }
  return NULL;
}

void CatchStmt::verify() {
  Stmt::verify();

  if (astTag != E_CatchStmt) {
    INT_FATAL(this, "CatchStmt::verify. Bad astTag");
  }

  if (!_body) {
    INT_FATAL(this, "CatchStmt::verify. _body is missing");
  }

  if (!body()) {
    INT_FATAL(this, "CatchStmt::verify. Invalid catch body");
  }
}

GenRet CatchStmt::codegen() {
  INT_FATAL("CatchStmt should be removed before codegen");
  GenRet ret;
  return ret;
}
