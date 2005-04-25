#include "insertIndexType.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"
#include "symtab.h"
#include "stringutil.h"
#include "view.h"
#include "fixup.h"

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
  
  IndexType* index_type = 
  dynamic_cast<IndexType*>(domain_type->idxType);
  if (!index_type){
    //RED This should be an anonymous domain, and thus, arithmetic
    //It would be more elegant if the domain type would have an init_expr field
    index_type = new IndexType(domain_type->initExpr);
  } 

  Symbol* index_sym = Symboltable::lookupInScope(name, commonModule->modScope);
  if (index_sym) {
    index_type = (IndexType*)index_sym->type;
    domain_type->idxType = index_sym->type;
    type = domain_type;
    /*** SJD: I couldn't figure out where to put this, but the
         defaultVal needs to be set for IndexTypes.  I don't know why
         this was working without this. **/
         //RED -- this code was never executed -- wonder how it worked with it :)
         //since type is always domain at this point...
    //if (TupleType* tuple_type = dynamic_cast<TupleType*>(type->getType())) {
    if (TupleType* tuple_type = dynamic_cast<TupleType*>(index_type->getType())) {
      tuple_type->rebuildDefaultVal();
      index_type->defaultVal = tuple_type->defaultVal->copy();
      fixup_expr(index_sym->defPoint);
    }
  } else {
    SymScope* saveScope = Symboltable::setCurrentScope(commonModule->modScope);
    TypeSymbol* index_sym = new TypeSymbol(name, index_type);
    index_type->addSymbol(index_sym);
    DefExpr* def_expr = new DefExpr(index_sym);
    DefStmt* def_stmt = new DefStmt(def_expr);
    if (!index_type->defaultVal) {
      index_type->defaultVal = index_type->idxType->defaultVal->copy();
    }
    commonModule->stmts->insertBefore(def_stmt);
    Symboltable::setCurrentScope(saveScope);
  }
}
