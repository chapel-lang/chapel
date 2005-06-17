#include <typeinfo>
#include "scopeResolveSymbols.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"
#include "../traversals/fixup.h"


static void check_legal_overload(Symbol* sym) {
  if (sym->overload) {
    int count = 0;
    for (Symbol* tmp = sym; tmp; tmp = tmp->overload) {
      if (!tmp->getFnSymbol()) {
        count++;
      }
    }
    if (count >= 2) {
      char* redefinitionLocations = "";
      for (Symbol* tmp = sym->overload; tmp; tmp = tmp->overload) {
        if (!tmp->getFnSymbol()) {
          redefinitionLocations =
            glomstrings(3, redefinitionLocations, "\n  ", tmp->stringLoc());
        }
      }
      USR_FATAL(sym, "'%s' has multiple definitions, redefined at:%s",
                sym->name, redefinitionLocations);
    }
  }
}


static Symbol* resolveUnresolvedType(Vec<char*>* names) {
  Symbol* typeSym = Symboltable::lookup(names->v[0]);
  for (int i = 1; i < names->n; i++) {
    if (typeSym) {
      if (StructuralType* type = dynamic_cast<StructuralType*>(typeSym->type)) {
        typeSym = Symboltable::lookupInScope(names->v[i], type->structScope);
      }
    }
  }
  return typeSym;
}


static void resolve_type_helper(FnSymbol* currentFunction, Type* &type) {
  if (UnresolvedType* unresolvedType = dynamic_cast<UnresolvedType*>(type)) {
    Symbol* new_type = resolveUnresolvedType(unresolvedType->names);
    if (new_type) {
      check_legal_overload(new_type);
      if (ParamSymbol* param = dynamic_cast<ParamSymbol*>(new_type)) {
        type = param->typeVariable->type;
      } else if (ForwardingSymbol* forward =
          dynamic_cast<ForwardingSymbol*>(new_type)) {
        type = forward->forward->type;
      } else if (!dynamic_cast<UnresolvedType*>(new_type->type)) {
        type = new_type->type;
      } else {
        resolve_type_helper(currentFunction, new_type->type);
        type = new_type->type;
      }
    } else {
      INT_FATAL(type, "Error resolving type");
    }
  } else if (UserType* user_type = dynamic_cast<UserType*>(type)) {
    resolve_type_helper(currentFunction, user_type->definition);
    if (!user_type->defaultVal) {
      user_type->defaultVal = user_type->definition->defaultVal->copy();
      fixup(user_type->symbol->defPoint);
    }
  } else if (IndexType* index_type = dynamic_cast<IndexType*>(type)) {
    resolve_type_helper(currentFunction, index_type->idxType);
  } else if (ArrayType* array_type = dynamic_cast<ArrayType*>(type)) {
    resolve_type_helper(currentFunction, array_type->elementType);
  } else if (TupleType* tuple_type = dynamic_cast<TupleType*>(type)) {
    for (int i = 0; i < tuple_type->components.n; i++) {
      resolve_type_helper(currentFunction, tuple_type->components.v[i]);
    }
  } else if (MetaType* metaType = dynamic_cast<MetaType*>(type)) {
    if (UnresolvedType* unresolvedType = dynamic_cast<UnresolvedType*>(metaType->base)) {
      Symbol* new_type = resolveUnresolvedType(unresolvedType->names);
      if (new_type) {
        check_legal_overload(new_type);
        if (ParamSymbol* param = dynamic_cast<ParamSymbol*>(new_type)) {
          type = getMetaType(param->typeVariable->type);
        } else if (ForwardingSymbol* forward =
                   dynamic_cast<ForwardingSymbol*>(new_type)) {
          type = getMetaType(forward->forward->type);
        } else if (!dynamic_cast<UnresolvedType*>(new_type->type)) {
          type = getMetaType(new_type->type);
        } else {
          resolve_type_helper(currentFunction, new_type->type);
          type = getMetaType(new_type->type);
        }
      } else {
        INT_FATAL(type, "Error resolving type");
      }
    }
  }
}


