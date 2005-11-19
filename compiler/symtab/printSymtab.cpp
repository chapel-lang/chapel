#include "printSymtab.h"
#include "symtab.h"


PrintSymtab::PrintSymtab(FILE* init_outfile) :
  outfile(init_outfile)
{}


void PrintSymtab::run(Vec<ModuleSymbol*>* modules) {
  SymtabTraversal::run(modules);
}

 
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
  char* indent = sym->parentScope->indentStr();
  fprintf(outfile, "%s%s\n", indent, sym->name);
}


void printSymtab(void) {
  Pass* pass = new PrintSymtab();
  pass->run(Symboltable::getModules(pass->whichModules));
}
