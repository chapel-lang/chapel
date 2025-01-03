/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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

#include "runpasses.h"

#include "checks.h"
#include "driver.h"
#include "log.h"
#include "parser.h"
#include "passes.h"
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

// These entries should be kept in the same order as those in the pass list
#define LOG_parseAndConvertUast                'p'
#define LOG_checkGeneratedAst                  LOG_NEVER
#define LOG_readExternC                        LOG_NO_SHORT
#define LOG_cleanup                            LOG_NO_SHORT
#define LOG_scopeResolve                       's'
#define LOG_flattenClasses                     LOG_NO_SHORT
#define LOG_normalize                          'n'
#define LOG_checkNormalized                    LOG_NEVER
#define LOG_buildDefaultFunctions              LOG_NO_SHORT
#define LOG_createTaskFunctions                LOG_NO_SHORT
#define LOG_expandExternArrayCalls             LOG_NO_SHORT
#define LOG_resolve                            'r'
#define LOG_resolveIntents                     LOG_NO_SHORT
#define LOG_checkResolved                      LOG_NEVER
#define LOG_replaceArrayAccessesWithRefTemps   LOG_NO_SHORT
#define LOG_flattenFunctions                   LOG_NO_SHORT
#define LOG_cullOverReferences                 LOG_NO_SHORT
#define LOG_lowerErrorHandling                 LOG_NO_SHORT
#define LOG_callDestructors                    LOG_NO_SHORT
#define LOG_lowerIterators                     LOG_NO_SHORT
#define LOG_parallel                           LOG_NO_SHORT
#define LOG_prune                              LOG_NO_SHORT
#define LOG_bulkCopyRecords                    LOG_NO_SHORT
#define LOG_removeUnnecessaryAutoCopyCalls     LOG_NO_SHORT
#define LOG_inlineFunctions                    LOG_NO_SHORT
#define LOG_scalarReplace                      LOG_NO_SHORT
#define LOG_refPropagation                     LOG_NO_SHORT
#define LOG_copyPropagation                    LOG_NO_SHORT
#define LOG_deadCodeElimination                LOG_NO_SHORT
#define LOG_removeEmptyRecords                 LOG_NO_SHORT
#define LOG_localizeGlobals                    LOG_NO_SHORT
#define LOG_loopInvariantCodeMotion            LOG_NO_SHORT
#define LOG_prune2                             LOG_NO_SHORT
#define LOG_returnStarTuplesByRefArgs          LOG_NO_SHORT
#define LOG_insertWideReferences               LOG_NO_SHORT
#define LOG_optimizeOnClauses                  LOG_NO_SHORT
#define LOG_addInitCalls                       LOG_NO_SHORT
#define LOG_insertLineNumbers                  LOG_NO_SHORT
#define LOG_denormalize                        LOG_NO_SHORT
#define LOG_codegen                            'c'
#define LOG_makeBinary                         LOG_NEVER

#define RUN(x) { x, check_ ## x, #x, LOG_ ## x }

//
// passlist: contains passes in the order that they are called
//
static PassInfo sPassList[] = {
  // Chapel to AST
  RUN(parseAndConvertUast),     // parse files and generate AST
  RUN(checkGeneratedAst),       // checks semantics of generated AST

  // Read in runtime and included C header file types/prototypes
  RUN(readExternC),

  // create wrapper functions to allow arrays to be passed to extern routines
  RUN(expandExternArrayCalls),

  // Scope resolution and normalization
  RUN(cleanup),                 // post parsing transformations
  RUN(scopeResolve),            // resolve symbols by scope
  RUN(flattenClasses),          // denest nested classes

  RUN(normalize),               // normalization transformations
  RUN(checkNormalized),         // check semantics of normalized AST

  RUN(buildDefaultFunctions),   // build default functions
  RUN(createTaskFunctions),     // convert 'begin' et al. to functions

  // Function resolution and shallow type inference
  RUN(resolve),                 // resolves function calls and types
  RUN(resolveIntents),          // resolve argument intents
  RUN(checkResolved),           // checks semantics of resolved AST

  RUN(replaceArrayAccessesWithRefTemps), // replace multiple array access calls with reference temps

  // Post-resolution cleanup
  RUN(flattenFunctions),        // denest nested functions
  RUN(cullOverReferences),      // remove excess references
  RUN(lowerErrorHandling),      // lower error handling constructs
  RUN(callDestructors),
  RUN(lowerIterators),          // lowers iterators into functions/classes
  RUN(parallel),                // parallel transforms
  RUN(prune),                   // prune AST of dead functions and types

  // Optimizations
  RUN(bulkCopyRecords),         // replace simple assignments with PRIM_ASSIGN.
  RUN(removeUnnecessaryAutoCopyCalls),
  RUN(inlineFunctions),         // function inlining
  RUN(scalarReplace),           // scalar replace all tuples
  RUN(refPropagation),          // reference propagation
  RUN(copyPropagation),         // copy propagation
  RUN(deadCodeElimination),     // eliminate dead code
  RUN(removeEmptyRecords),      // remove empty records
  RUN(localizeGlobals),         // pull out global constants from loop runs
  RUN(loopInvariantCodeMotion), // move loop invariant code above loop runs
  RUN(prune2),                  // prune AST of dead functions and types again

  RUN(returnStarTuplesByRefArgs),

  RUN(insertWideReferences),    // inserts wide references for on clauses
  RUN(optimizeOnClauses),       // Optimize on clauses
  RUN(addInitCalls),            // Add module init calls and guards.

  // AST to C or LLVM
  RUN(insertLineNumbers),       // insert line numbers for error messages
  RUN(denormalize),             // denormalize -- remove local temps
  RUN(codegen),                 // generate C or LLVM code
  RUN(makeBinary)               // invoke underlying C or LLVM compiler
};

