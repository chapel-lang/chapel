/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include "AstDump.h"

#include "expr.h"
#include "IfExpr.h"
#include "log.h"
#include "LoopExpr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"

#include "WhileDoStmt.h"
#include "DoWhileStmt.h"
#include "CForLoop.h"
#include "ForallStmt.h"
#include "ForLoop.h"
#include "LoopStmt.h"
#include "ParamForLoop.h"
#include "TryStmt.h"
#include "CatchStmt.h"
#include "DeferStmt.h"

AstDump::AstDump() {
  mName      =     0;
  mPath      =     0;
  mFP        =     0;
  mIndent    =     0;
  mNeedSpace   = false;
  mDontCloseFP = false;
}

AstDump::AstDump(FILE* fp) {
  mName      =     0;
  mPath      =     0;
  mFP        =     fp;
  mIndent    =     0;
  mNeedSpace   = false;
  mDontCloseFP = true;
}

AstDump::~AstDump() {
  close();
}

void AstDump::view(const char* passName, int passNum) {
  forv_Vec(ModuleSymbol, module, allModules) {
    if (log_module[0] == '\0' || strcmp(log_module, module->name) == 0) {
      AstDump logger;

      if (logger.open(module, passName, passNum) == true) {
        module->accept(&logger);
        logger.close();
      }
    }
  }
}

bool AstDump::open(const ModuleSymbol* module, const char* passName, int passNum) {
  char numBuf[4];

  snprintf(numBuf, 4, "%02d", passNum);

  mName      = astr(module->name, "_", numBuf, passName, ".ast");
  mPath      = astr(log_dir, mName);
  mFP        = fopen(mPath, "w");
  mIndent    = 0;
  mNeedSpace = false;

  if (mFP != 0) {
    fprintf(mFP, "AST dump for %s after pass %s.\n", module->name, passName);
    fprintf(mFP, "Module use list: ");
    for_vector(ModuleSymbol, usedMod, module->modUseList) {
      fprintf(mFP, "%s ", usedMod->name);
    }
    fprintf(mFP, "\n");
  }

  return (mFP != 0) ? true : false;
}

bool AstDump::close() {
  bool retval = false;

  if (mDontCloseFP)
    mFP = 0;

  if (mFP != 0 && fclose(mFP) == 0) {
    mFP    =    0;
    retval = true;
  }

  return retval;
}

//
// ArgSymbol
//
bool AstDump::enterArgSym(ArgSymbol* node) {
  return false;
}

//
// CallExpr
//
bool AstDump::enterCallExpr(CallExpr* node) {
  if (isBlockStmt(node->parentExpr)) {
    newline();
  }

  if (FnSymbol* fn = node->theFnSymbol()) {
    if (fn->hasFlag(FLAG_BEGIN_BLOCK))
      write("begin");
    else if (fn->hasFlag(FLAG_ON_BLOCK))
      write("on");
  }

  if (fLogIds) {
    fprintf(mFP, "(%d ", node->id);
    mNeedSpace = false;
  } else {
    write(true, "(", false);
  }

  if (!node->primitive) {
    write("call");
  } else {
    if (node->isPrimitive(PRIM_RETURN))
      write("return");

    else if (node->isPrimitive(PRIM_YIELD))
      write("yield");

    else {
      if (mNeedSpace)
        fputc(' ', mFP);

      fprintf(mFP, "'%s'", node->primitive->name);

      mNeedSpace = true;
    }
  }

  if (node->partialTag)
    write("(partial)");

  return true;
}

void AstDump::exitCallExpr(CallExpr* node) {
  write(false, ")", true);
}


