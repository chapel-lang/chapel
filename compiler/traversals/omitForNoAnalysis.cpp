#include "omitForNoAnalysis.h"
#include "stmt.h"
#include "expr.h"
#include "type.h"
#include "stringutil.h"


void OmitForNoAnalysis::postProcessStmt(Stmt* stmt) {
  static int uid = 0;

  DefStmt* def_stmt = dynamic_cast<DefStmt*>(stmt);

  if (!def_stmt) {
    return;
  }

  Pragma* pr = def_stmt->pragmas;
  while (pr) {
    if (!strcmp(pr->str, "omit for noanalysis")) {
      DefExpr* def_expr = dynamic_cast<DefExpr*>(def_stmt->defExprList);
      while (def_expr) {
        Symbol* sym = def_expr->sym;
        while (sym) {
          sym->name = glomstrings(4, "_OMIT_FOR_NOANALYSIS_", 
                                  intstring(uid++), "_", sym->name);
          sym = nextLink(Symbol, sym);
        }
        def_expr = nextLink(DefExpr, def_expr);
      }
      def_stmt->extract();
      return;
    }
    pr = dynamic_cast<Pragma *>(pr->next);
  }
}


void OmitForNoAnalysis::run(ModuleSymbol* moduleList) {
  if (analyzeAST) {
    return;
  }
  ModuleSymbol* mod = moduleList;
  while (mod) {
    mod->startTraversal(this);
    
    mod = nextLink(ModuleSymbol, mod);
  }
}