static const size_t passListSize = sizeof(sPassList) / sizeof(sPassList[0]);

static void runPass(PhaseTracker& tracker, size_t passIndex);

// Set up and validate flags-specified pass to stop after.
static void setupStopAfterPass() {
  // --parse-only conflicts with otherwise specified pass to stop after
  if (fParseOnly) {
    if (stopAfterPass[0]) {
      USR_FATAL("cannot provide both parse-only and stop after pass flags");
    }
    strcpy(stopAfterPass, "checkGeneratedAst");
  }

  // ensure pass to stop after exists
  if (stopAfterPass[0]) {
    bool stopAfterPassValid = false;
    for (size_t i = 0; i < passListSize; i++) {
      if (strcmp(sPassList[i].name, stopAfterPass) == 0) {
        stopAfterPassValid = true;
        break;
      }
    }
    if (!stopAfterPassValid) {
      USR_FATAL("Requested to stop after pass '%s', but no such pass exists",
                stopAfterPass);
    }
  }
}

void runPasses(PhaseTracker& tracker) {

  setupLogfiles();

  if (printPasses == true || printPassesFile != 0) {
    if (fDriverCompilationPhase) {
      Phase::ReportText(
          "Timing for driver compilation phase\n--------------\n");
    } else if (fDriverMakeBinaryPhase) {
      Phase::ReportText(
          "\n\nTiming for driver makeBinary phase\n--------------\n");
    }
    tracker.ReportPass();
  }

  setupStopAfterPass();

  for (size_t i = 0; i < passListSize; i++) {
    // skip until makeBinary if in makeBinary phase invocation
    if (fDriverMakeBinaryPhase && strcmp(sPassList[i].name, "makeBinary") != 0) {
      continue;
    }

    runPass(tracker, i);

    USR_STOP(); // quit if fatal errors were encountered in pass

    currentPassNo++;

    // quit before makeBinary in compilation phase invocation
    if (fDriverCompilationPhase && strcmp(sPassList[i].name, "codegen") == 0) {
      break;
    }

    // Break early if this is a parse-only run
    if (fParseOnly && strcmp(sPassList[i].name, "checkParsed") == 0) {
      break;
    }

    // Breaks early if the user specified to stop after this pass
    if (stopAfterPass[0] != '\0' &&
        strcmp(sPassList[i].name, stopAfterPass) == 0) {
      break;
    }
  }

  destroyAst();
  teardownLogfiles();
}

static void runPass(PhaseTracker& tracker, size_t passIndex) {
  PassInfo* info = &sPassList[passIndex];

  //
  // The primary work for this pass
  //

  tracker.StartPhase(info->name, PhaseTracker::kPrimary);

  if (fPrintStatistics[0] != '\0' && passIndex > 0)
    printStatistics("clean");

  (*(info->passFunction))();

  //
  // Statistics and logging
  //

  if (fPrintStatistics[0] != '\0')
    printStatistics(info->name);

  logWriteLog(info->name, currentPassNo, info->logTag);

  considerExitingEndOfPass();

  //
  // An optional verify pass
  //
  tracker.StartPhase(info->name, PhaseTracker::kVerify);
  (*(info->checkFunction))(); // Run per-pass check function.

  //
  // Clean up the global pointers to AST.
  // Skip if we're on the backend invocation of the compiler, in which case
  // there is no AST.
  //
  if (!fDriverMakeBinaryPhase) {
    tracker.StartPhase(info->name, PhaseTracker::kCleanAst);
    cleanAst();
  }

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

void initPassesForLogging() {
  for (size_t i = 0; i < passListSize; i++) {
    PassInfo* pass = &sPassList[i];

    logMakePassAvailable(pass->name, pass->logTag);
  }
}
