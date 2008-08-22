#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // used only for sleep
#include "chpllaunch.h"
#include "chplmem.h"
#include "error.h"

static char* consoleFilename;
static const char* pbsFilename = ".chpl-pbs-qsub";

char* chpl_launch_create_command(int argc, char* argv[], int32_t numLocales) {
  int i;
  int size;
  char baseCommand[256];
  char* command;
  int procsPerNode = 1;  // BLC -- is this the value we want to use?
  FILE* pbsFile;

  consoleFilename = (char*)malloc(strlen(argv[0]) + strlen(".console") + 1);
  sprintf(consoleFilename, "%s.console", argv[0]);

  pbsFile = fopen(pbsFilename, "w");
  fprintf(pbsFile, "#!/bin/sh\n\n");
  fprintf(pbsFile, "#PBS -N Chapel-%s\n", argv[0]);
  fprintf(pbsFile, "#PBS -l mppwidth=%d\n", numLocales);
  fprintf(pbsFile, "#PBS -l mppnppn=%d\n", procsPerNode);
  fprintf(pbsFile, "#PBS -l walltime=30\n");    // BLC -- ditto?
  fprintf(pbsFile, "#PBS -joe -o %s\n", consoleFilename); // BLC -- ditto?
  fprintf(pbsFile, "#PBS -W block=true\n");
  fprintf(pbsFile, "\n");
  fprintf(pbsFile, "cd $PBS_O_WORKDIR\n");
  fprintf(pbsFile, "\n");
  fprintf(pbsFile, "aprun -n%d -N%d ./%s_real", numLocales, procsPerNode, argv[0]);
  for (i=1; i<argc; i++) {
    fprintf(pbsFile, " %s", argv[i]);
  }
  fprintf(pbsFile, "\n");
  fclose(pbsFile);

  sprintf(baseCommand, "qsub %s > /dev/null", pbsFilename);

  size = strlen(baseCommand) + 1;

  command = chpl_malloc(size, sizeof(char*), "zebra command buffer", -1, "");
  
  sprintf(command, "%s", baseCommand);

  if (strlen(command)+1 > size) {
    chpl_internal_error("buffer overflow");
  }

  return command;
}


void chpl_launch_cleanup(void) {
  char command[1024];
  /* This is hacky, but seems necessary in order to ensure the console
     file exists by the time we return */
  sleep(1);
  sprintf(command, "cat %s", consoleFilename);
  printf("system: %s\n", command);
  system(command);

  sprintf(command, "rm %s", consoleFilename);
  printf("system: %s\n", command);
  system(command);

  sprintf(command, "rm %s", pbsFilename);
  printf("system: %s\n", command);
  system(command);
}
