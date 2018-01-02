/*
 * Copyright 2004-2018 Cray Inc.
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
#include <sys/stat.h>
#include <unistd.h>
#include "chpllaunch.h"
#include "chpl-mem.h"
#include "chpltypes.h"
#include "error.h"

#define LAUNCH_PATH_HELP WRAP_TO_STR(LAUNCH_PATH)
#define WRAP_TO_STR(x) TO_STR(x)
#define TO_STR(x) #x

#define baseSBATCHFilename ".chpl-slurm-sbatch-"
#define baseExpectFilename ".chpl-expect-"
#define baseSysFilename ".chpl-sys-"

#define CHPL_WALLTIME_FLAG "--walltime"
#define CHPL_PARTITION_FLAG "--partition"
#define CHPL_EXCLUDE_FLAG "--exclude"

static char* debug = NULL;
static char* walltime = NULL;
static char* partition = NULL;
static char* exclude = NULL;
char slurmFilename[FILENAME_MAX];
char expectFilename[FILENAME_MAX];
char sysFilename[FILENAME_MAX];

/* copies of binary to run per node */
#define procsPerNode 1  

#define launcherAccountEnvvar "CHPL_LAUNCHER_ACCOUNT"

typedef enum {
  slurmpro,
  uma,
  slurm,
  unknown
} sbatchVersion;


// Check what version of slurm is on the system
static sbatchVersion determineSlurmVersion(void) {
  const int buflen = 256;
  char version[buflen];
  char *argv[3];
  argv[0] = (char *) "sbatch";
  argv[1] = (char *) "--version";
  argv[2] = NULL;

  memset(version, 0, buflen);
  if (chpl_run_utility1K("sbatch", argv, version, buflen) <= 0) {
    chpl_error("Error trying to determine slurm version", 0, 0);
  }

  if (strstr(version, "SBATCHPro")) {
    return slurmpro;
  } else if (strstr(version, "wrapper sbatch SBATCH UMA 1.0")) {
    return uma;
  } else if (strstr(version, "slurm")) {
    return slurm;
  } else {
    return unknown;
  }
}

static int getNumCoresPerLocale(void) {
  int numCores = 0;
  char* numCoresString = getenv("CHPL_LAUNCHER_CORES_PER_LOCALE");

  if (numCoresString) {
    numCores = atoi(numCoresString);
    if (numCores != 0)
      return numCores;
  }

  return numCores;
}

static void genNumLocalesOptions(FILE* slurmFile, sbatchVersion sbatch, 
                                 int32_t numLocales,
                                 int32_t numCoresPerLocale) {
  //char* queue = getenv("CHPL_LAUNCHER_QUEUE");
  char* constraint = getenv("CHPL_LAUNCHER_CONSTRAINT");

  // command line walltime takes precedence over env var
  if (!walltime) {
    walltime = getenv("CHPL_LAUNCHER_WALLTIME");
  }

  // command line partition takes precedence over env var
  if (!partition) {
    partition = getenv("CHPL_LAUNCHER_PARTITION");
  }

  // command line exclude list takes precedence over env var
  if (!exclude) {
    exclude = getenv("CHPL_LAUNCHER_EXCLUDE");
  }

  /*
  if (queue)
    fprintf(slurmFile, "#SBATCH -q %s\n", queue);
    */
  if (walltime) 
    fprintf(slurmFile, "#SBATCH --time=%s\n", walltime);
  if (partition)
    fprintf(slurmFile, "#SBATCH --partition=%s\n", partition);
  if (exclude)
    fprintf(slurmFile, "#SBATCH --exclude=%s\n", exclude);
  switch (sbatch) {
/* Only slurm has been tested
  case slurmpro:
  case unknown:
    fprintf(slurmFile, "#SBATCH --nodes=%d\n", numLocales);
//    fprintf(slurmFile, "#SBATCH --ntasks-per-node=%d\n", procsPerNode);
    if (numCoresPerLocale)
      fprintf(slurmFile, "#SBATCH --ntasks-per-node=%d\n", numCoresPerLocale);

      break;
      */
  case slurm:
    fprintf(slurmFile, "#SBATCH --nodes=%d\n", numLocales);
    fprintf(slurmFile, "#SBATCH --ntasks-per-node=1\n");
    // If needed a constraint can be specified with the env var CHPL_LAUNCHER_CONSTRAINT
    if (constraint) {
      fprintf(slurmFile, "#SBATCH --constraint=%s\n", constraint);
    }
    fprintf(slurmFile, "#SBATCH --exclusive\n");

    break;
  default:
    break;
  }
}

