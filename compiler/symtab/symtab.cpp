#include <typeinfo>
#include "driver.h"
#include "expr.h"
#include "misc.h"
#include "printSymtab.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"
#include "yy.h"
#include "../passes/filesToAST.h"


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
static SymScope* preludeScope = NULL;
static SymScope* postludeScope = NULL;
static SymScope* currentScope = NULL;
static ModuleSymbol* firstModule = NULL;
static ModuleSymbol* currentModule = NULL;

ModuleSymbol* commonModule = NULL;
SymScope* internalScope = NULL;

void Symboltable::init(void) {
  rootScope = new SymScope(SCOPE_INTRINSIC);
  currentScope = rootScope;
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

  // Setup common module and scope
  // Hacked nested module, for declaring arrays-of-primitives types, e.g.
  commonModule = new ModuleSymbol("_CommonModule", false);
  commonModule->setModScope(new SymScope(SCOPE_MODULE));
  commonModule->modScope->parent = preludeScope;
  commonModule->modScope->setContext(NULL, commonModule);

  preludeScope->child = commonModule->modScope;
  currentScope = commonModule->modScope;

  commonModule->stmts = new NoOpStmt();
  SET_BACK(commonModule->stmts);

  firstModule = appendLink(firstModule, commonModule);

  prelude->modScope = preludeScope;          // SJD: Why here?
  internalPrelude->modScope = internalScope; // I just put it here
                                             // But it should go elsewhere
  preludeScope->symContext = prelude;
  internalScope->symContext = internalPrelude;
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
  Symbol* sym;
  if (publicSym) {
    sym = lookupInScope(name, preludeScope);
  } else {
    sym = lookupInScope(name, internalScope);
  }
  if (!sym) {
    INT_FATAL("lookupInternal failed on %s", name);
  }
  return sym;
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

static bool ModuleDefContainsOnlyNestedModules(Stmt* def) {
  Stmt* stmt = def;

  while (stmt) {
    if (DefStmt* def_stmt = dynamic_cast<DefStmt*>(stmt)) {
      if (!def_stmt->moduleDef()) {
	return false;
      }
    }
    else {
      return false;
    }
    
    stmt = nextLink(Stmt, stmt);
  }
  return true;
}


static Stmt* ModuleDefContainsNestedModules(Stmt* def) {
  Stmt* stmt = def;

  while (stmt) {
    if (DefStmt* def_stmt = dynamic_cast<DefStmt*>(stmt)) {
      if (def_stmt->moduleDef()) {
	return stmt;
      }
    }
    
    stmt = nextLink(Stmt, stmt);
  }
  return NULL;
}


DefExpr* Symboltable::finishModuleDef(ModuleSymbol* mod, Stmt* definition) {
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

  DefExpr* defExpr = new DefExpr(mod);

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
      modScope->setContext(NULL, mod, defExpr);
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

  return defExpr;
}

VarSymbol* Symboltable::defineVars(Symbol* idents, Type* type, Expr* init,
				   varType vartag, consType constag) {
  VarSymbol* varList;
  VarSymbol* newVar;
  VarSymbol* lastVar;

  // BLC: Placeholder until low-level type inference is hooked in to
  // handle some cases -- infer type from initializer if possible
  if (type == dtUnknown && init) {
    type = init->typeInfo();
  }
  
  newVar = new VarSymbol(idents->name, type, init->copy(), vartag, constag);

  varList = newVar;
  lastVar = newVar;
  //link.h=> next cell in list
  idents = nextLink(Symbol, idents);
  while (idents != NULL) {
    newVar = new VarSymbol(idents->name, type, init->copy(), vartag, constag);
    lastVar->next = newVar;
    lastVar = newVar;

    idents = nextLink(Symbol, idents);
  }

  return varList;
}

ParamSymbol* Symboltable::defineParams(paramType tag, Symbol* syms,
				       Type* type, Expr* init) {
  ParamSymbol* list = new ParamSymbol(tag, syms->name, type, init);
  syms = nextLink(Symbol, syms);
  while (syms != NULL) {
    list->append(new ParamSymbol(tag, syms->name, type, init));
    syms = nextLink(Symbol, syms);
  }
  return list;
}

DefExpr* Symboltable::defineVarDef(Symbol* idents, Type* type, 
				       Expr* init, varType vartag, 
				       consType constag) {
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
  VarSymbol* varList = defineVars(idents, type, init, vartag, constag);
  DefExpr* expr = new DefExpr(varList);
  varList->setDefPoint(expr);
  return expr;
}

DefExpr* Symboltable::defineVarDef1(Symbol* idents, Type* type, 
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
  DefExpr* expr = new DefExpr(varList);
  varList->setDefPoint(expr);
  return expr;
}

DefExpr* Symboltable::defineVarDef2(DefExpr* exprs,
					varType vartag, 
					consType constag) {
  DefExpr* expr = exprs;
  while (expr) {
    VarSymbol* var = dynamic_cast<VarSymbol*>(expr->sym);
    while (var) {
      var->consClass = constag;
      var->varClass = vartag;
      if (constag == VAR_PARAM && !var->init){
      	USR_FATAL(var->init, "No initializer for parameter.");
      }
      var = nextLink(VarSymbol, var);
    }
    expr = nextLink(DefExpr, expr);
  }
  return exprs;
}

DefStmt* Symboltable::defineSingleVarDefStmt(char* name, Type* type, 
					     Expr* init, varType vartag, 
					     consType constag) {
  Symbol* sym = new Symbol(SYMBOL, name);
  return new DefStmt(defineVarDef(sym, type, init, vartag, constag));
}

/* Converts expressions like i and j in [(i,j) in D] to symbols */
static Symbol* exprToIndexSymbols(Expr* expr, Symbol* indices = NULL) {
  if (!expr) {
    return indices;
  }

  for (Expr* tmp = expr; tmp; tmp = nextLink(Expr, tmp)) {
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


Expr* Symboltable::startLetExpr(void) {
  Symboltable::pushScope(SCOPE_LETEXPR);
  return new LetExpr(NULL, NULL);
}


Expr* Symboltable::finishLetExpr(Expr* let_expr, DefExpr* exprs, Expr* inner_expr) {
  LetExpr* let = dynamic_cast<LetExpr*>(let_expr);

  if (!let) {
    INT_FATAL(let_expr, "LetExpr expected");
  }

  DefExpr* expr = exprs;
  while (expr) {
    let->syms = appendLink(let->syms, expr->sym);
    expr = nextLink(DefExpr, expr);
  }

  let->setInnerExpr(inner_expr);

  SymScope* let_scope = Symboltable::popScope();
  let_scope->setContext(NULL, NULL, let);
  let->setLetScope(let_scope);
  let->syms->setDefPoint(let);
  return let;
}


ForallExpr* Symboltable::startForallExpr(Expr* domainExpr, Expr* indexExpr) {
  Symboltable::pushScope(SCOPE_FORALLEXPR);

  Symbol* indices = exprToIndexSymbols(indexExpr);
  // HACK: this is a poor assumption -- that all index variables are
  // integers

  return new ForallExpr(domainExpr, indices);
}


ForallExpr* Symboltable::finishForallExpr(ForallExpr* forallExpr, 
					  Expr* argExpr) {
  if (argExpr) {
    forallExpr->setForallExpr(argExpr);
  }
  SymScope* forallScope = Symboltable::popScope();
  forallScope->setContext(NULL, NULL, forallExpr);
  forallExpr->setIndexScope(forallScope);
  forallExpr->indices->setDefPoint(forallExpr);

  return forallExpr;
}


Type* Symboltable::defineBuiltinType(char* name, char* cname, Expr* init) {
  Type* newType = new Type(TYPE_BUILTIN, init);
  TypeSymbol* sym = new TypeSymbol(name, newType);
  sym->cname = copystring(cname);
  newType->addSymbol(sym);

  builtinTypes.add(newType);

  return newType;
}


FnSymbol* Symboltable::startFnDef(FnSymbol* fnsym) {
  Symboltable::pushScope(SCOPE_PARAM);

  return fnsym;
}


DefExpr* Symboltable::finishFnDef(FnSymbol* fnsym, Symbol* formals, 
				  Type* retType, Stmt* body, bool isExtern) {
  SymScope* paramScope = Symboltable::popScope();
  fnsym->finishDef(formals, retType, body, paramScope, isExtern);
  DefExpr* def_expr = new DefExpr(fnsym);
  paramScope->setContext(NULL, fnsym, def_expr);
  fnsym->setDefPoint(def_expr);
  formals->setDefPoint(def_expr);
  return def_expr;
}


DefStmt* Symboltable::defineFunction(char* name, Symbol* formals, 
				     Type* retType, Stmt* body, 
				     bool isExtern) {
  FnSymbol* fnsym = startFnDef(new FnSymbol(name));
  return new DefStmt(finishFnDef(fnsym, formals, retType, body, isExtern));
}


TypeSymbol* Symboltable::startClassDef(char* name, bool isValueClass, bool isUnion) {
  ClassType* newdt;
  TypeSymbol* newsym;

  newdt = new ClassType(isValueClass, isUnion);
  newsym = new TypeSymbol(name, newdt);
  (newdt)->addSymbol(newsym);
  Symboltable::pushScope(SCOPE_CLASS);

  return newsym;
}


DefExpr* Symboltable::finishClassDef(TypeSymbol* classSym, 
				     Stmt* definition) {
  ClassType* classType = dynamic_cast<ClassType*>(classSym->type);
  classType->addDeclarations(definition);
  SymScope *classScope = Symboltable::popScope();
  classType->setClassScope(classScope);
  DefExpr* classDefExpr = new DefExpr(classSym);
  classSym->setDefPoint(classDefExpr);
  classScope->setContext(NULL, classSym, classDefExpr);

  return classDefExpr;
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
  VarSymbol* newDomSym = new VarSymbol(name, unknownDomType, NULL, VAR_NORMAL, VAR_CONST);
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
  if (scope != NULL) {
    scope->traverse(traversal);
    traverseSymtab(traversal, scope->child);
    traverseSymtab(traversal, scope->sibling);
  }
}


void Symboltable::traverse(SymtabTraversal* traversal) {
  traverseSymtab(traversal, rootScope);
}


void Symboltable::traverseFromScope(SymtabTraversal* traversal,
				    SymScope* scope) {
  if (scope != NULL) {
    scope->traverse(traversal);
    traverseSymtab(traversal, scope->child);
  }
}
