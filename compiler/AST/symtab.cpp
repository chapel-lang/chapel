#include <typeinfo>
#include "expr.h"
#include "misc.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"
#include "yy.h"


static bool parsingInternalPrelude = true;
static bool parsingPrelude = false;


class SymLink : public ILink {
public:
  Symbol* pSym;
  
  SymLink(Symbol* init_pSym = nilSymbol);

  void traverse(Traversal* traversal);

  void print(FILE* outfile);
  void codegen(FILE* outfile);
};


SymLink::SymLink(Symbol* init_pSym) :
  pSym(init_pSym)
{}


void SymLink::traverse(Traversal* traversal) {
  INT_FATAL(this, "traverse not yet implemented for SymLinks");
}


void SymLink::print(FILE* outfile) {
  pSym->print(outfile);
}

void SymLink::codegen(FILE* outfile) {
  fprintf(outfile, "This is SymLink's codegen method.\n");
}


SymScope::SymScope(scopeType init_type, int init_level) :
  type(init_type),
  level(init_level),
  stmtContext(nilStmt),
  symContext(nilSymbol),
  parent(NULL),
  child(NULL),
  sibling(NULL),
  firstSym(NULL),
  lastSym(NULL),
  useBeforeDefSyms(NULL)
{}


void SymScope::setContext(Stmt* stmt, Symbol* sym) {
  stmtContext = stmt;
  symContext = sym;
}


void SymScope::insert(Symbol* sym) {
  table.put(sym->name, sym);

  SymLink* newLink = new SymLink(sym);

  if (firstSym == NULL) {
    firstSym = newLink;
    lastSym = firstSym;
  } else {
    lastSym->append(newLink);
    lastSym = newLink;
  }
}


SymScope* SymScope::findEnclosingScopeType(scopeType t) {
  if (type == t) {
    return this;
  } else {
    if (parent == NULL) {
      INT_FATAL("can't find scope");
    }
    return parent->findEnclosingScopeType(t);
  }
}


void SymScope::addUndefined(UseBeforeDefSymbol* sym) {
  SymLink* newLink = new SymLink(sym);
  useBeforeDefSyms = appendLink(useBeforeDefSyms, newLink);
}

void SymScope::addUndefinedToFile(UseBeforeDefSymbol* sym) {
  SymScope* scope = Symboltable::getCurrentScope();

  if (parsingInternalPrelude) {
    scope = findEnclosingScopeType(SCOPE_INTERNAL_PRELUDE);
  } else if (parsingPrelude) {
    INT_FATAL(sym, "undefined symbol in prelude: %s", sym->name);
  } else {
    scope = findEnclosingScopeType(SCOPE_FILE);
  }
  
  scope->addUndefined(sym);
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

      undefined = nextLink(SymLink, undefined);
    }
    exit(1);
  }
}


static char* indentStr(FILE* outfile, int level) {
  static char* spaces = "                                                     "
                        "                          ";
  int printLevel;
  switch (level) {
  case SCOPE_INTRINSIC:
    printLevel = 0;
    break;
  case SCOPE_INTERNAL_PRELUDE:
  case SCOPE_PRELUDE:
    printLevel = 1;
    break;
  default:
    printLevel = level;
  }

  int maxspaces = strlen(spaces);
  int offset = maxspaces-(2*printLevel);
  if (offset < 0) {
    offset = 0;
  }

  return spaces + offset;
}


void SymScope::print(FILE* outfile, bool tableOrder) {
  char* indent = indentStr(outfile, level);

  // don't bother printing empty scopes
  if (firstSym == NULL) {
    return;
  }

  fprintf(outfile, "%s", indent);
  fprintf(outfile, "======================================================\n");

  fprintf(outfile, "%s", indent);
  fprintf(outfile, "SCOPE: ");
  switch (type) {
  case SCOPE_INTRINSIC:
    fprintf(outfile, "intrinsic\n");
    break;
  case SCOPE_INTERNAL_PRELUDE:
    fprintf(outfile, "internal prelude\n");
    break;
  case SCOPE_PRELUDE:
    fprintf(outfile, "prelude\n");
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
  case SCOPE_CLASS:
    fprintf(outfile, "class\n");
    break;
  }

  fprintf(outfile, "%s", indent);
  fprintf(outfile, "------------------------------------------------------\n");

  if (tableOrder) {
    int i;
    Vec<Symbol*> symlist;

    table.get_values(symlist);
    for (i=0; i<symlist.n; i++) {
      fprintf(outfile, "%s", indent);
      symlist.v[i]->print(outfile);
      fprintf(outfile, "\n");
    }
  } else {
    if (firstSym != NULL) {
      fprintf(outfile, "%s", indent);
      firstSym->printList(outfile, glomstrings(2, "\n", indent));
      fprintf(outfile, "\n");
    }
  }

  fprintf(outfile, "%s", indent);
  fprintf(outfile, "======================================================\n");
}


