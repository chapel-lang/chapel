#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include "AstDumpToHtml.h"

#include "view.h"

#include "driver.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"

#include <cstdio>
#include <inttypes.h>

int            AstDumpToHtml::sPassIndex   = 1;



AstDumpToHtml::AstDumpToHtml() {
  mPassNum = 0;
  mName    = 0;
  mPath    = 0;
  mFP      = 0;
}

AstDumpToHtml::~AstDumpToHtml() {
  close();
}

void AstDumpToHtml::init() {
  if (!(html_index_file = fopen(astr(log_dir, "index.html"), "w"))) {
    USR_FATAL("cannot open html index file \"%s\" for writing", astr(log_dir, "index.html"));
  }
  
  fprintf(html_index_file, "<HTML>\n");
  fprintf(html_index_file, "<HEAD>\n");
  fprintf(html_index_file, "<TITLE> Compilation Dump </TITLE>\n");
  fprintf(html_index_file, "<SCRIPT SRC=\"http://chapel.cray.com/developer/mktree.js\" LANGUAGE=\"JavaScript\"></SCRIPT>");
  fprintf(html_index_file, "<LINK REL=\"stylesheet\" HREF=\"http://chapel.cray.com/developer/mktree.css\">");
  fprintf(html_index_file, "</HEAD>\n");
  fprintf(html_index_file, "<div style=\"text-align: center;\"><big><big><span style=\"font-weight: bold;\">");
  fprintf(html_index_file, "Compilation Dump<br><br></span></big></big>\n");
  fprintf(html_index_file, "<div style=\"text-align: left;\">\n\n");
  
  fprintf(html_index_file, "<TABLE CELLPADDING=\"0\" CELLSPACING=\"0\">");
}

void AstDumpToHtml::done() {
  fprintf(html_index_file, "</TABLE>");
  fprintf(html_index_file, "</HTML>\n");

  fclose(html_index_file);
}

void AstDumpToHtml::view(const char* passName) {
  INT_ASSERT(sPassIndex  == currentPassNo);
  INT_ASSERT(passName    == currentPassName);

  fprintf(html_index_file, "<TR><TD>");
  fprintf(html_index_file,
          "%s%s[%d]", passName,
          fdump_html_include_system_modules ? "<br>" : " ", lastNodeIDUsed());
  fprintf(html_index_file, "</TD><TD>");

  forv_Vec(ModuleSymbol, module, allModules) {
    if (fdump_html_include_system_modules == true      ||
        module->modTag                    == MOD_MAIN  ||
        module->modTag                    == MOD_USER) {
      AstDumpToHtml logger;

      if (logger.open(module, passName, sPassIndex) == true) {
        for_alist(stmt, module->block->body)
          logger.write(stmt);

        logger.close();
      }
    }
  }

  fprintf(html_index_file, "</TD></TR>");
  fflush(html_index_file);

  sPassIndex++;
}

bool AstDumpToHtml::open(ModuleSymbol* module, const char* passName, int passNum) {
  mName    = html_file_name(passNum, module->name);
  mPath    = astr(log_dir, mName);
  mFP      = fopen(mPath, "w");
  mPassNum = passNum;

  if (mFP != 0) {
    fprintf(html_index_file, "&nbsp;&nbsp;<a href=\"%s\">%s</a>\n", mName, module->name);

    fprintf(mFP, "<CHPLTAG=\"%s\">\n", passName);
    fprintf(mFP, "<HTML>\n");
    fprintf(mFP, "<HEAD>\n");
    fprintf(mFP, "<TITLE> AST for Module %s after Pass %s </TITLE>\n", module->name, passName);
    fprintf(mFP, "<SCRIPT SRC=\"http://chapel.cray.com/developer/mktree.js\" LANGUAGE=\"JavaScript\"></SCRIPT>\n");
    fprintf(mFP, "<LINK REL=\"stylesheet\" HREF=\"http://chapel.cray.com/developer/mktree.css\">\n");
    fprintf(mFP, "</HEAD><BODY%s>\n",
           fdump_html_wrap_lines ? "" : " style=\"white-space: nowrap;\"");

    if (currentPassNo > 1)
      fprintf(mFP, "<A HREF=%s>previous pass</A> &nbsp;\n", html_file_name(currentPassNo - 1, module->name));

    if (true)
      fprintf(mFP, "<A HREF=%s>next pass</A>\n",            html_file_name(currentPassNo + 1, module->name));

    fprintf(mFP, "<div style=\"text-align: center;\"><big><big><span style=\"font-weight: bold;\">");
    fprintf(mFP, "AST for Module %s after Pass %s <br><br></span></big></big>\n", module->name, passName);
    fprintf(mFP, "<div style=\"text-align: left;\">\n\n");
    fprintf(mFP, "<B>module \n");

    writeSymbol(module, true);

    fprintf(mFP, "</B>\n");
  }

  return (mFP != 0) ? true : false;
}

