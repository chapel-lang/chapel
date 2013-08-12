#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include <cstdlib>
#include <inttypes.h>
#include "view.h"
#include "astutil.h"
#include "stmt.h"
#include "expr.h"
#include "symbol.h"
#include "type.h"
#include "stringutil.h"
#include "log.h"
#include "driver.h" // For log_dir

static void
list_sym(Symbol* sym, bool type = true) {
  if (VarSymbol* var = toVarSymbol(sym)) {
    if (var->immediate) {
      if (var->immediate->const_kind == NUM_KIND_INT) {
        printf("%"PRId64" ", var->immediate->int_value());
        return;
      } else if (var->immediate->const_kind == CONST_KIND_STRING) {
        printf("\"%s\" ", var->immediate->v_string);
        return;
      }
    }
  }
  if (toFnSymbol(sym)) {
    printf("fn ");
  } else if (toArgSymbol(sym)) {
    printf("arg ");
  } else if (toTypeSymbol(sym)) {
    printf("type ");
  }
  printf("%s", sym->name);
  printf("[%d]", sym->id);
  if (!type) {
    printf(" ");
  } else if (FnSymbol* fn = toFnSymbol(sym)) {
    printf(":%s", fn->retType->symbol->name);
    printf("[%d] ", fn->retType->symbol->id);
  } else if (sym->type && sym->type->symbol) {
    printf(":%s", sym->type->symbol->name);
    printf("[%d] ", sym->type->symbol->id);
  } else {
    printf(" ");
  }
}


static bool
list_line(Expr* expr) {
  if (IS_STMT(expr))
    return true;
  if (CondStmt* cond = toCondStmt(expr->parentExpr)) {
    if (cond->condExpr == expr)
      return false;
  }
  if (!expr->parentExpr || IS_STMT(expr->parentExpr))
    return true;
  return false;
}


static void
list_ast(BaseAST* ast, int indent = 0) {
  if (Expr* expr = toExpr(ast)) {
    if (list_line(expr)) {
      printf("%-7d ", expr->id);
      for (int i = 0; i < indent; i++)
        printf(" ");
    }
    if (GotoStmt* e = toGotoStmt(ast)) {
      printf("goto ");
      if (SymExpr* label = toSymExpr(e->label)) {
        if (label->var != gNil) {
          list_ast(e->label, indent+1);
        }
      } else {
        list_ast(e->label, indent+1);
      }
    } else if (toBlockStmt(ast)) {
      printf("{\n");
    } else if (toCondStmt(ast)) {
      printf("if ");
    } else if (CallExpr* e = toCallExpr(expr)) {
      if (e->primitive)
        printf("%s( ", e->primitive->name);
      else
        printf("call( ");
    } else if (NamedExpr* e = toNamedExpr(expr)) {
      printf("%s = ", e->name);
    } else if (toDefExpr(expr)) {
      printf("def ");
    } else if (SymExpr* e = toSymExpr(expr)) {
      list_sym(e->var, false);
    } else if (UnresolvedSymExpr* e = toUnresolvedSymExpr(expr)) {
      printf("%s ", e->unresolved);
    }
  }

  if (Symbol* sym = toSymbol(ast))
    list_sym(sym);
  if (toFnSymbol(ast) || toModuleSymbol(ast)) {
    printf("\n");
  }

  int new_indent = indent;

  if (Expr* expr = toExpr(ast))
    if (list_line(expr))
      new_indent = indent+2;

  AST_CHILDREN_CALL(ast, list_ast, new_indent);

  if (Expr* expr = toExpr(ast)) {
    if (toCallExpr(expr)) {
      printf(") ");
    }
    if (toBlockStmt(ast)) {
      printf("%-7d ", expr->id);
      for (int i = 0; i < indent; i++)
        printf(" ");
      printf("}\n");
    } else if (CondStmt* cond = toCondStmt(expr->parentExpr)) {
      if (cond->condExpr == expr)
        printf("\n");
    } else if (!toCondStmt(expr) && list_line(expr)) {
      DefExpr* def = toDefExpr(expr);
      if (!(def && (toFnSymbol(def->sym) ||
                    toModuleSymbol(def->sym))))
        printf("\n");
    }
  }
}


