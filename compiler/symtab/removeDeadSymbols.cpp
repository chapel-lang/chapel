#include "removeDeadSymbols.h"
#include "analysis.h"
#include "stmt.h"
#include "expr.h"
#include "../traversals/view.h"


class RemoveDeadTypesFns : public SymtabTraversal {
 public:
  RemoveDeadTypesFns(void) {
    whichModules = MODULES_USER;
  }


  void processSymbol(Symbol* sym) {
#ifdef USE_AST_IS_USED
    if (!AST_is_used(sym)) {
      sym->isDead = true;
      
      if (sym->defPoint) {
	// BLC: This is overkill, but mirrors what the original
	// removeDeadSymbols did
	sym->defPoint->stmt->extract();
      }
      fprintf(stderr, "%s is dead\n", sym->name);
    }
#else
    if (TypeSymbol* typeSym = dynamic_cast<TypeSymbol*>(sym)) {
      if (!type_is_used(typeSym)) {
	typeSym->isDead = true;
	
	// BLC: this is a bad assumption: that if one DefExpr is dead
	// then the whole statement can be extracted; instead, we
	// should pull just the defExpr (but this breaks other stuff
	// that assumes that every defStmt).
	
	sym->defPoint->stmt->extract();
      }
    }
    if (FnSymbol* fnSym = dynamic_cast<FnSymbol*>(sym)) {
      if (!function_is_used(fnSym)) {
	fnSym->isDead = true;
	// BLC: see comment on previous conditional
	sym->defPoint->stmt->extract();
      }
    }
#endif
  }
};


class EnsureVarTypesNotDead : public SymtabTraversal {
 public:
  EnsureVarTypesNotDead(void) {
    whichModules = MODULES_USER;
  }

  void processSymbol(Symbol* sym) {
    if (!sym->isDead) {
      /*
      if (sym->type->symbol->isDead) {
	fprintf(stderr, "saving %s from being killed\n", 
	        sym->type->symbol->name);
      }
      */

      if (sym->type && sym->type->symbol) {
	sym->type->symbol->isDead = false;
      }
    }
  }
};


RemoveDeadSymbols::RemoveDeadSymbols(void) {
  whichModules = MODULES_USER;
}



// BLC: The strategy taken here is to remove dead stuff as 
// we did before, and then to re-animate types that analysis
// says are dead, but which we need for code generation
// purposes (for example, if a variable is defined to be type
// myclass, and it is used for its nil value but is never
// instantiated, analysis considers myclass to be dead, but
// codegen still relies on it in order to declare the variable's
// C representation.
//
// Having implemented this, I worry that it may be naive since
// there may be nontrivial relationsips between types and vars,
// types and types, etc.  We could either iterate until we reach
// a fixed point, or we could work on getting analysis to support
// codegen's notion of deadness.


void RemoveDeadSymbols::run(ModuleSymbol* moduleList) {
  RemoveDeadTypesFns* trav1 = new RemoveDeadTypesFns();
  trav1->run(moduleList);

  EnsureVarTypesNotDead* trav2 = new EnsureVarTypesNotDead();
  trav2->run(moduleList);
}