bool AstDumpToHtml::close() {
  bool retval = false;

  if (mFP != 0) {
    fprintf(mFP, "</BODY></HTML>\n");

    retval = (fclose(mFP) == 0) ? true : false;
    mFP    = 0;
  }
  

  return retval;
}


void AstDumpToHtml::write(BaseAST* ast) {
  if (Expr* expr = toExpr(ast)) {
    if (isBlockStmt(expr)) {
      fprintf(mFP, "<DL>\n");

      if (FnSymbol* fn = toFnSymbol(expr->parentSymbol))
        if (expr == fn->where)
          fprintf(mFP, "<B>where</B>\n");

      fprintf(mFP, "{");

      printBlockID(expr);

    } else if (GotoStmt* s = toGotoStmt(expr)) {
      fprintf(mFP, "<DL>\n");

      switch (s->gotoTag) {
        case GOTO_NORMAL: 
          fprintf(mFP, "<B>goto</B> ");
          break;

        case GOTO_BREAK: 
          fprintf(mFP, "<B>break</B> ");
          break;

        case GOTO_CONTINUE: 
          fprintf(mFP, "<B>continue</B> ");
          break;

        case GOTO_RETURN: 
          fprintf(mFP, "<B>gotoReturn</B> ");
          break;

        case GOTO_GETITER_END: 
          fprintf(mFP, "<B>gotoGetiterEnd</B> ");
          break;

        case GOTO_ITER_RESUME: 
          fprintf(mFP, "<B>gotoIterResume</B> ");
          break;

        case GOTO_ITER_END: 
          fprintf(mFP, "<B>gotoIterEnd</B> ");
          break;
      }

      if (SymExpr* label = toSymExpr(s->label))
        if (label->var != gNil)
          writeSymbol(label->var, true);

    } else if (toCondStmt(expr)) {
      fprintf(mFP, "<DL>\n");
      fprintf(mFP, "<B>if</B> ");

    } else {
      if (expr->getStmtExpr() && expr->getStmtExpr() == expr) {
        fprintf(mFP, "<DL>\n");
      }

      fprintf(mFP, " ");

      if (DefExpr* e = toDefExpr(expr)) {
        if (FnSymbol* fn = toFnSymbol(e->sym)) {
          fprintf(mFP, "<UL CLASS =\"mktree\">\n<LI>");

          adjacent_passes(fn);

          fprintf(mFP, "<CHPLTAG=\"FN%d\">\n", fn->id);
          fprintf(mFP, "<B>function ");

          writeFnSymbol(fn);

          fprintf(mFP, "</B><UL>\n");

        } else if (structuralTypeSymbol(e->sym)) {
          fprintf(mFP, "<UL CLASS =\"mktree\">\n");
          fprintf(mFP, "<LI>");

          if (DefExpr *def = toDefExpr( ast)) {
            if (def->sym->hasFlag(FLAG_SYNC))
              fprintf(mFP, "<B>sync</B> ");

            if (def->sym->hasFlag(FLAG_SINGLE))
              fprintf(mFP, "<B>single</B> ");
          }

          fprintf(mFP, "<B>type ");

          writeSymbol(e->sym, true);

          fprintf(mFP, "</B><UL>\n");

        } else if (toTypeSymbol(e->sym)) {
          fprintf(mFP, "<B>type </B> ");
          writeSymbol(e->sym, true);

        } else if (VarSymbol* vs=toVarSymbol(e->sym)) {
          if (vs->type->symbol->hasFlag(FLAG_SYNC))
            fprintf(mFP, "<B>sync </B>");

          if (vs->type->symbol->hasFlag(FLAG_SINGLE))
            fprintf(mFP, "<B>single </B>");

          fprintf(mFP, "<B>var </B> ");
          writeSymbol(e->sym, true);

        } else if (ArgSymbol* s = toArgSymbol(e->sym)) {
          switch (s->intent) {
           case INTENT_IN: 
             fprintf(mFP, "<B>in</B> ");
             break;

           case INTENT_INOUT: 
             fprintf(mFP, "<B>inout</B> ");
             break;

           case INTENT_OUT: 
             fprintf(mFP, "<B>out</B> ");
             break;

           case INTENT_CONST: 
             fprintf(mFP, "<B>const</B> ");
             break;

           case INTENT_CONST_IN: 
             fprintf(mFP, "<B>const in</B> ");
             break;

           case INTENT_CONST_REF: 
             fprintf(mFP, "<B>const ref</B> ");
             break;

           case INTENT_REF: 
             fprintf(mFP, "<B>ref</B> ");
             break;

           case INTENT_PARAM: 
             fprintf(mFP, "<B>param</B> ");
             break;

           case INTENT_TYPE: 
             fprintf(mFP, "<B>type</B> ");
             break;
             
            case INTENT_BLANK:
              break;
          }

          fprintf(mFP, "<B>arg</B> ");

          writeSymbol(e->sym, true);

        } else if (toLabelSymbol(e->sym)) {
          fprintf(mFP, "<B>label</B> ");
          writeSymbol(e->sym, true);

        } else if (toModuleSymbol(e->sym)) {
          fprintf(mFP, "</DL>\n");
          // Don't process nested modules -- they'll be handled at the
          // top-level
          return;

        } else {
          fprintf(mFP, "<B>def</B> ");
          writeSymbol(e->sym, true);
        }

      } else if (VarSymbol* e = get_constant(expr)) {
        if (e->immediate) {
          const size_t bufSize = 128;
          char         imm[bufSize];

          snprint_imm(imm, bufSize, *e->immediate);

          fprintf(mFP, "<i><FONT COLOR=\"blue\">%s%s</FONT></i>", imm, is_imag_type(e->type) ? "i" : "");

        } else {
          fprintf(mFP, "<i><FONT COLOR=\"blue\">%s</FONT></i>", e->name);
        }

      } else if (SymExpr* e = toSymExpr(expr)) {
        writeSymbol(e->var, false);

      } else if (UnresolvedSymExpr* e = toUnresolvedSymExpr(expr)) {
        fprintf(mFP, "<FONT COLOR=\"red\">%s</FONT>", e->unresolved);

      } else if (NamedExpr* e = toNamedExpr(expr)) {
        fprintf(mFP, "(%s = ", e->name);

      } else if (CallExpr* e = toCallExpr(expr)) {
        if (e->isResolved()) {
          if (e->isResolved()->hasFlag(FLAG_BEGIN_BLOCK))
            fprintf(mFP, "begin ");
          else if (e->isResolved()->hasFlag(FLAG_ON_BLOCK))
            fprintf(mFP, "on ");
        }

        fprintf(mFP, "(%d ", e->id);

        if (!e->primitive) {
          fprintf(mFP, "<B>call</B> ");
        } else if (e->isPrimitive(PRIM_RETURN))
          fprintf(mFP, "<B>return</B> ");

        else if (e->isPrimitive(PRIM_YIELD))
          fprintf(mFP, "<B>yield</B> ");

        else
          fprintf(mFP, "'%s' ", e->primitive->name);

        if (e->partialTag)
          fprintf(mFP, "(partial) ");
      } else {
        fprintf(mFP, "(%s", expr->astTagAsString());
      }
    }
  }

  // This is a hook in to AST_CHILDREN_CALL macro until the Visitorc can be dropped in
  AST_CHILDREN_CALL(ast, tempHack, this);

#if 0
  AST_CHILDREN_CALL(ast, html_view_ast, html_file, pass);
#endif

  if (Expr* expr = toExpr(ast)) {
    if (DefExpr* e = toDefExpr(expr)) {
      if (toFnSymbol(e->sym) || 
          (toTypeSymbol(e->sym) &&
           toAggregateType(e->sym->type))) {

        fprintf(mFP, "</UL>\n");

        if (FnSymbol* fn = toFnSymbol(e->sym)) {
          fprintf(mFP, "<CHPLTAG=\"FN%d\">\n", fn->id);
        }

        fprintf(mFP, "</UL>\n");
      }

    } else if (toSymExpr(expr)) {

    } else if (toCallExpr(expr) || toNamedExpr(expr)) {
      fprintf(mFP, ")");

    }

    if (toBlockStmt(expr) ||
        toCondStmt(expr) ||
        toGotoStmt(expr) ||
        (expr->getStmtExpr() && expr->getStmtExpr() == expr)) {
      if (toBlockStmt(expr)) {
        fprintf(mFP, "}");
        printBlockID(expr);
      }
      fprintf(mFP, "</DL>\n");
    }
  }
}

