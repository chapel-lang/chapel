#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "chpllaunch.h"
#include "chpl_mem.h"
#include "chpltypes.h"
#include "error.h"

#define baseLLFilename ".chpl-mnsub-"

char llFilename[FILENAME_MAX];

/* copies of binary to run per node */
#define procsPerNode 1  
#define versionBuffLen 80

#define launcherAccountEnvvar "CHPL_LAUNCHER_CLASS"

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

#define DEBUG_LAUNCH

#ifndef DEBUG_LAUNCH
  mypid = getpid();
#else
  mypid = 0;
#endif
  sprintf(llFilename, "%s%d", baseLLFilename, (int)mypid);

  llFile = fopen(llFilename, "w");
  fprintf(llFile, "#!/bin/bash\n");
  fprintf(llFile, "# @ class = debug\n");
  fprintf(llFile, "# @ job_name = %s\n", basenamePtr);
  fprintf(llFile, "# @ initialdir = .\n");
  fprintf(llFile, "# @ output = %s_%%j.out\n", basenamePtr);
  fprintf(llFile, "# @ error = %s_%%j.out\n", basenamePtr);
  fprintf(llFile, "# @ total_tasks = %d\n", numLocales);
  fprintf(llFile, "# @ cpus_per_task = 4\n");
  fprintf(llFile, "# @ tasks_per_node = 1\n");
  fprintf(llFile, "# @ wall_clock_limit = 00:10:00\n");
  fprintf(llFile, "\n");
  fprintf(llFile, "$CHPL_HOME/third-party/gasnet/install/marenostrum-ibm/seg-everything/nodbg/bin/gasnetrun_mpi -n %d %s", numLocales, chpl_get_real_binary_name());
  for (i=1; i<argc; i++) {
      fprintf(llFile, " '%s'", argv[i]);
  }
  fprintf(llFile, " || echo -n \"\"\n");
  fclose(llFile);

  sprintf(baseCommand, "mnsubmit %s", llFilename);

  size = strlen(baseCommand) + 1;

  command = chpl_malloc(size, sizeof(char), CHPL_RT_MD_COMMAND_BUFFER, -1, "");
  
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


void chpl_launch(int argc, char* argv[], int32_t numLocales) {
  chpl_launch_using_system(chpl_launch_create_command(argc, argv, numLocales),
                           argv[0]);
  chpl_launch_cleanup();
}


int chpl_launch_handle_arg(int argc, char* argv[], int argNum,
                           int32_t lineno, chpl_string filename) {
  return 0;
}
