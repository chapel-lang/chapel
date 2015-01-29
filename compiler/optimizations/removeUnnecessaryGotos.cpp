#include "astutil.h"
#include "expr.h"
#include "optimizations.h"
#include "stmt.h"
#include <vector>
#include <set>
#include "stlUtil.h"

//
// Removes gotos where the label immediately follows the goto and
// unused labels
//
void removeUnnecessaryGotos(FnSymbol* fn) {
  std::vector<BaseAST*> asts;
  std::set<BaseAST*> labels;
  collect_asts_STL(fn, asts);
  for_vector(BaseAST, ast, asts) {
    if (GotoStmt* gotoStmt = toGotoStmt(ast)) {
      DefExpr* def = toDefExpr(gotoStmt->next);
      SymExpr* label = toSymExpr(gotoStmt->label);
      INT_ASSERT(label);
      if (def && def->sym == label->var)
        gotoStmt->remove();
      else
        labels.insert(label->var);
    }
  }

  for_vector(BaseAST, ast2, asts) {
    if (DefExpr* def = toDefExpr(ast2))
      if (LabelSymbol* label = toLabelSymbol(def->sym))
        if (labels.find(label) == labels.end())
          def->remove();
  }
}
