#include "driver.h"
#include "files.h"
#include "filesToAST.h"
#include "parser.h"
#include "stringutil.h"
#include "symbol.h"
#include "symtab.h"
#include "countTokens.h"
#include "yy.h"
#include "../traversals/fixup.h"
#include "runtime.h"

void parse(void) {
  yydebug = debugParserLevel;

  char* path = stringcat(sysdirToChplRoot(system_dir), "/modules/standard/");

  prelude = ParseFile(stringcat(path, "prelude.chpl"), MOD_STANDARD);
  baseModule = ParseFile(stringcat(path, "_chpl_base.chpl"), MOD_STANDARD);
  closureModule = ParseFile(stringcat(path, "_chpl_closure.chpl"), MOD_STANDARD);

  if (!fnostdincs) {
    fileModule = ParseFile(stringcat(path, "_chpl_file.chpl"), MOD_STANDARD);
  }
  if (!fnostdincs && !fnostdincs_but_file) {
    ParseFile(stringcat(path, "_chpl_complex.chpl"), MOD_STANDARD);
    tupleModule = ParseFile(stringcat(path, "_chpl_htuple.chpl"), MOD_STANDARD);
    ParseFile(stringcat(path, "_chpl_adomain.chpl"), MOD_STANDARD);
    ParseFile(stringcat(path, "_chpl_indefinite.chpl"), MOD_STANDARD);
    ParseFile(stringcat(path, "_chpl_sparse.chpl"), MOD_STANDARD);
    seqModule = ParseFile(stringcat(path, "_chpl_seq.chpl"), MOD_STANDARD);
    standardModule = ParseFile(stringcat(path, "_chpl_standard.chpl"), MOD_STANDARD);
  }

  int filenum = 0;
  char* inputFilename = NULL;

  while ((inputFilename = nthFilename(filenum++))) {
    ParseFile(inputFilename, MOD_USER);
  }
  finishCountingTokens();

  if (userModules.n == 0)
    ParseFile(stringcat(path, "i.chpl"), MOD_USER);

  Pass* fixup = new Fixup();
  fixup->run(Symboltable::getModules(MODULES_ALL));

  if (!fnostdincs && !fnostdincs_but_file) {
    chpl_htuple = dynamic_cast<TypeSymbol*>(Symboltable::lookupInScope("_htuple", tupleModule->modScope));
    chpl_seq = dynamic_cast<TypeSymbol*>(Symboltable::lookupInScope("seq", seqModule->modScope));
  }

  if (!fnostdincs) {
    chpl_stdin = dynamic_cast<VarSymbol*>(Symboltable::lookupInScope("stdin", fileModule->modScope));
    chpl_stdout = dynamic_cast<VarSymbol*>(Symboltable::lookupInScope("stdout", fileModule->modScope));
    chpl_stderr = dynamic_cast<VarSymbol*>(Symboltable::lookupInScope("stderr", fileModule->modScope));
  }

  chpl_input_filename = dynamic_cast<VarSymbol*>(Symboltable::lookupInScope("chpl_input_filename", prelude->modScope));
  chpl_input_lineno = dynamic_cast<VarSymbol*>(Symboltable::lookupInScope("chpl_input_lineno", prelude->modScope));

  setterToken = dynamic_cast<VarSymbol*>(Symboltable::lookupInScope("_setterToken", baseModule->modScope));
  methodToken = dynamic_cast<VarSymbol*>(Symboltable::lookupInScope("_methodToken", baseModule->modScope));

}
