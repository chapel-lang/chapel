#include "constructComplexLiterals.h"
#include "expr.h"
#include "symbol.h"
#include "stmt.h"
#include "symtab.h"

void ConstructComplexLiterals::postProcessExpr(Expr* expr) {
  if (ComplexLiteral* complexLiteral = dynamic_cast<ComplexLiteral*>(expr)) {
    complexLiteral->replace(new ParenOpExpr(new Variable(new UnresolvedSymbol("complex")), NULL));
  }
}
