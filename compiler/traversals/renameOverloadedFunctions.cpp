#include "renameOverloadedFunctions.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"

void RenameOverloadedFunctions::preProcessExpr(Expr* expr) {
  static int uid = 1; // Unique ID for user-overloaded functions

  if (DefExpr* def_expr = dynamic_cast<DefExpr*>(expr)) {
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(def_expr->sym)) {
      if (fn->overload) {
	FnSymbol* tmp = fn;

	while (tmp) {
	  if (!strstr(tmp->cname, "_user_overload_")) {
	    tmp->cname =
	      glomstrings(3, tmp->cname, "_user_overload_", intstring(uid++));
	  }
	  tmp = tmp->overload;
	}
      }
    }
  }
}
