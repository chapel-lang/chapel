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
#include "codegen.h"
#include "driver.h"
#include "ForLoop.h"
#include "LayeredValueTable.h"

#ifdef HAVE_LLVM
#include "llvm/IR/Module.h"
#endif

#include <algorithm>

#ifdef HAVE_LLVM


static llvm::MDNode* generateLoopMetadata()
{
  GenInfo* info = gGenInfo;
  auto &ctx = info->module->getContext();

  std::vector<llvm::Metadata*> args;
  auto tmpNode        = llvm::MDNode::getTemporary(ctx, llvm::None);
  args.push_back(tmpNode.get());

  // llvm.loop.vectorize.enable metadata is only used by LoopVectorizer to:
  // 1) Explicitly disable vectorization of particular loop
  // 2) Print warning when vectorization is enabled (using metadata) and vectorization didn't occur
  // It is however required for the Region Vectorizer
  if(fRegionVectorizer)
  {
    llvm::Metadata *loopVectorizeEnable[] = { llvm::MDString::get(ctx, "rv.loop.vectorize.enable"),
                                              llvm::ConstantAsMetadata::get(llvm::ConstantInt::get(llvm::Type::getInt1Ty(ctx), true))};
    args.push_back(llvm::MDNode::get(ctx, loopVectorizeEnable));

    /* Region Vectorizer no longer needs loop width
    if (fRegionVectorizer) {
      // Region Vectorizer needs loop width to be specified
      llvm::Metadata *loopVectorWidth[] = { llvm::MDString::get(ctx, "llvm.loop.vectorize.width"),
                                            llvm::ConstantAsMetadata::get(llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 8))};
      args.push_back(llvm::MDNode::get(ctx, loopVectorWidth));
    }*/

  }

  llvm::MDNode *loopMetadata = llvm::MDNode::get(ctx, args);
  loopMetadata->replaceOperandWith(0, loopMetadata);
  return loopMetadata;
}

static void addLoopMetadata(llvm::Instruction* instruction, llvm::MDNode* loopMetadata)
{
  instruction->setMetadata("llvm.loop", loopMetadata);
}

#endif

/************************************ | *************************************
*                                                                           *
* Instance methods                                                          *
*                                                                           *
************************************* | ************************************/

GenRet CForLoop::codegen()
{
  GenInfo* info    = gGenInfo;
  FILE*    outfile = info->cfile;
  GenRet   ret;

  codegenStmt(this);

  fixVectorizable();

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

    codegenVectorHint();

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
    llvm::Function*   func          = info->irBuilder->GetInsertBlock()->getParent();

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

    // However it is appealing to generate these low-level loops directly
    // in LLVM IR:
    //   * could avoid repeated loads
    //   * could simplify generated IR
    //   * could avoid problems identifying induction variables

    // Create the init basic block
    blockStmtInit = llvm::BasicBlock::Create(info->module->getContext(), FNAME("blk_c_for_init"));

    func->getBasicBlockList().push_back(blockStmtInit);

    // Insert an explicit branch from the current block to the init block
    info->irBuilder->CreateBr(blockStmtInit);

    // Now switch to the init block for code generation
    info->irBuilder->SetInsertPoint(blockStmtInit);

    // Code generate the init block.
    initBlock->body.codegen("");

    // Add the loop condition to figure out if we run the loop at all.
    GenRet       test0      = codegenCForLoopCondition(testBlock);
    llvm::Value* condValue0 = test0.val;

    // Normalize it to boolean
    if (condValue0->getType() != llvm::Type::getInt1Ty(info->module->getContext()))
      condValue0 = info->irBuilder->CreateICmpNE(condValue0,
                                               llvm::ConstantInt::get(condValue0->getType(), 0),
                                               FNAME("condition"));

    // Create the conditional branch
    info->irBuilder->CreateCondBr(condValue0, blockStmtBody, blockStmtEnd);

    // Now add the body.
    func->getBasicBlockList().push_back(blockStmtBody);

    info->irBuilder->SetInsertPoint(blockStmtBody);
    info->lvt->addLayer();

    llvm::MDNode* loopMetadata = nullptr;
    if(fNoVectorize == false && isVectorizable()) {
      loopMetadata = generateLoopMetadata();
      info->loopStack.emplace(loopMetadata, true);
    }

    body.codegen("");

    if(loopMetadata)
      info->loopStack.pop();

    info->lvt->removeLayer();

    incrBlock->body.codegen("");

    GenRet       test1      = codegenCForLoopCondition(testBlock);
    llvm::Value* condValue1 = test1.val;

    // Normalize it to boolean
    if (condValue1->getType() != llvm::Type::getInt1Ty(info->module->getContext()))
      condValue1 = info->irBuilder->CreateICmpNE(condValue1,
                                                 llvm::ConstantInt::get(condValue1->getType(), 0),
                                                 FNAME("condition"));

    // Create the conditional branch
    llvm::Instruction* endLoopBranch = info->irBuilder->CreateCondBr(condValue1, blockStmtBody, blockStmtEnd);

    if(loopMetadata)
      addLoopMetadata(endLoopBranch, loopMetadata);

    func->getBasicBlockList().push_back(blockStmtEnd);

    info->irBuilder->SetInsertPoint(blockStmtEnd);

    if (blockStmtBody) INT_ASSERT(blockStmtBody->getParent() == func);
    if (blockStmtEnd ) INT_ASSERT(blockStmtEnd->getParent()  == func);
#endif
  }

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
    // added so it can be replaced with the comma later. If inlining is on
    // the test will be a <= and it also needs to be codegenned with
    // codegenValue.
    //
    // TODO when the test operator is user specifiable and not just <= this
    // will need to be updated to include all possible conditionals. (I'm
    // imagining we'll want a separate function that can check if a primitive
    // is a conditional as I think we'll need that info elsewhere.)
    else if (call && (call->isResolved() || isRelationalOperator(call) ||
        call->isPrimitive(PRIM_GET_MEMBER_VALUE)))
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
  // since we previously had an appropriate "trailing" semicolon)
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
