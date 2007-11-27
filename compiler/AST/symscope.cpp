#include "expr.h"
#include "stmt.h"
#include "runtime.h"
#include "stringutil.h"
#include "symscope.h"
#include "files.h"


SymScope::SymScope(BaseAST* iastParent, SymScope* iparent) :
  astParent(iastParent),
  parent(iparent)
{ }


SymScope::~SymScope() {
  Vec<const char*> keys;
  visibleFunctions.get_keys(keys);
  forv_Vec(const char, key, keys) {
    delete visibleFunctions.get(key);
  }
}


void SymScope::define(Symbol* sym) {
  if (FnSymbol* fn = toFnSymbol(sym)) {
    if (fn->global)
      theProgram->block->blkScope->addVisibleFunction(fn);
    else
      addVisibleFunction(fn);
  }
  Symbol* tmp = table.get(sym->name);
  if (tmp) {
    sym->overloadNext = tmp->overloadNext;
    sym->overloadPrev = tmp;
    if (tmp->overloadNext)
      tmp->overloadNext->overloadPrev = sym;
    tmp->overloadNext = sym;
    sym->setParentScope(tmp->parentScope);
  } else {
    table.put(sym->name, sym);
    sym->overloadNext = NULL;
    sym->overloadPrev = NULL;
    sym->setParentScope(this);
  }
}


void SymScope::undefine(Symbol* sym) {
  if (FnSymbol* fn = toFnSymbol(sym)) {
    theProgram->block->blkScope->removeVisibleFunction(fn);
    removeVisibleFunction(fn);
  }
  Symbol* tmp = table.get(sym->name);
  if (tmp == sym) {
    tmp = sym->overloadNext;
    table.del(sym->name);
    if (tmp)
      table.put(sym->name, tmp);
  } else {
    if (!sym->overloadPrev)
      INT_FATAL(sym, "Symbol not found in scope from which deleted");
    if (sym->overloadPrev)
      sym->overloadPrev->overloadNext = sym->overloadNext;
    if (sym->overloadNext)
      sym->overloadNext->overloadPrev = sym->overloadPrev;
  }
  sym->overloadNext = NULL;
  sym->overloadPrev = NULL;
}


Symbol*
SymScope::lookupLocal(const char* name, Vec<SymScope*>* alreadyVisited, bool returnModules) {
  Symbol* sym;

  if (!alreadyVisited) {
    Vec<SymScope*> scopes;
    return lookupLocal(name, &scopes, returnModules);
  }

  if (alreadyVisited->set_in(this))
    return NULL;

  alreadyVisited->set_add(this);

  sym = table.get(name);

  if (sym && (!toModuleSymbol(sym) || returnModules))
    return sym;

  if (astParent && astParent->getModule()->block == astParent) {
    ModuleSymbol* mod = astParent->getModule();
    sym = mod->initFn->body->blkScope->lookupLocal(name, alreadyVisited, returnModules);
    if (sym && (!toModuleSymbol(sym) || returnModules))
      return sym;
  }

  Vec<ModuleSymbol*>* modUses = getModuleUses();
  if (modUses) {
    forv_Vec(ModuleSymbol, module, *modUses) {
      sym = module->block->blkScope->lookup(name, alreadyVisited, returnModules);
      if (sym && (!toModuleSymbol(sym) || returnModules))
        return sym;
    }
  }

  return NULL;
}


static void buildBreadthFirstUseTree(Vec<ModuleSymbol*>* current, Vec<Vec<ModuleSymbol*>*> &queue, Vec<ModuleSymbol*>* alreadySeen = NULL) {
  Vec<ModuleSymbol*>* next = new Vec<ModuleSymbol*>;
  if (!alreadySeen) {
    Vec<ModuleSymbol*> seen;
    return buildBreadthFirstUseTree(current, queue, &seen);
  }
  queue.add(current);
  forv_Vec(ModuleSymbol, module, *current) {
    Vec<ModuleSymbol*>* modules = module->block->blkScope->getModuleUses();
    if (modules) {
      forv_Vec(ModuleSymbol, mod, *modules) {
        if (!alreadySeen->set_in(mod)) {
          next->add(mod);
          alreadySeen->set_add(mod);
        }
      }
    }
  }
  if (next->n > 0) {
    buildBreadthFirstUseTree(next, queue, alreadySeen);
  }
}


