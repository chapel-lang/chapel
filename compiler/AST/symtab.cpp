#include <typeinfo>
#include "driver.h"
#include "expr.h"
#include "misc.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"
#include "yy.h"


enum parsePhaseType {
  PARSING_PRE,
  PARSING_INTERNAL_PRELUDE,
  PARSING_PRELUDE,
  PARSING_USERFILES,
  PARSING_POST
};

static parsePhaseType parsePhase = PARSING_PRE;


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
  exprContext(nilExpr),
  parent(NULL),
  child(NULL),
  sibling(NULL),
  firstSym(NULL),
  lastSym(NULL),
  useBeforeDefSyms(NULL)
{}


void SymScope::setContext(Stmt* stmt, Symbol* sym, Expr* expr) {
  stmtContext = stmt;
  symContext = sym;
  exprContext = expr;
}


bool SymScope::isEmpty(void) {
  return (firstSym == NULL);
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


void SymScope::addUndefined(UnresolvedSymbol* sym) {
  SymLink* newLink = new SymLink(sym);
  useBeforeDefSyms = appendLink(useBeforeDefSyms, newLink);
}

void SymScope::addUndefinedToFile(UnresolvedSymbol* sym) {
  SymScope* scope = Symboltable::getCurrentScope();

  switch (parsePhase) {
  case PARSING_INTERNAL_PRELUDE:
    scope = findEnclosingScopeType(SCOPE_INTERNAL_PRELUDE);
    break;
  case PARSING_PRELUDE:
    INT_FATAL(sym, "undefined symbol in prelude: %s", sym->name);
    break;
  default:
    scope = findEnclosingScopeType(SCOPE_MODULE);
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
      Symbol* sym = Symboltable::lookup(name, false, true);

      if (typeid(*sym) == typeid(UnresolvedSymbol)) {
	fprintf(stdout, "%s:%d ", undefined->filename, undefined->lineno);
	undefined->print(stdout);
	fprintf(stdout, "\n");
      }

      undefined = nextLink(SymLink, undefined);
    }
    exit(1);
  }
}


static char* indentStr(FILE* outfile, scopeType type, int level) {
  static char* spaces = "                                                     "
                        "                          ";
  int printLevel;
  switch (type) {
  case SCOPE_INTRINSIC:
  case SCOPE_POSTPARSE:
    printLevel = 0;
    break;
  case SCOPE_INTERNAL_PRELUDE:
  case SCOPE_PRELUDE:
    printLevel = 1;
    break;
  default:
    printLevel = level+1;
  }

  int maxspaces = strlen(spaces);
  int offset = maxspaces-(2*printLevel);
  if (offset < 0) {
    offset = 0;
  }

  return spaces + offset;
}


void SymScope::print(FILE* outfile, bool tableOrder) {
  char* indent = indentStr(outfile, type, level);

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
    fprintf(outfile, "intrinsic");
    break;
  case SCOPE_INTERNAL_PRELUDE:
    fprintf(outfile, "internal prelude");
    break;
  case SCOPE_PRELUDE:
    fprintf(outfile, "prelude");
    break;
  case SCOPE_MODULE:
    fprintf(outfile, "module");
    break;
  case SCOPE_PARAM:
    fprintf(outfile, "parameters");
    break;
  case SCOPE_FUNCTION:
    fprintf(outfile, "function");
    break;
  case SCOPE_LOCAL:
    fprintf(outfile, "local");
    break;
  case SCOPE_FORLOOP:
    fprintf(outfile, "for loop");
    break;
  case SCOPE_FORALLEXPR:
    fprintf(outfile, "forall expression");
    break;
  case SCOPE_CLASS:
    fprintf(outfile, "class");
    break;
  case SCOPE_POSTPARSE:
    fprintf(outfile, "post parsing");
    break;
  }
  Loc* scopeLoc = NULL;
  if (!symContext->isNull()) {
    fprintf(outfile, " ");
    symContext->print(outfile);
    scopeLoc = symContext;
  } else if (!exprContext->isNull()) {
    scopeLoc = exprContext;
  } else if (!stmtContext->isNull()) {
    scopeLoc = stmtContext;
  }
  if (scopeLoc) {
    fprintf(outfile, " (%s)", scopeLoc->stringLoc());
  }
  fprintf(outfile, "\n");

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


