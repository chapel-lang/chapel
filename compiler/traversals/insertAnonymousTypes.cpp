#include <typeinfo>
#include "insertAnonymousTypes.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"
#include "symtab.h"
#include "stringutil.h"


static void build_anon_array_type_def(Stmt* stmt, Type** type) {
  ArrayType* array_type = dynamic_cast<ArrayType*>(*type);

  if (!array_type) {
    INT_FATAL(stmt, "Array type expected");
  }

  if (array_type->symbol) {
    INT_FATAL(stmt, "Array type already resolved");
  }

  SymScope* saveScope;
  /** SJD: Why is SCOPE_INTRINSIC not internal? **/
  if (array_type->elementType->symbol->parentScope->isInternal() ||
      !array_type->elementType->symbol->defPoint) {
    saveScope = Symboltable::setCurrentScope(commonModule->modScope);
  }
  else {
    saveScope = Symboltable::setCurrentScope(array_type->elementType->symbol->parentScope);
  }
  if (ForallExpr* forall = dynamic_cast<ForallExpr*>(array_type->domain)) {
    if (Variable* var = dynamic_cast<Variable*>(forall->domains)) {
      if (var->next) {
	INT_FATAL(stmt, "Multiple domains not handled in declarations");
      }
      if (DomainType* domain_type = dynamic_cast<DomainType*>(var->var->type)) {
	char* name = glomstrings(5,
				 "_array_",
				 array_type->elementType->symbol->name,
				 "_",
				 intstring(domain_type->numdims), 
				 "d");
	if (Symbol* array_sym = Symboltable::lookupInCurrentScope(name)) {
	  *type = array_sym->type;
	}
	else {
	  TypeSymbol* array_sym = new TypeSymbol(name, array_type);
	  array_type->addSymbol(array_sym);
	  array_type->domainType = domain_type;
	  DefStmt* array_type_def = new DefStmt(array_sym);
	  array_sym->setDefPoint(array_type_def);
	  if (Symboltable::getCurrentScope() == commonModule->modScope) {
	    commonModule->stmts->insertAfter(array_type_def);
	  }
	  else {
	    Stmt* def_stmt = dynamic_cast<Stmt*>(array_type->elementType->symbol->defPoint);
	    if (!def_stmt) {
	      INT_FATAL(stmt, "Array with anonymous type not declared in statement not handled");
	    }
	    def_stmt->insertAfter(array_type_def);
	  }
	}
      }
      else {
	INT_FATAL(stmt, "Didn't find domain in this complicated case");
      }
    }
  }
  Symboltable::setCurrentScope(saveScope);
}


static void build_anon_tuple_type_def(Stmt* stmt, Type** type) {
  /***  Note I'm assuming a tuple with component types that are all
   ***  primitive types and I'm declaring this thing with a mangled
   ***  name in the commonModule.  This won't be possible when we
   ***  support tuples of different types.  In this case, they may
   ***  have to be defined in the scope they are used.
   ***/

  TupleType* tuple_type = dynamic_cast<TupleType*>(*type);

  if (!tuple_type) {
    INT_FATAL(*type, "Tuple type expected");
  }

  if (tuple_type->symbol) {
    INT_FATAL(tuple_type, "Tuple type already resolved");
  }

  SymScope* saveScope = Symboltable::setCurrentScope(commonModule->modScope);
  
  char* name = glomstrings(1, "_tuple");
  forv_Vec(Type, component, tuple_type->components) {
    name = glomstrings(3, name, "_", component->symbol->name);
  }
  if (Symbol* tuple_sym = Symboltable::lookupInCurrentScope(name)) {
    *type = tuple_sym->type;
  }
  else {
    TypeSymbol* tuple_sym = new TypeSymbol(name, tuple_type);
    tuple_type->addSymbol(tuple_sym);
    DefStmt* tuple_type_def = new DefStmt(tuple_sym);
    commonModule->stmts = appendLink(commonModule->stmts, tuple_type_def);
  }
  Symboltable::setCurrentScope(saveScope);
}


static void build_anon_domain_type_def(Stmt* stmt, Type** type) {
  /*** Note I'm only handling arithmetic domains and these are being
   *** put in the commonModule.
   ***/

  DomainType* domain_type = dynamic_cast<DomainType*>(*type);

  if (!domain_type) {
    INT_FATAL(*type, "Domain type expected");
  }

  if (domain_type->symbol) {
    INT_FATAL(domain_type, "Domain type already resolved");
  }

  if (domain_type->parent) {
    INT_FATAL(domain_type, "Only supporting arithmetic domains for now");
  }

  if (domain_type->numdims == 0) { // SJD: Duplicates original BLC assumption
    domain_type->numdims = 1;
  }

  char* name = glomstrings(3, "_domain_", intstring(domain_type->numdims), "d");
  Symbol* domain_sym = Symboltable::lookupInScope(name, commonModule->modScope);
  if (domain_sym) {
    *type = domain_sym->type;
  }
  else {
    SymScope* saveScope = Symboltable::setCurrentScope(commonModule->modScope);
    TypeSymbol* domain_sym = new TypeSymbol(name, domain_type);
    domain_type->addSymbol(domain_sym);
    DefStmt* domain_type_def = new DefStmt(domain_sym);
    domain_sym->setDefPoint(domain_type_def);
    commonModule->stmts->insertBefore(domain_type_def);
    Symboltable::setCurrentScope(saveScope);
    *type = domain_type;
  }
}

/*** don't pass tuple_type and type
 *** switch statement on type_id
 ***  get tuple_type etc. inside procedure
 ***/
static void build_anon_type_def(Stmt* stmt, Type** type) {
  if (typeid(**type) == typeid(ArrayType)) {
    build_anon_array_type_def(stmt, type);
  }
  else if (typeid(**type) == typeid(DomainType)) {
    build_anon_domain_type_def(stmt, type);
  }
  else if (typeid(**type) == typeid(TupleType)) {
    build_anon_tuple_type_def(stmt, type);
  }
}


void InsertAnonymousTypes::preProcessStmt(Stmt* stmt) {
  if (DefStmt* var_def = dynamic_cast<DefStmt*>(stmt)) {
    if (VarSymbol* var = dynamic_cast<VarSymbol*>(var_def->def_sym)) {
      build_anon_type_def(stmt, &var->type);
    }
  }
}
