#include "astutil.h"
#include "driver.h"
#include "files.h"
#include "parser.h"
#include "passes.h"
#include "stringutil.h"
#include "symbol.h"
#include "countTokens.h"
#include "yy.h"
#include "runtime.h"

static ModuleSymbol* parseStandardModule(const char* name) {
  static const char* modulePath = NULL;
  if (modulePath == NULL) {
    modulePath = astr(chplhome, "/modules/standard/");
  }
  return ParseFile(astr(modulePath, name), MOD_STANDARD);
}

static void parseStandardModules(void) {
  baseModule = parseStandardModule("ChapelBase.chpl");
  if (!fNoStdIncs) {
    standardModule = parseStandardModule("ChapelStandard.chpl");
    parseStandardModule("ChapelLocale.chpl");
    fileModule = parseStandardModule("ChapelIO.chpl");
    tupleModule = parseStandardModule("ChapelTuple.chpl");
    parseStandardModule("ChapelReduce.chpl");
    parseStandardModule("ChapelRange.chpl");
    domainModule = parseStandardModule("ChapelArray.chpl");
    parseStandardModule("ChapelUtil.chpl");
    parseStandardModule("SingleLocaleArithmetic.chpl");
    parseStandardModule("SingleLocaleAssociative.chpl");
    parseStandardModule("SingleLocaleSparse.chpl");
    parseStandardModule("SingleLocaleEnum.chpl");
    parseStandardModule("SingleLocaleOpaque.chpl");

    parseStandardModule("Types.chpl");
    parseStandardModule("Math.chpl");

    parseStandardModule("List.chpl");
    parseStandardModule("Time.chpl");
    parseStandardModule("Random.chpl");
    parseStandardModule("BitOps.chpl");
    parseStandardModule("Memory.chpl");
    parseStandardModule("Schedules.chpl");
    parseStandardModule("Sort.chpl");
    parseStandardModule("Search.chpl");

    // standard distributions
    parseStandardModule("DistCSR.chpl");
  }
}

void parse(void) {
  yydebug = debugParserLevel;

  parseStandardModules();

  int filenum = 0;
  const char* inputFilename = NULL;

  while ((inputFilename = nthFilename(filenum++))) {
    if (isChplSource(inputFilename)) {
      ParseFile(inputFilename, MOD_USER);
    }
  }
  finishCountingTokens();

  if (!fNoStdIncs) {
    chpl_array = domainModule->lookupType("_array");
    chpl_stdin = fileModule->lookupVar("stdin");
    chpl_stdout = fileModule->lookupVar("stdout");
    chpl_stderr = fileModule->lookupVar("stderr");
    dtWriter = fileModule->lookupType("Writer")->type;
  }
}
