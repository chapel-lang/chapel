#include "destructureTupleAssignments.h"
#include "stmt.h"
#include "expr.h"
#include "type.h"
#include "stringutil.h"

void DestructureTupleAssignments::postProcessStmt(Stmt* stmt) {
  ExprStmt* assign_stmt = dynamic_cast<ExprStmt*>(stmt);

  if (!assign_stmt) {
    return;
  }

  AssignOp* assign_expr = dynamic_cast<AssignOp*>(assign_stmt->expr);

  if (!assign_expr) {
    return;
  }

  TupleType* left_type = dynamic_cast<TupleType*>(assign_expr->left->typeInfo());
  TupleType* right_type = dynamic_cast<TupleType*>(assign_expr->right->typeInfo());

  Tuple* left_tuple = dynamic_cast<Tuple*>(assign_expr->left);
  Tuple* right_tuple = dynamic_cast<Tuple*>(assign_expr->right);

  if (!left_type && !left_tuple && !right_type && !right_tuple) {
    return;
  }

  int left_n =
    (left_type) 
    ? left_n = left_type->components.n 
    : left_n = left_tuple->exprs->length();

  int right_n =
    (right_type) 
    ? right_n = right_type->components.n 
    : right_n = right_tuple->exprs->length();

  if (left_n != right_n) {
    INT_FATAL(stmt, "Non-matching tuple assign should be caught earlier");
  }

  if (assign_expr->type != GETS_NORM) {
    INT_FATAL(stmt, "Non-standard tuple assign should be caught earlier");
  }

  for (int i = 1; i <= left_n; i++) {
    TupleSelect* new_left =
      new TupleSelect(assign_expr->left->copy(), new IntLiteral(intstring(i), i));
    TupleSelect* new_right =
      new TupleSelect(assign_expr->right->copy(), new IntLiteral(intstring(i), i));
    AssignOp* new_assign_expr =
      new AssignOp(assign_expr->type, new_left, new_right);
    ExprStmt* new_assign_stmt = new ExprStmt(new_assign_expr);
    stmt->insertBefore(new_assign_stmt);
  }
  stmt->extract();
}