static char* chpl_launch_create_command(int argc, char* argv[], 
                                        int32_t numLocales) {
  int i;
  int size;
  char baseCommand[256];
  char* command;
  FILE* slurmFile, *expectFile;
  char* projectString = getenv(launcherAccountEnvvar);
  char* constraint = getenv("CHPL_LAUNCHER_CONSTRAINT");
  char* outputfn = getenv("CHPL_LAUNCHER_SLURM_OUTPUT_FILENAME");
  char* basenamePtr = strrchr(argv[0], '/');
  pid_t mypid;

  if (basenamePtr == NULL) {
      basenamePtr = argv[0];
  } else {
      basenamePtr++;
  }
  chpl_compute_real_binary_name(argv[0]);

  // command line walltime takes precedence over env var
  if (!walltime) {
    walltime = getenv("CHPL_LAUNCHER_WALLTIME");
  }

  // command line partition takes precedence over env var
  if (!partition) {
    partition = getenv("CHPL_LAUNCHER_PARTITION");
  }

  // command line exclude list takes precedence over env var
  if (!exclude) {
    exclude = getenv("CHPL_LAUNCHER_EXCLUDE");
  }

  if (debug) {
    mypid = 0;
  } else {
    mypid = getpid();
  }
  sprintf(sysFilename, "%s%d", baseSysFilename, (int)mypid);
  sprintf(expectFilename, "%s%d", baseExpectFilename, (int)mypid);
  sprintf(slurmFilename, "%s%d", baseSBATCHFilename, (int)mypid);

  if (getenv("CHPL_LAUNCHER_USE_SBATCH") != NULL) {
    slurmFile = fopen(slurmFilename, "w");
    fprintf(slurmFile, "#!/bin/sh\n\n");
    fprintf(slurmFile, "#SBATCH -J Chpl-%.10s\n", basenamePtr);
    genNumLocalesOptions(slurmFile, determineSlurmVersion(), numLocales, getNumCoresPerLocale());
    if (projectString && strlen(projectString) > 0)
      fprintf(slurmFile, "#SBATCH -A %s\n", projectString);
    if (getenv("CHPL_LAUNCHER_USE_SBATCH") != NULL) {
//    fprintf(slurmFile, "#SBATCH -joe\n");  
    if (outputfn!=NULL) 
      fprintf(slurmFile, "#SBATCH -o %s\n", outputfn);
    else
      fprintf(slurmFile, "#SBATCH -o %s.%%j.out\n", argv[0]);
//    fprintf(slurmFile, "cd $SBATCH_O_WORKDIR\n");
      fprintf(slurmFile, "%s/gasnetrun_ibv -n %d %s ",
              WRAP_TO_STR(LAUNCH_PATH), numLocales, chpl_get_real_binary_name());
      for (i=1; i<argc; i++) {
        fprintf(slurmFile, " '%s'", argv[i]);
      }
      fprintf(slurmFile, "\n");
    }
  fclose(slurmFile);
  chmod( slurmFilename, 0755);
  }
  if (getenv("CHPL_LAUNCHER_USE_SBATCH") == NULL) {
  expectFile = fopen(expectFilename, "w");
  if (verbosity < 2) {
//    fprintf(expectFile, "log_user 0\n");
  }
  fprintf(expectFile, "set timeout -1\n");
//  fprintf(expectFile, "chmod +x %s\n",slurmFilename);
  fprintf(expectFile, "set prompt \"(%%|#|\\\\$|>) $\"\n");

//  fprintf(expectFile, "spawn sbatch ");
  fprintf(expectFile, "spawn -noecho salloc ");
  fprintf(expectFile, "-J %.10s ",basenamePtr); // pass 
  fprintf(expectFile, "-N %d ",numLocales); 
  fprintf(expectFile, "--ntasks-per-node=1 ");
  fprintf(expectFile, "--exclusive "); //  give exclusive access to the nodes
  fprintf(expectFile, "--time=%s ",walltime); 
  if(partition)
    fprintf(expectFile, "--partition=%s ",partition);
  if(exclude)
    fprintf(expectFile, "--exclude=%s ",exclude);
  if (constraint) {
    fprintf(expectFile, " -C %s", constraint);
  }
//  fprintf(expectFile, "-I %s ", slurmFilename);
  fprintf(expectFile, " %s/gasnetrun_ibv -n %d %s ", 
          WRAP_TO_STR(LAUNCH_PATH), numLocales, chpl_get_real_binary_name());
  for (i=1; i<argc; i++) {
    fprintf(expectFile, " %s", argv[i]);
  }
//  fprintf(expectFile, "\\n\"\n");
  fprintf(expectFile, "\n\n");
//  fprintf(expectFile, "expect -re $prompt\n");
//  fprintf(expectFile, "send \"cd \\$SBATCH_O_WORKDIR\\n\"\n");
//  fprintf(expectFile, "expect -re $prompt\n");
//  fprintf(expectFile, "sleep 10\n");
//  fprintf(expectFile, "interact -o -re $prompt {return}\n");
//  fprintf(expectFile, "send_user \"\\n\"\n");
//  fprintf(expectFile, "send \"exit\\n\"\n");
  fprintf(expectFile, "interact -o -re $prompt {return}\n");
  fclose(expectFile);
  sprintf(baseCommand, "expect %s", expectFilename);
  } else {
//    sprintf(baseCommand, "sbatch %s\n", slurmFilename);
    sprintf(baseCommand, "sbatch %s\n", slurmFilename);
  }

  size = strlen(baseCommand) + 1;

  command = chpl_mem_allocMany(size, sizeof(char), CHPL_RT_MD_COMMAND_BUFFER, -1, 0);
  
  sprintf(command, "%s", baseCommand);

  if (strlen(command)+1 > size) {
    chpl_internal_error("buffer overflow");
  }

  return command;
}

