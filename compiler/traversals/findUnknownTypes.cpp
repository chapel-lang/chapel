#include "analysis.h"
#include "expr.h"
#include "findUnknownTypes.h"
#include "stmt.h"
#include "../passes/runAnalysis.h"


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


class FindReturn : public Traversal {
 public:
  bool found;
  FindReturn(void);
  void preProcessStmt(Stmt* stmt);
};

FindReturn::FindReturn() {
  found = false;
}

void FindReturn::preProcessStmt(Stmt* stmt) {
  if (dynamic_cast<ReturnStmt*>(stmt)) {
    found = true;
  }
}


void FindUnknownTypes::preProcessSymbol(Symbol* sym) {
  if (sym->type == dtUnknown) {
    sym->type = type_info(sym);
  }
  FnSymbol* fnSym = dynamic_cast<FnSymbol*>(sym);
  if (fnSym) {
    if (fnSym->retType == dtUnknown) {
      if (RunAnalysis::runCount > 0) {
	fnSym->retType = return_type_info(fnSym);
      }
      else {
	FindReturn* traversal = new FindReturn();
	TRAVERSE_LS(fnSym->body, traversal, true);
	if (traversal->found) {
	  INT_FATAL(sym, "Analysis required to determine return type");
	}
	else {
	  fnSym->retType = dtVoid;
	}
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
