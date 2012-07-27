#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include "chpllaunch.h"
#include "chpl-mem.h"
#include "chpltypes.h"
#include "error.h"


#define baseExpectFilename ".chpl-expect-"
#define EXPECT "expect"

#define CHPL_CC_ARG "-cc"
#define CHPL_WALLTIME_FLAG "--walltime"
#define CHPL_QUEUE_FLAG "--queue"

static char *_ccArg = NULL;
static char* debug = NULL;
static char* walltime = NULL;
static char* queue = NULL;

static char expectFilename[FILENAME_MAX];

extern int fileno(FILE *stream);

/* copies of binary to run per node */
#define procsPerNode 1  
#define versionBuffLen 80

#define launcherAccountEnvvar "CHPL_LAUNCHER_ACCOUNT"

typedef enum {
  pbspro,
  nccs,
  unknown
} qsubVersion;


static qsubVersion determineQsubVersion(void) {
  const int buflen = 256;
  char version[buflen];
  char *argv[3];
  argv[0] = (char *) "qsub";
  argv[1] = (char *) "--version";
  argv[2] = NULL;
  
  memset(version, 0, buflen);
  if (chpl_run_utility1K("qsub", argv, version, buflen) <= 0) {
    chpl_error("Error trying to determine qsub version", 0, 0);
  }

  if (strstr(version, "NCCS")) {
    return nccs;
  } else if (strstr(version, "PBSPro")) {
    return pbspro;
  } else {
    return unknown;
  }
}

static int getNumCoresPerLocale(void) {
  const int buflen = 256;
  char buf[buflen];
  int numCores = -1;
  char* numCoresString = getenv("CHPL_LAUNCHER_CORES_PER_LOCALE");

  if (numCoresString) {
    numCores = atoi(numCoresString);
    if (numCores <= 0)
      chpl_warning("CHPL_LAUNCHER_CORES_PER_LOCALE set to invalid value.", 0, 0);
  }

  if (numCores <= 0) {
    char *argv[3];
    int charsRead;
    argv[0] = (char *) "cnselect";
    argv[1] = (char *) "-Lnumcores";
    argv[2] = NULL;
  
    memset(buf, 0, buflen);
    if ((charsRead = chpl_run_utility1K("cnselect", argv, buf, buflen)) <= 0) {
      chpl_error("Error trying to determine number of cores per node", 0, 0);
    }

    if (sscanf(buf, "%d", &numCores) != 1) {
      chpl_error("unable to determine number of cores per locale; please set CHPL_LAUNCHER_CORES_PER_LOCALE", 0, 0);
    }
  }

  return numCores;
}

static char* genQsubOptions(char* genFilename, char* projectString, qsubVersion qsub, 
                            int32_t numLocales, int32_t numCoresPerLocale) {
  const size_t maxOptLength = 256;
  char* optionString;
  int length = 0;
  if (!queue) {
    queue = getenv("CHPL_LAUNCHER_QUEUE");
  }
  if (!walltime) {
    walltime = getenv("CHPL_LAUNCHER_WALLTIME");
  }
  optionString = chpl_mem_allocMany(maxOptLength, sizeof(char), CHPL_RT_MD_COMMAND_BUFFER, -1, "");

  length += snprintf(optionString + length, maxOptLength - length,
                     "-z -V -I -N Chpl-%.10s", genFilename);

  if (projectString && strlen(projectString) != 0) {
    length += snprintf(optionString + length, maxOptLength - length,
                       " -A %s", projectString);
  }
  if (queue) {
    length += snprintf(optionString + length, maxOptLength - length,
                       " -q %s", queue);
  }
  if (walltime) {
    length += snprintf(optionString + length, maxOptLength - length,
                       " -l walltime=%s", walltime);
  }
  switch (qsub) {
  case pbspro:
  case unknown:
    length += snprintf(optionString + length, maxOptLength - length,
                       " -l mppwidth=%d -l mppnppn=%d -l mppdepth=%d",
                       numLocales, procsPerNode, numCoresPerLocale);
    break;
  case nccs:
    if (!queue && !walltime)
      chpl_error("An execution time must be specified for the NCCS launcher if no queue is\n"
                 "specified -- use the CHPL_LAUNCHER_WALLTIME and/or CHPL_LAUNCHER_QUEUE\n"
                 "environment variables", 0, 0);
    length += snprintf(optionString + length, maxOptLength - length,
                       " -l size=%d\n", numCoresPerLocale*numLocales);
    break;
  }
  return optionString;
}

