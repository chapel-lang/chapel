#include "copyPropagation.h"
#include "vec.h"
#include "expr.h"
#include "stmt.h"
#include "bb.h"
#include "symtab.h"
#include "../traversals/view.h"


CopyPropagation::CopyPropagation() {
  whichModules = MODULES_CODEGEN;
}


void CopyPropagation::processSymbol(Symbol* sym) {
  if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym)) {
    buildBasicBlocks(fn);
//     print_view(fn);
//     printBasicBlocks(fn);
//     printf("\n\n");
  }
}


void copyPropagation(void) {
  Pass* pass = new CopyPropagation();
  pass->run(Symboltable::getModules(pass->whichModules));
}
