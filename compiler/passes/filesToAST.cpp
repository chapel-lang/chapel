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
#include "config.h"


static ModuleSymbol* parseInternalModule(const char* name) {
  ModuleSymbol* modsym = ParseMod(name, MOD_INTERNAL);
  if (modsym == NULL) {
    INT_FATAL("Couldn't find module %s\n", name);
  }
  return modsym;
}


static void setIteratorTags(void) {
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
}


static void parseInternalModules(void) {
  baseModule = parseInternalModule("ChapelBase");

  setIteratorTags();

  addStdRealmsPath();
  standardModule = parseInternalModule("ChapelStandard"); 
}


static void countTokensInCmdLineFiles(void) {
  int filenum = 0;
  const char* inputFilename;
  while ((inputFilename = nthFilename(filenum++))) {
    if (isChplSource(inputFilename)) {
      ParseFile(inputFilename, MOD_MAIN);
    }
  }
  finishCountingTokens();
}


void parse(void) {
  yydebug = debugParserLevel;

  if (countTokens)
    countTokensInCmdLineFiles();

  parseInternalModules();

  parseDependentModules(MOD_INTERNAL);

  // These types are a required part of the compiler/module interface
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (!strcmp(ts->name, "_array")) {
      if (dtArray) {
        USR_WARN("'_array' defined more than once in Chapel Internal modules.");
      }
      dtArray = toClassType(ts->type);
    } else if (!strcmp(ts->name, "_tuple")) {
      if (dtTuple) {
        USR_WARN("'_tuple' defined more than once in Chapel Internal modules.");
      }
      dtTuple = toClassType(ts->type);
    } else if (!strcmp(ts->name, "locale")) {
      if (dtLocale) {
        USR_WARN("'locale' defined more than once in Chapel Internal modules.");
      }
      dtLocale = toClassType(ts->type);
    } else if (!strcmp(ts->name, "BaseArr")) {
      if (dtBaseArr) {
        USR_WARN("'BaseArr' defined more than once in Chapel Internal modules.");
      }
      dtBaseArr = toClassType(ts->type);
    } else if (!strcmp(ts->name, "BaseDom")) {
      if (dtBaseDom) {
        USR_WARN("'BaseDom' dfined more than once in Chapel Internal modules.");
      }
      dtBaseDom = toClassType(ts->type);
    } else if (!strcmp(ts->name, "BaseDist")) {
      if (dtDist) {
        USR_WARN("'BaseDist' defined more than once in Chapel Internal modules.");
      }
      dtDist = toClassType(ts->type);
    } else if (!strcmp(ts->name, "Writer")) {
      if (dtWriter) {
        USR_WARN("'Writer' defined more than once in Chapel Internal modules.");
      }
      dtWriter = toClassType(ts->type);
    } else if (!strcmp(ts->name, "file")) {
      if (dtChapelFile) {
        USR_WARN("'file' defined more than once in Chapel Internal modules.");
      }
      dtChapelFile = toClassType(ts->type);
    }
  }

  if (!dtArray) {
    USR_FATAL_CONT("'_array' not defined in Chapel Internal modules.");
  }
  if (!dtTuple) {
    USR_FATAL_CONT("'_tuple' not defined in Chapel Internal modules.");
  }
  if (!dtLocale) {
    USR_FATAL_CONT("'locale' not defined in Chapel Internal modules.");
  }
  if (!dtBaseArr) {
    USR_FATAL_CONT("'BaseArr' not defined in Chapel Internal modules.");
  }
  if (!dtBaseDom) {
    USR_FATAL_CONT("'BaseDom' not defined in Chapel Internal modules.");
  }
  if (!dtDist) {
    USR_FATAL_CONT("'BaseDist' not defined in Chapel Internal modules.");
  }
  if (!dtWriter) {
    USR_FATAL_CONT("'Writer' not defined in Chapel Internal modules.");
  }
  if (!dtChapelFile) {
    USR_FATAL_CONT("'file' not defined in Chapel Internal modules.");
  }
  USR_STOP();

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
    if (mod != standardModule && mod != theProgram && mod != rootModule &&
        (!fRuntime || mod->modTag != MOD_MAIN)) {
      mod->block->addUse(standardModule);
      mod->modUseSet.clear();
      mod->modUseList.clear();
      mod->modUseSet.set_add(standardModule);
      mod->modUseList.add(standardModule);
    }
  }

  checkConfigs();

  baseModule->block->addUse(rootModule);
  finishCountingTokens();
}
