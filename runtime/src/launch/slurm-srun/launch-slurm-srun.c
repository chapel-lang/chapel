/*
 * Copyright 2004-2014 Cray Inc.
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
#define baseExpectFilename ".chpl-expect-"
#define baseSysFilename ".chpl-sys-"

#define CHPL_WALLTIME_FLAG "--walltime"
#define CHPL_GENERATE_SBATCH_SCRIPT "--generate-sbatch-script"
#define CHPL_NODELIST_FLAG "--nodelist"

static char* debug = NULL;
static char* walltime = NULL;
static int generate_sbatch_script = 0;
static char* nodelist = NULL;

char slurmFilename[FILENAME_MAX];
char expectFilename[FILENAME_MAX];
char sysFilename[FILENAME_MAX];

/* copies of binary to run per node */
#define procsPerNode 1

#define versionBuffLen 80
typedef enum {
  slurmpro,
  nccs,
  torque,
  uma,
  slurm,
  unknown
} sbatchVersion;

// Check what version of slurm is on the system 
// Since this is c we actually write the version to a file 
// and then get the version out 
static sbatchVersion determineSlurmVersion(void) {
  char version[versionBuffLen+1] = "";
  char* versionPtr = version;
  FILE* sysFile;
  int i;
  char * command; 
  sprintf(sysFilename, "%s", baseSysFilename);
  
  command = chpl_glom_strings(3, "sbatch --version > ", sysFilename, " 2>&1");
  system(command);
  sysFile = fopen(sysFilename, "r");
  for (i=0; i<versionBuffLen; i++) {
    char tmp;
    fscanf(sysFile, "%c", &tmp);
    if (tmp == '\n') {
      *versionPtr++ = '\0';
      break;
    } else {
      *versionPtr++ = tmp;
    }
  }

  fclose(sysFile);
  sprintf(command, "rm %s", sysFilename);
  system(command);

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
  char* argv[7];
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


  memset(buf, 0, buflen);
  if (chpl_run_utility1K("sinfo", argv, buf, buflen) <= 0)
    chpl_error("Error trying to determine number of cores per node", 0, 0);

  if (sscanf(buf, "%d", &numCores) != 1)
    chpl_error("unable to determine number of cores per locale; "
               "please set CHPL_LAUNCHER_CORES_PER_LOCALE", 0, 0);

  return numCores;
}

