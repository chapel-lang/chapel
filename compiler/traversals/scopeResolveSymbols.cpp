#include "scopeResolveSymbols.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"

static void resolve_type_helper(Type* &type) {
  if (dynamic_cast<UnresolvedType*>(type)) {
    Symbol* new_type = Symboltable::lookup(type->symbol->name);
    if (new_type && !new_type->isNull()) {
      if (!dynamic_cast<UnresolvedType*>(new_type->type)) {
	type = new_type->type;
      }
      else {
	resolve_type_helper(new_type->type);
	type = new_type->type;
      }
    }
    else {
      INT_FATAL(type, "Error resolving type");
    }
  }
  if (UserType* user_type = dynamic_cast<UserType*>(type)) {
    resolve_type_helper(user_type->definition);
  }
  if (ArrayType* array_type = dynamic_cast<ArrayType*>(type)) {
    resolve_type_helper(array_type->elementType);
  }
  if (TupleType* tuple_type = dynamic_cast<TupleType*>(type)) {
    for (int i = 0; i < tuple_type->components.n; i++) {
      resolve_type_helper(tuple_type->components.v[i]);
    }
    tuple_type->rebuildDefaultVal();
  }
}

void ScopeResolveSymbols::preProcessExpr(Expr* expr) {
  if (CastExpr* cast_expr = dynamic_cast<CastExpr*>(expr)) {
    resolve_type_helper(cast_expr->newType);
  }
  if (Variable* variable_expr = dynamic_cast<Variable*>(expr)) {
    if (dynamic_cast<UnresolvedSymbol*>(variable_expr->var)) {
      Symbol* new_symbol = Symboltable::lookup(variable_expr->var->name);
      if (new_symbol) {
	if (!dynamic_cast<FnSymbol*>(new_symbol)) {
	  variable_expr->var = new_symbol;
	}
      }
      else {
	if (strcmp(variable_expr->var->name, "__primitive")) {
	  USR_FATAL(expr, "Unable to resolve token '%s'",
		    variable_expr->var->name);
	}
      }
    }
  }
}

void ScopeResolveSymbols::preProcessSymbol(Symbol* sym) {
  resolve_type_helper(sym->type);
  FnSymbol* fnSym = dynamic_cast<FnSymbol*>(sym);
  if (fnSym) {
    resolve_type_helper(fnSym->retType);
  }
}
