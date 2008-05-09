#include "astutil.h"
#include "driver.h"
#include "files.h"
#include "parser.h"
#include "passes.h"
#include "stringutil.h"
#include "symbol.h"
#include "countTokens.h"
#include "yy.h"

static ModuleSymbol* fileModule = NULL;
static ModuleSymbol* domainModule = NULL;

static ModuleSymbol* parseStandardModule(const char* name) {
  static const char* modulePath = NULL;
  if (modulePath == NULL) {
    modulePath = astr(CHPL_HOME, "/modules/standard/");
  }
  return ParseFile(astr(modulePath, name), MOD_STANDARD);
}

static void parseStandardModules(void) {
  baseModule = parseStandardModule("ChapelBase.chpl");
  if (!fNoStdIncs) {
    parseStandardModule("ChapelStandard.chpl");
    parseStandardModule("ChapelLocale.chpl");
    fileModule = parseStandardModule("ChapelIO.chpl");
    parseStandardModule("ChapelTuple.chpl");
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
    dtArray = toClassType(domainModule->lookupType("_array")->type);
    dtWriter = toClassType(fileModule->lookupType("Writer")->type);
    dtChapelFile = toClassType(fileModule->lookupType("file")->type);
  }
}
