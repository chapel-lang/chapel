/*
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

#include "CatchStmt.h"

#include "AstVisitor.h"
#include "DecoratedClassType.h"
#include "stringutil.h"
#include "TryStmt.h"
#include "wellknown.h"

CatchStmt* CatchStmt::build(DefExpr* def, BlockStmt* body) {
  const char* name = def->sym->name;
  Expr* type = def->exprType;
  return new CatchStmt(name, type, body);
}

CatchStmt* CatchStmt::build(const char* name, Expr* type, BlockStmt* body) {
  return new CatchStmt(name, type, body);
}

CatchStmt* CatchStmt::build(const char* name, BlockStmt* body) {
  return new CatchStmt(name, NULL, body);
}

CatchStmt* CatchStmt::build(BlockStmt* body) {
  return new CatchStmt(NULL, NULL, body);
}

CatchStmt::CatchStmt(const char* name, Expr* type, BlockStmt* body)
  : Stmt(E_CatchStmt) {

  _name = name ? astr(name) : NULL;
  _type = type;
  _body = body;

  gCatchStmts.add(this);
}

CatchStmt::~CatchStmt() {

}

const char* CatchStmt::name() const {
  return _name;
}
Expr* CatchStmt::type() const {
  return _type;
}

BlockStmt* CatchStmt::body() const {
  return _body;
}

bool CatchStmt::isCatchall() const {
  if (_name == NULL)
    return true;

  if (_type == NULL)
    return true;

  if (SymExpr* typeSe = toSymExpr(type()))
    if (canonicalClassType(typeSe->symbol()->type) == dtError)
      return true;

  if (UnresolvedSymExpr* urse = toUnresolvedSymExpr(type()))
    if (urse->unresolved == dtError->symbol->name)
      return true;

  return false;
}

void CatchStmt::accept(AstVisitor* visitor) {
  if (visitor->enterCatchStmt(this)) {
    for_alist(node, _body->body) {
      node->accept(visitor);
    }
    visitor->exitCatchStmt(this);
  }
}

CatchStmt* CatchStmt::copyInner(SymbolMap* map) {
  return new CatchStmt(_name, COPY_INT(_type), COPY_INT(_body));
}

void CatchStmt::replaceChild(Expr* old_ast, Expr* new_ast) {
  if (_type == old_ast) {
    _type = new_ast;
  } else if (_body == old_ast) {
    _body = toBlockStmt(new_ast);
  }
}

Expr* CatchStmt::getFirstExpr() {
  if (_type) {
    return _type;
  }
  if (_body) {
    return _body->getFirstExpr();
  }
  return NULL;
}

Expr* CatchStmt::getNextExpr(Expr* expr) {
  Expr* retVal = this;

  if (expr == _type) {
    retVal = _body->getFirstExpr();
  }
  return retVal;
}
void CatchStmt::verify() {
  Stmt::verify();

  if (astTag != E_CatchStmt) {
    INT_FATAL(this, "CatchStmt::verify. Bad astTag");
  }

  if (_type) {
    if (_type->parentExpr != this ||
        _type->parentSymbol != this->parentSymbol)
      INT_FATAL(this, "CatchStmt::verify. _type has bad parent");
  }

  if (!_body) {
    INT_FATAL(this, "CatchStmt::verify. _body is missing");
  }
}

void CatchStmt::cleanup()
{
  /*
   Introduce the variable representing the catch variable and define it
   in a way that will:
     - allow resolving references to it in scopeResolve
     - allow resolving the type of it in resolve

   In particular, convert

     catch e: SomeType
     {
       body(e)
     }

   into

     catch e: SomeType
     {
       var castedError = dynamic cast current error -> SomeType

       if castedError != nil {
         var e: owned SomeType = new owned(castedError)
         body(e)
       }
     }
   */

  // Check isCatchall before setting _name or _type
  bool catchall = isCatchall();
  // Below, we will transform even catchall block so that the error will
  // be freed appropriately.

  Expr* typeExpr = NULL;
  if (_type != NULL) {
    typeExpr = _type->copy();
  } else {
    typeExpr = new SymExpr(dtError->symbol);
  }
  INT_ASSERT(typeExpr);

  const char* name = _name;
  if (name == NULL)
    name = astr("chpl_anon_error");

  VarSymbol* castedError = newTemp();

  Expr* unmanagedType = new CallExpr(PRIM_TO_UNMANAGED_CLASS, typeExpr);
  Expr* castedCurrentError = NULL;
  if (catchall) {
    castedCurrentError = new CallExpr(PRIM_CURRENT_ERROR);
  } else {
    castedCurrentError = new CallExpr(PRIM_DYNAMIC_CAST,
                                      unmanagedType,
                                      new CallExpr(PRIM_CURRENT_ERROR));
  }

  DefExpr* castedErrorDef = new DefExpr(castedError, castedCurrentError);

  BlockStmt* newBody = new BlockStmt();
  BlockStmt* oldBody = body();
  INT_ASSERT(oldBody);
  _body->replace(newBody);

  newBody->insertAtHead(castedErrorDef);

  VarSymbol* error = new VarSymbol(name);
  Expr* ownedCastedError = new CallExpr(PRIM_NEW,
                                        new CallExpr("_owned", castedError));

  DefExpr* errorDef = new DefExpr(error, ownedCastedError);

  if (catchall) {
    newBody->insertAtTail(errorDef);
    newBody->insertAtTail(oldBody);

  } else {
    CallExpr*  errorExists = new CallExpr(PRIM_NOTEQUAL, castedError, gNil);

    BlockStmt* ifBody = new BlockStmt();
    BlockStmt* elseBody = new BlockStmt();
    CondStmt* cond = new CondStmt(errorExists, ifBody, elseBody);
    castedErrorDef->insertAfter(cond);

    VarSymbol* error = new VarSymbol(name);
    Expr* ownedCastedError = new CallExpr(PRIM_NEW,
                                          new CallExpr("_owned", castedError));
    DefExpr* errorDef = new DefExpr(error, ownedCastedError);
    ifBody->insertAtTail(errorDef);
    ifBody->insertAtTail(oldBody);

    // Find the parent try statement. If it's a try!, add a call
    // to halt so that isDefinedAllPaths works correctly.

    TryStmt* inTry = NULL;
    for (Expr* cur = this->parentExpr; cur != NULL; cur = cur->parentExpr) {
      if (TryStmt* t = toTryStmt(cur))
        inTry = t;
    }

    INT_ASSERT(inTry);
    if (inTry->tryBang())
      elseBody->insertAtTail(new CallExpr(PRIM_RT_ERROR));
  }

  // If we in the future support `throw;` to throw the currently caught
  // error, we'd update such a throw here to throw 'error'.
}


GenRet CatchStmt::codegen() {
  INT_FATAL("CatchStmt should be removed before codegen");
  GenRet ret;
  return ret;
}
