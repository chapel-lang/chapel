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

  SeqType* seq_type = dynamic_cast<SeqType*>(assign_expr->left->typeInfo());

  if (!seq_type) {
    INT_FATAL(stmt, "Sequence type not found in assignment of sequence literal");
  }

  for (Expr* tmp = seq_expr->exprls->first(); 
       tmp; 
       tmp = seq_expr->exprls->next()) {

    FnSymbol* fn = NULL;
    forv_Vec(FnSymbol, method, seq_type->methods) {
      if (!strcmp(method->name, "append")) {
        fn = method;
      }
    }

    if (!fn) {
      INT_FATAL(seq_type, "Cannot find append function in sequence type");
    }

    AList<Expr>* args = new AList<Expr>(assign_expr->left->copy());
    args->insertAtTail(tmp->copy());
    Expr* append_expr = new FnCall(new Variable(fn), args);
    stmt->insertBefore(new ExprStmt(append_expr));
  }
  stmt->remove();
}
