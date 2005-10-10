#include "normalizeParsedAST.h"
#include "astutil.h"
#include "expr.h"
#include "stmt.h"
#include "symtab.h"
#include "stringutil.h"


// Move anonymous record definition into separate statement
// Or move anonymous forall expression iterator into separate statement
void normalize_anonymous_record_or_forall_expression(DefExpr* def) {
  if ((!strncmp("_anon_record", def->sym->name, 12)) ||
      (!strncmp("_forallexpr", def->sym->name, 11))) {
    Stmt* stmt = def->parentStmt;
    def->replace(new SymExpr(def->sym));
    stmt->insertBefore(new ExprStmt(def));
  }
}


void NormalizeParsedAST::run(Vec<ModuleSymbol*>* modules) {
  Vec<BaseAST*> asts;
  collect_asts(&asts);

  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* a = dynamic_cast<DefExpr*>(ast)) {
      normalize_anonymous_record_or_forall_expression(a);
    }
  }
}
