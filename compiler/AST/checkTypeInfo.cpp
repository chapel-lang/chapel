#include <stdio.h>
#include "analysis.h"
#include "checkTypeInfo.h"
#include "expr.h"
#include "fa.h"
#include "stmt.h"


CheckTypeInfo::CheckTypeInfo(void) :
  Traversal("checkTypeInfo")
{}


void CheckTypeInfo::preProcessExpr(Expr* expr) {
  if (expr->ainfo) {
    Type* exprType = type_info(expr);
    if (exprType == NULL) {
      expr->printLoc(stderr);
      fprintf(stderr, " type_info() is NULL for:\n");
      expr->println(stderr);
      fprintf(stderr, "\n");
    } else {
      fprintf(stderr, "type of: ");
      expr->print(stderr);
      fprintf(stderr, " is: ");
      exprType->println(stderr);
      fprintf(stderr, "\n");
    }
  }
}
