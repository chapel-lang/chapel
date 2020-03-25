/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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
#include "chplcgfns.h"
#include "chpllaunch.h"
#include "chpl-mem.h"
#include "chpltypes.h"
#include "error.h"

#ifndef GASNETRUN_LAUNCHER
#error GASNETRUN_LAUNCHER must be defined
#endif

#define LAUNCH_PATH_HELP WRAP_TO_STR(LAUNCH_PATH)
#define WRAP_TO_STR(x) TO_STR(x)
#define TO_STR(x) #x

#define baseSBATCHFilename ".chpl-slurm-sbatch-"

#define CHPL_WALLTIME_FLAG "--walltime"
#define CHPL_PARTITION_FLAG "--partition"
#define CHPL_EXCLUDE_FLAG "--exclude"

static char* debug = NULL;
static char* walltime = NULL;
static char* partition = NULL;
static char* exclude = NULL;
char slurmFilename[FILENAME_MAX];

/* copies of binary to run per node */
#define procsPerNode 1  

#define launcherAccountEnvvar "CHPL_LAUNCHER_ACCOUNT"

typedef enum {
  slurm,
  unknown
} sbatchVersion;

static const char* nodeAccessStr = NULL;

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

  if (strstr(version, "slurm")) {
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

  if (walltime) 
    fprintf(slurmFile, "#SBATCH --time=%s\n", walltime);
  if (partition)
    fprintf(slurmFile, "#SBATCH --partition=%s\n", partition);
  if (exclude)
    fprintf(slurmFile, "#SBATCH --exclude=%s\n", exclude);
  switch (sbatch) {
  case slurm:
    fprintf(slurmFile, "#SBATCH --nodes=%d\n", numLocales);
    fprintf(slurmFile, "#SBATCH --ntasks-per-node=1\n");
    // If needed a constraint can be specified with the env var CHPL_LAUNCHER_CONSTRAINT
    if (constraint) {
      fprintf(slurmFile, "#SBATCH --constraint=%s\n", constraint);
    }
    if (nodeAccessStr != NULL)
      fprintf(slurmFile, "#SBATCH --%s\n", nodeAccessStr);

    break;
  default:
    break;
  }
}

static int propagate_environment(char* buf)
{
  int len = 0;

  // Indiscriminately propagate all environment variables.
  // We could do this more selectively, but we would be likely
  // to leave out something important.
  char *enviro_keys = chpl_get_enviro_keys(',');
  if (enviro_keys)
    len += sprintf(buf, " -E '%s'", enviro_keys);


  // If any of the relevant character set environment variables
  // are set, replicate the state of all of them.  This needs to
  // be done separately from the -E mechanism because the launcher
  // is written in Perl, which mangles the character set
  // environment.
  //
  // Note that if we are setting these variables, and one or more
  // of them is empty, we must set it with explicitly empty
  // contents (e.g. LC_ALL= instead of -u LC_ALL) so that the
  // Chapel launch mechanism will not overwrite it.
  char *lang = getenv("LANG");
  char *lc_all = getenv("LC_ALL");
  char *lc_collate = getenv("LC_COLLATE");
  if (lang || lc_all || lc_collate) {
    len += sprintf(buf+len, " env");
    len += sprintf(buf+len, " LANG=%s", lang ? lang : "");
    len += sprintf(buf+len, " LC_ALL=%s", lc_all ? lc_all : "");
    len += sprintf(buf+len, " LC_COLLATE=%s", lc_collate ? lc_collate : "");
  }
  return len;
}

