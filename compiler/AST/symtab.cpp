#include <map>
#include <string>
#include <typeinfo>
#include "expr.h"
#include "misc.h"
#include "stmt.h"
#include "symtab.h"
#include "yy.h"

using namespace std;

class SymLink : public ILink {
public:
  Symbol* pSym;
  
  SymLink(Symbol* init_pSym = NULL);

  void print(FILE* outfile);
};


SymLink::SymLink(Symbol* init_pSym) :
  pSym(init_pSym)
{}


void SymLink::print(FILE* outfile) {
  pSym->print(outfile);
}


class SymScope {
 public:
  scopeType type;

  SymScope* parent;
  SymScope* children;
  SymScope* sibling;

  SymLink* firstSym;
  SymLink* lastSym;

  SymLink* useBeforeDefSyms;

  map<string, Symbol*> table;

  SymScope(scopeType init_type);

  void insert(Symbol* sym);
  SymScope* findFileScope(void);

  void addUndefined(UseBeforeDefSymbol*);
  void addUndefinedToFile(UseBeforeDefSymbol*);
  void handleUndefined(void);

  void print(FILE* outfile = stdout, bool alphabetical = false);
};


SymScope::SymScope(scopeType init_type) :
  type(init_type),
  parent(NULL),
  children(NULL),
  sibling(NULL),
  firstSym(NULL),
  lastSym(NULL),
  useBeforeDefSyms(NULL)
{}


void SymScope::insert(Symbol* sym) {
  table[sym->name] = sym;

  SymLink* newLink = new SymLink(sym);

  if (firstSym == NULL) {
    firstSym = newLink;
    lastSym = firstSym;
  } else {
    lastSym->append(newLink);
    lastSym = newLink;
  }
}


SymScope* SymScope::findFileScope(void) {
  if (type == SCOPE_FILE) {
    return this;
  } else {
    return parent->findFileScope();
  }
}


void SymScope::addUndefined(UseBeforeDefSymbol* sym) {
  SymLink* newLink = new SymLink(sym);
  if (useBeforeDefSyms == NULL) {
    useBeforeDefSyms = newLink;
  } else {
    useBeforeDefSyms->append(newLink);
  }
}

void SymScope::addUndefinedToFile(UseBeforeDefSymbol* sym) {
  SymScope* fileScope = findFileScope();
  
  fileScope->addUndefined(sym);
}


void SymScope::handleUndefined(void) {
  SymLink* undefined = useBeforeDefSyms;

  if (undefined) {
    fprintf(stdout, "Undefined symbols in file %s:\n", yyfilename);
    fprintf(stdout, "--------------------------------------------------\n");
    while (undefined != NULL) {
      char* name = undefined->pSym->name;
      Symbol* sym = Symboltable::lookup(name, true);

      if (typeid(*sym) == typeid(UseBeforeDefSymbol)) {
	fprintf(stdout, "%s:%d ", undefined->filename, undefined->lineno);
	undefined->print(stdout);
	fprintf(stdout, "\n");
      }

      undefined = (SymLink*)(undefined->next);  // BLC: cast!
    }
    exit(1);
  }
}


void SymScope::print(FILE* outfile, bool alphabetical) {
  fprintf(outfile, "======================================================\n");
  fprintf(outfile, "SCOPE: ");
  switch (type) {
  case SCOPE_INTRINSIC:
    fprintf(outfile, "global, standard, intrinsic\n");
    break;
  case SCOPE_FILE:
    fprintf(outfile, "file\n");
    break;
  case SCOPE_PARAM:
    fprintf(outfile, "parameters\n");
    break;
  case SCOPE_FUNCTION:
    fprintf(outfile, "function\n");
    break;
  case SCOPE_LOCAL:
    fprintf(outfile, "local\n");
    break;
  case SCOPE_FORLOOP:
    fprintf(outfile, "for loop\n");
    break;
  }
  fprintf(outfile, "------------------------------------------------------\n");

  if (alphabetical) {
    map<string, Symbol*>::iterator pos;
    pos = table.begin();
    while (pos != table.end()) {
      (*pos).second->print(outfile);
      fprintf(outfile, "\n");
      pos++;
    }  
  } else {
    if (firstSym != NULL) {
      firstSym->printList(outfile, "\n");
      fprintf(outfile, "\n");
    }
  }

  fprintf(outfile, "======================================================\n");
}