static const char*
html_file_name( int pass, const char *module) {
  return astr( "pass", istr(pass), "_module_", astr(module, ".html"));
}


static void view_sym(Symbol* sym, bool number, int mark) {
  putchar('\'');
  if (sym->id == mark)
    printf("***");
  if (toFnSymbol(sym)) {
    printf("fn ");
  } else if (toArgSymbol(sym)) {
    printf("arg ");
  } else if (toTypeSymbol(sym)) {
    printf("type ");
  }
  printf("%s", sym->name);
  if (number)
    printf("[%d]", sym->id);
  if (FnSymbol* fn = toFnSymbol(sym)) {
    printf(":%s", fn->retType->symbol->name);
    if (number)
      printf("[%d]", fn->retType->symbol->id);
  } else if (sym->type && sym->type->symbol) {
    printf(":%s", sym->type->symbol->name);
    if (number)
      printf("[%d]", sym->type->symbol->id);
  }
  if (sym->hasFlag(FLAG_GENERIC))
    putchar('?');
  putchar('\'');
}


static void
view_ast(BaseAST* ast, bool number = false, int mark = -1, int indent = 0) {
  if (!ast)
    return;
  if (Expr* expr = toExpr(ast)) {
    printf("\n");
    for (int i = 0; i < indent; i++)
      printf(" ");
    printf("(");
    if (ast->id == mark)
      printf("***");
    if (number)
      printf("%d ", ast->id);
    printf("%s", astTagName[expr->astTag]);

    if (isBlockStmt(expr))
      if (FnSymbol* fn = toFnSymbol(expr->parentSymbol))
        if (expr == fn->where)
          printf(" where");

    if (GotoStmt *gs= toGotoStmt(ast)) {
      printf( " ");
      view_ast(gs->label, number, mark, indent+1);
    }

    if (CallExpr* call = toCallExpr(expr))
      if (call->primitive)
        printf(" %s", call->primitive->name);

    if (NamedExpr* named = toNamedExpr(expr))
      printf(" \"%s\"", named->name);

    if (toDefExpr(expr))
      printf(" ");

    int64_t i;
    const char *str;
    if (get_int(expr, &i)) {
      printf(" %" PRId64, i);
    } else if (get_string(expr, &str)) {
      printf(" \"%s\"", str);
    }

    if (SymExpr* sym = toSymExpr(expr)) {
      printf(" ");
      view_sym(sym->var, number, mark);
    } else if (UnresolvedSymExpr* sym = toUnresolvedSymExpr(expr)) {
      printf(" '%s'", sym->unresolved);
    }
  }

  if (Symbol* sym = toSymbol(ast)) {
    view_sym(sym, number, mark);
  }

  AST_CHILDREN_CALL(ast, view_ast, number, mark, indent+2);

  if (toExpr(ast))
    printf(")");
}

// complement view_ast(), which does not do justice to types
static void type_nprint_view(BaseAST* ast) {
  if (Type* type = toType(ast))
    if (Symbol* ts = type->symbol)
      printf(":%s[%d]\n", ts->name, type->id);
}
static void type_print_view(BaseAST* ast) {
  if (Type* type = toType(ast))
    if (Symbol* ts = type->symbol) {
      printf(":%s", ts->name);
      if (type->symbol->hasFlag(FLAG_GENERIC))
        putchar('?');
      putchar('\n');
    }
}

void list_view(BaseAST* ast) {
  if (ast==NULL) {
    printf("<NULL>");
  } else {
    if (toSymbol(ast))
      printf("%-7d ", ast->id);
    list_ast(ast);
  }
  printf("\n");
  fflush(stdout);
}

void list_view_noline(BaseAST* ast) {
  if (ast==NULL) {
    printf("<NULL>");
  } else {
    if (toSymbol(ast))
      printf("%-7d ", ast->id);
    list_ast(ast);
  }
  fflush(stdout);
}

