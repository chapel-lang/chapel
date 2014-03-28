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

char slurmFilename[FILENAME_MAX];
char expectFilename[FILENAME_MAX];
char sysFilename[FILENAME_MAX];

/* copies of binary to run per node */
#define versionBuffLen 80

#define launcherAccountEnvvar "CHPL_LAUNCHER_ACCOUNT"

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
  
  sprintf(sysFilename, "%s%d", baseSysFilename);
  
  char* command = chpl_glom_strings(3, "sbatch --version > ", sysFilename, " 2>&1");
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
 // sprintf(command, "rm %s", sysFilename);
 // system(command);

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


// Get the number of locales from the environment setting or return 
// zero if unset. Currently this is unused for slurm 
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


// create the command that will actually launch the program and 
// create any files needed for the launch like the batch script 
static char* chpl_launch_create_command(int argc, char* argv[], 
                                        int32_t numLocales) {
  int i;
  int size;
  char baseCommand[256];
  char* command;
  FILE* slurmFile, *expectFile;
  char* projectString = getenv(launcherAccountEnvvar);
  char* constraint = getenv("CHPL_LAUNCHER_CONSTRAINT");
  char* walltime = getenv("CHPL_LAUNCHER_WALLTIME");
  char* outputfn = getenv("CHPL_LAUNCHER_SLURM_OUTPUT_FILENAME");
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
  // set the filenames 
  sprintf(expectFilename, "%s%d", baseExpectFilename, (int)mypid);
  sprintf(slurmFilename, "%s%d", baseSBATCHFilename, (int)mypid);

  // if were running a batch job 
  if (getenv("CHPL_LAUNCHER_USE_SBATCH") != NULL) {
    // open the batch file and create the header 
    slurmFile = fopen(slurmFilename, "w");
    fprintf(slurmFile, "#!/bin/sh\n\n");
    
    // set the job name 
    fprintf(slurmFile, "#SBATCH -J Chpl-%.10s\n", basenamePtr);

    // Set the walltime if it was specified 
    if (walltime) { 
      fprintf(slurmFile, "#SBATCH --time=%s\n", walltime);
    }
    
    // request the number of locales and claim 1 task per node (more chapel
    // tasks can run per node, this is just for slurm) 
    fprintf(slurmFile, "#SBATCH --nodes=%d\n", numLocales);
    fprintf(slurmFile, "#SBATCH --ntasks-per-node=1\n");

    // If needed a constraint can be specified with the env var CHPL_LAUNCHER_CONSTRAINT
    if (constraint) {
      fprintf(slurmFile, "#SBATCH --constraint=%s\n", constraint);
    }
    
    //request exclusive access to nodes 
    fprintf(slurmFile, "#SBATCH --exclusive\n");
    
    // set the account name if one was provided  
    if (projectString && strlen(projectString) > 0) {
      fprintf(slurmFile, "#SBATCH -A %s\n", projectString);
    }
 
    // set the output name to either the user specified.<jobID>.out 
    // or to the binaryName.<jobID>.out if none specified
    if (outputfn!=NULL) {
      fprintf(slurmFile, "#SBATCH -o %s.%%j.out\n", outputfn);
    }
    else {
      fprintf(slurmFile, "#SBATCH -o %s.%%j.out\n", argv[0]);
    }

    // add the srun command 
    fprintf(slurmFile, "srun -n %d %s ", numLocales, chpl_get_real_binary_name());
    
    //add any arguments passed to the launcher to the binary 
    for (i=1; i<argc; i++) {
      fprintf(slurmFile, " '%s'", argv[i]);
    }
    fprintf(slurmFile, "\n");

    //close the batch file and change permissions 
    fclose(slurmFile);
    chmod( slurmFilename, 0755);

    // the baseCommand is what will call the batch file
    // that was just created 
    sprintf(baseCommand, "sbatch %s\n", slurmFilename);
  }
  // if we're running an interactive job 
  else {
    // expect is used to launch an interactive job 
    // create the file and set some things for expect 
    expectFile = fopen(expectFilename, "w");
    fprintf(expectFile, "set timeout -1\n");
    fprintf(expectFile, "set prompt \"(%%|#|\\\\$|>) $\"\n");
    
    // create a silent salloc command and set 
    fprintf(expectFile, "spawn -noecho salloc ");

    //set the job name 
    fprintf(expectFile, "-J %.10s ",basenamePtr);  
    
    // request the number of locales and claim 1 task per node (more chapel
    // tasks can run per node, this is just for slurm) 
    fprintf(expectFile, "-N %d ",numLocales); 
    fprintf(expectFile, "--ntasks-per-node=1 ",numLocales); 
    
    // request exclusive access
    fprintf(expectFile, "--exclusive "); //  give exclusive access to the nodes
    
    // Set the walltime, this is required for interactive jobs 
    fprintf(expectFile, "--time=%s ",walltime); 
    
    // set any constraints 
    if (constraint) {
      fprintf(expectFile, " -C %s", constraint);
    }

    // the actual srun command 
    fprintf(expectFile, "srun -n %d %s ", numLocales, chpl_get_real_binary_name());
    
    //add any arguments passed to the launcher to the binary 
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

// clean up the batch file or expect file in an interactive job 
static void chpl_launch_cleanup(void) {
#ifndef DEBUG_LAUNCH
  char command[1024];

  if (getenv("CHPL_LAUNCHER_USE_SBATCH") == NULL) {
    sprintf(command, "rm %s", expectFilename);
    system(command);
  } else {
    sprintf(command, "rm %s", slurmFilename);
    system(command);
  }

#endif
}


int chpl_launch(int argc, char* argv[], int32_t numLocales) {

  sbatchVersion sVersion = determineSlurmVersion();
  if (sVersion != slurm) {
    printf("Error: This launcher is only compatible with native slurm\n");
    printf("Slurm version was %d\n", sVersion);
    return 1;
  }

  int retcode =
    chpl_launch_using_system(chpl_launch_create_command(argc, argv, numLocales),
                             argv[0]);
  chpl_launch_cleanup();
  return retcode;
}

// Currently the only args that are handled are the num locales
// We could probably also  handle walltime here instead of just 
// as an environment variable and we will probably need to handle 
// flags for numa-ness 
int chpl_launch_handle_arg(int argc, char* argv[], int argNum,
                           int32_t lineno, chpl_string filename) {
  return 0;
}


void chpl_launch_print_help(void) {
}
