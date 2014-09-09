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

#include "stmt.h"

#include "astutil.h"
#include "codegen.h"
#include "expr.h"
#include "files.h"
#include "misc.h"
#include "passes.h"
#include "stringutil.h"

#include "AstVisitor.h"

#include <cstring>
#include <algorithm>

// remember these so we can update their labels' iterResumeGoto
Map<GotoStmt*,GotoStmt*> copiedIterResumeGotos;

// remember these so we can remove their iterResumeGoto
Vec<LabelSymbol*> removedIterResumeLabels;

#ifdef HAVE_LLVM

#define FNAME(str) (llvm::Twine(getFunction()->cname) + llvm::Twine("_") + llvm::Twine(getFunction()->codegenUniqueNum) + llvm::Twine(str) + llvm::Twine("_"))

#endif

void codegenStmt(Expr* stmt) {
  GenInfo* info    = gGenInfo;
  FILE*    outfile = info->cfile;

  info->lineno   = stmt->linenum();
  info->filename = stmt->fname();

  if( outfile ) {
    if (stmt->linenum() > 0) {
      if (printCppLineno) {
        info->cStatements.push_back(
            "/* ZLINE: " + numToString(stmt->linenum())
            + " " + stmt->fname() + " */\n");
      } 
    }

    if (fGenIDS)
      info->cStatements.push_back("/* " + numToString(stmt->id) + "*/ ");
  }

  ++gStmtCount;
}


/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

Stmt::Stmt(AstTag astTag) : Expr(astTag) {

}
Stmt::~Stmt() {

}

bool Stmt::isStmt() const { 
  return true; 
}



/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

BlockStmt::BlockStmt(Expr* initBody, BlockTag initBlockTag) :
  Stmt(E_BlockStmt),
  blockTag(initBlockTag),
  blockInfo(NULL),
  modUses(NULL),
  breakLabel(NULL),
  continueLabel(NULL),
  userLabel(NULL),
  byrefVars(NULL) {

  body.parent = this;

  if (initBody)
    body.insertAtTail(initBody);

  gBlockStmts.add(this);
}


BlockStmt::~BlockStmt() { 

}

void BlockStmt::verify() {
  Expr::verify();

  if (astTag != E_BlockStmt) {
    INT_FATAL(this, "BlockStmt::verify. Bad astTag");
  }

  if (body.parent != this)
    INT_FATAL(this, "BlockStmt::verify. Bad body.parent");

  for_alist(expr, body) {
    if (expr->parentExpr != this)
      INT_FATAL(this, "BlockStmt::verify. Bad body.expr->parentExpr");
  }

  if (blockInfo && blockInfo->parentExpr != this)
    INT_FATAL(this, "BlockStmt::verify. Bad blockInfo->parentExpr");

  if (modUses   && modUses->parentExpr   != this)
    INT_FATAL(this, "BlockStmt::verify. Bad modUses->parentExpr");

  if (byrefVars) {
    if (byrefVars->parentExpr != this)
      INT_FATAL(this, "BlockStmt::verify. Bad byrefVars->parentExpr");

    for_actuals(varExp, byrefVars) {
      if (!isSymExpr(varExp) && !isUnresolvedSymExpr(varExp))
        INT_FATAL(this, "BlockStmt::verify. Bad expression kind in byrefVars");
    }
  }
}


BlockStmt*
BlockStmt::copyInner(SymbolMap* map) {
  BlockStmt* _this = new BlockStmt();

  _this->blockTag      = blockTag;
  _this->blockInfo     = COPY_INT(blockInfo);
  _this->modUses       = COPY_INT(modUses);
  _this->breakLabel    = breakLabel;
  _this->continueLabel = continueLabel;
  _this->byrefVars     = COPY_INT(byrefVars);

  for_alist(expr, body)
    _this->insertAtTail(COPY_INT(expr));

  return _this;
}


// Note that newAst can be NULL to reflect deletion
void BlockStmt::replaceChild(Expr* oldAst, Expr* newAst) {
  CallExpr* oldExpr = toCallExpr(oldAst);
  CallExpr* newExpr = (newAst != NULL) ? toCallExpr(newAst) : NULL;

  if (oldExpr == NULL)
    INT_FATAL(this, "BlockStmt::replaceChild. oldAst is not a CallExpr");

  else if (oldExpr == blockInfo)
    blockInfo = newExpr;

  else if (oldExpr == modUses)
    modUses   = newExpr;

  else if (oldExpr == byrefVars)
    byrefVars = newExpr;

  else
    INT_FATAL(this, "BlockStmt::replaceChild. Failed to match the oldAst ");
}


