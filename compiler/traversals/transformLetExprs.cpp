#include "transformLetExprs.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"
#include "symtab.h"
#include "stringutil.h"
#include "insertFunctionTemps.h"


void TransformLetExprs::postProcessExpr(Expr* expr) {
  static int uid = 1;
  if (LetExpr* letExpr = dynamic_cast<LetExpr*>(expr)) {
    Stmt* stmt = letExpr->parentStmt;
    letExpr->replace(letExpr->innerExpr);
    for_alist(DefExpr, def, letExpr->symDefs) {
      def->sym->cname = glomstrings(3, def->sym->cname, "_let_", intstring(uid++));
      stmt->insertBefore(new ExprStmt(def));
    }
  }
}
