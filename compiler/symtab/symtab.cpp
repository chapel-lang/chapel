#include <typeinfo>
#include "driver.h"
#include "expr.h"
#include "misc.h"
#include "printSymtab.h"
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


static int currentLevel = 0;
static SymScope* rootScope = NULL;
static SymScope* internalScope = NULL;
static SymScope* preludeScope = NULL;
static SymScope* postludeScope = NULL;
static SymScope* currentScope = NULL;
static FnSymbol* currentFn = NULL;
static ModuleSymbol* firstModule = NULL;
static ModuleSymbol* currentModule = NULL;


void Symboltable::init(void) {
  rootScope = new SymScope(SCOPE_INTRINSIC);

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
    if (typeid(*sym) == typeid(FnSymbol) &&
	typeid(*prevDefInScope) == typeid(FnSymbol)) {
      FnSymbol* origFn = dynamic_cast<FnSymbol*>(prevDefInScope);
      FnSymbol* newFn = dynamic_cast<FnSymbol*>(sym);
      FnSymbol* lastOverload = origFn;
      while (lastOverload->overload) {
	lastOverload = lastOverload->overload;
      }
      // this is the equivalent of the .put above
      lastOverload->overload = newFn;
      newFn->setParentScope(origFn->parentScope);
    } else {
      USR_FATAL(sym, "redefinition of symbol %s (previous definition at %s)",
		sym->name, prevDefInScope->stringLoc());
    }
  } else {
    scope->insert(sym);
  }
}


void Symboltable::define(Symbol* sym) {
  defineInScope(sym, currentScope);
}


Symbol* Symboltable::lookupInScope(char* name, SymScope* scope) {
  if (scope == NULL) {
    INT_FATAL("NULL scope passed to lookupInScope()");
  }
  return scope->table.get(name);
}


Symbol* Symboltable::lookupFromScope(char* name, SymScope* scope,
				     bool genError) {
  if (scope == NULL) {
    INT_FATAL("NULL scope passed to lookupFromScope()");
  }
  while (scope != NULL) {
    Symbol* sym = lookupInScope(name, scope);
    if (sym != NULL) {
      return sym;
    }
    scope = scope->parent;
  }

  if (genError && strcmp(name, "__primitive") != 0) {
    INT_FATAL("lookupFromScope failed for %s", name);
  }

  return NULL;
}


Symbol* Symboltable::lookup(char* name, bool genError) {
  return lookupFromScope(name, currentScope, genError);
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
  blkStmt->setBlkScope(stmtScope);

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

    // define the module's init function.  This should arguably go
    // in the module's scope, but that doesn't currently seem to
    // work.
    mod->stmts = definition;
    mod->createInitFn();

    // pop the module's scope
    if (!mod->internal) {
      SymScope* modScope = Symboltable::popScope();
      modScope->setContext(modDefStmt, mod);
      mod->setModScope(modScope);
    }

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

  newVar = new VarSymbol(idents->name, type, init->copy(), vartag, isConst);

  varList = newVar;
  lastVar = newVar;

  idents = nextLink(Symbol, idents);
  while (idents != NULL) {
    newVar = new VarSymbol(idents->name, type, init->copy(), vartag, isConst);
    lastVar->next = newVar;
    lastVar = newVar;

    idents = nextLink(Symbol, idents);
  }

  return varList;
}


ParamSymbol* Symboltable::defineParams(paramType tag, Symbol* syms,
				       Type* type) {
  ParamSymbol* list = new ParamSymbol(tag, syms->name, type);
  syms = nextLink(Symbol, syms);
  while (syms != NULL) {
    list->append(new ParamSymbol(tag, syms->name, type));
    syms = nextLink(Symbol, syms);
  }
  return list;
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
  VarDefStmt* stmt = new VarDefStmt(varList);
  varList->setDefPoint(stmt);
  return stmt;
}


VarDefStmt* Symboltable::defineVarDefStmt1(Symbol* idents, Type* type, 
					   Expr* init) {

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

  VarSymbol* varList = defineVars(idents, type, init);
  VarDefStmt* stmt = new VarDefStmt(varList);
  varList->setDefPoint(stmt);
  return stmt;
}


VarDefStmt* Symboltable::defineVarDefStmt2(VarDefStmt* stmts,
					  varType vartag, 
					  bool isConst) {
  VarDefStmt* stmt = stmts;
  while (stmt) {
    VarSymbol* var = stmt->var;
    while (var) {
      var->isConst = isConst;
      var->varClass = vartag;
      var = nextLink(VarSymbol, var);
    }
    stmt = nextLink(VarDefStmt, stmt);
  }
  return stmts;
}


