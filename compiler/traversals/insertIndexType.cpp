#include "insertIndexType.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"
#include "symtab.h"
#include "stringutil.h"
#include "view.h"

void InsertIndexType::preProcessType(Type* type) {
  DomainType* domain_type = dynamic_cast<DomainType*>(type);

  if (!domain_type) {
    return;
  }
  
  char* name;
  
  if (!(domain_type->parent)){
    name = glomstrings(3, "_index_", intstring(domain_type->numdims), "d");
  }
  else{
    name = glomstrings(2, "_index_", domain_type->symbol->name);
  }
  
  IndexType* index_type = dynamic_cast<IndexType*>(domain_type->idxType);
  if (!index_type){
        return;
  } 
  
  Symbol* index_sym = Symboltable::lookupInScope(name, commonModule->modScope);
  if (index_sym) {
    type = index_sym->type;
  }
  else {
          SymScope* saveScope = Symboltable::setCurrentScope(commonModule->modScope);
        TypeSymbol* index_sym = new TypeSymbol(name, index_type);
        index_type->addSymbol(index_sym);
        DefExpr* def_expr = new DefExpr(index_sym);
        DefStmt* def_stmt = new DefStmt(def_expr);
        index_sym->setDefPoint(def_expr);
        domain_type->idxType = index_type;
                commonModule->stmts->insertBefore(def_stmt);
        Symboltable::setCurrentScope(saveScope);
        }
}
