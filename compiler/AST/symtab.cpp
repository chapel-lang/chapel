#include <map>
#include <string>
#include "stmt.h"
#include "symtab.h"

using namespace std;

static map<string, Symbol*> table;


class Scope {
 public:
  scopeType type;
  Scope* parent;
  Scope* children;
  Scope* sibling;
  map<string, Symbol*> table;

  Scope(scopeType init_type);
};


Scope::Scope(scopeType init_type) :
  type(init_type),
  parent(NULL),
  children(NULL),
  sibling(NULL)
{}


static Scope* rootScope = new Scope(SCOPE_INTRINSIC);
static Scope* currentScope = rootScope;


void Symboltable::pushScope(scopeType type) {
  if (currentScope->type == SCOPE_PARAM &&
      type == SCOPE_LOCAL) {
    type = SCOPE_FUNCTION;
  }
  Scope* newScope = new Scope(type);
  Scope* children = currentScope->children;

  if (children == NULL) {
    currentScope->children = newScope;
  } else {
    while (children->sibling != NULL) {
      children = children->sibling;
    }
    children->sibling = newScope;
  }
  newScope->parent = currentScope;
  currentScope = newScope;
}


void Symboltable::popScope(void) {
  Scope* prevScope = currentScope->parent;

  if (prevScope == NULL) {
    fprintf(stderr, "ERROR: popping too many scopes");
    exit(1);
  } else {
    currentScope = prevScope;
  }
}


void Symboltable::define(Symbol* sym) {
  currentScope->table[sym->name] = sym;
}


Symbol* Symboltable::lookup(char* name) {
  Scope* scope;
  
  scope = currentScope;
  
  while (scope != NULL) {
    if (scope->table.find(name) != scope->table.end()) {
      return scope->table[name];
    }
    scope = scope->parent;
  }

  return NULL;
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
