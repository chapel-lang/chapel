#include <stdio.h>
#include "codegen.h"
#include "driver.h"
#include "files.h"
#include "../traversals/createConfigVarTable.h"
#include "symbol.h"
#include "symtab.h"


void Codegen::run(ModuleSymbol* moduleList) {
  if (suppressCodegen) {
    return;
  }

  openMakefile(moduleList->filename, system_dir);

  ModuleSymbol* currentModule = moduleList;

  CreateConfigVarTable* createConfigVarTable = new CreateConfigVarTable();
  createConfigVarTable->run(currentModule);
  createConfigVarTable->closeCFile();

  while (currentModule) {
    if (!currentModule->internal) {
      currentModule->codegenDef();
    }
    currentModule = nextLink(ModuleSymbol, currentModule);
  }

  closeMakefile();
}