void AstDumpToHtml::writeFnSymbol(FnSymbol* fn) {
  bool first = true;

  if (fn->_this && fn->_this->defPoint) {
    writeSymbol(fn->_this->type->symbol, false);
    fprintf(mFP, " . ");
  }

  writeSymbol(fn, true);

  fprintf(mFP, " ( ");

  for_formals(formal, fn) {
    if (!first) {
      fprintf(mFP, " , ");
    } else {
      first = false;
    }

    writeSymbol(formal, true);
  }

  fprintf(mFP, " ) ");

  if (fn->retTag == RET_VAR)
    fprintf(mFP, "<b>var</b> ");

  else if (fn->retTag == RET_PARAM)
    fprintf(mFP, "<b>param</b> ");

  else if (fn->retTag == RET_TYPE)
    fprintf(mFP, "<b>type</b> ");

  if (fn->retType && fn->retType->symbol) {
    fprintf(mFP, " : ");
    writeSymbol(fn->retType->symbol, false);
  }
}

void AstDumpToHtml::writeSymbol(Symbol* sym, bool def) {
  if (def) {
    fprintf(mFP, "<A NAME=\"SYM%d\">", sym->id);
  } else {
    if (sym->defPoint && sym->defPoint->parentSymbol && sym->defPoint->getModule()) {
      INT_ASSERT(hasHref(sym));

      fprintf(mFP, "<A HREF=\"%s#SYM%d\">",
              html_file_name(mPassNum, sym->defPoint->getModule()->name),
              sym->id);
    } else {
      INT_ASSERT(!hasHref(sym));

      fprintf(mFP, "<A>");
    }
  }

  if (toFnSymbol(sym)) {
    fprintf(mFP, "<FONT COLOR=\"blue\">");

  } else if (toTypeSymbol(sym)) {
    fprintf(mFP, "<FONT COLOR=\"green\">");

  } else {
    fprintf(mFP, "<FONT COLOR=\"red\">");
  }

  fprintf(mFP, "%s", sym->name);
  fprintf(mFP, "</FONT>");
  fprintf(mFP, "<FONT COLOR=\"grey\">[%d]</FONT>", sym->id);
  fprintf(mFP, "</A>");

  if (def &&
      !toTypeSymbol(sym) &&
      sym->type &&
      sym->type->symbol &&
      sym->type != dtUnknown) {
    fprintf(mFP, ":");
    writeSymbol(sym->type->symbol, false);
  }
}

