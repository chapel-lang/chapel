#include "constructComplexLiterals.h"
#include "expr.h"
#include "symbol.h"
#include "stmt.h"
#include "symtab.h"

void ConstructComplexLiterals::postProcessExpr(Expr* expr) {
  if (ComplexLiteral* complexLiteral = dynamic_cast<ComplexLiteral*>(expr)) {
    AList<Expr>* argList = new AList<Expr>();
    FloatLiteral* realPart = new FloatLiteral(complexLiteral->realStr,
                                              complexLiteral->realVal);
    argList->add(realPart);
    // remove i, why is this here anyway?  --SJD
    complexLiteral->str[strlen(complexLiteral->str)-1] = '\0';
    FloatLiteral* imagPart = new FloatLiteral(complexLiteral->str,
                                              complexLiteral->imagVal);
    argList->add(imagPart);
    complexLiteral->replace(new ParenOpExpr(new Variable(new UnresolvedSymbol("complex")), argList));
  }
}
