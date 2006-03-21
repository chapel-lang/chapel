#include "build.h"
#include "baseAST.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "type.h"

FnSymbol* build_if_expr(Expr* e, Expr* e1, Expr* e2) {
  static int uid = 1;
  FnSymbol* fn = new FnSymbol(stringcat("_if_fn", intstring(uid++)));
  fn->retRef = true;
  fn->formals = new AList<DefExpr>();
  fn->addPragma("inline");
  if (e2)
    fn->insertAtTail(new CondStmt(e, new ReturnStmt(e1), new ReturnStmt(e2)));
  else
    USR_FATAL("if-then expressions currently require an else-clause");
  return fn;
}


FnSymbol* build_let_expr(AList<Stmt>* decls, Expr* expr) {
  static int uid = 1;
  FnSymbol* fn = new FnSymbol(stringcat("_let_fn", intstring(uid++)));
  fn->formals = new AList<DefExpr>();
  fn->addPragma("inline");
  fn->insertAtTail(decls);
  fn->insertAtTail(new ReturnStmt(expr));
  return fn;
}


static void build_loop_labels(BlockStmt* body) {
  static int uid = 1;
  body->pre_loop = new LabelSymbol(stringcat("_pre_loop", intstring(uid)));
  body->post_loop = new LabelSymbol(stringcat("_post_loop", intstring(uid)));
  uid++;
}


AList<Stmt>* build_while_do_block(Expr* cond, BlockStmt* body) {
  if (body->blockTag != BLOCK_NORMAL)
    INT_FATAL(body, "Abnormal block passed to build_while_do_block");
  body->blockTag = BLOCK_WHILE_DO;
  build_loop_labels(body);
  AList<Stmt>* stmts = new AList<Stmt>();
  stmts->insertAtTail(new LabelStmt(new DefExpr(body->pre_loop)));
  stmts->insertAtTail(new CondStmt(cond, body));
  body->insertAtTail(new GotoStmt(goto_normal, body->pre_loop));
  stmts->insertAtTail(new LabelStmt(new DefExpr(body->post_loop)));
  return stmts;
}


AList<Stmt>* build_do_while_block(Expr* cond, BlockStmt* body) {
  if (body->blockTag != BLOCK_NORMAL)
    INT_FATAL(body, "Abnormal block passed to build_do_while_block");
  body->blockTag = BLOCK_DO_WHILE;
  build_loop_labels(body);
  AList<Stmt>* stmts = new AList<Stmt>();
  stmts->insertAtTail(new LabelStmt(new DefExpr(body->pre_loop)));
  stmts->insertAtTail(body);
  stmts->insertAtTail(new CondStmt(cond, new GotoStmt(goto_normal, body->pre_loop)));
  stmts->insertAtTail(new LabelStmt(new DefExpr(body->post_loop)));
  return stmts;
}


AList<Stmt>* build_for_block(BlockTag tag,
                             AList<DefExpr>* indices,
                             AList<Expr>* iterators,
                             BlockStmt* body) {
  static int uid = 1;
  if (body->blockTag != BLOCK_NORMAL)
    INT_FATAL(body, "Abnormal block passed to build_for_block");
  body->blockTag = tag;
  AList<Stmt>* stmts = new AList<Stmt>();
  build_loop_labels(body);
  VarSymbol* index = new VarSymbol(stringcat("_index_", intstring(uid)));
  if (indices->length() > 1) {
    int i = 1;
    for_alist(DefExpr, indexDef, indices) {
      indexDef->remove();
      indexDef->init = new CallExpr(index, new_IntLiteral(i++));
      body->insertAtHead(indexDef);
    }
  } else {
    DefExpr* indexDef = indices->only();
    indexDef->remove();
    indexDef->init = new SymExpr(index);
    body->insertAtHead(indexDef);
  }
  VarSymbol* iterator = new VarSymbol(stringcat("_iterator_", intstring(uid)));
  VarSymbol* cursor = new VarSymbol(stringcat("_cursor_", intstring(uid)));
  stmts->insertAtTail(new DefExpr(iterator, iterators->only()->copy()));
  stmts->insertAtTail(new DefExpr(cursor, new CallExpr(new CallExpr(".", iterator, new_StringLiteral("getHeadCursor")))));
  stmts->insertAtTail(new LabelStmt(new DefExpr(body->pre_loop)));

  stmts->insertAtTail(new CondStmt(new CallExpr("!", new CallExpr(new CallExpr(".", iterator, new_StringLiteral("isValidCursor?")), cursor)), new GotoStmt(goto_normal, body->post_loop)));

  stmts->insertAtTail(new DefExpr(index, new CallExpr(new CallExpr(".", iterator, new_StringLiteral("getValue")), cursor)));
  stmts->insertAtTail(body);
  stmts->insertAtTail(new CallExpr("=", cursor, new CallExpr(new CallExpr(".", iterator, new_StringLiteral("getNextCursor")), cursor)));
  stmts->insertAtTail(new GotoStmt(goto_normal, body->pre_loop));
  stmts->insertAtTail(new LabelStmt(new DefExpr(body->post_loop)));
  uid++;
  return stmts;
}


