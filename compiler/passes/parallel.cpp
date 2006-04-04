// This pass performs some transformations to enable parallel code
// generation.

#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"
#include "symbol.h"
#include "symtab.h"
#include "stringutil.h"
#include "driver.h"

// This pass moves cobegin statements into nested functions. Currently,
// each statement is moved to within it's own function and the
// appropriate function def and call expressions are added. 
void
cobegin () {
  int  ufid = 1;

  forv_Vec (ModuleSymbol, mod, allModules) {
    Vec<BaseAST*> asts;
    collect_asts(&asts, mod);
    forv_Vec (BaseAST, ast, asts) {
      if (BlockStmt *b=dynamic_cast<BlockStmt*>(ast)) {
        if (BLOCK_COBEGIN == b->blockTag) {
          // replace with a new cobegin block
          BlockStmt *newb = new BlockStmt();
          newb->blockTag = BLOCK_COBEGIN;
          for_alist (Stmt, stmt, b->body) {
            char *fname = stringcat ("_cobegin_stmt", intstring (ufid++));
            FnSymbol *fn = new FnSymbol (fname, NULL, new AList<DefExpr>(), dtVoid);
            stmt->remove ();
            fn->insertAtTail (stmt);            // move stmt to new function
            newb->body->insertAtHead (new DefExpr (fn));
            newb->body->insertAtTail (new CallExpr (fname));
            // stmt->insertBefore (new CallExpr (fname)); // in blk add fn call
            // b->insertBefore (new DefExpr (fn)); // add def to before cobegin blk
          }
          b->insertBefore (newb);
          b->remove ();
          delete b;
        }
      }
    }
  }
}


void
parallel (void) {
  if (parallelPass) {
    cobegin ();      // move cobegin stmts to within a function
  }
}

