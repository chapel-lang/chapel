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
                             new BlockStmt(handle_return_expr(ce->thenExpr, lvalue)),
                             new BlockStmt(handle_return_expr(ce->elseExpr, lvalue)));
    else
      newStmt = new ExprStmt(new AssignOp(GETS_NORM, e, new Variable(lvalue)));
  return new AList<Stmt>(newStmt);
}


static void
replace_return(BaseAST *ast, Symbol *lvalue) {
  if (ReturnStmt *ret = dynamic_cast<ReturnStmt *>(ast)) {
    Symboltable::pushScope(SCOPE_LOCAL);
    AList<Stmt> *body = handle_return_expr(ret->expr, lvalue);
    body->insertAtTail(new ReturnStmt(NULL));
    ast = new BlockStmt(body, Symboltable::popScope());
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
  SymScope* saveScope = Symboltable::setCurrentScope(old_fn->parentScope);
  ExprStmt *expr_stmt = old_expr_stmt->copy(true);
  DefExpr *def_expr = dynamic_cast<DefExpr*>(expr_stmt->expr);
  FnSymbol *fn = dynamic_cast<FnSymbol*>(def_expr->sym);
  fn->retRef = false;
  fn->retType = dtVoid;
  def_expr->exprType = NULL;
  fn->name = glomstrings(2, "=", old_fn->name);
  fn->cname = glomstrings(2, "_setter_", old_fn->cname);
  Symboltable::undefineInScope(fn, fn->parentScope);
  Symboltable::defineInScope(fn, fn->parentScope);
  old_expr_stmt->insertAfter(expr_stmt);
  Symboltable::setCurrentScope(fn->paramScope);
  ParamSymbol* lvalue = new ParamSymbol(PARAM_BLANK, "_lvalue", old_fn->retType);
  fn->formals->insertAtTail(new DefExpr(lvalue));
  replace_return(fn->body, lvalue);
  if (old_fn->typeBinding) {
    old_fn->typeBinding->type->methods.add(fn);
    fn->typeBinding = old_fn->typeBinding;
  }
  Symboltable::setCurrentScope(saveScope);
}



