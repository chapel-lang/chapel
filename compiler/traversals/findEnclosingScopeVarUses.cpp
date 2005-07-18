#include "findEnclosingScopeVarUses.h"
#include "symscope.h"
#include "symtab.h"

FindEnclosingScopeVarUses::FindEnclosingScopeVarUses(SymScope* encl_scope) {
  _encl_scope = encl_scope;
  _var_uses_vec = new Vec<Symbol*>();
}

void FindEnclosingScopeVarUses::postProcessExpr(Expr* expr) {
  if (Variable* v = dynamic_cast<Variable*>(expr)) {
    //variable defined in enclosing function scope
    Symbol* v_sym = v->var;
    if (Symboltable::lookupInScope(v_sym->name, _encl_scope))
        _var_uses_vec->add_exclusive(v_sym);
  }
}

Vec<Symbol*>* FindEnclosingScopeVarUses::getVarUses() {return _var_uses_vec;}
