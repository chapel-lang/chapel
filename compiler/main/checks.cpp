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

// checks.cpp

#include "checks.h"

#include "expr.h"
#include "passes.h"
#include "primitive.h"
#include "resolution.h"

//
// Static function declarations.
//
// (These collect checks applicable to various phases.)
//

static void check_afterEveryPass(); // Checks to be performed after every pass.
static void check_afterScopeResolve(); // Checks to be performeed after the
                                       // scopeResolve pass.
static void check_afterNormalization(); // Checks to be performed after
                                        // normalization.
static void check_afterResolution(); // Checks to be performed after every pass
                                     // following resolution.
static void check_afterCallDestructors(); // Checks to be performed after every
                                          // pass following callDestructors.
static void check_afterLowerIterators();
static void checkAggregateTypes(); // Checks that class and record types have
                                   // default initializers and default type
                                   // constructors.
static void checkResolveRemovedPrims(void); // Checks that certain primitives
                                            // are removed after resolution
static void checkTaskRemovedPrims(); // Checks that certain primitives are
                                     // removed after task functions are
                                     // created.
static void checkLowerIteratorsRemovedPrims();
static void checkFlagRelationships(); // Checks expected relationships between
                                      // flags.
static void checkAutoCopyMap();
static void checkFormalActualBaseTypesMatch();
static void checkRetTypeMatchesRetVarType();
static void checkFormalActualTypesMatch();


//
// Implementations.
//

void check_parse()
{
  check_afterEveryPass();
}

void check_checkParsed()
{
  // The checkParsed pass should not make any changes, so skip checks.
}

void check_readExternC()
{
  check_afterEveryPass();
  // Suggestion: Ensure extern C types defined.
}

void check_expandExternArrayCalls()
{
  check_afterEveryPass();
}

void check_cleanup()
{
  check_afterEveryPass();
  // Suggestion: Identify and check normalizations applied by cleanup pass.
}

void check_scopeResolve()
{
  check_afterEveryPass();
  check_afterScopeResolve();
  // Suggestion: Ensure identifiers resolved to scope.
}

void check_flattenClasses()
{
  check_afterEveryPass();
  // Suggestion: Ensure classes have no nested class definitions.
}

void check_docs()
{
  // Docs should not alter the tree, so no checks are required.
}

void check_normalize()
{
  check_afterEveryPass();
}

void check_checkNormalized()
{
  // The checkNormalized pass should not make any changes, so skip checks.
}

void check_buildDefaultFunctions()
{
  check_afterEveryPass();
  check_afterNormalization();
  // Suggestion: Ensure each class/record type has its own default functions.
}

void check_createTaskFunctions()
{
  checkTaskRemovedPrims();
  check_afterEveryPass();
  check_afterNormalization();
}

void check_resolve()
{
  checkResolveRemovedPrims();
  check_afterEveryPass();
  check_afterNormalization();
  checkReturnTypesHaveRefTypes();
  checkAutoCopyMap();
}

void check_resolveIntents()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterResolution();
  // Suggestion: Ensure now using a reduced set of intents.
}

void check_checkResolved()
{
  // The checkResolved pass should not make any changes, so skip checks.
}

void check_processIteratorYields() {
  check_afterEveryPass();
  check_afterNormalization();
  check_afterResolution();
}

void check_flattenFunctions()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterResolution();
  checkFormalActualTypesMatch();
  // Suggestion: Ensure no nested functions.
}

void check_cullOverReferences()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterResolution();
  checkFormalActualTypesMatch();
}

void check_callDestructors()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  // Suggestion: Ensure every constructor call has a matching destructor call.
}

void check_lowerIterators()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
//  check_afterResolution(); // Oho! Iterator functions do not obey the invariant
  // checked in checkReturnPaths() [semanticChecks.cpp:250].
  // So check_afterResolution has been disabled in this and all subsequent post-pass
  // checks.
  // lowerIterators() should be revised to produce code that obeys all
  // invariants and then these (paranoid) tests re-enabled.
}

void check_parallel()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
  // Suggestion: Ensure parallelization applied (if not --local).
}

void check_prune()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
  // Suggestion: Ensure no dead classes or functions.
}

void check_complex2record()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
  // Suggestion: Ensure no more constants or variables of complex type.
}

void check_bulkCopyRecords()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
}

void check_removeUnnecessaryAutoCopyCalls()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
  // Suggestion: Ensure no unnecessary autoCopy calls.
}

void check_inlineFunctions()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
}

void check_scalarReplace()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
  // Suggestion: Ensure no constant expresions.
}

void check_refPropagation()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
}

void check_copyPropagation()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();}


void check_deadCodeElimination()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators(); 
  // Suggestion: Ensure no dead code.
}

void check_removeWrapRecords()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
  // Suggestion: Ensure no more wrap records.
}

void check_removeEmptyRecords()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
  // Suggestion: Ensure no empty records.
}

void check_localizeGlobals()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
}

void check_loopInvariantCodeMotion()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
}

void check_prune2()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
  // Suggestion: Ensure no dead classes or functions.
}

