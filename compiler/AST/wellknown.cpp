/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#include "wellknown.h"

#include "driver.h"
#include "expr.h"
#include "symbol.h"

// The well-known types
AggregateType* dtArray;
AggregateType* dtBaseArr;
AggregateType* dtBaseDom;
AggregateType* dtCFI_cdesc_t;
AggregateType* dtDist;
AggregateType* dtError;
AggregateType* dtExternalArray;
AggregateType* dtLocaleID;
AggregateType* dtMainArgument;
AggregateType* dtOnBundleRecord;
AggregateType* dtOpaqueArray;
AggregateType* dtTaskBundleRecord;
AggregateType* dtTuple;
AggregateType* dtRef;


// The well-known functions
FnSymbol *gChplHereAlloc;
FnSymbol *gChplHereFree;
FnSymbol *gChplDecRunningTask;
FnSymbol *gChplIncRunningTask;
FnSymbol *gChplDoDirectExecuteOn;
FnSymbol *gBuildTupleType;
FnSymbol *gBuildTupleTypeNoRef;
FnSymbol *gBuildStarTupleType;
FnSymbol *gBuildStarTupleTypeNoRef;
FnSymbol *gPrintModuleInitFn;
FnSymbol *gGetDynamicEndCount;
FnSymbol *gSetDynamicEndCount;
FnSymbol *gChplDeleteError;
FnSymbol *gChplUncaughtError;
FnSymbol *gChplPropagateError;
FnSymbol *gChplSaveTaskError;
FnSymbol *gChplForallError;
FnSymbol *gAtomicFenceFn;
FnSymbol *gChplAfterForallFence;
FnSymbol *gChplCreateStringWithLiteral;

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/


void gatherIteratorTags() {
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (strcmp(ts->name, iterKindTypename) == 0) {
      if (EnumType* enumType = toEnumType(ts->type)) {
        for_alist(expr, enumType->constants) {
          if (DefExpr* def = toDefExpr(expr)) {
            const char* name = def->sym->name;

            if        (strcmp(name, iterKindLeaderTagname)     == 0) {
              gLeaderTag     = def->sym;

            } else if (strcmp(name, iterKindFollowerTagname)   == 0) {
              gFollowerTag   = def->sym;

            } else if (strcmp(name, iterKindStandaloneTagname) == 0) {
              gStandaloneTag = def->sym;
            }
          }
        }
      }
    }
  }
}

// This structure and the following array provide a list of types that must be
// defined in module code.
struct WellKnownType
{
  const char*     name;
  AggregateType** type_;
  bool            isClass;
};

// These types are a required part of the compiler/module interface.
static WellKnownType sWellKnownTypes[] = {
  { "_array",                &dtArray,            false },
  { "BaseArr",               &dtBaseArr,          true  },
  { "BaseDom",               &dtBaseDom,          true  },
  { "BaseDist",              &dtDist,             true  },
  { "CFI_cdesc_t",           &dtCFI_cdesc_t,      false },
  { "chpl_external_array",   &dtExternalArray,    false },
  { "chpl_localeID_t",       &dtLocaleID,         false },
  { "chpl_main_argument",    &dtMainArgument,     false },
  { "chpl_comm_on_bundle_t", &dtOnBundleRecord,   false },
  { "chpl_opaque_array",     &dtOpaqueArray,      false },
  { "chpl_task_bundle_t",    &dtTaskBundleRecord, false },
  { "_tuple",                &dtTuple,            false },
  { "_ref",                  &dtRef,              true  },
  { "Error",                 &dtError,            true  }
};

static void removeIfUndefinedGlobalType(AggregateType*& t) {
  if (t->symbol == NULL || t->symbol->defPoint == NULL) {
    // This means there was no declaration of this type
    if (t->symbol)
      gTypeSymbols.remove(gTypeSymbols.index(t->symbol));

    gAggregateTypes.remove(gAggregateTypes.index(t));

    delete t;

    t = NULL;
  }
}

// Gather well-known types from among types known at this point.
void gatherWellKnownTypes() {
  int nEntries = sizeof(sWellKnownTypes) / sizeof(sWellKnownTypes[0]);

  // Harvest well-known types from among the global type definitions.
  // We check before assigning to the well-known type dt<typename>,
  // to ensure that it is null.  In that way we can flag duplicate
  // definitions.
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    for (int i = 0; i < nEntries; ++i) {
      WellKnownType& wkt = sWellKnownTypes[i];

      if (strcmp(ts->name, wkt.name) == 0) {
        if (*wkt.type_ != NULL) {
          USR_WARN(ts,
                   "'%s' defined more than once in Chapel internal modules.",
                   wkt.name);
        }

        INT_ASSERT(ts->type);

        if (wkt.isClass == true && isClass(ts->type) == false) {
          USR_FATAL_CONT(ts->type,
                         "The '%s' type must be a class.",
                         wkt.name);
        }

        *wkt.type_ = toAggregateType(ts->type);
      }
    }
  }

  if (fMinimalModules == false) {
    // Make sure all well-known types are defined.
    for (int i = 0; i < nEntries; ++i) {
      WellKnownType& wkt = sWellKnownTypes[i];

      if (*wkt.type_ == NULL) {
        if (wkt.type_ == &dtCFI_cdesc_t && !fLibraryFortran) {
          // This should only be defined when --library-fortran is used
        } else {
          USR_FATAL_CONT("Type '%s' must be defined in the "
                         "Chapel internal modules.",
                         wkt.name);
        }
      }
    }

    USR_STOP();

  } else {
    removeIfUndefinedGlobalType(dtString);
    removeIfUndefinedGlobalType(dtBytes);
    removeIfUndefinedGlobalType(dtLocale);
    removeIfUndefinedGlobalType(dtOwned);
    removeIfUndefinedGlobalType(dtShared);
  }
}