AList<Stmt>* build_param_for(char* index, Expr* low, Expr* high, AList<Stmt>* stmts) {
  static int uid = 1;
  FnSymbol* fn = new FnSymbol(stringcat("_param_for_fn", intstring(uid++)));
  fn->formals =
    new AList<DefExpr>(new DefExpr(new ArgSymbol(INTENT_PARAM, index, dtInt)));
  fn->addPragma("inline");
  stmts->insertAtTail(new CallExpr(fn->name, new CallExpr("+", new SymExpr(index), new_IntLiteral(1))));
  fn->insertAtTail(
    new CondStmt(new CallExpr("<=", new SymExpr(index), high), stmts));
  Stmt* def_stmt = new ExprStmt(new DefExpr(fn));
  Stmt* call_stmt = new ExprStmt(new CallExpr(fn->name, low));
  return new AList<Stmt>(def_stmt, call_stmt);
}


AList<Stmt>* build_assignplus(Expr* lhs, Expr* rhs) {
  static int uid = 1;
  FnSymbol* fn;
  AList<Stmt>* stmts = new AList<Stmt>();

  fn = new FnSymbol(stringcat("_assignplus", intstring(uid)));
  fn->formals =
    new AList<DefExpr>(
      new DefExpr(
        new ArgSymbol(INTENT_BLANK, "_lhs", dtAny)));
  fn->addPragma("inline");
  fn->insertAtTail(new CallExpr("=", lhs->copy(), new CallExpr("+", lhs->copy(), rhs->copy())));
  stmts->insertAtTail(new DefExpr(fn));

  fn = new FnSymbol(stringcat("_assignplus", intstring(uid)));
  fn->formals =
    new AList<DefExpr>(
      new DefExpr(
        new ArgSymbol(INTENT_BLANK, "_lhs", dtUnknown), NULL,
          new SymExpr("_domain")));
  fn->addPragma("inline");
  fn->insertAtTail(new CallExpr(new CallExpr(".", lhs->copy(), new_StringLiteral("add")), rhs->copy()));
  stmts->insertAtTail(new DefExpr(fn));

  stmts->insertAtTail(new CallExpr(fn->name, lhs->copy()));
  uid++;
  return stmts;
}


AList<Stmt>* build_type_select(AList<Expr>* exprs, AList<WhenStmt>* whenstmts) {
  static int uid = 1;
  FnSymbol* fn;
  AList<Stmt>* stmts = new AList<Stmt>();
  bool has_otherwise = false;

  for_alist(WhenStmt, whenstmt, whenstmts) {
    if (whenstmt->caseExprs->length() == 0) {
      if (has_otherwise)
        USR_FATAL(exprs, "Type select statement has multiple otherwise clauses");
      has_otherwise = true;
      fn = new FnSymbol(stringcat("_typeselect", intstring(uid)));
      fn->formals = new AList<DefExpr>();
      int lid = 1;
      for_alist(Expr, expr, exprs) {
        fn->formals->insertAtTail(
          new DefExpr(
            new ArgSymbol(INTENT_BLANK,
                          stringcat("_t", intstring(lid++)),
                          dtAny)));
      }
      fn->addPragma("inline");
      fn->insertAtTail(whenstmt->doStmt->body->copy());
      stmts->insertAtTail(new DefExpr(fn));
    } else {
      if (whenstmt->caseExprs->length() != exprs->length())
        USR_FATAL(whenstmt, "Type select statement requires number of selectors to be equal to number of when conditions");
      fn = new FnSymbol(stringcat("_typeselect", intstring(uid)));
      fn->formals = new AList<DefExpr>();
      int lid = 1;
      for_alist(Expr, expr, whenstmt->caseExprs) {
        fn->formals->insertAtTail(
          new DefExpr(
            new ArgSymbol(INTENT_BLANK,
                          stringcat("_t", intstring(lid++)),
                          dtUnknown), NULL, expr->copy()));
      }
      fn->addPragma("inline");
      fn->insertAtTail(whenstmt->doStmt->body->copy());
      stmts->insertAtTail(new DefExpr(fn));
    }
  }
  stmts->insertAtTail(new CallExpr(fn->name, exprs));
  return stmts;
}


Expr* build_reduce_expr(Expr* red, Expr *seq) {
  if (!dynamic_cast<CallExpr*>(red))
    return new CallExpr("_reduce", new CallExpr(red), seq);
  else
    return new CallExpr("_reduce", red, seq);
}
