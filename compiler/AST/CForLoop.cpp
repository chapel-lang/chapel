/*
 * Copyright 2004-2020 Cray Inc.
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

#include "CForLoop.h"

#include "astutil.h"
#include "AstVisitor.h"
#include "build.h"
#include "ForLoop.h"

#include <algorithm>

/************************************ | *************************************
*                                                                           *
* Factory methods for the Parser                                            *
*                                                                           *
************************************* | ************************************/

// A WhileDo loop may have a C_FOR_LOOP prim as the termination condition
BlockStmt* CForLoop::buildCForLoop(CallExpr* call, BlockStmt* body)
{
  BlockStmt* retval = buildChapelStmt();

  if (call->isPrimitive(PRIM_BLOCK_C_FOR_LOOP) == true)
  {
    CForLoop*    loop          = new CForLoop(body);

    Expr*        initClause    = call->get(1)->copy();
    Expr*        testClause    = call->get(2)->copy();
    Expr*        incrClause    = call->get(3)->copy();

    BlockStmt*   initBlock     = new BlockStmt(initClause, BLOCK_C_FOR_LOOP);
    BlockStmt*   testBlock     = new BlockStmt(testClause, BLOCK_C_FOR_LOOP);
    BlockStmt*   incrBlock     = new BlockStmt(incrClause, BLOCK_C_FOR_LOOP);

    LabelSymbol* continueLabel = new LabelSymbol("_continueLabel");
    LabelSymbol* breakLabel    = new LabelSymbol("_breakLabel");

    loop->mContinueLabel = continueLabel;
    loop->mBreakLabel    = breakLabel;

    loop->loopHeaderSet(initBlock, testBlock, incrBlock);

    loop->insertAtTail(new DefExpr(continueLabel));

    retval->insertAtTail(loop);
    retval->insertAtTail(new DefExpr(breakLabel));
  }
  else
  {
    INT_ASSERT(false);
  }

  return retval;
}

CForLoop* CForLoop::buildWithBodyFrom(ForLoop* forLoop)
{
  SymbolMap map;
  CForLoop* retval = new CForLoop();

  retval->astloc            = forLoop->astloc;
  retval->blockTag          = forLoop->blockTag;
  retval->mBreakLabel       = forLoop->breakLabelGet();
  retval->mContinueLabel    = forLoop->continueLabelGet();
  retval->mOrderIndependent = forLoop->isOrderIndependent();

  for_alist(expr, forLoop->body)
    retval->insertAtTail(expr->copy(&map, true));

  update_symbols(retval, &map);

  return retval;
}

// Provide an abstraction around a requirement to find the CForLoop for
// a BlockStmt that is presumed to be one of the header clauses
CForLoop* CForLoop::loopForClause(BlockStmt* clause)
{
  CForLoop* retval = toCForLoop(clause->parentExpr);

  INT_ASSERT(clause->blockTag == BLOCK_C_FOR_LOOP);
  INT_ASSERT(retval);

  return retval;
}

/************************************ | *************************************
*                                                                           *
* Instance methods                                                          *
*                                                                           *
************************************* | ************************************/

CForLoop::CForLoop() : LoopStmt(0)
{
  mInitClause = 0;
  mTestClause = 0;
  mIncrClause = 0;
}

CForLoop::CForLoop(BlockStmt* initBody) : LoopStmt(initBody)
{
  mInitClause = 0;
  mTestClause = 0;
  mIncrClause = 0;
}

CForLoop::~CForLoop()
{

}

CForLoop* CForLoop::copy(SymbolMap* mapRef, bool internal)
{
  SymbolMap  localMap;
  SymbolMap* map       = (mapRef != 0) ? mapRef : &localMap;
  CForLoop*  retval    = new CForLoop();

  retval->astloc            = astloc;
  retval->blockTag          = blockTag;

  retval->mBreakLabel       = mBreakLabel;
  retval->mContinueLabel    = mContinueLabel;
  retval->mOrderIndependent = mOrderIndependent;

  if (initBlockGet() != 0 && testBlockGet() != 0 && incrBlockGet() != 0)
    retval->loopHeaderSet(initBlockGet()->copy(map, true),
                          testBlockGet()->copy(map, true),
                          incrBlockGet()->copy(map, true));

  else if (initBlockGet() != 0 && testBlockGet() != 0 && incrBlockGet() != 0)
    INT_ASSERT(false);

  for_alist(expr, body)
    retval->insertAtTail(expr->copy(map, true));

  if (internal == false)
    update_symbols(retval, map);

  return retval;
}

