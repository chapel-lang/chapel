#include "link.h"
#include "module.h"
#include "pass.h"


void DummyPass::run(Module* moduleList) {
  fprintf(stdout, "Running dummy pass\n");
}
