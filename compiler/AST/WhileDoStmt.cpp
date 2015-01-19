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

#include "WhileDoStmt.h"

#include "AstVisitor.h"
#include "build.h"
#include "CForLoop.h"
#include "codegen.h"

/************************************ | *************************************
*                                                                           *
* Factory methods for the Parser                                            *
*                                                                           *
************************************* | ************************************/

BlockStmt* WhileDoStmt::build(Expr* cond, BlockStmt* body)
{
  BlockStmt* retval = NULL;

  if (isPrimitiveCForLoop(cond) == true)
  {
    retval = CForLoop::buildCForLoop(toCallExpr(cond), body);
  }

  else if (fUseIPE == false)
  {
    VarSymbol*   condVar       = newTemp();
    CallExpr*    condTest      = new CallExpr("_cond_test", cond);

    LabelSymbol* continueLabel = new LabelSymbol("_continueLabel");
    LabelSymbol* breakLabel    = new LabelSymbol("_breakLabel");

    WhileDoStmt* loop          = new WhileDoStmt(condVar, body);

    loop->mContinueLabel = continueLabel;
    loop->mBreakLabel    = breakLabel;

    loop->insertAtTail(new DefExpr(continueLabel));
    loop->insertAtTail(new CallExpr(PRIM_MOVE, condVar, condTest->copy()));

    retval = new BlockStmt();

    retval->insertAtTail(new DefExpr(condVar));
    retval->insertAtTail(new CallExpr(PRIM_MOVE, condVar, condTest->copy()));
    retval->insertAtTail(loop);
    retval->insertAtTail(new DefExpr(breakLabel));
  }

  else
  {
    CallExpr* condTest = new CallExpr("_cond_test", cond);

    retval = new WhileDoStmt(condTest, body);
  }

  return retval;
}

// C for loops are invoked with 'while __primitive("C for loop" ...)'
// This checks if we had such a case and if we did builds the c for loop
// instead of the while loop and returns it.
bool WhileDoStmt::isPrimitiveCForLoop(Expr* cond)
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

WhileDoStmt::WhileDoStmt(Expr* cond, BlockStmt* body) :
WhileStmt(cond, body)
{

}

WhileDoStmt::WhileDoStmt(VarSymbol* var, BlockStmt* body) :
  WhileStmt(var, body)
{

}

WhileDoStmt::~WhileDoStmt()
{

}

WhileDoStmt* WhileDoStmt::copy(SymbolMap* map, bool internal)
{
  Expr*        condExpr = 0;
  BlockStmt*   body     = 0;
  WhileDoStmt* retval   = new WhileDoStmt(condExpr, body);

  retval->copyShare(*this, map, internal);

  return retval;
}

bool WhileDoStmt::isWhileDoStmt() const
{
  return true;
}

GenRet WhileDoStmt::codegen()
{
  GenInfo* info    = gGenInfo;
  FILE*    outfile = info->cfile;
  GenRet   ret;

  codegenStmt(this);

  if (outfile)
  {
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

  INT_ASSERT(!byrefVars); // these should not persist past parallel()

  return ret;
}

void WhileDoStmt::accept(AstVisitor* visitor)
{
  if (visitor->enterWhileDoStmt(this) == true)
  {
    for_alist(next_ast, body)
      next_ast->accept(visitor);

    if (condExprGet() != 0)
      condExprGet()->accept(visitor);

    if (modUses)
      modUses->accept(visitor);

    if (byrefVars)
      byrefVars->accept(visitor);

    visitor->exitWhileDoStmt(this);
  }
}

Expr* WhileDoStmt::getFirstExpr()
{
  Expr* retval = 0;

  if (condExprGet() != 0)
    retval = condExprGet()->getFirstExpr();

  else if (body.head      != 0)
    retval = body.head->getFirstExpr();

  else
    retval = this;

  return retval;
}

Expr* WhileDoStmt::getNextExpr(Expr* expr)
{
  Expr* retval = this;

  if (expr == condExprGet() && body.head != NULL)
    retval = body.head->getFirstExpr();

  return retval;
}
