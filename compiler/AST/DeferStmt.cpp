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

#include "DeferStmt.h"

#include "AstVisitor.h"
#include "AstVisitorTraverse.h"
#include "CatchStmt.h"

BlockStmt* DeferStmt::build(BlockStmt* body) {
  return buildChplStmt(new DeferStmt(body));
}

BlockStmt* DeferStmt::buildChplStmt(Expr* expr) {
  return new BlockStmt(expr, BLOCK_SCOPELESS);
}

DeferStmt::DeferStmt(BlockStmt* body)
  : Stmt(E_DeferStmt),
   _body(body) {

  gDeferStmts.add(this);
}

DeferStmt::~DeferStmt() {

}

BlockStmt* DeferStmt::body() const {
  return _body;
}

void DeferStmt::accept(AstVisitor* visitor) {
/*  if (visitor->enterDeferStmt(this)) {
    if (_body) {
      _body->accept(visitor);
    }

    visitor->exitDeferStmt(this);
  }*/
}

DeferStmt* DeferStmt::copyInner(SymbolMap* map) {
  DeferStmt* copy = new DeferStmt(COPY_INT(_body));
  return copy;
}

void DeferStmt::replaceChild(Expr* oldAst, Expr* newAst) {
  if (oldAst == _body) {
    _body = toBlockStmt(newAst);
  }
}

Expr* DeferStmt::getFirstChild() {
  return _body;
}

Expr* DeferStmt::getFirstExpr() {
  if (_body) {
    return _body->getFirstExpr();
  }
  return NULL;
}

Expr* DeferStmt::getNextExpr(Expr* expr) {
  return this;
}

void DeferStmt::verify() {
  Stmt::verify(E_DeferStmt);

  if (!_body) {
    INT_FATAL(this, "DeferStmt::verify. _body is missing");
  }
}

GenRet DeferStmt::codegen() {
  INT_FATAL("DeferStmt should be removed before codegen");
  GenRet ret;
  return ret;
}