void AstDumpToHtml::adjacent_passes(Symbol* sym) {
  if (hasHref(sym)) {
    if (mPassNum > 1)
      fprintf(mFP, "<A HREF=\"%s#SYM%d\">&laquo</A>",   html_file_name(mPassNum - 1, sym), sym->id);

    if (true)
      fprintf(mFP, "<A HREF=\"%s#SYM%d\">&raquo</A>\n", html_file_name(mPassNum + 1, sym), sym->id);
  }
}

void AstDumpToHtml::printBlockID(Expr* expr) {
  if (fdump_html_print_block_IDs)
    fprintf(mFP, " %d", expr->id);
}

const char* AstDumpToHtml::html_file_name(int passNum, Symbol* sym) {
  return html_file_name(passNum, sym->defPoint->getModule()->name);
}

const char* AstDumpToHtml::html_file_name(int passNum, const char* module) {
  return astr("pass", istr(passNum), "_module_", astr(module, ".html"));
}

AggregateType* AstDumpToHtml::structuralTypeSymbol(Symbol* s) {
  if (TypeSymbol* ts = toTypeSymbol(s))
    if (AggregateType* st = toAggregateType(ts->type))
      return st;

  return NULL;
}

bool AstDumpToHtml::hasHref(Symbol* sym) {
  return sym->defPoint               && 
         sym->defPoint->parentSymbol &&
         sym->defPoint->getModule();
}


void AstDumpToHtml::tempHack(BaseAST* ast, AstDumpToHtml* handle) {
  handle->write(ast);
}
