#include <stdio.h>
#include "codegen.h"
#include "files.h"
#include "module.h"
#include "nils.h"

extern int suppressCodegen;
extern char system_dir[];


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
