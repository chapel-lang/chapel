#include "astutil.h"
#include "driver.h"
#include "files.h"
#include "parser.h"
#include "stringutil.h"
#include "symbol.h"
#include "countTokens.h"
#include "yy.h"
#include "runtime.h"

static ModuleSymbol* parseStandardModule(char* name) {
  static char* modulePath = NULL;
  if (modulePath == NULL) {
    modulePath = stringcat(chplhome, "/modules/standard/");
  }
  return ParseFile(stringcat(modulePath, name), MOD_STANDARD);
}

static void parseStandardModules(void) {
  baseModule = parseStandardModule("_chpl_base.chpl");
  if (!fnostdincs) {
    fileModule = parseStandardModule("_IO.chpl");
    tupleModule = parseStandardModule("_Tuple.chpl");
    domainModule = parseStandardModule("_Arrays.chpl");
    parseStandardModule("_SingleLocaleArithmetic.chpl");
    parseStandardModule("_SingleLocaleAssociative.chpl");
    parseStandardModule("_SingleLocaleSparse.chpl");
    seqModule = parseStandardModule("_chpl_seq.chpl");
    parseStandardModule("_chpl_ds.chpl");
    parseStandardModule("_chpl_machine.chpl");
    standardModule = parseStandardModule("_chpl_standard.chpl");
    parseStandardModule("Time.chpl");
    parseStandardModule("Random.chpl");
    parseStandardModule("Standard.chpl");
    parseStandardModule("Types.chpl");
    parseStandardModule("Math.chpl");
    parseStandardModule("BitOps.chpl");
    parseStandardModule("Memory.chpl");
    parseStandardModule("Schedules.chpl");
  }
}

void parse(void) {
  yydebug = debugParserLevel;

  parseStandardModules();

  int filenum = 0;
  char* inputFilename = NULL;

  while ((inputFilename = nthFilename(filenum++))) {
    ParseFile(inputFilename, MOD_USER);
  }
  finishCountingTokens();

  if (!fnostdincs) {
    chpl_seq = seqModule->lookupType("seq");
    chpl_array = domainModule->lookupType("_array");
    chpl_stdin = fileModule->lookupVar("stdin");
    chpl_stdout = fileModule->lookupVar("stdout");
    chpl_stderr = fileModule->lookupVar("stderr");
    dtWriter = fileModule->lookupType("Writer")->type;
  }
}