//
// DefExpr
//
bool AstDump::enterDefExpr(DefExpr* node) {
  Symbol* sym    = node->sym;
  bool    retval = true;

  if (isModuleSymbol(sym)) {
    retval = false;

  } else {
    if (isBlockStmt(node->parentExpr) ||
        isForallStmt(node->parentExpr)) {
      newline();
    }

    if (FnSymbol* fn = toFnSymbol(sym)) {
      writeFnSymbol(fn);

    } else if (isTypeSymbol(sym)) {
      if (isAggregateType(sym->type)) {
        if (sym->hasFlag(FLAG_SYNC))
          write("sync");

        if (sym->hasFlag(FLAG_SINGLE))
          write("single");
      }

      writeSymbol("type", sym, true);

    } else if (VarSymbol* vs = toVarSymbol(sym)) {
      if (isSyncType(vs->type)) {
        write("sync");

      } else if (isSingleType(vs->type)) {
        write("single");
      }

      write(true, sym->qualType().qualStr(), false);
      writeSymbol("", sym, true);
      writeFlags(mFP, sym);

    } else if (isLabelSymbol(sym)) {
      writeSymbol("label", sym, true);

    } else if (isEnumSymbol(sym)) {
      writeSymbol("def", sym, true);

    }
  }

  return retval;
}

//
// NamedExpr
//
bool AstDump::enterNamedExpr(NamedExpr* node) {
  if (isBlockStmt(node->parentExpr)) {
    newline();
  }

  fprintf(mFP, "(%s =", node->name);
  mNeedSpace = true;

  return true;
}

void AstDump::exitNamedExpr(NamedExpr* node) {
  write(false, ")", true);
}


//
// IfExpr
//

bool AstDump::enterIfExpr(IfExpr* node) {
  if (fLogIds) {
    fprintf(mFP, "(%d ", node->id);
    mNeedSpace = false;
  } else {
    write(true, "(", false);
  }
  write("IfExpr ");
  node->getCondition()->accept(this);
  write("then");
  node->getThenStmt()->accept(this);
  write("else");
  node->getElseStmt()->accept(this);
  write(")");
  return false;
}

void AstDump::exitIfExpr(IfExpr* node) {
}


//
// SymExpr
//
void AstDump::visitSymExpr(SymExpr* node) {
  Symbol*    sym = node->symbol();
  VarSymbol* var = toVarSymbol(sym);

  if (isBlockStmt(node->parentExpr) == true) {
    newline();
  }

  if (var != 0 && var->immediate != 0) {
    const size_t bufSize = 128;
    char         imm[bufSize];
    char         buff[bufSize + 1];

    snprint_imm(imm, bufSize, *var->immediate);
    sprintf(buff, "%s%s", imm, is_imag_type(var->type) ? "i" : "");

    write(buff);

  } else {
    writeSymbol(sym, false);
  }
}


//
// UnresolvedSymExpr
//
void AstDump::visitUsymExpr(UnresolvedSymExpr* node) {
  if (isBlockStmt(node->parentExpr)) {
    newline();
  }

  write(node->unresolved);
}

//
// LoopExpr
//
bool AstDump::enterLoopExpr(LoopExpr* node) {
  if (isBlockStmt(node->parentExpr)) {
    newline();
  }

  if (fLogIds) {
    fprintf(mFP, "(%d ", node->id);
    mNeedSpace = false;
  } else {
    write(true, "(", false);
  }

  if (node->forall) {
    if (node->maybeArrayType) write("[ ");
    else write("forall ");
  } else {
    write("for ");
  }

  if (node->indices) {
    node->indices->accept(this);
    write(" in ");
  }

  if (node->zippered) write("zip");
  node->iteratorExpr->accept(this);

  if (node->maybeArrayType) write("]");
  else write("do");

  if (node->cond) {
    write(" if ");
    node->cond->accept(this);
    write(" then ");
  }

  node->loopBody->accept(this);

  write(")");

  return false;
}

void AstDump::exitLoopExpr(LoopExpr* node) {
}


//
// UseStmt
//
void AstDump::visitUseStmt(UseStmt* node) {
  if (isBlockStmt(node->parentExpr)) {
    newline();
  }

  if (fLogIds) {
    fprintf(mFP, "(%d ", node->id);
    mNeedSpace = false;
  } else {
    write(true, "(", false);
  }

  if (mNeedSpace)
    fputc(' ', mFP);

  fprintf(mFP, "'use'");

  mNeedSpace = true;

  node->src->accept(this);

  if (node->isARename()) {
    fprintf(mFP, " 'as' %s", node->getRename());
  }

  if (!node->isPlainUse()) {
    node->writeListPredicate(mFP);
    bool first = outputVector(mFP, node->named);
    outputRenames(mFP, node->renamed, first);
  }

  write(false, ")", true);
}