Symbol*
SymScope::lookup(const char* name, Vec<SymScope*>* alreadyVisited, bool returnModules, bool scanModuleUses) {
  Vec<SymScope*> nestedscopes;
  if (!alreadyVisited) {
    return lookup(name, &nestedscopes, returnModules, scanModuleUses);
  }
  if (!scanModuleUses) {
    nestedscopes.copy(*alreadyVisited);
    alreadyVisited = &nestedscopes;
  }

  if (alreadyVisited->set_in(this))
    return NULL;
  alreadyVisited->set_add(this);

  Vec<Symbol*> symbols;
  Symbol* sym = NULL;
  sym = table.get(name);
  if (sym && (returnModules || !toModuleSymbol(sym))) {
    symbols.set_add(sym);
  }
  if (symbols.n == 0 && scanModuleUses) {
    Vec<ModuleSymbol*>* modules = getModuleUses();  
    if (modules && modules->n > 0) {
      Vec<Vec<ModuleSymbol*>*> moduleQueue;
      buildBreadthFirstUseTree(modules, moduleQueue);
      forv_Vec(Vec<ModuleSymbol*>, layer, moduleQueue) {
        forv_Vec(ModuleSymbol, mod, *layer) {
          sym = mod->initFn->body->blkScope->lookup(name, alreadyVisited, returnModules, false);
          if (sym && (returnModules || !toModuleSymbol(sym))) {
            symbols.set_add(sym);
          }
        }
        if (symbols.n > 0)
          break;
      }
      bool first = true;
      forv_Vec(Vec<ModuleSymbol*>, layer, moduleQueue) {
        if (first) {
          first = false;
          continue;
        }
        delete layer;
      }
    }
  }
  if (scanModuleUses && symbols.n == 0) {
    if (parent) {
      if (astParent && astParent->getModule()->block == astParent) {
        ModuleSymbol* mod = astParent->getModule();
        sym = mod->initFn->body->blkScope->lookup(name, alreadyVisited, returnModules, scanModuleUses);
        if (sym && (returnModules || !toModuleSymbol(sym)))
          symbols.set_add(sym);

        if (symbols.n == 0 && parent) {
          sym = parent->lookup(name, alreadyVisited, returnModules, scanModuleUses);
          if (sym && (returnModules || !toModuleSymbol(sym)))
            symbols.set_add(sym);
        }

      } else {
        FnSymbol* fn = toFnSymbol(astParent);
        if (fn && fn->_this && toClassType(fn->_this->type)) {
          ClassType* ct = toClassType(fn->_this->type);
          sym = ct->structScope->lookup(name, alreadyVisited, returnModules, scanModuleUses);
          if (sym && (returnModules || !toModuleSymbol(sym)))
            symbols.set_add(sym);
          else {
            sym = parent->lookup(name, alreadyVisited, returnModules, scanModuleUses);
            if (sym && (!toModuleSymbol(sym) || returnModules))
              symbols.set_add(sym);
          }
        } else {
          sym = parent->lookup(name, alreadyVisited, returnModules, scanModuleUses);
          if (sym && (!toModuleSymbol(sym) || returnModules))
            symbols.set_add(sym);
        }
      }
    }
  }
  if (symbols.n > 1) {
    if (!toFnSymbol(symbols.v[0]))
      USR_FATAL(symbols.v[0], "Symbol %s multiply defined", name);
  }
  if (symbols.n == 0) {
    return NULL;
  } else {
    return symbols.v[0];
  }
}


void SymScope::addModuleUse(ModuleSymbol* mod) {
  Vec<ModuleSymbol*>* modUses = getModuleUses();
  if (!modUses)
    INT_FATAL(astParent, "Bad call to addModuleUse");
  modUses->add(mod);
}


Vec<ModuleSymbol*>* SymScope::getModuleUses() {
  if (BlockStmt* block = toBlockStmt(astParent))
    return &block->modUses;
  return NULL;
}


void SymScope::print() {
  print(false, 0);
}


