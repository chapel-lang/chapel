#include "view.h"
#include "astutil.h"
#include "stmt.h"
#include "expr.h"
#include "symbol.h"
#include "symscope.h"
#include "type.h"
#include "stringutil.h"
#include "chpl_log.h"


static Map<int, char*> *symid2file;         // map symbol id to html file w/ tag


static void
view_ast(BaseAST* ast, bool number = false, long mark = -1, int indent = 0) {
  if (Expr* expr = dynamic_cast<Expr*>(ast)) {
    printf("\n");
    for (int i = 0; i < indent; i++)
      printf(" ");
    printf("(");
    if (ast->id == mark)
      printf("***");
    if (number)
      printf("%ld ", ast->id);
    printf("%s", astTypeName[expr->astType]);

    if (CallExpr* call = dynamic_cast<CallExpr*>(expr))
      if (call->primitive)
        printf(" %s", call->primitive->name);

    if (NamedExpr* named = dynamic_cast<NamedExpr*>(expr))
      printf(" \"%s\"", named->name);

    if (dynamic_cast<DefExpr*>(expr))
      printf(" ");

    long i;
    char *str;
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
        printf("[%ld]", sym->var->id);
      if (sym->var->type && sym->var->type->symbol) {
        printf(":%s", sym->var->type->symbol->name);
        if (number)
          printf("[%ld]", sym->var->type->symbol->id);
      }
      if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym->var)) {
        printf(":%s", fn->retType->symbol->name);
        if (number)
          printf("[%ld]", fn->retType->symbol->id);
      }

      printf("'");
    }
  }

  if (Stmt* stmt = dynamic_cast<Stmt*>(ast)) {
    printf("\n");
    for (int i = 0; i < indent; i++)
      printf(" ");
    printf("(");
    if (ast->id == mark)
      printf("***");
    if (number)
      printf("%ld ", ast->id);
    printf("%s", astTypeName[stmt->astType]);
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
      printf("[%ld]", sym->id);
    if (sym->type && sym->type->symbol) {
      printf(":%s", sym->type->symbol->name);
      if (number)
        printf("[%ld]", sym->type->symbol->id);
    }
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym)) {
      printf(":%s", fn->retType->symbol->name);
      if (number)
        printf("[%ld]", fn->retType->symbol->id);
    }
    printf("'");
  }

  Vec<BaseAST*> asts;
  get_ast_children(ast, asts);
  forv_Vec(BaseAST, ast, asts)
    view_ast(ast, number, mark, indent + 2);

  if (dynamic_cast<Expr*>(ast) || dynamic_cast<Stmt*>(ast)) {
    printf(")");
  }
}

static void view_symtab(BaseAST* ast, bool number = false, int indent = 0) {
  SymScope* scope = NULL;
  if (BlockStmt* a = dynamic_cast<BlockStmt*>(ast))
    scope = a->blkScope;
  else if (ModuleSymbol* a = dynamic_cast<ModuleSymbol*>(ast))
    scope = a->modScope;
  else if (FnSymbol* a = dynamic_cast<FnSymbol*>(ast))
    scope = a->argScope;
  else if (TypeSymbol* a = dynamic_cast<TypeSymbol*>(ast))
    if (ClassType* ct = dynamic_cast<ClassType*>(a->definition))
      scope = ct->structScope;

  if (scope)
    scope->print(number, indent);

  Vec<BaseAST*> asts;
  get_ast_children(ast, asts);
  forv_Vec(BaseAST, ast, asts)
    view_symtab(ast, number, indent + 2);
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


void iprint_view(long id) {
  Vec<BaseAST*> asts;
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts)
    if (ast->id == id)
      print_view(ast);
}


void inprint_view(long id) {
  Vec<BaseAST*> asts;
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts)
    if (ast->id == id)
      nprint_view(ast);
}


void print_symtab(BaseAST* ast) {
  view_symtab(ast);
  fflush(stdout);
}

void nprint_symtab(BaseAST* ast) {
  view_symtab(ast, true);
  fflush(stdout);
}