static int level = SCOPE_INTRINSIC;
static SymScope* rootScope = NULL;
static SymScope* internalScope = NULL;
static SymScope* preludeScope = NULL;
static SymScope* currentScope = NULL;
static FnSymbol* currentFn = NULL;


void Symboltable::init(void) {
  rootScope = new SymScope(SCOPE_INTRINSIC, SCOPE_INTRINSIC);

  currentScope = rootScope;
  level = SCOPE_INTRINSIC;

  currentFn = nilFnSymbol;
}


void Symboltable::parseInternalPrelude(void) {
  internalScope = new SymScope(SCOPE_INTERNAL_PRELUDE, SCOPE_INTERNAL_PRELUDE);
  internalScope->parent = rootScope;
  rootScope->child = internalScope;

  currentScope = internalScope;
  level = SCOPE_PRELUDE;

  parsingInternalPrelude = true;
  parsingPrelude = false;
}


void Symboltable::parsePrelude(void) {
  preludeScope = new SymScope(SCOPE_PRELUDE, SCOPE_PRELUDE);
  preludeScope->parent = rootScope;
  internalScope->sibling = preludeScope;

  currentScope = preludeScope;
  level = SCOPE_PRELUDE;
  
  parsingInternalPrelude = false;
  parsingPrelude = true;
}


void Symboltable::doneParsingPreludes(void) {
  parsingInternalPrelude = false;
  parsingPrelude = false;
}


bool Symboltable::parsingUserCode(void) {
  return (!parsingInternalPrelude && !parsingPrelude);
}


void Symboltable::pushScope(scopeType type) {
  SymScope* newScope = new SymScope(type, ++level);
  SymScope* child = currentScope->child;

  if (child == NULL) {
    currentScope->child = newScope;
  } else {
    while (child->sibling != NULL) {
      child = child->sibling;
    }
    child->sibling = newScope;
  }
  newScope->parent = currentScope;
  currentScope = newScope;
}


SymScope* Symboltable::popScope(void) {
  //  currentScope->handleUndefined();

  level--;
  SymScope* topScope = currentScope;
  SymScope* prevScope = currentScope->parent;

  if (prevScope == NULL) {
    INT_FATAL("ERROR: popping too many scopes");
  } else {
    currentScope = prevScope;
  }
  return topScope;
}


SymScope* Symboltable::getCurrentScope(void) {
  return currentScope;
}


FnSymbol* Symboltable::getCurrentFn(void) {
  return currentFn;
}


void Symboltable::define(Symbol* sym) {
  currentScope->insert(sym);
}


Symbol* Symboltable::lookupInScope(char* name, SymScope* scope) {
  return scope->table.get(name);
}


Symbol* Symboltable::lookupInternal(char* name) {
  return lookupInScope(name, internalScope);
}


