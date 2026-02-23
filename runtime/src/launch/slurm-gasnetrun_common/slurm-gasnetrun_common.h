/*
 * Copyright 2020-2026 Hewlett Packard Enterprise Development LP
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
#include "chpl-env.h"
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
#define CHPL_NODELIST_FLAG "--nodelist"
#define CHPL_PARTITION_FLAG "--partition"
#define CHPL_EXCLUDE_FLAG "--exclude"
#define CHPL_GPUS_PER_NODE_FLAG "--gpus-per-node"
#define CHPL_LAUNCHER_PASSTHROUGH_FLAG "--system-launcher-flags"

#define CHPL_LPN_VAR "LOCALES_PER_NODE"

static char* debug = NULL;
static char* walltime = NULL;
static char* nodelist = NULL;
static char* partition = NULL;
static char* exclude = NULL;
static char* gpusPerNode = NULL;
static char** launcherPassthroughFlags = NULL;
static int numLauncherPassthroughFlags = 0;
char* slurmFilename = NULL;

/* copies of binary to run per node */
#define procsPerNode 1

#define launcherAccountEnvvar "CHPL_LAUNCHER_ACCOUNT"

typedef enum {
  slurm,
  unknown
} sbatchVersion;

static const char* nodeAccessStr = NULL;

static void appendPassthroughFlag(char*** array, int* size, const char* flag) {
  *array = (char**)chpl_mem_realloc(*array, (*size + 1) * sizeof(char*),
                                    CHPL_RT_MD_COMMAND_BUFFER, -1, 0);
  (*array)[*size] = (char*)string_copy((char*)flag, -1, 0);
  (*size)++;
}

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

static chpl_bool getSlurmDebug(void) {
  chpl_bool result = false;
  char *debugString = getenv("SALLOC_DEBUG");
  if (debugString) {
    result = (atoi(debugString) != 0) ? true : false;
  }
  return result;
}

static void genNumLocalesOptions(FILE* slurmFile, sbatchVersion sbatch,
                                 int32_t numLocales,
                                 int32_t numCoresPerLocale,
                                 int32_t numNodes) {
  char* constraint = getenv("CHPL_LAUNCHER_CONSTRAINT");

  // command line walltime takes precedence over env var
  if (!walltime) {
    walltime = getenv("CHPL_LAUNCHER_WALLTIME");
  }

  // command line nodelist takes precedence over env var
  if (!nodelist) {
    nodelist = getenv("CHPL_LAUNCHER_NODELIST");
  }

  // command line partition takes precedence over env var
  if (!partition) {
    partition = getenv("CHPL_LAUNCHER_PARTITION");
  }

  // command line exclude list takes precedence over env var
  if (!exclude) {
    exclude = getenv("CHPL_LAUNCHER_EXCLUDE");
  }

  // command line gpus per node takes precedence over env var
  if (!gpusPerNode) {
    gpusPerNode = getenv("CHPL_LAUNCHER_GPUS_PER_NODE");
  }


  if (walltime)
    fprintf(slurmFile, "#SBATCH --time=%s\n", walltime);
  if (nodelist)
    fprintf(slurmFile, "#SBATCH --nodelist=%s\n", nodelist);
  if (partition)
    fprintf(slurmFile, "#SBATCH --partition=%s\n", partition);
  if (exclude)
    fprintf(slurmFile, "#SBATCH --exclude=%s\n", exclude);
  if (gpusPerNode)
    fprintf(slurmFile, "#SBATCH --gpus-per-node=%s\n", gpusPerNode);
  switch (sbatch) {
  case slurm: {
    fprintf(slurmFile, "#SBATCH --nodes=%d\n", numNodes);
    fprintf(slurmFile, "#SBATCH --ntasks=%d\n", numLocales);
    // If needed a constraint can be specified with the env var CHPL_LAUNCHER_CONSTRAINT
    if (constraint) {
      fprintf(slurmFile, "#SBATCH --constraint=%s\n", constraint);
    }
    if (nodeAccessStr != NULL)
      fprintf(slurmFile, "#SBATCH --%s\n", nodeAccessStr);

    break;
  }
  default:
    break;
  }
}

// Append environment variables using chpl_append_to_cmd.
// charsWritten may be NULL, in which case it is ignored.
static void propagate_environment(char** buf, int* charsWritten)
{
  int ignoredCharsWritten;
  if (!charsWritten) {
    charsWritten = &ignoredCharsWritten;
  }

  // Indiscriminately propagate all environment variables.
  // We could do this more selectively, but we would be likely
  // to leave out something important.
  char *enviro_keys = chpl_get_enviro_keys(',');
  if (enviro_keys) {
    chpl_append_to_cmd(buf, charsWritten, " -E '%s'", enviro_keys);
    chpl_mem_free(enviro_keys, 0, 0);
  }
}

