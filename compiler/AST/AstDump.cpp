#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include "AstDump.h"

#include "driver.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"


AstDump::AstDump() {
  mName      =     0;
  mPath      =     0;
  mFP        =     0;
  mIndent    =     0;
  mNeedSpace = false;
}

AstDump::~AstDump() {
  close();
}

void AstDump::view(const char* passName, int passNum) {
  forv_Vec(ModuleSymbol, module, allModules) {
    if (log_module[0] == '\0' || strcmp(log_module, module->name) == 0) {
      AstDump logger;

      if (logger.open(module, passName, passNum) == true) {
        logger.write(module);
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
  }

  return (mFP != 0) ? true : false;
}

void AstDump::write(BaseAST* ast) {
  if (header(ast) == true) {
    AST_CHILDREN_CALL(ast, tempHack, this);

    footer(ast);
  }
}

bool AstDump::close() {
  bool retval = false;
  
  if (mFP != 0 && fclose(mFP) == 0) {
    mFP    =    0;
    retval = true;
  }

  return retval;
}

bool AstDump::header(BaseAST* ast) {
  bool retval = true;

  if (BlockStmt* stmt = toBlockStmt(ast)) {
    newline();

    if (FnSymbol* fn = toFnSymbol(stmt->parentSymbol))
      if (stmt == fn->where)
        write(false, "where ", false);

    write("{");
    printBlockID(stmt);
    ++mIndent;

  } else if (isExternBlockStmt(ast)) {
    write("(ExternBlockStmt");

  } else if (GotoStmt* s = toGotoStmt(ast)) {
    switch (s->gotoTag) {
      case GOTO_NORMAL:      write("goto");           break;
      case GOTO_BREAK:       write("break");          break;
      case GOTO_CONTINUE:    write("continue");       break;
      case GOTO_RETURN:      write("gotoReturn");     break;
      case GOTO_GETITER_END: write("gotoGetiterEnd"); break;
      case GOTO_ITER_RESUME: write("gotoIterResume"); break;
      case GOTO_ITER_END:    write("gotoIterEnd");    break;
    }

    if (SymExpr* label = toSymExpr(s->label)) {
      if (label->var != gNil) {
        writeSymbol(label->var, true);
      }
    }

  } else if (isCondStmt(ast)) {
    newline();
    write("if");

  } else if (DefExpr* e = toDefExpr(ast)) {
    Symbol* sym = e->sym;

    if (isModuleSymbol(sym)) {
      retval = false;

    } else {
      if (isBlockStmt(e->parentExpr)) {
        newline();
      }

      if (FnSymbol* fn = toFnSymbol(sym)) {
        writeFnSymbol(fn);

      } else if (isTypeSymbol(sym)) {
        if (toAggregateType(sym->type)) {
          if (sym->hasFlag(FLAG_SYNC))
            write("sync");

          if (sym->hasFlag(FLAG_SINGLE))
            write("single");
        }

        writeSymbol("type", sym, true);

      } else if (VarSymbol* vs = toVarSymbol(sym)) {
        if (vs->type->symbol->hasFlag(FLAG_SYNC))
          write("sync");

        if (vs->type->symbol->hasFlag(FLAG_SINGLE))
          write("single");

        writeSymbol("var", sym, true);

      } else if (isLabelSymbol(sym)) {
        writeSymbol("label", sym, true);

      } else if (isEnumSymbol(sym)) {
        writeSymbol("def", sym, true);

      }
    }

  } else if (SymExpr* e = toSymExpr(ast)) {
    if (isBlockStmt(e->parentExpr)) {
      newline();
    }

    if (VarSymbol* c = get_constant(e)) {
      if (c->immediate) {
        const size_t bufSize = 128;
        char         imm[bufSize];
        char         buff[bufSize];

        snprint_imm(imm, bufSize, *c->immediate);
        sprintf(buff, "%s%s", imm, is_imag_type(c->type) ? "i" : "");

        write(buff);
      } else {
        write(c->name);
      }

    } else {
      writeSymbol(e->var, false);
    }

  } else if (UnresolvedSymExpr* e = toUnresolvedSymExpr(ast)) {
    if (isBlockStmt(e->parentExpr)) {
      newline();
    }

    write(e->unresolved);

  } else if (NamedExpr* e = toNamedExpr(ast)) {
    if (isBlockStmt(e->parentExpr)) {
      newline();
    }

    fprintf(mFP, "(%s =", e->name);
    mNeedSpace = true;

  } else if (CallExpr* e = toCallExpr(ast)) {
    if (isBlockStmt(e->parentExpr)) {
      newline();
    }

    if (FnSymbol* fn = e->isResolved()) {
      if (fn->hasFlag(FLAG_BEGIN_BLOCK))
        write("begin");
      else if (fn->hasFlag(FLAG_ON_BLOCK))
        write("on");
    }

    if (fLogIds) {
      fprintf(mFP, "(%d ", e->id);
      mNeedSpace = false;
    } else {
      write(true, "(", false);
    }

    if (!e->primitive) {
      write("call");
    } else {
      if (e->isPrimitive(PRIM_RETURN))
        write("return");

      else if (e->isPrimitive(PRIM_YIELD))
        write("yield");

      else {
        if (mNeedSpace)
          fputc(' ', mFP);

        fprintf(mFP, "'%s'", e->primitive->name);

        mNeedSpace = true;
      }
    }

    if (e->partialTag)
      write( true, "(partial)", true);

  } else if (isPrimitiveType(ast)) {

  } else if (isAggregateType(ast)) {

  } else if (isEnumType(ast)) {

  } else if (isVarSymbol(ast)) {

  } else if (isTypeSymbol(ast)) {

  } else if (isFnSymbol(ast)) {

  } else if (isEnumSymbol(ast)) {

  } else if (isModuleSymbol(ast)) {

  } else if (isLabelSymbol(ast)) {

  } else if (isArgSymbol(ast)) {
    retval = false;

  } else {
    USR_FATAL("This cannot happen");
  }

  return retval;
}

void AstDump::footer(BaseAST* ast) {
  if (isCallExpr(ast)) {
    write(false, ")", true);
  }

  else if (isNamedExpr(ast)) {
    write(false, ")", true);
  }

  else if (Expr* expr = toBlockStmt(ast)) {
    --mIndent;
    newline();
    write(false, "}", true);
    printBlockID(expr);
  }
}

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
        write(block->body.first());
      else
        write(formal->typeExpr);
    }

    if (formal->defaultExpr) {
      write("=");

      if (BlockStmt* block = toBlockStmt(formal->defaultExpr))
        write(block->body.first());
      else
        write(formal->defaultExpr);
    }
  }

  write( false, ")", true);

  switch (fn->retTag) {
    case RET_VALUE:                 break;
    case RET_VAR:   write("var");   break;
    case RET_PARAM: write("param"); break;
    case RET_TYPE:  write("type");  break;
  }

  if (fn->retType && fn->retType->symbol) {
    writeSymbol(":", fn->retType->symbol, false);
  }
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
        case INTENT_CONST_REF: write("const ref arg"); break;
        case INTENT_REF:       write("ref arg");       break;
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
    write(false, "?", false);

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

void AstDump::newline() {
  fputc('\n', mFP);

  for (int i = 0; i < 2 * mIndent; ++i)
    fputc(' ', mFP);

  mNeedSpace = false;
}

void AstDump::tempHack(BaseAST* ast, AstDump* handle) {
  handle->write(ast);
}