static int currentLevel = 0;
static SymScope* rootScope = NULL;
static SymScope* internalScope = NULL;
static SymScope* preludeScope = NULL;
static SymScope* postludeScope = NULL;
static SymScope* currentScope = NULL;
static FnSymbol* currentFn = NULL;
static ModuleSymbol* firstModule = NULL;
static ModuleSymbol* currentModule = NULL;
static Symbol* mainFn = NULL;


void Symboltable::init(void) {
  rootScope = new SymScope(SCOPE_INTRINSIC);
  mainFn = nilSymbol;

  currentScope = rootScope;

  currentFn = nilFnSymbol;
}


void Symboltable::parseInternalPrelude(void) {
  internalScope = new SymScope(SCOPE_INTERNAL_PRELUDE);
  internalScope->parent = rootScope;
  rootScope->child = internalScope;

  currentScope = internalScope;

  parsePhase = PARSING_INTERNAL_PRELUDE;
}


void Symboltable::parsePrelude(void) {
  preludeScope = new SymScope(SCOPE_PRELUDE);
  preludeScope->parent = rootScope;
  internalScope->sibling = preludeScope;

  currentScope = preludeScope;

  parsePhase = PARSING_PRELUDE;
}


void Symboltable::doneParsingPreludes(void) {
  parsePhase = PARSING_USERFILES;
}


void Symboltable::doneParsingUserFiles(void) {
  postludeScope = new SymScope(SCOPE_POSTPARSE);
  postludeScope->parent = rootScope;
  preludeScope->sibling = postludeScope;

  currentScope = postludeScope;

  parsePhase = PARSING_POST;
}


bool Symboltable::parsingUserCode(void) {
  return (parsePhase == PARSING_USERFILES);
}


