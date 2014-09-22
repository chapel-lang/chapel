/*
 * Copyright 2004-2014 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

bool parsed = false;

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
  printModuleInitModule = parseInternalModule("PrintModuleInitOrder"); 
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


// This structure and the following array provide a list of types that must be
// defined in module code.  At this point, they are all classes.
struct WellKnownType
{
  const char* name;
  AggregateType** type_;
  bool isClass;
};


// These types are a required part of the compiler/module interface.
WellKnownType wellKnownTypes[] = {
  {"_array",            &dtArray,       false},
  {"_tuple",            &dtTuple,       false},
  {"locale",            &dtLocale,      true},
  {"chpl_localeID_t",   &dtLocaleID,    false},
  {"BaseArr",           &dtBaseArr,     true},
  {"BaseDom",           &dtBaseDom,     true},
  {"BaseDist",          &dtDist,        true},
  {"Writer",            &dtWriter,      true},
  {"Reader",            &dtReader,      true},
  {"chpl_main_argument",&dtMainArgument,false}
};


// Gather well-known types from among types known at this point.
static void gatherWellKnownTypes()
{
  static const char* mult_def_message = "'%s' defined more than once in Chapel internal modules.";
  static const char* class_reqd_message = "The '%s' type must be a class.";
  static const char* wkt_reqd_message = "Type '%s' must be defined in the Chapel internal modules.";
  int nEntries = sizeof(wellKnownTypes) / sizeof(WellKnownType);

  // Harvest well-known types from among the global type definitions.
  // We check before assigning to the well-known type dt<typename>, to ensure that it
  // is null.  In that way we can flag duplicate definitions.
  forv_Vec(TypeSymbol, ts, gTypeSymbols)
  {
    for (int i = 0; i < nEntries; ++i)
    {
      WellKnownType& wkt = wellKnownTypes[i];
      if (!strcmp(ts->name, wkt.name))
      {
        if (*wkt.type_ != NULL)
          USR_WARN(ts, mult_def_message, wkt.name);
        INT_ASSERT(ts->type); // We expect the symbol to be defined.
        if (wkt.isClass && !isClass(ts->type))
          USR_FATAL_CONT(ts->type, class_reqd_message, wkt.name);
        *wkt.type_ = toAggregateType(ts->type);
      }
    }
  }

  //
  // When compiling for minimal modules, we don't require any specific
  // well-known types to be defined.
  //
  if (fMinimalModules) {
    return;
  }

  // Make sure all well-known types are defined.
  for (int i = 0; i < nEntries; ++i)
  {
    WellKnownType& wkt = wellKnownTypes[i];
    if (*wkt.type_ == NULL)
      USR_FATAL_CONT(wkt_reqd_message, wkt.name);
  }

  USR_STOP();
}


void parse(void) {
  yydebug = debugParserLevel;

  if (countTokens)
    countTokensInCmdLineFiles();

  parseInternalModules();

  parseDependentModules(MOD_INTERNAL);

  gatherWellKnownTypes();

  {
    int         filenum       = 0;
    const char* inputFilename = 0;

    while ((inputFilename = nthFilename(filenum++))) {
      if (isChplSource(inputFilename)) {
        addModulePathFromFilename(inputFilename);
      }
    }
  }

  addDashMsToUserPath();

  if (printSearchDirs) {
    printModuleSearchPath();
  }

  {
    int         filenum       = 0;
    const char* inputFilename = 0;

    while ((inputFilename = nthFilename(filenum++))) {
      if (isChplSource(inputFilename)) {
        ParseFile(inputFilename, MOD_MAIN);
      }
    }
  }

  parseDependentModules(MOD_USER);

  forv_Vec(ModuleSymbol, mod, allModules) {
    // Filter out modules that don't want to include ChapelStandard by default.
    if (mod->hasFlag(FLAG_NO_USE_CHAPELSTANDARD) == false) {
      mod->moduleUseAddChapelStandard();
    }
  }

  checkConfigs();

  // This block is necessary because SET_LINENO is scoped (it declares a local
  // variable).
  {
    SET_LINENO(baseModule);
    baseModule->block->moduleUseAdd(rootModule);
  }

  finishCountingTokens();

  parsed = true;
}