void SymScope::print(bool number, int indent) {
  Vec<Symbol*> symbols;
  table.get_values(symbols);
  if (!symbols.n && (!astParent || !getModuleUses()->n))
    return;
  for (int i = 0; i < indent; i++)
    printf(" ");
  printf("=================================================================\n");
  for (int i = 0; i < indent; i++)
    printf(" ");
  if (astParent) {
    if (number)
      printf("%d", astParent->id);
    printf(" %s", astTagName[astParent->astTag]);
  }
  if (Symbol* sym = toSymbol(astParent))
    printf(" %s", sym->name);
  printf("\n");
  for (int i = 0; i < indent; i++)
    printf(" ");
  printf("-----------------------------------------------------------------\n");
  if (astParent) {
    forv_Vec(ModuleSymbol, mod, *getModuleUses()) {
      if (mod) {
        for (int i = 0; i < indent; i++)
          printf(" ");
        printf("use %s", mod->name);
        if (number)
          printf("[%d]", mod->id);
        printf("\n");
      }
    }
  }
  forv_Vec(Symbol, sym, symbols) {
    if (sym) {
      for (int i = 0; i < indent; i++)
        printf(" ");
      printf("%s (", sym->name);
      for (Symbol* tmp = sym; tmp; tmp = tmp->overloadNext) {
        printf("%s", tmp->cname);
        if (number)
          printf("[%d]", tmp->id);
        if (tmp->overloadNext)
          printf(", ");
      }
      printf(")\n");
    }
  }
  for (int i = 0; i < indent; i++)
    printf(" ");
  printf("=================================================================\n");
}


void SymScope::codegen(FILE* outfile) {
  Vec<Symbol*> symbols;
  table.get_values(symbols);
  forv_Vec(Symbol, sym, symbols) {
    for (Symbol* tmp = sym; tmp; tmp = tmp->overloadNext)
      if (!toTypeSymbol(tmp))
        tmp->codegenDef(outfile);
  }
}


static int compareLineno(const void* v1, const void* v2) {
  FnSymbol* fn1 = *(FnSymbol**)v1;
  FnSymbol* fn2 = *(FnSymbol**)v2;
  if (fn1->lineno > fn2->lineno)
    return 1;
  else if (fn1->lineno < fn2->lineno)
    return -1;
  else
    return 0;
}


void SymScope::codegenFunctions(FILE* outfile) {
  Vec<FnSymbol*> fns;
  Vec<Symbol*> symbols;
  table.get_values(symbols);
  forv_Vec(Symbol, sym, symbols) {
    for (Symbol* tmp = sym; tmp; tmp = tmp->overloadNext) {
      if (FnSymbol* fn = toFnSymbol(tmp)) {
        if (!fn->isExtern)
          fns.add(fn);
      }
    }
  }
  qsort(fns.v, fns.n, sizeof(fns.v[0]), compareLineno);
  forv_Vec(FnSymbol, fn, fns) {
    fn->codegenDef(outfile);
  }
}


void SymScope::addVisibleFunction(FnSymbol* fn) {
  if (!fn->visible)
    return;
  Vec<FnSymbol*>* fs = visibleFunctions.get(fn->name);
  if (!fs) fs = new Vec<FnSymbol*>;
  fs->add(fn);
  visibleFunctions.put(fn->name, fs);
}


void SymScope::removeVisibleFunction(FnSymbol* fn) {
  if (!fn->visible)
    return;
  Vec<FnSymbol*>* fs = visibleFunctions.get(fn->name);
  if (!fs) return;
  for (int i = 0; i < fs->n; i++) {
    if (fs->v[i] == fn) {
      fs->v[i] = NULL;
    }
  }
}


void SymScope::getVisibleFunctions(Vec<FnSymbol*>* allVisibleFunctions,
                                   const char* name,
                                   bool recursed) {

  // to avoid infinite loop because of cyclic module uses
  static Vec<SymScope*> visited;
  if (!recursed)
    visited.clear();
  if (visited.set_in(this))
    return;
  visited.set_add(this);

  Vec<FnSymbol*>* fs = visibleFunctions.get(name);
  if (fs)
    allVisibleFunctions->append(*fs);
  Vec<ModuleSymbol*>* modUses = getModuleUses();
  if (modUses) {
    forv_Vec(ModuleSymbol, module, *modUses) {
      module->block->blkScope->getVisibleFunctions(allVisibleFunctions, name, true);
    }
  }
  if (astParent) {
    if (FnSymbol* fn = toFnSymbol(astParent)) {
      if (fn->visiblePoint && fn->visiblePoint->parentScope)
        fn->visiblePoint->parentScope->getVisibleFunctions(allVisibleFunctions, name, true);
    }
    if (astParent->getModule()->block == astParent) {
      ModuleSymbol* mod = astParent->getModule();
      mod->initFn->body->blkScope->getVisibleFunctions(allVisibleFunctions, name, true);
    }
  }
  if (parent)
    parent->getVisibleFunctions(allVisibleFunctions, name, true);
}
