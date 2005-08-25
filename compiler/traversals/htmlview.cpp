#include <stdarg.h>
#include <typeinfo>
#include "htmlview.h"
#include "stmt.h"
#include "expr.h"
#include "symbol.h"
#include "type.h"
#include "symtab.h"
#include "stringutil.h"
#include "log.h"

HtmlView::HtmlView(int ashow_analysis_info) 
  : show_analysis_info(ashow_analysis_info)
{ }

char* strrstr(const char* s, const char* s2) {
  char* result = strstr(s, s2);
  char* next = result;
  while (next) {
    result = next;
    next = strstr(result+1, s2);
  }
  return result;
}

void HtmlView::write(char* fmt, ...) {
  va_list args;

  va_start(args, fmt);
  vsprintf(html_string+strlen(html_string), fmt, args);
  va_end(args);
  if (strlen(html_string) > 2048-128) {
    INT_FATAL("HtmlView string too small");
  }
}

void HtmlView::output() {
  fprintf(html_file, "%s", html_string);
  html_string[0] = '\0';
}

void HtmlView::run(Vec<ModuleSymbol*>* modules) {
  static int uid = 1;
  char* filename;
  if (!strncmp(args, "html ", 5)) {
    html = copystring(args+5);

  }
  forv_Vec(ModuleSymbol, mod, *modules) {
    filename = glomstrings(5, "pass", intstring(uid), "_module_", mod->name, ".html");
    fprintf(html_index_file, "&nbsp;&nbsp;<a href=\"%s\">%s</a>\n", filename, mod->name);
    html_file = fopen(glomstrings(2, log_dir, filename), "w");
    write("<CHPLTAG=\"%s\">\n", html);
    write("<HTML>\n");
    write("<HEAD>\n");
    write("<TITLE> AST for Module %s after Pass %s </TITLE>\n", mod->name, html);
    write("<SCRIPT SRC=\"%s/etc/www/mktree.js\" LANGUAGE=\"JavaScript\"></SCRIPT>", 
          system_dir);
    write("<LINK REL=\"stylesheet\" HREF=\"%s/etc/www/mktree.css\">", 
          system_dir);
    write("</HEAD>\n");
    write("<div style=\"text-align: center;\"><big><big><span style=\"font-weight: bold;\">");
    write("AST for Module %s after Pass %s <br><br></span></big></big>\n", mod->name, html);
    write("<div style=\"text-align: left;\">\n\n");
    write("<B>module %s</B>\n", mod->name);
    output();

    mod->startTraversal(this);

    write("</HTML>\n");
    output();
    fclose(html_file);
  }
  uid++;
}

void HtmlView::preProcessStmt(Stmt* stmt) {
  if (typeid(ExprStmt) == typeid(*stmt)) {
    return;
  }
  write("<DL>\n");
  if (dynamic_cast<BlockStmt*>(stmt)) {
    write("{");
  } else if (dynamic_cast<WhileLoopStmt*>(stmt)) {
    write("<B>while</B> ");
  } else if (ForLoopStmt* s = dynamic_cast<ForLoopStmt*>(stmt)) {
    switch (s->forLoopStmtTag) {
    case FORLOOPSTMT_FOR:
      write("<B>for</B> ");
      break;
    case FORLOOPSTMT_ORDEREDFORALL:
      write("<B>ordered forall</B> ");
      break;
    case FORLOOPSTMT_FORALL:
      write("<B>forall</B> ");
      break;
    }
  } else if (dynamic_cast<ReturnStmt*>(stmt)) {
    write("<B>return</B> ");
  } else {
    write("%s", astTypeName[stmt->astType]);
  }
}

void HtmlView::postProcessStmt(Stmt* stmt) {
  if (typeid(ExprStmt) == typeid(*stmt)) {
    output();
    return;
  }
  if (dynamic_cast<BlockStmt*>(stmt)) {
    write("}");
  }
  write("</DL>\n");
  output();
}

ClassType *
structuralTypeSymbol(Symbol *s) {
  if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(s))
    if (ClassType *st = dynamic_cast<ClassType*>(ts->definition))
      return st;
  return NULL;
}

void HtmlView::preProcessExpr(Expr* expr) {
  if (dynamic_cast<MemberAccess*>(expr)) {
    return;
  }
  write("<DL>\n");
  if (DefExpr* e = dynamic_cast<DefExpr*>(expr)) {
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(e->sym)) {
      write("<UL CLASS =\"mktree\">\n<LI>");
      write("<CHPLTAG=\"FN%d\">\n", fn->id);
      for (BaseAST* tmp = fn->copyFrom; tmp; tmp = tmp->copyFrom) {
        write("<CHPLTAG=\"FN%d\">\n", tmp->id);
      }
      write("<B>function ");
      html_print_fnsymbol(fn);
      write("</B><UL>\n");
    } else if (structuralTypeSymbol(e->sym)) {
      write("<UL CLASS =\"mktree\">\n");
      write("<LI><B>type ");
      html_print_symbol(e->sym, true);
      write("</B><UL>\n");
    } else if (dynamic_cast<TypeSymbol*>(e->sym)) {
      write("<B>type</B> ");
      html_print_symbol(e->sym, true);
    } else if (dynamic_cast<VarSymbol*>(e->sym)) {
      write("<B>var</B> ");
      html_print_symbol(e->sym, true);
    } else if (dynamic_cast<ArgSymbol*>(e->sym)) {
      write("<B>arg</B> ");
      html_print_symbol(e->sym, true);
    } else {
      write("<B>def</B> ");
      html_print_symbol(e->sym, true);
    }
  } else if (Literal* e = dynamic_cast<Literal*>(expr)) {
    if (dynamic_cast<StringLiteral*>(e)) {
      write("<FONT COLOR=\"lightblue\">'%s'</FONT>", e->str);
    } else {
      write("<FONT COLOR=\"lightblue\">%s</FONT>", e->str);
    }
  } else if (SymExpr* e = dynamic_cast<SymExpr*>(expr)) {
    html_print_symbol(e->var, false);
  } else if (NamedExpr* e = dynamic_cast<NamedExpr*>(expr)) {
    write("(%s = ", e->name);
  } else if (CallExpr* e = dynamic_cast<CallExpr*>(expr)) {
    write("(");
    if (e->opTag == OP_NONE) {
      write("<B>call</B> ");
    }
  } else {
    write("(%s", astTypeName[expr->astType]);
  }
}

