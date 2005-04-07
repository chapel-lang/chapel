#include "insertTemps.h"
#include "symtab.h"
#include "symscope.h"
#include "symbol.h"
#include "type.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"

void InsertTemps::postProcessExpr(Expr* expr) {
  static int uid = 1;

  if (SeqExpr* seq_expr = dynamic_cast<SeqExpr*>(expr)) {

    /**
     ** No temporary if the expression is top-level in a DefStmt
     **/
    if (seq_expr->parent == NULL && dynamic_cast<DefStmt*>(seq_expr->stmt)) {
      return;
    }

    char* temp_name = glomstrings(2, "_seq_temp_", intstring(uid++));
    Type* temp_type = dtUnknown;

    if (!analyzeAST) {
      temp_type = seq_expr->typeInfo();
    }

    DefStmt* def_stmt = Symboltable::defineSingleVarDefStmt(temp_name,
                                                            temp_type,
                                                            seq_expr->copy(),
                                                            VAR_NORMAL,
                                                            VAR_VAR);

    expr->stmt->insertBefore(def_stmt);

    expr->replace(new Variable(def_stmt->varDef()));
  }
}
