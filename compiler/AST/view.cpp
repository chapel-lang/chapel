#include "view.h"
#include "astutil.h"
#include "stmt.h"
#include "expr.h"
#include "symbol.h"
#include "symscope.h"
#include "type.h"
#include "stringutil.h"
#include "log.h"


static void
list_sym(Symbol* sym, bool type = true) {
  if (VarSymbol* var = dynamic_cast<VarSymbol*>(sym)) {
    if (var->immediate) {
      if (var->immediate->const_kind == NUM_KIND_INT) {
        printf("%lld ", var->immediate->int_value());
        return;
      } else if (var->immediate->const_kind == CONST_KIND_STRING) {
        printf("\"%s\" ", var->immediate->v_string);
        return;
      }
    }
  }
  if (dynamic_cast<FnSymbol*>(sym)) {
    printf("fn ");
  } else if (dynamic_cast<ArgSymbol*>(sym)) {
    printf("arg ");
  } else if (dynamic_cast<TypeSymbol*>(sym)) {
    printf("type ");
  }
  printf("%s", sym->name);
  printf("[%d]", sym->id);
  if (!type) {
    printf(" ");
  } else if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym)) {
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
  if (CondStmt* cond = dynamic_cast<CondStmt*>(expr->parentExpr)) {
    if (cond->condExpr == expr)
      return false;
  }
  if (!expr->parentExpr || IS_STMT(expr->parentExpr))
    return true;
  return false;
}


static void
list_ast(BaseAST* ast, int indent = 0) {
  if (Expr* expr = dynamic_cast<Expr*>(ast)) {
    if (list_line(expr)) {
      printf("%-7d ", expr->id);
      for (int i = 0; i < indent; i++)
        printf(" ");
    }
    if (GotoStmt* e = dynamic_cast<GotoStmt*>(ast)) {
      printf("goto ");
      if (e->label) {
        list_ast(e->label, indent+1);
      }
    } else if (dynamic_cast<BlockStmt*>(ast)) {
      printf("{\n");
    } else if (dynamic_cast<CondStmt*>(ast)) {
      printf("if ");
    } else if (CallExpr* e = dynamic_cast<CallExpr*>(expr)) {
      if (e->primitive)
        printf("%s( ", e->primitive->name);
      else
        printf("call( ");
    } else if (NamedExpr* e = dynamic_cast<NamedExpr*>(expr)) {
      printf("%s = ", e->name);
    } else if (dynamic_cast<DefExpr*>(expr)) {
      printf("def ");
    } else if (SymExpr* e = dynamic_cast<SymExpr*>(expr)) {
      list_sym(e->var, false);
    }
  }

  if (Symbol* sym = dynamic_cast<Symbol*>(ast))
    list_sym(sym);
  if (dynamic_cast<FnSymbol*>(ast) || dynamic_cast<ModuleSymbol*>(ast)) {
    printf("\n");
  }

  int new_indent = indent;

  if (Expr* expr = dynamic_cast<Expr*>(ast))
    if (list_line(expr))
      new_indent = indent+2;

  Vec<BaseAST*> asts;
  get_ast_children(ast, asts);
  forv_Vec(BaseAST, ast, asts)
    list_ast(ast, new_indent);

  if (Expr* expr = dynamic_cast<Expr*>(ast)) {
    if (dynamic_cast<CallExpr*>(expr)) {
      printf(") ");
    }
    if (dynamic_cast<BlockStmt*>(ast)) {
      printf("%-7d ", expr->id);
      for (int i = 0; i < indent; i++)
        printf(" ");
      printf("}\n");
    } else if (CondStmt* cond = dynamic_cast<CondStmt*>(expr->parentExpr)) {
      if (cond->condExpr == expr)
        printf("\n");
    } else if (!dynamic_cast<CondStmt*>(expr) && list_line(expr)) {
      DefExpr* def = dynamic_cast<DefExpr*>(expr);
      if (!(def && (dynamic_cast<FnSymbol*>(def->sym) ||
                    dynamic_cast<ModuleSymbol*>(def->sym))))
        printf("\n");
    }
  }
}


static char*
html_file_name( int pass, const char *module) {
  return stringcat( "pass", intstring(pass), "_module_", module, ".html");
}

