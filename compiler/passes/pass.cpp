#include "link.h"
#include "pass.h"


void DummyPass::run(ModuleSymbol* moduleList) {
  fprintf(stdout, "Running dummy pass\n");
}