void check_returnStarTuplesByRefArgs()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
}

void check_insertWideReferences()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
}

void check_narrowWideReferences()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
}

void check_optimizeOnClauses()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
}

void check_addInitCalls()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
}

void check_insertLineNumbers()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
}

void check_codegen()
{
  // This pass should not change the AST, so no checks are required.
}

void check_makeBinary()
{
  // This pass should not change the AST, so no checks are required.
}

//////////////////////////////////////////////////////////////////////////
// Utility functions
//

// Extra structural checks on the AST, applicable to all passes.
void check_afterEveryPass()
{
  if (fVerify) 
  {
    verify();
    checkForDuplicateUses();
    checkFlagRelationships();
  }
}

static void check_afterScopeResolve()
{
  if (fVerify)
  {
    checkAggregateTypes();
  }
}

// Checks that should remain true after the normalization pass is complete.
static void check_afterNormalization()
{
  if (fVerify)
  {
    checkNormalized();
  }
}

// Checks that should remain true after the functionResolution pass is complete.
static void check_afterResolution()
{
  checkReturnTypesHaveRefTypes();
  if (fVerify)
  {
    checkTaskRemovedPrims();
    checkResolveRemovedPrims();
// Disabled for now because user warnings should not be logged multiple times:
//    checkResolved();
// Disabled for now because it does not hold when named externs are present.
// See test/extern/hilde/namedExtern.chpl.
//    checkNoUnresolveds();
    checkFormalActualBaseTypesMatch();
    checkRetTypeMatchesRetVarType();
    checkAutoCopyMap();
  }
}


// Checks that should remain true after the callDestructors pass is complete.
static void check_afterCallDestructors()
{
  // Disabled because it is not true after the first prune pass.
  //  checkReturnTypesHaveRefTypes();
  if (fVerify)
  {
// Disabled for now because user warnings should not be logged multiple times:
//    checkResolved();
// Disabled for now because it does not hold when named externs are present.
// See test/extern/hilde/namedExtern.chpl.
//    checkNoUnresolveds();
    checkFormalActualTypesMatch();
  }
}


static void check_afterLowerIterators()
{
  checkLowerIteratorsRemovedPrims();
}


//
// Checks that class and record types have a default initializer and a default
// type constructor.
//
static void checkAggregateTypes()
{
  forv_Vec(AggregateType, at, gAggregateTypes)
  {
    if (! at->defaultInitializer)
      INT_FATAL(at, "aggregate type has no initializer");
    if (! at->defaultTypeConstructor)
      INT_FATAL(at, "aggregate type has no default type constructor");
  }
}


//
// Checks that certain primitives are removed after function resolution
//
static void
checkResolveRemovedPrims(void) {
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->primitive) {
      switch(call->primitive->tag) {
        case PRIM_BLOCK_PARAM_LOOP:
        case PRIM_INIT:
        case PRIM_NO_INIT:
        case PRIM_TYPE_INIT:
        case PRIM_LOGICAL_FOLDER:
        case PRIM_TYPEOF:
        case PRIM_TYPE_TO_STRING:
        case PRIM_IS_SYNC_TYPE:
        case PRIM_IS_SINGLE_TYPE:
        case PRIM_IS_TUPLE_TYPE:
        case PRIM_IS_STAR_TUPLE_TYPE:
        case PRIM_IS_SUBTYPE:
        case PRIM_TUPLE_EXPAND:
        case PRIM_QUERY:
        case PRIM_QUERY_PARAM_FIELD:
        case PRIM_QUERY_TYPE_FIELD:
        case PRIM_ERROR:
        case PRIM_FORALL_LOOP:
          if (call->parentSymbol)
            INT_FATAL("Primitive should no longer be in AST");
          break;
        default:
          break;
      }
    }
  }
}

static void 
checkTaskRemovedPrims()
{
  forv_Vec(CallExpr, call, gCallExprs)
    if (call->primitive)
      switch(call->primitive->tag)
      {
       case PRIM_BLOCK_BEGIN:
       case PRIM_BLOCK_COBEGIN:
       case PRIM_BLOCK_COFORALL:
       case PRIM_BLOCK_ON:
       case PRIM_BLOCK_BEGIN_ON:
       case PRIM_BLOCK_COBEGIN_ON:
       case PRIM_BLOCK_COFORALL_ON:
        if (call->parentSymbol)
          INT_FATAL("Primitive should no longer be in AST");
        break;
       default:
        break;
      }
}

static void 
checkLowerIteratorsRemovedPrims()
{
  forv_Vec(CallExpr, call, gCallExprs)
    if (call->primitive)
      switch(call->primitive->tag)
      {
       case PRIM_YIELD:
        if (call->parentSymbol)
          INT_FATAL("Primitive should no longer be in AST");
        break;
       default:
        break;
      }
}

