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

#include "DoWhileStmt.h"

#include "AstVisitor.h"
#include "build.h"

/************************************ | *************************************
*                                                                           *
* Factory methods for the Parser                                            *
*                                                                           *
************************************* | ************************************/

BlockStmt* DoWhileStmt::build(Expr* cond, BlockStmt* body)
{
  VarSymbol*   condVar       = newTemp();
  CallExpr*    condTest      = new CallExpr("_cond_test", cond);
  LabelSymbol* continueLabel = new LabelSymbol("_continueLabel");
  LabelSymbol* breakLabel    = new LabelSymbol("_breakLabel");
  DoWhileStmt* loop          = 0;
  BlockStmt*   retval        = new BlockStmt();

  // make variables declared in the scope of the body visible to
  // expressions in the condition of a do..while block
  if (body->length() == 1 && toBlockStmt(body->body.only()))
  {
    body = toBlockStmt(body->body.only());
    body->remove();
  }

  body->insertAtTail(new DefExpr(continueLabel));
  body->insertAtTail(new CallExpr(PRIM_MOVE, condVar, condTest->copy()));

  loop = new DoWhileStmt(condVar, body);

  loop->mContinueLabel = continueLabel;
  loop->mBreakLabel    = breakLabel;

  retval->insertAtTail(new DefExpr(condVar));

  retval->insertAtTail(loop);

  retval->insertAtTail(new DefExpr(breakLabel));

  return retval;
}
/************************************ | *************************************
*                                                                           *
* Instance methods                                                          *
*                                                                           *
************************************* | ************************************/

DoWhileStmt::DoWhileStmt(Expr* expr, BlockStmt* body) :
  WhileStmt(expr, body)
{

}

DoWhileStmt::DoWhileStmt(VarSymbol* var, BlockStmt* body) :
  WhileStmt(var, body)
{

}

DoWhileStmt::~DoWhileStmt()
{

}

DoWhileStmt* DoWhileStmt::copy(SymbolMap* map, bool internal)
{
  Expr*        cond   = NULL;
  BlockStmt*   body   = NULL;
  DoWhileStmt* retval = new DoWhileStmt(cond, body);

  retval->copyShare(*this, map, internal);

  return retval;
}

bool DoWhileStmt::isDoWhileStmt() const
{
  return true;
}

void DoWhileStmt::accept(AstVisitor* visitor)
{
  if (visitor->enterDoWhileStmt(this) == true)
  {
    for_alist(next_ast, body)
      next_ast->accept(visitor);

    if (condExprGet() != 0)
      condExprGet()->accept(visitor);

    if (useList)
      useList->accept(visitor);

    if (byrefVars)
      byrefVars->accept(visitor);

    visitor->exitDoWhileStmt(this);
  }
}

Expr* DoWhileStmt::getFirstExpr()
{
  Expr* retval = 0;

  if (condExprGet() != 0)
    retval = condExprGet()->getFirstExpr();

  else if (body.head      != 0)
    retval = body.head->getFirstExpr();

  else
    retval = this;

  return retval;
}

Expr* DoWhileStmt::getNextExpr(Expr* expr)
{
  Expr* retval = this;

  if (expr == condExprGet() && body.head != NULL)
    retval = body.head->getFirstExpr();

  return retval;
}
