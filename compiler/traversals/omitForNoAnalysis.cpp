#include "omitForNoAnalysis.h"
#include "stmt.h"
#include "expr.h"
#include "type.h"
#include "symtab.h"
#include "stringutil.h"
#include "../symtab/symtabTraversal.h"


static int omit_for_noanalysis_uid = 0;


class KillSubSymbols : public SymtabTraversal {
 public:
  void processSymbol(Symbol* sym);
};


void KillSubSymbols::processSymbol(Symbol* sym) {
  sym->name = glomstrings(4, "_REMOVED_FOR_OMIT_FOR_NOANALYSIS_", 
                          intstring(omit_for_noanalysis_uid++),
                          "_", sym->name);
  sym->parentScope->remove(sym);
}


void OmitForNoAnalysis::postProcessStmt(Stmt* stmt) {
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
          sym->name = glomstrings(4, "_REMOVED_FOR_OMIT_FOR_NOANALYSIS_", 
                                  intstring(omit_for_noanalysis_uid++),
                                  "_", sym->name);
          sym->parentScope->remove(sym);
          /*** SJD: Ugh -- We only omit for sub symbols in a class type
               because it is difficult to start a symtab traversal on
               a symbol. Should ask Brad (BLC) about this. ***/
          if (ClassType* ctype = dynamic_cast<ClassType*>(sym->type)) {
            KillSubSymbols traversal;
            Symboltable::traverseFromScope(&traversal, ctype->classScope);
          }
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
