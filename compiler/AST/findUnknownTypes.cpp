#include "expr.h"
#include "findUnknownTypes.h"
#include "stmt.h"

FindUnknownTypes::FindUnknownTypes(void) :
  Traversal()
{}


void FindUnknownTypes::preProcessStmt(Stmt* stmt) {
  //  fprintf(stderr, "STMT:\n");
  //  stmt->print(stderr);
}

void FindUnknownTypes::postProcessStmt(Stmt* stmt) {
  //  fprintf(stderr, "STMT\n\n\n");
}


void FindUnknownTypes::preProcessSymbol(Symbol* sym) {
  if (sym->type == dtUnknown) {
    fprintf(stderr, "Found symbol with unknown type: %s (%s:%d)\n",
	    sym->name, sym->filename, sym->lineno);
  }
}


void FindUnknownTypes::preProcessExpr(Expr* expr) {
  /*
  if (expr->typeInfo() == dtUnknown) {
    fprintf(stderr, "Found expression with unknown type:\n");
    expr->print(stderr);
    fprintf(stderr, "\n\n");
  }
  */
}




void PrintStmts::preProcessStmt(Stmt* stmt) {
  fprintf(stdout, "STMT:\n");
  stmt->print(stdout);
  fprintf(stdout, "\n\n\n");
}
