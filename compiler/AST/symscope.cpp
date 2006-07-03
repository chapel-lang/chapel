#include "expr.h"
#include "stmt.h"
#include "analysis.h"
#include "stringutil.h"
#include "symscope.h"
#include "files.h"

#define OPERATOR_CHAR(_c) \
  (((_c > ' ' && _c < '0') || (_c > '9' && _c < 'A') || \
    (_c > 'Z' && _c < 'a') || (_c > 'z')) &&            \
   _c != '_'&& _c != '?' && _c != '$')                  \


static bool
isGloballyVisible(FnSymbol* fn) {
  for_alist(DefExpr, def, fn->formals) {
    if (ClassType* ct = dynamic_cast<ClassType*>(def->sym->type)) {
      if (ct->isNominalType()) {
        return true;
      }
    }
  }
  if (fn->fnClass == FN_CONSTRUCTOR) {
    if (ClassType* ct = dynamic_cast<ClassType*>(fn->retType)) {
      if (ct->isNominalType()) {
        return true;
      }
    }
  }
  return false;
}


SymScope::SymScope(BaseAST* iastParent, SymScope* iparent) :
  astParent(iastParent),
  parent(iparent)
{ }


void SymScope::define(Symbol* sym) {
  Symbol* tmp = table.get(sym->name);
  if (tmp) {
    if (tmp == sym) {
      return;
      //INT_FATAL(sym, "Attempt to define symbol %s twice", sym->name);
    }
    while (tmp->overload) {
      tmp = tmp->overload;
      if (tmp == sym) {
        INT_FATAL(sym, "Attempt to define symbol %s twice", sym->name);
      }
    }
    tmp->overload = sym;
    sym->setParentScope(tmp->parentScope);
  } else {
    table.put(sym->name, sym);
    sym->setParentScope(this);
    symbols.add(sym);
  }
  if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym)) {
    if (isGloballyVisible(fn)) {
      rootScope->addVisibleFunction(fn);
    } else {
      addVisibleFunction(fn);
    }
  }
}


void SymScope::undefine(Symbol* sym) {
  if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym)) {
    rootScope->removeVisibleFunction(fn);
    removeVisibleFunction(fn);
  }
  for (int i = 0; i < symbols.n; i++) {
    if (symbols.v[i]) {
      if (symbols.v[i] == sym) {
        symbols.v[i] = sym->overload;
        table.del(sym->name);
        if (symbols.v[i])
          table.put(sym->name, symbols.v[i]);
        sym->overload = NULL;
        return;
      } else {
        Symbol* tmp = symbols.v[i];
        while (tmp->overload) {
          if (tmp->overload == sym) {
            tmp->overload = sym->overload;
            sym->overload = NULL;
            return;
          }
          tmp = tmp->overload;
        }
      }
    }
  }
  INT_FATAL(sym, "Symbol not found in scope from which deleted");
}


Symbol*
SymScope::lookupLocal(char* name, Vec<SymScope*>* alreadyVisited) {
  Symbol* sym;

  if (!alreadyVisited) {
    Vec<SymScope*> scopes;
    return lookupLocal(name, &scopes);
  }

  if (alreadyVisited->set_in(this))
    return NULL;

  alreadyVisited->set_add(this);

  sym = table.get(name);
  if (sym)
    return sym;

  if (astParent) {
    forv_Vec(ModuleSymbol, module, astParent->uses) {
      sym = module->modScope->lookupLocal(name, alreadyVisited);
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
      }
    }
  }
  if (parent)
    return parent->lookup(name);
  else
    return NULL;
}


void SymScope::print() {
  print(false, 0);
}


void SymScope::print(bool number, int indent) {
  if (!symbols.n && (!astParent || !astParent->uses.n))
    return;
  for (int i = 0; i < indent; i++)
    printf(" ");
  printf("=================================================================\n");
  for (int i = 0; i < indent; i++)
    printf(" ");
  if (astParent) {
    if (number)
      printf("%ld", astParent->id);
    printf(" %s", astTypeName[astParent->astType]);
  }
  if (Symbol* sym = dynamic_cast<Symbol*>(astParent))
    printf(" %s", sym->name);
  printf("\n");
  for (int i = 0; i < indent; i++)
    printf(" ");
  printf("-----------------------------------------------------------------\n");
  if (astParent) {
    forv_Vec(ModuleSymbol, mod, astParent->uses) {
      if (mod) {
        for (int i = 0; i < indent; i++)
          printf(" ");
        printf("use %s", mod->name);
        if (number)
          printf("[%ld]", mod->id);
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
          printf("[%ld]", tmp->id);
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
  if (dynamic_cast<ModuleSymbol*>(astParent)) // because initFn has modScope
    return;
  forv_Vec(Symbol, sym, symbols) {
    for (Symbol* tmp = sym; tmp; tmp = tmp->overload)
      if (!dynamic_cast<TypeSymbol*>(tmp))
        tmp->codegenDef(outfile);
  }
}


void SymScope::codegenFunctions(FILE* outfile) {
  forv_Vec(Symbol, sym, symbols) {
    for (Symbol* tmp = sym; tmp; tmp = tmp->overload) {
      if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(tmp)) {
        ts->definition->codegenStringToType(codefile);
        ts->definition->codegenConfigVarRoutines(codefile);
      }
    }
  }
  forv_Vec(Symbol, sym, symbols) {
    for (Symbol* tmp = sym; tmp; tmp = tmp->overload) {
      if (dynamic_cast<FnSymbol*>(tmp)) {
        tmp->codegenDef(outfile);
      }
    }
  }
}


void SymScope::addVisibleFunction(FnSymbol* fn) {
  if (!fn->visible)
    return;
  int is_setter = (fn->name[0] == '=' && !OPERATOR_CHAR(fn->name[1]) &&
                   fn->name[1] != '\0');
  char* n = cannonicalize_string(fn->name + (is_setter ? 1 : 0));
  Vec<FnSymbol*>* fs = visibleFunctions.get(n);
  if (!fs) fs = new Vec<FnSymbol*>;
  fs->add(fn);
  visibleFunctions.put(n, fs);
}


void SymScope::removeVisibleFunction(FnSymbol* fn) {
  if (!fn->visible)
    return;
  int is_setter = (fn->name[0] == '=' && !OPERATOR_CHAR(fn->name[1]) &&
                   fn->name[1] != '\0');
  char* n = cannonicalize_string(fn->name + (is_setter ? 1 : 0));
  Vec<FnSymbol*>* fs = visibleFunctions.get(n);
  if (!fs) return;
  for (int i = 0; i < fs->n; i++) {
    if (fs->v[i] == fn) {
      fs->v[i] = NULL;
    }
  }
}


void SymScope::getVisibleFunctions(Vec<FnSymbol*>* allVisibleFunctions,
                                   char* name,
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
  if (astParent) {
    forv_Vec(ModuleSymbol, module, astParent->uses) {
      module->modScope->getVisibleFunctions(allVisibleFunctions, name, true);
    }
  }
  if (parent)
    parent->getVisibleFunctions(allVisibleFunctions, name, true);
}


void SymScope::printVisibleFunctions() {
  for (int i = 0; i < visibleFunctions.n; i++) {
    Vec<FnSymbol *> *fs = visibleFunctions.v[i].value;
    if (fs) {
      forv_Vec(FnSymbol, fn, *fs) {
        if (fn) {
          fn->print(stdout);
          printf(" %ld\n", fn->id);
        }
      }
    }
  }
}
