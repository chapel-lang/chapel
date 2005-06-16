#include "moduleList.h"
#include <typeinfo>
#include "view.h"
#include "stmt.h"
#include "expr.h"
#include "symbol.h"
#include "type.h"
#include "symtab.h"
#include "stringutil.h"

static void html_print_symbol(FILE* html_file, Symbol* sym) {
  if (dynamic_cast<FnSymbol*>(sym)) {
    fprintf(html_file, "<FONT COLOR=\"blue\">");
  } else if (dynamic_cast<TypeSymbol*>(sym)) {
    fprintf(html_file, "<FONT COLOR=\"green\">");
  } else {
    fprintf(html_file, "<FONT COLOR=\"red\">");
  }
  fprintf(html_file, "%s", sym->name);
  fprintf(html_file, "</FONT>");
  fprintf(html_file, "<FONT COLOR=\"grey\">[%ld]</FONT>", sym->id);
  if (!dynamic_cast<TypeSymbol*>(sym) && sym->type && sym->type->symbol) {
    fprintf(html_file, ":<FONT COLOR=\"green\">%s</FONT>", sym->type->symbol->name);
    fprintf(html_file, "<FONT COLOR=\"grey\">[%ld]</FONT>", sym->type->symbol->id);
  }
}

static void html_print_fnsymbol(FILE* html_file, FnSymbol* fn) {
  if (fn->typeBinding) {
    html_print_symbol(html_file, fn->typeBinding);
    fprintf(html_file, " . ");
  }
  html_print_symbol(html_file, fn);
  fprintf(html_file, " ( ");
  bool first = true;
  for_alist(ParamSymbol, arg, fn->formals) {
    if (!first) {
      fprintf(html_file, " , ");
    } else {
      first = false;
    }
    html_print_symbol(html_file, arg);
  }
  fprintf(html_file, " ) ");
}

View::View(bool initNumberSymbols, FILE* init_html_file) {
  indent = 0;
  numberSymbols = initNumberSymbols;
  html_file = init_html_file;
}

void View::run(ModuleList* moduleList) {
  if (!strncmp(args, "html ", 5)) {
    html = copystring(args+5);
    fprintf(html_file, "<HTML>\n");
    fprintf(html_file, "<HEAD>\n");
    fprintf(html_file, "<TITLE> AST after pass %s </TITLE>\n", html);
    fprintf(html_file, "<SCRIPT SRC=\"%s/etc/www/mktree.js\" LANGUAGE=\"JavaScript\"></SCRIPT>", 
           system_dir);
    fprintf(html_file, "<LINK REL=\"stylesheet\" HREF=\"%s/etc/www/mktree.css\">", 
            system_dir);
    fprintf(html_file, "</HEAD>\n");
    fprintf(html_file, "<div style=\"text-align: center;\"><big><big><span style=\"font-weight: bold;\">");
    fprintf(html_file, "AST after Pass %s <br><br></span></big></big>\n", html);
    fprintf(html_file, "<div style=\"text-align: left;\">\n\n");
  }
  ModuleSymbol* mod = moduleList->first();
  while (mod) {
    if (html) {
      fprintf(html_file, "<UL CLASS=\"mktree\">\n");
      fprintf(html_file, "<LI><B>module %s</B>\n", mod->name);
      fprintf(html_file, "<UL>\n");
    } else {
      printf("\nMODULE: %s\n", mod->name);
    }
    mod->startTraversal(this);
    if (html) {
      fprintf(html_file, "</UL></UL>\n");
    }
    mod = moduleList->next();
  }
  if (html) {
    fprintf(html_file, "\n</HTML>\n");
  } else {
    printf("\n\n");
  }
}

void View::preProcessStmt(Stmt* stmt) {
  if (html) {
    if (DefStmt* s = dynamic_cast<DefStmt*>(stmt)) {
      if (s->definesFunctions() || s->definesTypes()) {
        return;
      }
    }
    fprintf(html_file, "<BR>");
    for (int i = 0; i < indent; i++) {
      fprintf(html_file, "&nbsp;");
    }
    if (dynamic_cast<BlockStmt*>(stmt)) {
      fprintf(html_file, "{");
    } else {
      fprintf(html_file, "(%s", astTypeName[stmt->astType]);
    }
  } else {
    printf("\n");
    for (int i = 0; i < indent; i++) {
      printf(" ");
    }
    printf("(%s", astTypeName[stmt->astType]);
  }
  indent += 2;
}