void mark_view(BaseAST* ast, long id) {
  view_ast(ast, false, id);
  printf("\n\n");
  fflush(stdout);
}

static ClassType *
structuralTypeSymbol(Symbol *s) {
  if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(s))
    if (ClassType *st = dynamic_cast<ClassType*>(ts->definition))
      return st;
  return NULL;
}

static void
html_print_symbol(char *filename, FILE* html_file, Symbol* sym, bool def, bool show_analysis_info) {
  if (!sym->isUnresolved) {
    if (def) {
      fprintf(html_file, "<A NAME=\"SYM%ld\">", sym->id);
      symid2file->put( sym->id, filename);
    } else {
      if (char *deffile = symid2file->get( sym->id)) {
        fprintf(html_file, "<A HREF=\"%s#SYM%ld\">", deffile, sym->id);
      } else {
        fprintf(html_file, "<A HREF=\"#SYM%ld\">", sym->id);
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
  if (!sym->isUnresolved) {
    fprintf(html_file, "<FONT COLOR=\"grey\">[%ld]</FONT>", sym->id);
  }
  fprintf(html_file, "</A>");
  if (def &&
      !dynamic_cast<TypeSymbol*>(sym) &&
      sym->type &&
      sym->type->symbol &&
      sym->type != dtUnknown) {
    fprintf(html_file, ":");
    html_print_symbol( filename, html_file, sym->type->symbol, false, show_analysis_info);
  }
  if (show_analysis_info) {
    Vec<SymExpr *> constants;
    constant_info(sym, constants);
    if (constants.n) {
      if (constants.n > 1)
        fprintf(html_file, " constants: { ");
      else
        fprintf(html_file, " constant: ");
      for (int i = 0; i < constants.n; i++) {
        if (i > 0) fprintf(html_file, ", ");
        char *s;
        if (get_string(constants.v[i], &s))
          fprintf(html_file, "<i><FONT COLOR=\"blue\">'%s'</FONT></i>", s);
        else if (SymExpr* v = dynamic_cast<SymExpr* >(constants.v[i]))          
          html_print_symbol( filename, html_file, v->var, false, show_analysis_info);
        else 
          INT_FATAL("bad constant");
      }
      if (constants.n > 1)
        fprintf(html_file, " }");
    }
  }
}

static void
html_print_fnsymbol(char* filename, FILE* html_file, FnSymbol* fn, bool show_analysis_info) {
  if (fn->_this) {
    html_print_symbol( filename, html_file, fn->_this->type->symbol, false, show_analysis_info);
    fprintf(html_file, " . ");
  }
  html_print_symbol( filename, html_file, fn, true, show_analysis_info);
  fprintf(html_file, " ( ");
  bool first = true;
  for_alist(DefExpr, arg, fn->formals) {
    if (!first) {
      fprintf(html_file, " , ");
    } else {
      first = false;
    }
    html_print_symbol( filename, html_file, arg->sym, true, show_analysis_info);
  }
  fprintf(html_file, " ) ");
  if (fn->retType->symbol) {
    fprintf(html_file, " : ");
    html_print_symbol( filename, html_file, fn->retType->symbol, false, show_analysis_info);
  }
}

static void
html_view_ast(char *filename, FILE* html_file, BaseAST* ast, bool show_analysis_info) {
  if (Stmt* stmt = dynamic_cast<Stmt*>(ast)) {
    fprintf(html_file, "<DL>\n");
    if (BlockStmt *b=dynamic_cast<BlockStmt*>(stmt)) {
      fprintf(html_file, "{");
      switch( b->blockTag) {
      case BLOCK_ATOMIC: fprintf( html_file, "atomic"); break;
      case BLOCK_BEGIN: fprintf( html_file, "begin"); break;
      case BLOCK_COBEGIN: fprintf( html_file, "cobegin"); break;
      case BLOCK_DO_WHILE: fprintf( html_file, "do while"); break;
      case BLOCK_FOR: fprintf( html_file, "for"); break;
      case BLOCK_FORALL: fprintf( html_file, "forall"); break;
      case BLOCK_ORDERED_FORALL: fprintf( html_file, "ordered forall"); break;
      default:
        break;
      }
    } else if (GotoStmt* s = dynamic_cast<GotoStmt*>(stmt)) {
      switch (s->goto_type) {
      case goto_normal: fprintf(html_file, "<B>goto</B> "); break;
      case goto_break: fprintf(html_file, "<B>break</B> "); break;
      case goto_continue: fprintf(html_file, "<B>continue</B> "); break;
      }
      if (s->label)
        html_print_symbol( filename, html_file, s->label, true, show_analysis_info);
    } else if (dynamic_cast<CondStmt*>(stmt)) {
      fprintf(html_file, "<B>if</B> ");
    } else if (dynamic_cast<LabelStmt*>(stmt)) {
      fprintf(html_file, "<B>label</B> ");
    } else if (ReturnStmt* s = dynamic_cast<ReturnStmt*>(stmt)) {
      if (s->yield)
        fprintf(html_file, "<B>yield</B> ");
      else
        fprintf(html_file, "<B>return</B> ");
    } else if (!dynamic_cast<ExprStmt*>(ast)) {
      fprintf(html_file, "%s", astTypeName[stmt->astType]);
    }
  }

  if (Expr* expr = dynamic_cast<Expr*>(ast)) {
    fprintf(html_file, " ");
    if (DefExpr* e = dynamic_cast<DefExpr*>(expr)) {
      if (FnSymbol* fn = dynamic_cast<FnSymbol*>(e->sym)) {
        fprintf(html_file, "<UL CLASS =\"mktree\">\n<LI>");
        fprintf(html_file, "<CHPLTAG=\"FN%ld\">\n", fn->id);
        fprintf(html_file, "<B>function ");
        html_print_fnsymbol( filename, html_file, fn, show_analysis_info);
        fprintf(html_file, "</B><UL>\n");
      } else if (structuralTypeSymbol(e->sym)) {
        fprintf(html_file, "<UL CLASS =\"mktree\">\n");
        fprintf(html_file, "<LI><B>type ");
        html_print_symbol( filename, html_file, e->sym, true, show_analysis_info);
        fprintf(html_file, "</B><UL>\n");
      } else if (dynamic_cast<TypeSymbol*>(e->sym)) {
        fprintf(html_file, "<B>type</B> ");
        html_print_symbol( filename, html_file, e->sym, true, show_analysis_info);
      } else if (VarSymbol* vs=dynamic_cast<VarSymbol*>(e->sym)) {
        if (vs->is_ref) {
          fprintf(html_file, "<B>ref var</B> ");
        } else {
          fprintf(html_file, "<B>var</B> ");
        }
        html_print_symbol( filename, html_file, e->sym, true, show_analysis_info);
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
        html_print_symbol( filename, html_file, e->sym, true, show_analysis_info);
      } else {
        fprintf(html_file, "<B>def</B> ");
        html_print_symbol( filename, html_file, e->sym, true, show_analysis_info);
      }
    } else if (VarSymbol* e = get_constant(expr)) {
      if (e->immediate->const_kind == IF1_CONST_KIND_STRING) {
        fprintf(html_file, "<i><FONT COLOR=\"blue\">'%s'</FONT></i>", e->immediate->v_string);
      } else {
        fprintf(html_file, "<i><FONT COLOR=\"blue\">%s</FONT></i>", e->cname);
      }
    } else if (SymExpr* e = dynamic_cast<SymExpr*>(expr)) {
      html_print_symbol( filename, html_file, e->var, false, show_analysis_info);
    } else if (NamedExpr* e = dynamic_cast<NamedExpr*>(expr)) {
      fprintf(html_file, "(%s = ", e->name);
    } else if (CallExpr* e = dynamic_cast<CallExpr*>(expr)) {
      fprintf(html_file, "(%ld ", e->id);
      if (!e->primitive) {
        fprintf(html_file, "<B>call</B> ");
      } else {
        fprintf(html_file, "'%s' ", e->primitive->name);
      }
      if (e->partialTag == PARTIAL_OK)
        fprintf(html_file, "(partial ok) ");
      if (e->partialTag == PARTIAL_ALWAYS)
        fprintf(html_file, "(partial always) ");
    } else if (ImportExpr* e = dynamic_cast<ImportExpr*>(expr)) {
      if (e->importTag == IMPORT_WITH) {
        fprintf(html_file, "<B>with</B>");
      } else {
        fprintf(html_file, "<B>use</B>");
      }
    } else {
      fprintf(html_file, "(%s", astTypeName[expr->astType]);
    }
  }

  Vec<BaseAST*> asts;
  get_ast_children(ast, asts);
  forv_Vec(BaseAST, ast, asts)
    html_view_ast(filename, html_file, ast, show_analysis_info);

  if (Stmt* stmt = dynamic_cast<Stmt*>(ast)) {
    if (dynamic_cast<BlockStmt*>(stmt))
      fprintf(html_file, "}");
    fprintf(html_file, "</DL>\n");
  }

  if (Expr* expr = dynamic_cast<Expr*>(ast)) {
    if (DefExpr* e = dynamic_cast<DefExpr*>(expr)) {
      if (dynamic_cast<FnSymbol*>(e->sym) || 
          (dynamic_cast<TypeSymbol*>(e->sym) &&
           dynamic_cast<ClassType*>(e->sym->type))) {
        fprintf(html_file, "</UL>\n");
        if (FnSymbol* fn = dynamic_cast<FnSymbol*>(e->sym)) {
          fprintf(html_file, "<CHPLTAG=\"FN%ld\">\n", fn->id);
        }
        fprintf(html_file, "</UL>\n");
      }
    } else if (dynamic_cast<SymExpr*>(expr)) {
    } else {
      fprintf(html_file, ")");
    }
    if (show_analysis_info && expr->parentSymbol) {
      Vec<FnSymbol *> fns;
      call_info(expr, fns);
      if (fns.n) {
        fprintf(html_file, " calls: ");
        for (int i = 0; i < fns.n; i++) {
          if (i > 0) fprintf(html_file, " ");
          html_print_symbol( filename, html_file, fns.v[i], false, show_analysis_info);
        }
      }
    }
  }
}

void html_view(int show_analysis_info) {
  static int uid = 1;
  FILE* html_file;
  char* filename;
  symid2file = new Map<int, char*>;
  forv_Vec(ModuleSymbol, mod, allModules) {
    filename = stringcat("pass", intstring(uid), "_module_", mod->name, ".html");
    fprintf(html_index_file, "&nbsp;&nbsp;<a href=\"%s\">%s</a>\n", filename, mod->name);
    html_file = fopen(stringcat(log_dir, filename), "w");
    fprintf(html_file, "<CHPLTAG=\"%s\">\n", currentTraversal);
    fprintf(html_file, "<HTML>\n");
    fprintf(html_file, "<HEAD>\n");
    fprintf(html_file, "<TITLE> AST for Module %s after Pass %s </TITLE>\n", mod->name, currentTraversal);
    fprintf(html_file, "<SCRIPT SRC=\"%s/etc/www/mktree.js\" LANGUAGE=\"JavaScript\"></SCRIPT>", system_dir);
    fprintf(html_file, "<LINK REL=\"stylesheet\" HREF=\"%s/etc/www/mktree.css\">", system_dir);
    fprintf(html_file, "</HEAD>\n");
    fprintf(html_file, "<div style=\"text-align: center;\"><big><big><span style=\"font-weight: bold;\">");
    fprintf(html_file, "AST for Module %s after Pass %s <br><br></span></big></big>\n", mod->name, currentTraversal);
    fprintf(html_file, "<div style=\"text-align: left;\">\n\n");
    fprintf(html_file, "<B>module %s</B>\n", mod->name);
    for_alist(Stmt, stmt, mod->stmts)
      html_view_ast(filename, html_file, stmt, show_analysis_info);
    fprintf(html_file, "</HTML>\n");
    fclose(html_file);
  }
  uid++;
  delete symid2file;
}
