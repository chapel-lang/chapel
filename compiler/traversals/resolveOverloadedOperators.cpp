#include "resolveOverloadedOperators.h"
#include "symtab.h"
#include "symscope.h"
#include "symbol.h"
#include "type.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"

void ResolveOverloadedOperators::postProcessExpr(Expr* expr) {
  BinOp* op = dynamic_cast<BinOp*>(expr);

  if (!op) {
    return;
  }
}
