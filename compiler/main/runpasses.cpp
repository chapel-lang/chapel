#include "runpasses.h"

#include "checks.h"        // For check function prototypes.
#include "log.h"           // For LOG_<passname> #defines.
#include "passes.h"        // For pass function prototypes.
#include "PhaseTracker.h"

#include <cstdio>
#include <sys/time.h>

int   currentPassNo   = 1;

struct PassInfo {
  void (*passFunction) ();      // The function which implements the pass.
  void (*checkFunction)();      // per-pass check function
  const char* name;
  char        logTag;
};

#define RUN(x) {    x, check_ ## x,   #x, LOG_ ## x }

//
// passlist: contains passes in the order that they are called
//
static PassInfo sPassList[] = {
  // Chapel to AST
  RUN(parse),                   // parse files and create AST
  RUN(checkParsed),             // checks semantics of parsed AST

  // Read in runtime and included C header file types/prototypes
  RUN(readExternC),

  // create wrapper functions to allow arrays to be passed to extern routines
  RUN(expandExternArrayCalls),

  // Scope resolution and normalization
  RUN(cleanup),                 // post parsing transformations
  RUN(scopeResolve),            // resolve symbols by scope
  RUN(flattenClasses),          // denest nested classes
  RUN(docs),                    // if fDocs is set, this will generate docs.
                                // if the executable is named "chpldoc" then
                                // the application will stop after this phase

  RUN(normalize),               // normalization transformations
  RUN(checkNormalized),         // check semantics of normalized AST

  RUN(buildDefaultFunctions),   // build default functions
  RUN(createTaskFunctions),     // convert 'begin' et al. to functions

  // Function resolution and shallow type inference
  RUN(resolve),                 // resolves function calls and types
  RUN(resolveIntents),          // resolve argument intents
  RUN(checkResolved),           // checks semantics of resolved AST

  // Post-resolution cleanup
  RUN(processIteratorYields),   // adjustments to iterators
  RUN(flattenFunctions),        // denest nested functions
  RUN(cullOverReferences),      // remove excess references
  RUN(callDestructors),
  RUN(lowerIterators),          // lowers iterators into functions/classes
  RUN(parallel),                // parallel transforms
  RUN(prune),                   // prune AST of dead functions and types

  // Optimizations
  RUN(complex2record),          // change complex numbers into records
  RUN(bulkCopyRecords),         // replace simple assignments with PRIM_ASSIGN.
  RUN(removeUnnecessaryAutoCopyCalls),
  RUN(inlineFunctions),         // function inlining
  RUN(scalarReplace),           // scalar replace all tuples
  RUN(refPropagation),          // reference propagation
  RUN(copyPropagation),         // copy propagation
  RUN(deadCodeElimination),     // eliminate dead code
  RUN(removeWrapRecords),       // remove _array, _domain, and 
                                // _distribution records
  RUN(removeEmptyRecords),      // remove empty records
  RUN(localizeGlobals),         // pull out global constants from loop runs
  RUN(prune2),                  // prune AST of dead functions and types again

  RUN(returnStarTuplesByRefArgs),

  RUN(insertWideReferences),    // inserts wide references for on clauses
  RUN(optimizeOnClauses),       // Optimize on clauses
  RUN(addInitCalls),            // Add module init calls and guards.
  RUN(loopInvariantCodeMotion), // move loop invarient code above loop runs

  // AST to C or LLVM
  RUN(insertLineNumbers),       // insert line numbers for error messages
  RUN(codegen),                 // generate C code
  RUN(makeBinary)               // invoke underlying C compiler
};

static void runPass(PhaseTracker& tracker, size_t passIndex);

void runPasses(PhaseTracker& tracker) {
  bool   isChpldoc    = (strcmp(chplBinaryName, "chpldoc") == 0);
  size_t passListSize = sizeof(sPassList) / sizeof(sPassList[0]);

  setupLogfiles();

  if (printPasses == true || printPassesFile != 0) {
    tracker.ReportPass();
  }

  for (size_t i = 0; i < passListSize; i++) {
    runPass(tracker, i);

    USR_STOP(); // quit if fatal errors were encountered in pass

    currentPassNo++;

    // Break early if this is a chpl doc run
    if (isChpldoc == true && strcmp(sPassList[i].name, "docs") == 0) {
      break;
    }
  }

  destroyAst();
  teardownLogfiles();
}

static void runPass(PhaseTracker& tracker, size_t passIndex) {
  PassInfo* info = &sPassList[passIndex];

  tracker.StartPhase(info->name, PhaseTracker::kPrimary);

  if (fPrintStatistics[0] != '\0' && passIndex > 0)
    printStatistics("clean");

  (*(info->passFunction))();

  tracker.StartPhase(info->name, PhaseTracker::kVerify);

  if (fPrintStatistics[0] != '\0')
    printStatistics(info->name);

  log_writeLog(info->name, currentPassNo, info->logTag);

  considerExitingEndOfPass();

  tracker.StartPhase(info->name, PhaseTracker::kCleanAst);
  cleanAst();

  (*(info->checkFunction))(); // Run per-pass check function.

  if (printPasses == true || printPassesFile != 0) {
    tracker.ReportPass();
  }
}

//
// The logging machinery wants to know a "name" for every pass that it can
// match to command line arguments but does not, currently, want to know
// about the pass list itself.
//
//  This function provides a vector of the pass list names
//
// This routine also verifies that each non-NUL flag is unique.

void initLogFlags(Vec<char>& valid_log_flags) {
  size_t passListSize = sizeof(sPassList) / sizeof(sPassList[0]);

  for (size_t i = 0; i < passListSize; i++) {
    PassInfo* pass = &sPassList[i];
    char      tag  = pass->logTag;

    if (tag != NUL) {
      INT_ASSERT(!valid_log_flags.set_in(tag));
      valid_log_flags.set_add(tag);
    }
  }
}
