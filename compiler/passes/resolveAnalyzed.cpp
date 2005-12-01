#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "runtime.h"
#include "stmt.h"
#include "symbol.h"
#include "symtab.h"
#include "stringutil.h"


void resolve_analyzed(void) {
  Vec<BaseAST*> asts;
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {
    if (TypeSymbol* ts = dynamic_cast<TypeSymbol*>(ast)) {
      if (!type_is_used(ts))
        ts->defPoint->parentStmt->remove();
    } else if (FnSymbol* fs = dynamic_cast<FnSymbol*>(ast)) {
      if (!function_is_used(fs))
        fs->defPoint->parentStmt->remove();
    }
  }
}
