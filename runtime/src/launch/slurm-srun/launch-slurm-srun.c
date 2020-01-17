/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

#define baseSBATCHFilename ".chpl-slurm-sbatch-"

#define CHPL_WALLTIME_FLAG "--walltime"
#define CHPL_GENERATE_SBATCH_SCRIPT "--generate-sbatch-script"
#define CHPL_NODELIST_FLAG "--nodelist"
#define CHPL_PARTITION_FLAG "--partition"
#define CHPL_EXCLUDE_FLAG "--exclude"

static char* debug = NULL;
static char* walltime = NULL;
static int generate_sbatch_script = 0;
static char* nodelist = NULL;
static char* partition = NULL;
static char* exclude = NULL;

char slurmFilename[FILENAME_MAX];

/* copies of binary to run per node */
#define procsPerNode 1

typedef enum {
  slurmpro,
  uma,
  slurm,
  unknown
} sbatchVersion;

// /tmp is always available on cray compute nodes (it's a memory mounted dir.)
// If we ever need this to run on non-cray machines, we should update this to
// look for the ISO/IEC 9945 env var options first, then P_tmpdir, then "/tmp"
static const char* getTmpDir(void) {
  return "/tmp";
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


// Get the number of locales from the environment variable or if that is not 
// set just use sinfo to get the number of cpus. 
static int getCoresPerLocale(void) {
  int numCores = -1;
  const int buflen = 1024;
  char buf[buflen];
  char partition_arg[128];
  char* argv[8];
  char* numCoresString = getenv("CHPL_LAUNCHER_CORES_PER_LOCALE");

  if (numCoresString) {
    numCores = atoi(numCoresString);
    if (numCores > 0)
      return numCores;
    chpl_warning("CHPL_LAUNCHER_CORES_PER_LOCALE must be > 0.", 0, 0);
  }

  argv[0] = (char *)  "sinfo";        // use sinfo to get num cpus
  argv[1] = (char *)  "--exact";      // get exact otherwise you get 16+, etc
  argv[2] = (char *)  "--format=%c";  // format to get num cpu per node (%c)
  argv[3] = (char *)  "--sort=+=#c";  // sort by num cpu (lower to higher)
  argv[4] = (char *)  "--noheader";   // don't show header (hide "CPU" header)
  argv[5] = (char *)  "--responding"; // only care about online nodes
  argv[6] = NULL;
  // Set the partition if it was specified
  if (partition) {
    sprintf(partition_arg, "--partition=%s", partition);
    argv[6] = partition_arg;
    argv[7] = NULL;
  }

  memset(buf, 0, buflen);
  if (chpl_run_utility1K("sinfo", argv, buf, buflen) <= 0)
    chpl_error("Error trying to determine number of cores per node", 0, 0);

  if (sscanf(buf, "%d", &numCores) != 1)
    chpl_error("unable to determine number of cores per locale; "
               "please set CHPL_LAUNCHER_CORES_PER_LOCALE", 0, 0);

  return numCores;
}
#define MAX_COM_LEN (FILENAME_MAX + 128)
// create the command that will actually launch the program and 
// create any files needed for the launch like the batch script 
static char* chpl_launch_create_command(int argc, char* argv[], 
                                        int32_t numLocales) {
  int i;
  int size;
  char baseCommand[MAX_COM_LEN];
  char* command;
  FILE* slurmFile;
  char* account = getenv("CHPL_LAUNCHER_ACCOUNT");
  char* constraint = getenv("CHPL_LAUNCHER_CONSTRAINT");
  char* outputfn = getenv("CHPL_LAUNCHER_SLURM_OUTPUT_FILENAME");
  char* errorfn = getenv("CHPL_LAUNCHER_SLURM_ERROR_FILENAME");
  char* nodeAccessEnv = getenv("CHPL_LAUNCHER_NODE_ACCESS");
  const char* nodeAccessStr = NULL;

  char* basenamePtr = strrchr(argv[0], '/');
  pid_t mypid;

  // For programs with large amounts of output, a lot of time can be
  // spent syncing the stdout buffer to the output file. This can cause
  // tests to run extremely slow and can cause stdout and stderr to
  // become mixed in odd ways since stdout is buffered but stderr isn't.
  // To alleviate this problem (and to allow accurate external timings
  // of tests) this allows the output to be "buffered" to <tmpDir> and
  // copied once the job is done.
  //
  // Note that this should work even for multi-locale tests since all
  // the output is piped through a single node.
  //
  // The *NoFmt versions are the same as the regular version, except
  // that instead of using slurms output formatters, they use the
  // corresponding env var. e.g. you have to use '--output=%j.out to
  // have the output file be <jobid>.out, but when we copy the tmp file
  // to the real output file, the %j and other formatters aren't
  // available so we have to use the equivalent slurm env var
  // (SLURM_JOB_ID.) The env vars can't be used when specifying --output
  // because they haven't been initialized yet
  char* bufferStdout    = getenv("CHPL_LAUNCHER_SLURM_BUFFER_STDOUT");
  const char* tmpDir    = getTmpDir();
  char stdoutFile         [MAX_COM_LEN];
  char stdoutFileNoFmt    [MAX_COM_LEN];
  char tmpStdoutFileNoFmt [MAX_COM_LEN];

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

  // command line exclude takes precedence over env var
  if (!exclude) {
    exclude = getenv("CHPL_LAUNCHER_EXCLUDE");
  }

  // request exclusive node access by default, but allow user to override
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

  // Elliot, 12/02/14: TODO we have a bunch of similar commands to build up the
  // interactive and batch versions. It would be nicer to build up the commands
  // and postprocess depending on interactive vs batch. As in build up "--quiet
  // --nodes ..." and afterwards split on ' ' and then add #SBATCH and a
  // newline for batch mode and leave it as is for interactive"

  // if were running a batch job 
  if (getenv("CHPL_LAUNCHER_USE_SBATCH") != NULL || generate_sbatch_script) {
    // set the sbatch filename
    sprintf(slurmFilename, "%s%d", baseSBATCHFilename, (int)mypid);

    // open the batch file and create the header 
    slurmFile = fopen(slurmFilename, "w");
    fprintf(slurmFile, "#!/bin/sh\n\n");
    
    // set the job name 
    fprintf(slurmFile, "#SBATCH --job-name=Chpl-%.10s\n", basenamePtr);
 
    // suppress informational messages, will still display errors 
    fprintf(slurmFile, "#SBATCH --quiet\n");
    
    // request the number of locales, with 1 task per node, and number of cores
    // cpus-per-task. We probably don't need --nodes and --ntasks specified
    // since 1 task-per-node with n --tasks implies -n nodes
    fprintf(slurmFile, "#SBATCH --nodes=%d\n", numLocales);
    fprintf(slurmFile, "#SBATCH --ntasks=%d\n", numLocales);
    fprintf(slurmFile, "#SBATCH --ntasks-per-node=%d\n", procsPerNode);
    fprintf(slurmFile, "#SBATCH --cpus-per-task=%d\n", getCoresPerLocale());
    
    // request specified node access
    if (nodeAccessStr != NULL)
      fprintf(slurmFile, "#SBATCH --%s\n", nodeAccessStr);

    // request access to all memory
    fprintf(slurmFile, "#SBATCH --mem=0\n");

    // Set the walltime if it was specified 
    if (walltime) { 
      fprintf(slurmFile, "#SBATCH --time=%s\n", walltime);
    }

    // Set the nodelist if it was specified
    if (nodelist) {
      fprintf(slurmFile, "#SBATCH --nodelist=%s\n", nodelist);
    }

    // Set the partition if it was specified
    if (partition) {
      fprintf(slurmFile, "#SBATCH --partition=%s\n", partition);
    }

    // Set the exclude list if it was specified
    if (exclude) {
      fprintf(slurmFile, "#SBATCH --exclude=%s\n", exclude);
    }

    // If needed a constraint can be specified with the env var CHPL_LAUNCHER_CONSTRAINT
    if (constraint) {
      fprintf(slurmFile, "#SBATCH --constraint=%s\n", constraint);
    }

    // set the account name if one was provided  
    if (account && strlen(account) > 0) {
      fprintf(slurmFile, "#SBATCH --account=%s\n", account);
    }

    // set the output file name to either the user specified
    // name or to the binaryName.<jobID>.out if none specified
    if (outputfn != NULL) {
      sprintf(stdoutFile,      "%s", outputfn);
      sprintf(stdoutFileNoFmt, "%s", outputfn);
    }
    else {
      sprintf(stdoutFile,      "%s.%s.out", argv[0], "%j");
      sprintf(stdoutFileNoFmt, "%s.%s.out", argv[0], "$SLURM_JOB_ID");
    }

    // We have slurm use the real output file to capture slurm errors/timeouts
    // We only redirect the program output to the tmp file
    fprintf(slurmFile, "#SBATCH --output=%s\n", stdoutFile);

    if (errorfn != NULL) {
      fprintf(slurmFile, "#SBATCH --error=%s\n", errorfn);
    }

    // If we're buffering the output, set the temp output file name.
    // It's always <tmpDir>/binaryName.<jobID>.out.
    if (bufferStdout != NULL) {
      sprintf(tmpStdoutFileNoFmt, "%s/%s.%s.out", tmpDir, argv[0], "$SLURM_JOB_ID");
    }

    // add the srun command and the (possibly wrapped) binary name.
    fprintf(slurmFile, "srun --kill-on-bad-exit %s %s ",
        chpl_get_real_binary_wrapper(), chpl_get_real_binary_name());

    // add any arguments passed to the launcher to the binary 
    for (i=1; i<argc; i++) {
      fprintf(slurmFile, "'%s' ", argv[i]);
    }

    // buffer stdout to the tmp stdout file
    if (bufferStdout != NULL) {
      fprintf(slurmFile, "> %s", tmpStdoutFileNoFmt);
    }
    fprintf(slurmFile, "\n");

    // After the job is run, if we buffered stdout to <tmpDir>, we need
    // to copy the output to the actual output file. The <tmpDir> output
    // will only exist on one node, ignore failures on the other nodes
    if (bufferStdout != NULL) {
      fprintf(slurmFile, "cat %s >> %s\n", tmpStdoutFileNoFmt, stdoutFileNoFmt);
      fprintf(slurmFile, "rm  %s &> /dev/null\n", tmpStdoutFileNoFmt);
    }

    // close the batch file and change permissions 
    fclose(slurmFile);
    chmod(slurmFilename, 0755);

    if (generate_sbatch_script) {
      fprintf(stdout, "SBATCH script written to '%s'\n", slurmFilename);
    }

    // the baseCommand is what will call the batch file
    // that was just created 
    sprintf(baseCommand, "sbatch %s\n", slurmFilename);
  }
  // else we're running an interactive job 
  else {
    char iCom[1024];
    int len;

    len = 0;

    // set the job name 
    len += sprintf(iCom+len, "--job-name=CHPL-%.10s ",basenamePtr);
    
    // suppress informational messages, will still display errors 
    len += sprintf(iCom+len, "--quiet ");

    // request the number of locales, with 1 task per node, and number of cores
    // cpus-per-task. We probably don't need --nodes and --ntasks specified
    // since 1 task-per-node with n --tasks implies -n nodes
    len += sprintf(iCom+len, "--nodes=%d ",numLocales);
    len += sprintf(iCom+len, "--ntasks=%d ", numLocales);
    len += sprintf(iCom+len, "--ntasks-per-node=%d ", procsPerNode);
    len += sprintf(iCom+len, "--cpus-per-task=%d ", getCoresPerLocale());
    
    // request specified node access
    if (nodeAccessStr != NULL)
      len += sprintf(iCom+len, "--%s ", nodeAccessStr);

    // request access to all memory
    len += sprintf(iCom+len, "--mem=0 ");

    // kill the job if any program instance halts with non-zero exit status
    len += sprintf(iCom+len, "--kill-on-bad-exit ");
    
    // Set the walltime if it was specified 
    if (walltime) {
      len += sprintf(iCom+len, "--time=%s ",walltime);
    }

    // Set the nodelist if it was specified
    if (nodelist) {
      len += sprintf(iCom+len, "--nodelist=%s ", nodelist);
    }

    // Set the partition if it was specified
    if (partition) {
      len += sprintf(iCom+len, "--partition=%s ", partition);
    }

    // Set the exclude list if it was specified
    if (exclude) {
      len += sprintf(iCom+len, "--exclude=%s ", exclude);
    }

    // set any constraints 
    if (constraint) {
      len += sprintf(iCom+len, "--constraint=%s ", constraint);
    }
    
    // set the account name if one was provided  
    if (account && strlen(account) > 0) {
      len += sprintf(iCom+len, "--account=%s ", account);
    }
    
    // add the (possibly wrapped) binary name
    len += sprintf(iCom+len, "%s %s ",
        chpl_get_real_binary_wrapper(), chpl_get_real_binary_name());

    // add any arguments passed to the launcher to the binary 
    for (i=1; i<argc; i++) {
      len += sprintf(iCom+len, "%s ", argv[i]);
    }
   
    // launch the job using srun
    sprintf(baseCommand, "srun %s ", iCom);
  }

  // copy baseCommand into command and return it 
  size = strlen(baseCommand) + 1;
  command = chpl_mem_allocMany(size, sizeof(char), CHPL_RT_MD_COMMAND_BUFFER, -1, 0);
  sprintf(command, "%s", baseCommand);
  if (strlen(command)+1 > size) {
    chpl_internal_error("buffer overflow");
  }
  
  return command;
}


static void genSBatchScript(int argc, char *argv[], int numLocales) {
  chpl_launch_create_command(argc, argv, numLocales);
}


// clean up the batch file
static void chpl_launch_cleanup(void) {
  // leave file around if we're debugging 
  if (!debug) {
    // remove sbatch file unless it was explicitly generated by the user
    if (getenv("CHPL_LAUNCHER_USE_SBATCH") != NULL && !generate_sbatch_script) {
      if (unlink(slurmFilename)) {
        char msg[FILENAME_MAX + 128];
        snprintf(msg, sizeof(msg), "Error removing temporary file '%s': %s",
                 slurmFilename, strerror(errno));
        chpl_warning(msg, 0, 0);
      }
    }
  }
}


int chpl_launch(int argc, char* argv[], int32_t numLocales) {
  int retcode;
  
  // check the slurm version before continuing 
  sbatchVersion sVersion = determineSlurmVersion();
  if (sVersion != slurm) {
    printf("Error: This launcher is only compatible with native slurm\n");
    printf("Slurm version was %d\n", sVersion);
    return 1;
  }
 
  debug = getenv("CHPL_LAUNCHER_DEBUG");
 
  // generate a batch script and exit if user wanted to 
  if (generate_sbatch_script) {
    genSBatchScript(argc, argv, numLocales);
    retcode = 0;
  }
  // otherwise generate the batch file or srun command and execute it
  else {
    retcode = chpl_launch_using_system(chpl_launch_create_command(argc, argv,
          numLocales), argv[0]);
  
    chpl_launch_cleanup();
  }
  return retcode;
}


// handle launcher args
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

  // handle --nodelist <nodelist> or --nodelist=<nodelist>
  if (!strcmp(argv[argNum], CHPL_NODELIST_FLAG)) {
    nodelist = argv[argNum+1];
    return 2;
  } else if (!strncmp(argv[argNum], CHPL_NODELIST_FLAG"=", strlen(CHPL_NODELIST_FLAG))) {
    nodelist = &(argv[argNum][strlen(CHPL_NODELIST_FLAG)+1]);
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

  // handle --generate-sbatch-script
  if (!strcmp(argv[argNum], CHPL_GENERATE_SBATCH_SCRIPT)) {
    generate_sbatch_script = 1;
    return 1;
  }

  // Elliot, 12/02/14: TODO we should have a core binding option here similar
  // to aprun's -cc to handle binding to cores / numa domains, etc For now you
  // can just set the SLURM_CPU_BIND env var

  return 0;
}



void chpl_launch_print_help(void) {
  fprintf(stdout, "LAUNCHER FLAGS:\n");
  fprintf(stdout, "===============\n");
  fprintf(stdout, "  %s : generate an sbatch script and exit\n", CHPL_GENERATE_SBATCH_SCRIPT);
  fprintf(stdout, "  %s <HH:MM:SS> : specify a wallclock time limit\n", CHPL_WALLTIME_FLAG);
  fprintf(stdout, "                           (or use $CHPL_LAUNCHER_WALLTIME)\n");
  fprintf(stdout, "  %s <nodelist> : specify a nodelist to use\n", CHPL_NODELIST_FLAG);
  fprintf(stdout, "                           (or use $CHPL_LAUNCHER_NODELIST)\n");
  fprintf(stdout, "  %s <partition> : specify a partition to use\n", CHPL_PARTITION_FLAG);
  fprintf(stdout, "                           (or use $CHPL_LAUNCHER_PARTITION)\n");
  fprintf(stdout, "  %s <nodes> : specify node(s) to exclude\n", CHPL_EXCLUDE_FLAG);
  fprintf(stdout, "                           (or use $CHPL_LAUNCHER_EXCLUDE)\n");
}