void View::postProcessStmt(Stmt* stmt) {
  if (html) {
    if (DefStmt* s = dynamic_cast<DefStmt*>(stmt)) {
      if (s->definesFunctions() || s->definesTypes()) {
        return;
      }
    }
    if (dynamic_cast<BlockStmt*>(stmt)) {
      fprintf(html_file, "<BR>");
      for (int i = 0; i < indent; i++) {
        fprintf(html_file, "&nbsp;");
      }
      fprintf(html_file, "}");
    } else {
      fprintf(html_file, ")");
    }
  } else {
    printf(")");
  }
  indent -= 2;
}

void View::preProcessExpr(Expr* expr) {
  if (html) {
    fprintf(html_file, "<BR>\n");
    for (int i = 0; i < indent; i++) {
      fprintf(html_file, "&nbsp;");
    }
    if (DefExpr* e = dynamic_cast<DefExpr*>(expr)) {
      if (FnSymbol* fn = dynamic_cast<FnSymbol*>(e->sym)) {
        fprintf(html_file, "<UL CLASS =\"mktree\">\n");
        fprintf(html_file, "<LI><B>function ");
        html_print_fnsymbol(html_file, fn);
        fprintf(html_file, "</B><UL>\n");
      } else if (TypeSymbol* ts = dynamic_cast<TypeSymbol*>(e->sym)) {
        fprintf(html_file, "<UL CLASS =\"mktree\">\n");
        fprintf(html_file, "<LI><B>type ");
        html_print_symbol(html_file, ts);
        fprintf(html_file, "</B><UL>\n");
      } else {
        fprintf(html_file, "(%s ", astTypeName[expr->astType]);
        html_print_symbol(html_file, e->sym);
      }
    } else {
      fprintf(html_file, "(%s", astTypeName[expr->astType]);
    }
    if (IntLiteral* e = dynamic_cast<IntLiteral*>(expr)) {
      fprintf(html_file, " <FONT COLOR=\"lightblue\">%ld</FONT>", e->val);
    } else if (StringLiteral* e = dynamic_cast<StringLiteral*>(expr)) {
      fprintf(html_file, " <FONT COLOR=\"lightblue\">\"%s\"</FONT>", e->str);
    } else if (Variable* e = dynamic_cast<Variable*>(expr)) {
      fprintf(html_file, " ");
      html_print_symbol(html_file, e->var);
    }
  } else {
    printf("\n");
    for (int i = 0; i < indent; i++) {
      printf(" ");
    }
    printf("(");
    printf("%s", astTypeName[expr->astType]);
    if (IntLiteral* e = dynamic_cast<IntLiteral*>(expr)) {
      printf(" %ld", e->val);
    } else if (StringLiteral* e = dynamic_cast<StringLiteral*>(expr)) {
      printf(" \"%s\"", e->str);
    }
  }
  indent += 2;
}

void View::postProcessExpr(Expr* expr) {
  if (html) {
    if (MemberAccess* e = dynamic_cast<MemberAccess*>(expr)) {
      fprintf(html_file, " . ");
      html_print_symbol(html_file, e->member);
    } else if (DefExpr* e = dynamic_cast<DefExpr*>(expr)) {
      if (dynamic_cast<FnSymbol*>(e->sym) || 
          dynamic_cast<TypeSymbol*>(e->sym)) {
        fprintf(html_file, "</UL></UL>\n");
      } else {
        fprintf(html_file, ")");
      }
    } else {
      fprintf(html_file, ")");
    }
  } else {
    printf(")");
  }
  indent -= 2;
}

void View::preProcessSymbol(Symbol* sym) {
  if (!html) {
    printf(" '");
    if (dynamic_cast<FnSymbol*>(sym)) {
      printf("fn ");
    } else if (dynamic_cast<ParamSymbol*>(sym)) {
      printf("arg ");
    } else if (dynamic_cast<TypeSymbol*>(sym)) {
      printf("type ");
    }
    printf("%s", sym->name);
    if (numberSymbols) {
      printf("[%ld]", sym->id);
    }
    if (sym->type && sym->type->symbol) {
      printf(":%s", sym->type->symbol->name);
      if (numberSymbols) {
        printf("[%ld]", sym->type->symbol->id);
      }
    }
    printf("'");
  }
}

void View::postProcessSymbol(Symbol* sym) {

}

void View::preProcessType(Type* type) {

}

void View::postProcessType(Type* type) {

}

void print_view(BaseAST* ast) {
  TRAVERSE(ast, new View(), true);
  printf("\n\n");
  fflush(stdout);
}

void nprint_view(BaseAST* ast) {
  TRAVERSE(ast, new View(true), true);
  printf("\n\n");
  fflush(stdout);
}
