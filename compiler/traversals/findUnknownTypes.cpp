#include "analysis.h"
#include "expr.h"
#include "findUnknownTypes.h"
#include "stmt.h"


FindUnknownTypes::FindUnknownTypes(void) {
  processInternalModules = false;
}


void FindUnknownTypes::preProcessStmt(Stmt* stmt) {
  //  fprintf(stderr, "STMT:\n");
  //  stmt->print(stderr);
}

void FindUnknownTypes::postProcessStmt(Stmt* stmt) {
  //  fprintf(stderr, "STMT\n\n\n");
}


void FindUnknownTypes::preProcessSymbol(Symbol* sym) {
  if (sym->type == dtUnknown) {
    sym->type = type_info(sym);
  }
  FnSymbol* fnSym = dynamic_cast<FnSymbol*>(sym);
  if (fnSym) {
    if (fnSym->retType == dtUnknown) {
      fnSym->retType = return_type_info(fnSym);
      // BLC: this is a mini-hack, as analysis currently returns dtUnknown
      // for things that don't return anything rather than dtVoid
      if (fnSym->retType == dtUnknown) {
	fnSym->retType = dtVoid;
      }
    }
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