void Symboltable::pushScope(scopeType type) {
  SymScope* newScope = new SymScope(type, ++currentLevel);
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

  currentLevel--;
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


SymScope* Symboltable::setCurrentScope(SymScope* newScope) {
  SymScope* oldScope = currentScope;

  currentScope = newScope;
  currentLevel = newScope->level;

  return oldScope;
}


ModuleSymbol* Symboltable::getModuleList(void) {
  return firstModule;
}


FnSymbol* Symboltable::getCurrentFn(void) {
  return currentFn;
}


void Symboltable::defineInScope(Symbol* sym, SymScope* scope) {
  Symbol* prevDefInScope = Symboltable::lookupInScope(sym->name, scope);
  if (prevDefInScope) {
    // only allow redefinition of functions in a single scope currently
    if (typeid(*sym) != typeid(FnSymbol) &&
	typeid(*sym) != typeid(FnSymbol)) {
      USR_FATAL(sym, "redefinition of symbol %s (previous definition at %s)",
		sym->name, prevDefInScope->stringLoc());
    }
  }
  scope->insert(sym);
}


void Symboltable::define(Symbol* sym) {
  defineInScope(sym, currentScope);
}


Symbol* Symboltable::lookupInScope(char* name, SymScope* scope) {
  return scope->table.get(name);
}


Symbol* Symboltable::lookupFromScope(char* name, SymScope* scope) {
  while (scope != NULL) {
    Symbol* sym = lookupInScope(name, scope);
    if (sym != NULL) {
      return sym;
    }
    scope = scope->parent;
  }

  UnresolvedSymbol* undefinedSym = new UnresolvedSymbol(name);

  return undefinedSym;
}


Symbol* Symboltable::lookupInCurrentScope(char* name) {
  return lookupInScope(name, currentScope);
}


Symbol* Symboltable::lookupInternal(char* name, bool publicSym) {
  if (publicSym) {
    return lookupInScope(name, preludeScope);
  } else {
    return lookupInScope(name, internalScope);
  }
}


TypeSymbol* Symboltable::lookupInternalType(char* name, bool publicSym) {
  Symbol* sym = lookupInternal(name, publicSym);
  TypeSymbol* retsym = dynamic_cast<TypeSymbol *>(sym);
  if (retsym == NULL) {
    INT_FATAL("lookupInternalType failed");
  }
  return retsym;
}


Symbol* Symboltable::lookup(char* name, bool genError, bool inLexer) {
  SymScope* scope;
  
  scope = currentScope;
  
  while (scope != NULL) {
    Symbol* sym = lookupInScope(name, scope);
    if (sym != NULL) {
      return sym;
    }

    scope = scope->parent;
  }

  if (genError) {
    Symboltable::dump(stderr);
    fail("lookup of %s failed", name);
  }

  if (!inLexer) {
    //    Symboltable::print(stdout);
    /*
    fprintf(stdout, "%s:%d '%s' used before defined\n", yyfilename, yylineno,
	    name);
    */
  }

  UnresolvedSymbol* undefinedSym = new UnresolvedSymbol(name);

  if (!inLexer) {
    currentScope->addUndefinedToFile(undefinedSym);
  }

  return undefinedSym;
}


BlockStmt* Symboltable::startCompoundStmt(void) {
  scopeType type = SCOPE_LOCAL;
  if (currentScope->type == SCOPE_PARAM) {
    type = SCOPE_FUNCTION;
  }

  Symboltable::pushScope(type);
  return new BlockStmt();
}


BlockStmt* Symboltable::finishCompoundStmt(BlockStmt* blkStmt, Stmt* body) {
  blkStmt->addBody(body);

  SymScope* stmtScope = Symboltable::popScope();
  stmtScope->setContext(blkStmt);

  return blkStmt;
}


ModuleSymbol* Symboltable::startModuleDef(char* name, bool internal) {
  ModuleSymbol* newModule = new ModuleSymbol(name, internal);

  // if this is a software rather than a file module and there
  // is a current module that is also a software module, then
  // this is a nested module, which we can't handle
  if (!newModule->isFileModule() && currentModule && 
      !currentModule->isFileModule()) {
    USR_FATAL(newModule, "Can't handle nested modules yet");
  } else {
    if (!internal) {
      // Typically all modules would push scopes;  however, if this
      // is a software module within a file module, and the file
      // module's scope is empty, we can re-use it for simplicity
      // This will have to change once we support nested modules
      // to work with the case where the file is empty up to the
      // first software module, but then contains other file module
      // code after the first software module.
      if (newModule->isFileModule()) {
	Symboltable::pushScope(SCOPE_MODULE);
      } else {
	if (!currentScope->isEmpty()) {
	  USR_FATAL(newModule, "Can't handle nested modules yet");
	}
      }
    }
  }

  currentModule = newModule;

  return newModule;
}


/*
static bool ModuleDefIsInteresting(Stmt* def) {
  Stmt* stmt = def;

  while (stmt) {
    if (!stmt->isNull()) {
      if (typeid(*stmt) != typeid(ModuleDefStmt)) {
	return true;
      }
    }
    
    stmt = nextLink(Stmt, stmt);
  }
  return false;
}
*/


static bool ModuleDefContainsOnlyNestedModules(Stmt* def) {
  Stmt* stmt = def;

  while (stmt) {
    if (!stmt->isNull()) {
      if (typeid(*stmt) != typeid(ModuleDefStmt)) {
	return false;
      }
    }
    
    stmt = nextLink(Stmt, stmt);
  }
  return true;
}


static Stmt* ModuleDefContainsNestedModules(Stmt* def) {
  Stmt* stmt = def;

  while (stmt) {
    if (!stmt->isNull()) {
      if (typeid(*stmt) == typeid(ModuleDefStmt)) {
	return stmt;
      }
    }
    
    stmt = nextLink(Stmt, stmt);
  }
  return NULL;
}


ModuleDefStmt* Symboltable::finishModuleDef(ModuleSymbol* mod, 
					    Stmt* definition) {
  bool empty = false;
  if (!mod->internal) {
    if (ModuleDefContainsOnlyNestedModules(definition)) {
      // This is the case for a file module that contains a number
      // of software modules and nothing else.  Such modules should
      // essentially be dropped on the floor, as the file only
      // served as a container, not as a module
      empty = true;
    } else {
      Stmt* moduleDef = ModuleDefContainsNestedModules(definition);
      // if the definition contains anything other than module
      // definitions, then this is the case of a file that contains
      // a software module and some other stuff, which is a nested
      // module, and we can't handle that case yet
      if (moduleDef) {
	USR_FATAL(moduleDef, "Can't handle nested modules yet");
      } else {
	// for now, define all modules in the prelude scope, since
	// they can't be nested
	defineInScope(mod, preludeScope);
      }
    }
  }

  ModuleDefStmt* modDefStmt = new ModuleDefStmt(mod);

  if (!empty) {
    firstModule = appendLink(firstModule, mod);

    // pop the module's scope
    if (!mod->internal) {
      SymScope* modScope = Symboltable::popScope();
      modScope->setContext(modDefStmt, mod);
    }

    // define the module's init function.  This should arguably go
    // in the module's scope, but that doesn't currently seem to
    // work.
    mod->stmts = definition;
    mod->createInitFn();

    // if this was a software scope within a file scope, we borrowed
    // its (empty) scope in startModuleDef.  Give it a new scope to
    // work with here.
    if (!mod->internal) {
      if (!mod->isFileModule()) {
	Symboltable::pushScope(SCOPE_MODULE);
      }
    }
  }

  // HACK: should actually look at parent module in general case
  currentModule = NULL;

  return modDefStmt;
}


TypeDefStmt* Symboltable::defineUserType(char* name, Type* definition, 
					 Expr* initializer) {
  UserType* newtype = new UserType(definition, initializer);
  Symbol* typeSym = new TypeSymbol(name, newtype);
  newtype->addName(typeSym);
  define(typeSym);
  return new TypeDefStmt(newtype);
}


ParamSymbol* Symboltable::defineParams(paramType intent, Symbol* idents, 
				       Type* type) {
  ParamSymbol* paramList;
  ParamSymbol* newParam;
  ParamSymbol* lastParam;

  newParam = new ParamSymbol(intent, idents->name, type);
  define(newParam);

  paramList = newParam;
  lastParam = newParam;

  idents = nextLink(Symbol, idents);
  while (idents != NULL) {
    newParam = new ParamSymbol(intent, idents->name, type);
    define(newParam);
    lastParam->next = newParam;
    lastParam = newParam;

    idents = nextLink(Symbol, idents);
  }

  return paramList;
}


ParamSymbol* Symboltable::copyParams(ParamSymbol* formals) {
  ParamSymbol* paramList;
  ParamSymbol* newParam;
  ParamSymbol* lastParam;

  newParam = dynamic_cast<ParamSymbol*>(formals->copy());
  define(newParam);
  
  paramList = newParam;
  lastParam = newParam;

  formals = nextLink(ParamSymbol, formals);
  while (formals != NULL) {
    newParam = dynamic_cast<ParamSymbol*>(formals->copy());
    define(newParam);
    lastParam->next = newParam;
    lastParam = newParam;

    formals = nextLink(ParamSymbol, formals);
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


VarDefStmt* Symboltable::defineVarDefStmt(Symbol* idents, Type* type, 
					  Expr* init, varType vartag, 
					  bool isConst) {

  /** SJD: This is a stopgap measure to deal with changing sequences
      into domains when the type of a declared variable is a domain.
      It replaces the syntax of assigning domains using square
      brackets.
  **/
  if (dynamic_cast<DomainType*>(type) &&
      !dynamic_cast<ForallExpr*>(init)) {
    if (dynamic_cast<Tuple*>(init)) {
      init = new ForallExpr(dynamic_cast<Tuple*>(init)->exprs);
    }
    else {
      init = new ForallExpr(init);
    }
  }

  VarSymbol* varList = defineVars(idents, type, init, vartag, isConst);
  return new VarDefStmt(varList, init);
}


/* Converts expressions like i and j in [(i,j) in D] to symbols */
static Symbol* exprToIndexSymbols(Expr* expr, Symbol* indices = nilSymbol) {

  for (Expr* tmp = expr; tmp && !(tmp->isNull()); tmp = nextLink(Expr, tmp)) {
    Variable* varTmp = dynamic_cast<Variable*>(tmp);

    if (!varTmp) {
      Tuple* tupTmp = dynamic_cast<Tuple*>(tmp);
      if (!tupTmp) {
	USR_FATAL(tmp, "Index variable expected");
      }
      else {
	return exprToIndexSymbols(tupTmp->exprs, indices);
      }
    }
    else {
      indices = appendLink(indices, new Symbol(SYMBOL, varTmp->var->name));
    }
  }
  return indices;
}


ForallExpr* Symboltable::startForallExpr(Expr* domainExpr, Expr* indexExpr) {
  Symboltable::pushScope(SCOPE_FORALLEXPR);

  VarSymbol* indexVars;
  if (indexExpr->isNull()) {
    indexVars = nilVarSymbol;
  } else {
    Symbol* newSyms = exprToIndexSymbols(indexExpr);
    // HACK: this is a poor assumption -- that all index variables are
    // integers
    indexVars = Symboltable::defineVars(newSyms, dtInteger);
  }

  return new ForallExpr(domainExpr, indexVars);
}


ForallExpr* Symboltable::finishForallExpr(ForallExpr* forallExpr, 
					  Expr* argExpr) {
  if (!argExpr->isNull()) {
    forallExpr->setForallExpr(argExpr);
  }

  SymScope* forallScope = Symboltable::popScope();
  forallScope->setContext(nilStmt, nilSymbol, forallExpr);

  return forallExpr;
}


Type* Symboltable::defineBuiltinType(char* name, char* cname, Expr* init) {
  Type* newType = new Type(TYPE_BUILTIN, init);
  TypeSymbol* sym = new TypeSymbol(name, newType);
  sym->cname = copystring(cname);
  newType->addName(sym);

  define(sym);
  builtinTypes.add(newType);

  return newType;
}


FnSymbol* Symboltable::startFnDef(char* name, bool insert) {
  FnSymbol* newFn = new FnSymbol(name, currentFn);
  if (insert) {
    define(newFn);
  }
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
  fnsym->paramScope = paramScope;
  currentFn = currentFn->parentFn;

  return fnstmt;
}


FnDefStmt* Symboltable::defineFunction(char* name, Symbol* formals, 
				       Type* retType, Stmt* body, 
				       bool isExtern) {
  FnSymbol* fnsym = startFnDef(name);
  return finishFnDef(fnsym, formals, retType, body, isExtern);
}


TypeSymbol* Symboltable::startClassDef(char* name, bool isValueClass) {
  ClassType* newdt;
  TypeSymbol* newsym;

  newdt = new ClassType(isValueClass);
  newsym = new TypeSymbol(name, newdt);
  (newdt)->addName(newsym);
  define(newsym);
  Symboltable::pushScope(SCOPE_CLASS);

  return newsym;
}


TypeDefStmt* Symboltable::finishClassDef(TypeSymbol* classSym, 
					 Stmt* definition) {
  SymScope *classScope = Symboltable::popScope();

  ClassType* classType = dynamic_cast<ClassType*>(classSym->type);
  classType->addScope(classScope);
  classType->addDefinition(definition);
  TypeDefStmt* classdefStmt = new TypeDefStmt(classType);
  
  classScope->setContext(classdefStmt, classSym);

  return classdefStmt;
}


ForLoopStmt* Symboltable::startForLoop(bool forall, Symbol* indices, 
				       Expr* domain) {
  Symboltable::pushScope(SCOPE_FORLOOP);
  // HACK: dtInteger is wrong -- same as with forallExpr HACK
  VarSymbol* indexVars = defineVars(indices, dtInteger);
  return new ForLoopStmt(forall, indexVars, domain);
}


ForLoopStmt* Symboltable::finishForLoop(ForLoopStmt* forstmt, Stmt* body) {
  forstmt->addBody(body);

  SymScope* forScope = Symboltable::popScope();
  forScope->setContext(forstmt);

  return forstmt;
}


ForallExpr* Symboltable::defineQueryDomain(char* name) {
  DomainType* unknownDomType = new DomainType();
  VarSymbol* newDomSym = new VarSymbol(name, unknownDomType, VAR_NORMAL, true);
  define(newDomSym); // may need to postpone this until statement point --BLC
  Variable* newDom = new Variable(newDomSym);

  return new ForallExpr(newDom);
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
