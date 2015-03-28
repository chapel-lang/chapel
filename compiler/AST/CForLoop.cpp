/*
 * Copyright 2004-2015 Cray Inc.
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
#include "codegen.h"
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
// a BlockStmt that is presumed to be one of the header claiuses
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

void CForLoop::insertAtExit(Expr* expr)
{ incrBlockGet()->insertAtTail(expr); }

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

  if (modUses                   != 0)
    INT_FATAL(this, "CForLoop::verify. modUses   is not NULL");

  if (byrefVars                 != 0)
    INT_FATAL(this, "CForLoop::verify. byrefVars is not NULL");
}

GenRet CForLoop::codegen()
{
  GenInfo* info    = gGenInfo;
  FILE*    outfile = info->cfile;
  GenRet   ret;

  codegenStmt(this);

  if (outfile)
  {
    BlockStmt*  initBlock = initBlockGet();

    // These copy calls are needed or else values get code generated twice.
    std::string init      = codegenCForLoopHeader(initBlock->copy());

    BlockStmt*  testBlock = testBlockGet();
    std::string test      = codegenCForLoopHeader(testBlock->copy());

    // wrap the test with paren. Could probably check if it already has
    // outer paren to make the code a little cleaner.
    if (test != "")
      test = "(" + test + ")";

    BlockStmt*  incrBlock = incrBlockGet();
    std::string incr      = codegenCForLoopHeader(incrBlock->copy());
    std::string hdr       = "for (" + init + "; " + test + "; " + incr + ") ";

    codegenOrderIndependence();

    info->cStatements.push_back(hdr);

    if (this != getFunction()->body)
      info->cStatements.push_back("{\n");

    body.codegen("");

    if (this != getFunction()->body)
    {
      std::string end  = "}";
      CondStmt*   cond = toCondStmt(parentExpr);

      if (!cond || !(cond->thenStmt == this && cond->elseStmt))
        end += "\n";

      info->cStatements.push_back(end);
    }
  }

  else
  {
#ifdef HAVE_LLVM
    llvm::Function*   func          = info->builder->GetInsertBlock()->getParent();

    llvm::BasicBlock* blockStmtInit = NULL;
    llvm::BasicBlock* blockStmtBody = NULL;
    llvm::BasicBlock* blockStmtEnd  = NULL;

    BlockStmt*        initBlock     = initBlockGet();
    BlockStmt*        testBlock     = testBlockGet();
    BlockStmt*        incrBlock     = incrBlockGet();

    assert(initBlock && testBlock && incrBlock);

    getFunction()->codegenUniqueNum++;

    blockStmtBody = llvm::BasicBlock::Create(info->module->getContext(), FNAME("blk_body"));
    blockStmtEnd  = llvm::BasicBlock::Create(info->module->getContext(), FNAME("blk_end"));

    // In order to track more easily with the C backend and because mem2reg should optimize
    // all of these cases, we generate a for loop as the same as
    // if(cond) do { body; step; } while(cond).

    // Create the init basic block
    blockStmtInit = llvm::BasicBlock::Create(info->module->getContext(), FNAME("blk_c_for_init"));

    func->getBasicBlockList().push_back(blockStmtInit);

    // Insert an explicit branch from the current block to the init block
    info->builder->CreateBr(blockStmtInit);

    // Now switch to the init block for code generation
    info->builder->SetInsertPoint(blockStmtInit);

    // Code generate the init block.
    initBlock->body.codegen("");

    // Add the loop condition to figure out if we run the loop at all.
    GenRet       test0      = codegenCForLoopCondition(testBlock);
    llvm::Value* condValue0 = test0.val;

    // Normalize it to boolean
    if (condValue0->getType() != llvm::Type::getInt1Ty(info->module->getContext()))
      condValue0 = info->builder->CreateICmpNE(condValue0,
                                               llvm::ConstantInt::get(condValue0->getType(), 0),
                                               FNAME("condition"));

    // Create the conditional branch
    info->builder->CreateCondBr(condValue0, blockStmtBody, blockStmtEnd);

    // Now add the body.
    func->getBasicBlockList().push_back(blockStmtBody);

    info->builder->SetInsertPoint(blockStmtBody);
    info->lvt->addLayer();

    body.codegen("");

    info->lvt->removeLayer();

    incrBlock->body.codegen("");

    GenRet       test1      = codegenCForLoopCondition(testBlock);
    llvm::Value* condValue1 = test1.val;

    // Normalize it to boolean
    if (condValue1->getType() != llvm::Type::getInt1Ty(info->module->getContext()))
      condValue1 = info->builder->CreateICmpNE(condValue1,
                                               llvm::ConstantInt::get(condValue1->getType(), 0),
                                               FNAME("condition"));

    // Create the conditional branch
    info->builder->CreateCondBr(condValue1, blockStmtBody, blockStmtEnd);

    func->getBasicBlockList().push_back(blockStmtEnd);

    info->builder->SetInsertPoint(blockStmtEnd);

    if (blockStmtBody) INT_ASSERT(blockStmtBody->getParent() == func);
    if (blockStmtEnd ) INT_ASSERT(blockStmtEnd->getParent()  == func);
#endif
  }

  INT_ASSERT(!byrefVars); // these should not persist past parallel()

  return ret;
}

// This function is used to codegen the init, test, and incr segments of c for
// loops. In c for loops instead of using statements comma operators must be
// used. So for the init instead of generating something like:
//   i = 4;
//   j = 4;
//
// We need to generate:
// i = 4, j = 4
std::string CForLoop::codegenCForLoopHeader(BlockStmt* block)
{
  GenInfo*    info = gGenInfo;
  std::string seg  = "";

  for_alist(expr, block->body)
  {
    CallExpr* call = toCallExpr(expr);

    // Generate defExpr normally (they always get codegenned at the top of a
    // function currently, if that changes this code will probably be wrong.)
    if (DefExpr* defExpr = toDefExpr(expr))
    {
      defExpr->codegen();
    }

    // If inlining is off, the init, test, and incr are just functions and we
    // need to generate them inline so we use codegenValue. The semicolon is
    // added so it can be replaced with the comma later. If inlinining is on
    // the test will be a <= and it also needs to be codegenned with
    // codegenValue.
    //
    // TODO when the test operator is user specifiable and not just <= this
    // will need to be updated to include all possible conditionals. (I'm
    // imagining we'll want a separate function that can check if a primitive
    // is a conditional as I think we'll need that info elsewhere.)
    else if (call && (call->isResolved() || isRelationalOperator(call)))
    {
      std::string callStr = codegenValue(call).c;

      if (callStr != "")
      {
        seg += callStr + ';';
      }
    }

    // Similar to above, generate symExprs
    else if (SymExpr* symExpr = toSymExpr(expr))
    {
      std::string symStr = codegenValue(symExpr).c;

      if (symStr != "")
      {
        seg += symStr + ';';
      }
    }

    // Everything else is just a bunch of statements. We do normal codegen() on
    // them which ends up putting whatever got codegenned into CStatements. We
    // pop all of those back off (note that the order we pop and attach to our
    // segment is important.)
    else
    {
      int prevStatements = (int) info->cStatements.size();

      expr->codegen();

      int newStatements  = (int) info->cStatements.size() - prevStatements;

      for (std::vector<std::string>::iterator it = info->cStatements.end() - newStatements;
           it != info->cStatements.end();
           ++it)
      {
        seg += *it;
      }

      info->cStatements.erase(info->cStatements.end() - newStatements,
                              info->cStatements.end());
    }
  }

  // replace all the semicolons (from "statements") with commas
  std::replace(seg.begin(), seg.end(), ';', ',');

  // remove all the newlines
  seg.erase(std::remove(seg.begin(), seg.end(), '\n'), seg.end());

  // remove the last character if any were generated (it's a trailing comma
  // since we previously had an appropriate "trailing" semicolon
  if (seg.size () > 0)
    seg.resize (seg.size () - 1);

  return seg;
}

GenRet CForLoop::codegenCForLoopCondition(BlockStmt* block)
{
  GenRet ret;

#ifdef HAVE_LLVM
  for_alist(expr, block->body)
  {
    ret = expr->codegen();
  }

  return codegenValue(ret);

#else

  return ret;

#endif
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
