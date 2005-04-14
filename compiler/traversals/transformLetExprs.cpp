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
  Traversal::run(moduleList);
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
    Stmt* let_stmt = let_expr->getStmt();
    SymScope* let_scope = let_expr->letScope;
    Expr* inner_copy = let_expr->innerExpr->copy(false, NULL, NULL, &lets);
    let_expr->replace(inner_copy);
    Stmt* let_stmt_copy = let_stmt->copy(false, NULL, NULL, &lets);
    DefStmt* def_stmt = new DefStmt(let_expr->symDefs);
    def_stmt->append(let_stmt_copy);
    let_stmt_copy = def_stmt;
    BlockStmt* block_stmt = new BlockStmt(let_stmt_copy);
    let_scope->stmtContext = block_stmt;
    let_scope->exprContext = NULL;
    let_scope->type = SCOPE_LOCAL;
    block_stmt->setBlkScope(let_scope);

    DefExpr* def_expr = dynamic_cast<DefExpr*>(let_expr->symDefs);
    while (def_expr) {
      Symbol* tmp = def_expr->sym;
      while (tmp) {
        tmp->cname = glomstrings(3, tmp->cname, "_let_", intstring(uid++));
        tmp = nextLink(Symbol, tmp);
      }
      def_expr = nextLink(DefExpr, def_expr);
    }
    let_stmt->replace(block_stmt);
    Symboltable::setCurrentScope(save_scope);
  }
}
