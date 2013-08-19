#include "baseAST.h"
#include "files.h"
#include "misc.h"
#include "log.h"
#include "runpasses.h"
#include "stringutil.h"
#include "view.h"
#include "primitive.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/time.h>

bool printPasses = false;

struct PassInfo {
  void (*pass_fn)(void); // The function which implements the pass.
  void (*check_fn)(void); // per-pass check function
  const char *name;
  char log_tag;
};

#include "passlist.h"


static void runPass(const char *passName, void (*pass)(void), void (*check)(), char log_tag) {
  static struct timeval startTimeBetweenPasses;
  static struct timeval stopTimeBetweenPasses;
  static double timeBetweenPasses = -1.0;
  static double totalTime = 0.0;
  struct timeval startTime;
  struct timeval stopTime;
  struct timezone timezone;

  if (printPasses) {
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
  if (printPasses) {
    fprintf(stderr, "%32s :", passName);
    fflush(stderr);
    gettimeofday(&startTime, &timezone);
  }
  (*pass)();
  if (printPasses) {
    gettimeofday(&stopTime, &timezone);
    fprintf(stderr, "%8.3f seconds",
            ((double)((stopTime.tv_sec*1e6+stopTime.tv_usec) - 
                      (startTime.tv_sec*1e6+startTime.tv_usec))) / 1e6);
    if (developer) fprintf(stderr, "  [%d]", lastNodeIDUsed());
    fprintf(stderr, "\n");
    totalTime += ((double)((stopTime.tv_sec*1e6+stopTime.tv_usec) - 
                           (startTime.tv_sec*1e6+startTime.tv_usec))) / 1e6;
    if (!strcmp(passName, "makeBinary")) {
      fprintf(stderr, "%32s :%8.3f seconds\n", "time between passes",
              timeBetweenPasses);
      fprintf(stderr, "%32s :%8.3f seconds\n", "total time",
              totalTime+timeBetweenPasses);
    }
  }
  if (strlen(fPrintStatistics))
    printStatistics(passName);
  if (fdump_html) {
    html_view(passName);
  }
  if (logging(log_tag))
    dump_ast(passName, currentPassNo);
  if (printPasses) {
    gettimeofday(&startTimeBetweenPasses, &timezone);
  }
  considerExitingEndOfPass();
  cleanAst();
  (*check)(); // Run per-pass check function.
  //printPrimitiveCounts(passName);
}


static void dump_index_header(FILE* f) {
  fprintf(f, "<HTML>\n");
  fprintf(f, "<HEAD>\n");
  fprintf(f, "<TITLE> Compilation Dump </TITLE>\n");
  fprintf(f, "<SCRIPT SRC=\"%s/compiler/etc/www/mktree.js\" LANGUAGE=\"JavaScript\"></SCRIPT>", 
         CHPL_HOME);
  fprintf(f, "<LINK REL=\"stylesheet\" HREF=\"%s/compiler/etc/www/mktree.css\">", 
         CHPL_HOME);
  fprintf(f, "</HEAD>\n");
  fprintf(f, "<div style=\"text-align: center;\"><big><big><span style=\"font-weight: bold;\">");
  fprintf(f, "Compilation Dump<br><br></span></big></big>\n");
  fprintf(f, "<div style=\"text-align: left;\">\n\n");
}


static void dump_index_footer(FILE* f) {
  fprintf(f, "</HTML>\n");
}

static void setupLogfiles() {
  if (logging() || fdump_html || *deletedIdFilename)
    ensureDirExists(log_dir, "ensuring directory for log files exists");

  if (log_dir[strlen(log_dir)-1] != '/') 
    strcat(log_dir, "/");

  if (fdump_html) {
    if (!(html_index_file = fopen(astr(log_dir, "index.html"), "w"))) {
      USR_FATAL("cannot open html index file \"%s\" for writing", astr(log_dir, "index.html"));
    }
    dump_index_header(html_index_file);
    fprintf(html_index_file, "<TABLE CELLPADDING=\"0\" CELLSPACING=\"0\">");
  }
  if (deletedIdFilename[0] != '\0') {
    deletedIdHandle = fopen(deletedIdFilename, "w");
    if (!deletedIdHandle) {
      USR_FATAL("cannot open file to log deleted AST ids\"%s\" for writing", deletedIdFilename);
    }
  }
}

static void teardownLogfiles() {
  if (fdump_html) {
    fprintf(html_index_file, "</TABLE>");
    dump_index_footer(html_index_file);
    fclose(html_index_file);
  }
  if (deletedIdON) {
    fclose(deletedIdHandle);
    deletedIdHandle = NULL;
  }
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
