#include <typeinfo>
#include "view.h"
#include "stmt.h"
#include "expr.h"
#include "symbol.h"
#include "type.h"
#include "symtab.h"
#include "stringutil.h"

View::View() {
  indent = 0;
}

void View::preProcessStmt(Stmt* stmt) {
  printf("\n");
  for (int i = 0; i < indent; i++) {
    printf(" ");
  }
  printf("(%s", astTypeName[stmt->astType]);
  indent += 2;
}

void View::postProcessStmt(Stmt* stmt) {
  printf(")");
  indent -= 2;
}

void View::preProcessExpr(Expr* expr) {
  printf("\n");
  for (int i = 0; i < indent; i++) {
    printf(" ");
  }
  printf("(%s", astTypeName[expr->astType]);
  indent += 2;
}

void View::postProcessExpr(Expr* expr) {
  printf(")");
  indent -= 2;
}

void View::preProcessSymbol(Symbol* sym) {
  printf(" '%s'", sym->name);
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
