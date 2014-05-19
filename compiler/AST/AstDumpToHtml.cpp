#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include "view.h"

#include "driver.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"

#include <inttypes.h>


static void dump_index_header(FILE* f) {
  fprintf(f, "<HTML>\n");
  fprintf(f, "<HEAD>\n");
  fprintf(f, "<TITLE> Compilation Dump </TITLE>\n");
  fprintf(f, "<SCRIPT SRC=\"http://chapel.cray.com/developer/mktree.js\" LANGUAGE=\"JavaScript\"></SCRIPT>");
  fprintf(f, "<LINK REL=\"stylesheet\" HREF=\"http://chapel.cray.com/developer/mktree.css\">");
  fprintf(f, "</HEAD>\n");
  fprintf(f, "<div style=\"text-align: center;\"><big><big><span style=\"font-weight: bold;\">");
  fprintf(f, "Compilation Dump<br><br></span></big></big>\n");
  fprintf(f, "<div style=\"text-align: left;\">\n\n");
}


static void dump_index_footer(FILE* f) {
  fprintf(f, "</HTML>\n");
}

void html_log_init() {
  if (!(html_index_file = fopen(astr(log_dir, "index.html"), "w"))) {
    USR_FATAL("cannot open html index file \"%s\" for writing", astr(log_dir, "index.html"));
  }
  
  dump_index_header(html_index_file);
  
  fprintf(html_index_file, "<TABLE CELLPADDING=\"0\" CELLSPACING=\"0\">");
}

void html_log_done() {
  fprintf(html_index_file, "</TABLE>");
  dump_index_footer(html_index_file);
  fclose(html_index_file);
}

static const char*
html_file_name( int pass, const char *module) {
  return astr( "pass", istr(pass), "_module_", astr(module, ".html"));
}

static const char*
html_file_name( int pass, Symbol* sym) {
  return html_file_name(pass, sym->defPoint->getModule()->name);
}

static bool
hasHref(Symbol* sym) {
  return sym->defPoint && sym->defPoint->parentSymbol &&
    sym->defPoint->getModule();
}

static AggregateType*
structuralTypeSymbol(Symbol *s) {
  if (TypeSymbol *ts = toTypeSymbol(s))
    if (AggregateType *st = toAggregateType(ts->type))
      return st;
  return NULL;
}

static void
html_adjacent_passes(FILE* html_file, int pass, Symbol* sym) {
  if (hasHref(sym)) {
    if (pass > 1)
      fprintf(html_file, "<A HREF=\"%s#SYM%d\">&laquo</A>",
              html_file_name(pass-1, sym), sym->id);
    if (true)
      fprintf(html_file, "<A HREF=\"%s#SYM%d\">&raquo</A>\n",
              html_file_name(pass+1, sym), sym->id);
  }
}

static void
printBlockID(FILE* html_file, Expr* expr) {
  if (fdump_html_print_block_IDs)
    fprintf(html_file, " %d", expr->id);
}

static void
html_print_symbol(FILE* html_file, int pass, Symbol* sym, bool def) {
  if (def) {
    fprintf(html_file, "<A NAME=\"SYM%d\">", sym->id);
  } else {
    if (sym->defPoint && sym->defPoint->parentSymbol && sym->defPoint->getModule()) {
      INT_ASSERT(hasHref(sym));
      fprintf(html_file, "<A HREF=\"%s#SYM%d\">",
              html_file_name( pass, sym->defPoint->getModule()->name),
              sym->id);
    } else {
      INT_ASSERT(!hasHref(sym));
      fprintf(html_file, "<A>");
    }
  }
  if (toFnSymbol(sym)) {
    fprintf(html_file, "<FONT COLOR=\"blue\">");
  } else if (toTypeSymbol(sym)) {
    fprintf(html_file, "<FONT COLOR=\"green\">");
  } else {
    fprintf(html_file, "<FONT COLOR=\"red\">");
  }
  fprintf(html_file, "%s", sym->name);
  fprintf(html_file, "</FONT>");
  fprintf(html_file, "<FONT COLOR=\"grey\">[%d]</FONT>", sym->id);
  fprintf(html_file, "</A>");
  if (def &&
      !toTypeSymbol(sym) &&
      sym->type &&
      sym->type->symbol &&
      sym->type != dtUnknown) {
    fprintf(html_file, ":");
    html_print_symbol( html_file, pass, sym->type->symbol, false);
  }
}

