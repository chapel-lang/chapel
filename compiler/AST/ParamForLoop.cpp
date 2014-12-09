/*
 * Copyright 2004-2014 Cray Inc.
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

#include "ParamForLoop.h"

#include "build.h"

/************************************ | *************************************
*                                                                           *
* Factory methods for the Parser                                            *
*                                                                           *
************************************* | ************************************/

BlockStmt* ParamForLoop::buildParamForLoop(VarSymbol* indexVar,
                                           Expr*      range,
                                           BlockStmt* stmts)
{
  VarSymbol*   lowVar     = newParamVar();
  VarSymbol*   highVar    = newParamVar();
  VarSymbol*   strideVar  = newParamVar();

  LabelSymbol* breakLabel = new LabelSymbol("_breakLabel");

  CallExpr*    call       = toCallExpr(range);
  Expr*        low        = NULL;
  Expr*        high       = NULL;
  Expr*        stride     = NULL;

  BlockStmt*   outer      = new BlockStmt();

  if (call && call->isNamed("by"))
  {
    stride = call->get(2)->remove();
    call   = toCallExpr(call->get(1));
  }
  else
  {
    stride = new SymExpr(new_IntSymbol(1));
  }

  if (call && call->isNamed("_build_range"))
  {
    low    = call->get(1)->remove();
    high   = call->get(1)->remove();
  }
  else
  {
    USR_FATAL(range, "iterators for param-for-loops must be literal ranges");
  }

  outer->insertAtTail(new DefExpr(indexVar, new_IntSymbol((int64_t) 0)));

  outer->insertAtTail(new DefExpr(lowVar));
  outer->insertAtTail(new CallExpr(PRIM_MOVE, lowVar,    low));

  outer->insertAtTail(new DefExpr(highVar));
  outer->insertAtTail(new CallExpr(PRIM_MOVE, highVar,   high));

  outer->insertAtTail(new DefExpr(strideVar));
  outer->insertAtTail(new CallExpr(PRIM_MOVE, strideVar, stride));

  outer->insertAtTail(new ParamForLoop(indexVar,
                                       lowVar,
                                       highVar,
                                       strideVar,
                                       breakLabel,
                                       stmts));

  outer->insertAtTail(new DefExpr(breakLabel));

  return buildChapelStmt(outer);
}

VarSymbol* ParamForLoop::newParamVar()
{
  VarSymbol* retval = newTemp();

  retval->addFlag(FLAG_MAYBE_PARAM);

  return retval;
}

/************************************ | *************************************
*                                                                           *
* Instance methods                                                          *
*                                                                           *
************************************* | ************************************/

ParamForLoop::ParamForLoop(VarSymbol*   indexVar,
                           VarSymbol*   lowVar,
                           VarSymbol*   highVar,
                           VarSymbol*   strideVar,
                           LabelSymbol* breakLabel,
                           BlockStmt*   initBody) : LoopStmt(initBody)
{
  mIndexVariable  = indexVar;
  mLowVariable    = lowVar;
  mHighVariable   = highVar;
  mStrideVariable = strideVar;

  breakLabelSet(breakLabel);

  BlockStmt::blockInfoSet(new CallExpr(PRIM_BLOCK_PARAM_LOOP,
                                       indexVar,
                                       lowVar,
                                       highVar,
                                       strideVar));
}

ParamForLoop::~ParamForLoop()
{

}

bool ParamForLoop::isParamForLoop() const
{
  return true;
}

CallExpr* ParamForLoop::paramInfoGet() const
{
  return BlockStmt::blockInfoGet();
}

CallExpr* ParamForLoop::blockInfoGet() const
{
  printf("Migration: ParamForLoop   %12d Unexpected call to blockInfoGet()\n", id);

  return BlockStmt::blockInfoGet();
}

CallExpr* ParamForLoop::blockInfoSet(CallExpr* expr)
{
  printf("Migration: ParamForLoop   %12d Unexpected call to blockInfoSet()\n", id);

  return BlockStmt::blockInfoSet(expr);
}

Expr* ParamForLoop::getFirstExpr()
{
  Expr* retval = 0;

  if (paramInfoGet() != 0)
    retval = paramInfoGet()->getFirstExpr();

  else if (body.head      != 0)
    retval = body.head->getFirstExpr();

  else
    retval = this;

  return retval;
}

Expr* ParamForLoop::getNextExpr(Expr* expr)
{
  Expr* retval = this;

  if (expr == paramInfoGet() && body.head != 0)
    retval = body.head->getFirstExpr();

  return retval;
}
