#include <typeinfo>
#include "driver.h"
#include "expr.h"
#include "misc.h"
#include "moduleList.h"
#include "printSymtab.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"
#include "yy.h"
#include "../passes/filesToAST.h"

enum parsePhaseType {
  PARSING_PRE,
  PARSING_PRELUDE,
  PARSING_USERFILES,
  PARSING_POST
};

static parsePhaseType parsePhase = PARSING_PRE;


static SymScope* rootScope = NULL;
static SymScope* preludeScope = NULL;
static SymScope* postludeScope = NULL;
static SymScope* currentScope = NULL;
static ModuleSymbol* currentModule = NULL;

ModuleSymbol* commonModule = NULL;

static ModuleList* moduleList = new ModuleList();

static void registerModule(ModuleSymbol* mod) {
  switch (mod->modtype) {
  case MOD_INTERNAL:
  case MOD_STANDARD:
  case MOD_COMMON:
  case MOD_USER:
    moduleList->insertAtTail(mod);
    break;
  case MOD_SENTINEL:
    INT_FATAL(mod, "Should never try to register a sentinel module");
  }
}

void Symboltable::init(void) {
  rootScope = new SymScope(SCOPE_INTRINSIC);
  currentScope = rootScope;
}


void Symboltable::parsePrelude(void) {
  preludeScope = new SymScope(SCOPE_PRELUDE);
  preludeScope->parent = rootScope;
  rootScope->child = preludeScope;

  currentScope = preludeScope;

  parsePhase = PARSING_PRELUDE;
}


void Symboltable::doneParsingPreludes(void) {
  parsePhase = PARSING_USERFILES;

  // Setup common module and scope
  // Hacked nested module, for declaring arrays-of-primitives types, e.g.
  commonModule = new ModuleSymbol("_CommonModule", MOD_COMMON);
  Symboltable::pushScope(SCOPE_MODULE);
  commonModule->setModScope(currentScope);
  commonModule->modScope->setContext(NULL, commonModule, NULL);

  commonModule->stmts->insertAtTail(new BlockStmt(new AList<Stmt>()));

  registerModule(commonModule);

  prelude->modScope = preludeScope;          // SJD: Why here?
  preludeScope->symContext = prelude;
}


void Symboltable::doneParsingUserFiles(void) {
  // pop common module scope
  Symboltable::popScope();
  postludeScope = new SymScope(SCOPE_POSTPARSE);
  postludeScope->parent = rootScope;
  preludeScope->sibling = postludeScope;

  currentScope = postludeScope;

  parsePhase = PARSING_POST;
}


bool Symboltable::parsingUserCode(void) {
  return (parsePhase == PARSING_USERFILES);
}


void Symboltable::removeScope(SymScope* scope) {
  if (scope->parent->child == scope) {
    scope->parent->child = scope->sibling;
    return;
  } else {
    for (SymScope* tmp = scope->parent->child; tmp; tmp = tmp->sibling) {
      if (tmp->sibling == scope) {
        tmp->sibling = scope->sibling;
        return;
      }
    }
  }
  INT_FATAL("Unable to remove SymScope");
}


void Symboltable::pushScope(scopeType type) {
  SymScope* newScope = new SymScope(type);
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

  if (currentScope->visibleFunctions.n > 0) {
    // visibleFunctions already computed
    newScope->setVisibleFunctions(NULL);
  }

  currentScope = newScope;
}


SymScope* Symboltable::popScope(void) {
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

  return oldScope;
}


ModuleSymbol* Symboltable::getCurrentModule(void) {
  return getCurrentScope()->getModule();
}


ModuleList* Symboltable::getModuleList(moduleSet whichModules) {
  moduleList->filter = whichModules;
  switch (whichModules) {
  case MODULES_ALL:
  case MODULES_CODEGEN:
  case MODULES_USER:
    return moduleList;
  default:
    INT_FATAL("Unexpected case in getModuleList: %d\n", whichModules);
    return NULL;
  }
}


void Symboltable::undefineInScope(Symbol* sym, SymScope* scope) {
  scope->remove(sym); // overload logic there, for some reason
}


