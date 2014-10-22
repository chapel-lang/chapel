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

#include "WhileDoStmt.h"

#include "build.h"
#include "ForLoop.h"

/************************************ | *************************************
*                                                                           *
* Factory methods for the Parser                                            *
*                                                                           *
************************************* | ************************************/

BlockStmt* WhileDoStmt::build(Expr* cond, BlockStmt* body) 
{
  BlockStmt* retval = NULL;

  if (isCForLoop(cond) == true)
  {
    retval = ForLoop::buildCForLoop(toCallExpr(cond), body);
  }

  else
  {
    VarSymbol*   condVar       = newTemp();
    CallExpr*    condTest      = new CallExpr("_cond_test", cond);

    LabelSymbol* continueLabel = new LabelSymbol("_continueLabel");
    LabelSymbol* breakLabel    = new LabelSymbol("_breakLabel");

    WhileDoStmt* loop          = new WhileDoStmt(body);

    loop->blockInfoSet(new CallExpr(PRIM_BLOCK_WHILEDO_LOOP, condVar));

    loop->continueLabel = continueLabel;
    loop->breakLabel    = breakLabel;

    loop->insertAtTail(new DefExpr(continueLabel));
    loop->insertAtTail(new CallExpr(PRIM_MOVE, condVar, condTest->copy()));

    retval = buildChapelStmt();

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
bool WhileDoStmt::isCForLoop(Expr* cond)
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

WhileDoStmt::WhileDoStmt(BlockStmt* initBody) : WhileStmt(initBody)
{

}

WhileDoStmt::~WhileDoStmt()
{

}

WhileDoStmt* WhileDoStmt::copy(SymbolMap* map, bool internal) 
{
  WhileDoStmt* retval = new WhileDoStmt(NULL);

  retval->copyShare(*this, map, internal);

  return retval;
}
