#include "processDefs.h"
#include "symtab.h"
#include "symscope.h"
#include "stmt.h"
#include "expr.h"
#include "type.h"
#include "stringutil.h"
#include "insertVariableInitializations.h"


static void insert_default_init(Stmt* stmt, VarSymbol* var) {
  Type* type = var->type;

  //
  // SJD: This code squelches inserting the default init statement for
  // classes within their own constructor.  This avoids generating
  // code like the following which crashed my whole system:
  //
  //   ClassName _construct_ClassName(void) {
  //     ClassName this;
  //     this = _construct_ClassName();  <=== Squelched
  //
  if (StructuralType* class_type = dynamic_cast<StructuralType*>(type)) {
    if (DefStmt* def_stmt = dynamic_cast<DefStmt*>(class_type->constructor)) {
      if (DefExpr* def_expr = dynamic_cast<DefExpr*>(def_stmt->defExprList)) {
        if (def_expr->sym == stmt->parentSymbol) {
          return;
        }
      }
    }
  }

  if (var->consClass != VAR_VAR) {
    return;
  }

  ExprStmt* assign_stmt = new ExprStmt(new VarInitExpr(var));
  insert_default_init_stmt(var, assign_stmt);
}

/*
static void insert_user_init(Stmt* stmt, VarSymbol* var) {
  if (var->init) {
    AssignOp* assign_expr = new AssignOp(GETS_NORM,
                                         new Variable(var),
                                         var->init->copy());
    ExprStmt* assign_stmt = new ExprStmt(assign_expr);
    stmt->insertAfter(assign_stmt);
  }
}
*/

static void insert_init(Stmt* stmt, VarSymbol* var) {
  insert_default_init(stmt, var);
  //insert_user_init(stmt, var); -- done later
}


ProcessDefs::ProcessDefs() {
  whichModules = MODULES_USER;
}


void ProcessDefs::postProcessStmt(Stmt* stmt) {
  DefStmt* def_stmt = dynamic_cast<DefStmt*>(stmt);

  if (!def_stmt) {
    return;
  }

  if (dynamic_cast<TypeSymbol*>(def_stmt->parentSymbol)) {
    return;
  }

  if (!def_stmt->varDef()) {
    return;
  }

  DefExpr* def_expr = dynamic_cast<DefExpr*>(def_stmt->defExprList);
  while (def_expr) {
    VarSymbol* var = dynamic_cast<VarSymbol*>(def_expr->sym);
    while (var) {
      insert_init(stmt, var);
      var = nextLink(VarSymbol, var);
    }
    def_expr = nextLink(DefExpr, def_expr);
  }
}