void HtmlView::postProcessExpr(Expr* expr) {
  if (MemberAccess* e = dynamic_cast<MemberAccess*>(expr)) {
    write(".");
    html_print_symbol(e->member, false);
    return;
  } else if (DefExpr* e = dynamic_cast<DefExpr*>(expr)) {
    if (dynamic_cast<FnSymbol*>(e->sym) || 
        (dynamic_cast<TypeSymbol*>(e->sym) &&
         dynamic_cast<ClassType*>(e->sym->type))) {
      write("</UL>\n");
      if (FnSymbol* fn = dynamic_cast<FnSymbol*>(e->sym)) {
        for (BaseAST* tmp = fn->copyFrom; tmp; tmp = tmp->copyFrom) {
          write("<CHPLTAG=\"FN%d\">\n", tmp->id);
        }
        write("<CHPLTAG=\"FN%d\">\n", fn->id);
      }
      write("</UL>\n");
    }
  } else if (dynamic_cast<Literal*>(expr)) {
  } else if (dynamic_cast<SymExpr*>(expr)) {
  } else {
    write(")");
  }
  if (show_analysis_info && expr->parentSymbol) {
    Vec<FnSymbol *> fns;
    call_info(expr, fns, CALL_INFO_FIND_ALL);
    if (fns.n) {
      write(" calls: ");
      for (int i = 0; i < fns.n; i++) {
        if (i > 0) write(" ");
        html_print_symbol(fns.v[i], false);
      }
    }
  }
  char* open = strrstr(html_string, "<DL>");
  if (expr->parentExpr && open && (strlen(html_string) - (open - html_string) < 128)) {
    memmove(open, open+4, strlen(open)+4);
  } else {
    write("</DL>\n");
  }
  if (strlen(html_string) > 512) {
    output();
  }
}

void HtmlView::html_print_symbol(Symbol* sym, bool def) {
  if (!sym->isUnresolved) {
    if (def)
      write("<A NAME=\"SYM%d\">", sym->id);
    else
      write("<A HREF=\"#SYM%d\">", sym->id);
  }
  if (dynamic_cast<FnSymbol*>(sym)) {
    write("<FONT COLOR=\"blue\">");
  } else if (dynamic_cast<TypeSymbol*>(sym)) {
    write("<FONT COLOR=\"green\">");
  } else {
    write("<FONT COLOR=\"red\">");
  }
  write("%s", sym->name);
  write("</FONT>");
  if (!sym->isUnresolved) {
    write("<FONT COLOR=\"grey\">[%ld]</FONT>", sym->id);
  }
  write("</A>");
  if (def &&
      !dynamic_cast<TypeSymbol*>(sym) &&
      sym->type &&
      sym->type->symbol &&
      sym->type != dtUnknown) {
    write(":");
    html_print_symbol(sym->type->symbol, false);
  }
  if (show_analysis_info) {
    Vec<Expr *> constants;
    constant_info(sym, constants);
    if (constants.n) {
      if (constants.n > 1)
        write(" constants: { ");
      else
        write(" constant: ");
      for (int i = 0; i < constants.n; i++) {
        if (i > 0) write(", ");
        if (Literal *l = dynamic_cast<Literal *>(constants.v[i]))
          write("<FONT COLOR=\"lightblue\">'%s'</FONT>", l->str);
        else if (SymExpr* v = dynamic_cast<SymExpr* >(constants.v[i]))          
          html_print_symbol(v->var, false);
        else 
          INT_FATAL("bad constant");
      }
      if (constants.n > 1)
        write(" }");
    }
  }
}

void HtmlView::html_print_fnsymbol(FnSymbol* fn) {
  if (fn->typeBinding) {
    html_print_symbol(fn->typeBinding, false);
    write(" . ");
  }
  html_print_symbol(fn, true);
  write(" ( ");
  bool first = true;
  for_alist(DefExpr, arg, fn->formals) {
    if (!first) {
      write(" , ");
    } else {
      first = false;
    }
    html_print_symbol(arg->sym, true);
  }
  write(" ) ");
  if (fn->retType->symbol) {
    write(" : ");
    html_print_symbol(fn->retType->symbol, false);
  }
  output();
}
