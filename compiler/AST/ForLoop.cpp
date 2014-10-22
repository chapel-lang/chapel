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
                                 Expr*      iteratorExpr,
                                 BlockStmt* body,
                                 bool       coforall,
                                 bool       zippered) 
{
  VarSymbol*   index         = newTemp("_indexOfInterest");
  VarSymbol*   iterator      = newTemp("_iterator");
  CallExpr*    iterInit      = 0;
  CallExpr*    iterMove      = 0;
  ForLoop*     loop          = new ForLoop(body);
  LabelSymbol* continueLabel = new LabelSymbol("_continueLabel");
  LabelSymbol* breakLabel    = new LabelSymbol("_breakLabel");
  BlockStmt*   retval        = buildChapelStmt();

  iterator->addFlag(FLAG_EXPR_TEMP);


  // Unzippered loop, treat all objects (including tuples) the same
  if (zippered == false) 
    iterInit = new CallExpr(PRIM_MOVE, iterator, new CallExpr("_getIterator", iteratorExpr));

  // Expand tuple to a tuple containing appropriate iterators for each value.
  else 
    iterInit = new CallExpr(PRIM_MOVE, iterator, new CallExpr("_getIteratorZip", iteratorExpr));

  index->addFlag(FLAG_INDEX_OF_INTEREST);

  iterMove = new CallExpr(PRIM_MOVE, index, new CallExpr("iteratorIndex", iterator));

  if (indices == 0)
    indices = new UnresolvedSymExpr("chpl__elidedIdx");

  checkIndices(indices);

  destructureIndices(loop, indices, new SymExpr(index), coforall);

  if (coforall)
    index->addFlag(FLAG_COFORALL_INDEX_VAR);
  
  loop->blockInfoSet(new CallExpr(PRIM_BLOCK_FOR_LOOP, index, iterator));

  loop->continueLabel = continueLabel;
  loop->breakLabel    = breakLabel;

  loop->insertAtTail(new DefExpr(continueLabel));

  retval->insertAtTail(new DefExpr(index));
  retval->insertAtTail(new DefExpr(iterator));

  retval->insertAtTail(iterInit);
  retval->insertAtTail(new BlockStmt(iterMove, BLOCK_TYPE));

  retval->insertAtTail(loop);

  retval->insertAtTail(new DefExpr(breakLabel));
  retval->insertAtTail(new CallExpr("_freeIterator", iterator));

  return retval;
}

BlockStmt* ForLoop::buildCForLoop(CallExpr* call, BlockStmt* body) 
{
  // Regular loop setup
  BlockStmt*   loop          = new BlockStmt(body);
  LabelSymbol* continueLabel = new LabelSymbol("_continueLabel");
  LabelSymbol* breakLabel    = new LabelSymbol("_breakLabel");
  BlockStmt*   retval        = buildChapelStmt();

  // C for loops have the form:
  //   __primitive("C for loop", initExpr, testExpr, incrExpr)
  //
  // This simply wraps the init, test, and incr expr with block stmts
  call->get(1)->replace(new BlockStmt(call->get(1)->copy()));
  call->get(2)->replace(new BlockStmt(call->get(2)->copy()));
  call->get(3)->replace(new BlockStmt(call->get(3)->copy()));

  loop->blockInfoSet(call);

  loop->continueLabel = continueLabel;
  loop->breakLabel    = breakLabel;

  loop->insertAtTail(new DefExpr(continueLabel));

  retval->insertAtTail(loop);
  retval->insertAtTail(new DefExpr(breakLabel));

  return retval;
}

/************************************ | *************************************
*                                                                           *
* Instance methods                                                          *
*                                                                           *
************************************* | ************************************/

ForLoop::ForLoop(BlockStmt* initBody) : BlockStmt(initBody)
{

}

ForLoop::~ForLoop()
{

}

ForLoop* ForLoop::copy(SymbolMap* mapRef, bool internal) 
{
  SymbolMap  localMap;
  SymbolMap* map       = (mapRef != 0) ? mapRef : &localMap;
  CallExpr*  blockInfo = blockInfoGet();
  ForLoop*   retval    = new ForLoop(NULL);

  retval->astloc        = astloc;
  retval->blockTag      = blockTag;

  retval->breakLabel    = breakLabel;
  retval->continueLabel = continueLabel;

  if (blockInfo != 0)
    retval->blockInfoSet(blockInfo->copy(map, true));

  if (modUses   != 0)
    retval->modUses = modUses->copy(map, true);

  if (byrefVars != 0)
    retval->byrefVars = byrefVars->copy(map, true);

  for_alist(expr, body)
    retval->insertAtTail(expr->copy(map, true));

  if (internal == false)
    update_symbols(retval, map);

  return retval;
}