bool CForLoop::isCForLoop() const
{
  return true;
}

void CForLoop::loopHeaderSet(BlockStmt* initBlock,
                             BlockStmt* testBlock,
                             BlockStmt* incrBlock)
{
  initBlock->blockTag = BLOCK_C_FOR_LOOP;
  testBlock->blockTag = BLOCK_C_FOR_LOOP;
  incrBlock->blockTag = BLOCK_C_FOR_LOOP;

  mInitClause = initBlock;
  mTestClause = testBlock;
  mIncrClause = incrBlock;
}

BlockStmt* CForLoop::initBlockGet() const
{
  return mInitClause;
}

BlockStmt* CForLoop::testBlockGet() const
{
  return mTestClause;
}

BlockStmt* CForLoop::incrBlockGet() const
{
  return mIncrClause;
}

CallExpr* CForLoop::blockInfoGet() const
{
  printf("Migration: CForLoop  %12d Unexpected call to blockInfoGet()\n", id);

  return 0;
}

CallExpr* CForLoop::blockInfoSet(CallExpr* expr)
{
  printf("Migration: CForLoop  %12d Unexpected call to blockInfoSet()\n", id);

  return 0;
}

bool CForLoop::deadBlockCleanup()
{
  bool retval = false;

  if (BlockStmt* test = testBlockGet())
  {
    if (test->body.length == 0)
    {
      remove();
      retval = true;
    }
  }

  return retval;
}

void CForLoop::verify()
{
  BlockStmt::verify();

  if (BlockStmt::blockInfoGet() != 0)
    INT_FATAL(this, "CForLoop::verify. blockInfo is NULL");

  if (initBlockGet()            == 0)
    INT_FATAL(this, "CForLoop::verify. initBlock is NULL");

  if (testBlockGet()            == 0)
    INT_FATAL(this, "CForLoop::verify. testBlock is NULL");

  if (incrBlockGet()            == 0)
    INT_FATAL(this, "CForLoop::verify. incrBlock is NULL");

  if (initBlockGet()->blockTag  != BLOCK_C_FOR_LOOP)
    INT_FATAL(this, "CForLoop::verify. initBlock is not BLOCK_C_FOR_LOOP");

  if (testBlockGet()->blockTag  != BLOCK_C_FOR_LOOP)
    INT_FATAL(this, "CForLoop::verify. testBlock is not BLOCK_C_FOR_LOOP");

  if (incrBlockGet()->blockTag  != BLOCK_C_FOR_LOOP)
    INT_FATAL(this, "CForLoop::verify. incrBlock is not BLOCK_C_FOR_LOOP");

  if (useList                   != 0)
    INT_FATAL(this, "CForLoop::verify. useList   is not NULL");

  if (byrefVars                 != 0)
    INT_FATAL(this, "CForLoop::verify. byrefVars is not NULL");
}

void CForLoop::accept(AstVisitor* visitor)
{
  if (visitor->enterCForLoop(this) == true)
  {
    for_alist(next_ast, body)
      next_ast->accept(visitor);

    if (initBlockGet() != 0)
      initBlockGet()->accept(visitor);

    if (testBlockGet() != 0)
      testBlockGet()->accept(visitor);

    if (incrBlockGet() != 0)
      incrBlockGet()->accept(visitor);

    visitor->exitCForLoop(this);
  }
}

Expr* CForLoop::getFirstExpr()
{
  Expr* retval = 0;

  if (mInitClause != 0)
  {
    INT_ASSERT(mTestClause != 0);
    INT_ASSERT(mIncrClause != 0);

    retval = mInitClause->getFirstExpr();
  }

  else if (body.head != 0)
  {
    INT_ASSERT(mTestClause == 0);
    INT_ASSERT(mIncrClause == 0);

    retval = body.head->getFirstExpr();
  }

  else
    retval = this;

  return retval;
}

Expr* CForLoop::getNextExpr(Expr* expr)
{
  Expr* retval = 0;

  if (expr == mInitClause)
  {
    INT_ASSERT(mTestClause != 0);
    INT_ASSERT(mIncrClause != 0);

    retval = mTestClause->getFirstExpr();
  }

  else if (expr == mTestClause)
    retval = mIncrClause->getFirstExpr();

  else if (expr == mIncrClause && body.head != NULL)
    retval = body.head->getFirstExpr();

  else
    retval = this;

  return retval;
}
