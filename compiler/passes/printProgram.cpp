#include "stmt.h"

void printProgram(void) {
  forv_Vec(ModuleSymbol, mod, userModules) {
    mod->stmts->print(stdout, "\n");
    printf("\n");
  }
}
