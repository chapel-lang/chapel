#include "printSymtab.h"


PrintSymtab::PrintSymtab(FILE* init_outfile) :
  outfile(init_outfile)
{ }
  
void PrintSymtab::preProcessScope(SymScope* scope) {
  //  if (!scope->isEmpty()) {
    scope->printHeader(outfile, scope->parentLength() * 2);
    //  }
}

void PrintSymtab::postProcessScope(SymScope* scope) {
  //  if (!scope->isEmpty()) {
    scope->printFooter(outfile, scope->parentLength() * 2);
    //  }
}

void PrintSymtab::processSymbol(Symbol* sym) {
//   for (int i = 0; i < sym->parentScope->parentLength() * 2; i++) {
//     printf(" ");
//   }
//   fprintf(outfile, "%s\n", sym->name);
}
