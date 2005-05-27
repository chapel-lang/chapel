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

    if (!analyzeAST) {
      INT_FATAL(expr, "Analysis required for sequences");
    }

    char* temp_name = glomstrings(2, "_seq_temp_", intstring(uid++));
    Type* temp_type = Symboltable::lookup("seq2")->typeInfo();

    Type* elt_type = seq_expr->exprls->representative()->typeInfo();

    if (!elt_type || elt_type == dtUnknown) {
      INT_FATAL(expr, "Sequence literal is of unknown type, not handled");
    }

    Expr* temp_init =
      new ParenOpExpr(new Variable(Symboltable::lookup("seq2")->typeInfo()->symbol),
                      new AList<Expr>(new Variable(elt_type->symbol)));

    DefStmt* def_stmt = Symboltable::defineSingleVarDefStmt(temp_name,
                                                            temp_type,
                                                            temp_init,
                                                            VAR_NORMAL,
                                                            VAR_VAR);

    expr->getStmt()->insertBefore(def_stmt);


    Symbol* seq = def_stmt->defExprls->representative()->sym;

    BinOp* binOp = NULL;
    for (Expr* tmp = seq_expr->exprls->first(); tmp; tmp = seq_expr->exprls->next()) {
      if (binOp) {
        binOp = new BinOp(BINOP_SEQCAT, binOp, tmp->copy());
      } else {
        binOp = new BinOp(BINOP_SEQCAT, new Variable(seq), tmp->copy());
      }
    }

    expr->getStmt()->insertBefore(
      new ExprStmt(new AssignOp(GETS_NORM, new Variable(seq), binOp)));

    expr->replace(new Variable(seq));
  }
}
