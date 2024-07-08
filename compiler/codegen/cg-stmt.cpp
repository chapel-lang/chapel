/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#include "stmt.h"

#include "astutil.h"
#include "AstVisitor.h"
#include "codegen.h"
#include "driver.h"
#include "expr.h"
#include "files.h"
#include "ImportStmt.h"
#include "LayeredValueTable.h"
#include "llvmDebug.h"
#include "llvmTracker.h"
#include "llvmVer.h"
#include "misc.h"
#include "passes.h"
#include "stlUtil.h"
#include "stringutil.h"

#ifdef HAVE_LLVM
#include "llvm/IR/Module.h"
#endif

#include <algorithm>
#include <cstring>
#include <vector>

void codegenStmt(Expr* stmt) {
  GenInfo* info    = gGenInfo;
  FILE*    outfile = info->cfile;

  info->lineno   = stmt->linenum();
  info->filename = stmt->fname();

  if( outfile ) {
    if (printCppLineno && stmt->linenum() > 0)
        info->cStatements.push_back(zlineToString(stmt));
    if (fGenIDS)
      info->cStatements.push_back(idCommentTemp(stmt));
  } else {
#ifdef HAVE_LLVM
    if (debug_info && stmt->linenum() > 0) {
      // Adjust the current line number, but leave the scope alone.
      llvm::MDNode* scope;

      if(stmt->inTree() && stmt->parentSymbol->astTag == E_FnSymbol) {
        scope = debug_info->get_function((FnSymbol *)stmt->parentSymbol);
      } else {
        scope = info->irBuilder->getCurrentDebugLocation().getScope();
      }

#if HAVE_LLVM_VER >= 120
      info->irBuilder->SetCurrentDebugLocation(
                  llvm::DILocation::get(scope->getContext(), stmt->linenum(),
                                        /*col=*/ 0, scope, nullptr, false));
#else
      info->irBuilder->SetCurrentDebugLocation(
                  llvm::DebugLoc::get(stmt->linenum(),0 /*col*/,scope));
#endif
    }
#endif
  }

  ++gStmtCount;
}


/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

GenRet UseStmt::codegen() {
  GenRet ret;
  INT_FATAL(this, "UseStmt::codegen not implemented");
  return ret;
}

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

GenRet ImportStmt::codegen() {
  GenRet ret;
  INT_FATAL(this, "ImportStmt::codegen not implemented");
  return ret;
}

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

GenRet BlockStmt::codegen() {
  GenInfo* info    = gGenInfo;
  FILE*    outfile = info->cfile;
  GenRet   ret;

  codegenStmt(this);

  if (outfile) {
    if (this != getFunction()->body) {
      if (this->blockInfoGet()) {
        if (this->blockInfoGet()->isPrimitive(PRIM_BLOCK_LOCAL)) {
          info->cStatements.push_back("/* local block */\n");
        }
      }
      info->cStatements.push_back("{\n");
    }

    body.codegen("");

    if (this != getFunction()->body) {
      std::string end  = "}";
      CondStmt*   cond = toCondStmt(parentExpr);

      if (!cond || !(cond->thenStmt == this && cond->elseStmt))
        end += "\n";

      info->cStatements.push_back(end);
    }

  } else {
#ifdef HAVE_LLVM
    llvm::Function*   func          = info->irBuilder->GetInsertBlock()->getParent();
    llvm::BasicBlock* blockStmtBody = NULL;

    getFunction()->codegenUniqueNum++;

    blockStmtBody = llvm::BasicBlock::Create(info->module->getContext(), FNAME("blk_body"));
    trackLLVMValue(blockStmtBody);

    llvm::BranchInst* toBody = info->irBuilder->CreateBr(blockStmtBody);
    trackLLVMValue(toBody);

    // Now add the body.
#if HAVE_LLVM_VER >= 160
    func->insert(func->end(), blockStmtBody);
#else
    func->getBasicBlockList().push_back(blockStmtBody);
#endif

    info->irBuilder->SetInsertPoint(blockStmtBody);

    info->lvt->addLayer();

    for_alist(node, this->body) {
      node->codegen();
    }

    info->lvt->removeLayer();

    INT_ASSERT(blockStmtBody->getParent() == func);
#endif
  }

  return ret;
}

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