static char* chpl_launch_create_command(int argc, char* argv[], 
                                        int32_t numLocales) {
  int i;
  int size;
  char baseCommand[2*FILENAME_MAX];
  char envProp[2*FILENAME_MAX];
  char* command;
  FILE* slurmFile;
  char* projectString = getenv(launcherAccountEnvvar);
  char* constraint = getenv("CHPL_LAUNCHER_CONSTRAINT");
  char* outputfn = getenv("CHPL_LAUNCHER_SLURM_OUTPUT_FILENAME");
  char* errorfn = getenv("CHPL_LAUNCHER_SLURM_ERROR_FILENAME");
  char* basenamePtr = strrchr(argv[0], '/');
  char* nodeAccessEnv = NULL;
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

  // request exclusive node access by default, but allow user to override
  nodeAccessEnv = getenv("CHPL_LAUNCHER_NODE_ACCESS");
  if (nodeAccessEnv == NULL || strcmp(nodeAccessEnv, "exclusive") == 0) {
    nodeAccessStr = "exclusive";
  } else if (strcmp(nodeAccessEnv, "shared") == 0 ||
             strcmp(nodeAccessEnv, "share") == 0 ||
             strcmp(nodeAccessEnv, "oversubscribed") == 0  ||
             strcmp(nodeAccessEnv, "oversubscribe") == 0) {
    nodeAccessStr = "share";
  } else if (strcmp(nodeAccessEnv, "unset") == 0) {
    nodeAccessStr = NULL;
  } else {
    chpl_warning("unsupported 'CHPL_LAUNCHER_NODE_ACCESS' option", 0, 0);
    nodeAccessStr = "exclusive";
  }

  if (debug) {
    mypid = 0;
  } else {
    mypid = getpid();
  }
  sprintf(slurmFilename, "%s%d", baseSBATCHFilename, (int)mypid);

  if (getenv("CHPL_LAUNCHER_USE_SBATCH") != NULL) {
    slurmFile = fopen(slurmFilename, "w");
    fprintf(slurmFile, "#!/bin/sh\n\n");
    fprintf(slurmFile, "#SBATCH -J Chpl-%.10s\n", basenamePtr);
    genNumLocalesOptions(slurmFile, determineSlurmVersion(), numLocales, getNumCoresPerLocale());

    if (projectString && strlen(projectString) > 0)
      fprintf(slurmFile, "#SBATCH -A %s\n", projectString);

    if (outputfn != NULL)
      fprintf(slurmFile, "#SBATCH -o %s\n", outputfn);
    else
      fprintf(slurmFile, "#SBATCH -o %s.%%j.out\n", argv[0]);

    if (errorfn != NULL)
      fprintf(slurmFile, "#SBATCH -e %s\n", errorfn);

    fprintf(slurmFile, "%s/%s/%s -n %d -N %d -c 0",
            CHPL_THIRD_PARTY, WRAP_TO_STR(LAUNCH_PATH), GASNETRUN_LAUNCHER,
            numLocales, numLocales);

    propagate_environment(envProp);
    fprintf(slurmFile, "%s", envProp);

    fprintf(slurmFile, " %s %s", chpl_get_real_binary_wrapper(), chpl_get_real_binary_name());

    for (i=1; i<argc; i++) {
      fprintf(slurmFile, " '%s'", argv[i]);
    }
    fprintf(slurmFile, "\n");

    fclose(slurmFile);
    chmod(slurmFilename, 0755);

    sprintf(baseCommand, "sbatch %s\n", slurmFilename);
  } else {
    char iCom[2*FILENAME_MAX-10];
    int len = 0;

    len += sprintf(iCom+len, "--quiet ");
    len += sprintf(iCom+len, "-J %.10s ", basenamePtr);
    len += sprintf(iCom+len, "-N %d ", numLocales);
    len += sprintf(iCom+len, "--ntasks-per-node=1 ");
    if (nodeAccessStr != NULL)
      len += sprintf(iCom+len, "--%s ", nodeAccessStr);
    if (walltime)
      len += sprintf(iCom+len, "--time=%s ", walltime);
    if(partition)
      len += sprintf(iCom+len, "--partition=%s ", partition);
    if(exclude)
      len += sprintf(iCom+len, "--exclude=%s ", exclude);
    if (constraint)
      len += sprintf(iCom+len, " -C %s", constraint);
    len += sprintf(iCom+len, " %s/%s/%s -n %d -N %d -c 0",
                   CHPL_THIRD_PARTY, WRAP_TO_STR(LAUNCH_PATH),
                   GASNETRUN_LAUNCHER, numLocales, numLocales);
    len += propagate_environment(iCom+len);
    len += sprintf(iCom+len, " %s %s", chpl_get_real_binary_wrapper(), chpl_get_real_binary_name());
    for (i=1; i<argc; i++) {
      len += sprintf(iCom+len, " %s", argv[i]);
    }

    sprintf(baseCommand, "salloc %s", iCom);
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
    if (getenv("CHPL_LAUNCHER_USE_SBATCH") != NULL) {
      unlink(slurmFilename);
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