ScopeResolveSymbols::ScopeResolveSymbols() {
  currentFunction = NULL;
  defList = new Map<SymScope*,Vec<VarSymbol*>*>();
}


void ScopeResolveSymbols::preProcessExpr(Expr* expr) {
  if (DefExpr* def_expr = dynamic_cast<DefExpr*>(expr)) {
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(def_expr->sym)) {
      currentFunction = fn;
    }
  }

  if (CastExpr* cast_expr = dynamic_cast<CastExpr*>(expr)) {
    resolve_type_helper(currentFunction, cast_expr->newType);
  }

  if (Variable* sym_use = dynamic_cast<Variable*>(expr)) {
    if (dynamic_cast<UnresolvedSymbol*>(sym_use->var)) {
      SymScope* currentScope = Symboltable::getCurrentScope();
      char* name = sym_use->var->name;
      
      if (!strcmp(name, "__primitive")) {
        return;
      }

      VarSymbol* sym_in_scope =
        dynamic_cast<VarSymbol*>(Symboltable::lookupInCurrentScope(name));
      
      if (sym_in_scope) {
        Vec<VarSymbol*>* sym_defs = defList->get(currentScope);
        if (!sym_defs || !sym_defs->set_in(sym_in_scope)) {
          USR_FATAL(expr, "Variable '%s' used before it is defined", name);
        }
      }
      
      Symbol* sym_resolve = Symboltable::lookup(name);
      
      if (VarSymbol* var_resolve = dynamic_cast<VarSymbol*>(sym_resolve)) {
        ModuleSymbol* mod =
          dynamic_cast<ModuleSymbol*>(var_resolve->parentScope->symContext);
        if (mod && currentScope == mod->initFn->body->blkScope) {
          Vec<VarSymbol*>* sym_defs = defList->get(currentScope);
          if (!sym_defs || !sym_defs->set_in(var_resolve)) {
            USR_FATAL(expr, "Variable '%s' used before it is defined", name);
          }
        }
      }

      if (sym_resolve) {
        check_legal_overload(sym_resolve);
        if (!dynamic_cast<FnSymbol*>(sym_resolve)) {
          if (ForwardingSymbol* forward =
              dynamic_cast<ForwardingSymbol*>(sym_resolve)) {
            if (!dynamic_cast<FnSymbol*>(forward->forward)) {
              sym_use->var = forward->forward;
              sym_use->forward = forward;
            }
          } else {
            sym_use->var = sym_resolve;
          }
        }
      } else {
        USR_FATAL(expr, "Symbol '%s' is not defined", name);
      }
    }
  }
}


void ScopeResolveSymbols::postProcessExpr(Expr* expr) {
  if (DefExpr* def_expr = dynamic_cast<DefExpr*>(expr)) {
    if (dynamic_cast<FnSymbol*>(def_expr->sym)) {
      currentFunction = NULL;
    } else {
      SymScope* currentScope = Symboltable::getCurrentScope();
      Vec<VarSymbol*>* new_vars = def_expr->varDefSet();
      if (new_vars) {
        Vec<VarSymbol*>* old_vars = defList->get(currentScope);
        if (old_vars) {
          old_vars->set_union(*new_vars);
        } else {
        defList->put(currentScope, new_vars);
        }
      }
    }
  }
}


void ScopeResolveSymbols::preProcessSymbol(Symbol* sym) {
  SymScope* paramScope = NULL;
  SymScope* saveScope = NULL;

  FnSymbol* fnSym = dynamic_cast<FnSymbol*>(sym);
  if (fnSym) {
    paramScope = fnSym->paramScope;

    /* for a function, its return type (also currently stored as its
       type... grumble grumble) may be defined in terms of symbols
       defined in the paramScope */
    if (paramScope) {
      saveScope = Symboltable::setCurrentScope(paramScope);
    }
  }

  if (sym->type) {
    resolve_type_helper(currentFunction, sym->type);
  }

  if (paramScope) {
    resolve_type_helper(currentFunction, fnSym->retType);
    Symboltable::setCurrentScope(saveScope);
  }
}
