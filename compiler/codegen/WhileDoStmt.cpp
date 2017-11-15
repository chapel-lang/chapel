/*
 * Copyright 2004-2017 Cray Inc.
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
#include "codegen.h"
#include "LayeredValueTable.h"

#ifdef HAVE_LLVM
#include "llvm/IR/Module.h"
#endif


/************************************ | *************************************
*                                                                           *
* Instance methods                                                          *
*                                                                           *
************************************* | ************************************/

GenRet WhileDoStmt::codegen()
{
  GenInfo* info    = gGenInfo;
  FILE*    outfile = info->cfile;
  GenRet   ret;

  codegenStmt(this);

  if (outfile)
  {

    codegenOrderIndependence();

    std::string hdr = "while (" + codegenValue(condExprGet()).c + ") ";

    info->cStatements.push_back(hdr);

    if (this != getFunction()->body)
      info->cStatements.push_back("{\n");

    body.codegen("");

    if (this != getFunction()->body)
    {
      std::string end  = "}";
      CondStmt*   cond = toCondStmt(parentExpr);

      if (cond == 0 || !(cond->thenStmt == this && cond->elseStmt))
        end += "\n";

      info->cStatements.push_back(end);
    }
  }

  else
  {
#ifdef HAVE_LLVM
    llvm::Function*   func          = info->builder->GetInsertBlock()->getParent();
    llvm::BasicBlock* blockStmtBody = NULL;
    llvm::BasicBlock* blockStmtEnd  = NULL;
    llvm::BasicBlock* blockStmtCond = NULL;

    getFunction()->codegenUniqueNum++;

    blockStmtBody = llvm::BasicBlock::Create(info->module->getContext(), FNAME("blk_body"));
    blockStmtEnd  = llvm::BasicBlock::Create(info->module->getContext(), FNAME("blk_end"));
    blockStmtCond = llvm::BasicBlock::Create(info->module->getContext(), FNAME("blk_cond"));

    func->getBasicBlockList().push_back(blockStmtCond);

    // Insert an explicit branch from the current block to the loop start.
    info->builder->CreateBr(blockStmtCond);

    // Now switch to the condition for code generation
    info->builder->SetInsertPoint(blockStmtCond);

    GenRet            condValueRet     = codegenValue(condExprGet());
    llvm::Value*      condValue        = condValueRet.val;

    if (condValue->getType() != llvm::Type::getInt1Ty(info->module->getContext()))
    {
      condValue = info->builder->CreateICmpNE(condValue,
                                              llvm::ConstantInt::get(condValue->getType(), 0),
                                              FNAME("condition"));
    }

    // Now we might go either to the Body or to the End.
    info->builder->CreateCondBr(condValue, blockStmtBody, blockStmtEnd);

    // Now add the body.
    func->getBasicBlockList().push_back(blockStmtBody);

    info->builder->SetInsertPoint(blockStmtBody);
    info->lvt->addLayer();

    body.codegen("");

    info->lvt->removeLayer();

    if (blockStmtCond)
      info->builder->CreateBr(blockStmtCond);
    else
      info->builder->CreateBr(blockStmtEnd);

    func->getBasicBlockList().push_back(blockStmtEnd);

    info->builder->SetInsertPoint(blockStmtEnd);

    if (blockStmtCond) INT_ASSERT(blockStmtCond->getParent() == func);
    if (blockStmtBody) INT_ASSERT(blockStmtBody->getParent() == func);
    if (blockStmtEnd)  INT_ASSERT(blockStmtEnd->getParent()  == func);
#endif
  }

  return ret;
}
