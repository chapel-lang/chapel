#include <map>
#include <string>
#include "stmt.h"
#include "symtab.h"
#include "yy.h"

using namespace std;

class Scope {
 public:
  scopeType type;

  Scope* parent;
  Scope* children;
  Scope* sibling;

  //  Symbol* firstSym;
  //  Symbol* lastSym;
  map<string, Symbol*> table;

  Scope(scopeType init_type);

  void insert(Symbol* sym);

  void print(FILE* outfile = stdout);
};


Scope::Scope(scopeType init_type) :
  type(init_type),
  parent(NULL),
  children(NULL),
  sibling(NULL)
  //  firstSym(NULL),
  //  lastSym(NULL)
{}


void Scope::insert(Symbol* sym) {
  /*
  if (sym->next != NULL || sym->prev != NULL) {
    fprintf(stderr, "WARNING A: '%s' linked\n", sym->name);
  }
  fprintf(stderr, "Thought I was inserting '%s'\n", sym->name);
  */
  table[sym->name] = sym;
  /*
  if (sym->next != NULL || sym->prev != NULL) {
    fprintf(stderr, "WARNING B: '%s' linked\n", sym->name);
  }
  fprintf(stderr, "Got past map insertion\n");
  //  print(stderr);
  if (firstSym == NULL) {
    firstSym = sym;
    lastSym = sym;
  } else {
    if (sym->next != NULL || sym->prev != NULL) {
      fprintf(stderr, "WARNING C: '%s' linked\n", sym->name);
    }
    fprintf(stderr, "Last sym is: ");
    lastSym->print(stderr);
    fprintf(stderr, "\n");
    lastSym->append(sym);
    fprintf(stderr, "Got past append\n");
    lastSym = sym;
  }
  fprintf(stderr, "--------\n");
  */
}


void Scope::print(FILE* outfile) {
  fprintf(outfile, "======================================================\n");
  switch (type) {
  case SCOPE_INTRINSIC:
    fprintf(outfile, "SCOPE: global, standard, intrinsic\n");
    break;
  case SCOPE_FILE:
    fprintf(outfile, "SCOPE: file\n");
    break;
  case SCOPE_PARAM:
    fprintf(outfile, "SCOPE: parameters\n");
    break;
  case SCOPE_FUNCTION:
    fprintf(outfile, "SCOPE: function\n");
    break;
  case SCOPE_LOCAL:
    fprintf(outfile, "SCOPE: local\n");
    break;
  }
  /*
  fprintf(outfile, "------------------------------------------------------\n");
  if (firstSym != NULL) {
    firstSym->printList(outfile, "\n");
    fprintf(outfile, "\n");
  }
  */
  fprintf(outfile, "------------------------------------------------------\n");
  map<string, Symbol*>::iterator pos;
  pos = table.begin();
  while (pos != table.end()) {
    (*pos).second->print(outfile);
    fprintf(outfile, "\n");
    pos++;
  }
  fprintf(outfile, "======================================================\n");
}


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
  currentScope->insert(sym);
}


Symbol* Symboltable::lookup(char* name, bool genError) {
  Scope* scope;
  
  scope = currentScope;
  
  while (scope != NULL) {
    if (scope->table.find(name) != scope->table.end()) {
      return scope->table[name];
    }
    scope = scope->parent;
  }

  if (genError) {
    //    Symboltable::print(stderr);
    fprintf(stderr, "%s:%d '%s' used before defined\n", yyfilename, yylineno,
	    name);
    
  }
  return new Symbol(name);
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
  VarSymbol* lastParam;

  newParam = new VarSymbol(idents->name, vartag, isConst, type);
  define(newParam);

  paramList = newParam;
  lastParam = newParam;

  idents = (Symbol*)(idents->next); // BLC: yuck!
  while (idents != NULL) {
    newParam = new VarSymbol(idents->name, vartag, isConst, type);
    define(newParam);    
    lastParam->next = newParam;
    lastParam = newParam;

    idents = (Symbol*)(idents->next); // BLC: yuck!
  }

  return new VarDefStmt(paramList, init);
}


EnumSymbol* Symboltable::defineEnumList(Symbol* symList) {
  EnumSymbol* enumList;
  EnumSymbol* lastEnum;
  EnumSymbol* newEnum;
  Symbol* sym = symList;
  int val = 0;

  newEnum = new EnumSymbol(sym->name, val);
  define(newEnum);
  enumList = newEnum;
  lastEnum = newEnum;
  sym = (Symbol*)(sym->next);   // BLC: yuck!
  while (sym != NULL) {
    val++;
    newEnum = new EnumSymbol(sym->name, val);
    define(newEnum);
    lastEnum->append(newEnum);
    lastEnum = newEnum;

    sym = (Symbol*)(sym->next);  // BLC: yuck!
  }

  return enumList;
}


static void printHelp(FILE* outfile, Scope* aScope) {
  if (aScope == NULL) {
    return;
  } else {
    printHelp(outfile, aScope->parent);
    aScope->print(outfile);
  }
}


void Symboltable::print(FILE* outfile) {
  printHelp(outfile, currentScope);
}
