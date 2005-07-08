#include "preAnalysisHacks.h"
#include "stmt.h"
#include "expr.h"
#include "symbol.h"
#include "type.h"

void PreAnalysisHacks::postProcessStmt(Stmt* stmt) {
  if (ForLoopStmt* loop = dynamic_cast<ForLoopStmt*>(stmt)) {
    if (dynamic_cast<Tuple*>(loop->domain) ||
        dynamic_cast<DomainType*>(loop->domain->typeInfo())) {
      return;
    }
    loop->domain->replace(
      new ParenOpExpr(
        new MemberAccess(
          loop->domain->copy(),
          new UnresolvedSymbol("_forall")),
        new AList<Expr>()));
  }
}
