#include <typeinfo>
#include "insertAnonymousTypes.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"
#include "symtab.h"
#include "stringutil.h"
#include "fixup.h"


void InsertAnonymousTypes::postProcessExpr(Expr* expr) {
  if (DefExpr* def = dynamic_cast<DefExpr*>(expr)) {
    if (!strncmp("_anon_record", def->sym->name, 12)) {
      if (ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(def->parentStmt)) {
        def->replace(new SymExpr(new UnresolvedSymbol(def->sym->name)));
        exprStmt->insertBefore(new ExprStmt(def));
      }
    }
  }
}
