#include "moduleList.h"
#include <typeinfo>
#include "view.h"
#include "stmt.h"
#include "expr.h"
#include "symbol.h"
#include "type.h"
#include "symtab.h"
#include "stringutil.h"
#include "log.h"

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
  if (fn->retType->symbol) {
  fprintf(html_file, " : ");
    html_print_symbol(html_file, fn->retType->symbol);
  }
}

static void html_indent(FILE* html_file, int indent) {
  for (int i = 0; i < indent; i++) {
    fprintf(html_file, "&nbsp;&nbsp;&nbsp;");
  }
}

View::View(bool initNumberSymbols) {
  indent = 0;
  numberSymbols = initNumberSymbols;
}

void View::run(ModuleList* moduleList) {
  static int uid = 1;
  char* filename;
  if (!strncmp(args, "html ", 5)) {
    html = copystring(args+5);

  }
  ModuleSymbol* mod = moduleList->first();
  while (mod) {
    if (html) {
      filename = glomstrings(5, "pass", intstring(uid), "_module_", mod->name, ".html");
      fprintf(html_index_file, "&nbsp;&nbsp;<a href=\"%s\">ast&nbsp;%s</a>\n", filename, mod->name);
      html_file = fopen(glomstrings(2, log_dir, filename), "w");
      fprintf(html_file, "<CHPLTAG=\"%s\">\n", html);
      fprintf(html_file, "<HTML>\n");
      fprintf(html_file, "<HEAD>\n");
      fprintf(html_file, "<TITLE> AST for Module %s after Pass %s </TITLE>\n", mod->name, html);
      fprintf(html_file, "<SCRIPT SRC=\"%s/etc/www/mktree.js\" LANGUAGE=\"JavaScript\"></SCRIPT>", 
              system_dir);
      fprintf(html_file, "<LINK REL=\"stylesheet\" HREF=\"%s/etc/www/mktree.css\">", 
              system_dir);
      fprintf(html_file, "</HEAD>\n");
      fprintf(html_file, "<div style=\"text-align: center;\"><big><big><span style=\"font-weight: bold;\">");
      fprintf(html_file, "AST for Module %s after Pass %s <br><br></span></big></big>\n", mod->name, html);
      fprintf(html_file, "<div style=\"text-align: left;\">\n\n");
      fprintf(html_file, "<B>module %s</B>\n", mod->name);
    } else {
      printf("\nMODULE: %s\n", mod->name);
    }
    mod->startTraversal(this);
    if (html) {
      fprintf(html_file, "</HTML>\n");
      fclose(html_file);
    }
    mod = moduleList->next();
  }
  uid++;
  if (html) {
    fprintf(html_index_file, "<BR>\n");
  } else {
    printf("\n\n");
  }
}

void View::preProcessStmt(Stmt* stmt) {
  if (html) {
    if (dynamic_cast<DefStmt*>(stmt) ||
        typeid(ExprStmt) == typeid(*stmt)) {
      return;
    }
    fprintf(html_file, "<BR>");
    html_indent(html_file, indent);
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
    if (dynamic_cast<DefStmt*>(stmt) ||
        typeid(ExprStmt) == typeid(*stmt)) {
      return;
    }
    if (dynamic_cast<BlockStmt*>(stmt)) {
      fprintf(html_file, "<BR>");
      indent -= 2;
      html_indent(html_file, indent);
      fprintf(html_file, "}");
      return;
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
    if (dynamic_cast<MemberAccess*>(expr)) {
      return;
    }
    fprintf(html_file, "<BR>\n");
    html_indent(html_file, indent);
    if (DefExpr* e = dynamic_cast<DefExpr*>(expr)) {
      if (FnSymbol* fn = dynamic_cast<FnSymbol*>(e->sym)) {
        fprintf(html_file, "<UL CLASS =\"mktree\">\n<LI>");
        fprintf(html_file, "<CHPLTAG=\"FN%d\">\n", fn->id);
        fprintf(html_file, "<B>function ");
        html_print_fnsymbol(html_file, fn);
        fprintf(html_file, "</B><UL>\n");
      } else if (dynamic_cast<TypeSymbol*>(e->sym) &&
                 dynamic_cast<StructuralType*>(e->sym->type)) {
        fprintf(html_file, "<UL CLASS =\"mktree\">\n");
        fprintf(html_file, "<LI><B>type ");
        html_print_symbol(html_file, e->sym);
        fprintf(html_file, "</B><UL>\n");
      } else if (dynamic_cast<TypeSymbol*>(e->sym)) {
        fprintf(html_file, "<B>type ");
        html_print_symbol(html_file, e->sym);
        fprintf(html_file, "</B>");
      } else if (dynamic_cast<VarSymbol*>(e->sym)) {
        fprintf(html_file, "<B>var ");
        html_print_symbol(html_file, e->sym);
        fprintf(html_file, "</B>");
      } else {
        fprintf(html_file, "(%s ", astTypeName[expr->astType]);
        html_print_symbol(html_file, e->sym);
      }
    } else if (Literal* e = dynamic_cast<Literal*>(expr)) {
      fprintf(html_file, "<FONT COLOR=\"lightblue\">'%s'</FONT>", e->str);
    } else if (Variable* e = dynamic_cast<Variable*>(expr)) {
      html_print_symbol(html_file, e->var);
    } else if (AssignOp* e = dynamic_cast<AssignOp*>(expr)) {
      fprintf(html_file, "(%s", cGetsOp[e->type]);
    } else if (BinOp* e = dynamic_cast<BinOp*>(expr)) {
      fprintf(html_file, "(%s", cBinOp[e->type]);
    } else if (UnOp* e = dynamic_cast<UnOp*>(expr)) {
      fprintf(html_file, "(%s", cUnOp[e->type]);
    } else {
      fprintf(html_file, "(%s", astTypeName[expr->astType]);
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
      return;
    } else if (DefExpr* e = dynamic_cast<DefExpr*>(expr)) {
      if (dynamic_cast<FnSymbol*>(e->sym) || 
          (dynamic_cast<TypeSymbol*>(e->sym) &&
           dynamic_cast<StructuralType*>(e->sym->type))) {
        fprintf(html_file, "</UL>\n");
        if (FnSymbol* fn = dynamic_cast<FnSymbol*>(e->sym)) {
          fprintf(html_file, "<CHPLTAG=\"FN%d\">\n", fn->id);
        }
        fprintf(html_file, "</UL>\n");
      }
    } else if (dynamic_cast<Literal*>(expr)) {
    } else if (dynamic_cast<Variable*>(expr)) {
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
