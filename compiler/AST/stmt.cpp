#include <cstring>
#include "astutil.h"
#include "expr.h"
#include "files.h"
#include "misc.h"
#include "passes.h"
#include "stmt.h"
#include "stringutil.h"
#include "codegen.h"


// remember these so we can update their labels' iterResumeGoto
Map<GotoStmt*,GotoStmt*> copiedIterResumeGotos;

// remember these so we can remove their iterResumeGoto
Vec<LabelSymbol*> removedIterResumeLabels;

#ifdef HAVE_LLVM

#define FNAME(str) (llvm::Twine(getFunction()->cname) + llvm::Twine("_") + llvm::Twine(getFunction()->codegenUniqueNum) + llvm::Twine(str) + llvm::Twine("_"))

#endif

void codegenStmt(Expr* stmt) {
  GenInfo* info = gGenInfo;
  FILE* outfile = info->cfile;
  info->lineno = stmt->linenum();
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
}


BlockStmt::BlockStmt(Expr* init_body, BlockTag init_blockTag) :
  Expr(E_BlockStmt),
  blockTag(init_blockTag),
  body(),
  blockInfo(NULL),
  modUses(NULL),
  breakLabel(NULL),
  continueLabel(NULL),
  userLabel(NULL),
  byrefVars(NULL)
{
  body.parent = this;
  if (init_body)
    body.insertAtTail(init_body);
  gBlockStmts.add(this);
}


BlockStmt::~BlockStmt() { }


void BlockStmt::verify() {
  Expr::verify();
  if (astTag != E_BlockStmt) {
    INT_FATAL(this, "Bad BlockStmt::astTag");
  }
  if (body.parent != this)
    INT_FATAL(this, "Bad AList::parent in BlockStmt");
  for_alist(expr, body) {
    if (expr->parentExpr != this)
      INT_FATAL(this, "Bad BlockStmt::body::parentExpr");
  }
  if (blockInfo && blockInfo->parentExpr != this)
    INT_FATAL(this, "Bad BlockStmt::blockInfo::parentExpr");
  if (modUses && modUses->parentExpr != this)
    INT_FATAL(this, "Bad BlockStmt::blockInfo::parentExpr");
  if (byrefVars) {
    if (byrefVars->parentExpr != this)
      INT_FATAL(this, "Bad BlockStmt::byrefVars::parentExpr");
    for_actuals(varExp, byrefVars) {
      if (!isSymExpr(varExp) && !isUnresolvedSymExpr(varExp))
        INT_FATAL(this, "Bad expresion kind in BlockStmt::byrefVars");
    }
  }
}


BlockStmt*
BlockStmt::copyInner(SymbolMap* map) {
  BlockStmt* _this = new BlockStmt();
  _this->blockTag = blockTag;
  for_alist(expr, body)
    _this->insertAtTail(COPY_INT(expr));
  _this->blockInfo = COPY_INT(blockInfo);
  _this->modUses = COPY_INT(modUses);
  _this->breakLabel = breakLabel;
  _this->continueLabel = continueLabel;
  _this->byrefVars = COPY_INT(byrefVars);
  return _this;
}


void BlockStmt::replaceChild(Expr* old_ast, Expr* new_ast) {
  if (old_ast == blockInfo)
    blockInfo = toCallExpr(new_ast);
  else if (old_ast == modUses)
    modUses = toCallExpr(new_ast);
  else if (old_ast == byrefVars)
    byrefVars = toCallExpr(new_ast);
  else
    INT_FATAL(this, "Unexpected case in BlockStmt::replaceChild");
}