// This function is used to codegen the init, test, and incr segments of c for
// loops. In c for loops instead of using statements comma operators must be
// used. So for the init instead of generating something like:
//   i = 4;
//   j = 4;
//
// We need to generate:
// i = 4, j = 4
static std::string codegenCForLoopHeaderSegment(BlockStmt* block) {
  GenInfo* info = gGenInfo;
  std::string seg = "";

  for_alist(expr, block->body) {
    CallExpr* call = toCallExpr(expr);

    // Generate defExpr normally (they always get codegenned at the top of a
    // function currently, if that changes this code will probably be wrong.)
    if (DefExpr* defExpr = toDefExpr(expr)) {
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
    else if (call && (call->isResolved() || isRelationalOperator(call))) {

      std::string callStr = codegenValue(call).c;
      if (callStr != "") {
        seg += callStr + ';';
      }

    }
    // Similar to above, generate symExprs
    else if (SymExpr* symExpr = toSymExpr(expr)) {
      std::string symStr = codegenValue(symExpr).c;
      if (symStr != "") {
        seg += symStr + ';';
      }
    }
    // Everything else is just a bunch of statements. We do normal codegen() on
    // them which ends up putting whatever got codegenned into CStatements. We
    // pop all of those back off (note that the order we pop and attach to our
    // segment is important.)
    else {
      int prevStatements = (int)info->cStatements.size();
      expr->codegen();
      int newStatements = (int)info->cStatements.size() - prevStatements;
      for (std::vector<std::string>::iterator it = info->cStatements.end() -
          newStatements; it != info->cStatements.end(); ++it) {
        seg += *it;
      }
      info->cStatements.erase(info->cStatements.end()-newStatements,
          info->cStatements.end());
    }
  }

  // replace all the semicolons (from "statements") with commas
  std::replace(seg.begin(), seg.end(), ';', ',');

  // remove all the newlines
  seg.erase(std::remove(seg.begin(), seg.end(), '\n'), seg.end());

  // remove the last character if any were generated (it's a trailing comma
  // since we previously had an appropriate "trailing" semicolon
  if (seg.size () > 0)  seg.resize (seg.size () - 1);

  return seg;
}

#ifdef HAVE_LLVM
static GenRet codegenCForLoopConditional(BlockStmt* block)
{
  GenRet ret;
  for_alist(expr, block->body) {
    ret = expr->codegen();
  }
  return codegenValue(ret);
}
#endif

GenRet BlockStmt::codegen() {
  GenInfo* info    = gGenInfo;
  FILE*    outfile = info->cfile;
  GenRet   ret;

  codegenStmt(this);

  if( outfile ) {
    if (blockInfo) {
      if (blockInfo->isPrimitive(PRIM_BLOCK_WHILEDO_LOOP)) {
        std::string hdr = "while (" + codegenValue(blockInfo->get(1)).c + ") ";
        info->cStatements.push_back(hdr);
      } else if (blockInfo->isPrimitive(PRIM_BLOCK_DOWHILE_LOOP)) {
        info->cStatements.push_back("do ");
      } else if (blockInfo->isPrimitive(PRIM_BLOCK_FOR_LOOP)) {
        std::string hdr = "for (;" + codegenValue(blockInfo->get(1)).c + ";) ";
        info->cStatements.push_back(hdr);
      } else if (blockInfo->isPrimitive(PRIM_BLOCK_C_FOR_LOOP)) {
        BlockStmt* initBlock = toBlockStmt(blockInfo->get(1));
        // These copy calls are needed or else values get code generated twice.
        std::string init = codegenCForLoopHeaderSegment(initBlock->copy());

        BlockStmt* testBlock = toBlockStmt(blockInfo->get(2));
        std::string test = codegenCForLoopHeaderSegment(testBlock->copy());
        // wrap the test with paren. Could probably check if it already has
        // outer paren to make the code a little cleaner.
        if (test != "") test = "(" + test + ")";

        BlockStmt* incrBlock = toBlockStmt(blockInfo->get(3));
        std::string incr = codegenCForLoopHeaderSegment(incrBlock->copy());

        std::string hdr = "for (" + init + "; " + test + "; " + incr + ") ";
        info->cStatements.push_back(hdr);
      }
    }

    if (this != getFunction()->body)
      info->cStatements.push_back("{\n");

    body.codegen("");

    if (blockInfo && blockInfo->isPrimitive(PRIM_BLOCK_DOWHILE_LOOP)) {
      std::string ftr= "} while (" + codegenValue(blockInfo->get(1)).c + ");\n";
      info->cStatements.push_back(ftr);
    } else if (this != getFunction()->body) {
      std::string end = "}";
      CondStmt* cond = toCondStmt(parentExpr);
      if (!cond || !(cond->thenStmt == this && cond->elseStmt))
        end += "\n";
      info->cStatements.push_back(end);
    }
  } else {

#ifdef HAVE_LLVM
    llvm::Function *func = info->builder->GetInsertBlock()->getParent();

    getFunction()->codegenUniqueNum++;

    llvm::BasicBlock *blockStmtInit = NULL;
    llvm::BasicBlock *blockStmtCond = NULL;
    llvm::BasicBlock *blockStmtBody = NULL;
    llvm::BasicBlock *blockStmtEndCond = NULL;
    llvm::BasicBlock *blockStmtEnd = NULL;

    blockStmtBody = llvm::BasicBlock::Create(
        info->module->getContext(), FNAME("blk_body"));
   
    if(blockInfo) {
      blockStmtEnd = llvm::BasicBlock::Create(
          info->module->getContext(), FNAME("blk_end"));
      if (blockInfo->isPrimitive(PRIM_BLOCK_C_FOR_LOOP)) {
        // C for loop...
        // blockInfo->get(1) is the initialization block 
        // blockInfo->get(2) is the test block 
        // blockInfo->get(3) is the increment block

        // In order to track more easily with the C backend
        // and because mem2reg should optimize all of these
        // cases, we generate a for loop as the same
        // as if(cond) do { body; step; } while(cond).

        BlockStmt* initBlock = toBlockStmt(blockInfo->get(1));
        BlockStmt* testBlock = toBlockStmt(blockInfo->get(2));
        BlockStmt* incrBlock = toBlockStmt(blockInfo->get(3));

        assert(initBlock && testBlock && incrBlock);

        // Create the init basic block
        blockStmtInit = llvm::BasicBlock::Create(
            info->module->getContext(), FNAME("blk_c_for_init"));
        func->getBasicBlockList().push_back(blockStmtInit);

        // Insert an explicit branch from the current block to the init block
        info->builder->CreateBr(blockStmtInit);

        // Now switch to the init block for code generation
        info->builder->SetInsertPoint(blockStmtInit);

        // Code generate the init block.
        initBlock->body.codegen("");

        // Add the loop condition to figure out if we run the loop at all.
        GenRet test = codegenCForLoopConditional(testBlock);
        // Add a branch based on test.
        llvm::Value* condValue = test.val;
        // Normalize it to boolean
        if( condValue->getType() !=
            llvm::Type::getInt1Ty(info->module->getContext()) ) {
          condValue = info->builder->CreateICmpNE(
              condValue,
              llvm::ConstantInt::get(condValue->getType(), 0),
              FNAME("condition"));
        }
        // Create the conditional branch
        info->builder->CreateCondBr(condValue, blockStmtBody, blockStmtEnd);

      } else if (blockInfo->isPrimitive(PRIM_BLOCK_WHILEDO_LOOP) ||
                 blockInfo->isPrimitive(PRIM_BLOCK_FOR_LOOP)) {
        // Add the condition block.
        blockStmtCond = llvm::BasicBlock::Create(
            info->module->getContext(), FNAME("blk_cond"));
        func->getBasicBlockList().push_back(blockStmtCond);
        // Insert an explicit branch from the current block to the loop start.
        info->builder->CreateBr(blockStmtCond);
        // Now switch to the condition for code generation 
        info->builder->SetInsertPoint(blockStmtCond);

        GenRet condValueRet = codegenValue(blockInfo->get(1));
        llvm::Value *condValue = condValueRet.val;
        if( condValue->getType() !=
            llvm::Type::getInt1Ty(info->module->getContext()) ) {
          condValue = info->builder->CreateICmpNE(
              condValue,
              llvm::ConstantInt::get(condValue->getType(), 0),
              FNAME("condition"));
        }

        // Now we might do either to the Body or to the End.
        info->builder->CreateCondBr(condValue, blockStmtBody, blockStmtEnd);
      } else {
        info->builder->CreateBr(blockStmtBody);
      }
    } else {
      info->builder->CreateBr(blockStmtBody);
    }
    
    // Now add the body.
    func->getBasicBlockList().push_back(blockStmtBody);
    info->builder->SetInsertPoint(blockStmtBody);
    
    info->lvt->addLayer();

    body.codegen("");
    info->lvt->removeLayer();
    
    if(blockInfo) {
      if(blockInfo->isPrimitive(PRIM_BLOCK_C_FOR_LOOP)) {
        // Generate the step operations and the termination test
        // after the loop body
        BlockStmt* testBlock = toBlockStmt(blockInfo->get(2));
        BlockStmt* incrBlock = toBlockStmt(blockInfo->get(3));

        // Code generate the step operations.
        incrBlock->body.codegen("");

        GenRet test = codegenCForLoopConditional(testBlock);
        // Add a branch based on test.
        llvm::Value* condValue = test.val;
        // Normalize it to boolean
        if( condValue->getType() !=
            llvm::Type::getInt1Ty(info->module->getContext()) ) {
          condValue = info->builder->CreateICmpNE(
              condValue,
              llvm::ConstantInt::get(condValue->getType(), 0),
              FNAME("condition"));
        }
        // Create the conditional branch
        info->builder->CreateCondBr(condValue, blockStmtBody, blockStmtEnd);
      } else if(blockInfo->isPrimitive(PRIM_BLOCK_DOWHILE_LOOP)) {
        // Add the condition block.
        blockStmtEndCond = llvm::BasicBlock::Create(
            info->module->getContext(), FNAME("blk_end_cond"));
        func->getBasicBlockList().push_back(blockStmtEndCond);
        // Insert an explicit branch from the body block to the loop condition.
        info->builder->CreateBr(blockStmtEndCond);
        // set insert point
        info->builder->SetInsertPoint(blockStmtEndCond);
        GenRet condValueRet = codegenValue(blockInfo->get(1));
        llvm::Value *condValue = condValueRet.val;
        if( condValue->getType() !=
            llvm::Type::getInt1Ty(info->module->getContext()) ) {
          condValue = info->builder->CreateICmpNE(
              condValue,
              llvm::ConstantInt::get(condValue->getType(), 0),
              FNAME("condition"));
        }
        info->builder->CreateCondBr(condValue, blockStmtBody, blockStmtEnd);
      } else if( blockStmtCond ) {
        info->builder->CreateBr(blockStmtCond);
      } else {
        info->builder->CreateBr(blockStmtEnd);
      }
      func->getBasicBlockList().push_back(blockStmtEnd);
      info->builder->SetInsertPoint(blockStmtEnd);
    }
    if( blockStmtCond ) INT_ASSERT(blockStmtCond->getParent() == func);
    if( blockStmtBody ) INT_ASSERT(blockStmtBody->getParent() == func);
    if( blockStmtEndCond ) INT_ASSERT(blockStmtEndCond->getParent() == func);
    if( blockStmtEnd ) INT_ASSERT(blockStmtEnd->getParent() == func);
 
#endif
  }
  INT_ASSERT(!byrefVars); // these should not persist past parallel()
  return ret;
}

// The BISON productions generate a large number of scope-less BlockStmt
// as an artifact of the processing.  This function is intended to be
// called from well-defined points in the parser to collapse these during
// the construction of the parse tree.
void
BlockStmt::appendChapelStmt(BlockStmt* stmt) {
  if (canFlattenChapelStmt(stmt) == true) {
    for_alist(expr, stmt->body) {
      this->insertAtTail(expr->remove());
    }
  } else {
    insertAtTail(stmt);
  }
}

//
// The Parser routinely generates scope-less BlockStmts to
// represent Chapel "statements".  In some cases it is possible
// to flatten the BlockStmt when appending a Chapel statement to
// a statement list.  This function returns TRUE if this is safe
//
//
// The BlockStmt must in fact be scope-less.  Then one or more of
// the following must be true
//
// 2014/07/03:  Pass 1
//   a) The BlockStmt contains a single item
//

bool
BlockStmt::canFlattenChapelStmt(const BlockStmt* stmt) const {
  bool retval = false;

  if (stmt->isScopeless() == true) {
    if (stmt->length() == 1)
      retval =  true;

    else
      retval = false;
  }

  return retval;
}


void
BlockStmt::insertAtHead(Expr* ast) {
  body.insertAtHead(ast);
}


void
BlockStmt::insertAtTail(Expr* ast) {
  body.insertAtTail(ast);
}


void
BlockStmt::insertAtHead(const char* format, ...) {
  va_list args;

  va_start(args, format);
  insertAtHead(new_Expr(format, args));
  va_end(args);
}


void
BlockStmt::insertAtTail(const char* format, ...) {
  va_list args;

  va_start(args, format);
  insertAtTail(new_Expr(format, args));
  va_end(args);
}


void
BlockStmt::insertAtTailBeforeGoto(Expr* ast) {
  if (isGotoStmt(body.tail))
    body.tail->insertBefore(ast);
  else
    body.insertAtTail(ast);
}


bool
BlockStmt::isScopeless() const {
  return blockTag == BLOCK_SCOPELESS;
}

bool
BlockStmt::isLoop() const {
  return (blockInfo &&
          (blockInfo->isPrimitive(PRIM_BLOCK_DOWHILE_LOOP) ||
           blockInfo->isPrimitive(PRIM_BLOCK_WHILEDO_LOOP) ||
           blockInfo->isPrimitive(PRIM_BLOCK_PARAM_LOOP) ||
           blockInfo->isPrimitive(PRIM_BLOCK_FOR_LOOP) ||
           blockInfo->isPrimitive(PRIM_BLOCK_C_FOR_LOOP)));
}


int
BlockStmt::length() const {
  return body.length;
}


void
BlockStmt::moduleUseAdd(ModuleSymbol* mod) {
  if (modUses == NULL) {
    modUses = new CallExpr(PRIM_USED_MODULES_LIST);

    if (parentSymbol)
      insert_help(modUses, this, parentSymbol);
  }

  modUses->insertAtTail(mod);
}


// Remove a module from the list of modules used by the module this block
// statement belongs to. The list of used modules is stored in modUses
bool
BlockStmt::moduleUseRemove(ModuleSymbol* mod) {
  bool retval = false;

  if (modUses != NULL) {
    for_alist(expr, modUses->argList) {
      if (SymExpr* symExpr = toSymExpr(expr)) {
        if (ModuleSymbol* curMod = toModuleSymbol(symExpr->var)) {
          if (curMod == mod) {
            symExpr->remove();
            
            retval = true;
            break;
          }
        }
      }
    }
  }

  return retval;
}

void
BlockStmt::moduleUseClear() {
  if (modUses != 0) {

    for_alist(expr, modUses->argList) {
      expr->remove();
    }

    // It's possible that this use definition is not alive
    if (isAlive(modUses))
      modUses->remove();

    modUses = 0;
  }
}

void 
BlockStmt::accept(AstVisitor* visitor) {
  if (visitor->enterBlockStmt(this) == true) {
    for_alist(next_ast, body)
      next_ast->accept(visitor);

    if (blockInfo)
      blockInfo->accept(visitor);

    if (modUses)
      modUses->accept(visitor);

    if (byrefVars)
      byrefVars->accept(visitor);

    visitor->exitBlockStmt(this);
  }
}

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

CondStmt::CondStmt(Expr* iCondExpr, BaseAST* iThenStmt, BaseAST* iElseStmt) :
  Stmt(E_CondStmt),
  condExpr(iCondExpr),
  thenStmt(NULL),
  elseStmt(NULL)
{
  if (Expr* s = toExpr(iThenStmt)) {
    BlockStmt* bs = toBlockStmt(s);
    if (bs && bs->blockTag == BLOCK_NORMAL && !bs->blockInfo)
      thenStmt = bs;
    else
      thenStmt = new BlockStmt(s);
    } else
    INT_FATAL(iThenStmt, "Bad then-stmt passed to CondStmt constructor");
  if (iElseStmt) {
    if (Expr* s = toExpr(iElseStmt)) {
      BlockStmt* bs = toBlockStmt(s);
      if (bs && bs->blockTag == BLOCK_NORMAL && !bs->blockInfo)
        elseStmt = bs;
      else
        elseStmt = new BlockStmt(s);
    } else
      INT_FATAL(iElseStmt, "Bad else-stmt passed to CondStmt constructor");
  }
  gCondStmts.add(this);
}


Expr*
CondStmt::fold_cond_stmt()
{
  // deadBlockElimination() can get rid of the condition expression 
  // without getting rid of the parent if.  We do that here.
  if (! condExpr)
  {
    this->remove();
    return NULL;
  }

  // Similarly, deadBlockElimination() can kill the THEN expression
  if (! thenStmt)
  {
    // Two cases:
    // If elseExpr is also null, just kill the whole IF.
    if (! elseStmt)
    {
      this->remove();
      return NULL;
    }
    // Otherwise, invert the condition and move the else clause into
    // the THEN slot.
    Expr* cond = new CallExpr(PRIM_UNARY_LNOT, condExpr);
    this->replaceChild(condExpr, cond);
    this->replaceChild(thenStmt, elseStmt);
    this->replaceChild(elseStmt, NULL);
  }

  Expr* result = NULL;
  if (SymExpr* cond = toSymExpr(condExpr))
  {
    if (VarSymbol* var = toVarSymbol(cond->var)) {
      if (var->immediate &&
          var->immediate->const_kind == NUM_KIND_BOOL) {
        SET_LINENO(this);
        result = new CallExpr(PRIM_NOOP);
        this->insertBefore(result);
        if (var->immediate->bool_value() == gTrue->immediate->bool_value()) {
          Expr* then_stmt = thenStmt;
          then_stmt->remove();
          this->replace(then_stmt);
        } else if (var->immediate->bool_value() == gFalse->immediate->bool_value()) {
          Expr* else_stmt = elseStmt;
          if (else_stmt) {
            else_stmt->remove();
            this->replace(else_stmt);
          } else {
            this->remove();
          }
        }
      }
    }
  }
  removeDeadIterResumeGotos();
  return result;
}


void CondStmt::verify() {
  Expr::verify();
  if (astTag != E_CondStmt) {
    INT_FATAL(this, "Bad CondStmt::astTag");
  }

  if (!condExpr) {
    INT_FATAL(this, "CondStmt has no condExpr");
  }

  if (!thenStmt) {
    INT_FATAL(this, "CondStmt has no thenStmt");
  }

  if (condExpr->list) {
    INT_FATAL(this, "CondStmt::condExpr is a list");
  }

  if (thenStmt->list) {
    INT_FATAL(this, "CondStmt::thenStmt is a list");
  }

  if (elseStmt && elseStmt->list) {
    INT_FATAL(this, "CondStmt::elseStmt is a list");
  }

  if (condExpr && condExpr->parentExpr != this)
    INT_FATAL(this, "Bad CondStmt::condExpr::parentExpr");

  if (thenStmt && thenStmt->parentExpr != this)
    INT_FATAL(this, "Bad CondStmt::thenStmt::parentExpr");

  if (elseStmt && elseStmt->parentExpr != this)
    INT_FATAL(this, "Bad CondStmt::elseStmt::parentExpr");

}


CondStmt*
CondStmt::copyInner(SymbolMap* map) {
  return new CondStmt(COPY_INT(condExpr),
                      COPY_INT(thenStmt),
                      COPY_INT(elseStmt));
}


void CondStmt::replaceChild(Expr* old_ast, Expr* new_ast) {
  if (old_ast == condExpr) {
    condExpr = new_ast;
  } else if (old_ast == thenStmt) {
    thenStmt = toBlockStmt(new_ast);
  } else if (old_ast == elseStmt) {
    elseStmt = toBlockStmt(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in CondStmt::replaceChild");
  }
}


GenRet CondStmt::codegen() {
  GenInfo* info = gGenInfo;
  FILE* outfile = info->cfile;
  GenRet ret;

  codegenStmt(this);
  if( outfile ) {
    info->cStatements.push_back("if (" + codegenValue(condExpr).c + ") ");
    thenStmt->codegen();
    if (elseStmt) {
      info->cStatements.push_back(" else ");
      elseStmt->codegen();
    }
  } else {
#ifdef HAVE_LLVM
    llvm::Function *func = info->builder->GetInsertBlock()->getParent();
    getFunction()->codegenUniqueNum++;

    llvm::BasicBlock *condStmtIf = llvm::BasicBlock::Create(
        info->module->getContext(), FNAME("cond_if"));
    llvm::BasicBlock *condStmtThen = llvm::BasicBlock::Create(
        info->module->getContext(), FNAME("cond_then"));
    llvm::BasicBlock *condStmtElse = NULL;
    llvm::BasicBlock *condStmtEnd = llvm::BasicBlock::Create(
        info->module->getContext(), FNAME("cond_end"));
          
    if(elseStmt) {
      condStmtElse = llvm::BasicBlock::Create(
          info->module->getContext(), FNAME("cond_else"));
    }
          
    info->lvt->addLayer();
    
    info->builder->CreateBr(condStmtIf);

    func->getBasicBlockList().push_back(condStmtIf);
    info->builder->SetInsertPoint(condStmtIf);
    
    GenRet condValueRet = codegenValue(condExpr);
    llvm::Value *condValue = condValueRet.val;
    if( condValue->getType() !=
        llvm::Type::getInt1Ty(info->module->getContext()) ) {
      condValue = info->builder->CreateICmpNE(
          condValue,
          llvm::ConstantInt::get(condValue->getType(), 0),
          FNAME("condition"));
    }
    info->builder->CreateCondBr(
        condValue,
        condStmtThen,
        (elseStmt) ? condStmtElse : condStmtEnd);
    
    func->getBasicBlockList().push_back(condStmtThen);
    info->builder->SetInsertPoint(condStmtThen);
    
    info->lvt->addLayer();
    thenStmt->codegen();

    info->builder->CreateBr(condStmtEnd);
    info->lvt->removeLayer();
    
    if(elseStmt) {
      func->getBasicBlockList().push_back(condStmtElse);
      info->builder->SetInsertPoint(condStmtElse);
    
      info->lvt->addLayer();
      elseStmt->codegen();
      info->builder->CreateBr(condStmtEnd);
      info->lvt->removeLayer();
    }
    
    func->getBasicBlockList().push_back(condStmtEnd);
    info->builder->SetInsertPoint(condStmtEnd);
    
    info->lvt->removeLayer();
#endif
  }
  return ret;
}


void CondStmt::accept(AstVisitor* visitor) {
  if (visitor->enterCondStmt(this) == true) {

    if (condExpr)
      condExpr->accept(visitor);

    if (thenStmt)
      thenStmt->accept(visitor);

    if (elseStmt)
      elseStmt->accept(visitor);

    visitor->exitCondStmt(this);
  }
}

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

GotoStmt::GotoStmt(GotoTag init_gotoTag, const char* init_label) :
  Stmt(E_GotoStmt),
  gotoTag(init_gotoTag),
  label(init_label ? (Expr*)new UnresolvedSymExpr(init_label)
                   : (Expr*)new SymExpr(gNil))
{
  gGotoStmts.add(this);
}


GotoStmt::GotoStmt(GotoTag init_gotoTag, Symbol* init_label) :
  Stmt(E_GotoStmt),
  gotoTag(init_gotoTag),
  label(new SymExpr(init_label))
{
  gGotoStmts.add(this);
}


GotoStmt::GotoStmt(GotoTag init_gotoTag, Expr* init_label) :
  Stmt(E_GotoStmt),
  gotoTag(init_gotoTag),
  label(init_label)
{
  if (!init_label)
    INT_FATAL(this, "GotoStmt initialized with null label");

  if (init_label->parentSymbol)
    INT_FATAL(this, "GotoStmt initialized with label already in tree");

  gGotoStmts.add(this);
}


LabelSymbol* getGotoLabelSymbol(GotoStmt* gs) {
  if (gs->label)
    if (SymExpr* labse = toSymExpr(gs->label))
      if (labse->var)
        return toLabelSymbol(labse->var);
  return NULL;
}

GotoStmt* getGotoLabelsIterResumeGoto(GotoStmt* gs) {
  LabelSymbol* labsym = getGotoLabelSymbol(gs);
  return labsym ? labsym->iterResumeGoto : NULL;
}

void GotoStmt::verify() {
  Expr::verify();

  if (astTag != E_GotoStmt) {
    INT_FATAL(this, "Bad GotoStmt::astTag");
  }

  if (!label)
    INT_FATAL(this, "GotoStmt has no label");

  if (label->list)
    INT_FATAL(this, "GotoStmt::label is a list");

  if (label && label->parentExpr != this)
    INT_FATAL(this, "Bad GotoStmt::label::parentExpr");

  // If the label has been resolved to a label
  if (SymExpr* se = toSymExpr(label)) {
    if (isLabelSymbol(se->var)) {
      Symbol* parent = se->var->defPoint->parentSymbol;

      // The parent should either be a function or a
      // module that does not yet have the initFn installed
      if (isFnSymbol(parent) == false) {
        ModuleSymbol* module = toModuleSymbol(parent);

        if (module == 0 || module->initFn != 0) {
          INT_FATAL(this, "goto label is not in a function");
        }
      }

      if (se->var->defPoint->parentSymbol != this->parentSymbol)
        INT_FATAL(this, "goto label is in a different function than the goto");

      GotoStmt* igs = getGotoLabelsIterResumeGoto(this);

      if ((gotoTag == GOTO_ITER_RESUME) == (igs == NULL))
        INT_FATAL(this,
                  "goto must be GOTO_ITER_RESUME iff its label has iterResumeGoto");

      if (gotoTag == GOTO_ITER_RESUME && igs != this)
        INT_FATAL(this,
                  "GOTO_ITER_RESUME goto's label's iterResumeGoto does not match the goto");
    }
  }
}


GotoStmt*
GotoStmt::copyInner(SymbolMap* map) {
  GotoStmt* copy = new GotoStmt(gotoTag, COPY_INT(label));

  // For a GOTO_ITER_RESUME: has the label symbol already been copied? ...
  LabelSymbol* labsym;
  if (gotoTag == GOTO_ITER_RESUME && (labsym = getGotoLabelSymbol(this))) {
    LabelSymbol* lcopy = NULL;
    if (Symbol* mcopy = map->get(labsym)) {
      lcopy = toLabelSymbol(mcopy);
      INT_ASSERT(lcopy); // a LabelSymbol gotta map to a LabelSymbol
    }
    if (lcopy) {
      // ... yes => update the copy's field,
      lcopy->iterResumeGoto = copy;
      // printf("GotoStmt-copy %d > %d  labsym %d > %d  updating the copy\n",
      //        this->id, copy->id, labsym->id, lcopy->id);
    } else {
      // ... no => remember this goto copy for later.
      copiedIterResumeGotos.put(this, copy);
      // printf("GotoStmt-copy %d > %d  labsym %d  remembering\n",
      //        this->id, copy->id, labsym->id);
    }
  }
  return copy;
}

//
// Ensure all remembered Gotos have been taken care of.
// Reset copiedIterResumeGotos.
//
void verifyNcleanCopiedIterResumeGotos() {
  for (int i = 0; i < copiedIterResumeGotos.n; i++)
    if (GotoStmt* copy = copiedIterResumeGotos.v[i].value)
      INT_FATAL(copy, "unhandled goto in copiedIterResumeGotos");
  copiedIterResumeGotos.clear();
}


void GotoStmt::replaceChild(Expr* old_ast, Expr* new_ast) {
  if (old_ast == label) {
    label = toSymExpr(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in GotoStmt::replaceChild");
  }
}


GenRet GotoStmt::codegen() {
  GenInfo* info = gGenInfo;
  FILE* outfile = info->cfile;
  GenRet ret;

  codegenStmt(this);
  if( outfile ) {
    info->cStatements.push_back("goto " + label->codegen().c + ";\n");
  } else {
#ifdef HAVE_LLVM
    llvm::Function *func = info->builder->GetInsertBlock()->getParent();
  
    const char *cname;
    if(isDefExpr(label)) {
      cname = toDefExpr(label)->sym->cname;
    }
    else {
      cname = toSymExpr(label)->var->cname;
    }
    
    llvm::BasicBlock *blockLabel;
    if(!(blockLabel = info->lvt->getBlock(cname))) {
      blockLabel = llvm::BasicBlock::Create(info->module->getContext(), cname);
      info->lvt->addBlock(cname, blockLabel);
    }
    
    info->builder->CreateBr(blockLabel);
 
    getFunction()->codegenUniqueNum++;

    llvm::BasicBlock *afterGoto = llvm::BasicBlock::Create(
        info->module->getContext(), FNAME("afterGoto"));
    func->getBasicBlockList().push_back(afterGoto);
    info->builder->SetInsertPoint(afterGoto);

#endif
  }
  return ret;
}

const char* GotoStmt::getName() {
  if (SymExpr* se = toSymExpr(label))
    return se->var->name;
  else if (UnresolvedSymExpr* use = toUnresolvedSymExpr(label))
    return use->unresolved;
  else
    return NULL;
}

void GotoStmt::accept(AstVisitor* visitor) {
  if (visitor->enterGotoStmt(this) == true) {

    if (label)
      label->accept(visitor);

    visitor->exitGotoStmt(this);
  }
}

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

ExternBlockStmt::ExternBlockStmt(const char* init_c_code) :
  Stmt(E_ExternBlockStmt),
  c_code(init_c_code)
{
  gExternBlockStmts.add(this);
}

void ExternBlockStmt::verify() {
  Expr::verify();
  if (astTag != E_ExternBlockStmt) {
    INT_FATAL(this, "Bad ExternBlockStmt::astTag");
  }
  if (!c_code) {
    INT_FATAL(this, "ExternBlockStmt has no c_code");
  }
}

void ExternBlockStmt::replaceChild(Expr* old_ast, Expr* new_ast) {
  INT_FATAL(this, "ExternBlockStmt replaceChild called");
}

GenRet ExternBlockStmt::codegen() {
  GenRet ret;
  // Needs to be handled specially by creating a C
  //  file per module..
  INT_FATAL(this, "ExternBlockStmt codegen called");
  return ret;
}


ExternBlockStmt* ExternBlockStmt::copyInner(SymbolMap* map) {
  ExternBlockStmt* copy = new ExternBlockStmt(c_code);

  return copy;
}

void ExternBlockStmt::accept(AstVisitor* visitor) {
  visitor->visitEblockStmt(this);
}

