#include "transformLetExprs.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"
#include "symtab.h"
#include "stringutil.h"
#include "insertFunctionTemps.h"


TransformLetExprs::TransformLetExprs() {
  lets.clear();
}


void TransformLetExprs::postProcessExpr(Expr* expr) {
  if (LetExpr* letExpr = dynamic_cast<LetExpr*>(expr)) {
    lets.add(letExpr);
  }
}


void TransformLetExprs::run(ModuleList* moduleList) {
  Traversal::run(moduleList);
  doTransformation();
}


void TransformLetExprs::doTransformation(void) {
  static int uid = 1;
  forv_Vec(BaseAST, ast, lets) {
    LetExpr* letExpr = dynamic_cast<LetExpr*>(ast);
    if (!letExpr) {
      INT_FATAL(ast, "LetExpr expected");
    }
    Stmt* letStmt = letExpr->getStmt();
    BlockStmt* blockStmt = Symboltable::startCompoundStmt();
    Expr* innerCopy = letExpr->innerExpr->copy(false, NULL, &lets);
    letExpr->replace(innerCopy);
    Map<BaseAST*,BaseAST*>* map = new Map<BaseAST*,BaseAST*>();
    AList<Stmt>* defStmts = new AList<Stmt>();
    for_alist(DefExpr, defExpr, letExpr->symDefs) {
      DefStmt* defStmt = new DefStmt(defExpr->copy(true, map, &lets));
      defStmt->defExpr->sym->cname =
        glomstrings(3, defStmt->defExpr->sym->cname, "_let_", intstring(uid++));
      defStmts->insertAtTail(defStmt);
    }
    Stmt* letStmtCopy = letStmt->copy(false, map, &lets);
    defStmts->insertAtTail(letStmtCopy);
    blockStmt = Symboltable::finishCompoundStmt(blockStmt, defStmts);
    letStmt->replace(blockStmt);
    TRAVERSE(blockStmt, new InsertFunctionTemps(), true);
  }
}
