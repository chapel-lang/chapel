#include <stdio.h>
#include "codegen.h"
#include "driver.h"
#include "files.h"
#include "module.h"
#include "nils.h"


void Codegen::run(Module* moduleList) {
  if (suppressCodegen) {
    return;
  }

  openMakefile(moduleList->filename, system_dir);

  currentModule = moduleList;
  while (currentModule) {
    currentModule->codegen();

    currentModule = nextLink(Module, currentModule);
  }
    
  verifyNilsUncorrupted();

  closeMakefile();

  makeAndCopyBinary();
}