void Symboltable::defineInScope(Symbol* sym, SymScope* scope) {
  Symbol* overload = Symboltable::lookupInScope(sym->name, scope);
  if (overload) {
    while (overload->overload) {
      overload = overload->overload;
    }
    overload->overload = sym;
    sym->setParentScope(overload->parentScope);
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
  if (!scope) {
    INT_FATAL("NULL scope passed to lookupFromScope()");
  }

  while (scope != NULL) {
    Symbol* sym = lookupInScope(name, scope);
    if (sym) {
      return sym;
    }
    if (scope->type == SCOPE_PARAM) {
      FnSymbol* fn = dynamic_cast<FnSymbol*>(scope->symContext);
      if (!fn) {
        INT_FATAL("Cannot find function from SCOPE_PARAM");
      }
      if (fn->typeBinding) {
        StructuralType* structuralType =
          dynamic_cast<StructuralType*>(fn->typeBinding->definition);
        if (structuralType) {
          Symbol* sym = lookupInScope(name, structuralType->structScope);
          if (sym) {
            return sym;
          }
        }
      }
    }
    forv_Vec(ModuleSymbol, module, scope->uses) {
      Symbol* sym = lookupInScope(name, module->modScope);
      if (sym) {
        return sym;
      }
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


Symbol* Symboltable::lookupInternal(char* name, scopeType scope) {
  Symbol* sym = NULL;

  switch (scope) {
  case SCOPE_INTRINSIC:
    sym = lookupInScope(name, rootScope);
    break;
  case SCOPE_PRELUDE:
    sym = lookupInScope(name, preludeScope);
    break;
  default:
    INT_FATAL("lookupInternal called with bad scope type");
  }
  if (!sym) {
    INT_FATAL("lookupInternal failed on %s", name);
  }

  return sym;
}


TypeSymbol* Symboltable::lookupInternalType(char* name) {
  Symbol* sym = lookupInternal(name, SCOPE_PRELUDE);
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


BlockStmt* Symboltable::finishCompoundStmt(BlockStmt* blkStmt, 
                                           AList<Stmt>* body) {
  blkStmt->addBody(body);

  SymScope* stmtScope = Symboltable::popScope();
  stmtScope->setContext(blkStmt);
  blkStmt->setBlkScope(stmtScope);

  return blkStmt;
}


ModuleSymbol* Symboltable::startModuleDef(char* name, modType modtype) {
  ModuleSymbol* newModule = new ModuleSymbol(name, modtype);

  // if this is a software rather than a file module and there
  // is a current module that is also a software module, then
  // this is a nested module, which we can't handle
  if (!newModule->isFileModule() && currentModule && 
      !currentModule->isFileModule()) {
    USR_FATAL(newModule, "Can't handle nested modules yet");
  } else {
    if (modtype != MOD_INTERNAL) {
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

bool ModuleDefContainsOnlyNestedModules(AList<Stmt>* def) {
  for_alist(Stmt, stmt, def) {
    if (ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(stmt)) {
      if (DefExpr* defExpr = dynamic_cast<DefExpr*>(expr_stmt->expr)) {
        if (!dynamic_cast<ModuleSymbol*>(defExpr->sym)) {
          return false;
        }
      } else {
        return false;
      }
    } else {
      return false;
    }
  }
  return true;
}


static Stmt* ModuleDefContainsNestedModules(AList<Stmt>* def) {
  for_alist(Stmt, stmt, def) {
    if (ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(stmt)) {
      if (DefExpr* defExpr = dynamic_cast<DefExpr*>(exprStmt->expr)) {
        if (dynamic_cast<ModuleSymbol*>(defExpr->sym)) {
          def->reset();
          return stmt;
        }
      }
    }
  }
  return NULL;
}


DefExpr* Symboltable::finishModuleDef(ModuleSymbol* mod, AList<Stmt>* def) {
  bool empty = false;
  if (mod->modtype != MOD_INTERNAL) {
    if (ModuleDefContainsOnlyNestedModules(def)) {
      // This is the case for a file module that contains a number
      // of software modules and nothing else.  Such modules should
      // essentially be dropped on the floor, as the file only
      // served as a container, not as a module
      empty = true;
    } else {
      Stmt* moduleDef = ModuleDefContainsNestedModules(def);
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
    registerModule(mod);

    // define the module's init function.  This should arguably go
    // in the module's scope, but that doesn't currently seem to
    // work.
    mod->stmts->add(def);
    // mod->createInitFn();

    // pop the module's scope
    if (mod->modtype != MOD_INTERNAL) {
      SymScope* modScope = Symboltable::popScope();
      modScope->setContext(NULL, mod, defExpr);
      mod->setModScope(modScope);
    }

    // if this was a software scope within a file scope, we borrowed
    // its (empty) scope in startModuleDef.  Give it a new scope to
    // work with here.
    if (mod->modtype != MOD_INTERNAL) {
      if (!mod->isFileModule()) {
        Symboltable::pushScope(SCOPE_MODULE);
      }
    }
  }

  // HACK: should actually look at parent module in general case
  currentModule = NULL;

  return defExpr;
}

static AList<VarSymbol>* symsToVars(AList<Symbol>* idents, Type* type) {
  AList<VarSymbol>* vlist = new AList<VarSymbol>();
  for_alist(Symbol, ident, idents) {
    vlist->insertAtTail(new VarSymbol(ident->name, type, VAR_NORMAL, VAR_VAR));
  }
  return vlist;
}

DefExpr*
Symboltable::defineParam(paramType tag, char* ident, Expr* type, Expr* init) {
  ParamSymbol* paramSymbol = new ParamSymbol(tag, ident, dtUnknown);
  Expr* userInit = init ? init : NULL;
  return new DefExpr(paramSymbol, userInit, type);
}


AList<Stmt>*
Symboltable::defineVarDef1(char* ident, Expr* type, Expr* init) {
  VarSymbol* varSymbol = new VarSymbol(ident, dtUnknown, VAR_NORMAL, VAR_VAR);
  Expr* userInit = init ? init : NULL;
  return new AList<Stmt>(new ExprStmt(new DefExpr(varSymbol, userInit, type)));
}


void Symboltable::defineVarDef2(AList<Stmt>* stmts, varType vartag, consType constag) {
  for_alist(Stmt, stmt, stmts) {
    if (ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(stmt)) {
      if (DefExpr* defExpr = dynamic_cast<DefExpr*>(exprStmt->expr)) {
        if (VarSymbol* var = dynamic_cast<VarSymbol*>(defExpr->sym)) {
          var->consClass = constag;
          var->varClass = vartag;
        } else {
          INT_FATAL(stmt, "Expected VarSymbol in Symboltable::defineVarDef2");
        }
      } else {
        INT_FATAL(stmt, "Expected DefExpr in Symboltable::defineVarDef2");
      }
    } else {
      INT_FATAL(stmt, "Expected ExprStmt in Symboltable::defineVarDef2");
    }
  }
}


DefExpr* Symboltable::defineSingleVarDef(char* name, Type* type, 
                                         Expr* init, varType vartag, 
                                         consType constag) {
  VarSymbol* var = new VarSymbol(name, type, vartag, constag);
  Expr* userInit = init ? init : NULL;
  return new DefExpr(var, userInit);
}


/* Converts expressions like i and j in [(i,j) in D] to symbols */
static AList<DefExpr>* exprToIndexSymbols(AList<Expr>* expr,
                                          AList<Expr>* domain, 
                                          AList<VarSymbol>* indices = new AList<VarSymbol>()) {
  if (expr) {
    for (Expr* tmp = expr->first(); tmp; tmp = expr->next()) {
      Variable* varTmp = dynamic_cast<Variable*>(tmp);
      if (!varTmp) {
        INT_FATAL(tmp, "Error");
      } else {
        // SJD: It is my thought that dtInteger should be dtUnknown. Then
        // analysis can figure it out based on the type of the "domain"
        // which may not be a domain but an array or a sequence too.
        indices->insertAtTail(new VarSymbol(varTmp->var->name, dtInteger));
      }
    }
  }
  AList<DefExpr>* defExprs = new AList<DefExpr>();
  if (!indices->isEmpty()) {
    VarSymbol* var = indices->popHead();
    while (var) {
      defExprs->insertAtTail(new DefExpr(var));
      var = indices->popHead();
    }
  }
  return defExprs;
}


Expr* Symboltable::startLetExpr(void) {
  Symboltable::pushScope(SCOPE_LETEXPR);
  return new LetExpr();
}


Expr* Symboltable::finishLetExpr(Expr* let_expr, AList<Stmt>* stmts, 
                                 Expr* inner_expr) {
  LetExpr* let = dynamic_cast<LetExpr*>(let_expr);

  if (!let) {
    INT_FATAL(let_expr, "LetExpr expected");
  }

  let->setSymDefs(stmts);
  let->setInnerExpr(inner_expr);

  SymScope* let_scope = Symboltable::popScope();
  let_scope->setContext(NULL, NULL, let);
  let->setLetScope(let_scope);
  return let;
}


ForallExpr* Symboltable::startForallExpr(AList<Expr>* domainExpr, 
                                         AList<Expr>* indexExpr) {
  Symboltable::pushScope(SCOPE_FORALLEXPR);
  
  AList<DefExpr>* indices = exprToIndexSymbols(indexExpr, domainExpr);
  // HACK: this is a poor assumption -- that all index variables are
  // integers

  return new ForallExpr(indices, domainExpr);
}


ForallExpr* Symboltable::finishForallExpr(ForallExpr* forallExpr, 
                                          Expr* argExpr) {
  forallExpr->innerExpr = argExpr;
  SymScope* forallScope = Symboltable::popScope();
  forallScope->setContext(NULL, NULL, forallExpr);
  forallExpr->indexScope = forallScope;
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


FnSymbol* Symboltable::startFnDef(FnSymbol* fnsym, bool noparens) {
  if (noparens) {
    if (getCurrentScope()->type != SCOPE_CLASS &&
        fnsym->typeBinding == NULL) {
      USR_FATAL(fnsym, 
                "Non-member functions must have parenthesized argument lists");
    }
  }

  Symboltable::pushScope(SCOPE_PARAM);

  return fnsym;
}


void Symboltable::continueFnDef(FnSymbol* fnsym, AList<DefExpr>* formals, 
                                Type* retType, bool isRef, Expr *whereExpr) {
  fnsym->continueDef(formals, retType, isRef, whereExpr);
  Symboltable::pushScope(SCOPE_FUNCTION);
}


FnSymbol* Symboltable::finishFnDef(FnSymbol* fnsym, BlockStmt* blockBody, 
                                   bool isExtern) {
  SymScope* fnScope = Symboltable::popScope();
  SymScope* paramScope = Symboltable::popScope();
  if (fnScope->type != SCOPE_FUNCTION ||
      paramScope->type != SCOPE_PARAM) {
    INT_FATAL(fnsym, "Scopes not popped correctly on finishFnDef");
  }
  if (blockBody->blkScope == NULL) {
    blockBody->blkScope = fnScope;
  }
  fnsym->finishDef(blockBody, paramScope, isExtern);
  paramScope->setContext(NULL, fnsym, NULL);
  return fnsym;
}


DefExpr* Symboltable::defineFunction(char* name, AList<DefExpr>* formals, 
                                     Type* retType, BlockStmt* body, 
                                     bool isExtern) {
  if (formals == NULL) {
    formals = new AList<DefExpr>();
  }
  FnSymbol* fnsym = startFnDef(new FnSymbol(name));
  continueFnDef(fnsym, formals, retType);
  return new DefExpr(finishFnDef(fnsym, body, isExtern));
}


DefExpr* Symboltable::defineStructType(char* name, // NULL = anonymous
                                       Type* type,
                                       SymScope* scope,
                                       AList<Stmt>* def) {
  StructuralType* structType = dynamic_cast<StructuralType*>(type);

  if (!structType) {
    INT_FATAL(type, "defineStructType called on non StructuralType");
  }

  TypeSymbol* sym = new TypeSymbol(name, structType);
  structType->addSymbol(sym);
  DefExpr* defExpr = new DefExpr(sym);
  scope->setContext(NULL, sym, defExpr);
  structType->setScope(scope);
  structType->addDeclarations(def);
  return defExpr;
}


ForLoopStmt* 
Symboltable::startForLoop(ForLoopStmtTag forLoopStmtTag,
                          AList<Symbol>* indices, 
                          Expr* iterator) {
  Symboltable::pushScope(SCOPE_FORLOOP);
  AList<VarSymbol>* indexVars = symsToVars(indices, dtInteger);
  AList<DefExpr>* defExpr = new AList<DefExpr>();
  VarSymbol* var = indexVars->popHead();
  while (var) {
    defExpr->insertAtTail(new DefExpr(var));
    var = indexVars->popHead();
  }
  return
    new ForLoopStmt(forLoopStmtTag, defExpr, new AList<Expr>(iterator));
}


ForLoopStmt* Symboltable::finishForLoop(ForLoopStmt* forstmt, Stmt* body) {
  if (BlockStmt* blockStmt = dynamic_cast<BlockStmt*>(body)) {
    forstmt->innerStmt = blockStmt;
  } else {
    forstmt->innerStmt = Symboltable::startCompoundStmt();
    forstmt->innerStmt = Symboltable::finishCompoundStmt(forstmt->innerStmt,
                                                     new AList<Stmt>(body));
  }

  forstmt->indexScope = Symboltable::popScope();
  forstmt->indexScope->setContext(forstmt);

  return forstmt;
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
  printit->run(NULL);
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
