#ifndef _HTMLVIEW_H_
#define _HTMLVIEW_H_

#include "traversal.h"
#include "baseAST.h"

class HtmlView : public Traversal {
 public:
  int indent;
  char* html;
  FILE* html_file;
  char html_string[2048];
  int pos;
  int show_analysis_info;
  virtual void run(ModuleList* moduleList);
  virtual void preProcessStmt(Stmt* stmt);
  virtual void postProcessStmt(Stmt* stmt);
  virtual void preProcessExpr(Expr* expr);
  virtual void postProcessExpr(Expr* expr);
  void write(char* fmt, ...);
  void output();
  void html_print_symbol(Symbol* sym, bool def);
  void html_print_fnsymbol(FnSymbol* fn);
  void html_indent(int indent);

  HtmlView(int ashow_analysis_info = 0);
};

#endif
