#include "astutil.h"
#include "driver.h"
#include "files.h"
#include "parser.h"
#include "stringutil.h"
#include "symbol.h"
#include "countTokens.h"
#include "yy.h"
#include "runtime.h"

void parse(void) {
  yydebug = debugParserLevel;

  char* path = stringcat(sysdirToChplRoot(system_dir), "/modules/standard/");

  baseModule = ParseFile(stringcat(path, "_chpl_base.chpl"), MOD_STANDARD);

  if (!fnostdincs) {
    fileModule = ParseFile(stringcat(path, "_chpl_file.chpl"), MOD_STANDARD);
    ParseFile(stringcat(path, "_chpl_complex.chpl"), MOD_STANDARD);
    tupleModule = ParseFile(stringcat(path, "_chpl_htuple.chpl"), MOD_STANDARD);
    domainModule = ParseFile(stringcat(path, "_chpl_adomain.chpl"), MOD_STANDARD);
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

  if (!fnostdincs) {
    chpl_seq = seqModule->lookupType("seq");
    chpl_array = domainModule->lookupType("_array");
    chpl_stdin = fileModule->lookupVar("stdin");
    chpl_stdout = fileModule->lookupVar("stdout");
    chpl_stderr = fileModule->lookupVar("stderr");
  }
}
