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

#include "astutil.h"
#include "AstVisitor.h"
#include "build.h"
#include "codegen.h"

#include <algorithm>

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
  ForLoop*     loop          = new ForLoop(index, iterator, body);
  LabelSymbol* continueLabel = new LabelSymbol("_continueLabel");
  LabelSymbol* breakLabel    = new LabelSymbol("_breakLabel");
  BlockStmt*   retval        = new BlockStmt();

  iterator->addFlag(FLAG_EXPR_TEMP);

  // Unzippered loop, treat all objects (including tuples) the same
  if (zippered == false)
    iterInit = new CallExpr(PRIM_MOVE, iterator, new CallExpr("_getIterator",    iteratorExpr));

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

/************************************ | *************************************
*                                                                           *
* Instance methods                                                          *
*                                                                           *
************************************* | ************************************/

ForLoop::ForLoop()
{

}

ForLoop::ForLoop(VarSymbol* index,
                 VarSymbol* iterator,
                 BlockStmt* initBody) : BlockStmt(initBody)
{
  forInfoSet(new CallExpr(PRIM_BLOCK_FOR_LOOP, index, iterator));
}

ForLoop::~ForLoop()
{

}

ForLoop* ForLoop::copy(SymbolMap* mapRef, bool internal)
{
  SymbolMap  localMap;
  SymbolMap* map       = (mapRef != 0) ? mapRef : &localMap;
  CallExpr*  blockInfo = forInfoGet();
  ForLoop*   retval    = new ForLoop();

  retval->astloc        = astloc;
  retval->blockTag      = blockTag;

  retval->breakLabel    = breakLabel;
  retval->continueLabel = continueLabel;

  if (blockInfo != 0)
    retval->forInfoSet(blockInfo->copy(map, true));

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

BlockStmt* ForLoop::copyBody()
{
  SymbolMap map;

  return copyBody(&map);
}

BlockStmt* ForLoop::copyBody(SymbolMap* map)
{
  BlockStmt* retval = new BlockStmt();

  retval->astloc        = astloc;
  retval->blockTag      = blockTag;

  retval->breakLabel    = breakLabel;
  retval->continueLabel = continueLabel;

  if (modUses   != 0)
    retval->modUses = modUses->copy(map, true);

  if (byrefVars != 0)
    retval->byrefVars = byrefVars->copy(map, true);

  for_alist(expr, body)
    retval->insertAtTail(expr->copy(map, true));

  update_symbols(retval, map);

  return retval;
}

bool ForLoop::isLoop() const
{
  // Noakes 2014/10/23.
  // There are operations can clear the blockInfo
  // i.e. convert a ForLoop back to a BlockStmt.
  return (forInfoGet() != 0) ? true : false;
}

bool ForLoop::isForLoop() const
{
  return forInfoGet() && forInfoGet()->isPrimitive(PRIM_BLOCK_FOR_LOOP);
}

// NOAKES 2014/11/18   This might be needed during transition
bool ForLoop::isCForLoop() const
{
  return forInfoGet() && forInfoGet()->isPrimitive(PRIM_BLOCK_C_FOR_LOOP);
}

SymExpr* ForLoop::indexGet() const
{
  CallExpr* callExpr = forInfoGet();
  SymExpr*  retval   = toSymExpr(callExpr->get(1));

  if (retval == 0)
    printf("ForLoop::index      Unexpected NULL result\n");

  return retval;
}

SymExpr* ForLoop::iteratorGet() const
{
  CallExpr* callExpr = forInfoGet();
  SymExpr*  retval   = toSymExpr(callExpr->get(2));

  if (retval == 0)
    printf("ForLoop::iterator   Unexpected NULL result\n");

  return retval;
}

// NOAKES 2014/11/26   Transitional
CallExpr* ForLoop::forInfoGet() const
{
 return BlockStmt::blockInfoGet();
}

CallExpr* ForLoop::forInfoSet(CallExpr* info)
{
  return BlockStmt::blockInfoSet(info);
}

CallExpr* ForLoop::blockInfoGet() const
{
  printf("Migration: ForLoop   %12d Unexpected call to blockInfoGet()\n", id);

  return 0;
}

CallExpr* ForLoop::blockInfoSet(CallExpr* expr)
{
  printf("Migration: ForLoop   %12d Unexpected call to blockInfoSet()\n", id);

  return 0;
}

bool ForLoop::deadBlockCleanup()
{
  bool retval = false;

  if (CallExpr* loop = forInfoGet())
  {
    if (BlockStmt* test = toBlockStmt(loop->get(2)))
    {
      if (test->body.length == 0)
      {
        remove();
        retval = true;
      }
    }
  }

  return retval;
}

void ForLoop::verify()
{
  BlockStmt::verify();

  if (BlockStmt::blockInfoGet() == 0)
    INT_FATAL(this, "ForLoop::verify. blockInfo is not NULL");

  if (forInfoGet() == 0)
    INT_FATAL(this, "ForLoop::verify. forInfo is NULL");

  if (forInfoGet()->isPrimitive(PRIM_BLOCK_FOR_LOOP) == false)
    INT_FATAL(this, "ForLoop::verify. blockInfo type is not PRIM_BLOCK_FOR_LOOP");

  if (modUses   != 0)
    INT_FATAL(this, "ForLoop::verify. modUses   is not NULL");

  if (byrefVars != 0)
    INT_FATAL(this, "ForLoop::verify. byrefVars is not NULL");
}

GenRet ForLoop::codegen()
{
  GenRet ret;

  INT_FATAL(this, "ForLoop::codegen This should be unreachable");

  return ret;
}

void ForLoop::accept(AstVisitor* visitor)
{
  if (visitor->enterForLoop(this) == true)
  {
    CallExpr* blockInfo = forInfoGet();

    for_alist(next_ast, body)
      next_ast->accept(visitor);

    if (blockInfo)
      blockInfo->accept(visitor);

    if (modUses)
      modUses->accept(visitor);

    if (byrefVars)
      byrefVars->accept(visitor);

    visitor->exitForLoop(this);
  }
}

Expr* ForLoop::getFirstExpr()
{
  Expr* retval = 0;

  if (forInfoGet() != 0)
    retval = forInfoGet()->getFirstExpr();

  else if (body.head      != 0)
    retval = body.head->getFirstExpr();

  else
    retval = this;

  return retval;
}

Expr* ForLoop::getNextExpr(Expr* expr)
{
  Expr* retval = NULL;

  if (expr == forInfoGet() && body.head != NULL)
    retval = body.head->getFirstExpr();

  return retval;
}
