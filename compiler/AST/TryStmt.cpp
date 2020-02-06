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

#include "TryStmt.h"

#include "AstVisitor.h"
#include "CatchStmt.h"

BlockStmt* TryStmt::build(bool tryBang, Expr* expr) {
  BlockStmt* body = new BlockStmt();
  if (BlockStmt* block = toBlockStmt(expr)) {
    // Flatten any scopeless blocks
    body->appendChapelStmt(block);
  } else {
    body->insertAtTail(expr);
  }
  return buildChplStmt(new TryStmt(tryBang, body, NULL));
}

BlockStmt* TryStmt::build(bool tryBang, BlockStmt* body, BlockStmt* catches,
                          bool isSyncTry) {
  return buildChplStmt(new TryStmt(tryBang, body, catches, isSyncTry));
}

BlockStmt* TryStmt::buildChplStmt(Expr* expr) {
  return new BlockStmt(expr, BLOCK_SCOPELESS);
}

// catches are stored in a BlockStmt for convenient parsing
TryStmt::TryStmt(bool tryBang, BlockStmt* body, BlockStmt* catches,
                 bool isSyncTry) : Stmt(E_TryStmt) {
  _tryBang = tryBang;
  _body    = body;
  _isSyncTry = isSyncTry;

  _catches.parent = this;
  if (catches) {
    for_alist(c, catches->body) {
      INT_ASSERT(isCatchStmt(c));
      _catches.insertAtTail(c->remove());
    }
  }

  gTryStmts.add(this);
}

TryStmt::~TryStmt() {

}

BlockStmt* TryStmt::body() const {
  return _body;
}

bool TryStmt::tryBang() const {
  return _tryBang;
}

// Indicates if the try/catch statement was inserted due to the presence of a
// sync block instead of explicit user code.
bool TryStmt::isSyncTry() const {
  return _isSyncTry;
}

void TryStmt::accept(AstVisitor* visitor) {
  if (visitor->enterTryStmt(this)) {
    if (_body) {
      _body->accept(visitor);
    }

    visitor->exitTryStmt(this);

    for_alist(c, _catches) {
      CatchStmt* catchStmt = toCatchStmt(c);
      catchStmt->accept(visitor);
    }
  }
}

TryStmt* TryStmt::copyInner(SymbolMap* map) {
  TryStmt* copy = new TryStmt(_tryBang, COPY_INT(_body), NULL);
  for_alist(c, _catches) {
    copy->_catches.insertAtTail(COPY_INT(c));
  }
  return copy;
}

void TryStmt::replaceChild(Expr* old_ast, Expr* new_ast) {
  if (old_ast == _body) {
    _body = toBlockStmt(new_ast);
  }
  // catches are handled automatically (AList)
}

Expr* TryStmt::getFirstExpr() {
  if (_body) {
    return _body->getFirstExpr();
  }
  return NULL;
}

Expr* TryStmt::getNextExpr(Expr* expr) {
  Expr* retVal = this;

  if (expr == _body) {
    if (Expr* firstCatch = _catches.first()) {
      retVal = firstCatch->getFirstExpr();
    }
  } else if (expr->list == &_catches) {
    if (Expr* nextCatch = expr->next) {
      retVal = nextCatch->getFirstExpr();
    }
  }

  return retVal;
}

void TryStmt::verify() {
  Stmt::verify();

  if (astTag != E_TryStmt) {
    INT_FATAL(this, "TryStmt::verify. Bad astTag");
  }

  if (!_body) {
    INT_FATAL(this, "TryStmt::verify. _body is missing");
  }

  for_alist(c, _catches) {
    if (!isCatchStmt(c))
      INT_FATAL(this, "TryStmt::verify. _catches contains a non-CatchStmt");
  }
}

GenRet TryStmt::codegen() {
  INT_FATAL("TryStmt should be removed before codegen");
  GenRet ret;
  return ret;
}
