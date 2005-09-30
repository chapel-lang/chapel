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
  PARSING_PRELUDE,
  PARSING_USERFILES,
  PARSING_POST
};

static parsePhaseType parsePhase = PARSING_PRE;


SymScope* rootScope = NULL;
SymScope* preludeScope = NULL;
static SymScope* postludeScope = NULL;
static SymScope* currentScope = NULL;
static ModuleSymbol* currentModule = NULL;

ModuleSymbol* commonModule = NULL;

static Vec<ModuleSymbol*> allModules;     // Contains all modules
static Vec<ModuleSymbol*> codegenModules; // Contains codegened modules
static Vec<ModuleSymbol*> userModules;    // Contains user modules

static void registerModule(ModuleSymbol* mod) {
  switch (mod->modtype) {
  case MOD_USER:
    userModules.add(mod);
  case MOD_STANDARD:
  case MOD_COMMON:
    codegenModules.add(mod);
  case MOD_INTERNAL:
    allModules.add(mod);
    break;
  default:
    INT_FATAL(mod, "Unable to register module");
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
  commonModule->modScope->astParent = commonModule;

  commonModule->stmts->insertAtTail(new BlockStmt());

  registerModule(commonModule);

  prelude->modScope = preludeScope;          // SJD: Why here?
  preludeScope->astParent = prelude;
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


Vec<ModuleSymbol*>* Symboltable::getModules(moduleSet whichModules) {
  switch (whichModules) {
  case MODULES_ALL:
    return &allModules;
  case MODULES_CODEGEN:
    return &codegenModules;
  case MODULES_USER:
    return &userModules;
  default:
    INT_FATAL("Unexpected case in getModules: %d\n", whichModules);
    return NULL;
  }
}


Symbol* Symboltable::lookupInScope(char* name,
                                   SymScope* scope,
                                   Vec<SymScope*>* scopesAlreadyVisited) {
  if (!scope) {
    INT_FATAL("NULL scope passed to lookupInScope()");
  }

  if (scopesAlreadyVisited && scopesAlreadyVisited->set_in(scope)) {
    return NULL;
  }

  Symbol* sym = scope->table.get(name);
  if (!sym) {
    if (scope->uses.n) {
      if (!scopesAlreadyVisited)
        scopesAlreadyVisited = new Vec<SymScope*>();
      scopesAlreadyVisited->set_add(scope);
      forv_Vec(ModuleSymbol, module, scope->uses) {
        sym = lookupInScope(name, module->modScope, scopesAlreadyVisited);
        if (sym)
          break;
      }
    }
  }
  return sym;
}


Symbol* Symboltable::lookupFromScope(char* name, SymScope* scope) {
  if (!scope) {
    INT_FATAL("NULL scope passed to lookupFromScope()");
  }
  while (scope != NULL) {
    Symbol* sym = lookupInScope(name, scope);
    if (sym) {
      return sym;
    }
    if (scope->type == SCOPE_ARG) {
      FnSymbol* fn = dynamic_cast<FnSymbol*>(scope->astParent);
      if (!fn) {
        INT_FATAL("Cannot find function from SCOPE_ARG");
      }
      if (fn->typeBinding) {
        ClassType* structuralType =
          dynamic_cast<ClassType*>(fn->typeBinding->definition);
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
  return NULL;
}


Symbol* Symboltable::lookup(char* name) {
  return lookupFromScope(name, currentScope);
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
        preludeScope->define(mod);
      }
    }
  }

  DefExpr* defExpr = new DefExpr(mod);

  if (!empty) {
    registerModule(mod);
    mod->stmts->insertAtTail(def);

    // pop the module's scope
    if (mod->modtype != MOD_INTERNAL) {
      SymScope* modScope = Symboltable::popScope();
      modScope->astParent = mod;
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


DefExpr*
Symboltable::defineParam(intentTag tag, char* ident, Expr* type, Expr* init) {
  ArgSymbol* argSymbol = new ArgSymbol(tag, ident, dtUnknown);
  if (tag == INTENT_TYPE) {
    char *name = stringcat("__type_variable_", argSymbol->name);
    VariableType* new_type = new VariableType(getMetaType(NULL));
    TypeSymbol* new_type_symbol = new TypeSymbol(name, new_type);
    new_type->addSymbol(new_type_symbol);
    argSymbol->type = getMetaType(NULL);
    argSymbol->genericSymbol = new_type_symbol;
  } 
#if 0
  else if (tag == INTENT_PARAM) {
    char *name = stringcat("__parameter_", argSymbol->name);
    VarSymbol *varSymbol = new VarSymbol(name, dtUnknown, VAR_NORMAL, VAR_PARAM);
    argSymbol->genericSymbol = varSymbol;
  }
#endif
  return new DefExpr(argSymbol, init, type);
}


void
setVarSymbolAttributes(AList<Stmt>* stmts, varType vartag, consType constag) {
  for_alist(Stmt, stmt, stmts) {
    if (ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(stmt)) {
      if (DefExpr* defExpr = dynamic_cast<DefExpr*>(exprStmt->expr)) {
        if (VarSymbol* var = dynamic_cast<VarSymbol*>(defExpr->sym)) {
          var->consClass = constag;
          var->varClass = vartag;
          continue;
        }
      }
    }
    INT_FATAL(stmt, "Major error in setVarSymbolAttributes");
  }
}


static AList<DefExpr>* exprToIndexSymbols(AList<Expr>* indices) {
  AList<DefExpr>* defExprs = new AList<DefExpr>();
  for_alist(Expr, tmp, indices) {
    SymExpr* varTmp = dynamic_cast<SymExpr*>(tmp);
    if (!varTmp) {
      INT_FATAL(tmp, "Error, Variable expected in index list");
    } else {
      // Hack: set to integer
      defExprs->insertAtTail(
        new DefExpr(
          new VarSymbol(varTmp->var->name, dtInteger)));
    }
  }
  return defExprs;
}


ForallExpr* Symboltable::defineForallExpr(AList<Expr>* indices, 
                                          AList<Expr>* iterators,
                                          Expr* innerExpr) {
  return new ForallExpr(exprToIndexSymbols(indices), iterators, innerExpr);
}


ForLoopStmt* 
Symboltable::defineForLoop(ForLoopStmtTag forLoopStmtTag,
                           AList<Expr>* indices, 
                           AList<Expr>* iterators,
                           BlockStmt* innerStmt) {
  return new ForLoopStmt(forLoopStmtTag,
                         exprToIndexSymbols(indices),
                         iterators,
                         innerStmt);
}


PrimitiveType* Symboltable::definePrimitiveType(char* name, char* cname, Symbol *initSymbol) {
  PrimitiveType *t = 
    dynamic_cast<PrimitiveType*>(defineBuiltinType(name, cname, new PrimitiveType(initSymbol)));
  if (initSymbol)
    initSymbol->type = t; // little chicken and the egg problem
  return t;
}

Type* Symboltable::defineBuiltinType(char* name, char* cname, Type *newType) {
  TypeSymbol* sym = new TypeSymbol(name, newType);
  rootScope->define(sym);
  sym->cname = stringcpy(cname);
  newType->addSymbol(sym);

  builtinTypes.add(newType);

  return newType;
}


FnSymbol* Symboltable::startFnDef(FnSymbol* fnsym) {
  return fnsym;
}


void Symboltable::continueFnDef(FnSymbol* fnsym, AList<DefExpr>* formals, 
                                Type* retType, bool isRef, Expr *whereExpr) {
  fnsym->formals = formals;
  fnsym->retType = retType;
  fnsym->retRef = isRef;
  fnsym->whereExpr = whereExpr;
}


FnSymbol* Symboltable::finishFnDef(FnSymbol* fnsym, BlockStmt* blockBody) {
  fnsym->body = blockBody;
  if (strcmp(fnsym->name, "main") == 0 && fnsym->formals->isEmpty()) {
    if (!FnSymbol::mainFn) {
      FnSymbol::mainFn = fnsym;
      fnsym->parentScope = Symboltable::getCurrentScope(); // Yuck, SJD
    } else {
      USR_FATAL(fnsym, "main multiply defined -- first occurrence at %s",
                FnSymbol::mainFn->stringLoc());
    }
  }
  return fnsym;
}


DefExpr* Symboltable::defineFunction(char* name, AList<DefExpr>* formals, 
                                     Type* retType, BlockStmt* body) {
  if (formals == NULL) {
    formals = new AList<DefExpr>();
  }
  FnSymbol* fnsym = startFnDef(new FnSymbol(name));
  continueFnDef(fnsym, formals, retType);
  return new DefExpr(finishFnDef(fnsym, body));
}


DefExpr* Symboltable::defineStructType(char* name, // NULL = anonymous
                                       Type* type,
                                       AList<Stmt>* def) {
  ClassType* structType = dynamic_cast<ClassType*>(type);

  if (!structType) {
    INT_FATAL(type, "defineStructType called on non ClassType");
  }

  TypeSymbol* sym = new TypeSymbol(name, structType);
  structType->addSymbol(sym);
  DefExpr* defExpr = new DefExpr(sym);
  structType->addDeclarations(def);
  return defExpr;
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
