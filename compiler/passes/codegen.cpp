#include <stdio.h>
#include "codegen.h"
#include "driver.h"
#include "files.h"
#include "nils.h"
#include "symbol.h"


void Codegen::run(ModuleSymbol* moduleList) {
  if (suppressCodegen) {
    return;
  }

  openMakefile(moduleList->filename, system_dir);

  ModuleSymbol* currentModule = moduleList;
  while (currentModule) {
    currentModule->codegenDef();

    currentModule = nextLink(ModuleSymbol, currentModule);
  }
    
  verifyNilsUncorrupted();

  closeMakefile();

  makeAndCopyBinary();
}