static char** chpl_launch_create_argv(int argc, char* argv[], 
                                      int32_t numLocales) {
  const int largc = 2;
  char *largv[largc];
  int i;
  FILE* expectFile;
  char* projectString = getenv(launcherAccountEnvvar);
  char* basenamePtr = strrchr(argv[0], '/');
  char* qsubOptions;
  pid_t mypid;
  int numCoresPerLocale;
  const char *ccArg = _ccArg ? _ccArg : "none";

  if (basenamePtr == NULL) {
      basenamePtr = argv[0];
  } else {
      basenamePtr++;
  }
  chpl_compute_real_binary_name(argv[0]);

  if (!debug) {
    mypid = getpid();
  } else {
    mypid = 0;
  }
  sprintf(expectFilename, "%s%d", baseExpectFilename, (int)mypid);

  numCoresPerLocale = getNumCoresPerLocale();

  qsubOptions = genQsubOptions(basenamePtr, projectString, determineQsubVersion(),
                               numLocales, numCoresPerLocale);

  expectFile = fopen(expectFilename, "w");
  if (verbosity < 2) {
    fprintf(expectFile, "log_user 0\n");
  }
  fprintf(expectFile, "set timeout -1\n");
  fprintf(expectFile, "set chpl_prompt \"chpl-%d # \"\n", mypid);
  if (verbosity > 0) {
    fprintf(expectFile, "spawn tcsh -f\n");
    fprintf(expectFile, "send \"set prompt=\\\"$chpl_prompt\\\"\\n\"\n");
    fprintf(expectFile, "expect -ex $chpl_prompt {}\n");
    fprintf(expectFile, "expect -ex $chpl_prompt {}\n");
    fprintf(expectFile, "expect -ex $chpl_prompt {}\n");
    fprintf(expectFile, "send \"df -T . \\n\"\n");
    fprintf(expectFile, "expect {\n");
    fprintf(expectFile, "  -ex lustre {}\n");
    fprintf(expectFile, "  -ex $chpl_prompt {\n");
    fprintf(expectFile, "    send_user \"warning: Executing this program from a non-Lustre file system may cause it \\nto be unlaunchable, or for file I/O to be performed on a non-local file system.\\nContinue anyway? (\\[y\\]/n) \"\n");
    fprintf(expectFile, "    interact {\n");
    fprintf(expectFile, "      \\015           {return}\n");
    fprintf(expectFile, "      -echo -re \"y|Y\" {return}\n");
    fprintf(expectFile, "      -echo -re \"n|N\" {send_user \"\\n\" ; exit 0}\n");
    fprintf(expectFile, "      eof               {send_user \"\\n\" ; exit 0}\n");
    fprintf(expectFile, "      \003              {send_user \"\\n\" ; exit 0}\n");
    fprintf(expectFile, "      -echo -re \".\"   {send_user \"\\nContinue anyway? (\\[y\\]/n) \"}\n");
    fprintf(expectFile, "    }\n");
    fprintf(expectFile, "    send_user  \"\\n\"\n");
    fprintf(expectFile, "  }\n");
    fprintf(expectFile, "}\n");
    fprintf(expectFile, "send \"exit\\n\"\n");
    fprintf(expectFile, "close\n");
  }
  fprintf(expectFile, "spawn qsub %s\n", qsubOptions);
  fprintf(expectFile, "expect {\n");
  fprintf(expectFile, "  \"A project was not specified\" {send_user "
          "\"error: A project account must be specified via \\$" 
          launcherAccountEnvvar "\\n\" ; exit 1}\n");
  fprintf(expectFile, "  -ex \"qsub: waiting\" {}\n");
  fprintf(expectFile, "}\n");
  fprintf(expectFile, "expect -re \"qsub:.*ready\" {}\n");
  fprintf(expectFile, "send \"tcsh -f\\n\"\n");
  fprintf(expectFile, "send \"set prompt=\\\"$chpl_prompt\\\"\\n\"\n");
  fprintf(expectFile, "expect -ex $chpl_prompt {}\n");
  fprintf(expectFile, "expect -ex $chpl_prompt {}\n");
  fprintf(expectFile, "expect -ex $chpl_prompt {}\n");
  fprintf(expectFile, "send \"cd \\$PBS_O_WORKDIR\\n\"\n");
  fprintf(expectFile, "expect -ex $chpl_prompt {}\n");
  if (verbosity > 2) {
    fprintf(expectFile, "send \"aprun -cc %s -q -b -d%d -n1 -N1 ls %s\\n\"\n",
            ccArg, numCoresPerLocale, chpl_get_real_binary_name());
    fprintf(expectFile, "expect {\n");
    fprintf(expectFile, "  \"failed: chdir\" {send_user "
            "\"error: %s must be launched from and/or stored on a "
            "cross-mounted file system\\n\" ; exit 1}\n", 
            basenamePtr);
    fprintf(expectFile, "  -ex $chpl_prompt {}\n");
    fprintf(expectFile, "}\n");
  }
  fprintf(expectFile, "send \"%s aprun ",
          isatty(fileno(stdout)) ? "" : "stty -onlcr;");
  if (verbosity < 2) {
    fprintf(expectFile, "-q ");
  }
  fprintf(expectFile, "-cc %s -d%d -n%d -N%d %s", ccArg, numCoresPerLocale,
          numLocales, procsPerNode, chpl_get_real_binary_name());
  for (i=1; i<argc; i++) {
    fprintf(expectFile, " '%s'", argv[i]);
  }
  fprintf(expectFile, "\\n\"\n");
  fprintf(expectFile, "interact -o -ex $chpl_prompt {return}\n");
  fprintf(expectFile, "send \"echo CHPL_EXIT_CODE:\\$?\\n\"\n");
  fprintf(expectFile, "expect {\n");
  fprintf(expectFile, "  -ex \"CHPL_EXIT_CODE:0\" {set exitval \"0\"}\n");
  fprintf(expectFile, "  -re \"CHPL_EXIT_CODE:.\" {set exitval \"1\"}\n");
  fprintf(expectFile, "}\n");
  fprintf(expectFile, "expect -ex $chpl_prompt {}\n");
  fprintf(expectFile, "send \"exit\\n\"\n"); // exit tcsh
  fprintf(expectFile, "send \"exit\\n\"\n"); // exit qsub
  fprintf(expectFile, "close\n");
  if (verbosity > 1) {
    fprintf(expectFile, "send_user \"\\n\\n\"\n");
  }
  fprintf(expectFile, "exit $exitval\n");    // exit
  fclose(expectFile);

  largv[0] = (char *) EXPECT;
  largv[1] = expectFilename;

  return chpl_bundle_exec_args(0, NULL, largc, largv);
}

