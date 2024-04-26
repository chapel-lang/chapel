/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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
#include "stringutil.h"
#include "symbol.h"
#include "typeSpecifier.h"

#include "global-ast-vecs.h"

// The well-known types
AggregateType* dtArray;
AggregateType* dtBaseArr;
AggregateType* dtBaseDom;
AggregateType* dtBytes;
AggregateType* dtCFI_cdesc_t;
AggregateType* dtDist;
AggregateType* dtDomain;
AggregateType* dtError;
AggregateType* dtExternalArray;
AggregateType* dtLocale;
AggregateType* dtLocaleID;
AggregateType* dtMainArgument;
AggregateType* dtObject;
AggregateType* dtOnBundleRecord;
AggregateType* dtOpaqueArray;
AggregateType* dtOwned;
AggregateType* dtRange;
AggregateType* dtRef;
AggregateType* dtShared;
AggregateType* dtString;
AggregateType* dtTaskBundleRecord;
AggregateType* dtTuple;

// these are only used when the dyno resolver is active
AggregateType* dtCPointer;
AggregateType* dtCPointerConst;

Type* dt_c_int;
Type* dt_c_uint;
Type* dt_c_long;
Type* dt_c_ulong;
Type* dt_c_longlong;
Type* dt_c_ulonglong;
Type* dt_c_char;
Type* dt_c_schar;
Type* dt_c_uchar;
Type* dt_c_short;
Type* dt_c_ushort;
Type* dt_c_intptr;
Type* dt_c_uintptr;
Type* dt_c_ptrdiff;
Type* dt_ssize_t;
Type* dt_size_t;

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
FnSymbol *gAllocateStringLiteralsBuf;
FnSymbol *gChplCreateStringWithLiteral;
FnSymbol *gChplCreateBytesWithLiteral;
FnSymbol *gChplBuildLocaleId;

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void initializeWellKnownTypes();
static void initializeWellKnownFunctions();

void initializeWellKnown() {
  initializeWellKnownTypes();
  initializeWellKnownFunctions();
}

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
struct WellKnownAggregateType
{
  // the class/record name in the modules,
  // which will be an astr after initializeWellKnown runs
  const char*     name;
  // pointer to the global variable storing the type
  AggregateType** type_;
  // is it a class? if not, we assume it is a record
  bool            isClass;
};

// These types are a required part of the compiler/module interface.
static WellKnownAggregateType sWellKnownAggregateTypes[] = {
  // name                    global               isClass
  { "BaseArr",               &dtBaseArr,          true  },
  { "BaseDist",              &dtDist,             true  },
  { "BaseDom",               &dtBaseDom,          true  },
  { "CFI_cdesc_t",           &dtCFI_cdesc_t,      false },
  { "Error",                 &dtError,            true  },
  { "_array",                &dtArray,            false },
  { "_domain",               &dtDomain,           false },
  { "_ref",                  &dtRef,              true  },
  { "chpl_comm_on_bundle_t", &dtOnBundleRecord,   false },
  { "chpl_external_array",   &dtExternalArray,    false },
  { "chpl_localeID_t",       &dtLocaleID,         false },
  { "chpl_main_argument",    &dtMainArgument,     false },
  { "chpl_opaque_array",     &dtOpaqueArray,      false },
  { "chpl_task_bundle_t",    &dtTaskBundleRecord, false },
};

// Similar to WellKnownAggregateType, but this one lists types
// that are needed early in compilation (e.g. in build.cpp).
// For these, the global type will be initialized with a dummy value
// in initializeWellKnown. Its its contents will be replaced by the
// actual type when the AST for that type is generated.
struct WellKnownAggregateTypeNeededEarly
{
  // the class/record name in the modules,
  // which will be an astr after initializeWellKnown runs
  const char*     name;
  // the user-facing name for the type
  const char*     userFacingName;
  // pointer to the global variable storing the type
  AggregateType** type_;
  // is it a class? if not, we assume it is a record
  bool            isClass;
};

