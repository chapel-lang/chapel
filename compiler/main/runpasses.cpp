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

// These entries should be kept in the same order as the entries in passlist.h.
#define LOG_parse                              'p'
#define LOG_checkParsed                        LOG_NEVER
#define LOG_docs                               LOG_NEVER
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
  RUN(parse),                   // parse files and create AST
  RUN(checkParsed),             // checks semantics of parsed AST
  RUN(docs),                    // if fDocs is set, this will generate docs.
                                // if the executable is named "chpldoc" then
                                // the application will stop after this phase

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
  RUN(codegen),                 // generate C code
  RUN(makeBinary)               // invoke underlying C compiler
};

static void runPass(PhaseTracker& tracker, size_t passIndex, bool isChpldoc);

void runPasses(PhaseTracker& tracker, bool isChpldoc) {
  size_t passListSize = sizeof(sPassList) / sizeof(sPassList[0]);

  setupLogfiles();

  if (printPasses == true || printPassesFile != 0) {
    tracker.ReportPass();
  }

  for (size_t i = 0; i < passListSize; i++) {
    runPass(tracker, i, isChpldoc);

    USR_STOP(); // quit if fatal errors were encountered in pass

    currentPassNo++;

    // Break early if this is a parse-only run
    if (fParseOnly ==  true && strcmp(sPassList[i].name, "checkParsed") == 0) {
      break;
    }

    // Breaks early if the user specified to stop after this pass
    if (stopAfterPass[0] != '\0' && strcmp(sPassList[i].name, stopAfterPass) == 0) {
      break;
    }

    // Break early if this is a chpl doc run
    if (isChpldoc == true && strcmp(sPassList[i].name, "docs") == 0) {
      break;
    }
  }

  destroyAst();
  teardownLogfiles();
}

static void runPass(PhaseTracker& tracker, size_t passIndex, bool isChpldoc) {
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
  // Clean up the global pointers to AST.  If we're running chpldoc,
  // there's no real reason to run this step (and at the time of this
  // writing, it didn't work if we hadn't parsed all the 'use'd
  // modules.
  //
  if (!isChpldoc) {
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
  size_t passListSize = sizeof(sPassList) / sizeof(sPassList[0]);

  for (size_t i = 0; i < passListSize; i++) {
    PassInfo* pass = &sPassList[i];

    logMakePassAvailable(pass->name, pass->logTag);
  }
}
