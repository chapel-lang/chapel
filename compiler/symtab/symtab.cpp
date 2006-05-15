#include <typeinfo>
#include "driver.h"
#include "expr.h"
#include "misc.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"
#include "yy.h"
#include "runtime.h"


SymScope* rootScope = NULL;

Vec<ModuleSymbol*> allModules;     // Contains all modules
Vec<ModuleSymbol*> codegenModules; // Contains codegened modules
Vec<ModuleSymbol*> userModules;    // Contains user modules

void registerModule(ModuleSymbol* mod) {
  switch (mod->modtype) {
  case MOD_USER:
    userModules.add(mod);
  case MOD_INSTANTIATED:
  case MOD_STANDARD:
    codegenModules.add(mod);
  case MOD_INTERNAL:
    allModules.add(mod);
    break;
  default:
    INT_FATAL(mod, "Unable to register module");
  }
}

void Symboltable::init(void) {
  rootScope = new SymScope(SCOPE_INTRINSIC);
}


void Symboltable::removeScope(SymScope* scope) {
  if (scope->parent->child == scope) {
    scope->parent->child = scope->sibling;
    return;
  } else {
    for (SymScope* tmp = scope->parent->child; tmp; tmp = tmp->sibling) {
      if (tmp->sibling == scope) {
        tmp->sibling = scope->sibling;
        return;
      }
    }
  }
  INT_FATAL("Unable to remove SymScope");
}


SymScope* Symboltable::pushScope(scopeType type, SymScope* parent) {
  if (!parent)
    parent = rootScope;
  SymScope* newScope = new SymScope(type);
  SymScope* child = parent->child;

  if (child == NULL) {
    parent->child = newScope;
  } else {
    while (child->sibling != NULL) {
      child = child->sibling;
    }
    child->sibling = newScope;
  }
  newScope->parent = parent;

  return newScope;
}


Symbol* Symboltable::lookupInScope(char* name,
                                   SymScope* scope,
                                   Vec<SymScope*>* scopesAlreadyVisited) {
  if (!scope) {
    INT_FATAL("NULL scope passed to lookupInScope()");
  }

  if (scopesAlreadyVisited && scopesAlreadyVisited->set_in(scope)) {
    return NULL;
  }

  Symbol* sym = scope->table.get(name);
  if (!sym) {
    if (scope->uses.n) {
      if (!scopesAlreadyVisited)
        scopesAlreadyVisited = new Vec<SymScope*>();
      scopesAlreadyVisited->set_add(scope);
      forv_Vec(ModuleSymbol, module, scope->uses) {
        sym = lookupInScope(name, module->modScope, scopesAlreadyVisited);
        if (sym)
          break;
      }
    }
  }
  return sym;
}



Symbol* Symboltable::lookupInFileModuleScope(char* name) {
  if (fileModule == NULL) {
    INT_FATAL("File module scope is NULL in lookupInFileModuleScope()");
  }
  Symbol* sym = lookupInScope(name, fileModule->modScope);
  if (sym == NULL) {
    INT_FATAL("Couldn't find '%s' in file module scope", name);
  }
  return sym;
}


Symbol* Symboltable::lookupFromScope(char* name, SymScope* scope) {
  if (!scope) {
    INT_FATAL("NULL scope passed to lookupFromScope()");
  }
  while (scope != NULL) {
    Symbol* sym = lookupInScope(name, scope);
    if (sym) {
      return sym;
    }
    if (scope->type == SCOPE_ARG) {
      FnSymbol* fn = dynamic_cast<FnSymbol*>(scope->astParent);
      if (!fn) {
        INT_FATAL("Cannot find function from SCOPE_ARG");
      }
      if (fn->typeBinding) {
        ClassType* structuralType =
          dynamic_cast<ClassType*>(fn->typeBinding->definition);
        if (structuralType) {
          Symbol* sym = lookupInScope(name, structuralType->structScope);
          if (sym) {
            return sym;
          }
        }
      }
    }
    scope = scope->parent;
  }
  return NULL;
}


DefExpr*
Symboltable::defineParam(intentTag tag, char* ident, Expr* type, Expr* init, Expr* variable) {
  ArgSymbol* argSymbol = new ArgSymbol(tag, ident, dtUnknown, init, variable);
  if (tag == INTENT_TYPE) {
    char *name = stringcat("__type_variable_", argSymbol->name);
    VariableType* new_type = new VariableType(getMetaType(NULL));
    TypeSymbol* new_type_symbol = new TypeSymbol(name, new_type);
    argSymbol->type = getMetaType(NULL);
    argSymbol->genericSymbol = new_type_symbol;
  } 
#if 0
  else if (tag == INTENT_PARAM) {
    char *name = stringcat("__parameter_", argSymbol->name);
    VarSymbol *varSymbol = new VarSymbol(name, dtUnknown, VAR_NORMAL, VAR_PARAM);
    argSymbol->genericSymbol = varSymbol;
  }
#endif
  return new DefExpr(argSymbol, NULL, type);
}


void
setVarSymbolAttributes(AList<Stmt>* stmts, varType vartag, consType constag) {
  for_alist(Stmt, stmt, stmts) {
    if (ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(stmt)) {
      if (DefExpr* defExpr = dynamic_cast<DefExpr*>(exprStmt->expr)) {
        if (VarSymbol* var = dynamic_cast<VarSymbol*>(defExpr->sym)) {
          var->consClass = constag;
          var->varClass = vartag;
          continue;
        }
      }
    }
    INT_FATAL(stmt, "Major error in setVarSymbolAttributes");
  }
}


PrimitiveType* Symboltable::definePrimitiveType(char* name, char* cname, Symbol *initSymbol) {
  PrimitiveType *t = 
    dynamic_cast<PrimitiveType*>(defineBuiltinType(name, cname, new PrimitiveType(initSymbol)));
  if (initSymbol)
    initSymbol->type = t; // little chicken and the egg problem
  return t;
}

Type* Symboltable::defineBuiltinType(char* name, char* cname, Type *newType) {
  TypeSymbol* sym = new TypeSymbol(name, newType);
  rootScope->define(sym);
  sym->cname = stringcpy(cname);
  return newType;
}


DefExpr* Symboltable::defineStructType(char* name, // NULL = anonymous
                                       Type* type,
                                       AList<Stmt>* def) {
  ClassType* structType = dynamic_cast<ClassType*>(type);

  if (!structType) {
    INT_FATAL(type, "defineStructType called on non ClassType");
  }

  TypeSymbol* sym = new TypeSymbol(name, structType);
  DefExpr* defExpr = new DefExpr(sym);
  structType->addDeclarations(def);
  return defExpr;
}


PrimitiveType* 
Symboltable::createPrimitiveType (char      *name,
                                  char      *cname,
                                  char      *ltype_name,
                                  char      *ltype_cname) {
  PrimitiveType *newtype = Symboltable::definePrimitiveType (name, cname);
  if (ltype_name) {
    newtype->literalType = Symboltable::definePrimitiveType (ltype_name,
                                                             ltype_cname);
    newtype->literalType->dispatchParents.add (newtype);
  }

  return newtype;
}