//
// BlockStmt
//
bool AstDump::enterBlockStmt(BlockStmt* node) {
  newline();

  if (FnSymbol* fn = toFnSymbol(node->parentSymbol))
    if (node == fn->where)
      write(false, "where ", false);

  write("{");
  printBlockID(node);
  if ((node->blockTag & BLOCK_SCOPELESS))
    write("scopeless");
  if ((node->blockTag & BLOCK_TYPE_ONLY))
    write("type");
  if ((node->blockTag & BLOCK_EXTERN))
    write("extern");

  ++mIndent;

  return true;
}

void AstDump::exitBlockStmt(BlockStmt* node) {
  --mIndent;
  newline();
  write(false, "}", true);
  printBlockID(node);
}


bool AstDump::enterForallStmt(ForallStmt* node) {
  newline();
  write("Forall");
  write("{");
  ++mIndent;
  newline();
  write("induction variables:");
  ++mIndent;
  for_alist(expr, node->inductionVariables()) {
    newline();
    expr->accept(this);
  }
  --mIndent;
  newline();
  write("iterated expressions:");
  ++mIndent;
  for_alist(expr, node->iteratedExpressions()) {
    newline();
    expr->accept(this);
  }
  --mIndent;
  newline();
  write("shadow variables:");
  ++mIndent;
  for_alist(expr, node->shadowVariables()) {
    if (DefExpr* def = toDefExpr(expr)) {
      if (ShadowVarSymbol* sv = toShadowVarSymbol(def->sym)) {
        newline();
        writeSymbol(sv, true);
        write(sv->intentDescrString());
        if (sv->outerVarSym()) {
          write("outer var");
          writeSymbol(sv->outerVarSym(), false);
        }
        ++mIndent;
        if (sv->initBlock()) {
          newline();
          write("init block");
          sv->initBlock()->accept(this);
        }
        if (sv->deinitBlock()) {
          newline();
          write("deinit block");
          sv->deinitBlock()->accept(this);
        }
        --mIndent;
      }
    }
  }
  --mIndent;
  newline();
  write("other variables");
  ++mIndent;
  if (node->fRecIterIRdef) node->fRecIterIRdef->accept(this);
  if (node->fRecIterICdef) node->fRecIterICdef->accept(this);
  if (node->fRecIterGetIterator) node->fRecIterGetIterator->accept(this);
  if (node->fRecIterFreeIterator) node->fRecIterFreeIterator->accept(this);
  newline();
  --mIndent;
  write("forall body");
  node->loopBody()->accept(this);
  --mIndent;
  newline();
  write("}");
  return false;
}
void AstDump::exitForallStmt(ForallStmt* node) {
}

//
// WhileDoStmt
//
bool AstDump::enterWhileDoStmt(WhileDoStmt* node) {
  newline();

  if (FnSymbol* fn = toFnSymbol(node->parentSymbol))
    if (node == fn->where)
      write(false, "where ", false);

  write("WhileDo");
  printLoopStmtDetails(node);
  newline();
  write("{");
  printBlockID(node);
  ++mIndent;

  return true;
}

void AstDump::exitWhileDoStmt(WhileDoStmt* node) {
  --mIndent;
  newline();
  write(false, "}", true);
  printBlockID(node);
}


//
// DoWhileStmt
//
bool AstDump::enterDoWhileStmt(DoWhileStmt* node) {
  newline();

  if (FnSymbol* fn = toFnSymbol(node->parentSymbol))
    if (node == fn->where)
      write(false, "where ", false);

  write("DoWhile");
  printLoopStmtDetails(node);
  newline();
  write("{");
  printBlockID(node);
  ++mIndent;

  return true;
}

void AstDump::exitDoWhileStmt(DoWhileStmt* node) {
  --mIndent;
  newline();
  write(false, "}", true);
  printBlockID(node);
}


//
// ForLoop
//
bool AstDump::enterForLoop(ForLoop* node) {
  newline();

  if (FnSymbol* fn = toFnSymbol(node->parentSymbol))
    if (node == fn->where)
      write(false, "where ", false);

  write("ForLoop");
  printLoopStmtDetails(node);
  if (node->isLoweredForallLoop())
    write("lowered-forall");
  if (node->isForExpr())
    write("for-expr");
  newline();
  write("{");
  printBlockID(node);
  ++mIndent;

  return true;
}

