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


// Destructure ForLoopStmt indices
void destruct_for_indices(ForLoopStmt* stmt) {
  if (stmt->indices->length() > 1) {
    VarSymbol* indextmp = new VarSymbol("_index_temp");
    int i = 1;
    for_alist(DefExpr, indexDef, stmt->indices) {
      indexDef->remove();
      indexDef->init = new CallExpr(indextmp, new_IntLiteral(i++));
      stmt->innerStmt->insertAtHead(new ExprStmt(indexDef));
    }
    stmt->indices->insertAtTail(new DefExpr(indextmp));
  }
}


void NormalizeParsedAST::run(Vec<ModuleSymbol*>* modules) {
  Vec<BaseAST*> asts;
  collect_asts(&asts);

  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* a = dynamic_cast<DefExpr*>(ast)) {
      normalize_anonymous_record_or_forall_expression(a);
    } else if (ForLoopStmt* a = dynamic_cast<ForLoopStmt*>(ast)) {
      destruct_for_indices(a);
    }
  }
}
