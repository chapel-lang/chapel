#include "removeDeadSymbols.h"
#include "analysis.h"
#include "stmt.h"
#include "expr.h"
#include "../traversals/view.h"

void RemoveDeadSymbols::postProcessStmt(Stmt* stmt) {
  if (DefStmt* def_stmt = dynamic_cast<DefStmt*>(stmt)) {
    if (TypeSymbol* type_sym = def_stmt->typeDef()) {
      if (!type_is_used(type_sym)) {
	def_stmt->extract();
      }
    }
    if (FnSymbol* fn_sym = def_stmt->fnDef()) {
      if (!function_is_used(fn_sym)) {
	def_stmt->extract();
	//print_view(def_stmt);
      }
    }
  }
}
