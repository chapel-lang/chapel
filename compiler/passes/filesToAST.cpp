/*
 * Copyright 2004-2015 Cray Inc.
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

#include "passes.h"

#include "bison-chapel.h"
#include "config.h"
#include "countTokens.h"
#include "expr.h"
#include "files.h"
#include "parser.h"

bool parsed = false;

static void countTokensInCmdLineFiles();
static void setIteratorTags();
static void gatherWellKnownTypes();

// This structure and the following array provide a list of types that must be
// defined in module code.  At this point, they are all classes.
struct WellKnownType
{
  const char*     name;
  AggregateType** type_;
  bool            isClass;
};


// These types are a required part of the compiler/module interface.
static WellKnownType sWellKnownTypes[] = {
  {"_array",             &dtArray,        false},
  {"_tuple",             &dtTuple,        false},
  {"locale",             &dtLocale,        true},
  {"chpl_localeID_t",    &dtLocaleID,     false},
  {"BaseArr",            &dtBaseArr,       true},
  {"BaseDom",            &dtBaseDom,       true},
  {"BaseDist",           &dtDist,          true},
  {"Writer",             &dtWriter,        true},
  {"Reader",             &dtReader,        true},
  {"chpl_main_argument", &dtMainArgument, false},
  {"string",             &dtString,       false}
};


void parse() {
  yydebug = debugParserLevel;

  if (countTokens)
    countTokensInCmdLineFiles();

  baseModule            = parseMod("ChapelBase",           MOD_INTERNAL);
  INT_ASSERT(baseModule);

  setIteratorTags();

  standardModule        = parseMod("ChapelStandard",       MOD_INTERNAL);
  INT_ASSERT(standardModule);

  printModuleInitModule = parseMod("PrintModuleInitOrder", MOD_INTERNAL);
  INT_ASSERT(printModuleInitModule);

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

  int         filenum       = 0;
  const char* inputFilename = 0;

  while ((inputFilename = nthFilename(filenum++))) {
    if (isChplSource(inputFilename)) {
      parseFile(inputFilename, MOD_USER, true);
    }
  }

  parseDependentModules(MOD_USER);

  forv_Vec(ModuleSymbol, mod, allModules) {
    mod->addDefaultUses();
  }

  checkConfigs();

  finishCountingTokens();

  parsed = true;
}

static void countTokensInCmdLineFiles() {
  int         filenum       = 0;
  const char* inputFilename = 0;

  while ((inputFilename = nthFilename(filenum++))) {
    if (isChplSource(inputFilename)) {
      parseFile(inputFilename, MOD_USER, true);
    }
  }

  finishCountingTokens();
}


static void setIteratorTags() {
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (strcmp(ts->name, iterKindTypename) == 0) {
      if (EnumType* enumType = toEnumType(ts->type)) {
        for_alist(expr, enumType->constants) {
          if (DefExpr* def = toDefExpr(expr)) {
            if (strcmp(def->sym->name, iterKindLeaderTagname) == 0)
              gLeaderTag     = def->sym;

            else if (strcmp(def->sym->name, iterKindFollowerTagname) == 0)
              gFollowerTag   = def->sym;

            else if (strcmp(def->sym->name, iterKindStandaloneTagname) == 0)
              gStandaloneTag = def->sym;
          }
        }
      }
    }
  }
}

// Gather well-known types from among types known at this point.
static void gatherWellKnownTypes() {
  static const char* mult_def_message   = "'%s' defined more than once in Chapel internal modules.";
  static const char* class_reqd_message = "The '%s' type must be a class.";
  static const char* wkt_reqd_message   = "Type '%s' must be defined in the Chapel internal modules.";
  int                nEntries           = sizeof(sWellKnownTypes) / sizeof(sWellKnownTypes[0]);

  // Harvest well-known types from among the global type definitions.
  // We check before assigning to the well-known type dt<typename>, to ensure that it
  // is null.  In that way we can flag duplicate definitions.
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    for (int i = 0; i < nEntries; ++i) {
      WellKnownType& wkt = sWellKnownTypes[i];

      if (strcmp(ts->name, wkt.name) == 0) {
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
  if (fMinimalModules == false) {
    // Make sure all well-known types are defined.
    for (int i = 0; i < nEntries; ++i) {
      WellKnownType& wkt = sWellKnownTypes[i];

      if (*wkt.type_ == NULL)
        USR_FATAL_CONT(wkt_reqd_message, wkt.name);
    }

    USR_STOP();
  }
}
