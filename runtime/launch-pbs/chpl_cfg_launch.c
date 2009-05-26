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


#define basePBSFilename ".chpl-pbs-qsub-"
#define baseExpectFilename ".chpl-expect-"
#define baseSysFilename ".chpl-sys-"

char pbsFilename[FILENAME_MAX];
char expectFilename[FILENAME_MAX];
char sysFilename[FILENAME_MAX];

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
  char version[versionBuffLen+1] = "";
  char* versionPtr = version;
  FILE* sysFile;
  int i;

  char* command = chpl_glom_strings(3, "qsub --version > ", sysFilename, " 2>&1");
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
  if (strstr(version, "NCCS")) {
    return nccs;
  } else if (strstr(version, "PBSPro")) {
    return pbspro;
  } else {
    return unknown;
  }
}

static int getNumCoresPerLocale(void) {
  FILE* sysFile;
  int coreMask;
  int bitMask = 0x1;
  int numCores;
  char* numCoresString = getenv("CHPL_LAUNCHER_CORES_PER_LOCALE");

  if (numCoresString) {
    numCores = atoi(numCoresString);
    if (numCores != 0)
      return numCores;
  }

  char* command = chpl_glom_strings(2, "cnselect -Lcoremask > ", sysFilename);
  system(command);
  sysFile = fopen(sysFilename, "r");
  if (fscanf(sysFile, "%d\n", &coreMask) != 1 || !feof(sysFile)) {
    chpl_error("unable to determine number of cores per locale; please set CHPL_LAUNCHER_CORES_PER_LOCALE", 0, 0);
  }
  coreMask >>= 1;
  numCores = 1;
  while (coreMask & bitMask) {
    coreMask >>= 1;
    numCores += 1;
  }
  fclose(sysFile);
  return numCores;
}

static void genNumLocalesOptions(FILE* pbsFile, qsubVersion qsub, 
                                 int32_t numLocales,
                                 int32_t numCoresPerLocale) {
  char* queue = getenv("CHPL_LAUNCHER_QUEUE");
  switch (qsub) {
  case pbspro:
  case unknown:
    if (queue)
      fprintf(pbsFile, "#PBS -q %s\n", queue);
    fprintf(pbsFile, "#PBS -l mppwidth=%d\n", numLocales);
    fprintf(pbsFile, "#PBS -l mppnppn=%d\n", procsPerNode);
    fprintf(pbsFile, "#PBS -l mppdepth=%d\n", numCoresPerLocale);
    break;
  case nccs:
    if (queue)
      fprintf(pbsFile, "#PBS -q %s\n", queue);
    else
      fprintf(pbsFile, "#PBS -q debug\n");
    fprintf(pbsFile, "#PBS -l size=%d\n", numCoresPerLocale*numLocales);
    break;
  }
}

static char* chpl_launch_create_command(int argc, char* argv[], 
                                        int32_t numLocales) {
  int i;
  int size;
  char baseCommand[256];
  char* command;
  FILE* pbsFile, *expectFile;
  char* projectString = getenv(launcherAccountEnvvar);
  char* basenamePtr = strrchr(argv[0], '/');
  pid_t mypid;
  int numCoresPerLocale;

  if (basenamePtr == NULL) {
      basenamePtr = argv[0];
  } else {
      basenamePtr++;
  }

#ifndef DEBUG_LAUNCH
  mypid = getpid();
#else
  mypid = 0;
#endif
  sprintf(sysFilename, "%s%d", baseSysFilename, (int)mypid);
  sprintf(expectFilename, "%s%d", baseExpectFilename, (int)mypid);
  sprintf(pbsFilename, "%s%d", basePBSFilename, (int)mypid);

  numCoresPerLocale = getNumCoresPerLocale();

  pbsFile = fopen(pbsFilename, "w");
  fprintf(pbsFile, "#!/bin/sh\n\n");
  fprintf(pbsFile, "#PBS -N Chpl-%.10s\n", basenamePtr);
  genNumLocalesOptions(pbsFile, determineQsubVersion(), numLocales, numCoresPerLocale);
  if (projectString && strlen(projectString) > 0)
    fprintf(pbsFile, "#PBS -A %s\n", projectString);
  fclose(pbsFile);

  expectFile = fopen(expectFilename, "w");
  if (verbosity < 2) {
    fprintf(expectFile, "log_user 0\n");
  }
  fprintf(expectFile, "set timeout -1\n");
  fprintf(expectFile, "set prompt \"(%%|#|\\$|>) $\"\n");
  if (verbosity > 0) {
    fprintf(expectFile, "spawn tcsh -f\n");
    fprintf(expectFile, "expect -re $prompt\n");
    fprintf(expectFile, "send \"df -T . \\n\"\n");
    fprintf(expectFile, "expect {\n");
    fprintf(expectFile, "  -re $prompt {\n");
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
    fprintf(expectFile, "  lustre\n");
    fprintf(expectFile, "}\n");
    fprintf(expectFile, "send \"exit\\n\"\n");
  }
  fprintf(expectFile, "spawn qsub -z ");
  if (getenv("GASNET_MAX_SEGSIZE")) {
    fprintf(expectFile, "-vGASNET_MAX_SEGSIZE ");
  }
  fprintf(expectFile, "-I %s\n", pbsFilename);
  fprintf(expectFile, "expect {\n");
  fprintf(expectFile, "  \"A project was not specified\" {send_user "
          "\"error: A project account must be specified via \\$" 
          launcherAccountEnvvar "\\n\" ; exit 1}\n");
  fprintf(expectFile, "  -re $prompt\n");
  fprintf(expectFile, "}\n");
  fprintf(expectFile, "send \"cd \\$PBS_O_WORKDIR\\n\"\n");
  fprintf(expectFile, "expect -re $prompt\n");
  fprintf(expectFile, "send \"aprun -q -b -d%d -n1 -N1 ls %s_real\\n\"\n", numCoresPerLocale, argv[0]);
  fprintf(expectFile, "expect {\n");
  fprintf(expectFile, "  \"failed: chdir\" {send_user "
          "\"error: %s/%s_real must be launched from and/or stored on a "
          "cross-mounted file system\\n\" ; exit 1}\n", 
          basenamePtr, basenamePtr);
  fprintf(expectFile, "  -re $prompt\n");
  fprintf(expectFile, "}\n");
  fprintf(expectFile, "send \"aprun ");
  if (verbosity < 2) {
    fprintf(expectFile, "-q ");
  }
  fprintf(expectFile, "-d%d -n%d -N%d %s_real", numCoresPerLocale, numLocales, procsPerNode, argv[0]);
  for (i=1; i<argc; i++) {
    fprintf(expectFile, " '%s'", argv[i]);
  }
  fprintf(expectFile, "\\n\"\n");
  fprintf(expectFile, "interact -o -re $prompt {return}\n");
  fprintf(expectFile, "send_user \"\\n\"\n");
  fprintf(expectFile, "send \"exit\\n\"\n");
  fclose(expectFile);

  sprintf(baseCommand, "expect %s", expectFilename);

  size = strlen(baseCommand) + 1;

  command = chpl_malloc(size, sizeof(char), "command buffer", false, -1, "");
  
  sprintf(command, "%s", baseCommand);

  if (strlen(command)+1 > size) {
    chpl_internal_error("buffer overflow");
  }

  return command;
}

static void chpl_launch_cleanup(void) {
#ifndef DEBUG_LAUNCH
  char command[1024];

  sprintf(command, "rm %s", pbsFilename);
  system(command);

  sprintf(command, "rm %s", expectFilename);
  system(command);

  sprintf(command, "rm %s", sysFilename);
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
