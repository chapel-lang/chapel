#include "printSymtab.h"
#include "symtab.h"

void PrintSymtab::run(Module* moduleList) {
  Symboltable::dump(stdout);
}
