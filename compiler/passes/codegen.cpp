#include <stdio.h>
#include "codegen.h"
#include "driver.h"
#include "files.h"
#include "nils.h"
#include "../traversals/createConfigVarTable.h"
#include "symbol.h"
#include "symtab.h"

static void writeCommon(void) {
  fileinfo outfileinfo;
  fileinfo extheadfileinfo;
  fileinfo intheadfileinfo;

  openCFiles("_chpl_common", &outfileinfo, &extheadfileinfo, &intheadfileinfo);

  //  fileinfo* common_file = openTmpFile("_chpl_common.h");

  //  open_common(common_file);
  fprintf(codefile, "#include \"stdchpl.h\"\n");
  fprintf(codefile, "#include \"_chpl_common.h\"\n");
  fprintf(codefile, "#include \"_chpl_common-internal.h\"\n");
  commonModule->modScope->codegen(codefile, "\n");
  commonModule->stmts->codegenList(codefile, "\n");
  closeCFiles(&outfileinfo, &extheadfileinfo, &intheadfileinfo);
}


void Codegen::run(ModuleSymbol* moduleList) {
  if (suppressCodegen) {
    return;
  }

  openMakefile(moduleList->filename, system_dir);

  writeCommon();

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
