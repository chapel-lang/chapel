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
      if (dynamic_cast<FnCall*>(expr)) {
        data->add(expr);
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
  } else if (DefStmt* defStmt = dynamic_cast<DefStmt*>(stmt)) {
    if (defStmt->varDef()) {
      for (DefExpr* tmp = defStmt->defExprls; tmp; tmp = nextLink(DefExpr, tmp)) {
        exprs.add(tmp);
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
    Stmt* copyStmt = stmt->copy(false, NULL, NULL, &functions);
    blockStmt = Symboltable::finishCompoundStmt(blockStmt, copyStmt);
    stmt->replace(blockStmt);
    SymScope* saveScope = Symboltable::setCurrentScope(blockStmt->blkScope);
    bool no_default_init = 0;
    forv_Vec(BaseAST, ast, functions) {
      FnCall* function = dynamic_cast<FnCall*>(ast);
      if (!function) {
        INT_FATAL(ast, "FnCall expected");
      }
      // The following conditional keeps us from inserting function
      // temps if we are in an expression that has a different scope
      // (and will therefore need to be normalized first).  For
      // example, let expressions.  For these, we call
      // InsertFunctionTemps again on the body of the let expression.
      if (function->parentScope == copyStmt->parentScope) {
        if (function->typeInfo() != dtVoid) {
          char* temp_name = glomstrings(2, "_fntemp_", intstring(uid++));
          Expr* temp_init = function->copy(false, NULL, NULL, &functions);
          Type* temp_type = function->typeInfo();
          varType var_type = VAR_NORMAL;
          if (Variable *fn_var = dynamic_cast<Variable*>(function->baseExpr)) {
            if (FnSymbol *fn = dynamic_cast<FnSymbol*>(fn_var->var)) {
              if (fn->_getter && is_Value_Type(fn->retType)) {
                var_type = VAR_REF;
                no_default_init = true;
              }
            }
          }
          DefStmt* defStmt =
            Symboltable::defineSingleVarDefStmt(temp_name, temp_type,
                                                temp_init, var_type, VAR_VAR);
          copyStmt->insertBefore(defStmt);
          if (no_default_init)
            defStmt->varDef()->noDefaultInit = true;
          function->replace(new Variable(defStmt->varDef()));
        }
      }
    }
    Symboltable::setCurrentScope(saveScope);
  }
}
