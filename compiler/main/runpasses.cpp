#include "runpasses.h"

#include "checks.h"      // For check function prototypes.
#include "log.h"         // For LOG_<passname> #defines.
#include "passes.h"      // For pass function prototypes.

#include <cstdio>
#include <sys/time.h>

bool        printPasses     = false;
FILE*       printPassesFile = NULL;

int         currentPassNo   = 0;
const char* currentPassName = 0;

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
  RUN(parse),                            // parse files and create AST
  RUN(checkParsed),                      // checks semantics of parsed AST

  // Read in runtime and included C header file types/prototypes
  RUN(readExternC),

  // create wrapper functions to allow arrays to be passed to extern routines
  RUN(expandExternArrayCalls),

  // Scope resolution and normalization
  RUN(cleanup),                          // post parsing transformations
  RUN(scopeResolve),                     // resolve symbols by scope
  RUN(flattenClasses),                   // denest nested classes
  RUN(docs),                             // generates documentation instead of executable
                                         // if chpldoc is used instead of chpl, otherwise
                                         // generates both documentation and an executable

  RUN(normalize),                        // normalization transformations
  RUN(checkNormalized),                  // check semantics of normalized AST

  RUN(buildDefaultFunctions),            // build default functions
  RUN(createTaskFunctions),              // convert 'begin' et al. to functions

  // Function resolution and shallow type inference
  RUN(resolve),                          // resolves function calls and types
  RUN(resolveIntents),                   // resolve argument intents
  RUN(checkResolved),                    // checks semantics of resolved AST

  // Post-resolution cleanup
  RUN(processIteratorYields),            // adjustments to iterators
  RUN(flattenFunctions),                 // denest nested functions
  RUN(cullOverReferences),               // remove excess references
  RUN(callDestructors),
  RUN(lowerIterators),                   // lowers iterators into functions/classes
  RUN(parallel),                         // parallel transforms
  RUN(prune),                            // prune AST of dead functions and types

  // Optimizations
  RUN(complex2record),                   // change complex numbers into records
  RUN(bulkCopyRecords),                  // replace simple assignments with PRIM_ASSIGN.
  RUN(removeUnnecessaryAutoCopyCalls),
  RUN(inlineFunctions),                  // function inlining
  RUN(scalarReplace),                    // scalar replace all tuples
  RUN(refPropagation),                   // reference propagation
  RUN(copyPropagation),                  // copy propagation
  RUN(deadCodeElimination),              // eliminate dead code
  RUN(removeWrapRecords),                // remove _array, _domain, and _distribution records
  RUN(removeEmptyRecords),               // remove empty records
  RUN(localizeGlobals),                  // pull out global constants from loop runs
  RUN(prune2),                           // prune AST of dead functions and types again

  RUN(returnStarTuplesByRefArgs),

  RUN(insertWideReferences),             // inserts wide references for on clauses
  RUN(optimizeOnClauses),                // Optimize on clauses
  RUN(addInitCalls),                     // Add module initialization calls and guards.  
  RUN(loopInvariantCodeMotion),          // move loop invarient code above loop runs

  // AST to C or LLVM
  RUN(insertLineNumbers),                // insert line numbers for error messages
  RUN(codegen),                          // generate C code
  RUN(makeBinary)                        // invoke underlying C compiler
};

static void advanceCurrentPass(const char* passName);
static void runPass(size_t passIndex);
static void printPassTiming(char const* fmt, ...);

static struct timeval startTimeBetweenPasses;
static struct timeval stopTimeBetweenPasses;
static double         timeBetweenPasses =  0.0;
static double         totalTime         =  0.0;

void runPasses() {
  bool   chpldoc      = (strcmp(chplBinaryName, "chpldoc") == 0) ? true : false;
  size_t passListSize = sizeof(sPassList) / sizeof(sPassList[0]);

  setupLogfiles();

  if (chpldoc) 
    fDocs = true;

  for (size_t i = 0; i < passListSize; i++) {
    advanceCurrentPass(sPassList[i].name);

    runPass(i);

    USR_STOP(); // quit if fatal errors were encountered in pass

    // Break early if this is a chpl doc run
    if (chpldoc == true && strcmp(sPassList[i].name, "docs") == 0) {
      break;
    }
  }

  printPassTiming("%32s :%8.3f seconds\n", 
                  "time between passes",
                  timeBetweenPasses);

  printPassTiming("%32s :%8.3f seconds\n",
                  "total time",
                  totalTime + timeBetweenPasses);

  advanceCurrentPass("finishing up");

  destroyAst();
  teardownLogfiles();
}

static void advanceCurrentPass(const char* passName) {
  currentPassNo++;
  currentPassName = passName;
}

static void runPass(size_t passIndex) {
  PassInfo* info          = &sPassList[passIndex];
  bool      performTiming = printPasses == true || printPassesFile != NULL;

  struct timeval startTime;
  struct timeval stopTime;
  double         passTime = 0.0;

  gettimeofday(&stopTimeBetweenPasses, 0);

  if (passIndex > 0)
    timeBetweenPasses += 
      ((stopTimeBetweenPasses.tv_sec  * 1e6 + stopTimeBetweenPasses.tv_usec ) - 
       (startTimeBetweenPasses.tv_sec * 1e6 + startTimeBetweenPasses.tv_usec)) / 1e6;

  if (fPrintStatistics[0] != '\0' && passIndex > 0)
    printStatistics("clean");


  // Determine time taken to run this pass of the compiler
  gettimeofday(&startTime, 0);

  (*(info->passFunction))();

  gettimeofday(&stopTime, 0);



  if (fPrintStatistics[0] != '\0')
    printStatistics(info->name);

  log_writeLog(info->name, currentPassNo, info->logTag);


  // Start a timer that will complete at the entry to the next pass
  gettimeofday(&startTimeBetweenPasses, 0);

  considerExitingEndOfPass();
  cleanAst();

  (*(info->checkFunction))(); // Run per-pass check function.


  // Do the accounting/printing for this pass
  passTime   = ((stopTime.tv_sec  * 1e6 + stopTime.tv_usec ) - 
                (startTime.tv_sec * 1e6 + startTime.tv_usec)) / 1e6;

  totalTime += passTime;

  if (performTiming) {
    printPassTiming("%32s :%8.3f seconds", info->name, passTime);

    if (developer && printPasses)
      fprintf(stderr, "  [%d]", lastNodeIDUsed());

    printPassTiming("\n");
  }
}

// wrapper for printing timing info to stderr and/or a file
static void printPassTiming(char const* fmt, ...) {
  va_list ap;
  
  if (printPasses) {
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
  }

  if (printPassesFile != NULL) {
    va_start(ap, fmt);
    vfprintf(printPassesFile, fmt, ap);
    va_end(ap);
  }
}



//
// The logging machinery wants to know a "name" for every pass that it can match
// to command line arguments but does not, currently, want to know about the pass
// list itself.  This function provides a vector of the pass list names
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
