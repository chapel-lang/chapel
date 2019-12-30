/*
 * Copyright 2004-2020 Cray Inc.
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

// get popen/pclose
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 600
#endif
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
#include "../aprun/aprun-utils.h"

#define baseExpectFilename ".chpl-expect-"
#define EXPECT "expect"

#define CHPL_CC_ARG "-cc"
#define CHPL_WALLTIME_FLAG "--walltime"
#define CHPL_QUEUE_FLAG "--queue"
#define CHPL_GENERATE_QSUB_SCRIPT "--generate-qsub-script"

static char *_ccArg = NULL;
static char* debug = NULL;
static char* walltime = NULL;
static char* queue = NULL;
static int generate_qsub_script = 0;

static char expectFilename[FILENAME_MAX];

extern int fileno(FILE *stream);

/* copies of binary to run per node */
#define procsPerNode 1  
#define versionBuffLen 80

#define launcherAccountEnvvar "CHPL_LAUNCHER_ACCOUNT"

typedef enum {
  pbspro,
  nccs,
  moab, 
  unknown
} qsubVersion;


static qsubVersion determineQsubVersion(void) {
  const int buflen = 256;
  char version[buflen];
  char whichMoab[buflen];
  FILE *whichOutput;
  int fileError = 1;
  char *argv[3];
  argv[0] = (char *) "qsub";
  argv[1] = (char *) "--version";
  argv[2] = NULL;
  
  memset(version, 0, buflen);
  if (chpl_run_utility1K("qsub", argv, version, buflen) <= 0) {
    chpl_error("Error trying to determine qsub version", 0, 0);
  }

  if (strstr(version, "NCCS")||strstr(version, "OLCF")) {
    return nccs;
  } else if (strstr(version, "pbs_version") || strstr(version, "PBSPro")) {
    return pbspro;
  } else {
    memset(whichMoab, 0, buflen);
    whichOutput = popen("which moab 2>&1 >/dev/null", "r");  
    if (whichOutput != NULL ) {
      fgets(whichMoab, buflen, whichOutput);
      fileError = ferror(whichOutput); 
      pclose(whichOutput);
      if (strlen(whichMoab) == 0 && !fileError) {
        return moab;
      }
    }
    return unknown;
  }
}

//
// If generate_qsub_script, return the filename of the qsub script that
//   was written with the qsub options
// else return the qsub options for the command line as a string
//
static char* genQsubOptions(char* genFilename, char* projectString, qsubVersion qsub, 
                            int32_t numLocales, int32_t numCoresPerLocale) {
  const size_t maxOptLength = 256;
  char* optionString = NULL;
  int length = 0;
  FILE *qsubScript = NULL;
  char *qsubFilename = expectFilename;

  if (!queue) {
    queue = getenv("CHPL_LAUNCHER_QUEUE");
  }
  if (!walltime) {
    walltime = getenv("CHPL_LAUNCHER_WALLTIME");
  }

  if (generate_qsub_script) {
    pid_t mypid = debug ? 0 : getpid();
    sprintf(qsubFilename, "qsub.%s-%d", genFilename, (int) mypid);
    qsubScript = fopen(qsubFilename, "w");
    fprintf(qsubScript, "#PBS -j oe\n");
    fprintf(qsubScript, "#PBS -zV\n");
    fprintf(qsubScript, "#PBS -N Chpl-%.10s\n", genFilename);
  } else {
    optionString = chpl_mem_allocMany(maxOptLength, sizeof(char),
                                      CHPL_RT_MD_COMMAND_BUFFER, -1, 0);
    length += snprintf(optionString + length, maxOptLength - length,
                       "-z -V -I -N Chpl-%.10s", genFilename);
  }

  if (projectString && strlen(projectString) != 0) {
    if (generate_qsub_script) {
      fprintf(qsubScript, "#PBS -A %s\n", projectString);
    } else {
      length += snprintf(optionString + length, maxOptLength - length,
                         " -A %s", projectString);
    }
  }
  if (queue) {
    if (generate_qsub_script) {
      fprintf(qsubScript, "#PBS -q %s\n", queue);
    } else {
      length += snprintf(optionString + length, maxOptLength - length,
                         " -q %s", queue);
    }
  }
  if (walltime) {
    if (generate_qsub_script) {
      fprintf(qsubScript, "#PBS -l walltime=%s\n", walltime);
    } else {
      length += snprintf(optionString + length, maxOptLength - length,
                         " -l walltime=%s", walltime);
    }
  }
  switch (qsub) {
  case unknown:
    if (generate_qsub_script) {
      fprintf(qsubScript, "#PBS -l mppwidth=%d\n", numLocales);
      fprintf(qsubScript, "#PBS -l mppnppn=%d\n", procsPerNode);
      fprintf(qsubScript, "#PBS -l mppdepth=%d\n", numCoresPerLocale);
    } else {
      length += snprintf(optionString + length, maxOptLength - length,
                         " -l mppwidth=%d -l mppnppn=%d -l mppdepth=%d",
                         numLocales, procsPerNode, numCoresPerLocale);
    }
    break;
  case pbspro:
    if (generate_qsub_script) {
      // We always want to use scatter since we use one PE per node
      fprintf(qsubScript, "#PBS -l place=scatter\n");
      fprintf(qsubScript, "#PBS -l select=%d:ncpus=%d\n", numLocales, numCoresPerLocale);
    } else {
      // We always want to use scatter since we use one PE per node
      length += snprintf(optionString + length, maxOptLength - length,
                         " -l place=scatter");
      length += snprintf(optionString + length, maxOptLength - length,
                         " -l select=%d:ncpus=%d", numLocales, numCoresPerLocale);
    }
    break;
  case moab:
    if (generate_qsub_script) {
      fprintf(qsubScript, "#PBS -l nodes=%d\n", numLocales);
    } else {
      length += snprintf(optionString + length, maxOptLength - length,
                         " -l nodes=%d", numLocales);
    }
    break;
  case nccs:
    if (generate_qsub_script) {
      fprintf(qsubScript, "#PBS -l nodes=%d\n", numLocales);
    } else {
      if (!queue && !walltime)
        chpl_error("An execution time must be specified for the NCCS launcher if no queue is\n"
                   "specified -- use the CHPL_LAUNCHER_WALLTIME and/or CHPL_LAUNCHER_QUEUE\n"
                   "environment variables", 0, 0);
      length += snprintf(optionString + length, maxOptLength - length,
                         " -l nodes=%d\n", numLocales);
    }
    break;
  }

  if (generate_qsub_script) {
    fclose(qsubScript);
    optionString = qsubFilename;
  }

  return optionString;
}

