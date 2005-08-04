#include "insertFunctionTemps.h"
#include "symtab.h"
#include "symscope.h"
#include "symbol.h"
#include "type.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"


static void collect(Expr* expr, Vec<BaseAST*>* asts) {
  class CollectTraversal : public Traversal {
   public:
    Vec<BaseAST*>* data;
    CollectTraversal(Vec<BaseAST*>* init_data) {
      data = init_data;
    }
    void postProcessExpr(Expr* expr) {
      if (CallExpr* paren = dynamic_cast<CallExpr*>(expr)) {
        if (paren->opTag == OP_NONE) {
          data->add(expr);
        }
      }
    }
  };
  TRAVERSE(expr, new CollectTraversal(asts), true);
}


void InsertFunctionTemps::postProcessStmt(Stmt* stmt) {
  static int uid = 1;

  Vec<Expr*> exprs;
  if (ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(stmt)) {
    exprs.add(exprStmt->expr);
  } else if (WhileLoopStmt* whileStmt = dynamic_cast<WhileLoopStmt*>(stmt)) {
    exprs.add(whileStmt->condition);
  } else if (CondStmt* condStmt = dynamic_cast<CondStmt*>(stmt)) {
    exprs.add(condStmt->condExpr);
  } else if (ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(stmt)) {
    if (DefExpr* defExpr = dynamic_cast<DefExpr*>(exprStmt->expr)) {
      if (dynamic_cast<VarSymbol*>(defExpr->sym)) {
        exprs.add(defExpr);
      }
    }
  }

  Vec<BaseAST*> functions;
  functions.clear();
  forv_Vec(Expr, expr, exprs) {
    collect(expr, &functions);
  }
  if (functions.n >= 2) {
    BlockStmt* blockStmt = Symboltable::startCompoundStmt();
    Stmt* copyStmt = stmt->copy(false, NULL, &functions);
    blockStmt = Symboltable::finishCompoundStmt(blockStmt, 
                                                new AList<Stmt>(copyStmt));
    stmt->replace(blockStmt);
    SymScope* saveScope = Symboltable::setCurrentScope(blockStmt->blkScope);
    forv_Vec(BaseAST, ast, functions) {
      CallExpr* function = dynamic_cast<CallExpr*>(ast);
      if (!function) {
        INT_FATAL(ast, "CallExpr expected");
      }
      // The following conditional keeps us from inserting function
      // temps if we are in an expression that has a different scope
      // (and will therefore need to be normalized first).  For
      // example, let expressions.  For these, we call
      // InsertFunctionTemps again on the body of the let expression.
      if (function->parentScope == copyStmt->parentScope) {
        if (function->typeInfo() != dtVoid) {
          char* temp_name = glomstrings(2, "_fntemp_", intstring(uid++));
          Expr* temp_init = function->copy(false, NULL, &functions);
          Type* temp_type = function->typeInfo();
          DefExpr* defExpr =
            Symboltable::defineSingleVarDef(temp_name, temp_type,
                                            temp_init, VAR_NORMAL, VAR_VAR);
          copyStmt->insertBefore(new ExprStmt(defExpr));
          VarSymbol* var = dynamic_cast<VarSymbol*>(defExpr->sym);
          var->noDefaultInit = true;
          function->replace(new Variable(var));
        }
      }
    }
    Symboltable::setCurrentScope(saveScope);
  }
}
