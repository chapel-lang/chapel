#include "findCollidingNames.h"
#include "symtab.h"
#include "stringutil.h"

/* Goal:
 * Find symbols that have names which already exist and munge those.
 * Strategy:
 * Keep track of all function symbols, type symbols and variable symbols in vector structures.
 * Search those structures for colliding names and change the cname to a unique name.
 * Initially all the cnames are the same as name; then we change them as needed.
 * 
 */

void mangle(Vec<Symbol*> syms) {  
  UniqueName* un = new UniqueName("_u", "_");
  char* newname;
  
  while (syms.length() - 2 >= 0) {
    Symbol* s = syms.pop();
    Symbol* s1 = syms.pop();
    while(s->equalWith(s, s1)) {
      un->inc();
      newname = un->newName(intstring(s1->id));
      s1->cname = copystring(newname);
      if (syms.length()) { 
        s1 = syms.pop();
      }
      else break;
    }
    if (syms.length() > 0) { syms.add(s1);}
  }  
}

Vec<Symbol*> gather_varsyms(Symbol* varsym) {
  Vec<Symbol*> varsyms;
  SymScope* scope = varsym->parentScope;
  while(scope && scope->type != SCOPE_MODULE) {
    varsym->cname = copystring(varsym->name);
    varsyms.add_exclusive(varsym);
    scope = scope->parent;
  }
  return varsyms;
}

void FindCollidingNames::processSymbol(Symbol* sym) {
  if (_adhoc_to_uniform_mangling) {
    if (dynamic_cast<FnSymbol*>(sym)) {
      //RED: a brute force "demangling" attempt -- does not quite work
      //I wonder why (it seems that cnames are not really uniform, but have some
      //hidden additions to them: e.g. integer -> _integer, etc.)
      //sym->cname = copystring(sym->name);
      fnsyms.add(sym);
    }
    if (dynamic_cast<TypeSymbol*>(sym)) {
      sym->cname = copystring(sym->name);
      typesyms.add(sym); 
    }
    if (dynamic_cast<VarSymbol*>(sym)) {
      if (sym->parentScope->type == SCOPE_MODULE) {
        sym->cname = copystring(sym->name);
        globalvars.add(sym);
      }
      Vec<Symbol*> varsyms = gather_varsyms(sym);
      varsyms.quickSort(0, varsyms.length() - 1);
      mangle(varsyms);
    }
  }
}

void FindCollidingNames::run(ModuleList* moduleList) {
  if (_adhoc_to_uniform_mangling) {
    SymtabTraversal::run(moduleList);  
    fnsyms.quickSort(0, fnsyms.length()-1);
    typesyms.quickSort(0, typesyms.length()-1);
    globalvars.quickSort(0, globalvars.length()-1);
    mangle(fnsyms);
    mangle(typesyms);
    mangle(globalvars);
  }
}


