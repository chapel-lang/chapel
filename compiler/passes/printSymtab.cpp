#include "printSymtab.h"
#include "symtab.h"

void PrintSymtab::run(Stmt* program) {
  Symboltable::dump(stdout);
}
