#include "moduleList.h"
#include "printProgram.h"
#include "stmt.h"

PrintProgram::PrintProgram(void) {
  whichModules = MODULES_USER;
}

void PrintProgram::run(ModuleList* moduleList) {
  ModuleSymbol* mod = moduleList->first();
  while (mod) {
    // BLC: TODO: this seems to be a bug -- MOD_STANDARD shouldn't be
    // getting in here as I understand it
    if (mod->modtype == MOD_USER) {
      mod->stmts->print(stdout, "\n");
      printf("\n");
    }

    mod = moduleList->next();
  }
}
