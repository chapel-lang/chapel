#include <typeinfo>
#include "buildLValueFunctions.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"

BuildLValueFunctions::BuildLValueFunctions(void) {
  whichModules = MODULES_CODEGEN;
}

static AList<Stmt> *
handle_return_expr(Expr *e, Symbol *lvalue) {
  Stmt* newStmt = NULL;
  if (e)
    if (CondExpr *ce = dynamic_cast<CondExpr *>(e))
      newStmt = new CondStmt(ce->condExpr, 
                             handle_return_expr(ce->thenExpr, lvalue),
                             handle_return_expr(ce->elseExpr, lvalue));
    else
      newStmt = new ExprStmt(new CallExpr(OP_GETSNORM, e, lvalue));
  return new AList<Stmt>(newStmt);
}


static void
replace_return(BaseAST *ast, Symbol *lvalue) {
  if (ReturnStmt *ret = dynamic_cast<ReturnStmt *>(ast)) {
    AList<Stmt> *body = handle_return_expr(ret->expr->copy(), lvalue);
    body->insertAtTail(new ReturnStmt(NULL));
    ast = new BlockStmt(body);
    ret->replace((Stmt*)ast);
    return;
  }
  Vec<BaseAST *> asts;
  get_ast_children(ast, asts);
  forv_BaseAST(a, asts)
    if (isSomeStmt(a->astType))
      replace_return(a, lvalue);
}

void BuildLValueFunctions::preProcessStmt(Stmt* stmt) {
  ExprStmt *old_expr_stmt = dynamic_cast<ExprStmt*>(stmt);
  if (!old_expr_stmt)
    return;
  DefExpr *old_def_expr = dynamic_cast<DefExpr*>(old_expr_stmt->expr);
  if (!old_def_expr)
    return;
  FnSymbol *old_fn = dynamic_cast<FnSymbol*>(old_def_expr->sym);
  if (!old_fn || !old_fn->retRef)
    return;
  ExprStmt *expr_stmt = old_expr_stmt->copy();
  DefExpr *def_expr = dynamic_cast<DefExpr*>(expr_stmt->expr);
  FnSymbol *fn = dynamic_cast<FnSymbol*>(def_expr->sym);
  fn->retRef = false;
  fn->retType = dtVoid;
  def_expr->exprType = NULL;
  fn->name = old_fn->name;
  fn->cname = stringcat("_setter_", old_fn->cname);
  old_expr_stmt->insertAfter(expr_stmt);
  fn->formals->insertAtTail(new DefExpr(new ArgSymbol(INTENT_REF, "_setterTokenDummy", 
                                                      dtSetterToken)));
  ArgSymbol* lvalue = new ArgSymbol(INTENT_BLANK, "_lvalue", old_fn->retType);
  fn->formals->insertAtTail(new DefExpr(lvalue));
  replace_return(fn->body, lvalue);
  if (old_fn->typeBinding) {
    old_fn->typeBinding->definition->methods.add(fn);
    fn->typeBinding = old_fn->typeBinding;
  }
}