void print_view(BaseAST* ast) {
  if (ast==NULL) {
    printf("<NULL>");
  } else {
    type_print_view(ast);
    view_ast(ast);
  }
  printf("\n\n");
  fflush(stdout);
}

void print_view_noline(BaseAST* ast) {
  if (ast==NULL) {
    printf("<NULL>");
  } else {
    type_print_view(ast);
    view_ast(ast);
  }
  fflush(stdout);
}

void nprint_view(BaseAST* ast) {
  if (ast==NULL) {
    printf("<NULL>");
  } else {
    type_nprint_view(ast);
    view_ast(ast, true);
  }
  printf("\n\n");
  fflush(stdout);
}

void nprint_view_noline(BaseAST* ast) {
  if (ast==NULL) {
    printf("<NULL>");
  } else {
    type_nprint_view(ast);
    view_ast(ast, true);
  }
  fflush(stdout);
}


BaseAST* aid(int id) {
  #define match_id(type)                        \
    forv_Vec(type, a, g##type##s) {             \
      if (a->id == id)                          \
        return a;                               \
    }
  foreach_ast(match_id);
  return NULL;
}


void iprint_view(int id) {
  nprint_view(aid(id));
}


void mark_view(BaseAST* ast, int id) {
  view_ast(ast, false, id);
  printf("\n\n");
  fflush(stdout);
}

static ClassType *
structuralTypeSymbol(Symbol *s) {
  if (TypeSymbol *ts = toTypeSymbol(s))
    if (ClassType *st = toClassType(ts->type))
      return st;
  return NULL;
}

