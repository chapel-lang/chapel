#include "insertAnonymousTypes.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"
#include "symtab.h"
#include "stringutil.h"


static void build_anon_type_defs(VarDefStmt* stmt, Type* type) {
  if (ArrayType* array_type = dynamic_cast<ArrayType*>(type)) {
    if (!array_type->name->isNull()) {
      INT_FATAL(stmt, "Array type already resolved");
    }
    SymScope* save_scope;
    /** SJD: Why is SCOPE_INTRINSIC not internal? **/
    if (array_type->elementType->name->parentScope->isInternal() ||
	array_type->elementType->name->defPoint->isNull()) {
      save_scope = Symboltable::setCurrentScope(commonModule->modScope);
    }
    else {
      save_scope = Symboltable::setCurrentScope(array_type->elementType->name->parentScope);
    }
    if (ForallExpr* forall = dynamic_cast<ForallExpr*>(array_type->domain)) {
      if (Variable* var = dynamic_cast<Variable*>(forall->domains)) {
	if (!var->next->isNull()) {
	  INT_FATAL(stmt, "Multiple domains not handled in declarations");
	}
	if (DomainType* domain_type = dynamic_cast<DomainType*>(var->var->type)) {
	  char* name = glomstrings(5,
				   "_array_",
				   array_type->elementType->name->name,
				   "_",
				   intstring(domain_type->numdims), 
				   "d");
	  if (Symbol* array_symbol = Symboltable::lookupInCurrentScope(name)) {
	    stmt->var->type = array_symbol->type;
	  }
	  else {
	    TypeSymbol* array_symbol = new TypeSymbol(name, array_type);
	    array_type->addName(array_symbol);
	    array_type->domainType = domain_type;
	    TypeDefStmt* array_type_def = new TypeDefStmt(array_type);
	    if (Symboltable::getCurrentScope() == commonModule->modScope) {
	      commonModule->stmts = appendLink(commonModule->stmts, array_type_def);
	    }
	    else {
	      array_type->elementType->name->defPoint->insertAfter(array_type_def);
	    }
	  }
	}
	else {
	  INT_FATAL(stmt, "Didn't find domain in this complicated case");
	}
      }
    }
    Symboltable::setCurrentScope(save_scope);
  }
  else if (TupleType* tuple_type = dynamic_cast<TupleType*>(type)) {
    /***
     ***  Note I'm assuming a tuple with component types that are all
     ***  primitive types and I'm declaring this thing with a mangled
     ***  name in the commonModule.  This won't be possible when we
     ***  support tuples of different types.  In this case, they may
     ***  have to be defined in the scope they are used.
     ***/
    if (!tuple_type->name->isNull()) {
      INT_FATAL(stmt, "Tuple type already resolved");
    }
    SymScope* save_scope = Symboltable::setCurrentScope(commonModule->modScope);

    char* name = glomstrings(1, "_tuple");
    forv_Vec(Type, component, tuple_type->components) {
      name = glomstrings(3, name, "_", component->name->name);
    }
    if (Symbol* tuple_symbol = Symboltable::lookupInCurrentScope(name)) {
      stmt->var->type = tuple_symbol->type;
    }
    else {
      TypeSymbol* tuple_symbol = new TypeSymbol(name, tuple_type);
      tuple_type->addName(tuple_symbol);
      TypeDefStmt* tuple_type_def = new TypeDefStmt(tuple_type);
      commonModule->stmts = appendLink(commonModule->stmts, tuple_type_def);
    }
    Symboltable::setCurrentScope(save_scope);
  }
}

void InsertAnonymousTypes::preProcessStmt(Stmt* stmt) {
  if (VarDefStmt* var_def = dynamic_cast<VarDefStmt*>(stmt)) {
    build_anon_type_defs(var_def, var_def->var->type);
  }
}

void InsertAnonymousTypes::preProcessSymbol(Symbol* sym) {
  /***
   ***  Insert arithmetic domain type definitions in commonModule
   ***/
  if (DomainType* domain_type = dynamic_cast<DomainType*>(sym->type)) {
    if (!domain_type->name->isNull()) {
      return;
    }
    if (!domain_type->parent->isNull()) {
      INT_FATAL(sym, "Only supporting arithmetic domains for now");
    }
    if (domain_type->numdims == 0) {
      domain_type->numdims = 1;   // SJD: Ugh, duplicates BLC hack prior to
                                  // change that generalized domains and arrays
    }
    char* name = glomstrings(3, "_domain_", intstring(domain_type->numdims), "d");
    if (Symbol* domain_symbol = Symboltable::lookupInScope(name, commonModule->modScope)) {
      sym->type = domain_symbol->type;
    }
    else {
      SymScope* save_scope = Symboltable::setCurrentScope(commonModule->modScope);
      TypeSymbol* domain_symbol = new TypeSymbol(name, domain_type);
      domain_type->addName(domain_symbol);
      TypeDefStmt* domain_type_def = new TypeDefStmt(domain_type);
      commonModule->stmts = appendLink(commonModule->stmts, domain_type_def);
      Symboltable::setCurrentScope(save_scope);
      sym->type = domain_type;
    }
    return;
  }
  return;
}
