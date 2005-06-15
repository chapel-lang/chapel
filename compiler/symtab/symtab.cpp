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
  PARSING_INTERNAL_PRELUDE,
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
SymScope* internalScope = NULL;
bool _dtinteger_IndexType_switch = false;

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
  commonModule = new ModuleSymbol("_CommonModule", MOD_COMMON);
  Symboltable::pushScope(SCOPE_MODULE);
  commonModule->setModScope(currentScope);
  commonModule->modScope->setContext(NULL, commonModule, NULL);

  commonModule->stmts->insertAtTail(new NoOpStmt());

  registerModule(commonModule);

  prelude->modScope = preludeScope;          // SJD: Why here?
  internalPrelude->modScope = internalScope; // I just put it here
                                             // But it should go elsewhere
  preludeScope->symContext = prelude;
  internalScope->symContext = internalPrelude;
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
  if (!dynamic_cast<ForwardingSymbol*>(sym)) { // don't forward forwarding
    if (ModuleSymbol* mod = dynamic_cast<ModuleSymbol*>(scope->symContext)) {
      // Top level definition, add forwarding symbols to resolved uses
      forv_Vec(SymScope, scope, mod->usedBy) {
        SymScope* saveScope = Symboltable::setCurrentScope(scope);
        new ForwardingSymbol(sym);
        Symboltable::setCurrentScope(saveScope);
      }
    }
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
          dynamic_cast<StructuralType*>(fn->typeBinding->type);
        if (structuralType) {
          Symbol* sym = lookupInScope(name, structuralType->structScope);
          if (sym) {
            return sym;
          }
        }
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
  case SCOPE_INTERNAL_PRELUDE:
    sym = lookupInScope(name, internalScope);
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
  Symbol* sym = lookupInternal(name, SCOPE_INTERNAL_PRELUDE);
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
  Stmt* stmt = def->first();

  while (stmt) {
    if (DefStmt* def_stmt = dynamic_cast<DefStmt*>(stmt)) {
      for (DefExpr* defExpr = def_stmt->defExprls->first();
           defExpr;
           defExpr = def_stmt->defExprls->next()) {
        if (!dynamic_cast<ModuleSymbol*>(defExpr->sym)) {
          return false;
        }
      }
    } else {
      return false;
    }
    
    stmt = def->next();
  }
  return true;
}


