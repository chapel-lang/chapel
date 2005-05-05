#include "printSymtab.h"


PrintSymtab::PrintSymtab(FILE* init_outfile) :
  outfile(init_outfile)
{}


void PrintSymtab::run(ModuleSymbol* moduleList) {
  if (strcmp(args, "user") == 0) {
    whichModules = MODULES_USER;
  } else if (strcmp(args, "common") == 0) {
    whichModules = MODULES_COMMON_AND_USER;
  }
  SymtabTraversal::run(moduleList);
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
