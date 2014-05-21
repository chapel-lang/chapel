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
  if (Expr* expr = toExpr(ast))
    if (DefExpr* e = toDefExpr(expr))
      if (isModuleSymbol(e->sym))
        // Since we iterate modules at the top layer, don't visit nested modules here.
        return;

  if (Symbol* s = toSymbol(ast))
    if (isArgSymbol(s))
      // Don't show args; they are handled in log_ast_fnsymbol().
      return;

  header(ast);

  // This is a hook in to AST_CHILDREN_CALL macro until the Visitorc can be dropped in
  AST_CHILDREN_CALL(ast, tempHack, this);

  footer(ast);
}

bool AstDump::close() {
  bool retval = false;
  
  if (mFP != 0 && fclose(mFP) == 0) {
    mFP    =    0;
    retval = true;
  }

  return retval;
}

void AstDump::header(BaseAST* ast) {
  if (Expr* expr = toExpr(ast)) {
    if (isBlockStmt(expr)) {
      newline();

      if (FnSymbol* fn = toFnSymbol(expr->parentSymbol))
        if (expr == fn->where)
          write(false, "where ", false);

      write(false, "{", true);
      printBlockID(expr);
      ++mIndent;

    // This was the final dangling else
    } else if (isExternBlockStmt(expr)) {
      if (mNeedSpace)
        fputc(' ', mFP);

      fprintf(mFP, "(%s", expr->astTagAsString());
      mNeedSpace = true;

    } else if (GotoStmt* s = toGotoStmt(expr)) {
      switch (s->gotoTag) {
        case GOTO_NORMAL:      write(true, "goto",           true); break;
        case GOTO_BREAK:       write(true, "break",          true); break;
        case GOTO_CONTINUE:    write(true, "continue",       true); break;
        case GOTO_RETURN:      write(true, "gotoReturn",     true); break;
        case GOTO_GETITER_END: write(true, "gotoGetiterEnd", true); break;
        case GOTO_ITER_RESUME: write(true, "gotoIterResume", true); break;
        case GOTO_ITER_END:    write(true, "gotoIterEnd",    true); break;
      }

      if (SymExpr* label = toSymExpr(s->label)) {
        if (label->var != gNil) {
          writeSymbol(label->var, true);
        }
      }

    } else if (isCondStmt(expr)) {
      newline();
      write(true, "if", true);

    } else {
      if (isBlockStmt(expr->parentExpr)) {
        newline();
      }

      if (DefExpr* e = toDefExpr(expr)) {
        if (FnSymbol* fn = toFnSymbol(e->sym)) {
          write(true, "function", true);
          writeFnSymbol(fn);

        } else if (isTypeSymbol(e->sym)) {
          if (structuralTypeSymbol(e->sym)) {
            if (e->sym->hasFlag(FLAG_SYNC))
              write(true, "sync",   true);

            if (e->sym->hasFlag(FLAG_SINGLE))
              write(true, "single", true);
          }

          write(true, "type", true);
          writeSymbol(e->sym, true);

        } else if (VarSymbol* vs = toVarSymbol(e->sym)) {
          if (vs->type->symbol->hasFlag(FLAG_SYNC))
            write(true, "sync",   true);

          if (vs->type->symbol->hasFlag(FLAG_SINGLE))
            write(true, "single", true);

          write(true, "var", true);
          writeSymbol(e->sym, true);

        } else if (isArgSymbol(e->sym)) {
          // Argsymbols are handled in the function header.

        } else if (isLabelSymbol(e->sym)) {
          write(true, "label", true);
          writeSymbol(e->sym, true);

        } else if (isModuleSymbol(e->sym)) {
          write(true, "module", true);
          writeSymbol(e->sym, true);

        } else {
          write(true, "def", true);
          writeSymbol(e->sym, true);
        }

      } else if (SymExpr* e = toSymExpr(expr)) {

        if (VarSymbol* c = get_constant(expr)) {
          if (c->immediate) {
            const size_t bufSize = 128;
            char         imm[bufSize];

            snprint_imm(imm, bufSize, *c->immediate);

            if (mNeedSpace) 
              fputc(' ', mFP);

            fprintf(mFP, "%s%s", imm, is_imag_type(c->type) ? "i" : "");

            mNeedSpace = true;
          } else {
            write(true, c->name, true);
          }

        } else {
          writeSymbol(e->var, false);
        }

      } else if (UnresolvedSymExpr* e = toUnresolvedSymExpr(expr)) {
        write(true, e->unresolved, true);

      } else if (NamedExpr* e = toNamedExpr(expr)) {
        fprintf(mFP, "(%s =", e->name);
        mNeedSpace = true;

      } else if (CallExpr* e = toCallExpr(expr)) {
        if (FnSymbol* fn = e->isResolved()) {
          if (fn->hasFlag(FLAG_BEGIN_BLOCK))
            write(true, "begin", true);
          else if (fn->hasFlag(FLAG_ON_BLOCK))
            write(true, "on", true);
        }

        if (fLogIds) {
          fprintf(mFP, "(%d ", e->id);
          mNeedSpace = false;
        } else {
          write(true, "(", false);
        }

        if (!e->primitive) {
          write(true, "call", true);
        } else {
          if (e->isPrimitive(PRIM_RETURN))
            write(true, "return", true);

          else if (e->isPrimitive(PRIM_YIELD))
            write(true, "yield", true);

          else {
            if (mNeedSpace)
              fputc(' ', mFP);

            fprintf(mFP, "'%s'", e->primitive->name);

            mNeedSpace = true;
          }
        }

        if (e->partialTag)
          write( true, "(partial)", true);

      } else {
        USR_FATAL("This cannot happen");
      }
    }
  }
}

