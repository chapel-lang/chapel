#include "transformLetExprs.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"
#include "symtab.h"
#include "stringutil.h"


TransformLetExprs::TransformLetExprs() {
  lets.clear();
}


void TransformLetExprs::postProcessExpr(Expr* expr) {
  if (LetExpr* let_expr = dynamic_cast<LetExpr*>(expr)) {
    lets.add(let_expr);
  }
}


void TransformLetExprs::run(ModuleSymbol* moduleList) {
  ModuleSymbol* mod = moduleList;
  while (mod) {
    if (mod->internal == false || processInternalModules == true) {
      mod->startTraversal(this);
    }
    
    mod = nextLink(ModuleSymbol, mod);
  }
  doTransformation();
}


void TransformLetExprs::doTransformation(void) {
  static int uid = 1;
  forv_Vec(BaseAST, ast, lets) {
    LetExpr* let_expr = dynamic_cast<LetExpr*>(ast);
    if (!let_expr) {
      INT_FATAL(ast, "LetExpr expected");
    }
    SymScope* save_scope = Symboltable::setCurrentScope(let_expr->letScope->parent);
    Stmt* let_stmt = let_expr->stmt;
    Symbol* let_syms = let_expr->syms;
    SymScope* let_scope = let_expr->letScope;
    Expr* inner_copy = let_expr->innerExpr->copy(false, NULL, NULL, &lets);
    let_expr->replace(inner_copy);
    Stmt* let_stmt_copy = let_stmt->copy(false, NULL, NULL, &lets);
    BlockStmt* block_stmt = new BlockStmt(let_stmt_copy);
    let_scope->stmtContext = block_stmt;
    let_scope->exprContext = nilExpr;
    let_scope->type = SCOPE_LOCAL;
    block_stmt->setBlkScope(let_scope);
    Symbol* tmp = let_syms;
    while (tmp && !tmp->isNull()) {
      Symbol* next = nextLink(Symbol, tmp);
      if (VarSymbol* var = dynamic_cast<VarSymbol*>(tmp)) {
	var->prev = nilSymbol;
	var->next = nilSymbol;
	VarDefStmt* def_stmt = new VarDefStmt(var);
	var->cname = glomstrings(3, var->cname, "_let_", intstring(uid++));
	var->setDefPoint(def_stmt);
	let_stmt_copy->insertBefore(def_stmt);
      }
      else {
	INT_FATAL(tmp, "Case not handled in TransformLetExprs");
      }
      tmp = next;
    }
    let_stmt->replace(block_stmt);
    Symboltable::setCurrentScope(save_scope);
  }
}
