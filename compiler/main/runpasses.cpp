#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include "baseAST.h"
#include "files.h"
#include "misc.h"
#include "log.h"
#include "runpasses.h"
#include "stringutil.h"
#include "view.h"

void verify();
void cleanAst();
void destroyAst();
void printStatistics(const char* pass);

bool printPasses = false;

struct PassInfo {
  void (*fn)(void);
  const char *name;
};

#define FIRST {NULL, NULL}
#define LAST {NULL, NULL}
#define RUN(x) {x, #x}
#include "passlist.h"


static void runPass(const char *passName, void (*pass)(void)) {
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
    fprintf(stderr, "%8.3f seconds\n",  
            ((double)((stopTime.tv_sec*1e6+stopTime.tv_usec) - 
                      (startTime.tv_sec*1e6+startTime.tv_usec))) / 1e6);
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
  if (printPasses) {
    gettimeofday(&startTimeBetweenPasses, &timezone);
  }
  cleanAst();
  verify();
}


void dump_index_header(FILE* f) {
  fprintf(f, "<HTML>\n");
  fprintf(f, "<HEAD>\n");
  fprintf(f, "<TITLE> Compilation Dump </TITLE>\n");
  fprintf(f, "<SCRIPT SRC=\"%s/compiler/etc/www/mktree.js\" LANGUAGE=\"JavaScript\"></SCRIPT>", 
         chplhome);
  fprintf(f, "<LINK REL=\"stylesheet\" HREF=\"%s/compiler/etc/www/mktree.css\">", 
         chplhome);
  fprintf(f, "</HEAD>\n");
  fprintf(f, "<div style=\"text-align: center;\"><big><big><span style=\"font-weight: bold;\">");
  fprintf(f, "Compilation Dump<br><br></span></big></big>\n");
  fprintf(f, "<div style=\"text-align: left;\">\n\n");
}


void dump_index_footer(FILE* f) {
  fprintf(f, "</HTML>\n");
}


void runPasses(void) {
  if (fdump_html) {
    html_index_file = fopen(stringcat(log_dir, "index.html"), "w");
    dump_index_header(html_index_file);
    fprintf(html_index_file, "<TABLE CELLPADDING=\"0\" CELLSPACING=\"0\">");
  }
  PassInfo* pass = passlist+1;  // skip over FIRST
  while (pass->name != NULL) {
    runPass(pass->name, pass->fn);
    check_fatal_errors_encountered();
    pass++;
  }
  if (fdump_html) {
    fprintf(html_index_file, "</TABLE>");
    dump_index_footer(html_index_file);
    fclose(html_index_file);
  }
  destroyAst();
}
