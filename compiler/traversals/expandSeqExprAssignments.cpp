#include "expandSeqExprAssignments.h"
#include "stmt.h"
#include "expr.h"
#include "type.h"
#include "symtab.h"
#include "stringutil.h"

void ExpandSeqExprAssignments::postProcessStmt(Stmt* stmt) {
  ExprStmt* assign_stmt = dynamic_cast<ExprStmt*>(stmt);

  if (!assign_stmt) {
    return;
  }

  AssignOp* assign_expr = dynamic_cast<AssignOp*>(assign_stmt->expr);

  if (!assign_expr) {
    return;
  }

  SeqExpr* seq_expr = dynamic_cast<SeqExpr*>(assign_expr->right);

  if (!seq_expr) {
    return;
  }

  for (Expr* tmp = seq_expr->exprls; tmp; tmp = nextLink(Expr, tmp)) {
    Expr* args = assign_expr->left->copy();
    args->append(tmp->copy());



    Expr* seq_append =
      new ParenOpExpr(
                 new MemberAccess(assign_expr->left->copy(), 
                                  new UnresolvedSymbol("append")), tmp->copy());

    ExprStmt* new_stmt = new ExprStmt(seq_append);
    stmt->insertBefore(new_stmt);
  }
  stmt->extract();
}
