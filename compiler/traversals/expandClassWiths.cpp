#include "expandClassWiths.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"

void ExpandClassWiths::preProcessStmt(Stmt* stmt) {
  if (WithStmt* with = dynamic_cast<WithStmt*>(stmt)) {
    if (TypeSymbol* symType = dynamic_cast<TypeSymbol*>(with->parentSymbol)) {
      if (StructuralType* class_type = dynamic_cast<StructuralType*>(symType->type)) {
        AList<Stmt>* with_declarations = with->getStruct()->declarationList->copy(true);
        class_type->addDeclarations(with_declarations, with);
        return;
      }
    }
    USR_FATAL(stmt, "With statement is not in a class type definition");
  }
}
