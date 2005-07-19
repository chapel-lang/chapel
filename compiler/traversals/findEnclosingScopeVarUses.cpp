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
    SymScope* encl_scope = _encl_scope;
    //look up outer scopes until module scope is reached or symbol is found
    do {
      if (Symboltable::lookupInScope(v_sym->name, encl_scope)) {
        _var_uses_vec->add_exclusive(v_sym);
        return;
      }
      encl_scope = encl_scope->parent;
    }while (encl_scope->type != SCOPE_MODULE);
      
  }
}

Vec<Symbol*>* FindEnclosingScopeVarUses::getVarUses() {return _var_uses_vec;}