static void
view_ast(BaseAST* ast, bool number = false, int mark = -1, int indent = 0) {
  if (!ast)
    return;
  if (Expr* expr = dynamic_cast<Expr*>(ast)) {
    printf("\n");
    for (int i = 0; i < indent; i++)
      printf(" ");
    printf("(");
    if (ast->id == mark)
      printf("***");
    if (number)
      printf("%d ", ast->id);
    printf("%s", astTypeName[expr->astType]);

    if (GotoStmt *gs= dynamic_cast<GotoStmt*>(ast)) {
      if (gs->label) {
        printf( " ");
        view_ast( gs->label, number, mark, indent+1);
      }
    }

    if (CallExpr* call = dynamic_cast<CallExpr*>(expr))
      if (call->primitive)
        printf(" %s", call->primitive->name);

    if (NamedExpr* named = dynamic_cast<NamedExpr*>(expr))
      printf(" \"%s\"", named->name);

    if (dynamic_cast<DefExpr*>(expr))
      printf(" ");

    long i;
    const char *str;
    if (get_int(expr, &i)) {
      printf(" %ld", i);
    } else if (get_string(expr, &str)) {
      printf(" \"%s\"", str);
    }

    if (SymExpr* sym = dynamic_cast<SymExpr*>(expr)) {
      printf(" '");
      if (sym->var->id == mark)
        printf("***");
      if (dynamic_cast<FnSymbol*>(sym->var)) {
        printf("fn ");
      } else if (dynamic_cast<ArgSymbol*>(sym->var)) {
        printf("arg ");
      } else if (dynamic_cast<TypeSymbol*>(sym->var)) {
        printf("type ");
      }
      printf("%s", sym->var->name);
      if (number)
        printf("[%d]", sym->var->id);
      if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym->var)) {
        printf(":%s", fn->retType->symbol->name);
        if (number)
          printf("[%d]", fn->retType->symbol->id);
      } else if (sym->var->type && sym->var->type->symbol) {
        printf(":%s", sym->var->type->symbol->name);
        if (number)
          printf("[%d]", sym->var->type->symbol->id);
      }
      printf("'");
    }
  }

  if (Symbol* sym = dynamic_cast<Symbol*>(ast)) {
    printf("'");
    if (ast->id == mark)
      printf("***");
    if (dynamic_cast<FnSymbol*>(sym)) {
      printf("fn ");
    } else if (dynamic_cast<ArgSymbol*>(sym)) {
      printf("arg ");
    } else if (dynamic_cast<TypeSymbol*>(sym)) {
      printf("type ");
    }
    printf("%s", sym->name);
    if (number)
      printf("[%d]", sym->id);
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym)) {
      printf(":%s", fn->retType->symbol->name);
      if (number)
        printf("[%d]", fn->retType->symbol->id);
    } else if (sym->type && sym->type->symbol) {
      printf(":%s", sym->type->symbol->name);
      if (number)
        printf("[%d]", sym->type->symbol->id);
    }
    printf("'");
  }

  Vec<BaseAST*> asts;
  get_ast_children(ast, asts);
  forv_Vec(BaseAST, ast, asts)
    view_ast(ast, number, mark, indent + 2);

  if (dynamic_cast<Expr*>(ast))
    printf(")");
}

static void view_symtab(BaseAST* ast, bool number = false, int indent = 0) {
  SymScope* scope = NULL;
  if (BlockStmt* a = dynamic_cast<BlockStmt*>(ast))
    scope = a->blkScope;
  else if (ModuleSymbol* a = dynamic_cast<ModuleSymbol*>(ast))
    scope = a->block->blkScope;
  else if (FnSymbol* a = dynamic_cast<FnSymbol*>(ast))
    scope = a->argScope;
  else if (TypeSymbol* a = dynamic_cast<TypeSymbol*>(ast))
    if (ClassType* ct = dynamic_cast<ClassType*>(a->type))
      scope = ct->structScope;

  if (scope)
    scope->print(number, indent);

  Vec<BaseAST*> asts;
  get_ast_children(ast, asts);
  forv_Vec(BaseAST, ast, asts)
    view_symtab(ast, number, indent + 2);
}

void list_view(BaseAST* ast) {
  if (dynamic_cast<Symbol*>(ast))
    printf("%-7d ", ast->id);
  list_ast(ast);
  printf("\n");
  fflush(stdout);
}