static void
html_print_fnsymbol( FILE* html_file, int pass, FnSymbol* fn) {
  if (fn->_this && fn->_this->defPoint) {
    html_print_symbol( html_file, pass, fn->_this->type->symbol, false);
    fprintf(html_file, " . ");
  }
  html_print_symbol( html_file, pass, fn, true);
  fprintf(html_file, " ( ");
  bool first = true;
  for_formals(formal, fn) {
    if (!first) {
      fprintf(html_file, " , ");
    } else {
      first = false;
    }
    html_print_symbol( html_file, pass, formal, true);
  }
  fprintf(html_file, " ) ");
  if (fn->retTag == RET_VAR)
    fprintf(html_file, "<b>var</b> ");
  else if (fn->retTag == RET_PARAM)
    fprintf(html_file, "<b>param</b> ");
  else if (fn->retTag == RET_TYPE)
    fprintf(html_file, "<b>type</b> ");
  if (fn->retType && fn->retType->symbol) {
    fprintf(html_file, " : ");
    html_print_symbol( html_file, pass, fn->retType->symbol, false);
  }
}

static void
html_view_ast(BaseAST* ast, FILE* html_file, int pass) {
  if (Expr* expr = toExpr(ast)) {
    if (isBlockStmt(expr)) {
      fprintf(html_file, "<DL>\n");
      if (FnSymbol* fn = toFnSymbol(expr->parentSymbol))
        if (expr == fn->where)
          fprintf(html_file, "<B>where</B>\n");
      fprintf(html_file, "{");
      printBlockID(html_file, expr);
    } else if (GotoStmt* s = toGotoStmt(expr)) {
      fprintf(html_file, "<DL>\n");
      switch (s->gotoTag) {
      case GOTO_NORMAL: fprintf(html_file, "<B>goto</B> "); break;
      case GOTO_BREAK: fprintf(html_file, "<B>break</B> "); break;
      case GOTO_CONTINUE: fprintf(html_file, "<B>continue</B> "); break;
      case GOTO_RETURN: fprintf(html_file, "<B>gotoReturn</B> "); break;
      case GOTO_GETITER_END: fprintf(html_file, "<B>gotoGetiterEnd</B> "); break;
      case GOTO_ITER_RESUME: fprintf(html_file, "<B>gotoIterResume</B> "); break;
      case GOTO_ITER_END: fprintf(html_file, "<B>gotoIterEnd</B> "); break;
      }
      if (SymExpr* label = toSymExpr(s->label))
        if (label->var != gNil)
          html_print_symbol(html_file, pass, label->var, true);
    } else if (toCondStmt(expr)) {
      fprintf(html_file, "<DL>\n");
      fprintf(html_file, "<B>if</B> ");
    } else {
      if (expr->getStmtExpr() && expr->getStmtExpr() == expr) {
        fprintf(html_file, "<DL>\n");
      }
      fprintf(html_file, " ");
      if (DefExpr* e = toDefExpr(expr)) {
        if (FnSymbol* fn = toFnSymbol(e->sym)) {
          fprintf(html_file, "<UL CLASS =\"mktree\">\n<LI>");
          html_adjacent_passes(html_file, pass, fn);
          fprintf(html_file, "<CHPLTAG=\"FN%d\">\n", fn->id);
          fprintf(html_file, "<B>function ");
          html_print_fnsymbol( html_file, pass, fn);
          fprintf(html_file, "</B><UL>\n");
        } else if (structuralTypeSymbol(e->sym)) {
          fprintf(html_file, "<UL CLASS =\"mktree\">\n");
          fprintf(html_file, "<LI>");
          if (DefExpr *def = toDefExpr( ast)) {
            if (def->sym->hasFlag(FLAG_SYNC))
              fprintf( html_file, "<B>sync</B> ");
            if (def->sym->hasFlag(FLAG_SINGLE))
              fprintf( html_file, "<B>single</B> ");
          }
          fprintf(html_file, "<B>type ");
          html_print_symbol( html_file, pass, e->sym, true);
          fprintf(html_file, "</B><UL>\n");
        } else if (toTypeSymbol(e->sym)) {
          fprintf(html_file, "<B>type </B> ");
          html_print_symbol( html_file, pass, e->sym, true);
        } else if (VarSymbol* vs=toVarSymbol(e->sym)) {
          if (vs->type->symbol->hasFlag(FLAG_SYNC))
            fprintf( html_file, "<B>sync </B>");
          if (vs->type->symbol->hasFlag(FLAG_SINGLE))
            fprintf( html_file, "<B>single </B>");
          fprintf(html_file, "<B>var </B> ");
          html_print_symbol( html_file, pass, e->sym, true);
        } else if (ArgSymbol* s = toArgSymbol(e->sym)) {
          switch (s->intent) {
          case INTENT_IN: fprintf(html_file, "<B>in</B> "); break;
          case INTENT_INOUT: fprintf(html_file, "<B>inout</B> "); break;
          case INTENT_OUT: fprintf(html_file, "<B>out</B> "); break;
          case INTENT_CONST: fprintf(html_file, "<B>const</B> "); break;
          case INTENT_CONST_IN: fprintf(html_file, "<B>const in</B> "); break;
          case INTENT_CONST_REF: fprintf(html_file, "<B>const ref</B> "); break;
          case INTENT_REF: fprintf(html_file, "<B>ref</B> "); break;
          case INTENT_PARAM: fprintf(html_file, "<B>param</B> "); break;
          case INTENT_TYPE: fprintf(html_file, "<B>type</B> "); break;
          default: break;
          }
          fprintf(html_file, "<B>arg</B> ");
          html_print_symbol( html_file, pass, e->sym, true);
        } else if (toLabelSymbol(e->sym)) {
          fprintf(html_file, "<B>label</B> ");
          html_print_symbol( html_file, pass, e->sym, true);
        } else if (toModuleSymbol(e->sym)) {
          fprintf(html_file, "</DL>\n");
          // Don't process nested modules -- they'll be handled at the
          // top-level
          return;
        } else {
          fprintf(html_file, "<B>def</B> ");
          html_print_symbol( html_file, pass, e->sym, true);
        }
      } else if (VarSymbol* e = get_constant(expr)) {
        if (e->immediate) {
          const size_t bufSize = 128;
          char imm[bufSize];
          snprint_imm(imm, bufSize, *e->immediate);
          fprintf(html_file, "<i><FONT COLOR=\"blue\">%s%s</FONT></i>", imm, is_imag_type(e->type) ? "i" : "");
        } else {
          fprintf(html_file, "<i><FONT COLOR=\"blue\">%s</FONT></i>", e->name);
        }
      } else if (SymExpr* e = toSymExpr(expr)) {
        html_print_symbol(html_file, pass, e->var, false);
      } else if (UnresolvedSymExpr* e = toUnresolvedSymExpr(expr)) {
        fprintf(html_file, "<FONT COLOR=\"red\">%s</FONT>", e->unresolved);
      } else if (NamedExpr* e = toNamedExpr(expr)) {
        fprintf(html_file, "(%s = ", e->name);
      } else if (CallExpr* e = toCallExpr(expr)) {
        if (e->isResolved()) {
          if (e->isResolved()->hasFlag(FLAG_BEGIN_BLOCK))
            fprintf(html_file, "begin ");
          else if (e->isResolved()->hasFlag(FLAG_ON_BLOCK))
            fprintf(html_file, "on ");
        }
        fprintf(html_file, "(%d ", e->id);
        if (!e->primitive) {
          fprintf(html_file, "<B>call</B> ");
        } else {
          if (e->isPrimitive(PRIM_RETURN))
            fprintf(html_file, "<B>return</B> ");
          else if (e->isPrimitive(PRIM_YIELD))
            fprintf(html_file, "<B>yield</B> ");
          else
            fprintf(html_file, "'%s' ", e->primitive->name);
        }
        if (e->partialTag)
          fprintf(html_file, "(partial) ");
      } else {
        fprintf(html_file, "(%s", expr->astTagAsString());
      }
    }
  }

  AST_CHILDREN_CALL(ast, html_view_ast, html_file, pass);

  if (Expr* expr = toExpr(ast)) {
    if (DefExpr* e = toDefExpr(expr)) {
      if (toFnSymbol(e->sym) || 
          (toTypeSymbol(e->sym) &&
           toAggregateType(e->sym->type))) {
        fprintf(html_file, "</UL>\n");
        if (FnSymbol* fn = toFnSymbol(e->sym)) {
          fprintf(html_file, "<CHPLTAG=\"FN%d\">\n", fn->id);
        }
        fprintf(html_file, "</UL>\n");
      }
    } else if (toSymExpr(expr)) {
    } else if (toCallExpr(expr) ||
               toNamedExpr(expr)) {
      fprintf(html_file, ")");
    }

    if (toBlockStmt(expr) ||
        toCondStmt(expr) ||
        toGotoStmt(expr) ||
        (expr->getStmtExpr() && expr->getStmtExpr() == expr)) {
      if (toBlockStmt(expr)) {
        fprintf(html_file, "}");
        printBlockID(html_file, expr);
      }
      fprintf(html_file, "</DL>\n");
    }

  }
}

