#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "chpllaunch.h"
#include "chpl_mem.h"
#include "error.h"

#define baseSysFilename ".chpl-sys-"
char sysFilename[FILENAME_MAX];

#define CHPL_CC_ARG "-cc"
static char *_ccArg = NULL;

// TODO: Un-hard-code this stuff:

static int getNumCoresPerLocale(void) {
  FILE* sysFile;
  int coreMask;
  int bitMask = 0x1;
  int numCores;
  pid_t mypid;
  char* numCoresString = getenv("CHPL_LAUNCHER_CORES_PER_LOCALE");
  char* command;

  if (numCoresString) {
    numCores = atoi(numCoresString);
    if (numCores != 0)
      return numCores;
  }

#ifndef DEBUG_LAUNCH
  mypid = getpid();
#else
  mypid = 0;
#endif
  sprintf(sysFilename, "%s%d", baseSysFilename, (int)mypid);
  command = chpl_glom_strings(2, "cnselect -Lcoremask > ", sysFilename);
  system(command);
  sysFile = fopen(sysFilename, "r");
  if (fscanf(sysFile, "%d\n", &coreMask) != 1 || !feof(sysFile)) {
    chpl_error("unable to determine number of cores per locale; please set CHPL_LAUNCHER_CORES_PER_LOCALE", 0, 0);
  }
  coreMask >>= 1;
  numCores = 1;
  while (coreMask & bitMask) {
    coreMask >>= 1;
    numCores += 1;
  }
  fclose(sysFile);
  sprintf(command, "rm %s", sysFilename);
  system(command);
  return numCores;
}

static char* chpl_launch_create_command(int argc, char* argv[], 
                                        int32_t numLocales) {
  int i;
  int size;
  char baseCommand[256];
  char* command;
  const char *host = getenv("CHPL_HOST_PLATFORM");
  const char *ccArg = _ccArg ? _ccArg :
    (host && !strcmp(host, "xe-cle") ? "none" : "cpu");

  chpl_compute_real_binary_name(argv[0]);

  sprintf(baseCommand, "aprun -cc %s %s -d%d -n%d -N1 %s", ccArg,
          ((verbosity < 2) ? "-q" : ""), getNumCoresPerLocale(), numLocales, 
          chpl_get_real_binary_name());

  size = strlen(baseCommand) + 1;

  for (i=1; i<argc; i++) {
    size += strlen(argv[i]) + 3;
  }

  command = chpl_malloc(size, sizeof(char*), CHPL_RT_MD_COMMAND_BUFFER, -1, "");
  
  sprintf(command, "%s", baseCommand);
  for (i=1; i<argc; i++) {
    strcat(command, " '");
    strcat(command, argv[i]);
    strcat(command, "'");
  }

  if (strlen(command)+1 > size) {
    chpl_internal_error("buffer overflow");
  }

  return command;
}


void chpl_launch(int argc, char* argv[], int32_t numLocales) {
  chpl_launch_using_system(chpl_launch_create_command(argc, argv, numLocales),
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
      sprintf(msg, "'%s' is not a valid cpu assignment", _ccArg);
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