GenRet BlockStmt::codegen() {
  GenInfo* info = gGenInfo;
  FILE* outfile = info->cfile;
  GenRet ret;

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

    llvm::BasicBlock *blockStmtCond = NULL;
    llvm::BasicBlock *blockStmtBody = NULL;
    llvm::BasicBlock *blockStmtEndCond = NULL;
    llvm::BasicBlock *blockStmtEnd = NULL;
 
    blockStmtBody = llvm::BasicBlock::Create(
        info->module->getContext(), FNAME("blk_body"));
   
    if(blockInfo) {
      blockStmtEnd = llvm::BasicBlock::Create(
          info->module->getContext(), FNAME("blk_end"));
      if (blockInfo->isPrimitive(PRIM_BLOCK_WHILEDO_LOOP) ||
          blockInfo->isPrimitive(PRIM_BLOCK_FOR_LOOP)) {
        // Add the condition block.
        blockStmtCond = llvm::BasicBlock::Create(
            info->module->getContext(), FNAME("blk_cond"));
        func->getBasicBlockList().push_back(blockStmtCond);
        // Insert an explicit branch from the current block to the loop start.
        info->builder->CreateBr(blockStmtCond);
        // Now switch to the condition for code generation 
        info->builder->SetInsertPoint(blockStmtCond);

        llvm::Value *condValue =
          info->builder->CreateLoad(blockInfo->get(1)->codegen().val);
        condValue = info->builder->CreateICmpNE(
            condValue,
            llvm::ConstantInt::get(condValue->getType(), 0),
            FNAME("condition"));

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
      if(blockInfo->isPrimitive(PRIM_BLOCK_DOWHILE_LOOP)) {
        // Add the condition block.
        blockStmtEndCond = llvm::BasicBlock::Create(
            info->module->getContext(), FNAME("blk_end_cond"));
        func->getBasicBlockList().push_back(blockStmtEndCond);
        // Insert an explicit branch from the body block to the loop condition.
        info->builder->CreateBr(blockStmtEndCond);
        // set insert point
        info->builder->SetInsertPoint(blockStmtEndCond);
        llvm::Value *condValue =
          info->builder->CreateLoad(blockInfo->get(1)->codegen().val);
        condValue = info->builder->CreateICmpNE(
            condValue,
            llvm::ConstantInt::get(condValue->getType(), 0),
            FNAME("condition"));
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
BlockStmt::isLoop(void) {
  return (blockInfo &&
          (blockInfo->isPrimitive(PRIM_BLOCK_DOWHILE_LOOP) ||
           blockInfo->isPrimitive(PRIM_BLOCK_WHILEDO_LOOP) ||
           blockInfo->isPrimitive(PRIM_BLOCK_PARAM_LOOP) ||
           blockInfo->isPrimitive(PRIM_BLOCK_FOR_LOOP)));
}


int
BlockStmt::length(void) {
  return body.length;
}


void
BlockStmt::addUse(ModuleSymbol* mod) {
  if (!modUses) {
    modUses = new CallExpr(PRIM_USED_MODULES_LIST);
    if (parentSymbol)
      insert_help(modUses, this, parentSymbol);
  }
  modUses->insertAtTail(mod);
}


CondStmt::CondStmt(Expr* iCondExpr, BaseAST* iThenStmt, BaseAST* iElseStmt) :
  Expr(E_CondStmt),
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
          var->immediate->const_kind == NUM_KIND_UINT &&
          var->immediate->num_index == INT_SIZE_1) {
        SET_LINENO(this);
        result = new CallExpr(PRIM_NOOP);
        this->insertBefore(result);
        if (var->immediate->v_bool == gTrue->immediate->v_bool) {
          Expr* then_stmt = thenStmt;
          then_stmt->remove();
          this->replace(then_stmt);
        } else if (var->immediate->v_bool == gFalse->immediate->v_bool) {
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
    
    llvm::Value *condValue = condExpr->codegen().val;

    condValue = info->builder->CreateLoad(condValue, FNAME("condValue"));
    condValue = info->builder->CreateICmpNE(
        condValue,
        llvm::ConstantInt::get(condValue->getType(), 0),
        FNAME("condition"));
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


GotoStmt::GotoStmt(GotoTag init_gotoTag, const char* init_label) :
  Expr(E_GotoStmt),
  gotoTag(init_gotoTag),
  label(init_label ? (Expr*)new UnresolvedSymExpr(init_label)
                   : (Expr*)new SymExpr(gNil))
{
  gGotoStmts.add(this);
}


GotoStmt::GotoStmt(GotoTag init_gotoTag, Symbol* init_label) :
  Expr(E_GotoStmt),
  gotoTag(init_gotoTag),
  label(new SymExpr(init_label))
{
  gGotoStmts.add(this);
}


GotoStmt::GotoStmt(GotoTag init_gotoTag, Expr* init_label) :
  Expr(E_GotoStmt),
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
  if (SymExpr* se = toSymExpr(label)) {
    if (isLabelSymbol(se->var)) {
      if (!isFnSymbol(se->var->defPoint->parentSymbol))
        INT_FATAL(this, "goto label is not in a function");
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



ExternBlockStmt::ExternBlockStmt(const char* init_c_code) :
  Expr(E_ExternBlockStmt),
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


ExternBlockStmt*
ExternBlockStmt::copyInner(SymbolMap* map) {
  ExternBlockStmt* copy = new ExternBlockStmt(c_code);

  return copy;
}

