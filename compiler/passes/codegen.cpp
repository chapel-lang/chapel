#include <stdio.h>
#include "codegen.h"
#include "files.h"
#include "module.h"
#include "nils.h"

extern int suppressCodegen;
extern char system_dir[];


void Codegen::run(Stmt* program) {
  if (suppressCodegen) {
    return;
  }

  openMakefile(filename, system_dir);
  currentModule = new Module();
  currentModule->stmts = program;
  currentModule->codegen();
    
  verifyNilsUncorrupted();

  closeMakefile();

  makeAndCopyBinary();
}
