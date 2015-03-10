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

#include "stmt.h"

#include "astutil.h"
#include "codegen.h"
#include "expr.h"
#include "files.h"
#include "misc.h"
#include "passes.h"
#include "stringutil.h"

#include "codegen.h"
#include "llvmDebug.h"

#include "AstVisitor.h"

#include <cstring>
#include <algorithm>

// remember these so we can update their labels' iterResumeGoto
Map<GotoStmt*,GotoStmt*> copiedIterResumeGotos;

// remember these so we can remove their iterResumeGoto
Vec<LabelSymbol*> removedIterResumeLabels;

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
      info->cStatements.push_back("/* " + numToString(stmt->id) + " */ ");
  } else {
#ifdef HAVE_LLVM
    if (debug_info && stmt->linenum() > 0) 
    {
      // Adjust the current line number, but leave the scope alone.
      llvm::MDNode* scope;

      if(stmt->parentSymbol && stmt->parentSymbol->astTag == E_FnSymbol) {
        scope = debug_info->get_function((FnSymbol *)stmt->parentSymbol);
      } else {
        scope = info->builder->getCurrentDebugLocation().getScope(
                                                      info->llvmContext);
      }

      info->builder->SetCurrentDebugLocation(
                  llvm::DebugLoc::get(stmt->linenum(),0 /*col*/,scope));
    }
#endif
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
  modUses(NULL),
  userLabel(NULL),
  byrefVars(NULL),
  blockInfo(NULL) {
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

  _this->blockTag  = blockTag;
  _this->blockInfo = COPY_INT(blockInfo);
  _this->modUses   = COPY_INT(modUses);
  _this->byrefVars = COPY_INT(byrefVars);

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

  // TODO: Handle the above special cases uniformly by specializing the
  // traversal of the children by block statement type.  I think blockInfo is
  // being deprecated anyway....
}

CallExpr* BlockStmt::blockInfoGet() const {
  return blockInfo;
}

CallExpr* BlockStmt::blockInfoSet(CallExpr* expr) {
  blockInfo = expr;

  return blockInfo;
}

GenRet BlockStmt::codegen() {
  GenInfo* info    = gGenInfo;
  FILE*    outfile = info->cfile;
  GenRet   ret;

  codegenStmt(this);

  if (outfile) {
    if (this != getFunction()->body)
      info->cStatements.push_back("{\n");

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
    llvm::Function*   func          = info->builder->GetInsertBlock()->getParent();
    llvm::BasicBlock* blockStmtBody = NULL;

    getFunction()->codegenUniqueNum++;

    blockStmtBody = llvm::BasicBlock::Create(info->module->getContext(), FNAME("blk_body"));

    info->builder->CreateBr(blockStmtBody);

    // Now add the body.
    func->getBasicBlockList().push_back(blockStmtBody);

    info->builder->SetInsertPoint(blockStmtBody);

    info->lvt->addLayer();

    body.codegen("");

    info->lvt->removeLayer();

    INT_ASSERT(blockStmtBody->getParent() == func);
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

Expr*
BlockStmt::getFirstExpr() {
  Expr* retval = 0;

  if (blockInfo != 0)
    retval = blockInfo->getFirstExpr();

  else if (body.head      != 0)
    retval = body.head->getFirstExpr();

  else
    retval = this;

  return retval;
}

Expr*
BlockStmt::getNextExpr(Expr* expr) {
  Expr* retval = this;

  if (expr == blockInfo && body.head != 0)
    retval = body.head->getFirstExpr();

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
BlockStmt::isRealBlockStmt() const {
  return blockInfo == 0;
}

bool
BlockStmt::isScopeless() const {
  return blockTag == BLOCK_SCOPELESS;
}

bool
BlockStmt::isBlockType(PrimitiveTag tag) const {
  return blockInfo != 0 && blockInfo->isPrimitive(tag) == true;
}

bool
BlockStmt::isLoopStmt() const {
  return false;
}

bool
BlockStmt::isWhileStmt() const {
  return false;
}

bool
BlockStmt::isWhileDoStmt() const {
  return false;
}

bool
BlockStmt::isDoWhileStmt() const {
  return false;
}

bool
BlockStmt::isParamForLoop() const {
  return false;
}

bool
BlockStmt::isForLoop() const {
  return false;
}

bool
BlockStmt::isCoforallLoop() const {
  return false;
}

bool
BlockStmt::isCForLoop() const {
  return false;
}

void
BlockStmt::checkConstLoops() {

}

bool
BlockStmt::deadBlockCleanup() {
  return false;
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
  elseStmt(NULL) {

  if (Expr* s = toExpr(iThenStmt)) {
    BlockStmt* bs = toBlockStmt(s);

    if (bs && bs->blockTag == BLOCK_NORMAL && bs->isRealBlockStmt())
      thenStmt = bs;
    else
      thenStmt = new BlockStmt(s);
  } else {
    INT_FATAL(iThenStmt, "Bad then-stmt passed to CondStmt constructor");
  }

  if (iElseStmt) {
    if (Expr* s = toExpr(iElseStmt)) {
      BlockStmt* bs = toBlockStmt(s);

      if (bs && bs->blockTag == BLOCK_NORMAL && bs->isRealBlockStmt())
        elseStmt = bs;
      else
        elseStmt = new BlockStmt(s);

    } else {
      INT_FATAL(iElseStmt, "Bad else-stmt passed to CondStmt constructor");
    }
  }

  gCondStmts.add(this);
}

Expr*
CondStmt::foldConstantCondition() {
  Expr* result = NULL;

  if (SymExpr* cond = toSymExpr(condExpr)) {
    if (VarSymbol* var = toVarSymbol(cond->var)) {
      if (var->immediate && var->immediate->const_kind == NUM_KIND_BOOL) {

        SET_LINENO(this);

        result = new CallExpr(PRIM_NOOP);

        insertBefore(result);

        if (var->immediate->bool_value() == gTrue->immediate->bool_value()) {
          Expr* then_stmt = thenStmt;

          then_stmt->remove();
          replace(then_stmt);

        } else {
          Expr* else_stmt = elseStmt;

          if (else_stmt) {
            else_stmt->remove();
            replace(else_stmt);
          } else {
            remove();
          }
        }
      }
    }
  }

  return result;
}

void
CondStmt::verify() {
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

  if (condExpr->parentExpr != this)
    INT_FATAL(this, "Bad CondStmt::condExpr::parentExpr");

  if (thenStmt->parentExpr != this)
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


void
CondStmt::replaceChild(Expr* old_ast, Expr* new_ast) {
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


GenRet
CondStmt::codegen() {
  GenInfo* info    = gGenInfo;
  FILE*    outfile = info->cfile;
  GenRet   ret;

  codegenStmt(this);

  if ( outfile ) {
    info->cStatements.push_back("if (" + codegenValue(condExpr).c + ") ");

    thenStmt->codegen();

    if (elseStmt) {
      info->cStatements.push_back(" else ");
      elseStmt->codegen();
    }

  } else {
#ifdef HAVE_LLVM
    llvm::Function* func = info->builder->GetInsertBlock()->getParent();

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

    if (elseStmt) {
      condStmtElse = llvm::BasicBlock::Create(info->module->getContext(),
                                              FNAME("cond_else"));
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

void
CondStmt::accept(AstVisitor* visitor) {
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

Expr*
CondStmt::getFirstExpr() {
  return (condExpr != 0) ? condExpr->getFirstExpr() : this;
}

Expr*
CondStmt::getNextExpr(Expr* expr) {
  Expr* retval = this;

  if (expr == condExpr && thenStmt != NULL)
    retval = thenStmt->getFirstExpr();

  else if (expr == thenStmt && elseStmt != NULL)
    retval = elseStmt->getFirstExpr();

  return retval;
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

Expr* GotoStmt::getFirstExpr() {
  return (label != 0) ? label->getFirstExpr() : this;
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

Expr* ExternBlockStmt::getFirstExpr() {
  INT_FATAL(this, "unexpected ExternBlockStmt in getFirstExpr");
  return NULL;
}
