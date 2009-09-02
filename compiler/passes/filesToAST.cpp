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


static ModuleSymbol* parseInternalModule(const char* name) {
  return ParseMod(name, MOD_INTERNAL);
}


static void parseInternalModules(void) {
  baseModule = parseInternalModule("ChapelBase");
  addStdRealmsPath();
  standardModule = parseInternalModule("ChapelStandard"); 
}


void parse(void) {
  yydebug = debugParserLevel;

  parseInternalModules();

  parseDependentModules(MOD_INTERNAL);

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
  const char* inputFilename;

  while ((inputFilename = nthFilename(filenum++))) {
    if (isChplSource(inputFilename)) {
      addModulePathFromFilename(inputFilename);
    }
  }

  if (printSearchDirs) {
    printModuleSearchPath();
  }

  filenum = 0;
  while ((inputFilename = nthFilename(filenum++))) {
    if (isChplSource(inputFilename)) {
      ParseFile(inputFilename, MOD_MAIN);
    }
  }

  parseDependentModules(MOD_USER);

  
  forv_Vec(ModuleSymbol, mod, allModules) {
    if (mod != standardModule && mod != theProgram && mod != rootModule) {
      mod->block->addUse(standardModule);
    }
  }
  baseModule->block->addUse(rootModule);
  finishCountingTokens();
}