static void
html_print_symbol(FILE* html_file, int pass, Symbol* sym, bool def) {
  if (def) {
    fprintf(html_file, "<A NAME=\"SYM%d\">", sym->id);
  } else {
    if (sym->defPoint && sym->defPoint->parentSymbol && sym->defPoint->getModule()) {
      fprintf(html_file, "<A HREF=\"%s#SYM%d\">",
              html_file_name( pass, sym->defPoint->getModule()->name),
              sym->id);
    } else {
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
        fprintf(html_file, "(%s", astTagName[expr->astTag]);
      }
    }
  }

  AST_CHILDREN_CALL(ast, html_view_ast, html_file, pass);

  if (Expr* expr = toExpr(ast)) {
    if (DefExpr* e = toDefExpr(expr)) {
      if (toFnSymbol(e->sym) || 
          (toTypeSymbol(e->sym) &&
           toClassType(e->sym->type))) {
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
      if (toBlockStmt(expr))
        fprintf(html_file, "}");
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
          fdump_html_incude_system_modules ? "<br>" : " ", lastNodeIDUsed());
  fprintf(html_index_file, "</TD><TD>");
  forv_Vec(ModuleSymbol, mod, allModules) {
   if (fdump_html_incude_system_modules || mod->modTag == MOD_MAIN || mod->modTag == MOD_USER) {
    filename = html_file_name( uid, mod->name);
    fprintf(html_index_file, "&nbsp;&nbsp;<a href=\"%s\">%s</a>\n", filename, mod->name);
    html_file = fopen(astr(log_dir, filename), "w");
    fprintf(html_file, "<CHPLTAG=\"%s\">\n", passName);
    fprintf(html_file, "<HTML>\n");
    fprintf(html_file, "<HEAD>\n");
    fprintf(html_file, "<TITLE> AST for Module %s after Pass %s </TITLE>\n", mod->name, passName);
    fprintf(html_file, "<SCRIPT SRC=\"%s/compiler/etc/www/mktree.js\" LANGUAGE=\"JavaScript\"></SCRIPT>", CHPL_HOME);
    fprintf(html_file, "<LINK REL=\"stylesheet\" HREF=\"%s/compiler/etc/www/mktree.css\">", CHPL_HOME);
    fprintf(html_file, "</HEAD>\n");
    fprintf(html_file, "<div style=\"text-align: center;\"><big><big><span style=\"font-weight: bold;\">");
    fprintf(html_file, "AST for Module %s after Pass %s <br><br></span></big></big>\n", mod->name, passName);
    fprintf(html_file, "<div style=\"text-align: left;\">\n\n");
    fprintf(html_file, "<B>module \n");
    html_print_symbol(html_file, uid, mod, true);
    fprintf(html_file, "</B>\n");
    for_alist(stmt, mod->block->body)
      html_view_ast(stmt, html_file, uid);
    fprintf(html_file, "</HTML>\n");
    fclose(html_file);
   }
  }
  fprintf(html_index_file, "</TD></TR>");
  fflush(html_index_file);
  uid++;
}

static int log_indent;
static bool log_need_space;
static void log_ast(BaseAST* ast, FILE* file);
static void log_ast_header(BaseAST* ast, FILE* file);
static void log_ast_footer(BaseAST* ast, FILE* file);
static void log_ast_symbol(FILE* file, Symbol* sym, bool def);
static void log_ast_fnsymbol(FILE* file, FnSymbol* fn);

void dump_ast(const char* passName, int pass_num) {
  FILE* log_file;
  const char* filename;
  char numBuf[4];
  snprintf(numBuf, 4, "%02d", pass_num);

  forv_Vec(ModuleSymbol, mod, allModules) {
    if (*log_module &&  // is not NUL
        strcmp(log_module, mod->name))
      continue;

    filename = astr(mod->name, "_", numBuf, passName, ".ast");
    log_file = fopen(astr(log_dir, filename), "w");
    fprintf(log_file, "AST dump for %s after pass %s.\n", mod->name, passName);
    log_indent = 0; log_need_space = false;
    log_ast(mod, log_file);
    fclose(log_file);
  }
}

inline void log_newline(FILE* file) {
  putc('\n', file);
  for (int i = 0; i < 2 * log_indent; ++i)
    putc(' ', file);
  log_need_space = false;
}

inline void log_write(FILE* file, bool space_before, const char* text, bool space_after) {
  if (space_before && log_need_space)
    putc(' ', file);
  fputs(text, file);
  log_need_space = space_after;
}
  
static void
log_ast(BaseAST* ast, FILE* file) {
  if (Expr* expr = toExpr(ast))
    if (DefExpr* e = toDefExpr(expr))
      if (toModuleSymbol(e->sym))
        // Since we iterate modules at the top layer, don't visit nested modules here.
        return;
  if (Symbol* s = toSymbol(ast))
    if (toArgSymbol(s))
      // Don't show args; they are handled in log_ast_fnsymbol().
      return;

  log_ast_header(ast, file);
  AST_CHILDREN_CALL(ast, log_ast, file);
  log_ast_footer(ast, file);
}

static void
log_ast_header(BaseAST* ast, FILE* file) {
  if (Expr* expr = toExpr(ast)) {
    if (isBlockStmt(expr)) {
      log_newline(file);
      if (FnSymbol* fn = toFnSymbol(expr->parentSymbol))
        if (expr == fn->where)
          log_write(file, false, "where ", false);
      log_write(file, false, "{", true);
      ++log_indent;
    } else if (GotoStmt* s = toGotoStmt(expr)) {
      switch (s->gotoTag) {
       default: break;
       case GOTO_NORMAL:        log_write(file, true, "goto", true);            break;
       case GOTO_BREAK:         log_write(file, true, "break", true);           break;
       case GOTO_CONTINUE:      log_write(file, true, "continue", true);        break;
       case GOTO_RETURN:        log_write(file, true, "gotoReturn", true);      break;
       case GOTO_GETITER_END:   log_write(file, true, "gotoGetiterEnd", true);  break;
       case GOTO_ITER_RESUME:   log_write(file, true, "gotoIterResume", true);  break;
       case GOTO_ITER_END:      log_write(file, true, "gotoIterEnd", true);     break;
      }
      if (SymExpr* label = toSymExpr(s->label))
        if (label->var != gNil) {
          log_ast_symbol(file, label->var, true);
        }
    } else if (toCondStmt(expr)) {
      log_newline(file);
      log_write(file, true, "if", true);
    } else {
      if (expr->getStmtExpr() && expr->getStmtExpr() == expr) {
        log_newline(file);
      }
      if (DefExpr* e = toDefExpr(expr)) {
        if (FnSymbol* fn = toFnSymbol(e->sym)) {
          log_write(file, true, "function", true);
          log_ast_fnsymbol(file, fn);
        } else if (structuralTypeSymbol(e->sym)) {
          if (DefExpr *def = toDefExpr( ast)) {
            if (def->sym->hasFlag(FLAG_SYNC))
              log_write(file, true, "sync", true);
            if (def->sym->hasFlag(FLAG_SINGLE))
              log_write(file, true, "single", true);
          }
          log_write(file, true, "type", true);
          log_ast_symbol(file, e->sym, true);
        } else if (toTypeSymbol(e->sym)) {
          log_write(file, true, "type", true);
          log_ast_symbol(file, e->sym, true);
        } else if (VarSymbol* vs=toVarSymbol(e->sym)) {
          if (vs->type->symbol->hasFlag(FLAG_SYNC))
            log_write(file, true, "sync", true);
          if (vs->type->symbol->hasFlag(FLAG_SINGLE))
            log_write(file, true, "single", true);
          log_write(file, true, "var", true);
          log_ast_symbol(file, e->sym, true);
        } else if (toArgSymbol(e->sym)) {
          // Argsymbols are handled in the function header.
        } else if (toLabelSymbol(e->sym)) {
          log_write(file, true, "label", true);
          log_ast_symbol(file, e->sym, true);
        } else if (toModuleSymbol(e->sym)) {
          log_write(file, true, "module", true);
          log_ast_symbol(file, e->sym, true);
        } else {
          log_write(file, true, "def", true);
          log_ast_symbol(file, e->sym, true);
        }
      } else if (VarSymbol* e = get_constant(expr)) {
        if (e->immediate) {
          const size_t bufSize = 128;
          char imm[bufSize];
          snprint_imm(imm, bufSize, *e->immediate);
          if (log_need_space) putc(' ', file);
          fprintf(file, "%s%s", imm, is_imag_type(e->type) ? "i" : "");
          log_need_space = true;
        } else {
          log_write(file, true, e->name, true);
        }
      } else if (SymExpr* e = toSymExpr(expr)) {
        log_ast_symbol(file, e->var, false);
      } else if (UnresolvedSymExpr* e = toUnresolvedSymExpr(expr)) {
        log_write(file, true, e->unresolved, true);
      } else if (NamedExpr* e = toNamedExpr(expr)) {
        fprintf(file, "(%s =", e->name);
        log_need_space = true;
      } else if (CallExpr* e = toCallExpr(expr)) {
        if (e->isResolved()) {
          if (e->isResolved()->hasFlag(FLAG_BEGIN_BLOCK))
            log_write(file, true, "begin", true);
          else if (e->isResolved()->hasFlag(FLAG_ON_BLOCK))
            log_write(file, true, "on", true);
        }
        if (fLogIds) {
          fprintf(file, "(%d ", e->id);
          log_need_space = false;
        } else {
          log_write(file, true, "(", false);
        }
        if (!e->primitive) {
          log_write(file, true, "call", true);
        } else {
          if (e->isPrimitive(PRIM_RETURN))
            log_write(file, true, "return", true);
          else if (e->isPrimitive(PRIM_YIELD))
            log_write(file, true, "yield", true);
          else {
            if (log_need_space) putc(' ', file);
            fprintf(file, "'%s'", e->primitive->name);
            log_need_space = true;
          }
        }
        if (e->partialTag)
          log_write(file, true, "(partial)", true);
      } else {
        if (log_need_space) putc(' ', file);
        fprintf(file, "(%s", astTagName[expr->astTag]);
        log_need_space = true;
      }
    }
  }
}

static void
log_ast_footer(BaseAST* ast, FILE* file) {
  if (Expr* expr = toExpr(ast)) {
    if (toCallExpr(expr) || toNamedExpr(expr)) {
      log_write(file, false, ")", true);
    }

    if (toBlockStmt(expr)) {
      --log_indent;
      log_newline(file);
      log_write(file, false, "}", true);
    }
  }
}

static void
log_ast_symbol(FILE* file, Symbol* sym, bool def) {
  if (log_need_space) putc(' ', file);
  log_need_space = false;

  if (def)
    if (ArgSymbol* arg = toArgSymbol(sym)) {
      log_need_space = true;
      switch (arg->intent) {
       case INTENT_IN:      fprintf(file, "in");        break;
       case INTENT_INOUT:   fprintf(file, "inout");     break;
       case INTENT_OUT:     fprintf(file, "out");       break;
       case INTENT_CONST:   fprintf(file, "const");     break;
       case INTENT_CONST_IN:  fprintf(file, "const in");  break;
       case INTENT_CONST_REF: fprintf(file, "const ref"); break;
       case INTENT_REF:     fprintf(file, "ref");       break;
       case INTENT_PARAM:   fprintf(file, "param");     break;
       case INTENT_TYPE:    fprintf(file, "type");      break;
      default: log_need_space = false; break;
      }
      log_write(file, true, "arg", true);
    }

  log_write(file, true, sym->name, true);

  if (fLogIds)
    fprintf(file, "[%d]", sym->id);

  if (def &&
      !toTypeSymbol(sym) &&
      sym->type &&
      sym->type->symbol &&
      sym->type != dtUnknown) {
    log_write(file, false, ":", false);
    log_ast_symbol(file, sym->type->symbol, false);
  }

  if (sym->hasFlag(FLAG_GENERIC))
    log_write(file, false, "?", false);

  log_need_space = true;
}

static void
log_ast_fnsymbol(FILE* file, FnSymbol* fn) {
  if (fn->_this && fn->_this->defPoint) {
    log_ast_symbol(file, fn->_this->type->symbol, false);
    log_write(file, false, ".", false);
  }
  log_ast_symbol(file, fn, true);
  log_write(file, false, "(", false);
  bool first = true;
  for_formals(formal, fn) {
    if (first) {
      first = false;
    } else {
      log_write(file, false, ",", true);
    }
    log_ast_symbol(file, formal, true);
    if (formal->typeExpr) {
      log_write(file, true, ":", true);
      if (BlockStmt* block = toBlockStmt(formal->typeExpr))
        log_ast(block->body.first(), file);
      else
        log_ast(formal->typeExpr, file);
    }
    if (formal->defaultExpr) {
      log_write(file, true, "=", true);
      if (BlockStmt* block = toBlockStmt(formal->defaultExpr))
        log_ast(block->body.first(), file);
      else
        log_ast(formal->defaultExpr, file);
    }
  }
  log_write(file, false, ")", true);
  if (fn->retTag == RET_VAR)
    log_write(file, true, "var", true);
  else if (fn->retTag == RET_PARAM)
    log_write(file, true, "param", true);
  else if (fn->retTag == RET_TYPE)
    log_write(file, true, "type", true);
  if (fn->retType && fn->retType->symbol) {
    log_write(file, true, ":", true);
    log_ast_symbol(file, fn->retType->symbol, false);
  }
}

void map_view(SymbolMap* map, const char* msg) {
  if (msg) printf("SymbolMap %s\n", msg);
  int cnt = 0;
  bool temp_log_need_space = log_need_space;
  log_need_space = true;
  
  form_Map(SymbolMapElem, elm, *map) {
    Symbol* key = elm->key;
    Symbol* val = elm->value;
    if (key || val) {
      cnt++;
      printf(" ");
      if (key) {
        log_ast_symbol(stdout, key, true);
        printf(" %c", key->hasFlag(FLAG_CONST) ? 'c' : 'v');
      } else {
        printf("NULL");
      }
      if (val != NULL && val != gNil) {
        printf(" => ");
        log_ast_symbol(stdout, val, true);
        printf(" %c", val->hasFlag(FLAG_CONST) ? 'c' : 'v');
      } else {
        // nothing
      }
      printf("\n");
    }
  }
  printf("  %d elms\n", cnt);
  log_need_space = temp_log_need_space;
}
