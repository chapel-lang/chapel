#include "checkBackLinks.h"
#include "expr.h"
#include "misc.h"
#include "stmt.h"
#include "symbol.h"

static bool error;

void CheckBackLinks::preProcessStmt(Stmt* stmt) {
  if (stmt->parentFn == NULL || stmt->parentFn->isNull()) {
#ifdef DOESNT_WORK_YET
    if (!stmt->canLiveAtFileScope()) {
      fprintf(stderr, "Stmt missing parentFn pointer:\n");
      stmt->println(stderr);
      fprintf(stderr, "\n");
      error = true;
    }
#endif
  }
}


void CheckBackLinks::preProcessExpr(Expr* expr) {
  bool parentOK = false;

  if (expr->stmt == NULL || expr->stmt->isNull()) {
    fprintf(stderr, "Expr missing stmt pointer:\n");
    expr->println(stderr);
    fprintf(stderr, "\n");
    error = true;
  } else {
    // if this is a top-level Expr, it's OK that the parent is nil
    parentOK = expr->stmt->topLevelExpr(expr);
  }
  if (expr->parent == NULL || expr->isNull()) {
    if (!parentOK) {
      fprintf(stderr, "Expr missing parent pointer:\n");
      expr->println(stderr);
      fprintf(stderr, "\n");
      error = true;
    }
  }
}


void CheckBackLinks::preProcessSymbol(Symbol* symbol) {
  if (symbol->scope == NULL) {
    fprintf(stderr, "Symbol missing scope pointer:\n");
    symbol->println(stderr);
    fprintf(stderr, "\n");
    error = true;
  }
}


void CheckBackLinks::run(Stmt* program) {
  error = false;
  Traversal::run(program);
  if (error) {
    INT_WARNING("Problems were found in back links");
  }
}
