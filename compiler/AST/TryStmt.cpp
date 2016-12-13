/*
 * Copyright 2004-2016 Cray Inc.
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

#include "TryStmt.h"
#include "AstVisitor.h"

BlockStmt* TryStmt::build(bool tryBang, BlockStmt* body) {
  return buildChplStmt(new TryStmt(tryBang, body));
}

BlockStmt* TryStmt::buildChplStmt(Expr* expr) {
  return new BlockStmt(expr, BLOCK_SCOPELESS);
}

TryStmt::TryStmt(bool _tryBang, BlockStmt* _body) : Stmt(E_TryStmt) {
  tryBang = _tryBang;
  body    = _body;
}

TryStmt::~TryStmt() {

}

BlockStmt* TryStmt::getBody() const {
  return body;
}

void TryStmt::accept(AstVisitor* visitor) {
  if (visitor->enterTryStmt(this)) {
    if (body) {
      body->accept(visitor);
    }
    visitor->exitTryStmt(this);
  }
}

Expr* TryStmt::copy(SymbolMap* map, bool internal) {
  return NULL;
}

Expr* TryStmt::copyInner(SymbolMap* map) {
  return NULL;
}

void TryStmt::replaceChild(Expr* old_ast, Expr* new_ast) {

}

Expr* TryStmt::getFirstChild() {
  return body;
}

Expr* TryStmt::getFirstExpr() {
  return body->getFirstExpr();
}

Expr* TryStmt::getNextExpr(Expr* expr) {
  return this;
}
