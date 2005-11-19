#include <stdio.h>
#include "codegen.h"
#include "driver.h"
#include "files.h"
#include "mysystem.h"
#include "stringutil.h"
#include "symbol.h"
#include "symtab.h"
#include "../traversals/createConfigVarTable.h"


void codegen(void) {
  if (suppressCodegen)
    return;

  openMakefile(allModules.v[0]->filename, system_dir);

  CreateConfigVarTable* createConfigVarTable = new CreateConfigVarTable();
  createConfigVarTable->run(&allModules);
  createConfigVarTable->closeCFile();

  forv_Vec(ModuleSymbol, currentModule, allModules) {
    if (currentModule->modtype != MOD_INTERNAL) {
      mysystem(stringcat("# codegen-ing module", currentModule->name),
               "generating comment for --print-commands option");
      currentModule->codegenDef();
    }
  }

  closeMakefile();
  makeBinary();
}