void AstDump::footer(BaseAST* ast) {
    if (isCallExpr(ast)) {
      write( false, ")", true);
    }

    else if (isNamedExpr(ast)) {
      write( false, ")", true);
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
      write(true, ":", true);

      if (BlockStmt* block = toBlockStmt(formal->typeExpr))
        write(block->body.first());
      else
        write(formal->typeExpr);
    }

    if (formal->defaultExpr) {
      write(true, "=", true);

      if (BlockStmt* block = toBlockStmt(formal->defaultExpr))
        write(block->body.first());
      else
        write(formal->defaultExpr);
    }
  }

  write( false, ")", true);

  switch (fn->retTag) {
    case RET_VALUE:                             break;
    case RET_VAR:   write(true, "var",   true); break;
    case RET_PARAM: write(true, "param", true); break;
    case RET_TYPE:  write(true, "type",  true); break;
  }

  if (fn->retType && fn->retType->symbol) {
    write(true, ":", true);
    writeSymbol(fn->retType->symbol, false);
  }
}

void AstDump::writeSymbol(Symbol* sym, bool def) {
  if (mNeedSpace)
    fputc(' ', mFP);

  mNeedSpace = false;

  if (def) {
    if (ArgSymbol* arg = toArgSymbol(sym)) {
      mNeedSpace = true;

      switch (arg->intent) {
        case INTENT_IN:        fprintf(mFP, "in");        break;
        case INTENT_INOUT:     fprintf(mFP, "inout");     break;
        case INTENT_OUT:       fprintf(mFP, "out");       break;
        case INTENT_CONST:     fprintf(mFP, "const");     break;
        case INTENT_CONST_IN:  fprintf(mFP, "const in");  break;
        case INTENT_CONST_REF: fprintf(mFP, "const ref"); break;
        case INTENT_REF:       fprintf(mFP, "ref");       break;
        case INTENT_PARAM:     fprintf(mFP, "param");     break;
        case INTENT_TYPE:      fprintf(mFP, "type");      break;
        case INTENT_BLANK:     mNeedSpace = false;        break;
      }

      write(true, "arg", true);
    }
  }

  write(true, sym->name, true);

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

AggregateType* AstDump::structuralTypeSymbol(Symbol *s) {
  if (TypeSymbol* ts = toTypeSymbol(s))
    if (AggregateType* st = toAggregateType(ts->type))
      return st;

  return NULL;
}

void AstDump::tempHack(BaseAST* ast, AstDump* handle) {
  handle->write(ast);
}
