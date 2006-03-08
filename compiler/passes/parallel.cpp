/*
 * This pass moves cobegin statements into nested functions.
 */

#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"
#include "symbol.h"
#include "symtab.h"
#include "stringutil.h"
#include "driver.h"


void 
cobegin () {
  static int ufid = 1;

  forv_Vec (ModuleSymbol, mod, allModules) {
    Vec<BaseAST*> asts;
    collect_asts(&asts, mod);
    forv_Vec (BaseAST, ast, asts) {
      if (BlockStmt *b=dynamic_cast<BlockStmt*>(ast)) {
        if (BLOCK_COBEGIN == b->blockType) {
          for_alist (Stmt, stmt, b->body) {
            char *fname = stringcat ("_cobegin_stmt", intstring (ufid++));
            FnSymbol *fn = new FnSymbol (fname, NULL, new AList<DefExpr>(), dtVoid);
            stmt->insertBefore (new CallExpr (fname)); // in block add call to fn
            stmt->remove ();
            fn->insertAtTail (stmt);            // move stmt to new function
            b->insertBefore (new DefExpr (fn)); // add def to before cobegin blk
          }
        }
      }
    }
  }
}


void
parallel (void) {
  if (parallelPass) {
    cobegin ();     // move cobegin stmts to within a function
  }
}