static SymScope* rootScope = new SymScope(SCOPE_INTRINSIC);
static SymScope* currentScope = rootScope;


void Symboltable::pushScope(scopeType type) {
  if (currentScope->type == SCOPE_PARAM &&
      type == SCOPE_LOCAL) {
    type = SCOPE_FUNCTION;
  }
  SymScope* newScope = new SymScope(type);
  SymScope* children = currentScope->children;

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
  //  currentScope->handleUndefined();

  SymScope* prevScope = currentScope->parent;

  if (prevScope == NULL) {
    INT_FATAL(NULL, "ERROR: popping too many scopes");
  } else {
    currentScope = prevScope;
  }
}


void Symboltable::define(Symbol* sym) {
  currentScope->insert(sym);
}


Symbol* Symboltable::lookup(char* name, bool inLexer) {
  SymScope* scope;
  
  scope = currentScope;
  
  while (scope != NULL) {
    if (scope->table.find(name) != scope->table.end()) {
      return scope->table[name];
    }
    scope = scope->parent;
  }

  if (!inLexer) {
    //    Symboltable::print(stdout);
    /*
    fprintf(stdout, "%s:%d '%s' used before defined\n", yyfilename, yylineno,
	    name);
    */
  }

  UseBeforeDefSymbol* undefinedSym = new UseBeforeDefSymbol(name);

  if (!inLexer) {
    currentScope->addUndefinedToFile(undefinedSym);
  }

  return undefinedSym;
}


ParamSymbol* Symboltable::defineParams(paramType formaltag, Symbol* idents, 
				       Type* type) {
  ParamSymbol* paramList;
  ParamSymbol* newParam;
  ParamSymbol* lastParam;

  newParam = new ParamSymbol(PARAM_INOUT, idents->name, type);
  define(newParam);

  paramList = newParam;
  lastParam = newParam;

  idents = (Symbol*)(idents->next); // BLC: yuck!
  while (idents != NULL) {
    newParam = new ParamSymbol(PARAM_INOUT, idents->name, type);
    define(newParam);
    lastParam->next = newParam;
    lastParam = newParam;

    idents = (Symbol*)(idents->next); // BLC: yuck!
  }

  return paramList;
}


VarSymbol* Symboltable::defineVars(Symbol* idents, Type* type, Expr* init,
				   varType vartag, bool isConst) {
  VarSymbol* varList;
  VarSymbol* newVar;
  VarSymbol* lastVar;

  newVar = new VarSymbol(idents->name, vartag, isConst, type);
  define(newVar);

  varList = newVar;
  lastVar = newVar;

  idents = (Symbol*)(idents->next); // BLC: yuck!
  while (idents != NULL) {
    newVar = new VarSymbol(idents->name, vartag, isConst, type);
    define(newVar);    
    lastVar->next = newVar;
    lastVar = newVar;

    idents = (Symbol*)(idents->next); // BLC: yuck!
  }

  return varList;
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


FunSymbol* Symboltable::defineFunction(char* name, Symbol* formals, 
				       Type* retType, Stmt* body) {
  FunSymbol* newFun = new FunSymbol(name, formals, retType, body);
  define(newFun);

  return newFun;
}


VarSymbol* Symboltable::enterForLoop(Symbol* indices) {
  Symboltable::pushScope(SCOPE_FORLOOP);
  return defineVars(indices, dtInteger); // BLC: dtInteger is wrong
}


void Symboltable::exitForLoop(void) {
  Symboltable::popScope();
}


DomainExpr* Symboltable::defineQueryDomain(char* name) {
  DomainType* unknownDomType = new DomainType();
  VarSymbol* newDomSym = new VarSymbol(name, VAR_NORMAL, true, unknownDomType);
  define(newDomSym); // may need to postpone this until statement point --BLC
  Variable* newDom = new Variable(newDomSym);

  return new DomainExpr(newDom);
}


static void printHelp(FILE* outfile, SymScope* aScope) {
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
