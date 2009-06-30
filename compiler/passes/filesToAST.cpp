#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include "astutil.h"
#include "driver.h"
#include "files.h"
#include "misc.h"
#include "parser.h"
#include "passes.h"
#include "stringutil.h"
#include "symbol.h"
#include "countTokens.h"
#include "yy.h"

static ModuleSymbol* parseStandardModule(const char* name, 
                                         const char* modulePath = NULL) {
  static const char* defaultModulePath = NULL;
  if (defaultModulePath == NULL) {
    defaultModulePath = astr(CHPL_HOME, "/modules/standard/");
  }
  if (modulePath == NULL) {
    modulePath = defaultModulePath;
  }
  return ParseFile(astr(modulePath, name), MOD_STANDARD);
}

static void parseStandardModules(void) {
  baseModule = parseStandardModule("ChapelBase.chpl");

  int32_t numRealms = getNumRealms();
  standardModule = parseStandardModule("ChapelStandard.chpl"); 

  parseStandardModule(astr("ChapelNumLocales-", ((numRealms == 1) ?
                                                 "singlerealm" :
                                                 "multirealm"), ".chpl"));
  parseStandardModule("ChapelThreads.chpl");
  parseStandardModule("ChapelLocale.chpl");
  parseStandardModule(astr("ChapelRealmSpace-", ((numRealms == 1) ?
                                                 "singlerealm" :
                                                 "multirealm"), ".chpl"));
  parseStandardModule("ChapelRealm.chpl");
  parseStandardModule("ChapelIO.chpl");
  parseStandardModule("ChapelTuple.chpl");
  parseStandardModule("ChapelReduce.chpl");
  parseStandardModule("ChapelRange.chpl");
  parseStandardModule("ChapelArray.chpl");
  parseStandardModule("ChapelDistribution.chpl");
  parseStandardModule("ChapelUtil.chpl");

  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
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
    }
  }

  parseStandardModule("DefaultArithmetic.chpl");
  parseStandardModule("DefaultAssociative.chpl");
  parseStandardModule("DefaultSparse.chpl");
  parseStandardModule("DefaultEnum.chpl");
  parseStandardModule("DefaultOpaque.chpl");
  parseStandardModule("BlockDist.chpl");
  parseStandardModule("MultiBlockDist.chpl");

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
  parseStandardModule("Norm.chpl");

  // standard distributions
  parseStandardModule("DistCSR.chpl");
  parseStandardModule("GPUDist.chpl");
}

void parse(void) {
  yydebug = debugParserLevel;

  parseStandardModules();
  
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (!strcmp(ts->name, "_array")) {
      dtArray = toClassType(ts->type);
    } else if (!strcmp(ts->name, "BaseArr")) {
      dtBaseArr = toClassType(ts->type);
    } else if (!strcmp(ts->name, "BaseDom")) {
      dtBaseDom = toClassType(ts->type);
    } else if (!strcmp(ts->name, "BaseDist")) {
      dtDist = toClassType(ts->type);
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
