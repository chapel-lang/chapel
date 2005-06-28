#include "omitForNoAnalysis.h"
#include "stmt.h"
#include "expr.h"
#include "moduleList.h"
#include "symtab.h"
#include "stringutil.h"
#include "../symtab/symtabTraversal.h"
#include "type.h"


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

  if (def_stmt->hasPragma("omit for noanalysis")) {
    DefExpr* def_expr = def_stmt->defExprls->first();
    while (def_expr) {
      Symbol* sym = def_expr->sym;
      sym->parentScope->remove(sym);
      if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym)) {
        Symboltable::removeScope(fn->paramScope);
      } else if (StructuralType* ctype = dynamic_cast<StructuralType*>(sym->type)) {
        Symboltable::removeScope(ctype->structScope);
      } 
      def_expr = def_stmt->defExprls->next();
    }
    def_stmt->remove();
    return;
  }
}


void OmitForNoAnalysis::run(ModuleList* moduleList) {
  if (analyzeAST) {
    return;
  }
  ModuleSymbol* mod = moduleList->first();
  while (mod) {
    mod->startTraversal(this);
    
    mod = moduleList->next();
  }
}
