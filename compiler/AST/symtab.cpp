#include <map>
#include <string>
#include "stmt.h"
#include "symtab.h"

using namespace std;

static map<string, Symbol*> table;

void Symboltable::define(Symbol* sym) {
  table[sym->name] = sym;
}

Symbol* Symboltable::lookup(char* name) {
  if (table.find(name) == table.end()) {
    return NULL;
  } else {
    return table[name];
  }
}


ParamSymbol* Symboltable::defineParams(paramType formaltag, Symbol* idents, 
				       Type* type) {
  ParamSymbol* paramList;
  ParamSymbol* newParam;

  paramList = new ParamSymbol(PARAM_INOUT, idents->name, type);
  newParam = paramList;
  idents = (Symbol*)(idents->next); // BLC: yuck!
  while (idents != NULL) {
    newParam->next = new ParamSymbol(PARAM_INOUT, idents->name, type);
    newParam = (ParamSymbol*)(newParam->next);  // BLC: yuck!
    idents = (Symbol*)(idents->next); // BLC: yuck!
  }

  return paramList;
}


Stmt* Symboltable::defineVars(varType vartag, bool isConst, 
			      Symbol* idents, Type* type, Expr* init) {
  
  VarSymbol* paramList;
  VarSymbol* newParam;

  paramList = new VarSymbol(idents->name, vartag, isConst, type);

  newParam = paramList;
  idents = (Symbol*)(idents->next); // BLC: yuck!
  while (idents != NULL) {
    newParam->next = new VarSymbol(idents->name, vartag, isConst, type);
    newParam = (VarSymbol*)(newParam->next);  // BLC: yuck!
    idents = (Symbol*)(idents->next); // BLC: yuck!
  }

  return new VarDefStmt(paramList, init);
}
