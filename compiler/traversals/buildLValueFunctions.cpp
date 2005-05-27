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
    body->add(new ReturnStmt(NULL));
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
  DefStmt *old_def_stmt = dynamic_cast<DefStmt*>(stmt);
  if (!old_def_stmt)
    return;
  FnSymbol *old_fn = old_def_stmt->fnDef();
  if (!old_fn || !old_fn->retRef)
    return;
  SymScope* saveScope = Symboltable::setCurrentScope(old_fn->parentScope);
  DefStmt *def_stmt = (DefStmt*)old_def_stmt->copy(true);
  FnSymbol *fn = def_stmt->fnDef();
  fn->retRef = false;
  fn->retType = dtVoid;
  fn->name = glomstrings(2, "=", old_fn->name);
  fn->cname = glomstrings(2, "_setter_", old_fn->cname);
  fn->parentScope->remove(fn);
  fn->parentScope->insert(fn);
  old_def_stmt->insertAfter(def_stmt);
  Symboltable::setCurrentScope(fn->paramScope);
  Symbol* lvalue = new ParamSymbol(PARAM_BLANK, "_lvalue", old_fn->retType);
  lvalue->setDefPoint(def_stmt->defExprls->only());
  fn->formals->add(lvalue);
  replace_return(fn->body, lvalue);
  Symboltable::setCurrentScope(saveScope);
}



