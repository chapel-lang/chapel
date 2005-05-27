#include <stdio.h>
#include "codegen.h"
#include "driver.h"
#include "files.h"
#include "moduleList.h"
#include "symbol.h"
#include "symtab.h"
#include "../traversals/createConfigVarTable.h"


void Codegen::run(ModuleList* moduleList) {
  if (suppressCodegen) {
    return;
  }

  openMakefile(moduleList->filename, system_dir);

  CreateConfigVarTable* createConfigVarTable = new CreateConfigVarTable();
  createConfigVarTable->run(moduleList);
  createConfigVarTable->closeCFile();

  ModuleSymbol* currentModule = moduleList->first();
  while (currentModule) {
    if (currentModule->modtype != MOD_INTERNAL) {
      currentModule->codegenDef();
    }
    currentModule = moduleList->next();
  }

  closeMakefile();
}
