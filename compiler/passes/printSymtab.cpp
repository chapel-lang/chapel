#include "printSymtab.h"
#include "symtab.h"

void PrintSymtab::run(ModuleSymbol* moduleList) {
  Symboltable::dump(stdout);
}
