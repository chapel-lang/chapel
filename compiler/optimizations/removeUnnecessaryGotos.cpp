/**************************************************************************
  Copyright (c) 2004-2013, Cray Inc.  (See LICENSE file for more details)
**************************************************************************/


#include "astutil.h"
#include "expr.h"
#include "optimizations.h"
#include "stmt.h"

//
// Removes gotos where the label immediately follows the goto and
// unused labels
//
void removeUnnecessaryGotos(FnSymbol* fn) {
  Vec<BaseAST*> asts;
  Vec<BaseAST*> labels;
  collect_asts(fn, asts);
  forv_Vec(BaseAST, ast, asts) {
    if (GotoStmt* gotoStmt = toGotoStmt(ast)) {
      DefExpr* def = toDefExpr(gotoStmt->next);
      SymExpr* label = toSymExpr(gotoStmt->label);
      INT_ASSERT(label);
      if (def && def->sym == label->var)
        gotoStmt->remove();
      else
        labels.set_add(label->var);
    }
  }

  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = toDefExpr(ast))
      if (LabelSymbol* label = toLabelSymbol(def->sym))
        if (!labels.set_in(label))
          def->remove();
  }
}
