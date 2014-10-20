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

#include "ForLoop.h"

#include "build.h"

/************************************ | *************************************
*                                                                           *
* Factory methods for the Parser                                            *
*                                                                           *
************************************* | ************************************/

BlockStmt* ForLoop::buildForLoop(Expr*      indices,
                                 Expr*      iterExpr,
                                 BlockStmt* body,
                                 bool       coforall,
                                 bool       zippered) 
{
  VarSymbol* iter      = iteratorTemp();
  CallExpr*  iterInit  = 0;

  VarSymbol* index     = indexTemp(coforall);
  CallExpr*  indexInit = 0;

  ForLoop*   loop      = new ForLoop(indices,
                                     index,
                                     iter,
                                     coforall,
                                     body);
  BlockStmt* retval    = buildChapelStmt();

  retval->insertAtTail(new DefExpr(iter));

  if (zippered == false) 
  {
    // Unzippered loop, treat all objects (including tuples) the same
    iterInit = new CallExpr(PRIM_MOVE, 
                            iter, 
                            new CallExpr("_getIterator", iterExpr));
  }
  else 
  {
    // Expand tuple to a tuple containing appropriate iterators for each value.
    iterInit = new CallExpr(PRIM_MOVE, 
                            iter, 
                            new CallExpr("_getIteratorZip", iterExpr));
  }

  indexInit = new CallExpr(PRIM_MOVE, 
                           index,
                           new CallExpr("iteratorIndex", iter));

  retval->insertAtTail(iterInit);
  retval->insertAtTail(new DefExpr(index));
  retval->insertAtTail(new BlockStmt(indexInit, BLOCK_TYPE));

  retval->insertAtTail(loop);
  retval->insertAtTail(new DefExpr(loop->breakLabel));
  retval->insertAtTail(new CallExpr("_freeIterator", iter));

  return retval;
}

VarSymbol* ForLoop::iteratorTemp()
{
  VarSymbol* retval = newTemp("_iterator");

  retval->addFlag(FLAG_EXPR_TEMP);

  return retval;
}

VarSymbol* ForLoop::indexTemp(bool coforall)
{
  VarSymbol* retval = newTemp("_indexOfInterest");

  retval->addFlag(FLAG_INDEX_OF_INTEREST);

  if (coforall)
    retval->addFlag(FLAG_COFORALL_INDEX_VAR);

  return retval;
}

BlockStmt* ForLoop::buildCForLoop(CallExpr* call, BlockStmt* body)
{
  ForLoop*   loop   = new ForLoop(call, body);
  BlockStmt* retval = buildChapelStmt();

  retval->insertAtTail(loop);
  retval->insertAtTail(new DefExpr(loop->breakLabel));

  return retval;
}

/************************************ | *************************************
*                                                                           *
* Instance methods                                                          *
*                                                                           *
************************************* | ************************************/

ForLoop::ForLoop(Expr*      indices,
                 VarSymbol* index,
                 VarSymbol* iterator,
                 bool       coforall,
                 BlockStmt* initBody) :
  BlockStmt(initBody)
{
  addLabels();

  // insert temporary index when elided by user
  if (indices == NULL)
    indices = new UnresolvedSymExpr("chpl__elidedIdx");

  checkIndices(indices);

  blockInfoSet(new CallExpr(PRIM_BLOCK_FOR_LOOP, index, iterator));

  destructureIndices(this, indices, new SymExpr(index), coforall);

  insertAtTail(new DefExpr(continueLabel));
}

ForLoop::ForLoop(CallExpr* call, BlockStmt* initBody) :
  BlockStmt(initBody)
{
  addLabels();

  // C for loops have the form:
  //   __primitive("C for loop", initExpr, testExpr, incrExpr)
  //
  // This simply wraps the init, test, and incr expr with block stmts
  call->get(1)->replace(new BlockStmt(call->get(1)->copy()));
  call->get(2)->replace(new BlockStmt(call->get(2)->copy()));
  call->get(3)->replace(new BlockStmt(call->get(3)->copy()));

  blockInfoSet(call);

  insertAtTail(new DefExpr(continueLabel));
}

void ForLoop::addLabels()
{
  continueLabel = new LabelSymbol("_continueLabel");
  breakLabel    = new LabelSymbol("_breakLabel");

  continueLabel->addFlag(FLAG_COMPILER_GENERATED);
  continueLabel->addFlag(FLAG_LABEL_CONTINUE);

  breakLabel->addFlag(FLAG_COMPILER_GENERATED);
  breakLabel->addFlag(FLAG_LABEL_BREAK);
}

ForLoop::~ForLoop()
{

}
