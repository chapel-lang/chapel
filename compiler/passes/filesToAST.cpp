#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

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

  ModuleSymbol* modsym = NULL;

  ParseMod(name, MOD_INTERNAL);

  // vass sez: It would be better to push this down into ParseMod().
  // If ParseMod() sees that there is a single module within a file (whether wrapped or not),
  // it can return that one module.  Currently, if the content of the file is not a module body,
  // ParseMod() returns NULL.
  forv_Vec(ModuleSymbol, mod, gModuleSymbols)
    if (!strcmp(mod->name, name))
      modsym = mod;

  if (modsym == NULL) {
    INT_FATAL("Couldn't find module %s\n", name);
  }

  return modsym;
}


static void setIteratorTags(void) {
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (!strcmp(ts->name, iterKindTypename)) {
      if (EnumType* enumType = toEnumType(ts->type)) {
        for_alist(expr, enumType->constants) {
          if (DefExpr* def = toDefExpr(expr)) {
            if (!strcmp(def->sym->name, iterKindLeaderTagname))
              gLeaderTag = def->sym;
            else if (!strcmp(def->sym->name, iterKindFollowerTagname))
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
    }
    else if (!strcmp(ts->name, "Writer")) {
      if (dtWriter) {
        USR_WARN("'Writer' defined more than once in Chapel Internal modules.");
      }
      dtWriter = toClassType(ts->type);
    } else if (!strcmp(ts->name, "Reader")) {
      if (dtReader) {
        USR_WARN("'Reader' defined more than once in Chapel Internal modules.");
      }
      dtReader = toClassType(ts->type);
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
  if (!dtReader) {
    USR_FATAL_CONT("'Reader' not defined in Chapel Internal modules.");
  }
  USR_STOP();

  int filenum = 0;
  const char* inputFilename;

  while ((inputFilename = nthFilename(filenum++))) {
    if (isChplSource(inputFilename)) {
      addModulePathFromFilename(inputFilename);
    }
  }

  addDashMsToUserPath();

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
    // Filter out modules that don't want to include ChapelStandard by default.
    if (mod->hasFlag(FLAG_NO_DEFAULT_USE))
      continue;

    // ChapelStandard is added implicity to the "use" list of all other modules.
    {
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
