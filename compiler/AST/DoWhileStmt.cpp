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

#include "DoWhileStmt.h"

#include "build.h"

/************************************ | *************************************
*                                                                           *
* Factory methods for the Parser                                            *
*                                                                           *
************************************* | ************************************/

BlockStmt* DoWhileStmt::build(Expr* cond, BlockStmt* body) 
{
  VarSymbol*   condVar = newTemp();
  DoWhileStmt* loop    = new DoWhileStmt(condVar, cond, body);
  BlockStmt*   retval  = buildChapelStmt();

  retval->insertAtTail(new DefExpr(condVar));
  retval->insertAtTail(loop);
  retval->insertAtTail(new DefExpr(loop->breakLabel));

  return retval;
}

/************************************ | *************************************
*                                                                           *
* Instance methods                                                          *
*                                                                           *
************************************* | ************************************/

DoWhileStmt::DoWhileStmt(VarSymbol* condVar,
                         Expr*      cond,
                         BlockStmt* initBody) : WhileStmt(NULL)
{
  CallExpr* condTest = new CallExpr("_cond_test", cond);

  continueLabel = new LabelSymbol("_continueLabel");
  breakLabel    = new LabelSymbol("_breakLabel");

  continueLabel->addFlag(FLAG_COMPILER_GENERATED);
  continueLabel->addFlag(FLAG_LABEL_CONTINUE);

  breakLabel->addFlag(FLAG_COMPILER_GENERATED);
  breakLabel->addFlag(FLAG_LABEL_BREAK);

  blockInfoSet(new CallExpr(PRIM_BLOCK_DOWHILE_LOOP, condVar));

  // make variables declared in the scope of the body visible to
  // expressions in the condition of a do..while block
  if (initBody->length() == 1 && toBlockStmt(initBody->body.only())) 
  {
    initBody = toBlockStmt(initBody->body.only());
    initBody->remove();
  }

  initBody->insertAtTail(new DefExpr(continueLabel));
  initBody->insertAtTail(new CallExpr(PRIM_MOVE, condVar, condTest));

  body.insertAtTail(initBody);
}

DoWhileStmt::~DoWhileStmt()
{

}
