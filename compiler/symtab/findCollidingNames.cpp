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
  

void FindCollidingNames::processSymbol(Symbol* sym) {
  if (_adhoc_to_uniform_mangling) {
    if (FnSymbol* fnsym = dynamic_cast<FnSymbol*>(sym)) {
      fnsyms.add(fnsym);
    }
    if (TypeSymbol* typesym = dynamic_cast<TypeSymbol*>(sym)) {
      typesyms.add(typesym); 
    }
    if (VarSymbol* varsym = dynamic_cast<VarSymbol*>(sym)) {
      if (varsym->parentScope->type == SCOPE_MODULE) {
        globalvars.add(varsym);
      }
    }
  }
}

/*void FindCollidingNames::run(ModuleList* moduleList) {
  SymtabTraversal::run(moduleList);
  //RED: for test purposes only
  fnsyms.quickSort(0, fnsyms.length()-1);
}*/


