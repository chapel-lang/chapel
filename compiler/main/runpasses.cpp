/*
 * Copyright 2004-2016 Cray Inc.
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

// These entries should be kept in the same order as the entries in passlist.h.
#define LOG_parse                              'p'
#define LOG_checkParsed                        NUL
#define LOG_docs                               NUL
#define LOG_readExternC                        'c'
#define LOG_cleanup                            'u'
#define LOG_scopeResolve                       'S'
#define LOG_flattenClasses                     'b'
#define LOG_normalize                          'N'
#define LOG_checkNormalized                    NUL
#define LOG_buildDefaultFunctions              'D'
#define LOG_createTaskFunctions                't'
#define LOG_expandExternArrayCalls             NUL
#define LOG_resolve                            'R'
#define LOG_resolveIntents                     'i'
#define LOG_checkResolved                      NUL
#define LOG_replaceArrayAccessesWithRefTemps   'T'
#define LOG_processIteratorYields              'y'
#define LOG_flattenFunctions                   'e'
#define LOG_cullOverReferences                 'O'
#define LOG_callDestructors                    'd'
#define LOG_lowerIterators                     'L'
#define LOG_parallel                           'P'
#define LOG_prune                              'X'
#define LOG_bulkCopyRecords                    'B'
#define LOG_removeUnnecessaryAutoCopyCalls     'U'
#define LOG_inlineFunctions                    'I'
#define LOG_scalarReplace                      'r'
#define LOG_refPropagation                     'g'
#define LOG_copyPropagation                    'G'
#define LOG_deadCodeElimination                'x'
#define LOG_removeWrapRecords                  'w'
#define LOG_removeEmptyRecords                 'm'
#define LOG_localizeGlobals                    'l'
#define LOG_loopInvariantCodeMotion            'q'
#define LOG_prune2                             'Y'
#define LOG_returnStarTuplesByRefArgs          's'
#define LOG_insertWideReferences               'W'
#define LOG_optimizeOnClauses                  'o'
#define LOG_addInitCalls                       'M'
#define LOG_insertLineNumbers                  'n'
#define LOG_denormalize                        'Q'
#define LOG_codegen                            'E'
#define LOG_makeBinary                         NUL

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
  RUN(processIteratorYields),   // adjustments to iterators
  RUN(flattenFunctions),        // denest nested functions
  RUN(cullOverReferences),      // remove excess references
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
  RUN(removeWrapRecords),       // remove _array, _domain, and
                                // _distribution records
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

  log_writeLog(info->name, currentPassNo, info->logTag);

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
