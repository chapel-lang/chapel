#include "printProgram.h"
#include "stmt.h"
#include "module.h"

void PrintProgram::run(Module* moduleList) {
  Module* mod = moduleList;
  while (mod) {
    mod->stmts->printList(stdout, "\n");
    printf("\n");
    
    mod = nextLink(Module, mod);
  }
}