void html_view(const char* passName) {
  static int uid = 1;
  INT_ASSERT(uid == currentPassNo); //we could use latter for former
  INT_ASSERT(passName == currentPassName);
  FILE* html_file;
  const char* filename;

  fprintf(html_index_file, "<TR><TD>");
  fprintf(html_index_file, "%s%s[%d]", passName,
          fdump_html_include_system_modules ? "<br>" : " ", lastNodeIDUsed());
  fprintf(html_index_file, "</TD><TD>");
  forv_Vec(ModuleSymbol, mod, allModules) {
   if (fdump_html_include_system_modules || mod->modTag == MOD_MAIN || mod->modTag == MOD_USER) {
    filename = html_file_name( uid, mod->name);
    fprintf(html_index_file, "&nbsp;&nbsp;<a href=\"%s\">%s</a>\n", filename, mod->name);
    html_file = fopen(astr(log_dir, filename), "w");
    fprintf(html_file, "<CHPLTAG=\"%s\">\n", passName);
    fprintf(html_file, "<HTML>\n");
    fprintf(html_file, "<HEAD>\n");
    fprintf(html_file, "<TITLE> AST for Module %s after Pass %s </TITLE>\n", mod->name, passName);
    fprintf(html_file, "<SCRIPT SRC=\"http://chapel.cray.com/developer/mktree.js\" LANGUAGE=\"JavaScript\"></SCRIPT>\n");
    fprintf(html_file, "<LINK REL=\"stylesheet\" HREF=\"http://chapel.cray.com/developer/mktree.css\">\n");
    fprintf(html_file, "</HEAD><BODY%s>\n",
            fdump_html_wrap_lines ? "" : " style=\"white-space: nowrap;\"");
    if (currentPassNo > 1)
      fprintf(html_file, "<A HREF=%s>previous pass</A> &nbsp;\n", html_file_name(currentPassNo-1, mod->name));
    if (true)
      fprintf(html_file, "<A HREF=%s>next pass</A>\n",            html_file_name(currentPassNo+1, mod->name));
    fprintf(html_file, "<div style=\"text-align: center;\"><big><big><span style=\"font-weight: bold;\">");
    fprintf(html_file, "AST for Module %s after Pass %s <br><br></span></big></big>\n", mod->name, passName);
    fprintf(html_file, "<div style=\"text-align: left;\">\n\n");
    fprintf(html_file, "<B>module \n");
    html_print_symbol(html_file, uid, mod, true);
    fprintf(html_file, "</B>\n");
    for_alist(stmt, mod->block->body)
      html_view_ast(stmt, html_file, uid);
    fprintf(html_file, "</BODY></HTML>\n");
    fclose(html_file);
   }
  }
  fprintf(html_index_file, "</TD></TR>");
  fflush(html_index_file);
  uid++;
}
