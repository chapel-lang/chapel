#include "removeDeadSymbols.h"
#include "analysis.h"
#include "stmt.h"

void RemoveDeadSymbols::postProcessStmt(Stmt* stmt) {
  if (TypeDefStmt* def_stmt = dynamic_cast<TypeDefStmt*>(stmt)) {
    if (!type_is_used(def_stmt->type_sym)) {
      def_stmt->extract();
    }
  }
}