void list_view_noline(BaseAST* ast) {
  if (dynamic_cast<Symbol*>(ast))
    printf("%-7d ", ast->id);
  list_ast(ast);
  fflush(stdout);
}

void print_view(BaseAST* ast) {
  view_ast(ast);
  printf("\n\n");
  fflush(stdout);
}

void print_view_noline(BaseAST* ast) {
  view_ast(ast);
  fflush(stdout);
}

void nprint_view(BaseAST* ast) {
  view_ast(ast, true);
  printf("\n\n");
  fflush(stdout);
}

void nprint_view_noline(BaseAST* ast) {
  view_ast(ast, true);
  fflush(stdout);
}


BaseAST* ast(int id) {
  Vec<BaseAST*> asts;
  collect_asts(&asts);
  forv_Vec(BaseAST, a, asts)
    if (a->id == id)
      return a;
  return NULL;
}


void iprint_view(int id) {
  nprint_view(ast(id));
}


void print_symtab(BaseAST* ast) {
  view_symtab(ast);
  fflush(stdout);
}

void nprint_symtab(BaseAST* ast) {
  view_symtab(ast, true);
  fflush(stdout);
}

void mark_view(BaseAST* ast, int id) {
  view_ast(ast, false, id);
  printf("\n\n");
  fflush(stdout);
}

static ClassType *
structuralTypeSymbol(Symbol *s) {
  if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(s))
    if (ClassType *st = dynamic_cast<ClassType*>(ts->type))
      return st;
  return NULL;
}

static void
html_print_symbol(FILE* html_file, int pass, Symbol* sym, bool def) {
  if (!dynamic_cast<UnresolvedSymbol*>(sym)) {
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
  }
  if (dynamic_cast<FnSymbol*>(sym)) {
    fprintf(html_file, "<FONT COLOR=\"blue\">");
  } else if (dynamic_cast<TypeSymbol*>(sym)) {
    fprintf(html_file, "<FONT COLOR=\"green\">");
  } else {
    fprintf(html_file, "<FONT COLOR=\"red\">");
  }
  fprintf(html_file, "%s", sym->name);
  fprintf(html_file, "</FONT>");
  if (!dynamic_cast<UnresolvedSymbol*>(sym)) {
    fprintf(html_file, "<FONT COLOR=\"grey\">[%d]</FONT>", sym->id);
  }
  fprintf(html_file, "</A>");
  if (def &&
      !dynamic_cast<TypeSymbol*>(sym) &&
      sym->type &&
      sym->type->symbol &&
      sym->type != dtUnknown) {
    fprintf(html_file, ":");
    html_print_symbol( html_file, pass, sym->type->symbol, false);
  }
}

