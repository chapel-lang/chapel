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
  if (ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(stmt)) {
    exprs.add(expr_stmt->expr);
  }
  else if (WhileLoopStmt* while_stmt = dynamic_cast<WhileLoopStmt*>(stmt)) {
    exprs.add(while_stmt->condition);
  }
  else if (CondStmt* cond_stmt = dynamic_cast<CondStmt*>(stmt)) {
    exprs.add(cond_stmt->condExpr);
  }

  Vec<BaseAST*> functions;
  forv_Vec(Expr, expr, exprs) {
    collect(expr, &functions);
  }
  if (functions.n >= 2) {
    /*    NoOpStmt* noop_stmt = new NoOpStmt();
    stmt->replace(noop_stmt);
    stmt->next = NULL;
    stmt->prev = NULL;*/
    Stmt* copy_stmt = stmt->copy(false, NULL, NULL, &functions);
    BlockStmt* block_stmt = new BlockStmt(copy_stmt);
    stmt->replace(block_stmt);
    Symboltable::pushScope(SCOPE_LOCAL);
    forv_Vec(BaseAST, ast, functions) {
      FnCall* function = dynamic_cast<FnCall*>(ast);
      if (!function) {
        INT_FATAL(ast, "FnCall expected");
      }
      if (function->typeInfo() != dtVoid) {
        char* temp_name = glomstrings(2, "_fntemp_", intstring(uid++));
        Expr* temp_init = function->copy(false, NULL, NULL, &functions);
        Type* temp_type = function->typeInfo();
        DefStmt* def_stmt =
          Symboltable::defineSingleVarDefStmt(temp_name, temp_type,
                                              temp_init, VAR_NORMAL, VAR_VAR);
        copy_stmt->insertBefore(def_stmt);
        function->replace(new Variable(def_stmt->varDef()));
      }
    }
    SymScope* block_scope = Symboltable::popScope();
    block_scope->setContext(block_stmt);
    block_stmt->setBlkScope(block_scope);
  }
}
