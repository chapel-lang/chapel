#include "removeDeadSymbols.h"
#include "analysis.h"
#include "stmt.h"
#include "expr.h"
#include "../traversals/view.h"


void RemoveDeadSymbols::postProcessExpr(Expr* expr) {
  if (DefExpr* defExpr = dynamic_cast<DefExpr*>(expr)) {
    Symbol* sym = defExpr->sym;

#ifdef USE_AST_IS_USED
    // BLC: This is how we would want to do this, but it doesn't
    // work -- seems to kill all the stuff in the commonmodule
    // for some reason when the other code does not.
    if (!AST_is_used(sym)) {
      sym->isDead = true;
      // BLC: this is a bad assumption: that if one DefExpr is dead
      // then the whole statement can be extracted; instead, we
      // should pull just the defExpr (but this breaks other stuff
      // that assumes that every defStmt).
      defExpr->stmt->extract();
    }
#else
    if (TypeSymbol* typeSym = dynamic_cast<TypeSymbol*>(sym)) {
      if (!type_is_used(typeSym)) {
	typeSym->isDead = true;

	// BLC: this is a bad assumption: that if one DefExpr is dead
	// then the whole statement can be extracted; instead, we
	// should pull just the defExpr (but this breaks other stuff
	// that assumes that every defStmt).

	defExpr->stmt->extract();
      }
    }
    if (FnSymbol* fnSym = dynamic_cast<FnSymbol*>(sym)) {
      if (!function_is_used(fnSym)) {
	fnSym->isDead = true;
	// BLC: see comment on previous conditional
	defExpr->stmt->extract();
	//print_view(def_stmt);
      }
    }
#endif
  }
}
