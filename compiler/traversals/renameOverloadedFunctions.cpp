#include "renameOverloadedFunctions.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"

void RenameOverloadedFunctions::preProcessStmt(Stmt* stmt) {
  static int uid = 1; // Unique ID for user-overloaded functions

  if (DefStmt* def_stmt = dynamic_cast<DefStmt*>(stmt)) {
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(def_stmt->def_sym)) {
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
