#include "expandClassWiths.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"

void ExpandClassWiths::preProcessStmt(Stmt* stmt) {
  if (WithStmt* with = dynamic_cast<WithStmt*>(stmt)) {
    if (TypeSymbol* symType = dynamic_cast<TypeSymbol*>(with->parentSymbol)) {
      if (ClassType* class_type = dynamic_cast<ClassType*>(symType->type)) {
	Stmt* with_declarations = with->getClass()->declarationList->copyList(true);
	class_type->addDeclarations(with_declarations);
	return;
      }
    }
    USR_FATAL(stmt, "With statement is not in a class type definition");
  }
}
