// checks.cpp

#include "checks.h"

#include "expr.h"
#include "passes.h"
#include "primitive.h"

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
static void checkResolveRemovedPrims(void); // Checks that certain primitives
                                            // are removed after resolution



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
  check_afterEveryPass();
  check_afterNormalization();
}

void check_resolve()
{
  checkResolveRemovedPrims();
  check_afterEveryPass();
  check_afterNormalization();
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
//  check_afterResolution();
  // Suggestion: Ensure every constructor call has a matching destructor call.
}

void check_lowerIterators()
{
  check_afterEveryPass();
  check_afterNormalization();
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
//  check_afterResolution();
  // Suggestion: Ensure parallelization applied (if not --local).
}

void check_prune()
{
  check_afterEveryPass();
  check_afterNormalization();
//  check_afterResolution();
  // Suggestion: Ensure no dead classes or functions.
}

void check_complex2record()
{
  check_afterEveryPass();
  check_afterNormalization();
//  check_afterResolution();
  // Suggestion: Ensure no more constants or variables of complex type.
}

void check_removeUnnecessaryAutoCopyCalls()
{
  check_afterEveryPass();
  check_afterNormalization();
//  check_afterResolution();
  // Suggestion: Ensure no unnecessary autoCopy calls.
}

void check_inlineFunctions()
{
  check_afterEveryPass();
  check_afterNormalization();
//  check_afterResolution();
}

void check_scalarReplace()
{
  check_afterEveryPass();
  check_afterNormalization();
//  check_afterResolution();
  // Suggestion: Ensure no constant expresions.
}

void check_refPropagation()
{
  check_afterEveryPass();
  check_afterNormalization();
//  check_afterResolution();
}

void check_copyPropagation()
{
  check_afterEveryPass();
  check_afterNormalization();
//  check_afterResolution();
}

void check_deadCodeElimination()
{
  check_afterEveryPass();
  check_afterNormalization();
//  check_afterResolution();
  // Suggestion: Ensure no dead code.
}

void check_removeWrapRecords()
{
  check_afterEveryPass();
  check_afterNormalization();
//  check_afterResolution();
  // Suggestion: Ensure no more wrap records.
}

void check_removeEmptyRecords()
{
  check_afterEveryPass();
  check_afterNormalization();
//  check_afterResolution();
  // Suggestion: Ensure no empty records.
}

void check_localizeGlobals()
{
  check_afterEveryPass();
  check_afterNormalization();
//  check_afterResolution();
}

void check_loopInvariantCodeMotion()
{
  check_afterEveryPass();
  check_afterNormalization();
//  check_afterResolution();
}

void check_prune2()
{
  check_afterEveryPass();
  check_afterNormalization();
//  check_afterResolution();
  // Suggestion: Ensure no dead classes or functions.
}

void check_returnStarTuplesByRefArgs()
{
  check_afterEveryPass();
  check_afterNormalization();
//  check_afterResolution();
}

void check_insertWideReferences()
{
  check_afterEveryPass();
  check_afterNormalization();
//  check_afterResolution();
}

void check_optimizeOnClauses()
{
  check_afterEveryPass();
  check_afterNormalization();
//  check_afterResolution();
}

void check_addInitCalls()
{
  check_afterEveryPass();
  check_afterNormalization();
//  check_afterResolution();
}

void check_insertLineNumbers()
{
  check_afterEveryPass();
  check_afterNormalization();
//  check_afterResolution();
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

// Checks that should remain true after the resolution pass is complete.
static void check_afterResolution()
{
  if (fVerify)
  {
// Disabled for now because user warnings should not be logged multiple times:
//    checkResolved();
// Disabled for now because it does not hold when named externs are present.
// See test/extern/hilde/namedExtern.chpl.
//    checkNoUnresolveds();
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
        case PRIM_INIT:
        case PRIM_LOGICAL_FOLDER:
        case PRIM_TYPEOF:
        case PRIM_TYPE_TO_STRING:
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