static void chpl_launch_cleanup(void) {
  if (!debug) {
    if (unlink(expectFilename)) {
      char msg[1024];
      sprintf(msg, "Error removing temporary file '%s': %s", expectFilename,
              strerror(errno));
      chpl_warning(msg, 0, 0);
    }
  }
}


int chpl_launch(int argc, char* argv[], int32_t numLocales) {
  int retcode;
  debug = getenv("CHPL_LAUNCHER_DEBUG");
  retcode =
    chpl_launch_using_fork_exec(EXPECT,
                                chpl_launch_create_argv(argc, argv, numLocales),
                                argv[0]);
  chpl_launch_cleanup();
  return retcode;
}


int chpl_launch_handle_arg(int argc, char* argv[], int argNum,
                           int32_t lineno, chpl_string filename) {
  int numArgs = 0;
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
  if (!strcmp(argv[argNum], CHPL_CC_ARG)) {
    _ccArg = argv[argNum+1];
    numArgs = 2;
  } else if (!strncmp(argv[argNum], CHPL_CC_ARG"=", strlen(CHPL_CC_ARG))) {
    _ccArg = &(argv[argNum][strlen(CHPL_CC_ARG)+1]);
    numArgs = 1;
  }
  if (numArgs > 0) {
    if (strcmp(_ccArg, "none") &&
        strcmp(_ccArg, "numa_node") &&
        strcmp(_ccArg, "cpu")) {
      char msg[256];
      sprintf(msg, "'%s' is not a valid cpu assignment", _ccArg);
      chpl_error(msg, 0, 0);
    }
    return numArgs;
  }
  return 0;
}

void chpl_launch_print_help(void) {
  fprintf(stdout, "LAUNCHER FLAGS:\n");
  fprintf(stdout, "===============\n");
  fprintf(stdout, "  %s <cpu assignment>   : specify cpu assignment within a node:\n", CHPL_CC_ARG);
  fprintf(stdout, "                           none (default), numa_node, cpu\n");
  fprintf(stdout, "  %s <queue>        : specify a queue\n", CHPL_QUEUE_FLAG);
  fprintf(stdout, "                           (or use $CHPL_LAUNCHER_QUEUE)\n");
  fprintf(stdout, "  %s <HH:MM:SS>  : specify a wallclock time limit\n", CHPL_WALLTIME_FLAG);
  fprintf(stdout, "                           (or use $CHPL_LAUNCHER_WALLTIME)\n");
}