// create the command that will actually launch the program and 
// create any files needed for the launch like the batch script 
static char* chpl_launch_create_command(int argc, char* argv[], 
                                        int32_t numLocales) {
  int i;
  int size;
  char baseCommand[256];
  char* command;
  FILE* slurmFile, *expectFile;
  char* account = getenv("CHPL_LAUNCHER_ACCOUNT");
  char* constraint = getenv("CHPL_LAUNCHER_CONSTRAINT");
  char* outputfn = getenv("CHPL_LAUNCHER_SLURM_OUTPUT_FILENAME");
  char* basenamePtr = strrchr(argv[0], '/');
  pid_t mypid;

  // command line walltime takes precedence over env var
  if (!walltime) {
    walltime = getenv("CHPL_LAUNCHER_WALLTIME");
  }

  // command line nodelist takes precedence over env var
  if (!nodelist) {
    nodelist = getenv("CHPL_LAUNCHER_NODELIST");
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

  // set the filenames
  sprintf(expectFilename, "%s%d", baseExpectFilename, (int)mypid);
  sprintf(slurmFilename, "%s%d", baseSBATCHFilename, (int)mypid);

  // if were running a batch job 
  if (getenv("CHPL_LAUNCHER_USE_SBATCH") != NULL || generate_sbatch_script) {
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
    
    //request exclusive access to nodes 
    fprintf(slurmFile, "#SBATCH --exclusive\n");

    // Set the walltime if it was specified 
    if (walltime) { 
      fprintf(slurmFile, "#SBATCH --time=%s\n", walltime);
    }

    // Set the nodelist if it was specified
    if (nodelist) {
      fprintf(slurmFile, "#SBATCH --nodelist=%s\n", nodelist);
    }

    // If needed a constraint can be specified with the env var CHPL_LAUNCHER_CONSTRAINT
    if (constraint) {
      fprintf(slurmFile, "#SBATCH --constraint=%s\n", constraint);
    }

    // set the account name if one was provided  
    if (account && strlen(account) > 0) {
      fprintf(slurmFile, "#SBATCH --account=%s\n", account);
    }
 
    // set the output name to either the user specified
    // or to the binaryName.<jobID>.out if none specified
    if (outputfn!=NULL) {
      fprintf(slurmFile, "#SBATCH --output=%s\n", outputfn);
    }
    else {
      fprintf(slurmFile, "#SBATCH --output=%s.%%j.out\n", argv[0]);
    }

    // add the srun command 
    fprintf(slurmFile, "srun %s ", chpl_get_real_binary_name());
    
    // add any arguments passed to the launcher to the binary 
    for (i=1; i<argc; i++) {
      fprintf(slurmFile, " '%s'", argv[i]);
    }
    fprintf(slurmFile, "\n");

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
    // expect is used to launch an interactive job 
    // create the file and set some things for expect 
    expectFile = fopen(expectFilename, "w");
    fprintf(expectFile, "set timeout -1\n");
    fprintf(expectFile, "set prompt \"(%%|#|\\\\$|>) $\"\n");
    
    // create a silent salloc command
    fprintf(expectFile, "spawn -noecho srun ");

    // set the job name 
    fprintf(expectFile, "--job-name=CHPL-%.10s ",basenamePtr);  
    
    // suppress informational messages, will still display errors 
    fprintf(expectFile, "--quiet ");

    // request the number of locales, with 1 task per node, and number of cores
    // cpus-per-task. We probably don't need --nodes and --ntasks specified
    // since 1 task-per-node with n --tasks implies -n nodes
    fprintf(expectFile, "--nodes=%d ",numLocales); 
    fprintf(expectFile, "--ntasks=%d ", numLocales); 
    fprintf(expectFile, "--ntasks-per-node=%d ", procsPerNode); 
    fprintf(expectFile, "--cpus-per-task=%d ", getCoresPerLocale()); 
    
    // request exclusive access
    fprintf(expectFile, "--exclusive ");
    
    // Set the walltime if i was specified 
    if (walltime) {
      fprintf(expectFile, "--time=%s ", walltime); 
    }

    // Set the walltime if it was specified
    if (nodelist) {
      fprintf(expectFile, "--nodelist=%s ", nodelist);
    }

    // set any constraints 
    if (constraint) {
      fprintf(expectFile, " --constraint=%s ", constraint);
    }
    
    // set the account name if one was provided  
    if (account && strlen(account) > 0) {
      fprintf(expectFile, "--account=%s ", account);
    }

    // the actual srun command 
    fprintf(expectFile, "%s", chpl_get_real_binary_name());
    
    // add any arguments passed to the launcher to the binary 
    for (i=1; i<argc; i++) {
      fprintf(expectFile, " %s", argv[i]);
    }
    fprintf(expectFile, "\n\n");
   
    // do some things required for expect and close the file 
    fprintf(expectFile, "interact -o -re $prompt {return}\n");
    fclose(expectFile);
    
    // the baseCommand is what will call the expect file 
    sprintf(baseCommand, "expect %s", expectFilename);
  }

  // copy baseCommand into command and return it 
  size = strlen(baseCommand) + 1;
  command = chpl_mem_allocMany(size, sizeof(char), CHPL_RT_MD_COMMAND_BUFFER, -1, "");
  sprintf(command, "%s", baseCommand);
  if (strlen(command)+1 > size) {
    chpl_internal_error("buffer overflow");
  }
  
  return command;
}


static void genSBatchScript(int argc, char *argv[], int numLocales) {
  chpl_launch_create_command(argc, argv, numLocales);
}


// clean up the batch file or expect file in an interactive job 
static void chpl_launch_cleanup(void) {
  // leave file around if we're debugging 
  if (!debug) {
    // check if this is interactive or batch
    char* fileToRemove = NULL;
    if (getenv("CHPL_LAUNCHER_USE_SBATCH") == NULL) {
      fileToRemove = expectFilename;
    } else {
      fileToRemove = slurmFilename; 
    }
    
    // actually remove file 
    if (unlink(fileToRemove)) {
      char msg[1024];
      snprintf(msg, 1024, "Error removing temporary file '%s': %s",
               fileToRemove, strerror(errno));
      chpl_warning(msg, 0, 0);
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
  // otherwise generate the batch file or expect script and execute it
  else {
    retcode = chpl_launch_using_system(chpl_launch_create_command(argc, argv, 
              numLocales), argv[0]);
    
    chpl_launch_cleanup();
  }

  return retcode;
}


// handle launcher args
int chpl_launch_handle_arg(int argc, char* argv[], int argNum,
                           int32_t lineno, c_string filename) {
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

  // handle --generate-sbatch-script
  if (!strcmp(argv[argNum], CHPL_GENERATE_SBATCH_SCRIPT)) {
    generate_sbatch_script = 1;
    return 1;
  }

  // TODO we should have a core binding option here similar to aprun's -cc to
  // handle binding to cores / numa domains, etc
  // For now you can just set the SLURM_CPU_BIND env var

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


}