// These types are a required part of the compiler/module interface.
static WellKnownAggregateTypeNeededEarly sWellKnownAggregateTypesNeededEarly[]=
{
  // name         userFacingName  global            isClass
  { "_bytes",     "bytes",        &dtBytes,         false },
  { "_locale",    "locale",       &dtLocale,        false },
  { "_object",    "RootClass",    &dtObject,        true  },
  { "_owned",     nullptr,        &dtOwned,         false },
  { "_range",     "range",        &dtRange,         false },
  { "_shared",    nullptr,        &dtShared,        false },
  { "_string",    "string",       &dtString,        false },
  { "_tuple",     nullptr,        &dtTuple,         false },
  { "c_ptr",      "c_ptr",        &dtCPointer,      true },
  { "c_ptrConst", "c_ptrConst",   &dtCPointerConst, true },
};

struct WellKnownType
{
  const char*     name;
  Type**          type_;
};


static WellKnownType sWellKnownTypes[] = {
  // and types reflecting the C compiler
  { "c_int",                 &dt_c_int       },
  { "c_uint",                &dt_c_uint      },
  { "c_long",                &dt_c_long      },
  { "c_ulong",               &dt_c_ulong     },
  { "c_longlong",            &dt_c_longlong  },
  { "c_ulonglong",           &dt_c_ulonglong },
  { "c_char",                &dt_c_char      },
  { "c_schar",               &dt_c_schar     },
  { "c_uchar",               &dt_c_uchar     },
  { "c_short",               &dt_c_short     },
  { "c_ushort",              &dt_c_ushort    },
  { "c_intptr",              &dt_c_intptr    },
  { "c_uintptr",             &dt_c_uintptr   },
  { "c_ptrdiff",             &dt_c_ptrdiff   },
  { "c_ssize_t",             &dt_ssize_t     },
  { "c_size_t",              &dt_size_t      },
};

static void initializeWellKnownTypes() {
  int nTypes = sizeof(sWellKnownTypes) / sizeof(sWellKnownTypes[0]);
  int nAggregate = sizeof(sWellKnownAggregateTypes) /
                   sizeof(sWellKnownAggregateTypes[0]);
  int nEarlyAggregate = sizeof(sWellKnownAggregateTypesNeededEarly) /
                        sizeof(sWellKnownAggregateTypesNeededEarly[0]);

  // update the name fields to be astrs
  for (int i = 0; i < nTypes; i++) {
    WellKnownType& wkt = sWellKnownTypes[i];
    wkt.name = astr(wkt.name);
  }

  for (int i = 0; i < nAggregate; i++) {
    WellKnownAggregateType& wkt = sWellKnownAggregateTypes[i];
    wkt.name = astr(wkt.name);
  }

  for (int i = 0; i < nEarlyAggregate; i++) {
    WellKnownAggregateTypeNeededEarly& wkt =
      sWellKnownAggregateTypesNeededEarly[i];

    wkt.name = astr(wkt.name);
    if (wkt.userFacingName != nullptr) {
      wkt.userFacingName = astr(wkt.userFacingName);
    } else {
      wkt.userFacingName = wkt.name;
    }

    INT_ASSERT(*wkt.type_ == nullptr);
    // create the type and set the type_ field in the WellKnownAggregate
    auto tag = wkt.isClass ? AGGREGATE_CLASS : AGGREGATE_RECORD;
    AggregateType* t = new AggregateType(tag);
    const char* n = wkt.userFacingName ? wkt.userFacingName : wkt.name;
    t->symbol = new TypeSymbol(n, t);
    *wkt.type_ = t;
  }
}

AggregateType* shouldWireWellKnownType(const char* name) {
  int nEarlyAggregate = sizeof(sWellKnownAggregateTypesNeededEarly) /
                        sizeof(sWellKnownAggregateTypesNeededEarly[0]);

  // Performance TODO: should this use a set/map or binary search
  // instead of linear traversal?
  for (int i = 0; i < nEarlyAggregate; i++) {
    WellKnownAggregateTypeNeededEarly& wkt =
      sWellKnownAggregateTypesNeededEarly[i];

    if (0 == strcmp(name, wkt.name)) {
      return *wkt.type_;
    }
  }

  return nullptr;
}

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

static void multipleDefinedTypeError(Symbol* sym, const char* name) {
  USR_WARN(sym,
           "'%s' defined more than once in Chapel internal modules.",
           name);
}