static void chpl_launch_cleanup(void) {
  if (!debug) {
    char command[1024];
    if (getenv("CHPL_LAUNCHER_USE_SBATCH") == NULL) {
      sprintf(command, "rm %s", expectFilename);
      system(command);
    } else {
      sprintf(command, "rm %s", slurmFilename);
      system(command);
      sprintf(command, "rm %s", sysFilename);
      system(command);
    }
  }
}


int chpl_launch(int argc, char* argv[], int32_t numLocales) {
  int retcode;

  debug = getenv("CHPL_LAUNCHER_DEBUG");

  retcode = chpl_launch_using_system(chpl_launch_create_command(argc, argv, numLocales),
            argv[0]);
  chpl_launch_cleanup();
  return retcode;
}


int chpl_launch_handle_arg(int argc, char* argv[], int argNum,
                           int32_t lineno, int32_t filename) {

  // handle --walltime <walltime> or --walltime=<walltime>
  if (!strcmp(argv[argNum], CHPL_WALLTIME_FLAG)) {
    walltime = argv[argNum+1];
    return 2;
  } else if (!strncmp(argv[argNum], CHPL_WALLTIME_FLAG"=", strlen(CHPL_WALLTIME_FLAG))) {
    walltime = &(argv[argNum][strlen(CHPL_WALLTIME_FLAG)+1]);
    return 1;
  }

  // handle --partition <partition> or --partition=<partition>
  if (!strcmp(argv[argNum], CHPL_PARTITION_FLAG)) {
    partition = argv[argNum+1];
    return 2;
  } else if (!strncmp(argv[argNum], CHPL_PARTITION_FLAG"=", strlen(CHPL_PARTITION_FLAG))) {
    partition = &(argv[argNum][strlen(CHPL_PARTITION_FLAG)+1]);
    return 1;
  }

  // handle --exclude <nodes> or --exclude=<nodes>
  if (!strcmp(argv[argNum], CHPL_EXCLUDE_FLAG)) {
    exclude = argv[argNum+1];
    return 2;
  } else if (!strncmp(argv[argNum], CHPL_EXCLUDE_FLAG"=", strlen(CHPL_EXCLUDE_FLAG))) {
    exclude = &(argv[argNum][strlen(CHPL_EXCLUDE_FLAG)+1]);
    return 1;
  }
  return 0;
}


void chpl_launch_print_help(void) {
  fprintf(stdout, "LAUNCHER FLAGS:\n");
  fprintf(stdout, "===============\n");
  fprintf(stdout, "  %s <HH:MM:SS> : specify a wallclock time limit\n", CHPL_WALLTIME_FLAG);
  fprintf(stdout, "                           (or use $CHPL_LAUNCHER_WALLTIME)\n");
  fprintf(stdout, "  %s <partition> : specify a partition to use\n", CHPL_PARTITION_FLAG);
  fprintf(stdout, "                           (or use $CHPL_LAUNCHER_PARTITION)\n");
  fprintf(stdout, "  %s <nodes> : specify node(s) to exclude\n", CHPL_EXCLUDE_FLAG);
  fprintf(stdout, "                           (or use $CHPL_LAUNCHER_EXCLUDE)\n");
}
