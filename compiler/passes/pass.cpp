#include "link.h"
#include "pass.h"
#include "stringutil.h"


Pass::Pass(void) :
  args(""),
  whichModules(MODULES_ALL)
{}


void Pass::setArgs(char* init_args) {
  args = copystring(init_args);
}


void DummyPass::run(ModuleSymbol* moduleList) {
  fprintf(stdout, "Running dummy pass; args = %s\n", args);
}
