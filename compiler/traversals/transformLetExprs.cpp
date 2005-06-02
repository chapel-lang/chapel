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
    AList<DefExpr>* defExpr = letExpr->symDefs->copy(true, map, &lets);
    Stmt* letStmtCopy = letStmt->copy(false, map, &lets);
    AList<Stmt>* defStmt = new AList<Stmt>(new DefStmt(defExpr));
    defStmt->insertAtTail(letStmtCopy);
    blockStmt = Symboltable::finishCompoundStmt(blockStmt, defStmt);

    for (DefExpr* tmp = defExpr->first(); tmp; tmp = defExpr->next()) {
      tmp->sym->cname =
        glomstrings(3, tmp->sym->cname, "_let_", intstring(uid++));
    }

    letStmt->replace(blockStmt);
    TRAVERSE(blockStmt, new InsertFunctionTemps(), true);
  }
}
