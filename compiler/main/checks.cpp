// checks.cpp

#include "checks.h"

#include "expr.h"
#include "passes.h"
#include "primitive.h"
#include "../resolution/resolution.h"

//
// Static function declarations.
//
// (These collect checks applicable to various phases.)
//

static void check_afterEveryPass(); // Checks to be performed after every pass.
static void check_afterNormalization(); // Checks to be performed after
                                        // normalization.
static void check_afterResolution(); // Checks to be performed after every pass
                                     // following resolution.
static void check_afterCallDestructors(); // Checks to be performed after every
                                          // pass following callDestructors.
static void checkResolveRemovedPrims(void); // Checks that certain primitives
                                            // are removed after resolution
static void checkTaskRemovedPrims(); // Checks that certain primitives are
                                     // removed after task functions are
                                     // created.
static void checkFlagRelationships(); // Checks expected relationships between
                                      // flags.
static void checkAutoCopyMap();
static void checkFormalActualBaseTypesMatch();
static void checkFormalActualTypesMatch();
static void checkNoWrapperMoves();


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
  // Suggestion: Ensure no nested functions.
}

void check_cullOverReferences()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterResolution();
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
  // Suggestion: Ensure parallelization applied (if not --local).
}

void check_prune()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  // Suggestion: Ensure no dead classes or functions.
}

void check_complex2record()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  // Suggestion: Ensure no more constants or variables of complex type.
}

void check_bulkCopyRecords()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
}

void check_removeUnnecessaryAutoCopyCalls()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  // Suggestion: Ensure no unnecessary autoCopy calls.
}

void check_inlineFunctions()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
}

void check_scalarReplace()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  // Suggestion: Ensure no constant expresions.
}

void check_refPropagation()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
}

void check_copyPropagation()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
}

void check_deadCodeElimination()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  // Suggestion: Ensure no dead code.
}

void check_removeWrapRecords()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  // Suggestion: Ensure no more wrap records.
}

void check_removeEmptyRecords()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  // Suggestion: Ensure no empty records.
}

void check_localizeGlobals()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
}

void check_loopInvariantCodeMotion()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
}

void check_prune2()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
  // Suggestion: Ensure no dead classes or functions.
}

void check_returnStarTuplesByRefArgs()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
}

void check_insertWideReferences()
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
}

void check_addInitCalls()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
}

void check_insertLineNumbers()
{
  check_afterEveryPass();
  check_afterNormalization();
  check_afterCallDestructors();
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
    checkAutoCopyMap();
    // This one can be removed after all wrapper moves are purged.
    checkNoWrapperMoves();
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
        case PRIM_ERROR:
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

static void
checkFormalActualBaseTypesMatch()
{
  forv_Vec(CallExpr, call, gCallExprs)
  {
    if (FnSymbol* fn = call->isResolved())
    {
      if (fn->hasFlag(FLAG_EXTERN))
        continue;
      for_formals_actuals(formal, actual, call)
      {
        if (actual->typeInfo() == dtNil) {
          if (!isClass(formal->type))
            INT_FATAL(call, "nil is passed to the formal %s of a non-class type",
                      formal->name);
          continue;
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

static void
checkFormalActualTypesMatch()
{
  forv_Vec(CallExpr, call, gCallExprs)
  {
    if (FnSymbol* fn = call->isResolved())
    {
      if (fn->hasFlag(FLAG_EXTERN))
        continue;
      for_formals_actuals(formal, actual, call)
      {
        if (actual->typeInfo() == dtNil) {
          if (!isClass(formal->type))
            INT_FATAL(call, "nil is passed to the formal %s of a non-class type",
                      formal->name);
          continue;
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

// Checks to make sure there are no wrapper moves around assignment operator calls.
static void
checkNoWrapperMoves()
{
  forv_Vec(CallExpr, call, gCallExprs)
  {
    if (FnSymbol* fn = call->isResolved())
    {
      // We are only interested in assign ops.
      if (! fn->hasFlag(FLAG_ASSIGNOP))
        continue;

      CallExpr* parent = toCallExpr(call->parentExpr);
      if (! parent)
        continue;

      if (parent->isPrimitive(PRIM_MOVE))
        INT_FATAL(parent, "Tree should not contain any wrapper moves.");
    }
  }
}