void AstDump::exitForLoop(ForLoop* node) {
  --mIndent;
  newline();
  write(false, "}", true);
  printBlockID(node);
}


//
// CForLoop
//
bool AstDump::enterCForLoop(CForLoop* node) {
  newline();

  if (FnSymbol* fn = toFnSymbol(node->parentSymbol))
    if (node == fn->where)
      write(false, "where ", false);

  write("CForLoop");
  printLoopStmtDetails(node);
  newline();
  write("{");
  printBlockID(node);
  ++mIndent;

  return true;
}

void AstDump::exitCForLoop(CForLoop* node) {
  --mIndent;
  newline();
  write(false, "}", true);
  printBlockID(node);
}


//
// ParamForLoop
//
bool AstDump::enterParamForLoop(ParamForLoop* node) {
  newline();

  if (FnSymbol* fn = toFnSymbol(node->parentSymbol))
    if (node == fn->where)
      write(false, "where ", false);

  write("ParamForLoop");
  printLoopStmtDetails(node);
  newline();
  write("{");
  printBlockID(node);
  ++mIndent;

  return true;
}

void AstDump::exitParamForLoop(ParamForLoop* node) {
  --mIndent;
  newline();
  write(false, "}", true);
  printBlockID(node);
}


//
// CondStmt
//
bool AstDump::enterCondStmt(CondStmt* node) {
  newline();
  write("if");

  return true;
}

//
// ExternBlockStmt
//
void AstDump::visitEblockStmt(ExternBlockStmt* node) {
  write("(ExternBlockStmt");
}


//
// GotoStmt
//
bool AstDump::enterGotoStmt(GotoStmt* node) {
  newline();
  switch (node->gotoTag) {
    case GOTO_NORMAL:         write("goto");              break;
    case GOTO_BREAK:          write("break");             break;
    case GOTO_CONTINUE:       write("continue");          break;
    case GOTO_RETURN:         write("gotoReturn");        break;
    case GOTO_GETITER_END:    write("gotoGetiterEnd");    break;
    case GOTO_ITER_RESUME:    write("gotoIterResume");    break;
    case GOTO_ITER_END:       write("gotoIterEnd");       break;
    case GOTO_ERROR_HANDLING: write("gotoErrorHandling"); break;
    case GOTO_BREAK_ERROR_HANDLING: write("gotoBreakErrorHandling"); break;
  }

  if (SymExpr* label = toSymExpr(node->label)) {
    if (label->symbol() != gNil) {
      writeSymbol(label->symbol(), true);
    }
  }

  return true;
}


//
// ForwardingStmt
//
bool AstDump::enterForwardingStmt(ForwardingStmt* node) {
  write("forwarding (");
  return true;
}

void AstDump::exitForwardingStmt(ForwardingStmt* node) {
  write(")");
}


//
// DeferStmt
//
bool AstDump::enterDeferStmt(DeferStmt* node) {
  newline();
  write("Defer");
  newline();
  write("{");
  printBlockID(node);
  ++mIndent;
  return true;
}

void AstDump::exitDeferStmt(DeferStmt* node) {
  --mIndent;
  newline();
  write("}");
}


//
// TryStmt
//
bool AstDump::enterTryStmt(TryStmt* node) {
  newline();
  if (node->tryBang()) {
    write("Try!");
  } else {
    write("Try");
  }
  newline();
  write("{");
  ++mIndent;
  return true;
}

void AstDump::exitTryStmt(TryStmt* node) {
  --mIndent;
  newline();
  write("}");
}

//
// CatchStmt
//
bool AstDump::enterCatchStmt(CatchStmt* node) {
  newline();
  write("Catch");
  if (node->name() != NULL)
    write(node->name());
  if (UnresolvedSymExpr* urse = toUnresolvedSymExpr(node->type())) {
    write(":");
    write(urse->unresolved);
  } else if (SymExpr* se = toSymExpr(node->type())) {
    write(":");
    write(se->symbol()->name);
  }
  newline();
  write("{");
  ++mIndent;
  return true;
}

void AstDump::exitCatchStmt(CatchStmt* node) {
  --mIndent;
  newline();
  write("}");
}

//
// Helper functions
//

