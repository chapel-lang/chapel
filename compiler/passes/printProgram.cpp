#include "printProgram.h"
#include "stmt.h"

void PrintProgram::run(ModuleSymbol* moduleList) {
  ModuleSymbol* mod = moduleList;
  while (mod) {
    mod->stmts->printList(stdout, "\n");
    printf("\n");
    
    mod = nextLink(ModuleSymbol, mod);
  }
}
