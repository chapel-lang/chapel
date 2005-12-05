#include "expr.h"
#include "stmt.h"
#include "analysis.h"
#include "stringutil.h"
#include "symscope.h"
#include "symtab.h"
#include "../passes/filesToAST.h"
#include "../passes/runAnalysis.h"
#include "files.h"

#define OPERATOR_CHAR(_c) \
  (((_c > ' ' && _c < '0') || (_c > '9' && _c < 'A') || \
    (_c > 'Z' && _c < 'a') || (_c > 'z')) &&            \
   _c != '_'&& _c != '?' && _c != '$')                  \


static bool
isGloballyVisible(FnSymbol* fn) {
  if (fn->typeBinding) {
    if (ClassType* ct = dynamic_cast<ClassType*>(fn->typeBinding->definition)) {
      if (ct->isNominalType()) {
        return true;
      }
    }
  }
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


SymScope::SymScope(scopeType init_type) :
  type(init_type),
  lookupCache(NULL),
  astParent(NULL),
  parent(NULL),
  child(NULL),
  sibling(NULL)
{
  symbols.clear();
  uses.clear();
  visibleFunctions.clear();
}


void SymScope::setASTParent(BaseAST* ast) {
  astParent = ast;
}


bool SymScope::isEmpty(void) {
  return symbols.n == 0;
}


void SymScope::define(Symbol* sym) {
  Symbol* tmp = table.get(sym->name);
  if (tmp) {
    if (tmp == sym) {
      INT_FATAL(sym, "Attempt to define symbol %s twice", sym->name);
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


int SymScope::parentLength(void) {
  if (!parent) {
    return 0;
  } else {
    return parent->parentLength() + 1;
  }
}


static void print_indent(SymScope* scope) {
  static char* spaces = "                                                     "
                        "                                                     ";
  int offset = strlen(spaces) - 2*scope->parentLength();
  if (offset < 0)
    offset = 0;
  printf("%s", spaces + offset);
}


void SymScope::dump_only(void) {
  if (isEmpty())
    return;

  print_indent(this);
  printf("=================================================================\n");
  print_indent(this);
  printf("SCOPE:");
  if (astParent)
    printf(" %s", astTypeName[astParent->astType]);
  if (Symbol* sym = dynamic_cast<Symbol*>(astParent))
    printf(" %s", sym->name);
  if (astParent && astParent->lineno > 0)
    printf(" (%s)", astParent->stringLoc());
  printf("\n");
  print_indent(this);
  printf("-----------------------------------------------------------------\n");
  forv_Vec(Symbol, sym, symbols) {
    if (sym) {
      print_indent(this);
      printf("%s (", sym->name);
      for (Symbol* tmp = sym; tmp; tmp = tmp->overload) {
        printf("%s", tmp->cname);
        if (tmp->overload)
          printf(", ");
      }
      printf(")\n");
    }
  }
  print_indent(this);
  printf("=================================================================\n");
}


void SymScope::dump(void) {
  dump_only();
  for(SymScope* tmp = child; tmp; tmp = tmp->sibling) {
    tmp->dump();
  }
}


void SymScope::codegen(FILE* outfile, char* separator) {
  if (type > SCOPE_MODULE) { // Because initFn has modScope
    forv_Vec(Symbol, sym, symbols) {
      for (Symbol* tmp = sym; tmp; tmp = tmp->overload) {
        if (!dynamic_cast<TypeSymbol*>(tmp)) {
          tmp->codegenDef(outfile);
        }
      }
    }
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
  if (inAnalysis)
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
  forv_Vec(ModuleSymbol, module, uses) {
    module->modScope->getVisibleFunctions(allVisibleFunctions, name, true);
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


void
getSymbols(SymScope *scope, Vec<Symbol *> &symbols) {
  if (!scope) return;
  forv_Symbol(sym, scope->symbols)
    for (Symbol *s = sym; s; s = s->overload)
      symbols.set_add(s);
  getSymbols(scope->child, symbols);
  getSymbols(scope->sibling, symbols);
}
