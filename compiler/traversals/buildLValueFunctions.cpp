#include <typeinfo>
#include "buildLValueFunctions.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"

BuildLValueFunctions::BuildLValueFunctions(void) {
  whichModules = MODULES_CODEGEN;
}

static Stmt *
handle_return_expr(Expr *e, Symbol *lvalue) {
  if (!e)
    return NULL;
  if (CondExpr *ce = dynamic_cast<CondExpr *>(e)) {
    return new CondStmt(ce->condExpr, 
                        new BlockStmt(handle_return_expr(ce->thenExpr, lvalue)),
                        new BlockStmt(handle_return_expr(ce->elseExpr, lvalue)));
  } else
    return new ExprStmt(new AssignOp(GETS_NORM, e, new Variable(lvalue)));
}


static void
replace_return(BaseAST *ast, Symbol *lvalue) {
  if (ReturnStmt *ret = dynamic_cast<ReturnStmt *>(ast)) {
    Symboltable::pushScope(SCOPE_LOCAL);
    Stmt *assign = handle_return_expr(ret->expr, lvalue);
    ast = new BlockStmt(assign, Symboltable::popScope());
    assign->append(new ReturnStmt(NULL));
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
  lvalue->setDefPoint(def_stmt->defExprls);
  fn->formals = appendLink(fn->formals, lvalue);
  replace_return(fn->body, lvalue);
  Symboltable::setCurrentScope(saveScope);
}