static void
html_print_fnsymbol( FILE* html_file, int pass, FnSymbol* fn) {
  if (fn->_this) {
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
  if (fn->retRef)
    fprintf(html_file, "<b>ref</b> ");
  if (fn->retType->symbol) {
    fprintf(html_file, " : ");
    html_print_symbol( html_file, pass, fn->retType->symbol, false);
  }
}

static void
html_view_ast( FILE* html_file, int pass, BaseAST* ast) {
  if (Expr* expr = dynamic_cast<Expr*>(ast)) {
    if (BlockStmt *b=dynamic_cast<BlockStmt*>(expr)) {
      fprintf(html_file, "<DL>\n");
      fprintf(html_file, "{");
      switch( b->blockTag) {
      case BLOCK_ATOMIC: fprintf( html_file, "atomic"); break;
      case BLOCK_BEGIN: fprintf( html_file, "begin"); break;
      case BLOCK_COBEGIN: fprintf( html_file, "cobegin"); break;
      case BLOCK_DO_WHILE: fprintf( html_file, "do while"); break;
      case BLOCK_FOR: fprintf( html_file, "for"); break;
      case BLOCK_FORALL: fprintf( html_file, "forall"); break;
      case BLOCK_ORDERED_FORALL: fprintf( html_file, "ordered forall"); break;
      case BLOCK_SERIAL: fprintf( html_file, "serial"); break;
      default:
        break;
      }
    } else if (GotoStmt* s = dynamic_cast<GotoStmt*>(expr)) {
      fprintf(html_file, "<DL>\n");
      switch (s->goto_type) {
      case goto_normal: fprintf(html_file, "<B>goto</B> "); break;
      case goto_break: fprintf(html_file, "<B>break</B> "); break;
      case goto_continue: fprintf(html_file, "<B>continue</B> "); break;
      }
      if (s->label)
        html_print_symbol( html_file, pass, s->label, true);
    } else if (dynamic_cast<CondStmt*>(expr)) {
      fprintf(html_file, "<DL>\n");
      fprintf(html_file, "<B>if</B> ");
    } else {
      if (expr->getStmtExpr() && expr->getStmtExpr() == expr) {
        fprintf(html_file, "<DL>\n");
      }
      fprintf(html_file, " ");
      if (DefExpr* e = dynamic_cast<DefExpr*>(expr)) {
        if (FnSymbol* fn = dynamic_cast<FnSymbol*>(e->sym)) {
          fprintf(html_file, "<UL CLASS =\"mktree\">\n<LI>");
          fprintf(html_file, "<CHPLTAG=\"FN%d\">\n", fn->id);
          fprintf(html_file, "<B>function ");
          html_print_fnsymbol( html_file, pass, fn);
          fprintf(html_file, "</B><UL>\n");
        } else if (structuralTypeSymbol(e->sym)) {
          fprintf(html_file, "<UL CLASS =\"mktree\">\n");
          fprintf(html_file, "<LI>");
          if (DefExpr *def = dynamic_cast<DefExpr*>( ast))
            if (def->sym->hasPragma( "sync var")) {
              fprintf( html_file, "<B>sync</B> ");
            } else if (def->sym->hasPragma( "single var")) {
              fprintf( html_file, "<B>single</B> ");
            }
          fprintf(html_file, "<B>type ");
          html_print_symbol( html_file, pass, e->sym, true);
          fprintf(html_file, "</B><UL>\n");
        } else if (dynamic_cast<TypeSymbol*>(e->sym)) {
          fprintf(html_file, "<B>type </B> ");
          html_print_symbol( html_file, pass, e->sym, true);
        } else if (VarSymbol* vs=dynamic_cast<VarSymbol*>(e->sym)) {
          if (vs->type->symbol->hasPragma( "sync var")) {
            fprintf( html_file, "<B>sync </B>");
          } else if (vs->type->symbol->hasPragma( "single var")) {
            fprintf( html_file, "<B>single </B>");
          }
          fprintf(html_file, "<B>var </B> ");
          html_print_symbol( html_file, pass, e->sym, true);
        } else if (ArgSymbol* s = dynamic_cast<ArgSymbol*>(e->sym)) {
          switch (s->intent) {
          case INTENT_IN: fprintf(html_file, "<B>in</B> "); break;
          case INTENT_INOUT: fprintf(html_file, "<B>inout</B> "); break;
          case INTENT_OUT: fprintf(html_file, "<B>out</B> "); break;
          case INTENT_CONST: fprintf(html_file, "<B>const</B> "); break;
          case INTENT_REF: fprintf(html_file, "<B>ref</B> "); break;
          case INTENT_PARAM: fprintf(html_file, "<B>param</B> "); break;
          case INTENT_TYPE: fprintf(html_file, "<B>type</B> "); break;
          default: break;
          }
          fprintf(html_file, "<B>arg</B> ");
          html_print_symbol( html_file, pass, e->sym, true);
        } else if (dynamic_cast<LabelSymbol*>(e->sym)) {
          fprintf(html_file, "<B>label</B> ");
          html_print_symbol( html_file, pass, e->sym, true);
        } else {
          fprintf(html_file, "<B>def</B> ");
          html_print_symbol( html_file, pass, e->sym, true);
        }
      } else if (VarSymbol* e = get_constant(expr)) {
        if (e->immediate) {
          char imm[128];
          sprint_imm(imm, *e->immediate);
          fprintf(html_file, "<i><FONT COLOR=\"blue\">%s%s</FONT></i>", imm, is_imag_type(e->type) ? "i" : "");
        } else {
          fprintf(html_file, "<i><FONT COLOR=\"blue\">%s</FONT></i>", e->name);
        }
      } else if (SymExpr* e = dynamic_cast<SymExpr*>(expr)) {
        html_print_symbol( html_file, pass, e->var, false);
      } else if (NamedExpr* e = dynamic_cast<NamedExpr*>(expr)) {
        fprintf(html_file, "(%s = ", e->name);
      } else if (CallExpr* e = dynamic_cast<CallExpr*>(expr)) {
        fprintf(html_file, "(%d ", e->id);
        if (!e->primitive) {
          fprintf(html_file, "<B>call</B> ");
        } else {
          if (e->isPrimitive(PRIMITIVE_RETURN))
            fprintf(html_file, "<B>return</B> ");
          else if (e->isPrimitive(PRIMITIVE_YIELD))
            fprintf(html_file, "<B>yield</B> ");
          else
            fprintf(html_file, "'%s' ", e->primitive->name);
        }
        if (e->partialTag)
          fprintf(html_file, "(partial) ");
      } else {
        fprintf(html_file, "(%s", astTypeName[expr->astType]);
      }
    }
  }

  Vec<BaseAST*> asts;
  get_ast_children(ast, asts);
  forv_Vec(BaseAST, ast, asts)
    html_view_ast( html_file, pass, ast);

  if (Expr* expr = dynamic_cast<Expr*>(ast)) {
    if (DefExpr* e = dynamic_cast<DefExpr*>(expr)) {
      if (dynamic_cast<FnSymbol*>(e->sym) || 
          (dynamic_cast<TypeSymbol*>(e->sym) &&
           dynamic_cast<ClassType*>(e->sym->type))) {
        fprintf(html_file, "</UL>\n");
        if (FnSymbol* fn = dynamic_cast<FnSymbol*>(e->sym)) {
          fprintf(html_file, "<CHPLTAG=\"FN%d\">\n", fn->id);
        }
        fprintf(html_file, "</UL>\n");
      }
    } else if (dynamic_cast<SymExpr*>(expr)) {
    } else if (dynamic_cast<CallExpr*>(expr) ||
               dynamic_cast<NamedExpr*>(expr)) {
      fprintf(html_file, ")");
    }

    if (dynamic_cast<BlockStmt*>(expr) ||
        dynamic_cast<CondStmt*>(expr) ||
        dynamic_cast<GotoStmt*>(expr) ||
        expr->getStmtExpr() && expr->getStmtExpr() == expr) {
      if (dynamic_cast<BlockStmt*>(expr))
        fprintf(html_file, "}");
      fprintf(html_file, "</DL>\n");
    }

  }
}

void html_view(const char* passName) {
  static int uid = 1;
  FILE* html_file;
  char* filename;

  forv_Vec(ModuleSymbol, mod, allModules) {
    // filename = stringcat("pass", intstring(uid), "_module_", mod->name, ".html");
    filename = html_file_name( uid, mod->name);
    fprintf(html_index_file, "&nbsp;&nbsp;<a href=\"%s\">%s</a>\n", filename, mod->name);
    html_file = fopen(stringcat(log_dir, filename), "w");
    fprintf(html_file, "<CHPLTAG=\"%s\">\n", passName);
    fprintf(html_file, "<HTML>\n");
    fprintf(html_file, "<HEAD>\n");
    fprintf(html_file, "<TITLE> AST for Module %s after Pass %s </TITLE>\n", mod->name, passName);
    fprintf(html_file, "<SCRIPT SRC=\"%s/compiler/etc/www/mktree.js\" LANGUAGE=\"JavaScript\"></SCRIPT>", chplhome);
    fprintf(html_file, "<LINK REL=\"stylesheet\" HREF=\"%s/compiler/etc/www/mktree.css\">", chplhome);
    fprintf(html_file, "</HEAD>\n");
    fprintf(html_file, "<div style=\"text-align: center;\"><big><big><span style=\"font-weight: bold;\">");
    fprintf(html_file, "AST for Module %s after Pass %s <br><br></span></big></big>\n", mod->name, passName);
    fprintf(html_file, "<div style=\"text-align: left;\">\n\n");
    fprintf(html_file, "<B>module %s</B>\n", mod->name);
    for_alist(Expr, stmt, mod->block->body)
      html_view_ast( html_file, uid, stmt);
    fprintf(html_file, "</HTML>\n");
    fclose(html_file);
  }
  uid++;
}
