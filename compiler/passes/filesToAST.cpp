#include "driver.h"
#include "files.h"
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
    ParseFile(stringcat(path, "_chpl_math.chpl"), MOD_STANDARD);
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

  forv_Vec(ModuleSymbol, mod, allModules)
    insert_help(mod, NULL, NULL, mod, mod->modScope);

  findInternalTypes();

  if (!fnostdincs && !fnostdincs_but_file) {
    chpl_htuple = tupleModule->lookupType("_htuple");
    chpl_seq = seqModule->lookupType("seq");
  }
  if (!fnostdincs) {
    chpl_stdin = fileModule->lookupVar("stdin");
    chpl_stdout = fileModule->lookupVar("stdout");
    chpl_stderr = fileModule->lookupVar("stderr");
  }
  chpl_input_filename = prelude->lookupVar("chpl_input_filename");
  chpl_input_lineno = prelude->lookupVar("chpl_input_lineno");
  setterToken = baseModule->lookupVar("_setterToken");
  methodToken = baseModule->lookupVar("_methodToken");
}
