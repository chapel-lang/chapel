#include <stdio.h>
#include "codegen.h"
#include "driver.h"
#include "files.h"
#include "nils.h"
#include "../traversals/createConfigVarTable.h"
#include "symbol.h"
#include "symtab.h"

static void writeCommon(void) {
  fileinfo* common_file = openTmpFile("_chpl_common.h");

  fprintf(common_file->fptr, "#ifndef __CHPL_COMMON_H_\n");
  fprintf(common_file->fptr, "#define __CHPL_COMMON_H_\n");
  commonModule->stmts->codegenList(common_file->fptr, "\n");
  fprintf(common_file->fptr, "#endif\n");
  fclose(common_file->fptr);
}


void Codegen::run(ModuleSymbol* moduleList) {
  if (suppressCodegen) {
    return;
  }

  writeCommon();

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