static char** chpl_launch_create_argv(int argc, char* argv[], 
                                      int32_t numLocales) {
  const int largc = 2;
  char *largv[largc];
  int i;
  FILE* expectFile = NULL;
  char* projectString = getenv(launcherAccountEnvvar);
  char* basenamePtr = strrchr(argv[0], '/');
  char* qsubOptions;
  FILE* qsubScript = NULL;
  FILE *outfile;
  pid_t mypid;
  int numCoresPerLocale;
  char **aprun_cmd;

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
  snprintf(expectFilename, FILENAME_MAX, "%s%d",
           baseExpectFilename, (int)mypid);

  initAprunAttributes();
  numCoresPerLocale = getCoresPerLocale();

  qsubOptions = genQsubOptions(basenamePtr, projectString,
                               determineQsubVersion(),
                               numLocales, numCoresPerLocale);

  if (generate_qsub_script) {
    qsubScript = fopen(qsubOptions, "a");
    if (!qsubScript) {
      char msg[256];
      snprintf(msg, 256, "Error creating qsub script '%s': %s",
               qsubOptions, strerror(errno));
      chpl_error(msg, 0, 0);
    }
    fprintf(qsubScript, "cd $PBS_O_WORKDIR\n");
    fprintf(qsubScript, "aprun ");
  } else {
    expectFile = fopen(expectFilename, "w");
    if (!expectFile) {
      char msg[256];
      snprintf(msg, 256, "Error creating temporary script '%s': %s",
               qsubOptions, strerror(errno));
      chpl_error(msg, 0, 0);
    }

    if (verbosity < 2) {
      fprintf(expectFile, "log_user 0\n");
    }
    fprintf(expectFile, "set timeout -1\n");
    fprintf(expectFile, "set chpl_prompt \"chpl-%d # \"\n", mypid);
    if (verbosity > 0) {
      fprintf(expectFile, "spawn tcsh -f\n");
      fprintf(expectFile, "send \"set prompt=\\\"$chpl_prompt\\\"\\n\"\n");
      fprintf(expectFile, "expect -re \"\\n$chpl_prompt\" {}\n");
      fprintf(expectFile, "send \"df -T . \\n\"\n");
      fprintf(expectFile, "expect {\n");
      fprintf(expectFile, "  -ex lustre {}\n");
      fprintf(expectFile, "  -re \"\\n$chpl_prompt\" {\n");
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
    fprintf(expectFile, "expect -re \"\\n$chpl_prompt\" {}\n");
    fprintf(expectFile, "send \"cd \\$PBS_O_WORKDIR\\n\"\n");
    fprintf(expectFile, "expect -re \"\\n$chpl_prompt\" {}\n");
    if (verbosity > 2) {
      fprintf(expectFile, "send \"aprun -q %s%d ",
              getNumLocalesStr(), 1 /* only run on one locale */);
      fprintf(expectFile, "ls %s %s\\n\"\n",
          chpl_get_real_binary_wrapper(), chpl_get_real_binary_name());
      fprintf(expectFile, "expect {\n");
      fprintf(expectFile, "  \"failed: chdir\" {send_user "
              "\"error: %s must be launched from and/or stored on a "
              "cross-mounted file system\\n\" ; exit 1}\n", 
              basenamePtr);
      fprintf(expectFile, "  -ex \"$chpl_prompt\" {}\n");
      fprintf(expectFile, "}\n");
    }
    fprintf(expectFile, "send \"%s aprun ",
            isatty(fileno(stdout)) ? "" : "stty -onlcr;");
  }

  if (generate_qsub_script) {
    outfile = qsubScript;
  } else {
    outfile = expectFile;
  }

  aprun_cmd = chpl_create_aprun_cmd(argc, argv, numLocales, _ccArg);

  for (i=1; ; i++) {
    if (aprun_cmd[i] != NULL)
      fprintf(outfile, "%s ", aprun_cmd[i]);
    else
      break;
  }

  chpl_mem_free(aprun_cmd, 0, 0);

  if (generate_qsub_script) {
    fprintf(qsubScript, "\n\n");
    fclose(qsubScript);
    fprintf(stdout, "QSUB script written to '%s'\n", qsubOptions);
    return NULL;
  } else {
    fprintf(expectFile, "\\n\"\n");
    fprintf(expectFile, "interact -o -ex \"$chpl_prompt\" {return}\n");
    fprintf(expectFile, "send \"set retval=$?\\n\"\n");
    fprintf(expectFile, "send \"\\[ \\$retval = 0 \\] && echo \\\"JOB SUCCEEDED (done)\\\" || echo \\\"JOB FAILED: \\$retval (done)\\\"\\n\"\n");
    // Being a bit excessive with the expects here
    fprintf(expectFile, "expect -ex \"(done)\" {}\n");
    fprintf(expectFile, "expect -ex \"(done)\" {}\n");
    fprintf(expectFile, "expect {\n");
    fprintf(expectFile, "    -ex \"JOB SUCCEEDED\" { set exitval \"0\" }\n");
    fprintf(expectFile, "    -re \"JOB FAILED:.\" { set exitval \"1\" }\n");
    fprintf(expectFile, "}\n");
    fprintf(expectFile, "expect -re \"\\n$chpl_prompt\" {}\n");

    fprintf(expectFile, "send \"exit\\n\"\n"); // exit tcsh

    fprintf(expectFile, "send \"exit\\n\"\n"); // exit qsub
    fprintf(expectFile, "expect -re \"qsub:.*completed\" {}\n"); // flush buffers for good measure

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

}

static void genQsubScript(int argc, char *argv[], int numLocales) {
  chpl_launch_create_argv(argc, argv, numLocales);
}

static void chpl_launch_cleanup(void) {
  if (!debug) {
    if (unlink(expectFilename)) {
      char msg[FILENAME_MAX + 35];
      snprintf(msg, FILENAME_MAX + 35, "Error removing temporary file '%s': %s",
               expectFilename, strerror(errno));
      chpl_warning(msg, 0, 0);
    }
  }
}

int chpl_launch(int argc, char* argv[], int32_t numLocales) {
  int retcode;
  debug = getenv("CHPL_LAUNCHER_DEBUG");

  if (generate_qsub_script) {
    genQsubScript(argc, argv, numLocales);
    retcode = 0;
  } else {
    retcode =
      chpl_launch_using_fork_exec(EXPECT,
                                  chpl_launch_create_argv(argc, argv,
                                                          numLocales),
                                  argv[0]);
    chpl_launch_cleanup();
  }

  return retcode;
}


int chpl_launch_handle_arg(int argc, char* argv[], int argNum,
                           int32_t lineno, int32_t filename) {
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
  if (!strcmp(argv[argNum], CHPL_GENERATE_QSUB_SCRIPT)) {
    generate_qsub_script = 1;
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
      snprintf(msg, 256, "'%s' is not a valid cpu assignment", _ccArg);
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
  fprintf(stdout, "  %s : generate a qsub script and exit\n", CHPL_GENERATE_QSUB_SCRIPT);
  fprintf(stdout, "  %s <queue>        : specify a queue\n", CHPL_QUEUE_FLAG);
  fprintf(stdout, "                           (or use $CHPL_LAUNCHER_QUEUE)\n");
  fprintf(stdout, "  %s <HH:MM:SS>  : specify a wallclock time limit\n", CHPL_WALLTIME_FLAG);
  fprintf(stdout, "                           (or use $CHPL_LAUNCHER_WALLTIME)\n");
}

