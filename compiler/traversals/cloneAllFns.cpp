#include <typeinfo>
#include "cloneAllFns.h"
#include "stmt.h"

void CloneAllFns::preProcessStmt(Stmt* stmt) {
  if (typeid(*stmt) == typeid(FnDefStmt)) {
    FnDefStmt* origStmt = (FnDefStmt*)stmt;
    if (origStmt->fn->name[0] != '_') {
      /*** [12/9/2004] SJD: Removed this function;
	   what is this pass all about anyway?
	   origStmt->clone(); **/
    }
  }
}
