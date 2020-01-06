/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

#include "docsDriver.h"
#include "driver.h"
#include "expr.h"
#include "PartialCopyData.h"
#include "passes.h"
#include "primitive.h"
#include "resolution.h"
#include "TryStmt.h"

//
// Static function declarations.
//
// (These collect checks applicable to various phases.)
//

static void check_afterEveryPass(); // Checks to be performed after every pass.
static void check_afterScopeResolve(); // Checks to be performed after the
                                       // scopeResolve pass.
static void check_afterNormalization(); // Checks to be performed after
                                        // normalization.
static void check_afterResolution(); // Checks to be performed after every pass
                                     // following resolution.
static void check_afterResolveIntents();
static void check_afterLowerErrorHandling();
static void check_afterCallDestructors(); // Checks to be performed after every
                                          // pass following callDestructors.
static void check_afterLowerIterators();
static void checkIsIterator(); // Ensure each iterator is flagged so.
static void check_afterInlineFunctions();
static void checkResolveRemovedPrims(void); // Checks that certain primitives
                                            // are removed after resolution
static void checkNoRecordDeletes();  // No 'delete' on records.
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
  // checkIsIterator() will crash if there were certain USR_FATAL_CONT()
  // e.g. functions/vass/proc-iter/error-yield-in-proc-*
  exitIfFatalErrorsEncountered();
  checkIsIterator();
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
  check_afterResolveIntents();
  // Suggestion: Ensure now using a reduced set of intents.
}

void check_checkResolved()
{
  // The checkResolved pass should not make any changes, so skip checks.
}

void check_replaceArrayAccessesWithRefTemps()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterResolution();
  check_afterResolveIntents();
}

void check_flattenFunctions()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterResolution();
  check_afterResolveIntents();
  // Suggestion: Ensure no nested functions.
}

void check_cullOverReferences()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterResolution();
  check_afterResolveIntents();

  // No ContextCallExprs should remain in the tree.
  for_alive_in_Vec(ContextCallExpr, cc, gContextCallExprs) {
    INT_FATAL("ContextCallExpr should no longer be in AST");
  }
}

void check_lowerErrorHandling()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterLowerErrorHandling();
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
  check_afterResolveIntents();
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
  check_afterResolveIntents();
  // Suggestion: Ensure parallelization applied (if not --local).
}

void check_prune()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
  check_afterResolveIntents();
  // Suggestion: Ensure no dead classes or functions.
}

void check_bulkCopyRecords()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
  check_afterResolveIntents();
}

void check_removeUnnecessaryAutoCopyCalls()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
  check_afterResolveIntents();
  // Suggestion: Ensure no unnecessary autoCopy calls.
}

void check_inlineFunctions()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
  check_afterResolveIntents();
  check_afterInlineFunctions();
}

void check_scalarReplace()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
  check_afterResolveIntents();
  check_afterInlineFunctions();
  // Suggestion: Ensure no constant expressions.
}

void check_refPropagation()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
  check_afterResolveIntents();
  check_afterInlineFunctions();
}

void check_copyPropagation()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
  check_afterResolveIntents();
  check_afterInlineFunctions();
}


void check_deadCodeElimination()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
  check_afterResolveIntents();
  check_afterInlineFunctions();
  // Suggestion: Ensure no dead code.
}

void check_removeEmptyRecords()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
  check_afterResolveIntents();
  check_afterInlineFunctions();
  // Suggestion: Ensure no empty records.
}

void check_localizeGlobals()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
  check_afterResolveIntents();
  check_afterInlineFunctions();
}

void check_loopInvariantCodeMotion()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
  check_afterResolveIntents();
  check_afterInlineFunctions();
}

void check_prune2()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
  check_afterResolveIntents();
  check_afterInlineFunctions();
  // Suggestion: Ensure no dead classes or functions.
}

void check_returnStarTuplesByRefArgs()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
  check_afterResolveIntents();
  check_afterInlineFunctions();
}

void check_insertWideReferences()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
  check_afterResolveIntents();
  check_afterInlineFunctions();
}

void check_optimizeOnClauses()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
  check_afterResolveIntents();
  check_afterInlineFunctions();
}

void check_addInitCalls()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
  check_afterResolveIntents();
  check_afterInlineFunctions();
}

void check_insertLineNumbers()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  check_afterLowerIterators();
  check_afterInlineFunctions();
}

void check_denormalize() {
  //do we need to call any checks here ?
  //or implement new checks ?
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
    checkEmptyPartialCopyDataFnMap();
  }
}

