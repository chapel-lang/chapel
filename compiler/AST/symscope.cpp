#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symscope.h"
#include "files.h"


SymScope::SymScope(BaseAST* iastParent, SymScope* iparent) :
  astParent(iastParent),
  parent(iparent)
{ }


SymScope::~SymScope() {
  Vec<char*> keys;
  visibleFunctions.get_keys(keys);
  forv_Vec(char, key, keys) {
    delete visibleFunctions.get(key);
  }
}


void SymScope::define(Symbol* sym) {
  if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym)) {
    if (fn->global)
      rootScope->addVisibleFunction(fn);
    else
      addVisibleFunction(fn);
  }
  Symbol* tmp = table.get(sym->name);
  if (tmp) {
    if (tmp == sym)
      INT_FATAL(sym, "Attempt to define symbol %s twice", sym->name);
    while (tmp->overload) {
      tmp = tmp->overload;
      if (tmp == sym)
        INT_FATAL(sym, "Attempt to define symbol %s twice", sym->name);
    }
    tmp->overload = sym;
    sym->setParentScope(tmp->parentScope);
  } else {
    table.put(sym->name, sym);
    sym->setParentScope(this);
  }
}


void SymScope::undefine(Symbol* sym) {
  if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym)) {
    rootScope->removeVisibleFunction(fn);
    removeVisibleFunction(fn);
  }
  Symbol* tmp = table.get(sym->name);
  if (tmp == sym) {
    tmp = sym->overload;
    table.del(sym->name);
    if (tmp)
      table.put(sym->name, tmp);
    sym->overload = NULL;
    return;
  } else {
    while (tmp->overload) {
      if (tmp->overload == sym) {
        tmp->overload = sym->overload;
        sym->overload = NULL;
        return;
      }
      tmp = tmp->overload;
    }
  }
  INT_FATAL(sym, "Symbol not found in scope from which deleted");
}


Symbol*
SymScope::lookupLocal(char* name, Vec<SymScope*>* alreadyVisited, bool nestedUse) {
  Symbol* sym;

  if (!alreadyVisited) {
    Vec<SymScope*> scopes;
    return lookupLocal(name, &scopes, nestedUse);
  }

  if (alreadyVisited->set_in(this))
    return NULL;

  alreadyVisited->set_add(this);

  sym = table.get(name);

  if (sym)
    return sym;

  if (astParent && astParent->getModule()->block == astParent) {
    ModuleSymbol* mod = astParent->getModule();
    sym = mod->initFn->body->blkScope->lookupLocal(name, alreadyVisited, nestedUse);
    if (sym)
      return sym;
  }

  Vec<ModuleSymbol*>* modUses = getModuleUses();
  if (modUses && !nestedUse) {
    forv_Vec(ModuleSymbol, module, *modUses) {
      bool mynestedUse = nestedUse || (module->modtype == MOD_USER);
      sym = module->block->blkScope->lookupLocal(name, alreadyVisited, mynestedUse);
      if (sym)
        return sym;
    }
  }

  return NULL;
}


Symbol*
SymScope::lookup(char* name) {
  Symbol* sym = lookupLocal(name);
  if (sym)
    return sym;
  if (FnSymbol* fn = dynamic_cast<FnSymbol*>(astParent)) {
    if (fn->_this) {
      ClassType* ct = dynamic_cast<ClassType*>(fn->_this->type);
      if (ct) {
        Symbol* sym = ct->structScope->lookupLocal(name);
        if (sym)
          return sym;
        Type* outerType = ct->symbol->defPoint->parentSymbol->type;
        if (ClassType* ot = dynamic_cast<ClassType*>(outerType)) {
          // Nested class.  Look at the scope of the outer class
          Symbol* sym = ot->structScope->lookup(name);
          if (sym)
            return sym;
        }
      }
    }
  }
  if (parent)
    return parent->lookup(name);
  return NULL;
}


void SymScope::addModuleUse(ModuleSymbol* mod) {
  Vec<ModuleSymbol*>* modUses = getModuleUses();
  if (!modUses)
    INT_FATAL(astParent, "Bad call to addModuleUse");
  modUses->add(mod);
}


Vec<ModuleSymbol*>* SymScope::getModuleUses() {
  if (BlockStmt* block = dynamic_cast<BlockStmt*>(astParent))
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
    printf(" %s", astTypeName[astParent->astType]);
  }
  if (Symbol* sym = dynamic_cast<Symbol*>(astParent))
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
      for (Symbol* tmp = sym; tmp; tmp = tmp->overload) {
        printf("%s", tmp->cname);
        if (number)
          printf("[%d]", tmp->id);
        if (tmp->overload)
          printf(", ");
      }
      printf(")\n");
    }
  }
  for (int i = 0; i < indent; i++)
    printf(" ");
  printf("=================================================================\n");
}


void SymScope::codegen(FILE* outfile, char* separator) {
  Vec<Symbol*> symbols;
  table.get_values(symbols);
  forv_Vec(Symbol, sym, symbols) {
    for (Symbol* tmp = sym; tmp; tmp = tmp->overload)
      if (!dynamic_cast<TypeSymbol*>(tmp))
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
    for (Symbol* tmp = sym; tmp; tmp = tmp->overload) {
      if (FnSymbol* fn = dynamic_cast<FnSymbol*>(tmp)) {
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
                                   char* name,
                                   bool recursed,
                                   bool nestedUse) {

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
  if (modUses && !nestedUse) {
    forv_Vec(ModuleSymbol, module, *modUses) {
      module->block->blkScope->getVisibleFunctions(allVisibleFunctions, name, true, module->modtype == MOD_USER);
    }
  }
  if (astParent) {
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(astParent)) {
      if (fn->visiblePoint && fn->visiblePoint->parentScope)
        fn->visiblePoint->parentScope->getVisibleFunctions(allVisibleFunctions, name, true, nestedUse);
    }
    if (astParent->getModule()->block == astParent) {
      ModuleSymbol* mod = astParent->getModule();
      mod->initFn->body->blkScope->getVisibleFunctions(allVisibleFunctions, name, true, nestedUse);
    }
  }
  if (parent)
    parent->getVisibleFunctions(allVisibleFunctions, name, true, nestedUse);
}
