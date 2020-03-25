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

DeferStmt::DeferStmt(CallExpr* call)
  : Stmt(E_DeferStmt),
   _body(new BlockStmt(call)) {

  gDeferStmts.add(this);
}


DeferStmt::~DeferStmt() {

}

BlockStmt* DeferStmt::body() const {
  return _body;
}

void DeferStmt::accept(AstVisitor* visitor) {
  if (visitor->enterDeferStmt(this)) {
    if (_body) {
      _body->accept(visitor);
    }

    visitor->exitDeferStmt(this);
  }
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

// supporting checkDefersDoNotBreakOrReturn()
// Use an anonymous namespace to keep CheckDerefVisitor
// local to this file.

namespace {
  class CheckDeferVisitor : public AstVisitorTraverse {

  public:
    CheckDeferVisitor();

    // We are checking for 'break' or 'return'
    virtual bool   enterCallExpr       (CallExpr*          node);
    virtual bool   enterGotoStmt       (GotoStmt*          node);

    // We don't want to enter inner functions
    virtual bool   enterFnSym          (FnSymbol*          node);

    // Track loops, so we can know if a 'break' is inside a loop or not
    virtual bool   enterWhileDoStmt    (WhileDoStmt*       node);
    virtual void   exitWhileDoStmt     (WhileDoStmt*       node);

    virtual bool   enterDoWhileStmt    (DoWhileStmt*       node);
    virtual void   exitDoWhileStmt     (DoWhileStmt*       node);

    virtual bool   enterCForLoop       (CForLoop*          node);
    virtual void   exitCForLoop        (CForLoop*          node);

    virtual bool   enterForLoop        (ForLoop*           node);
    virtual void   exitForLoop         (ForLoop*           node);

    virtual bool   enterParamForLoop   (ParamForLoop*      node);
    virtual void   exitParamForLoop    (ParamForLoop*      node);

  private:
    int loopDepth;

    void enterLoop() {
      loopDepth++;
    }
    void exitLoop() {
      loopDepth--;
    }
  };

  CheckDeferVisitor::CheckDeferVisitor() {
    loopDepth = 0;
  }

  bool CheckDeferVisitor::enterCallExpr(CallExpr* node) {
    if (node->isPrimitive(PRIM_RETURN))
      USR_FATAL_CONT(node, "return cannot be used within a defer statement");
    return true;
  }
  bool CheckDeferVisitor::enterGotoStmt(GotoStmt* node) {
    if (loopDepth == 0)
      USR_FATAL_CONT(node, "break cannot be used within a defer statement");
    return true;
  }

  bool CheckDeferVisitor::enterFnSym(FnSymbol* node) {
    return false;
  }

  bool CheckDeferVisitor::enterWhileDoStmt(WhileDoStmt* node) {
    enterLoop();
    return true;
  }
  void CheckDeferVisitor::exitWhileDoStmt(WhileDoStmt* node) {
    exitLoop();
  }
  bool CheckDeferVisitor::enterDoWhileStmt(DoWhileStmt* node) {
    enterLoop();
    return true;
  }
  void CheckDeferVisitor::exitDoWhileStmt(DoWhileStmt* node) {
    exitLoop();
  }
  bool CheckDeferVisitor::enterCForLoop(CForLoop* node) {
    enterLoop();
    return true;
  }
  void CheckDeferVisitor::exitCForLoop(CForLoop* node) {
    exitLoop();
  }
  bool CheckDeferVisitor::enterForLoop(ForLoop* node) {
    enterLoop();
    return true;
  }
  void CheckDeferVisitor::exitForLoop(ForLoop* node) {
    exitLoop();
  }
  bool CheckDeferVisitor::enterParamForLoop(ParamForLoop* node) {
    enterLoop();
    return true;
  }
  void CheckDeferVisitor::exitParamForLoop(ParamForLoop* node) {
    exitLoop();
  }

} // end anonymous namespace

void checkDefersAfterParsing()
{
  forv_Vec(DeferStmt, defer, gDeferStmts) {
  
    // Check that there are no top-level defers;
    // each defer must be in a function (other than module init).
    ModuleSymbol* mod = toModuleSymbol(defer->parentSymbol);
    if (mod != NULL && defer->parentExpr == mod->block)
      USR_FATAL_CONT(defer, "defer can only be used within a function");

    // Make sure the DeferStmt does not include a break that is outside
    // of a loop.

    // Make sure that a DeferStmt never includes a return

    CheckDeferVisitor visitor;
    defer->body()->accept(&visitor);
  }
}