VarDefStmt* Symboltable::defineSingleVarDefStmt(char* name, Type* type, 
						Expr* init, varType vartag, 
						bool isConst, 
						Symbol** newVar) {
  Symbol* newVarAsSym = new Symbol(SYMBOL, name);
  VarDefStmt* retval = defineVarDefStmt(newVarAsSym, type, init, vartag, 
					isConst);
  *newVar = retval->var;

  return retval;
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

  Stmt* indexVars = nilStmt;
  if (!indexExpr->isNull()) {
    Symbol* newSyms = exprToIndexSymbols(indexExpr);
    // HACK: this is a poor assumption -- that all index variables are
    // integers
    indexVars = Symboltable::defineVarDefStmt(newSyms, dtInteger, 
						    nilExpr, VAR_NORMAL,
						    false);
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
  forallExpr->setIndexScope(forallScope);

  return forallExpr;
}


Type* Symboltable::defineBuiltinType(char* name, char* cname, Expr* init) {
  Type* newType = new Type(TYPE_BUILTIN, init);
  TypeSymbol* sym = new TypeSymbol(name, newType);
  sym->cname = copystring(cname);
  newType->addName(sym);

  builtinTypes.add(newType);

  return newType;
}


FnSymbol* Symboltable::startFnDef(FnSymbol* fnsym) {
  currentFn = fnsym;
  Symboltable::pushScope(SCOPE_PARAM);

  return fnsym;
}


FnDefStmt* Symboltable::finishFnDef(FnSymbol* fnsym, Symbol* formals, 
				    Type* retType, Stmt* body, bool isExtern) {
  SymScope* paramScope = Symboltable::popScope();
  fnsym->finishDef(formals, retType, body, paramScope, isExtern);
  FnDefStmt* fnstmt = new FnDefStmt(fnsym);
  paramScope->setContext(fnstmt, fnsym);
  fnsym->setDefPoint(fnstmt);
  formals->setDefPoint(fnstmt);
  return fnstmt;
}


FnDefStmt* Symboltable::defineFunction(char* name, Symbol* formals, 
				       Type* retType, Stmt* body, 
				       bool isExtern) {
  FnSymbol* fnsym = startFnDef(new FnSymbol(name));
  return finishFnDef(fnsym, formals, retType, body, isExtern);
}


TypeSymbol* Symboltable::startClassDef(char* name, bool isValueClass, bool isUnion) {
  ClassType* newdt;
  TypeSymbol* newsym;

  newdt = new ClassType(isValueClass, isUnion);
  newsym = new TypeSymbol(name, newdt);
  (newdt)->addName(newsym);
  Symboltable::pushScope(SCOPE_CLASS);

  return newsym;
}


TypeDefStmt* Symboltable::finishClassDef(TypeSymbol* classSym, 
					 Stmt* definition) {
  SymScope *classScope = Symboltable::popScope();

  ClassType* classType = dynamic_cast<ClassType*>(classSym->type);
  classType->setClassScope(classScope);
  classType->addDefinition(definition);
  TypeDefStmt* classdefStmt = new TypeDefStmt(classType);
  classSym->setDefPoint(classdefStmt);
  classScope->setContext(classdefStmt, classSym);

  return classdefStmt;
}


ForLoopStmt* Symboltable::startForLoop(bool forall, Symbol* indices, 
				       Expr* domain) {
  Symboltable::pushScope(SCOPE_FORLOOP);
  // HACK: dtInteger is wrong -- same as with forallExpr HACK
  VarSymbol* indexVars = defineVars(indices, dtInteger);
  ForLoopStmt* for_loop_stmt = new ForLoopStmt(forall, indexVars, domain);
  indexVars->setDefPoint(for_loop_stmt);
  return for_loop_stmt;
}


ForLoopStmt* Symboltable::finishForLoop(ForLoopStmt* forstmt, Stmt* body) {
  forstmt->addBody(body);

  SymScope* forScope = Symboltable::popScope();
  forScope->setContext(forstmt);
  forstmt->setIndexScope(forScope);

  return forstmt;
}


ForallExpr* Symboltable::defineQueryDomain(char* name) {
  DomainType* unknownDomType = new DomainType();
  VarSymbol* newDomSym = new VarSymbol(name, unknownDomType, nilExpr, VAR_NORMAL, true);
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


void Symboltable::dump(FILE* outfile) {
  PrintSymtab* printit = new PrintSymtab(outfile);
  printit->run();
}


static void traverseSymtab(SymtabTraversal* traversal, SymScope* scope) {
  if (scope == NULL) {
    return;
  } else {
    scope->traverse(traversal);
    traverseSymtab(traversal, scope->child);
    traverseSymtab(traversal, scope->sibling);
  }
}


void Symboltable::traverse(SymtabTraversal* traversal) {
  traverseSymtab(traversal, rootScope);
}