static char* chpl_launch_create_command(int argc, char* argv[],
                                        int32_t numLocales,
                                        int32_t numLocalesPerNode) {

  int i;
  int size;
  char* baseCommand = NULL;
  char* command;
  FILE* slurmFile;
  char* projectString = getenv(launcherAccountEnvvar);
  char* constraint = getenv("CHPL_LAUNCHER_CONSTRAINT");
  char* outputfn = getenv("CHPL_LAUNCHER_SLURM_OUTPUT_FILENAME");
  char* errorfn = getenv("CHPL_LAUNCHER_SLURM_ERROR_FILENAME");
  char* basenamePtr = strrchr(argv[0], '/');
  char* nodeAccessEnv = NULL;
  pid_t mypid;
  char  jobName[128];

  if (basenamePtr == NULL) {
      basenamePtr = argv[0];
  } else {
      basenamePtr++;
  }
  chpl_launcher_get_job_name(basenamePtr, jobName, sizeof(jobName));

  chpl_compute_real_binary_name(argv[0]);

  int32_t numNodes = (numLocales + numLocalesPerNode - 1) / numLocalesPerNode;

  // command line walltime takes precedence over env var
  if (!walltime) {
    walltime = getenv("CHPL_LAUNCHER_WALLTIME");
  }

  // command line nodelist takes precedence over env var
  if (!nodelist) {
    nodelist = getenv("CHPL_LAUNCHER_NODELIST");
  }

  // command line partition takes precedence over env var
  if (!partition) {
    partition = getenv("CHPL_LAUNCHER_PARTITION");
  }

  // Chapel env var takes precedence over Slurm
  if (!partition) {
    partition = getenv("SLURM_PARTITION");
  }

  // job's partition trumps everything
  char *tmp = getenv("SLURM_JOB_PARTITION");
  if (tmp) {
    partition = tmp;
  }

  // command line exclude list takes precedence over env var
  if (!exclude) {
    exclude = getenv("CHPL_LAUNCHER_EXCLUDE");
  }

  // command line gpus per node takes precedence over env var
  if (!gpusPerNode) {
    gpusPerNode = getenv("CHPL_LAUNCHER_GPUS_PER_NODE");
  }

  // append any user specified passthrough flags to the list of flags to pass through
  char* passthroughFlagsEnv = getenv("CHPL_LAUNCHER_PASSTHROUGH_FLAGS");
  if (passthroughFlagsEnv) {
    appendPassthroughFlag(&launcherPassthroughFlags, &numLauncherPassthroughFlags, passthroughFlagsEnv);
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
  int slurmFilenameLen =
      strlen(baseSBATCHFilename) + snprintf(NULL, 0, "%d", (int)mypid) + 1;
  slurmFilename = (char*)chpl_mem_allocMany(slurmFilenameLen, sizeof(char),
                                            CHPL_RT_MD_FILENAME, -1, 0);
  snprintf(slurmFilename, slurmFilenameLen, "%s%d", baseSBATCHFilename,
           (int)mypid);

  if (getenv("CHPL_LAUNCHER_USE_SBATCH") != NULL) {
    slurmFile = fopen(slurmFilename, "w");
    fprintf(slurmFile, "#!/bin/sh\n\n");
    fprintf(slurmFile, "#SBATCH -J %s\n", jobName);
    genNumLocalesOptions(slurmFile, determineSlurmVersion(), numLocales,
                         getNumCoresPerLocale(), numNodes);

    if (projectString && strlen(projectString) > 0)
      fprintf(slurmFile, "#SBATCH -A %s\n", projectString);

    // add any additional flags
    if (launcherPassthroughFlags != NULL) {
      for (int i = 0; i < numLauncherPassthroughFlags; i++) {
        fprintf(slurmFile, "#SBATCH %s\n", launcherPassthroughFlags[i]);
      }
    }

    if (outputfn != NULL)
      fprintf(slurmFile, "#SBATCH -o %s\n", outputfn);
    else
      fprintf(slurmFile, "#SBATCH -o %s.%%j.out\n", argv[0]);

    if (errorfn != NULL)
      fprintf(slurmFile, "#SBATCH -e %s\n", errorfn);

    fprintf(slurmFile, "%s/%s/%s -n %d -N %d -c 0",
            CHPL_THIRD_PARTY, WRAP_TO_STR(LAUNCH_PATH), GASNETRUN_LAUNCHER,
            numLocales, numLocales);

    char* envProp = NULL;
    propagate_environment(&envProp, NULL);
    fprintf(slurmFile, "%s", envProp);
    chpl_mem_free(envProp, 0, 0);

    fprintf(slurmFile, " %s %s", chpl_get_real_binary_wrapper(), chpl_get_real_binary_name());

    for (i=1; i<argc; i++) {
      fprintf(slurmFile, " '%s'", argv[i]);
    }
    fprintf(slurmFile, "\n");

    fclose(slurmFile);
    chmod(slurmFilename, 0755);
    const char* format="sbatch %s\n";
    int baseCommandLen = strlen(slurmFilename) + strlen(format);
    baseCommand = (char*)chpl_mem_allocMany(baseCommandLen, sizeof(char),
                                            CHPL_RT_MD_COMMAND_BUFFER, -1, 0);
    snprintf(baseCommand, baseCommandLen, format, slurmFilename);
  } else {
    char* iCom = NULL;
    int len = 0;

    if (!getSlurmDebug()) {
      chpl_append_to_cmd(&iCom, &len, "--quiet ");
    }
    chpl_append_to_cmd(&iCom, &len, "-J %s ", jobName);
    chpl_append_to_cmd(&iCom, &len, "-N %d ", numNodes);
    chpl_append_to_cmd(&iCom, &len, "--ntasks=%d ", numLocales);
    if (nodeAccessStr != NULL) chpl_append_to_cmd(&iCom, &len, "--%s ", nodeAccessStr);
    if (walltime) chpl_append_to_cmd(&iCom, &len, "--time=%s ", walltime);
    if (nodelist) chpl_append_to_cmd(&iCom, &len, "--nodelist=%s ", nodelist);
    if (partition) chpl_append_to_cmd(&iCom, &len, "--partition=%s ", partition);
    if (exclude) chpl_append_to_cmd(&iCom, &len, "--exclude=%s ", exclude);
    if (gpusPerNode) chpl_append_to_cmd(&iCom, &len, "--gpus-per-node=%s ", gpusPerNode);
    if (projectString && strlen(projectString) > 0)
      chpl_append_to_cmd(&iCom, &len, "--account=%s ", projectString);
    if (constraint) chpl_append_to_cmd(&iCom, &len, "-C %s", constraint);
    // add any additional flags
    if (launcherPassthroughFlags != NULL) {
      for (int i = 0; i < numLauncherPassthroughFlags; i++) {
        chpl_append_to_cmd(&iCom, &len, " %s", launcherPassthroughFlags[i]);
      }
    }
    chpl_append_to_cmd(&iCom, &len, " %s/%s/%s -n %d -N %d -c 0",
                   CHPL_THIRD_PARTY, WRAP_TO_STR(LAUNCH_PATH),
                   GASNETRUN_LAUNCHER, numLocales, numNodes);
    propagate_environment(&iCom, &len);
    chpl_append_to_cmd(&iCom, &len, " %s %s", chpl_get_real_binary_wrapper(),
                   chpl_get_real_binary_name());
    for (i=1; i<argc; i++) {
      chpl_append_to_cmd(&iCom, &len, " '%s'", argv[i]);
    }
    const char* format = "salloc %s";
    int baseCommandLen = strlen(format) + len + 1;
    baseCommand = (char*)chpl_mem_allocMany(baseCommandLen, sizeof(char),
                                            CHPL_RT_MD_COMMAND_BUFFER, -1, 0);
    snprintf(baseCommand, baseCommandLen, format, iCom);
    chpl_mem_free(iCom, 0, 0);
  }

  size = strlen(baseCommand) + 1;

  command =
      chpl_mem_allocMany(size, sizeof(char), CHPL_RT_MD_COMMAND_BUFFER, -1, 0);

  snprintf(command, size, "%s", baseCommand);
  chpl_mem_free(baseCommand, 0, 0);
  if (strlen(command)+1 > size) {
    chpl_internal_error("buffer overflow");
  }

  return command;
}

static void chpl_launch_cleanup(void) {
  if (!chpl_doDryRun() && !debug) {
    if (getenv("CHPL_LAUNCHER_USE_SBATCH") != NULL) {
      unlink(slurmFilename);
    }
  }
}


int chpl_launch(int argc, char* argv[], int32_t numLocales,
                int32_t numLocalesPerNode) {
  int retcode;

  debug = getenv("CHPL_LAUNCHER_DEBUG");

  char* command = chpl_launch_create_command(argc, argv, numLocales, numLocalesPerNode);
  retcode = chpl_launch_using_system(command, argv[0]);
  chpl_launch_cleanup();
  chpl_mem_free(slurmFilename, 0, 0);
  chpl_mem_free(command, 0, 0);
  return retcode;
}


int chpl_launch_handle_arg(int argc, char* argv[], int argNum,
                           int32_t lineno, int32_t filename) {

  // handle --walltime <walltime> or --walltime=<walltime>
  if (!strcmp(argv[argNum], CHPL_WALLTIME_FLAG)) {
    walltime = argv[argNum+1];
    return 2;
  } else if (!strncmp(argv[argNum], CHPL_WALLTIME_FLAG"=", strlen(CHPL_WALLTIME_FLAG"="))) {
    walltime = &(argv[argNum][strlen(CHPL_WALLTIME_FLAG"=")]);
    return 1;
  }

  // handle --nodelist <nodelist> or --nodelist=<nodelist>
  if (!strcmp(argv[argNum], CHPL_NODELIST_FLAG)) {
    nodelist = argv[argNum+1];
    return 2;
  } else if (!strncmp(argv[argNum], CHPL_NODELIST_FLAG"=", strlen(CHPL_NODELIST_FLAG"="))) {
    nodelist = &(argv[argNum][strlen(CHPL_NODELIST_FLAG"=")]);
    return 1;
  }

  // handle --partition <partition> or --partition=<partition>
  if (!strcmp(argv[argNum], CHPL_PARTITION_FLAG)) {
    partition = argv[argNum+1];
    return 2;
  } else if (!strncmp(argv[argNum], CHPL_PARTITION_FLAG"=", strlen(CHPL_PARTITION_FLAG"="))) {
    partition = &(argv[argNum][strlen(CHPL_PARTITION_FLAG"=")]);
    return 1;
  }

  // handle --exclude <nodes> or --exclude=<nodes>
  if (!strcmp(argv[argNum], CHPL_EXCLUDE_FLAG)) {
    exclude = argv[argNum+1];
    return 2;
  } else if (!strncmp(argv[argNum], CHPL_EXCLUDE_FLAG"=", strlen(CHPL_EXCLUDE_FLAG"="))) {
    exclude = &(argv[argNum][strlen(CHPL_EXCLUDE_FLAG"=")]);
    return 1;
  }

  // handle --gpus-per-node <gpus> or --gpus-per-node=<gpus>
  if (!strcmp(argv[argNum], CHPL_GPUS_PER_NODE_FLAG)) {
    gpusPerNode = argv[argNum+1];
    return 2;
  } else if (!strncmp(argv[argNum], CHPL_GPUS_PER_NODE_FLAG"=", strlen(CHPL_GPUS_PER_NODE_FLAG"="))) {
    gpusPerNode = &(argv[argNum][strlen(CHPL_GPUS_PER_NODE_FLAG"=")]);
    return 1;
  }

  // handle --system-launcher-flags <flags> or --system-launcher-flags=<flags>
  if (!strcmp(argv[argNum], CHPL_LAUNCHER_PASSTHROUGH_FLAG)) {
    appendPassthroughFlag(&launcherPassthroughFlags, &numLauncherPassthroughFlags, argv[argNum+1]);
    return 2;
  } else if (!strncmp(argv[argNum], CHPL_LAUNCHER_PASSTHROUGH_FLAG"=", strlen(CHPL_LAUNCHER_PASSTHROUGH_FLAG"="))) {
    appendPassthroughFlag(&launcherPassthroughFlags, &numLauncherPassthroughFlags, &(argv[argNum][strlen(CHPL_LAUNCHER_PASSTHROUGH_FLAG"=")]));
    return 1;
  }

  return 0;
}


const argDescTuple_t* chpl_launch_get_help(void) {
  static const
    argDescTuple_t args[] =
    { { CHPL_WALLTIME_FLAG " <HH:MM:SS>",
        "specify a wallclock time limit"
      },
      { "",
        "(or use $CHPL_LAUNCHER_WALLTIME)"
      },
      { CHPL_NODELIST_FLAG " <nodelist>",
        "specify a nodelist to use"
      },
      { "",
        "(or use $CHPL_LAUNCHER_NODELIST)"
      },
      { CHPL_PARTITION_FLAG " <partition>",
        "specify a partition to use"
      },
      { "",
        "(or use $CHPL_LAUNCHER_PARTITION)"
      },
      { CHPL_EXCLUDE_FLAG " <nodes>",
        "specify node(s) to exclude"
      },
      { "",
        "(or use $CHPL_LAUNCHER_EXCLUDE)"
      },
      {
        CHPL_GPUS_PER_NODE_FLAG " <gpus>",
        "specify the number of GPUs per node"
      },
      { "",
        "(or use $CHPL_LAUNCHER_GPUS_PER_NODE)"
      },
      {
        CHPL_LAUNCHER_PASSTHROUGH_FLAG " <flags>",
        "specify additional flags to pass through to the launcher"
      },
      { "",
        "(or use $CHPL_LAUNCHER_PASSTHROUGH_FLAGS)"
      },
      { NULL, NULL },
    };
  return args;
}