static void gatherType(Symbol* sym, Type* t, const char* name) {
  int nTypes = sizeof(sWellKnownTypes) / sizeof(sWellKnownTypes[0]);
  int nAggregate = sizeof(sWellKnownAggregateTypes) /
                   sizeof(sWellKnownAggregateTypes[0]);

  for (int i = 0; i < nTypes; ++i) {
    WellKnownType& wkt = sWellKnownTypes[i];

    if (name == wkt.name) {
      if (*wkt.type_ != NULL)
        multipleDefinedTypeError(sym, name);
      INT_ASSERT(t != NULL);
      *wkt.type_ = t;
    }
  }

  // is ts->name matching one in sWellKnownAggregateTypes?
  for (int i = 0; i < nAggregate; ++i) {
    WellKnownAggregateType& wkt = sWellKnownAggregateTypes[i];

    if (name == wkt.name) {
      if (isDecoratedClassType(t)) continue;

      if (*wkt.type_ != NULL)
        multipleDefinedTypeError(sym, name);
      INT_ASSERT(t != NULL);
      if (wkt.isClass == true && isClass(t) == false) {
        USR_FATAL_CONT(sym,
                       "The '%s' type must be a class.",
                       wkt.name);
      }
      *wkt.type_ = toAggregateType(t);
    }
  }
}

// Gather well-known types from among types known at this point.
void gatherWellKnownTypes() {
  int nTypes = sizeof(sWellKnownTypes) / sizeof(sWellKnownTypes[0]);
  int nAggregate = sizeof(sWellKnownAggregateTypes) /
                   sizeof(sWellKnownAggregateTypes[0]);
  int nEarlyAggregate = sizeof(sWellKnownAggregateTypesNeededEarly) /
                        sizeof(sWellKnownAggregateTypesNeededEarly[0]);

  // Check type aliases (for e.g. extern type c_int = int(32) )
  forv_Vec(VarSymbol, var, gVarSymbols) {
    if (var->defPoint != nullptr &&
        isModuleSymbol(var->defPoint->parentSymbol) &&
        var->hasFlag(FLAG_TYPE_VARIABLE)) {
      auto modTag = var->defPoint->getModule()->modTag;
      if (modTag == MOD_INTERNAL || modTag == MOD_STANDARD) {
        Type* t = NULL;
        if (var->type != dtUnknown) {
          t = var->type;
        } else {
          // handle extern type c_int = int(32)
          DefExpr* def = var->defPoint;
          if (CallExpr* call = toCallExpr(def->init)) {
            t = typeForTypeSpecifier(call, false);
          }
        }

        if (t != NULL && t != dtUnknown)
          gatherType(var, t, var->name);
      }
    }
  }

  // check types
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    // is ts->name matching one in sWellKnownTypes or sWellKnownAggregateTypes?
    if (ts->defPoint != nullptr) {
      auto modTag = ts->defPoint->getModule()->modTag;
      if (modTag == MOD_INTERNAL || modTag == MOD_STANDARD) {
        gatherType(ts, ts->type, ts->name);
      }
    }
  }

  if (fMinimalModules == false) {
    // Make sure all well-known types are defined.
    for (int i = 0; i < nTypes; ++i) {
      WellKnownType& wkt = sWellKnownTypes[i];

      if (*wkt.type_ == NULL) {
        USR_FATAL_CONT("Type '%s' must be defined in the "
                       "Chapel internal modules.",
                       wkt.name);
      }
    }

    for (int i = 0; i < nAggregate; ++i) {
      WellKnownAggregateType& wkt = sWellKnownAggregateTypes[i];

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
    // remove types that were defined with a dummy value if
    // we never encountered their defining record/class
    for (int i = 0; i < nEarlyAggregate; i++) {
      WellKnownAggregateTypeNeededEarly& wkt =
        sWellKnownAggregateTypesNeededEarly[i];
      removeIfUndefinedGlobalType(*wkt.type_);
    }
  }
}

