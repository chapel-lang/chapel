#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chpllaunch.h"
#include "chplmem.h"
#include "error.h"

static const char* pbsFilename = ".chpl-pbs-qsub";
static const char* expectFilename = ".chpl-expect";

#define launcherAccountEnvvar "CHPL_LAUNCHER_ACCOUNT"

char* chpl_launch_create_command(int argc, char* argv[], int32_t numLocales) {
  int i;
  int size;
  char baseCommand[256];
  char* command;
  int procsPerNode = 1;  // BLC -- is this the value we want to use?
  FILE* pbsFile, *expectFile;
  char* projectString = getenv(launcherAccountEnvvar);

  pbsFile = fopen(pbsFilename, "w");
  fprintf(pbsFile, "#!/bin/sh\n\n");
  fprintf(pbsFile, "#PBS -N Chapel-%s\n", argv[0]);
  //  fprintf(pbsFile, "#PBS -l size=%d\n", 4*numLocales);
  fprintf(pbsFile, "#PBS -l mppwidth=%d\n", numLocales);
  fprintf(pbsFile, "#PBS -l mppnppn=%d\n", procsPerNode);
  fprintf(pbsFile, "#PBS -l walltime=30\n");    // BLC -- ditto?
  if (projectString && strlen(projectString) > 0)
    fprintf(pbsFile, "#PBS -A %s\n", projectString);
  fclose(pbsFile);

  expectFile = fopen(expectFilename, "w");
  fprintf(expectFile, "log_user 0\n");
  fprintf(expectFile, "set timeout -1\n");
  fprintf(expectFile, "set prompt \"(%|#|\\$|>) $\"\n");
  fprintf(expectFile, "spawn qsub -z -I %s\n", pbsFilename);
  fprintf(expectFile, "expect {\n");
  fprintf(expectFile, "  \"A project was not specified\" {send_user \"A project account must be specified via \\$" launcherAccountEnvvar "\\n\" ; exit 1}\n");
  fprintf(expectFile, "  -re $prompt\n");
  fprintf(expectFile, "}\n");
  fprintf(expectFile, "send \"cd \\$PBS_O_WORKDIR\\n\"\n");
  fprintf(expectFile, "expect -re $prompt\n");
  fprintf(expectFile, "send \"aprun -q -b -n1 -N1 ls %s_real\\n\"\n", argv[0]);
  fprintf(expectFile, "expect {\n");
  fprintf(expectFile, "  \"failed: chdir\" {send_user \"%s must be stored on/launched from a cross-mounted file system\\n\" ; exit 1}\n", argv[0]);
  fprintf(expectFile, "  -re $prompt\n");
  fprintf(expectFile, "}\n");
  fprintf(expectFile, "send \"aprun -q -n%d -N%d %s_real",
          numLocales, procsPerNode, argv[0]);
  for (i=1; i<argc; i++) {
    fprintf(expectFile, " %s", argv[i]);
  }
  fprintf(expectFile, "\\n\"\n");
  fprintf(expectFile, "interact -o -re $prompt {send \"exit\\n\"}\n");
  fclose(expectFile);

  sprintf(baseCommand, "expect %s", expectFilename);

  size = strlen(baseCommand) + 1;

  command = chpl_malloc(size, sizeof(char*), "command buffer", -1, "");
  
  sprintf(command, "%s", baseCommand);

  if (strlen(command)+1 > size) {
    chpl_internal_error("buffer overflow");
  }

  return command;
}


void chpl_launch_cleanup(void) {
  char command[1024];

  sprintf(command, "rm %s", pbsFilename);
  printf("system: %s\n", command);
  system(command);

  sprintf(command, "rm %s", expectFilename);
  printf("system: %s\n", command);
  system(command);
}