struct WellKnownFn
{
  const char* name;
  FnSymbol**  fn;
  Flag        flag;
  FnSymbol*   lastNameMatchedFn;
};

// These functions are a required part of the compiler/module interface.
// These functions will always be resolved if they are concrete.
// (In the past, they needed to be marked 'export' but that is no longer
//  required).
static WellKnownFn sWellKnownFns[] = {
  {
    "chpl_here_alloc",
    &gChplHereAlloc,
    FLAG_LOCALE_MODEL_ALLOC
  },

  {
    "chpl_here_free",
    &gChplHereFree,
    FLAG_LOCALE_MODEL_FREE
  },

  {
    "chpl_taskRunningCntInc",
    &gChplIncRunningTask,
    FLAG_INC_RUNNING_TASK
  },

  {
    "chpl_taskRunningCntDec",
    &gChplDecRunningTask,
    FLAG_DEC_RUNNING_TASK
  },

  {
    "chpl_doDirectExecuteOn",
    &gChplDoDirectExecuteOn,
    FLAG_UNKNOWN
  },

  {
    "_build_tuple",
    &gBuildTupleType,
    FLAG_BUILD_TUPLE_TYPE
  },

  {
    "_build_tuple_noref",
    &gBuildTupleTypeNoRef,
    FLAG_BUILD_TUPLE_TYPE
  },

  {
    "*",
    &gBuildStarTupleType,
    FLAG_BUILD_TUPLE_TYPE
  },

  {
    "_build_star_tuple_noref",
    &gBuildStarTupleTypeNoRef,
    FLAG_BUILD_TUPLE_TYPE
  },

  {
    "printModuleInit",
    &gPrintModuleInitFn,
    FLAG_PRINT_MODULE_INIT_FN
  },

  {
    "chpl_task_getDynamicEndCount",
    &gGetDynamicEndCount,
    FLAG_UNKNOWN
  },

  {
    "chpl_task_setDynamicEndCount",
    &gSetDynamicEndCount,
    FLAG_UNKNOWN
  },

  {
    "chpl_delete_error",
    &gChplDeleteError,
    FLAG_UNKNOWN
  },

  {
    "chpl_uncaught_error",
    &gChplUncaughtError,
    FLAG_UNKNOWN
  },

  {
    "chpl_propagate_error",
    &gChplPropagateError,
    FLAG_UNKNOWN
  },

  {
    "chpl_save_task_error",
    &gChplSaveTaskError,
    FLAG_UNKNOWN
  },

  {
    "chpl_forall_error",
    &gChplForallError,
    FLAG_UNKNOWN
  },

  {
    "atomic_fence",
    &gAtomicFenceFn,
    FLAG_UNKNOWN
  },

  {
    "chpl_after_forall_fence",
    &gChplAfterForallFence,
    FLAG_UNKNOWN
  },

  {
    "chpl_createStringWithLiteral",
    &gChplCreateStringWithLiteral,
    FLAG_UNKNOWN
  },

};

void gatherWellKnownFns() {
  int nEntries = sizeof(sWellKnownFns) / sizeof(sWellKnownFns[0]);

  // Harvest well-known functions from among the global fn definitions.
  // We check before assigning to the associated global to ensure that it
  // is null.  In that way we can flag duplicate definitions.
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    for (int i = 0; i < nEntries; ++i) {
      WellKnownFn& wkfn = sWellKnownFns[i];

      if (strcmp(fn->name, wkfn.name) == 0) {
        wkfn.lastNameMatchedFn = fn;

        if (wkfn.flag == FLAG_UNKNOWN || fn->hasFlag(wkfn.flag) == true) {
          if (*wkfn.fn != NULL) {
            USR_WARN(fn,
                     "'%s' defined more than once in Chapel internal modules.",
                     wkfn.name);
          }

          *wkfn.fn = fn;
        }
      }
    }
  }

  if (fMinimalModules == false) {
    for (int i = 0; i < nEntries; ++i) {
      WellKnownFn& wkfn        = sWellKnownFns[i];
      FnSymbol*    lastMatched = wkfn.lastNameMatchedFn;
      FnSymbol*    fn          = *wkfn.fn;

      if (lastMatched == NULL) {
        USR_FATAL_CONT("Function '%s' must be defined in the "
                       "Chapel internal modules.",
                       wkfn.name);

      } else if (fn == NULL) {
        USR_FATAL_CONT(fn,
                       "The '%s' function is missing a required flag.",
                       wkfn.name);
      }
    }

    USR_STOP();
  }
}

std::vector<FnSymbol*> getWellKnownFunctions()
{
  std::vector<FnSymbol*> fns;

  int nEntries = sizeof(sWellKnownFns) / sizeof(sWellKnownFns[0]);

  for (int i = 0; i < nEntries; ++i) {
    WellKnownFn& wkfn = sWellKnownFns[i];
    if (*wkfn.fn != NULL)
      fns.push_back(*wkfn.fn);
  }

  return fns;
}

void clearGenericWellKnownFunctions()
{
  int nEntries = sizeof(sWellKnownFns) / sizeof(sWellKnownFns[0]);

  for (int i = 0; i < nEntries; ++i) {
    WellKnownFn& wkfn = sWellKnownFns[i];
    if (*wkfn.fn != NULL && (*wkfn.fn)->isGeneric())
      *wkfn.fn = NULL;
  }
}

