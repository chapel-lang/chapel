#include "constructComplexLiterals.h"
#include "expr.h"
#include "symbol.h"
#include "stmt.h"
#include "symtab.h"

void ConstructComplexLiterals::postProcessExpr(Expr* expr) {
  if (ComplexLiteral* complexLiteral = dynamic_cast<ComplexLiteral*>(expr)) {
    FloatLiteral* realPart = new FloatLiteral(complexLiteral->realStr,
                                              complexLiteral->realVal);
    // remove i, why is this here anyway?  --SJD
    complexLiteral->str[strlen(complexLiteral->str)-1] = '\0';
    FloatLiteral* imagPart = new FloatLiteral(complexLiteral->str,
                                              complexLiteral->imagVal);
    realPart->append(imagPart);
    complexLiteral->replace(new ParenOpExpr(new Variable(new UnresolvedSymbol("complex")), realPart));
  }
}
