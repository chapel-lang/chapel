#include "insertIndexType.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"
#include "symtab.h"
#include "stringutil.h"
#include "view.h"
#include "fixup.h"

//RED: weird workaround for inserting the index type for an index i in forall i in D
//since the domain type is not available when indices are inserted in the table
//we get the i->type from D->idxType later on.
void InsertIndexType::preProcessStmt(Stmt* stmt) {
  if (_dtinteger_IndexType_switch) {
  ForLoopStmt* for_stmt = dynamic_cast<ForLoopStmt*>(stmt);
  
  if (!for_stmt){
    return;    
  }
  DomainType* domain_type = dynamic_cast<DomainType*>(for_stmt->domain->typeInfo());
  
  if (!domain_type) {
    return;
  }
  
  DefExpr* indices_def = for_stmt->indices->first();
  
  VarSymbol* aVar = dynamic_cast<VarSymbol*>(indices_def->sym);
  int i = 0;

  char* temp_name = aVar->name;
  Type* temp_type = dtUnknown;

  if (i != 0) temp_name = glomstrings(i + 3, "_", temp_name, aVar->name);
  i++;
  //RED: naive special casing to distinguish the non-integer indices from the 
  //list of integer symbols; hopefully better ways of testing will be revealed
  //after better handling of forall (i, j, k, ...) in  ... 
  if (i > 1) {
    //Symbol* indices = new Symbol(SYMBOL, name);
    //printf("Name is %s \n", name);
    temp_type = domain_type->idxType;
    //for_stmt->indices->replace(indices);
    DefStmt* def_stmt = Symboltable::defineSingleVarDefStmt(temp_name,
                                                            temp_type,
                                                            for_stmt->indices->first()->copy(),
                                                            VAR_NORMAL,
                                                            VAR_VAR);
    for_stmt->indices->first()->getStmt()->insertBefore(def_stmt);                                                          
    VarSymbol* var = dynamic_cast<VarSymbol*>(def_stmt->defExpr->sym);
    for_stmt->indices->first()->replace(new Variable(var));
  } else {
    indices_def->sym->type = domain_type->idxType;
  }
}
}

void InsertIndexType::preProcessType(Type* type) {
  DomainType* domain_type = dynamic_cast<DomainType*>(type);

  if (!domain_type) {
    return;
  }
  
  char* name = NULL;
  
  if (!(domain_type->parent)){
    name = glomstrings(3, "_index_", intstring(domain_type->numdims), "d");
  }
  else{
    name = glomstrings(2, "_index_", domain_type->symbol->name);
  }
  
  if (!domain_type->initExpr){
    INT_FATAL(domain_type, "Domain has no initialization expression");
  }
  
  IndexType* index_type = domain_type->idxType;
  if (!index_type){
    //RED This should be an anonymous domain, and thus, arithmetic
    //It would be more elegant if the domain type would have an init_expr field
    index_type = new IndexType(domain_type->initExpr);
  } 

  Symbol* index_sym = Symboltable::lookupInScope(name, commonModule->modScope);
  if (index_sym) {
    index_type = dynamic_cast<IndexType*>(index_sym->type);
    if (index_type == NULL) {
      INT_FATAL(index_sym, "does not have IndexType type");
    }
    domain_type->idxType = index_type;
    type = domain_type;
    /*** SJD: I couldn't figure out where to put this, but the
         defaultVal needs to be set for IndexTypes.  I don't know why
         this was working without this. **/
         //RED -- this code was never executed -- wonder how it worked with it :)
         //since type is always domain at this point...
    //if (TupleType* tuple_type = dynamic_cast<TupleType*>(type->getType())) {
    if (dynamic_cast<TupleType*>(index_type->getType())) {
      fixup(index_sym->defPoint);
    }
  } else {
    SymScope* saveScope = Symboltable::setCurrentScope(commonModule->modScope);
    TypeSymbol* index_sym = new TypeSymbol(name, index_type);
    index_type->addSymbol(index_sym);
    DefExpr* def_expr = new DefExpr(index_sym);
    DefStmt* def_stmt = new DefStmt(def_expr);
    if (!index_type->defaultVal) {
      index_type->defaultVal = COPY(index_type->idxType->defaultVal);
    }
    commonModule->stmts->insertAtHead(def_stmt);
    Symboltable::setCurrentScope(saveScope);
  }
}
