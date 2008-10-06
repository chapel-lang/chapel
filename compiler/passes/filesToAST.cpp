#include "astutil.h"
#include "driver.h"
#include "files.h"
#include "parser.h"
#include "passes.h"
#include "stringutil.h"
#include "symbol.h"
#include "countTokens.h"
#include "yy.h"

static ModuleSymbol* parseStandardModule(const char* name) {
  static const char* modulePath = NULL;
  if (modulePath == NULL) {
    modulePath = astr(CHPL_HOME, "/modules/standard/");
  }
  return ParseFile(astr(modulePath, name), MOD_STANDARD);
}

static void parseStandardModules(void) {
  baseModule = parseStandardModule("ChapelBase.chpl");

  standardModule = parseStandardModule("ChapelStandard.chpl");
  parseStandardModule("ChapelLocale.chpl");
  parseStandardModule("ChapelIO.chpl");
  parseStandardModule("ChapelTuple.chpl");
  parseStandardModule("ChapelReduce.chpl");
  parseStandardModule("ChapelRange.chpl");
  parseStandardModule("ChapelArray.chpl");
  parseStandardModule("ChapelDistribution.chpl");
  parseStandardModule("ChapelUtil.chpl");
  parseStandardModule("DefaultArithmetic.chpl");
  parseStandardModule("DefaultAssociative.chpl");
  parseStandardModule("DefaultSparse.chpl");
  parseStandardModule("DefaultEnum.chpl");
  parseStandardModule("DefaultOpaque.chpl");

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
  parseStandardModule("Containers.chpl");

  // standard distributions
  parseStandardModule("DistCSR.chpl");
}

void parse(void) {
  yydebug = debugParserLevel;

  parseStandardModules();
  
  forv_Vec(TypeSymbol, ts, gTypes) {
    if (!strcmp(ts->name, "iterator")) {
      if (EnumType* enumType = toEnumType(ts->type)) {
        for_alist(expr, enumType->constants) {
          if (DefExpr* def = toDefExpr(expr)) {
            if (!strcmp(def->sym->name, "leader"))
              gLeaderTag = def->sym;
            else if (!strcmp(def->sym->name, "follower"))
              gFollowerTag = def->sym;
          }
        }
      }
    } else if (!strcmp(ts->name, "_array")) {
      dtArray = toClassType(ts->type);
    } else if (!strcmp(ts->name, "BaseArray")) {
      dtBaseArray = toClassType(ts->type);
    } else if (!strcmp(ts->name, "BaseDomain")) {
      dtBaseDomain = toClassType(ts->type);
    } else if (!strcmp(ts->name, "Distribution")) {
      dtDistribution = toClassType(ts->type);
    } else if (!strcmp(ts->name, "Writer")) {
      dtWriter = toClassType(ts->type);
    } else if (!strcmp(ts->name, "file")) {
      dtChapelFile = toClassType(ts->type);
    }
  }

  int filenum = 0;
  const char* inputFilename = NULL;
  while ((inputFilename = nthFilename(filenum++))) {
    if (isChplSource(inputFilename)) {
      ParseFile(inputFilename, MOD_USER);
    }
  }
  forv_Vec(ModuleSymbol, mod, allModules) {
    if (mod != standardModule && mod != theProgram && mod != rootModule) {
      mod->block->addUse(standardModule);
    }
  }
  baseModule->block->addUse(rootModule);
  finishCountingTokens();
}
