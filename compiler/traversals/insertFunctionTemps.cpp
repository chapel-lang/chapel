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
    if (DefExpr* defExpr = dynamic_cast<DefExpr*>(exprStmt->expr)) {
      if (dynamic_cast<VarSymbol*>(defExpr->sym)) {
        exprs.add(defExpr);
      }
    } else {
      exprs.add(exprStmt->expr);
    }
  } else if (WhileLoopStmt* whileStmt = dynamic_cast<WhileLoopStmt*>(stmt)) {
    exprs.add(whileStmt->condition);
  } else if (ForLoopStmt* forStmt = dynamic_cast<ForLoopStmt*>(stmt)) {
    exprs.add(forStmt->iterators->only());
  } else if (CondStmt* condStmt = dynamic_cast<CondStmt*>(stmt)) {
    exprs.add(condStmt->condExpr);
  }

  Vec<BaseAST*> functions;
  functions.clear();
  forv_Vec(Expr, expr, exprs) {
    collect(expr, &functions);
  }
  if (functions.n >= 2) {
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
      if (function->parentScope == stmt->parentScope) {
        if (function->typeInfo() != dtVoid) {
          char* temp_name = stringcat("_fntemp_", intstring(uid++));
          Type* temp_type = function->typeInfo();
          VarSymbol* var = new VarSymbol(temp_name, temp_type);
          stmt->insertBefore(new ExprStmt(new DefExpr(var)));
          stmt->insertBefore(new ExprStmt(new CallExpr(OP_GETS, var, function->copy(NULL, &functions))));
          var->noDefaultInit = true;
          function->replace(new SymExpr(var));
        }
      }
    }
  }
}


void insertFunctionTemps(void) {
  Pass* pass = new InsertFunctionTemps();
  pass->run(Symboltable::getModules(pass->whichModules));
}
