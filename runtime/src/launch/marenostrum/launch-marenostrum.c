/*
 * Copyright 2004-2015 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

#define LAUNCH_PATH_HELP WRAP_TO_STR(LAUNCH_PATH)
#define WRAP_TO_STR(x) TO_STR(x)
#define TO_STR(x) #x

#define baseLLFilename ".chpl-mnsub-"

#define udp 1
#define mpi 2
#define none 3

char llFilename[FILENAME_MAX];

#define launcherAccountEnvvar "CHPL_LAUNCHER_CLASS"

static char* debug = NULL;
static char* walltime = NULL;
static char* queue = NULL;

static char* chpl_launch_create_command(int argc, char* argv[], 
                                        int32_t numLocales) {
  int i;
  int size;
  char baseCommand[256];
  char* command;
  FILE* llFile;
  //  char* projectString = getenv(launcherAccountEnvvar);
  char* basenamePtr = strrchr(argv[0], '/');
  pid_t mypid;

  if (!walltime) {
    chpl_error("You must specify the wall clock time limit of your job using --walltime\n"
               "or CHPL_LAUNCHER_WALLTIME (HH:MM:SS)", 0, NULL);
  }

  if (basenamePtr == NULL) {
      basenamePtr = argv[0];
  } else {
      basenamePtr++;
  }

  chpl_compute_real_binary_name(argv[0]);

  if (debug) {
    mypid = 0;
  } else {
    mypid = getpid();
  }

  sprintf(llFilename, "%s%d", baseLLFilename, (int)mypid);

  llFile = fopen(llFilename, "w");
  fprintf(llFile, "#!/bin/bash\n");
  if (queue) {
    fprintf(llFile, "# @ class = %s\n", queue);
  }
  fprintf(llFile, "# @ job_name = %s\n", basenamePtr);
  fprintf(llFile, "# @ initialdir = .\n");
  fprintf(llFile, "# @ output = %s_%%j.out\n", basenamePtr);
  fprintf(llFile, "# @ error = %s_%%j.out\n", basenamePtr);
  fprintf(llFile, "# @ total_tasks = %d\n", numLocales);
  fprintf(llFile, "# @ cpus_per_task = 4\n");
  fprintf(llFile, "# @ tasks_per_node = 1\n");
  fprintf(llFile, "# @ wall_clock_limit = %s\n", walltime);
  fprintf(llFile, "\n");
#if CHPL_COMM_SUBSTRATE == udp
  fprintf(llFile, "MLIST=$(/opt/perf/bin/sl_get_machine_list -j=\\$SLURM_JOB_ID )\n");
  fprintf(llFile, "\n");
  fprintf(llFile, "export -n SSH_SERVERS\n");
  fprintf(llFile, "for i in $MLIST ; do\n");
  fprintf(llFile, "  export SSH_SERVERS=\"$SSH_SERVERS $i\" ;\n");
  fprintf(llFile, "done\n");
  if (debug) {
    fprintf(llFile, "echo $SSH_SERVERS\n");
  }
  fprintf(llFile, "\n");

  fprintf(llFile, "%samudprun ", WRAP_TO_STR(LAUNCH_PATH));
#elif CHPL_COMM_SUBSTRATE == mpi
  fprintf(llFile, "export MPIRUN_CMD='srun --kill-on-bad-exit %%C'\n");
  fprintf(llFile, "export MPIRUN_CMD_OK=true\n");
  fprintf(llFile, "\n");
  fprintf(llFile, "%sgasnetrun_mpi ", WRAP_TO_STR(LAUNCH_PATH));
#elif CHPL_COMM_SUBSTRATE == none
#else
#error "Unknown CHPL_COMM_SUBSTRATE"
#endif
#if CHPL_COMM_SUBSTRATE != none
  fprintf(llFile, "-n %d ", numLocales);
#endif
  fprintf(llFile, "%s", chpl_get_real_binary_name());
  for (i=1; i<argc; i++) {
    fprintf(llFile, " '%s'", argv[i]);
  }
  fprintf(llFile, " || echo -n \"\"\n");
  fclose(llFile);

  sprintf(baseCommand, "mnsubmit %s", llFilename);

  size = strlen(baseCommand) + 1;

  command = chpl_mem_allocMany(size, sizeof(char), CHPL_RT_MD_COMMAND_BUFFER, -1, "");
  
  sprintf(command, "%s", baseCommand);

  if (strlen(command)+1 > size) {
    chpl_internal_error("buffer overflow");
  }

  return command;
}


static void chpl_launch_cleanup(void) {
  if (!debug) {
    char command[1024];

    sprintf(command, "rm %s", llFilename);
    system(command);
  }
}


int chpl_launch(int argc, char* argv[], int32_t numLocales) {
  int retcode;
  debug = getenv("CHPL_LAUNCHER_DEBUG");
  if (!walltime) {
    walltime = getenv("CHPL_LAUNCHER_WALLTIME");
  }
  if (!queue) {
    queue = getenv("CHPL_LAUNCHER_QUEUE");
  }
  retcode = chpl_launch_using_system(chpl_launch_create_command(argc, argv, numLocales),
                                     argv[0]);
  chpl_launch_cleanup();
  return retcode;
}


#define CHPL_WALLTIME_FLAG "--walltime"
#define CHPL_QUEUE_FLAG "--queue"

int chpl_launch_handle_arg(int argc, char* argv[], int argNum,
                           int32_t lineno, c_string filename) {
  if (!strcmp(argv[argNum], CHPL_WALLTIME_FLAG)) {
    walltime = argv[argNum+1];
    return 2;
  } else if (!strncmp(argv[argNum], CHPL_WALLTIME_FLAG"=", strlen(CHPL_WALLTIME_FLAG))) {
    walltime = &(argv[argNum][strlen(CHPL_WALLTIME_FLAG)+1]);
    return 1;
  }
  if (!strcmp(argv[argNum], CHPL_QUEUE_FLAG)) {
    queue = argv[argNum+1];
    return 2;
  } else if (!strncmp(argv[argNum], CHPL_QUEUE_FLAG"=", strlen(CHPL_QUEUE_FLAG))) {
    queue = &(argv[argNum][strlen(CHPL_QUEUE_FLAG)+1]);
    return 1;
  }
  return 0;
}


void chpl_launch_print_help(void) {
  fprintf(stdout, "LAUNCHER FLAGS:\n");
  fprintf(stdout, "===============\n");
  fprintf(stdout, "--queue <queue>        : specify a queue (e.g., debug, interact)\n");
  fprintf(stdout, "                         (or use $CHPL_LAUNCHER_QUEUE)\n");
  fprintf(stdout, "--walltime <HH:MM:SS>  : specify a wallclock time limit\n");
  fprintf(stdout, "                         (or use $CHPL_LAUNCHER_WALLTIME)\n");
}
