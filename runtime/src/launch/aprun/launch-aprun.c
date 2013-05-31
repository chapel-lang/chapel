#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include "chpllaunch.h"
#include "chpl-mem.h"
#include "error.h"

// FIX ME WHEN WE SORT OUT THIS BATCH SCHEDULER + LAUNCHER STUFF
// THIS SHOULD GO INTO SOME HEADER FILE
typedef enum {
  aprun_cc,   // binding policy
  aprun_n,    // cores per locale
  aprun_d,    // num locales
  aprun_N,    // locales per node
  aprun_j,    // cpus per node (newer versions of aprun)
  aprun_none
} aprun_arg_t;

void initAprunAttributes(void);
const char* getCoresPerLocaleStr(void);
int getCoresPerLocale(void);
const char* getLocalesPerNodeStr(void);
int getLocalesPerNode(void);
const char* getCPUsPerCUStr(void);
int getCPUsPerCU(void);
const char* getNumLocalesStr(void);
const char* getAprunArgStr(aprun_arg_t arg); // possibly inline
int getAprunArg(aprun_arg_t argt);           // possibly inline
//
// FIX ME ABOVE
//

// #define baseSysFilename ".chpl-sys-"
// char sysFilename[FILENAME_MAX];

#define CHPL_CC_ARG "-cc"
static char *_ccArg = NULL;
static char* debug = NULL;

static char _nbuf[16];
static char _dbuf[16];
static char _Nbuf[16];
static char _jbuf[16];
static char** chpl_launch_create_argv(int argc, char* argv[],
                                      int32_t numLocales) {
  int largc = 8;
  char *largv[largc];
  const char *ccArg = _ccArg ? _ccArg : "none";
  int CPUsPerCU;

  initAprunAttributes();

  largv[0] = (char *) "aprun";
  largv[1] = (char *) "-q";
  largv[2] = (char *) getAprunArgStr(aprun_cc);
  largv[3] = (char *) ccArg;
  sprintf(_dbuf, "%s%d", getCoresPerLocaleStr(), getCoresPerLocale());
  largv[4] = _dbuf;
  sprintf(_nbuf, "%s%d", getNumLocalesStr(), numLocales);
  largv[5] = _nbuf;
  sprintf(_Nbuf, "%s%d", getLocalesPerNodeStr(), getLocalesPerNode());
  largv[6] = _Nbuf;
  if ((CPUsPerCU = getCPUsPerCU()) >= 0) {
    sprintf(_jbuf, "%s%d", getCPUsPerCUStr(), getCPUsPerCU());
    largv[7] = _jbuf;
  } else {
    largc--;
  }

  return chpl_bundle_exec_args(argc, argv, largc, largv);
}

int chpl_launch(int argc, char* argv[], int32_t numLocales) {
  debug = getenv("CHPL_LAUNCHER_DEBUG");
  return chpl_launch_using_exec("aprun",
                                chpl_launch_create_argv(argc, argv, numLocales),
                                argv[0]);
}


int chpl_launch_handle_arg(int argc, char* argv[], int argNum,
                           int32_t lineno, chpl_string filename) {
  int numArgs = 0;
  if (!strcmp(argv[argNum], CHPL_CC_ARG)) {
    _ccArg = argv[argNum+1];
    numArgs = 2;
  } else if (!strncmp(argv[argNum], CHPL_CC_ARG"=", strlen(CHPL_CC_ARG))) {
    _ccArg = &(argv[argNum][strlen(CHPL_CC_ARG)+1]);
    numArgs = 1;
  }
  if (numArgs > 0) {
    if (strcmp(_ccArg, "none") &&
        strcmp(_ccArg, "numa_node") &&
        strcmp(_ccArg, "cpu")) {
      char msg[256];
      snprintf(msg, 256, "'%s' is not a valid cpu assignment", _ccArg);
      chpl_error(msg, 0, 0);
    }
  }
  return numArgs;
}


void chpl_launch_print_help(void) {
  fprintf(stdout, "LAUNCHER FLAGS:\n");
  fprintf(stdout, "===============\n");
  fprintf(stdout, "  %s keyword     : specify cpu assignment within a node: none (default), numa_node, cpu\n", CHPL_CC_ARG);
}
