/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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

#include "WhileDoStmt.h"

#include "AstVisitor.h"
#include "build.h"
#include "CForLoop.h"
#include "driver.h"

/************************************ | *************************************
*                                                                           *
* Factory methods for the Parser                                            *
*                                                                           *
************************************* | ************************************/

static CallExpr* isPrimIfVar(Expr* cond) {
  if (CallExpr* call = toCallExpr(cond))
    if (call->isPrimitive(PRIM_IF_VAR))
      return call;
  return nullptr;
}

BlockStmt* WhileDoStmt::build(Expr* cond, BlockStmt* body)
{
  BlockStmt* retval = nullptr;

  if (isPrimitiveCForLoop(cond) == true)
  {
    retval = CForLoop::buildCForLoop(toCallExpr(cond), body);
  }
  else if (CallExpr* condIV = isPrimIfVar(cond))
  {
    // while var X = condExpr do ...
    DefExpr* varDef = toDefExpr(condIV->get(1)->remove());
    Expr*  condExpr = condIV->get(1)->remove();
    INT_ASSERT(! varDef->init && ! varDef->exprType); // from parser
    
    // The construction follows the non-IfVar case below, adding the following:
    //
    //  // at start of 'retval' block
    //  def while_borrow = chpl_checkBorrowIfVar(condExpr, true)
    //  ... use 'while_borrow' where the non-IfVar cases uses 'cond' ...
    //
    //  // at start of 'loop' body
    //  def X
    //  move X <- PRIM_TO_NON_NILABLE_CLASS(while_borrow)
    //
    //  // at end of 'loop' body
    //  move while_borrow, chpl_checkBorrowIfVar(condExpr, true)
    //  ... use 'while_borrow' where the non-IfVar cases uses 'cond' ...

    // todo: share code with the non-IfVar case (below)  -vass 2021-03

    VarSymbol*   borrow        = newTemp("while_borrow");
    CallExpr*    initBorrow    = new CallExpr("chpl_checkBorrowIfVar",
                                              condExpr, gTrue);
    VarSymbol*   condVar       = newTemp();
    CallExpr*    condTest      = new CallExpr("_cond_test", borrow);

    LabelSymbol* continueLabel = new LabelSymbol("_continueLabel");
    LabelSymbol* breakLabel    = new LabelSymbol("_breakLabel");

    WhileDoStmt* loop          = new WhileDoStmt(condVar, body);

    loop->mContinueLabel = continueLabel;
    loop->mBreakLabel    = breakLabel;

    loop->insertAtHead(varDef);
    varDef->init = new CallExpr(PRIM_TO_NON_NILABLE_CLASS, borrow);

    loop->insertAtTail(new DefExpr(continueLabel));
    loop->insertAtTail(new CallExpr(PRIM_MOVE, borrow, initBorrow->copy()));
    loop->insertAtTail(new CallExpr(PRIM_MOVE, condVar, condTest->copy()));

    retval = new BlockStmt();

    retval->insertAtTail(new DefExpr(borrow));
    borrow->defPoint->init = initBorrow;
    retval->insertAtTail(new DefExpr(condVar));
    retval->insertAtTail(new CallExpr(PRIM_MOVE, condVar, condTest->copy()));
    retval->insertAtTail(loop);
    retval->insertAtTail(new DefExpr(breakLabel));
  }
  else
  {
    VarSymbol*   condVar       = newTemp();
    CallExpr*    condTest      = new CallExpr("_cond_test", cond);

    LabelSymbol* continueLabel = new LabelSymbol("_continueLabel");
    LabelSymbol* breakLabel    = new LabelSymbol("_breakLabel");

    WhileDoStmt* loop          = new WhileDoStmt(condVar, body);

    loop->mContinueLabel = continueLabel;
    loop->mBreakLabel    = breakLabel;

    loop->insertAtTail(new DefExpr(continueLabel));
    loop->insertAtTail(new CallExpr(PRIM_MOVE, condVar, condTest->copy()));

    retval = new BlockStmt();

    retval->insertAtTail(new DefExpr(condVar));
    retval->insertAtTail(new CallExpr(PRIM_MOVE, condVar, condTest->copy()));
    retval->insertAtTail(loop);
    retval->insertAtTail(new DefExpr(breakLabel));
  }

  return retval;
}

// C for loops are invoked with 'while __primitive("C for loop" ...)'
// This checks if we had such a case and if we did builds the c for loop
// instead of the while loop and returns it.
bool WhileDoStmt::isPrimitiveCForLoop(Expr* cond)
{
  bool retval = false;

  if (CallExpr* call = toCallExpr(cond))
    retval = (call->isPrimitive(PRIM_BLOCK_C_FOR_LOOP)) ? true : false;

  return retval;
}

/************************************ | *************************************
*                                                                           *
* Instance methods                                                          *
*                                                                           *
************************************* | ************************************/

WhileDoStmt* WhileDoStmt::copyInner(SymbolMap* map)
{
  Expr*        condExpr = 0;
  BlockStmt*   body     = 0;
  WhileDoStmt* retval   = new WhileDoStmt(condExpr, body);

  retval->copyInnerShare(*this, map);

  return retval;
}

void WhileDoStmt::accept(AstVisitor* visitor)
{
  if (visitor->enterWhileDoStmt(this) == true)
  {
    for_alist(next_ast, body)
      next_ast->accept(visitor);

    if (condExprGet() != 0)
      condExprGet()->accept(visitor);

    if (useList)
      useList->accept(visitor);

    if (byrefVars)
      byrefVars->accept(visitor);

    visitor->exitWhileDoStmt(this);
  }
}

Expr* WhileDoStmt::getFirstExpr()
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

Expr* WhileDoStmt::getNextExpr(Expr* expr)
{
  Expr* retval = this;

  if (expr == condExprGet() && body.head != nullptr)
    retval = body.head->getFirstExpr();

  return retval;
}
