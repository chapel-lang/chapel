#include "checkIDs.h"
#include "chplalloc.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"

void CheckIDs::preProcessStmt(Stmt* stmt) {
  astlist[stmt->id] = stmt;
}


void CheckIDs::preProcessExpr(Expr* expr) {
  astlist[expr->id] = expr;
}


void CheckIDs::preProcessSymbol(Symbol* symbol) {
  astlist[symbol->id] = symbol;
}


void CheckIDs::preProcessType(Type* type) {
  astlist[type->id] = type;
}


void CheckIDs::run(Stmt* program) {
  long numids = BaseAST::getNumIDs();

  astlist = (BaseAST**)MALLOC(numids * sizeof(BaseAST*));
  for (long i=0; i<numids; i++) {
    astlist[i] = NULL;
  }

  Traversal::run(program);

  for (long i=0; i<numids; i++) {
    if (!astlist[i]) {
      fprintf(stderr, "Traversal didn't reach node %d:\n\n", i);
    } else {
      fprintf(stderr, "node %d:\n", i);
      astlist[i]->println(stderr);
      fprintf(stderr, "\n\n\n");
    }
  }
}
