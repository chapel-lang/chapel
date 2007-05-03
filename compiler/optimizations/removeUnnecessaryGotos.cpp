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
  collect_asts(&asts, fn);
  forv_Vec(BaseAST, ast, asts) {
    if (GotoStmt* gotoStmt = dynamic_cast<GotoStmt*>(ast)) {
      DefExpr* def = dynamic_cast<DefExpr*>(gotoStmt->next);
      if (def && def->sym == gotoStmt->label)
        gotoStmt->remove();
      else
        labels.set_add(gotoStmt->label);
    }
  }

  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = dynamic_cast<DefExpr*>(ast))
      if (LabelSymbol* label = dynamic_cast<LabelSymbol*>(def->sym))
        if (!labels.set_in(label))
          def->remove();
  }
}
