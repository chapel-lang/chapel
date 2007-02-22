#include "stmt.h"

void printProgram(void) {
  forv_Vec(ModuleSymbol, mod, userModules) {
    mod->block->print(stdout);
    printf("\n");
  }
}
