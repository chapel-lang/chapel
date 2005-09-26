#include <stdio.h>
#include "codegen.h"
#include "driver.h"
#include "files.h"
#include "mysystem.h"
#include "stringutil.h"
#include "symbol.h"
#include "symtab.h"
#include "../traversals/createConfigVarTable.h"


void Codegen::run(Vec<ModuleSymbol*>* modules) {
  if (suppressCodegen) {
    return;
  }

  openMakefile(modules->v[0]->filename, system_dir);

  CreateConfigVarTable* createConfigVarTable = new CreateConfigVarTable();
  createConfigVarTable->run(modules);
  createConfigVarTable->closeCFile();

  forv_Vec(ModuleSymbol, currentModule, *modules) {
    if (currentModule->modtype != MOD_INTERNAL) {
      mysystem(stringcat("# codegen-ing module", currentModule->name),
               "generating comment for --print-commands option");
      currentModule->codegenDef();
    }
  }

  closeMakefile();
}
