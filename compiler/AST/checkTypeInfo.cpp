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
  if (expr->ainfo == NULL) {
    expr->printLoc(stderr);
    fprintf(stderr, " ainfo is NULL for:\n");
    expr->print(stderr);
    fprintf(stderr, "\n\n");
  } else if (type_info(expr->ainfo) == NULL) {
    expr->printLoc(stderr);
    fprintf(stderr, " type_info() is NULL for:\n");
    expr->print(stderr);
    fprintf(stderr, "\n\n");
  }
}
