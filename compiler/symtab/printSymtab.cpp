#include "printSymtab.h"


PrintSymtab::PrintSymtab(FILE* init_outfile) :
  outfile(init_outfile)
{}
  
void PrintSymtab::preProcessScope(SymScope* scope) {
  if (!scope->isEmpty()) {
    scope->printHeader(outfile);
  }
}

void PrintSymtab::postProcessScope(SymScope* scope) {
  if (!scope->isEmpty()) {
    scope->printFooter(outfile);
  }
}

void PrintSymtab::processSymbol(Symbol* sym) {
  fprintf(outfile, "%s%s\n", sym->parentScope->indentStr(), sym->name);
}