static void check_afterScopeResolve()
{
  if (fVerify)
  {
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
    checkNoRecordDeletes();
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

static void check_afterResolveIntents()
{
  if (fVerify) {
    for_alive_in_Vec(DefExpr, def, gDefExprs) {
      Symbol* sym = def->sym;
      // Only look at Var or Arg symbols
      if (isLcnSymbol(sym)) {
        QualifiedType qual = sym->qualType();
        // MPF TODO: This should not be necessary
        // it is a workaround for problems with --verify
        // with tuple type constructors accepting domains.
        // It would be better to treat run-time types as
        // normal records.
        if (ArgSymbol* arg = toArgSymbol(sym))
          if (arg->intent == INTENT_TYPE)
            continue;

        if (qual.getQual() == QUAL_UNKNOWN) {
          INT_FATAL("Symbol should not have unknown qualifier: %s (%d)", sym->cname, sym->id);
        }
      }
    }
  }
}


static void check_afterLowerErrorHandling()
{
  if (fVerify)
  {
    // check that TryStmt is not in the tree
    forv_Vec(TryStmt, stmt, gTryStmts)
    {
      if (stmt->inTree())
        INT_FATAL(stmt, "TryStmt should no longer be in the tree");
    }

    // TODO: check no more CatchStmt

    // check no more PRIM_THROW
    forv_Vec(CallExpr, call, gCallExprs)
    {
      if (call->isPrimitive(PRIM_THROW) && call->inTree())
        INT_FATAL(call, "PRIM_THROW should no longer exist");
    }
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
  if (fVerify)
    checkArgsAndLocals();
}

static void check_afterInlineFunctions() {
  if (fVerify) {
    forv_Vec(DefExpr, def, gDefExprs) {
      Symbol* sym = def->sym;
      if (isLcnSymbol(sym) &&
          def->inTree() && // symbol is in the tree
          def->parentSymbol->hasFlag(FLAG_WIDE_REF) == false) {
        if (sym->type->symbol->hasFlag(FLAG_REF) ||
            sym->type->symbol->hasFlag(FLAG_WIDE_REF)) {
          INT_FATAL("Found reference type: %s[%d]\n", sym->cname, sym->id);
        }
      }
    }
  }
}

static void checkIsIterator() {
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_YIELD)) {
      FnSymbol* fn = toFnSymbol(call->parentSymbol);
      if (!fn && fDocs)
        // In docs mode some nodes are not in tree, so skip the check.
        continue;
      // Violations should have caused USR_FATAL_CONT in checkParsed().
      INT_ASSERT(fn && fn->isIterator());
    }
  }
}


//
// Checks that certain primitives are removed after function resolution
//
static void
checkResolveRemovedPrims(void) {
  for_alive_in_Vec(CallExpr, call, gCallExprs) {
    if (call->primitive) {
      switch(call->primitive->tag) {
        case PRIM_BLOCK_PARAM_LOOP:

        case PRIM_DEFAULT_INIT_VAR:
        case PRIM_INIT_FIELD:
        case PRIM_INIT_VAR:

        case PRIM_LOGICAL_FOLDER:
        case PRIM_TYPEOF:
        case PRIM_TYPE_TO_STRING:
        case PRIM_IS_TUPLE_TYPE:
        case PRIM_IS_STAR_TUPLE_TYPE:
        case PRIM_IS_SUBTYPE:
        case PRIM_REDUCE:
        case PRIM_REDUCE_ASSIGN:
        case PRIM_TUPLE_EXPAND:
        case PRIM_QUERY:
        case PRIM_QUERY_PARAM_FIELD:
        case PRIM_QUERY_TYPE_FIELD:
        case PRIM_ERROR:
        case PRIM_COERCE:
        case PRIM_GATHER_TESTS:
          if (call->parentSymbol)
            INT_FATAL("Primitive should no longer be in AST");
          break;
        default:
          break;
      }
    }
  }
}

static void checkNoRecordDeletes() {
  // No need to do for_alive_in_Vec - there shouldn't be any, period.
  // User errors are to be detected by chpl__delete() in the modules.
  forv_Vec(CallExpr, call, gCallExprs)
    if (FnSymbol* fn = call->resolvedFunction())
      if(fn->hasFlag(FLAG_DESTRUCTOR))
        if (!isClassLike(call->get(1)->typeInfo()->getValType()))
          INT_FATAL(call, "delete not on a class");
}

static void
checkTaskRemovedPrims()
{
  for_alive_in_Vec(CallExpr, call, gCallExprs)
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
  for_alive_in_Vec(CallExpr, call, gCallExprs)
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
  for_alive_in_Vec(DefExpr, def, gDefExprs)
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
  getAutoCopyTypeKeys(keys);
  forv_Vec(Type, key, keys)
  {
    if (hasAutoCopyForType(key)) {
      FnSymbol* fn = getAutoCopyForType(key);
      if (fn->numFormals() > 0) {
        Type* baseType = fn->getFormal(1)->getValType();
        INT_ASSERT(baseType == key);
      }
    }
  }
}


// TODO: Can this be merged with checkFormalActualTypesMatch()?
static void
checkFormalActualBaseTypesMatch()
{
  for_alive_in_Vec(CallExpr, call, gCallExprs)
  {
    if (! call->parentSymbol)
      // Call is not in tree
      continue;

    // Only look at calls in functions that have been resolved.
    if (! call->parentSymbol->hasFlag(FLAG_RESOLVED))
      continue;

    if (FnSymbol* fn = call->resolvedFunction())
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

          if (isClassLikeOrPtr(formal->type))
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
  for_alive_in_Vec(FnSymbol, fn, gFnSymbols)
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
  for_alive_in_Vec(CallExpr, call, gCallExprs)
  {
    if (FnSymbol* fn = call->resolvedFunction())
    {
      if (fn->hasFlag(FLAG_EXTERN))
        continue;

      for_formals_actuals(formal, actual, call)
      {
        if (actual->typeInfo() == dtNil) {
          if (formal->type == dtNil)
            // Exact match, so OK.
            continue;

          if (isClassLikeOrPtr(formal->type))
            // dtNil can be converted to any class type, so OK.
            continue;

          // All other cases == error.
          INT_FATAL(call, "nil is passed to the formal %s of a non-class type",
                    formal->name);
        }

        if (SymExpr* se = toSymExpr(actual)) {
          if (se->symbol() == gDummyRef && formal->hasFlag(FLAG_RETARG))
            // The compiler generates this combination.
            continue;
        }

        if (formal->getValType() != actual->getValType()) {
          INT_FATAL(call,
                    "actual formal type mismatch for %s: %s != %s",
                    fn->name,
                    actual->typeInfo()->symbol->name,
                    formal->type->symbol->name);
        }
      }
    }
  }
}

