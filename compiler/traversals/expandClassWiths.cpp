#include "expandClassWiths.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"

void ExpandClassWiths::preProcessStmt(Stmt* stmt) {
  if (WithStmt* with = dynamic_cast<WithStmt*>(stmt)) {
    if (TypeSymbol* symType = dynamic_cast<TypeSymbol*>(with->parentSymbol)) {
      if (ClassType* class_type = dynamic_cast<ClassType*>(symType->type)) {
	VarSymbol* field = NULL;
	Stmt* tmp = with;
	while (tmp) {
	  if (VarDefStmt* var_def_stmt = dynamic_cast<VarDefStmt*>(tmp)) {
	    field = var_def_stmt->var;
	    break;
	  }
	  tmp = nextLink(Stmt, tmp);
	}
	Stmt* with_declarations = with->getClass()->declarationList->copyList(true);
	class_type->addDeclarations(with_declarations, with, field);
	return;
      }
    }
    USR_FATAL(stmt, "With statement is not in a class type definition");
  }
}
