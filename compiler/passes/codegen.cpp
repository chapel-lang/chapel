#include <stdio.h>
#include "codegen.h"
#include "driver.h"
#include "files.h"
#include "nils.h"
#include "../traversals/createConfigVarTable.h"
#include "symbol.h"


void Codegen::run(ModuleSymbol* moduleList) {
  if (suppressCodegen) {
    return;
  }

  openMakefile(moduleList->filename, system_dir);

  ModuleSymbol* currentModule = moduleList;

  CreateConfigVarTable* createConfigVarTable = new CreateConfigVarTable();

  while (currentModule) {
    if (!currentModule->internal) {
      currentModule->codegenDef();
      currentModule->startTraversal(createConfigVarTable);
    }
    currentModule = nextLink(ModuleSymbol, currentModule);
  }
  createConfigVarTable->closeCFile();

  verifyNilsUncorrupted();

  closeMakefile();
}