void AstDump::writeFnSymbol(FnSymbol* fn) {
  bool first = true;

  write("function");

  if (fn->_this && fn->_this->defPoint) {
    writeSymbol(fn->_this->type->symbol, false);
    write(false, ".", false);
  }

  writeSymbol(fn, true);

  write(false, "(", false);

  for_formals(formal, fn) {
    if (first) {
      first = false;
    } else {
      write(false, ",", true);
    }

    writeSymbol(formal, true);

    if (formal->typeExpr) {
      write(":");

      if (BlockStmt* block = toBlockStmt(formal->typeExpr))
        block->body.first()->accept(this);
      else
        formal->typeExpr->accept(this);
    }

    if (formal->defaultExpr) {
      write("=");

      if (BlockStmt* block = toBlockStmt(formal->defaultExpr))
        block->body.first()->accept(this);
      else
        formal->defaultExpr->accept(this);
    }
  }

  write( false, ")", true);

  switch (fn->retTag) {
    case RET_VALUE:                 break;
    case RET_REF:   write("ref");   break;
    case RET_CONST_REF:   write("const ref");   break;
    case RET_PARAM: write("param"); break;
    case RET_TYPE:  write("type");  break;
  }

  if (fn->retType && fn->retType->symbol) {
    writeSymbol(":", fn->retType->symbol, false);
  }

  if (fn->throwsError()) {
    write("throws");
  }

  writeFlags(mFP, fn);
}

void AstDump::writeSymbol(const char* tag, Symbol* sym, bool def) {
  write(tag);
  writeSymbol(sym, def);
}

void AstDump::writeSymbol(Symbol* sym, bool def) {
  if (def) {
    if (ArgSymbol* arg = toArgSymbol(sym)) {
      switch (arg->intent) {
        case INTENT_IN:        write("in arg");        break;
        case INTENT_INOUT:     write("inout arg");     break;
        case INTENT_OUT:       write("out arg");       break;
        case INTENT_CONST:     write("const arg");     break;
        case INTENT_CONST_IN:  write("const in arg");  break;

        case INTENT_CONST_REF:
        case INTENT_REF:
        case INTENT_REF_MAYBE_CONST: {
          if ( (arg->intent & INTENT_FLAG_CONST) )
            write("const ");
          else if ( (arg->intent & INTENT_FLAG_MAYBE_CONST) )
            write("const? ");

          if (arg->isWideRef()) {
            write("wide-ref arg");
          } else {
            write("ref arg");
          }
          break;
        }
        case INTENT_PARAM:     write("param arg");     break;
        case INTENT_TYPE:      write("type arg");      break;
        case INTENT_BLANK:     write("arg");           break;
      }
    }
  }

  write(sym->name);

  if (fLogIds)
    fprintf(mFP, "[%d]", sym->id);

  if (def &&
      !toTypeSymbol(sym) &&
      sym->type &&
      sym->type->symbol &&
      sym->type != dtUnknown) {
    write(false, ":", false);
    writeSymbol(sym->type->symbol, false);
  }

  if (sym->hasFlag(FLAG_GENERIC))
    write(false, "(?)", false);

  if (def)
    if (ArgSymbol* arg = toArgSymbol(sym))
      if (arg->variableExpr)
        write("...");

  mNeedSpace = true;
}

void AstDump::write(const char* text) {
  write(true, text, true);
}

void AstDump::write(bool spaceBefore, const char* text, bool spaceAfter) {
  if (spaceBefore == true && mNeedSpace == true)
    fputc(' ', mFP);

  fputs(text, mFP);

  mNeedSpace = spaceAfter;
}

void AstDump::printBlockID(Expr* expr) {
  if (fdump_html_print_block_IDs)
    fprintf(mFP, " %d", expr->id);
}

void AstDump::printLoopStmtDetails(LoopStmt* loop) {
  if (fLogIds)
    fprintf(mFP, "[%d]", loop->id);
  if (loop->hasVectorizationHazard())
    write("hazard");
  if (loop->isOrderIndependent())
    write("order-independent");
}

void AstDump::newline() {
  fputc('\n', mFP);

  for (int i = 0; i < 2 * mIndent; ++i)
    fputc(' ', mFP);

  mNeedSpace = false;
}