static Stmt* ModuleDefContainsNestedModules(AList<Stmt>* def) {
  Stmt* stmt = def->first();

  while (stmt) {
    if (DefStmt* def_stmt = dynamic_cast<DefStmt*>(stmt)) {
      for (DefExpr* defExpr = def_stmt->defExprls->first();
           defExpr;
           defExpr = def_stmt->defExprls->next()) {
        if (dynamic_cast<ModuleSymbol*>(defExpr->sym)) {
          def->reset();
          def_stmt->defExprls->reset();
          return stmt;
        }
      }
    }
    
    stmt = def->next();
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

static AList<VarSymbol>* symsToVars(AList<Symbol>* idents, Type* type, 
                                    Expr* init = NULL, 
                                    varType vartag = VAR_NORMAL, 
                                    consType constag = VAR_VAR) {

  // BLC: Placeholder until low-level type inference is hooked in to
  // handle some cases -- infer type from initializer if possible
  if (type == dtUnknown && init) {
    type = init->typeInfo();
  }

  Symbol* ident = idents->first();
  AList<VarSymbol>* varList = new AList<VarSymbol>();
  while (ident) {
    VarSymbol* newVar;
    // SJD: All this code is hacky. Let's copy the type because it has
    // the domain expression and we need that to be duplicated for each
    // array variable.
    if (!type->symbol && dynamic_cast<ArrayType*>(type)) {
      newVar = new VarSymbol(ident->name, type->copy(), vartag, constag);
    } else {
      newVar = new VarSymbol(ident->name, type, vartag, constag);
    }
    varList->insertAtTail(newVar);

    ident = idents->next();
  }

  return varList;
}

AList<ParamSymbol>* Symboltable::defineParams(paramType tag, 
                                              AList<Symbol>* syms,
                                              Type* type, Expr* init) {
  Symbol* sym = syms->first();
  AList<Pragma>* pragma = sym->pragmas;
  AList<ParamSymbol>* list = new AList<ParamSymbol>();
  while (sym) {
    ParamSymbol* s = new ParamSymbol(tag, sym->name, type, init->copy());
    s->pragmas = pragma;
    list->insertAtTail(s);
    sym = syms->next();
  }

  return list;
}


AList<DefExpr>* Symboltable::defineVarDef1(AList<Symbol>* idents, Type* type, 
                                           Expr* init) {

  /** SJD: This is a stopgap measure to deal with changing sequences
      into domains when the type of a declared variable is a domain.
      It replaces the syntax of assigning domains using square
      brackets.
  **/
  
  if (dynamic_cast<DomainType*>(type) &&
      !dynamic_cast<ForallExpr*>(init)) {
    if (dynamic_cast<Tuple*>(init)) {
      init = new ForallExpr(dynamic_cast<Tuple*>(init)->exprs,
                            new AList<DefExpr>());
    }
    else {
      init = new ForallExpr(new AList<Expr>(init));
    }
  }

  AList<VarSymbol>* varList = symsToVars(idents, type, init);
  Symbol* var = varList->popHead();
  AList<DefExpr>* defExprs = new AList<DefExpr>();
  while (var) {
    defExprs->insertAtTail(new DefExpr(var, 
                              init ? new UserInitExpr(init->copy()) : NULL));
    
    
    var = varList->popHead();
  }

  return defExprs;
}


void Symboltable::defineVarDef2(AList<DefExpr>* exprs, varType vartag, 
                                consType constag) {
  DefExpr* expr = exprs->first();
  while (expr) {
    VarSymbol* var = dynamic_cast<VarSymbol*>(expr->sym);
    var->consClass = constag;
    var->varClass = vartag;
    expr = exprs->next();
  }
}


DefStmt* Symboltable::defineSingleVarDefStmt(char* name, Type* type, 
                                             Expr* init, varType vartag, 
                                             consType constag) {
  Symbol* sym = new Symbol(SYMBOL, name);
  AList<DefExpr>* defExpr = defineVarDef1(new AList<Symbol>(sym), type, init);
  defineVarDef2(defExpr, vartag, constag);
  return new DefStmt(defExpr);
}


/* Converts expressions like i and j in [(i,j) in D] to symbols */
static AList<DefExpr>* exprToIndexSymbols(AList<Expr>* expr,
                                          AList<Expr>* domain, 
                                          AList<VarSymbol>* indices = new AList<VarSymbol>()) {
  if (expr) {
    for (Expr* tmp = expr->first(); tmp; tmp = expr->next()) {
      Variable* varTmp = dynamic_cast<Variable*>(tmp);
      if (!varTmp) {
        Tuple* tupTmp = dynamic_cast<Tuple*>(tmp);
        if (!tupTmp) {
          USR_FATAL(tmp, "Index variable expected");
        } else {
          return exprToIndexSymbols(tupTmp->exprs, domain, indices);
        }
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


Expr* Symboltable::finishLetExpr(Expr* let_expr, AList<DefExpr>* exprs, 
                                 Expr* inner_expr) {
  LetExpr* let = dynamic_cast<LetExpr*>(let_expr);

  if (!let) {
    INT_FATAL(let_expr, "LetExpr expected");
  }

  let->setSymDefs(exprs);
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


void Symboltable::continueFnDef(FnSymbol* fnsym, AList<ParamSymbol>* formals, 
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


DefStmt* Symboltable::defineFunction(char* name, AList<ParamSymbol>* formals, 
                                     Type* retType, BlockStmt* body, 
                                     bool isExtern) {
  if (formals == NULL) {
    formals = new AList<ParamSymbol>();
  }
  FnSymbol* fnsym = startFnDef(new FnSymbol(name));
  continueFnDef(fnsym, formals, retType);
  return new DefStmt(new DefExpr(finishFnDef(fnsym, body, isExtern)));
}


Type* Symboltable::defineStructType(char* name, // NULL = anonymous
                                    Type* type,
                                    SymScope* scope,
                                    AList<Stmt>* def) {
  StructuralType* structType = dynamic_cast<StructuralType*>(type);

  if (!structType) {
    INT_FATAL(type, "defineStructType called on non StructuralType");
  }

  if (name) {
    TypeSymbol* sym = new TypeSymbol(name, structType);
    structType->addSymbol(sym);
    DefExpr* defExpr = new DefExpr(sym);
    scope->setContext(NULL, sym, defExpr);
  }
  structType->setScope(scope);
  structType->addDeclarations(def);
  return structType;
}


ForLoopStmt* Symboltable::startForLoop(bool forall, AList<Symbol>* indices, 
                                       Expr* domain) {
  Symboltable::pushScope(SCOPE_FORLOOP);
  AList<VarSymbol>* indexVars = NULL;
  //RED: attempt to put in practice SJD's belief
  if (!_dtinteger_IndexType_switch) {
    // SJD: It is my thought that dtInteger should be dtUnknown. Then
    // analysis can figure it out based on the type of the "domain"
    // which may not be a domain but an array or a sequence too.
    indexVars = symsToVars(indices, dtInteger);
  }
  //RED: here goes the index type logic
  //Index type will typically be the domain->idxType if this is known
  //E.g. for initialization of arrays
  //Or dtUnknown, otherwise -- e.g. when after parsing a domain does not
  //have an index type yet 
  else {
    DomainType* domain_type = dynamic_cast<DomainType*>(domain->typeInfo());
    if (domain_type) {
      indexVars = symsToVars(indices, domain_type->idxType);
    } else{
      //RED -- I think this should be dtUnknown if the index type of the domain is not known
      //this is saving us from dangerous assumptions; another possibility was to make it dtinteger
      //here, but I think that may cover incorrect behavior
      indexVars = symsToVars(indices, dtUnknown);
    }
  }
  AList<DefExpr>* defExpr = new AList<DefExpr>();
  VarSymbol* var = indexVars->popHead();
  while (var) {
    defExpr->insertAtTail(new DefExpr(var));
    var = indexVars->popHead();
  }
  ForLoopStmt* for_loop_stmt = new ForLoopStmt(forall, defExpr, domain);
  return for_loop_stmt;
}


ForLoopStmt* Symboltable::finishForLoop(ForLoopStmt* forstmt, Stmt* body) {
  forstmt->addBody(new AList<Stmt>(body));

  SymScope* forScope = Symboltable::popScope();
  forScope->setContext(forstmt);
  forstmt->setIndexScope(forScope);

  return forstmt;
}


ForallExpr* Symboltable::defineQueryDomain(char* name) {
  DomainType* unknownDomType = new DomainType();
  VarSymbol* newDomSym = 
    new VarSymbol(name, unknownDomType, VAR_NORMAL, VAR_CONST);
  Variable* newDom = new Variable(newDomSym);

  return new ForallExpr(new AList<Expr>(newDom));
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
