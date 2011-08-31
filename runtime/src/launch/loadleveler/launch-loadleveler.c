#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "chpllaunch.h"
#include "chpl-mem.h"
#include "chpltypes.h"
#include "error.h"

#define baseLLFilename ".chpl-ll-qsub-"
#define baseExpectFilename ".chpl-expect-"

char llFilename[FILENAME_MAX];
char expectFilename[FILENAME_MAX];

/* copies of binary to run per node */
#define procsPerNode 1  
#define versionBuffLen 80

#define launcherAccountEnvvar "CHPL_LAUNCHER_ACCOUNT"

static char* chpl_launch_create_command(int argc, char* argv[], 
                                        int32_t numLocales) {
  int i;
  int size;
  char baseCommand[256];
  char* command;
  FILE* llFile, *expectFile;
  char* projectString = getenv(launcherAccountEnvvar);
  char* basenamePtr = strrchr(argv[0], '/');
  pid_t mypid;

  if (basenamePtr == NULL) {
      basenamePtr = argv[0];
  } else {
      basenamePtr++;
  }

  chpl_compute_real_binary_name(argv[0]);

#ifndef DEBUG_LAUNCH
  mypid = getpid();
#else
  mypid = 0;
#endif
  sprintf(expectFilename, "%s%d", baseExpectFilename, (int)mypid);
  sprintf(llFilename, "%s%d", baseLLFilename, (int)mypid);

  llFile = fopen(llFilename, "w");
  fprintf(llFile, "# @ wall_clock_limit = 00:10:00\n");
  fprintf(llFile, "# @ job_type = parallel\n");
  fprintf(llFile, "# @ node = %d\n", numLocales);
  fprintf(llFile, "# @ tasks_per_node = 1\n");
  if (projectString && strlen(projectString) > 0)
      fprintf(llFile, "# @ class = %s\n", projectString);
  fprintf(llFile, "# @ output = out.$(jobid)\n");
  fprintf(llFile, "# @ error = err.$(jobid)\n");
  fprintf(llFile, "# @ queue\n");
  fprintf(llFile, "\n");
  fprintf(llFile, "%s", chpl_get_real_binary_name());
  for (i=1; i<argc; i++) {
      fprintf(llFile, " '%s'", argv[i]);
  }
  fprintf(llFile, "\n");
  fclose(llFile);

  sprintf(baseCommand, "llsubmit %s", llFilename);

  size = strlen(baseCommand) + 1;

  command = chpl_mem_allocMany(size, sizeof(char), CHPL_RT_MD_COMMAND_BUFFER, -1, "");
  
  sprintf(command, "%s", baseCommand);

  if (strlen(command)+1 > size) {
    chpl_internal_error("buffer overflow");
  }

  return command;
}

static void chpl_launch_cleanup(void) {
#ifndef DEBUG_LAUNCH
  char command[1024];

  sprintf(command, "rm %s", llFilename);
  system(command);

#endif
}


int chpl_launch(int argc, char* argv[], int32_t numLocales) {
  int retcode =
    chpl_launch_using_system(chpl_launch_create_command(argc, argv, numLocales),
                             argv[0]);
  chpl_launch_cleanup();
  return retcode;
}


int chpl_launch_handle_arg(int argc, char* argv[], int argNum,
                           int32_t lineno, chpl_string filename) {
  return 0;
}


void chpl_launch_print_help(void) {
}
