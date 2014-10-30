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
#include "codegen.h"

/************************************ | *************************************
*                                                                           *
* Factory methods for the Parser                                            *
*                                                                           *
************************************* | ************************************/

BlockStmt* DoWhileStmt::build(Expr* cond, BlockStmt* body) 
{
  VarSymbol*   condVar       = newTemp();
  CallExpr*    condTest      = new CallExpr("_cond_test", cond);
  LabelSymbol* continueLabel = new LabelSymbol("_continueLabel");
  LabelSymbol* breakLabel    = new LabelSymbol("_breakLabel");
  DoWhileStmt* loop          = 0;
  BlockStmt*   retval        = new BlockStmt();

  // make variables declared in the scope of the body visible to
  // expressions in the condition of a do..while block
  if (body->length() == 1 && toBlockStmt(body->body.only())) {
    body = toBlockStmt(body->body.only());
    body->remove();
  }

  body->insertAtTail(new DefExpr(continueLabel));
  body->insertAtTail(new CallExpr(PRIM_MOVE, condVar, condTest->copy()));

  loop = new DoWhileStmt(body);

  loop->blockInfoSet(new CallExpr(PRIM_BLOCK_DOWHILE_LOOP, condVar));

  loop->continueLabel = continueLabel;
  loop->breakLabel    = breakLabel;

  retval->insertAtTail(new DefExpr(condVar));

  retval->insertAtTail(loop);

  retval->insertAtTail(new DefExpr(breakLabel));

  return retval;
}
/************************************ | *************************************
*                                                                           *
* Instance methods                                                          *
*                                                                           *
************************************* | ************************************/

DoWhileStmt::DoWhileStmt(BlockStmt* initBody) : WhileStmt(initBody)
{

}

DoWhileStmt::~DoWhileStmt()
{

}

DoWhileStmt* DoWhileStmt::copy(SymbolMap* map, bool internal) 
{
  DoWhileStmt* retval = new DoWhileStmt(NULL);

  retval->copyShare(*this, map, internal);

  return retval;
}

bool DoWhileStmt::isDoWhileLoop() const
{
  return true;
}

GenRet DoWhileStmt::codegen() 
{
  GenInfo* info    = gGenInfo;
  FILE*    outfile = info->cfile;
  GenRet   ret;

  codegenStmt(this);

  if (outfile)
  {
    CallExpr* blockInfo = blockInfoGet();

    info->cStatements.push_back("do ");

    if (this != getFunction()->body)
      info->cStatements.push_back("{\n");

    body.codegen("");

    std::string ftr= "} while (" + codegenValue(blockInfo->get(1)).c + ");\n";

    info->cStatements.push_back(ftr);
  } 

  else 
  {
#ifdef HAVE_LLVM
    llvm::Function*   func             = info->builder->GetInsertBlock()->getParent();

    llvm::BasicBlock* blockStmtBody    = NULL;
    llvm::BasicBlock* blockStmtEnd     = NULL;
    llvm::BasicBlock* blockStmtEndCond = NULL;

    CallExpr*         blockInfo        = blockInfoGet();

    getFunction()->codegenUniqueNum++;

    blockStmtBody = llvm::BasicBlock::Create(info->module->getContext(), FNAME("blk_body"));
    blockStmtEnd  = llvm::BasicBlock::Create(info->module->getContext(), FNAME("blk_end"));

    info->builder->CreateBr(blockStmtBody);

    // Now add the body.
    func->getBasicBlockList().push_back(blockStmtBody);

    info->builder->SetInsertPoint(blockStmtBody);
    info->lvt->addLayer();

    body.codegen("");

    info->lvt->removeLayer();
    
    // Add the condition block.
    blockStmtEndCond = llvm::BasicBlock::Create(info->module->getContext(), FNAME("blk_end_cond"));

    func->getBasicBlockList().push_back(blockStmtEndCond);

    // Insert an explicit branch from the body block to the loop condition.
    info->builder->CreateBr(blockStmtEndCond);

    // set insert point
    info->builder->SetInsertPoint(blockStmtEndCond);

    GenRet       condValueRet = codegenValue(blockInfo->get(1));
    llvm::Value* condValue    = condValueRet.val;

    if (condValue->getType() != llvm::Type::getInt1Ty(info->module->getContext()))
    {
      condValue = info->builder->CreateICmpNE(condValue,
                                              llvm::ConstantInt::get(condValue->getType(), 0),
                                              FNAME("condition"));
    }

    info->builder->CreateCondBr(condValue, blockStmtBody, blockStmtEnd);

    func->getBasicBlockList().push_back(blockStmtEnd);

    info->builder->SetInsertPoint(blockStmtEnd);

    if (blockStmtBody   ) INT_ASSERT(blockStmtBody->getParent()    == func);
    if (blockStmtEndCond) INT_ASSERT(blockStmtEndCond->getParent() == func);
    if (blockStmtEnd    ) INT_ASSERT(blockStmtEnd->getParent()     == func);
#endif
  }

  INT_ASSERT(!byrefVars); // these should not persist past parallel()

  return ret;
}