std::vector<Type*> getWellKnownTypes()
{
  std::vector<Type*> types;

  int nTypes = sizeof(sWellKnownTypes) / sizeof(sWellKnownTypes[0]);
  int nAggregate = sizeof(sWellKnownAggregateTypes) /
                   sizeof(sWellKnownAggregateTypes[0]);
  int nEarlyAggregate = sizeof(sWellKnownAggregateTypesNeededEarly) /
                        sizeof(sWellKnownAggregateTypesNeededEarly[0]);

  for (int i = 0; i < nTypes; i++) {
    WellKnownType& wkt = sWellKnownTypes[i];
    if (*wkt.type_ != NULL)
      types.push_back(*wkt.type_);
  }
  for (int i = 0; i < nAggregate; i++) {
    WellKnownAggregateType& wkt = sWellKnownAggregateTypes[i];
    if (*wkt.type_ != NULL)
      types.push_back(*wkt.type_);
  }
  for (int i = 0; i < nEarlyAggregate; i++) {
    WellKnownAggregateTypeNeededEarly& wkt =
      sWellKnownAggregateTypesNeededEarly[i];
    if (*wkt.type_ != NULL)
      types.push_back(*wkt.type_);
  }

  return types;
}

void clearGenericWellKnownTypes()
{
  int nTypes = sizeof(sWellKnownTypes) / sizeof(sWellKnownTypes[0]);
  int nAggregate = sizeof(sWellKnownAggregateTypes) /
                   sizeof(sWellKnownAggregateTypes[0]);

  for (int i = 0; i < nTypes; i++) {
    WellKnownType& wkt = sWellKnownTypes[i];
    Type* t = *wkt.type_;
    if (t != NULL && t->symbol && t->symbol->hasFlag(FLAG_GENERIC))
      *wkt.type_ = NULL;
  }
  for (int i = 0; i < nAggregate; i++) {
    WellKnownAggregateType& wkt = sWellKnownAggregateTypes[i];
    if (*wkt.type_ != NULL && (*wkt.type_)->isGeneric())
      *wkt.type_ = NULL;
  }
}

Type* getWellKnownTypeWithName(const char* name) {
  int nTypes = sizeof(sWellKnownTypes) / sizeof(sWellKnownTypes[0]);
  int nAggregate = sizeof(sWellKnownAggregateTypes) /
                   sizeof(sWellKnownAggregateTypes[0]);

  name = astr(name);
  for (int i = 0; i < nTypes; i++) {
    WellKnownType& wkt = sWellKnownTypes[i];
    if (*wkt.type_ != NULL && wkt.name == name)
      return *wkt.type_;
  }
  for (int i = 0; i < nAggregate; i++) {
    WellKnownAggregateType& wkt = sWellKnownAggregateTypes[i];
    if (*wkt.type_ != NULL && wkt.name == name)
      return *wkt.type_;
  }

  return NULL;
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
    "allocate_string_literals_buf",
    &gAllocateStringLiteralsBuf,
    FLAG_UNKNOWN
  },

  {
    "chpl_createStringWithLiteral",
    &gChplCreateStringWithLiteral,
    FLAG_UNKNOWN
  },

  {
    "chpl_createBytesWithLiteral",
    &gChplCreateBytesWithLiteral,
    FLAG_UNKNOWN
  },

  {
    "chpl_buildLocaleID",
    &gChplBuildLocaleId,
    FLAG_UNKNOWN
  },
};

static void initializeWellKnownFunctions() {
  int nEntries = sizeof(sWellKnownFns) / sizeof(sWellKnownFns[0]);

  // update the name fields to be astrs
  for (int i = 0; i < nEntries; ++i) {
    WellKnownFn& wkfn = sWellKnownFns[i];
    wkfn.name = astr(wkfn.name);
  }
}

void gatherWellKnownFns() {
  int nEntries = sizeof(sWellKnownFns) / sizeof(sWellKnownFns[0]);

  // Harvest well-known functions from among the global fn definitions.
  // We check before assigning to the associated global to ensure that it
  // is null.  In that way we can flag duplicate definitions.
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    auto modTag = MOD_USER;
    if (fn->defPoint)
      if (auto mod = fn->defPoint->getModule())
        modTag = mod->modTag;

    if (modTag == MOD_INTERNAL || modTag == MOD_STANDARD) {
      for (int i = 0; i < nEntries; ++i) {
        WellKnownFn& wkfn = sWellKnownFns[i];

        if (fn->name == wkfn.name) {
          wkfn.lastNameMatchedFn = fn;

          if (wkfn.flag == FLAG_UNKNOWN || fn->hasFlag(wkfn.flag) == true) {
            if (*wkfn.fn != NULL) {
              USR_WARN(fn,
                       "'%s' defined more than once in Chapel internal modules",
                       wkfn.name);
            }

            *wkfn.fn = fn;
          }
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