GenRet
CondStmt::codegen() {
  GenInfo* info    = gGenInfo;
  FILE*    outfile = info->cfile;
  GenRet   ret;

  codegenStmt(this);

  if ( outfile ) {
    //here it's very possible that we end up with ( ) around condExpr. Extra
    //parentheses generated warnings from the backend compiler in some cases.
    //It didn't feel very safe to strip them at expr level as it might mess up
    //precedence thus, following conditional -- Engin

    std::string c_condExpr = codegenValue(condExpr).c;

    int numExtraPar = 0;
    //if (c_condExpr[0] == '(' && c_condExpr[c_condExpr.size()-1] == ')') {
    if (c_condExpr[numExtraPar] == '(' &&
        c_condExpr[c_condExpr.size()-(numExtraPar+1)] == ')') {
      numExtraPar++;
    }
    c_condExpr = c_condExpr.substr(numExtraPar,
        c_condExpr.length()-2*numExtraPar);
    info->cStatements.push_back("if (" + c_condExpr  + ") ");

    thenStmt->codegen();

    if (elseStmt) {
      info->cStatements.push_back(" else ");

      // If the first statement is the only statement and itself a
      // conditional, just dive in and codegen it to avoid a potential
      // cascade of curly brackets, striving instead for:
      //
      // ...
      // } else if (...) {
      // } else if (...) {
      // ...
      //
      Expr* firstStmt = elseStmt->body.head;
      if (elseStmt->length() == 1 && isCondStmt(firstStmt)) {
        firstStmt->codegen();
      } else {
        elseStmt->codegen();
      }
    }

  } else {
#ifdef HAVE_LLVM
    llvm::Function* func = info->irBuilder->GetInsertBlock()->getParent();

    getFunction()->codegenUniqueNum++;

    llvm::BasicBlock *condStmtIf = llvm::BasicBlock::Create(
        info->module->getContext(),
        FNAME("cond_if"));

    llvm::BasicBlock *condStmtThen = llvm::BasicBlock::Create(
        info->module->getContext(),
        FNAME("cond_then"));

    llvm::BasicBlock *condStmtElse = NULL;

    llvm::BasicBlock *condStmtEnd = llvm::BasicBlock::Create(
        info->module->getContext(),
        FNAME("cond_end"));

    trackLLVMValue(condStmtIf);
    trackLLVMValue(condStmtThen);
    trackLLVMValue(condStmtEnd);

    if (elseStmt) {
      condStmtElse = llvm::BasicBlock::Create(info->module->getContext(),
                                              FNAME("cond_else"));
      trackLLVMValue(condStmtElse);
    }

    info->lvt->addLayer();

    llvm::BranchInst* toCond = info->irBuilder->CreateBr(condStmtIf);
    trackLLVMValue(toCond);

#if HAVE_LLVM_VER >= 160
    func->insert(func->end(), condStmtIf);
#else
    func->getBasicBlockList().push_back(condStmtIf);
#endif
    info->irBuilder->SetInsertPoint(condStmtIf);

    GenRet condValueRet = codegenValue(condExpr);

    llvm::Value *condValue = condValueRet.val;

    if( condValue->getType() !=
        llvm::Type::getInt1Ty(info->module->getContext()) ) {
      condValue = info->irBuilder->CreateICmpNE(
          condValue,
          llvm::ConstantInt::get(condValue->getType(), 0),
          FNAME("condition"));
      trackLLVMValue(condValue);
    }

    llvm::BranchInst* condBr = info->irBuilder->CreateCondBr(
        condValue,
        condStmtThen,
        (elseStmt) ? condStmtElse : condStmtEnd);
    trackLLVMValue(condBr);

#if HAVE_LLVM_VER >= 160
    func->insert(func->end(), condStmtThen);
#else
    func->getBasicBlockList().push_back(condStmtThen);
#endif
    info->irBuilder->SetInsertPoint(condStmtThen);

    info->lvt->addLayer();
    thenStmt->codegen();

    llvm::BranchInst* toEnd1 = info->irBuilder->CreateBr(condStmtEnd);
    trackLLVMValue(toEnd1);
    info->lvt->removeLayer();

    if(elseStmt) {
#if HAVE_LLVM_VER >= 160
      func->insert(func->end(), condStmtElse);
#else
      func->getBasicBlockList().push_back(condStmtElse);
#endif
      info->irBuilder->SetInsertPoint(condStmtElse);

      info->lvt->addLayer();
      elseStmt->codegen();
      llvm::BranchInst* toEnd2 = info->irBuilder->CreateBr(condStmtEnd);
      trackLLVMValue(toEnd2);
      info->lvt->removeLayer();
    }

#if HAVE_LLVM_VER >= 160
    func->insert(func->end(), condStmtEnd);
#else
    func->getBasicBlockList().push_back(condStmtEnd);
#endif
    info->irBuilder->SetInsertPoint(condStmtEnd);

    info->lvt->removeLayer();
#endif
  }
  return ret;
}

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

GenRet GotoStmt::codegen() {
  GenInfo* info = gGenInfo;
  FILE* outfile = info->cfile;
  GenRet ret;

  codegenStmt(this);
  if( outfile ) {
    info->cStatements.push_back("goto " + label->codegen().c + ";\n");
  } else {
#ifdef HAVE_LLVM
    llvm::Function *func = info->irBuilder->GetInsertBlock()->getParent();

    const char *cname;
    if(isDefExpr(label)) {
      cname = toDefExpr(label)->sym->cname;
    }
    else {
      cname = toSymExpr(label)->symbol()->cname;
    }

    llvm::BasicBlock *blockLabel;
    if(!(blockLabel = info->lvt->getBlock(cname))) {
      blockLabel = llvm::BasicBlock::Create(info->module->getContext(), cname);
      trackLLVMValue(blockLabel);
      info->lvt->addBlock(cname, blockLabel);
    }

    llvm::BranchInst* toLabel = info->irBuilder->CreateBr(blockLabel);
    trackLLVMValue(toLabel);

    getFunction()->codegenUniqueNum++;

    llvm::BasicBlock *afterGoto = llvm::BasicBlock::Create(
        info->module->getContext(), FNAME("afterGoto"));
    trackLLVMValue(afterGoto);
#if HAVE_LLVM_VER >= 160
    func->insert(func->end(), afterGoto);
#else
    func->getBasicBlockList().push_back(afterGoto);
#endif
    info->irBuilder->SetInsertPoint(afterGoto);

#endif
  }
  return ret;
}

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

GenRet ForwardingStmt::codegen() {
  GenRet ret;
  // Should be lowered in resolution
  INT_FATAL(this, "ForwardingStmt codegen called");
  return ret;
}

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

GenRet ImplementsStmt::codegen() {
  INT_FATAL(this, "ImplementsStmt should have been lowered in resolution");
  GenRet dummy;
  return dummy;
}

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

GenRet ExternBlockStmt::codegen() {
  GenRet ret;
  // Needs to be handled specially by creating a C
  //  file per module..
  INT_FATAL(this, "ExternBlockStmt codegen called");
  return ret;
}
