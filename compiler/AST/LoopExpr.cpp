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

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif


#include "AstVisitor.h"
#include "AstVisitorTraverse.h"
#include "astutil.h"
#include "build.h"
#include "expr.h"
#include "ForLoop.h"
#include "LoopExpr.h"
#include "passes.h"
#include "scopeResolve.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "TransformLogicalShortCircuit.h"
#include "wellknown.h"

#include <vector>

// Finds all the UnresolvedSymExprs in the LoopExpr's indices expression and
// populates a flat AList of DefExprs. Used during scope resolution.
static void findLoopExprDefs(LoopExpr* fe, Expr* indices, AList& defIndices) {
  if (CallExpr* call = toCallExpr(indices)) {
    for_actuals(act, call) {
      findLoopExprDefs(fe, act, defIndices);
    }
  } else if (UnresolvedSymExpr* se = toUnresolvedSymExpr(indices)) {
    VarSymbol* idx = new VarSymbol(se->unresolved);
    idx->addFlag(FLAG_INDEX_VAR);
    idx->addFlag(FLAG_INSERT_AUTO_DESTROY);
    DefExpr* def = new DefExpr(idx);
    defIndices.insertAtTail(def);
  }
}

LoopExpr::LoopExpr(Expr* indices,
                       Expr* iteratorExpr,
                       Expr* expr,
                       Expr* cond,
                       bool maybeArrayType,
                       bool zippered,
                       bool forall) :
  Expr(E_LoopExpr),
  indices(indices),
  iteratorExpr(iteratorExpr),
  cond(cond),
  maybeArrayType(maybeArrayType),
  zippered(zippered),
  forall(forall)
{

  if (forall == false && maybeArrayType) {
    INT_FATAL("For-exprs cannot possibly result in an array type");
  }

  // 'expr' should be a BlockStmt so that any nested functions remain within
  // the LoopExpr (e.g. a reduction).
  if (BlockStmt* block = toBlockStmt(expr)) {
    this->expr = block;
  } else {
    this->expr = new BlockStmt(expr);
  }

  defIndices.parent = this;

  if (indices != NULL) {
    findLoopExprDefs(this, indices, defIndices);
  }

  gLoopExprs.add(this);
}

LoopExpr::LoopExpr(bool maybeArrayType, bool zippered, bool forall) :
  Expr(E_LoopExpr),
  maybeArrayType(maybeArrayType),
  zippered(zippered),
  forall(forall)
{
}

LoopExpr* LoopExpr::copyInner(SymbolMap* map) {
  LoopExpr* ret = new LoopExpr(maybeArrayType, zippered, forall);

  for_alist(ind, defIndices) {
    ret->defIndices.insertAtTail(COPY_INT(ind));
  }
  ret->indices        = COPY_INT(indices);
  ret->iteratorExpr   = COPY_INT(iteratorExpr);
  ret->expr           = COPY_INT(expr);
  ret->cond           = COPY_INT(cond);

  return ret;
}

void LoopExpr::replaceChild(Expr* old_ast, Expr* new_ast) {
  if (old_ast == indices)
    indices = new_ast;
  else if (old_ast == iteratorExpr)
    iteratorExpr = new_ast;
  else if (old_ast == expr)
    expr = toBlockStmt(new_ast);
  else if (old_ast == cond)
    cond = new_ast;
  else
    INT_FATAL(this, "unexpected case in LoopExpr::replaceChild");
}

void
LoopExpr::verify() {
  Expr::verify(E_LoopExpr);

  if (indices)      verifyParent(indices);
  if (iteratorExpr) verifyParent(iteratorExpr);
  if (cond)         verifyParent(cond);
  if (expr)         verifyParent(expr);

  if (indices)      verifyNotOnList(indices);
  if (iteratorExpr) verifyNotOnList(iteratorExpr);
  if (cond)         verifyNotOnList(cond);
  if (expr)         verifyNotOnList(expr);
}

void LoopExpr::accept(AstVisitor* visitor) {
  if (visitor->enterLoopExpr(this)) {
    for_alist(ind, defIndices) {
      ind->accept(visitor);
    }
    if (indices)      indices->accept(visitor);
    if (iteratorExpr) iteratorExpr->accept(visitor);
    if (cond)         cond->accept(visitor);
    if (expr)         expr->accept(visitor);

    visitor->exitLoopExpr(this);
  }
}

Expr* LoopExpr::getFirstExpr() {
  INT_FATAL(this, "LoopExpr::getFirstExpr() is not implemented");
  return NULL;
}

GenRet LoopExpr::codegen() {
  GenRet ret;
  INT_FATAL(this, "LoopExpr::codegen called");
  return ret;
}