// Some flags imply other flags.
// Make sure that these relations always hold.
static void
checkFlagRelationships()
{
  forv_Vec(DefExpr, def, gDefExprs)
  {
    // These tests apply to function symbols.
    if (FnSymbol* fn = toFnSymbol(def->sym))
    {
      // FLAG_EXTERN => FLAG_LOCAL_ARGS
      INT_ASSERT(!fn->hasFlag(FLAG_EXTERN) || fn->hasFlag(FLAG_LOCAL_ARGS));

      // FLAG_EXPORT => FLAG_LOCAL_ARGS
      INT_ASSERT(!fn->hasFlag(FLAG_EXPORT) || fn->hasFlag(FLAG_LOCAL_ARGS));
    }
  }
}

static void
checkAutoCopyMap()
{
  Vec<Type*> keys;
  autoCopyMap.get_keys(keys);
  forv_Vec(Type, key, keys)
  {
    FnSymbol* fn = autoCopyMap.get(key);
    Type* baseType = fn->getFormal(1)->getValType();
    INT_ASSERT(baseType == key);
  }
}


// TODO: Can this be merged with checkFormalActualTypesMatch()?
static void
checkFormalActualBaseTypesMatch()
{
  forv_Vec(CallExpr, call, gCallExprs)
  {
    if (! call->parentSymbol)
      // Call is not in tree
      continue;

    // Only look at calls in functions that have been resolved.
    if (! call->parentSymbol->hasFlag(FLAG_RESOLVED))
      continue;

    if (FnSymbol* fn = call->isResolved())
    {
      if (fn->hasFlag(FLAG_EXTERN))
        continue;

      if (! fn->hasFlag(FLAG_RESOLVED))
        continue;

      for_formals_actuals(formal, actual, call)
      {
        if (actual->typeInfo() == dtNil) {
          if (formal->type == dtNil)
            // Exact match, so OK.
            continue;

          if (isClass(formal->type))
            // dtNil can be converted to any class type, so OK.
            continue;

          // All other cases == error.
          INT_FATAL(call, "nil is passed to the formal %s of a non-class type",
                    formal->name);
        }

        if (formal->type->getValType() != actual->typeInfo()->getValType())
          INT_FATAL(call,
                    "actual formal type mismatch for %s: %s != %s",
                    fn->name,
                    actual->typeInfo()->symbol->name,
                    formal->type->symbol->name);
      }
    }
  }
}

// After resolution the retType field is just a cached version of the type of
// the return value variable.
static void
checkRetTypeMatchesRetVarType()
{
  forv_Vec(FnSymbol, fn, gFnSymbols)
  {
    if (fn->isIterator())
      // Iterators break this rule.
      // retType is the type of the iterator record
      // The return value type is the type of the index the iterator returns.
      continue;
    if (fn->hasFlag(FLAG_AUTO_II))
      // auto ii functions break this rule, but only during the time that
      // they are prototypes.  After the body is filled in, they should obey it.
      continue;
    INT_ASSERT(fn->retType == fn->getReturnSymbol()->type);
  }
}

static void
checkFormalActualTypesMatch()
{
  forv_Vec(CallExpr, call, gCallExprs)
  {
    if (FnSymbol* fn = call->isResolved())
    {
      if (fn->hasFlag(FLAG_EXTERN))
        continue;

      // Calls generated by the conversion of PRIM_TO_STANDALONE,
      // PRIM_TO_LEADER and PRIM_TO_FOLLOWER in postFold require fixup by
      // cleanupLeaderFollowerIteratorCalls() in lowerIterators.cpp.  Before
      // iterator lowering is complete, we skip those calls specifically,
      // because they are not expected to obey the invariant being tested here
      // (that formal and actual types match exactly).
      if (! iteratorsLowered)
      {
        // These tests are copied verbatim from the tests that select the calls
        // of interest in cleanupLeaderFollowerIteratorCalls().
        if (FnSymbol* fn = call->isResolved()) {
          if (fn->retType->symbol->hasFlag(FLAG_ITERATOR_RECORD) ||
              (isDefExpr(fn->formals.tail) &&
               !strcmp(toDefExpr(fn->formals.tail)->sym->name, "_retArg") &&
               toDefExpr(fn->formals.tail)->sym->getValType() &&
               toDefExpr(fn->formals.tail)->sym->getValType()->symbol->hasFlag(FLAG_ITERATOR_RECORD))) {
            if (!strcmp(call->parentSymbol->name, "_toLeader") ||
                !strcmp(call->parentSymbol->name, "_toFollower") ||
                !strcmp(call->parentSymbol->name, "_toFastFollower") ||
                !strcmp(call->parentSymbol->name, "_toStandalone")) {
              continue;
            }
          }
        }
      }

      for_formals_actuals(formal, actual, call)
      {
        if (actual->typeInfo() == dtNil) {
          if (formal->type == dtNil)
            // Exact match, so OK.
            continue;

          if (isClass(formal->type))
            // dtNil can be converted to any class type, so OK.
            continue;

          // All other cases == error.
          INT_FATAL(call, "nil is passed to the formal %s of a non-class type",
                    formal->name);
        }

        if (formal->type != actual->typeInfo())
          INT_FATAL(call,
                    "actual formal type mismatch for %s: %s != %s",
                    fn->name,
                    actual->typeInfo()->symbol->name,
                    formal->type->symbol->name);
      }
    }
  }
}

