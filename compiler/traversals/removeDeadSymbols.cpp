#include "removeDeadSymbols.h"
#include "analysis.h"
#include "stmt.h"

void RemoveDeadSymbols::postProcessStmt(Stmt* stmt) {
  if (DefStmt* def_stmt = dynamic_cast<DefStmt*>(stmt)) {
    if (TypeSymbol* type_sym = dynamic_cast<TypeSymbol*>(def_stmt->def_sym)) {
      if (!type_is_used(type_sym)) {
	def_stmt->extract();
      }
    }
  }
}
