#include "runpasses.h"

#include "baseAST.h"
#include "log.h"
#include "misc.h"
#include "primitive.h"
#include "stringutil.h"
#include "view.h"

#include "AstDump.h"
#include "AstDumpToHtml.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/time.h>

bool  printPasses     = false;
FILE* printPassesFile = NULL;

struct PassInfo {
  void (*pass_fn)(void); // The function which implements the pass.
  void (*check_fn)(void); // per-pass check function
  const char *name;
  char log_tag;
};

#include "passlist.h"

// wrapper for printing timing info to stderr and/or a file
static void printPassTiming(char const *fmt, ...) {
  va_list ap;
  
  if(printPasses) {
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

static void runPass(const char *passName, void (*pass)(void), void (*check)(), char log_tag) {
  static struct timeval startTimeBetweenPasses;
  static struct timeval stopTimeBetweenPasses;
  static double timeBetweenPasses = -1.0;
  static double totalTime = 0.0;
  struct timeval startTime;
  struct timeval stopTime;
  struct timezone timezone;
  static bool performTiming = printPasses || (printPassesFile != NULL);

  if (performTiming) {
    gettimeofday(&stopTimeBetweenPasses, &timezone);
    if (timeBetweenPasses < 0.0)
      timeBetweenPasses = 0.0;
    else
      timeBetweenPasses += 
        ((double)((stopTimeBetweenPasses.tv_sec*1e6+
                   stopTimeBetweenPasses.tv_usec) - 
                  (startTimeBetweenPasses.tv_sec*1e6+
                   startTimeBetweenPasses.tv_usec))) / 1e6;
  }
  if (strlen(fPrintStatistics) && strcmp(passName, "parse"))
    printStatistics("clean");
  if (performTiming) {
    printPassTiming("%32s :", passName);
    gettimeofday(&startTime, &timezone);
  }
  (*pass)();
  if (performTiming) {
    gettimeofday(&stopTime, &timezone);
    printPassTiming("%8.3f seconds",
            ((double)((stopTime.tv_sec*1e6+stopTime.tv_usec) - 
                      (startTime.tv_sec*1e6+startTime.tv_usec))) / 1e6);
    if (developer && printPasses) fprintf(stderr, "  [%d]", lastNodeIDUsed());
    printPassTiming("\n");
    totalTime += ((double)((stopTime.tv_sec*1e6+stopTime.tv_usec) - 
                           (startTime.tv_sec*1e6+startTime.tv_usec))) / 1e6;
    if (!strcmp(passName, "makeBinary")) {
      printPassTiming("%32s :%8.3f seconds\n", "time between passes",
              timeBetweenPasses);
      printPassTiming("%32s :%8.3f seconds\n", "total time",
              totalTime+timeBetweenPasses);
    }
  }

  if (strlen(fPrintStatistics))
    printStatistics(passName);

  log_writeLog(passName, currentPassNo, log_tag);

  if (performTiming) {
    gettimeofday(&startTimeBetweenPasses, &timezone);
  }

  considerExitingEndOfPass();
  cleanAst();
  (*check)(); // Run per-pass check function.
  //printPrimitiveCounts(passName);
}


static void advanceCurrentPass(const char* passName) {
  currentPassNo++;
  currentPassName = passName;
}

void runPasses(void) {
  setupLogfiles();
  PassInfo* pass = passlist+1;  // skip over FIRST
  bool chpldoc = strcmp(chplBinaryName, "chpldoc") == 0;
  if (chpldoc) 
    fDocs = true;
  while (pass->name != NULL) {
    advanceCurrentPass(pass->name);
    runPass(pass->name, pass->pass_fn, pass->check_fn, pass->log_tag);
    USR_STOP(); // quit if fatal errors were encountered in pass
    if (chpldoc && (strcmp(pass->name, "docs") == 0)) {
      break;
    }
    pass++;
  }
  advanceCurrentPass("finishing up");
  destroyAst();
  teardownLogfiles();
}

// Suck the pass flags out of the pass list, so that args to the logging command
// can be checked for validity.
// The pass letters are used by the driver to arrange for dumping AST logs
// of selected passes.
// This routine also verifies that each non-NUL flag is unique.
void initLogFlags(Vec<char>& valid_log_flags) {
  PassInfo* pass = passlist+1;  // skip over FIRST
  while (pass->name != NULL) {
    char tag = pass->log_tag;
    if (tag != NUL) {
      INT_ASSERT(!valid_log_flags.set_in(tag));
      valid_log_flags.set_add(tag);
    }
    pass++;
  }
}
