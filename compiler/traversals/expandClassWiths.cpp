#include "expandClassWiths.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"

void ExpandClassWiths::preProcessExpr(Expr* expr) {
  if (WithExpr* with = dynamic_cast<WithExpr*>(expr)) {
    if (TypeSymbol* symType = dynamic_cast<TypeSymbol*>(with->parentSymbol)) {
      if (ClassType* class_type = dynamic_cast<ClassType*>(symType->definition)) {
        AList<Stmt>* with_declarations = with->getStruct()->declarationList->copy(true);
        class_type->addDeclarations(with_declarations, with->parentStmt);
        return;
      }
    }
    USR_FATAL(expr, "Cannot find ClassType in WithExpr");
  }
}