Symbol* Symboltable::lookup(char* name, bool inLexer) {
  SymScope* scope;
  
  scope = currentScope;
  
  while (scope != NULL) {
    Symbol* sym = lookupInScope(name, scope);
    if (sym != NULL) {
      return sym;
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


ClassSymbol* Symboltable::lookupClass(char* name) {
  Symbol* pst = lookup(name);
  
  if (typeid(*pst) != typeid(ClassSymbol)) {
    char* error = glomstrings(3, "Expected '", name, 
			      "' to be a class but it's not");
    yyerror(error);
    return NULL;
  } else {
    return (ClassSymbol*)pst;
  }
}


void Symboltable::startCompoundStmt(void) {
  scopeType type = SCOPE_LOCAL;
  if (currentScope->type == SCOPE_PARAM) {
    type = SCOPE_FUNCTION;
  }

  Symboltable::pushScope(type);
}


BlockStmt* Symboltable::finishCompoundStmt(Stmt* body) {
  SymScope* stmtScope = Symboltable::popScope();
  BlockStmt* newStmt = new BlockStmt(body);
  stmtScope->setContext(newStmt, currentFn);

  return newStmt;
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

  idents = nextLink(Symbol, idents);
  while (idents != NULL) {
    newParam = new ParamSymbol(PARAM_INOUT, idents->name, type);
    define(newParam);
    lastParam->next = newParam;
    lastParam = newParam;

    idents = nextLink(Symbol, idents);
  }

  return paramList;
}


VarSymbol* Symboltable::defineVars(Symbol* idents, Type* type, Expr* init,
				   varType vartag, bool isConst) {
  VarSymbol* varList;
  VarSymbol* newVar;
  VarSymbol* lastVar;

  // BLC: Placeholder until low-level type inference is hooked in to
  // handle some cases -- infer type from initializer if possible
  if (type == dtUnknown) {
    type = init->typeInfo();
  }

  newVar = new VarSymbol(idents->name, type, vartag, isConst);
  define(newVar);

  varList = newVar;
  lastVar = newVar;

  idents = nextLink(Symbol, idents);
  while (idents != NULL) {
    newVar = new VarSymbol(idents->name, type, vartag, isConst);
    define(newVar);    
    lastVar->next = newVar;
    lastVar = newVar;

    idents = nextLink(Symbol, idents);
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
  sym = nextLink(Symbol, sym);
  while (sym != NULL) {
    val++;
    newEnum = new EnumSymbol(sym->name, val);
    define(newEnum);
    lastEnum->append(newEnum);
    lastEnum = newEnum;

    sym = nextLink(Symbol, sym);
  }

  return enumList;
}


Type* Symboltable::defineBuiltinType(char* name, bool placeholder) {
  Type* newType = new Type(TYPE_BUILTIN);
  TypeSymbol* sym = new TypeSymbol(name, newType);
  newType->addName(sym);

  define(sym);
  if (!placeholder) {
    builtinTypes.add(newType);
  }

  return newType;
}


FnSymbol* Symboltable::startFnDef(char* name) {
  FnSymbol* newFn = new FnSymbol(name, currentFn);
  define(newFn);
  currentFn = newFn;
  Symboltable::pushScope(SCOPE_PARAM);

  return newFn;
}


FnDefStmt* Symboltable::finishFnDef(FnSymbol* fnsym, Symbol* formals, 
				    Type* retType, Stmt* body, bool isExtern) {
  SymScope* paramScope = Symboltable::popScope();
  fnsym->finishDef(formals, retType, body, isExtern);
  FnDefStmt* fnstmt = new FnDefStmt(fnsym);
  paramScope->setContext(fnstmt, fnsym);
  currentFn = currentFn->parentFn;

  return fnstmt;
}


FnDefStmt* Symboltable::defineFunction(char* name, Symbol* formals, 
				       Type* retType, Stmt* body, 
				       bool isExtern) {
  FnSymbol* fnsym = startFnDef(name);
  return finishFnDef(fnsym, formals, retType, body, isExtern);
}


ClassSymbol* Symboltable::startClassDef(char* name, ClassSymbol* parent) {
  ClassType* newdt;
  ClassSymbol* newsym;

  if (parent->isNull()) {
    newdt = new ClassType();
  } else {
    newdt = new ClassType(parent->getType());
  }
  if (!parent->isNull() && 
      parent->scope->level == SCOPE_PRELUDE && 
      strcmp(parent->name, "reduction") == 0) {
    newsym = new ReduceSymbol(name, newdt);
  } else {
    newsym = new ClassSymbol(name, newdt);
  }
  (newdt)->addName(newsym);
  define(newsym);
  Symboltable::pushScope(SCOPE_CLASS);

  return newsym;
}


TypeDefStmt* Symboltable::finishClassDef(ClassSymbol* classSym, 
					 Stmt* definition) {
  SymScope *classScope = Symboltable::popScope();

  ClassType* classType = dynamic_cast<ClassType*>(classSym->type);
  classType->addScope(classScope);
  classType->addDefinition(definition);
  TypeDefStmt* classdefStmt = new TypeDefStmt(classType);
  
  classScope->setContext(classdefStmt, classSym);

  return classdefStmt;
}


VarSymbol* Symboltable::startForLoop(Symbol* indices) {
  Symboltable::pushScope(SCOPE_FORLOOP);
  return defineVars(indices, dtInteger); // BLC: dtInteger is wrong
}


ForLoopStmt* Symboltable::finishForLoop(bool forall, VarSymbol* index,
					Expr* domain, Stmt* body) {
  SymScope* forScope = Symboltable::popScope();
  ForLoopStmt* newStmt = new ForLoopStmt(forall, index, domain, body);
  forScope->setContext(newStmt, currentFn);

  return newStmt;
}


MemberAccess* Symboltable::defineMemberAccess(Expr* base, char* member) {
  Type* baseType = base->typeInfo();
  Symbol* memberSym;

extern int analyzeNewAST;
  if (baseType == dtUnknown || analyzeNewAST) {
    memberSym = new UseBeforeDefSymbol(member);
  } else if (typeid(*baseType) == typeid(ClassType)) {
    ClassType* classType = (ClassType*)baseType;
    memberSym = Symboltable::lookupInScope(member, classType->scope);
  } else {
    memberSym = new UseBeforeDefSymbol(member);
  }
  return new MemberAccess(base, memberSym);
}


DomainExpr* Symboltable::defineQueryDomain(char* name) {
  DomainType* unknownDomType = new DomainType();
  VarSymbol* newDomSym = new VarSymbol(name, unknownDomType, VAR_NORMAL, true);
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


static void dumpHelp(FILE* outfile, SymScope* scope) {
  if (scope == NULL) {
    return;
  } else {
    scope->print(outfile);
    dumpHelp(outfile, scope->child);
    dumpHelp(outfile, scope->sibling);
  }
}


void Symboltable::dump(FILE* outfile) {
  dumpHelp(outfile, rootScope);
}
