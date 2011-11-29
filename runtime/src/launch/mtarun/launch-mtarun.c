#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chpllaunch.h"
#include "chpl-mem.h"
#include "error.h"

static int numTeams = 0;
static int maxTeams = 0;

static char _tbuf[16];
static char _mbuf[16];
static char** chpl_launch_create_argv(int argc, char* argv[]) {
  const int largc = 5;
  char *largv[largc];
  int l = 0;

  largv[l++] = (char *) "mtarun";
  if (numTeams != 0) {
    largv[l++] = (char *) "-t";
    sprintf(_tbuf, "%d", numTeams);
    largv[l++] = _tbuf;
  }
  if (maxTeams != 0) {
    largv[l++] = (char *) "-m";
    sprintf(_mbuf, "%d", maxTeams);
    largv[l++] = _mbuf;
  }

  return chpl_bundle_exec_args(argc, argv, l, largv);
}


int  chpl_launch(int argc, char* argv[], int32_t numLocales) {
  if (numLocales != 1) {
    // This error should be taken care of before we get to this point
    chpl_internal_error("The XMT launcher only supports numLocales==1");
  }

  return chpl_launch_using_exec("mtarun",
                                chpl_launch_create_argv(argc, argv),
                                argv[0]);
}


int chpl_launch_handle_arg(int argc, char* argv[], int argNum,
                           int32_t lineno, chpl_string filename) {
  char *endptr;

  if ((strcmp(argv[argNum], "--numNodes") == 0) ||
      (strcmp(argv[argNum], "-nn") == 0)) {
    numTeams = strtol(argv[argNum+1], &endptr, 10);
    if ((endptr == argv[argNum+1]) || (numTeams <= 0)) {
      chpl_error("Invalid number of teams", 0, "<command-line>");
    }
    maxTeams = numTeams;
    return 2;
  } else if (strcmp(argv[argNum], "--numInitialTeams") == 0) {
    numTeams = strtol(argv[argNum+1], &endptr, 10);
    if ((endptr == argv[argNum+1]) || (numTeams <= 0)) {
      chpl_error("Invalid number of initial teams", 0, "<command-line>");
    }
    return 2;
  } else if (strcmp(argv[argNum], "--maxTeams") == 0) {
    maxTeams = strtol(argv[argNum+1], &endptr, 10);
    if ((endptr == argv[argNum+1]) || (maxTeams <= 0)) {
      chpl_error("Invalid number of max teams", 0, "<command-line>");
    }
    return 2;
  }
  return 0;
}


void chpl_launch_print_help(void) {
  fprintf(stdout, "LAUNCHER FLAGS:\n");
  fprintf(stdout, "===============\n");
  fprintf(stdout, "    --numInitialTeams <n>  : start with n teams\n");
  fprintf(stdout, "    --maxTeams <n>         : use no more than n teams\n");
  fprintf(stdout, "-nn,--numNodes <n>         : start with n teams and use no more\n");
}
