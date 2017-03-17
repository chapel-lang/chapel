/*
 * Copyright 2004-2017 Cray Inc.
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
#include "build.h"
#include "config.h"
#include "countTokens.h"
#include "docsDriver.h"
#include "expr.h"
#include "files.h"
#include "parser.h"

bool parsed = false;

static void countTokensInCmdLineFiles();
static void setIteratorTags();
static void gatherWellKnownTypes();
static void gatherWellKnownFns();

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
  {"chpl_main_argument", &dtMainArgument, false},
  {"chpl_comm_on_bundle_t", &dtOnBundleRecord,   false},
  {"chpl_task_bundle_t",    &dtTaskBundleRecord, false},
  {"Error",                 &dtError,            true}
};


struct WellKnownFn
{
  const char* name;
  FnSymbol**  fn;
  Flag        flag;
  FnSymbol*   lastNameMatchedFn;
};

// These functions are a required part of the compiler/module interface.
// They should generally be marked export so that they are always
// resolved.
static WellKnownFn sWellKnownFns[] = {
  {"chpl_here_alloc",         &gChplHereAlloc, FLAG_LOCALE_MODEL_ALLOC},
  {"chpl_here_free",          &gChplHereFree,  FLAG_LOCALE_MODEL_FREE},
  {"chpl_doDirectExecuteOn",  &gChplDoDirectExecuteOn, FLAG_UNKNOWN},
  {"_build_tuple",            &gBuildTupleType, FLAG_BUILD_TUPLE_TYPE},
  {"_build_tuple_noref",      &gBuildTupleTypeNoRef, FLAG_BUILD_TUPLE_TYPE},
  {"*",                       &gBuildStarTupleType, FLAG_BUILD_TUPLE_TYPE},
  {"_build_star_tuple_noref", &gBuildStarTupleTypeNoRef, FLAG_BUILD_TUPLE_TYPE},
  {"chpl_delete_error",       &gChplDeleteError, FLAG_UNKNOWN}
};

void parse() {
  yydebug = debugParserLevel;

  if (countTokens)
    countTokensInCmdLineFiles();

  //
  // If we're running chpldoc on just a single file, we don't want to
  // bring in all the base, standard, etc. modules -- just the file
  // we're documenting.
  //
  if (fDocs == false || fDocsProcessUsedModules) {
    baseModule            = parseMod("ChapelBase",           MOD_INTERNAL);
    INT_ASSERT(baseModule);

    setIteratorTags();

    standardModule        = parseMod("ChapelStandard",       MOD_INTERNAL);
    INT_ASSERT(standardModule);

    printModuleInitModule = parseMod("PrintModuleInitOrder", MOD_INTERNAL);
    INT_ASSERT(printModuleInitModule);

    parseDependentModules(MOD_INTERNAL);

    gatherWellKnownTypes();
    gatherWellKnownFns();
  }

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

  //
  // When generating chpldocs for just a single file, we don't want to
  // parse dependent modules, as we're just documenting the file at
  // hand.
  //
  if (fDocs == false || fDocsProcessUsedModules) {
    parseDependentModules(MOD_USER);

    forv_Vec(ModuleSymbol, mod, allModules) {
      mod->addDefaultUses();
    }
  }

  checkConfigs();
  convertForallExpressions();

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
  } else {
    if (dtString->symbol == NULL) {
      // This means there was no declaration of the string type.
      gAggregateTypes.remove(gAggregateTypes.index(dtString));
      delete dtString;
      dtString = NULL;
    }
  }
}

static void gatherWellKnownFns() {
  int nEntries = sizeof(sWellKnownFns) / sizeof(sWellKnownFns[0]);
  static const char* mult_def_message   = "'%s' defined more than once in Chapel internal modules.";
  static const char* flag_reqd_message = "The '%s' function is missing a required flag.";
  static const char* wkfn_reqd_message   = "Function '%s' must be defined in the Chapel internal modules.";

  // Harvest well-known functions from among the global fn definitions.
  // We check before assigning to the associated global to ensure that it
  // is null.  In that way we can flag duplicate definitions.
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    for (int i = 0; i < nEntries; ++i) {
      WellKnownFn& wkfn = sWellKnownFns[i];

      if (strcmp(fn->name, wkfn.name) == 0) {
        wkfn.lastNameMatchedFn = fn;
        if (wkfn.flag == FLAG_UNKNOWN || fn->hasFlag(wkfn.flag)) {
          if (*wkfn.fn != NULL)
            USR_WARN(fn, mult_def_message, wkfn.name);

          *wkfn.fn = fn;
        }
      }
    }
  }

  //
  // When compiling for minimal modules, we don't require any specific
  // well-known functions to be defined.
  //
  if (fMinimalModules == false) {
    // Make sure all well-known functions are defined.
    for (int i = 0; i < nEntries; ++i) {
      WellKnownFn& wkfn = sWellKnownFns[i];
      FnSymbol* lastMatched = wkfn.lastNameMatchedFn;
      FnSymbol* fn = *wkfn.fn;

      if (lastMatched == NULL)
        USR_FATAL_CONT(wkfn_reqd_message, wkfn.name);
      else if(! fn)
        USR_FATAL_CONT(fn, flag_reqd_message, wkfn.name);
    }

    USR_STOP();
  }
}
