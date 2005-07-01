#include <typeinfo>
#include "insertAnonymousTypes.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"
#include "symtab.h"
#include "stringutil.h"
#include "fixup.h"

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
    if (forall->domains->length() > 1) {
      INT_FATAL(stmt, "Multiple domains not handled in declarations");
    }
    if (Variable* var = dynamic_cast<Variable*>(forall->domains->only())) {
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
          array_type->defaultVal = COPY(array_type->elementType->defaultVal);
          DefExpr* def_expr = new DefExpr(array_sym);
          DefStmt* array_type_def = new DefStmt(def_expr);
          if (Symboltable::getCurrentScope() == commonModule->modScope) {
            commonModule->stmts->insertAtTail(array_type_def);
          }
          else {
            Stmt* def_stmt = array_type->elementType->symbol->defPoint->parentStmt;
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
  TupleType* tuple_type = dynamic_cast<TupleType*>(*type);

  if (!tuple_type) {
    INT_FATAL(*type, "Tuple type expected");
  }

  if (tuple_type->symbol) {
    INT_FATAL(tuple_type, "Tuple type already resolved");
  }

  TypeSymbol* tupleSymbol = 
    TypeSymbol::lookupOrDefineTupleTypeSymbol(&tuple_type->components);

  *type = tupleSymbol->type;
}


static void build_anon_structural_type_def(Stmt* stmt, Type** type) {
  StructuralType* structType = dynamic_cast<StructuralType*>(*type);

  if (!structType) {
    INT_FATAL(*type, "Structural type expected");
  }

  if (structType->symbol) {
    return;
  }

  SymScope* saveScope = NULL;
  if (Symboltable::getCurrentModule()->initFn == stmt->parentFunction()) {
    saveScope = Symboltable::setCurrentScope(Symboltable::getCurrentModule()->modScope);
  }

  char* name = glomstrings(2, "_anon_type_", intstring(structType->id));
  TypeSymbol* typeSymbol = new TypeSymbol(name, structType);
  structType->addSymbol(typeSymbol);
  forv_Vec(FnSymbol, method, structType->methods) {
    method->typeBinding = typeSymbol;
  }
  DefExpr* defExpr = new DefExpr(typeSymbol);
  structType->structScope->setContext(NULL, typeSymbol, defExpr);
  DefStmt* defStmt = new DefStmt(defExpr);
  if (saveScope) {
    Symboltable::setCurrentScope(saveScope);
    Symboltable::getCurrentModule()->stmts->insertAtHead(defStmt);
  } else {
    stmt->insertBefore(defStmt);
  }
}


static void build_anon_domain_type_def(Stmt* stmt, Type** type) {
  /*** Note I'm only handling arithmetic domains and these are being
   *** put in the commonModule.
   ***/

  DomainType* domain_type = dynamic_cast<DomainType*>(*type);

  if (!domain_type) {
    INT_FATAL(*type, "Domain type expected");
  }

  domain_type->computeRank();

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
    DefExpr* def_expr = new DefExpr(domain_sym);
    DefStmt* domain_type_def = new DefStmt(def_expr);
    commonModule->stmts->insertAtHead(domain_type_def);
    Symboltable::setCurrentScope(saveScope);
    *type = domain_type;
  }
}

void build_index_type_def(Stmt* stmt, Type** type) {
  IndexType* index_type = dynamic_cast<IndexType*>(*type);
  DomainType* domain_type = NULL;
  
  if (!index_type) {
    INT_FATAL(*type, "Index type expected");
  }
 
  if (!dynamic_cast<IntLiteral*>(index_type->idxExpr)) {
    Variable* var = dynamic_cast<Variable*>(index_type->idxExpr);
    if (var){
      domain_type = dynamic_cast<DomainType*>(var->var->type);
      index_type->idxType = domain_type->idxType->idxType;
    }
    else{
      INT_FATAL(index_type, "Invalid index expression.");
    }
  }
  char* name = NULL; 
  
  if (dynamic_cast<IntLiteral*>(index_type->idxExpr)) {
    name = glomstrings(3, "_index_", intstring(index_type->idxExpr->intVal()), "d");
  }
  else{
    if (!domain_type->parent){
      name = glomstrings(3, "_index_", intstring(domain_type->numdims), "d");
    }
    else {
      name = glomstrings(2, "_index_", domain_type->symbol->name);
    }
  }
  
  if (!name){
    INT_FATAL(index_type, "Invalid index expression.");  
  }
  
  Symbol* index_sym = Symboltable::lookupInScope(name, commonModule->modScope);
  if (index_sym) {
    *type = index_sym->type;
  }
  else {
    SymScope* saveScope = Symboltable::setCurrentScope(commonModule->modScope);
    TypeSymbol* index_sym = new TypeSymbol(name, index_type);
    index_type->addSymbol(index_sym);
    //RED: maybe this should be handled differently
    //Since there is no initialization for index types yet, this can go out
    /*if (!index_type->defaultVal) {
      index_type->defaultVal = index_type->idxType->defaultVal->copy();
    }*/
    DefExpr* def_expr = new DefExpr(index_sym);
    DefStmt* index_type_def = new DefStmt(def_expr);
    commonModule->stmts->insertAtHead(index_type_def);
    Symboltable::setCurrentScope(saveScope);
    *type = index_type;
  }
}
/*** don't pass tuple_type and type
 *** switch statement on type_id
 ***  get tuple_type etc. inside procedure
 ***/
static void build_anon_type_def(Stmt* stmt, Type** type) {
  if (dynamic_cast<ArrayType*>(*type)) {
    ArrayType* array_type = dynamic_cast<ArrayType*>(*type);
    build_anon_type_def(stmt, &array_type->elementType);
    build_anon_array_type_def(stmt, type);
  } else if (dynamic_cast<DomainType*>(*type)) {
    build_anon_domain_type_def(stmt, type);
  } else if (dynamic_cast<TupleType*>(*type)) {
    build_anon_tuple_type_def(stmt, type);
  } else if (dynamic_cast<IndexType*>(*type)) {
    build_index_type_def(stmt, type);
  } else if (dynamic_cast<StructuralType*>(*type)) {
    build_anon_structural_type_def(stmt, type);
  }
}


void InsertAnonymousTypes::preProcessStmt(Stmt* stmt) {
  if (DefStmt* def_stmt = dynamic_cast<DefStmt*>(stmt)) {
    DefExpr* def_expr = def_stmt->defExpr;
    if (ExprType* symExprType = dynamic_cast<ExprType*>(def_expr->sym->type)) {
      def_expr->exprType = symExprType->expr->copy();
      symExprType->expr = NULL;
      fixup(def_expr);
    }
    if (VarSymbol* var = dynamic_cast<VarSymbol*>(def_expr->sym)) {
      build_anon_type_def(stmt, &var->type);
    }
  }
}
