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

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include "expr.h"
#include "IfExpr.h"
#include "stmt.h"
#include "AstVisitor.h"

//
// Implementation of 'IfExpr'
//
// Currently intended to be lowered during normalize()
//

IfExpr::IfExpr(Expr* condition, Expr* thenExpr, Expr* elseExpr) :
  Expr(E_IfExpr),
  condition(NULL),
  thenStmt(NULL),
  elseStmt(NULL)
{
  this->condition = condition;

  if (BlockStmt* block = toBlockStmt(thenExpr)) {
    this->thenStmt = block;
  } else {
    this->thenStmt = new BlockStmt(thenExpr, BLOCK_SCOPELESS);
  }

  if (BlockStmt* block = toBlockStmt(elseExpr)) {
    this->elseStmt = block;
  } else {
    this->elseStmt = new BlockStmt(elseExpr, BLOCK_SCOPELESS);
  }

  gIfExprs.add(this);
}

Expr* IfExpr::getCondition() {
  return condition;
}

BlockStmt* IfExpr::getThenStmt() {
  return thenStmt;
}

BlockStmt* IfExpr::getElseStmt() {
  return elseStmt;
}

void IfExpr::verify() {
  Expr::verify(E_IfExpr);
  verifyParent(condition);
  verifyParent(thenStmt);
  verifyParent(elseStmt);

  verifyNotOnList(condition);
  verifyNotOnList(thenStmt);
  verifyNotOnList(elseStmt);
}

IfExpr*
IfExpr::copyInner(SymbolMap* map) {
  return new IfExpr(COPY_INT(condition), COPY_INT(thenStmt), COPY_INT(elseStmt));
}

void IfExpr::replaceChild(Expr* old_ast, Expr* new_ast) {
  if (old_ast == condition) {
    condition = new_ast;
  } else if (old_ast == thenStmt) {
    thenStmt = toBlockStmt(new_ast);
  } else if (old_ast == elseStmt) {
    elseStmt = toBlockStmt(new_ast);
  } else {
    INT_FATAL(this, "Unhandled case in IfExpr::replaceChild");
  }
}

void IfExpr::accept(AstVisitor* visitor) {
  if (visitor->enterIfExpr(this) == true) {

    condition->accept(visitor);
    thenStmt->accept(visitor);
    elseStmt->accept(visitor);

    visitor->exitIfExpr(this);
  }
}

QualifiedType IfExpr::qualType() {
  return QualifiedType(QUAL_VAL, dtUnknown);
}

void IfExpr::prettyPrint(std::ostream* o) {
  *o << "<IfExprType>";
}

Expr* IfExpr::getFirstExpr() {
  return (condition != NULL) ? condition->getFirstExpr() : this;
}

GenRet IfExpr::codegen() {
  GenRet ret;
  INT_FATAL(this, "IfExpr::codegen not implemented");
  return ret;
}

bool isLoweredIfExprBlock(BlockStmt* block) {
  if (CallExpr* call = toCallExpr(block->body.last()))
    if (call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN))
      if (SymExpr* lhs = toSymExpr(call->get(1)))
        if (lhs->symbol()->hasFlag(FLAG_IF_EXPR_RESULT))
          return true;

  return false;
}

