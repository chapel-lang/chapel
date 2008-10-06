#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chpllaunch.h"
#include "chplmem.h"
#include "error.h"

static const char* pbsFilename = ".chpl-pbs-qsub";
static const char* expectFilename = ".chpl-expect";

#define sysFilename ".chpl-tmp"
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

  system("qsub --version > "sysFilename);
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
  if (strcmp(version, " ") == 0) {
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
  int numCores = 1;

  /* BLC: This code is fairly specific to xt-cnl, but currently will
     only be called for the NCCS version of qsub */
  system("cnselect -Lcoremask > " sysFilename);
  sysFile = fopen(sysFilename, "r");
  if (fscanf(sysFile, "%d", &coreMask) != 1) {
    chpl_error("Cannot determine number of cores per node", 0, 0);
  }
  fclose(sysFile);
  coreMask >>= 1;
  while (coreMask & bitMask) {
    coreMask >>= 1;
    numCores += 1;
  }
  return numCores;
}

static void genNumLocalesOptions(FILE* pbsFile, qsubVersion qsub, 
                                 int32_t numLocales) {
  switch (qsub) {
  case pbspro:
  case unknown:
    fprintf(pbsFile, "#PBS -l mppwidth=%d\n", numLocales);
    fprintf(pbsFile, "#PBS -l mppnppn=%d\n", procsPerNode);
    break;
  case nccs:
    fprintf(pbsFile, "#PBS -l size=%d\n", getNumCoresPerLocale()*numLocales);
    break;
  }
}

char* chpl_launch_create_command(int argc, char* argv[], int32_t numLocales) {
  int i;
  int size;
  char baseCommand[256];
  char* command;
  FILE* pbsFile, *expectFile;
  char* projectString = getenv(launcherAccountEnvvar);
  char* basenamePtr = strrchr(argv[0], '/');
  if (basenamePtr == NULL) {
      basenamePtr = argv[0];
  } else {
      basenamePtr++;
  }

  pbsFile = fopen(pbsFilename, "w");
  fprintf(pbsFile, "#!/bin/sh\n\n");
  fprintf(pbsFile, "#PBS -N Chapel-%s\n", basenamePtr);
  genNumLocalesOptions(pbsFile, determineQsubVersion(), numLocales);
  fprintf(pbsFile, "#PBS -l walltime=30\n");    // BLC -- ditto?
  if (projectString && strlen(projectString) > 0)
    fprintf(pbsFile, "#PBS -A %s\n", projectString);
  fclose(pbsFile);

  expectFile = fopen(expectFilename, "w");
  if (verbosity < 2) {
    fprintf(expectFile, "log_user 0\n");
  }
  fprintf(expectFile, "set timeout -1\n");
  fprintf(expectFile, "set prompt \"(%|#|\\$|>) $\"\n");
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
  fprintf(expectFile, "spawn qsub -z -I %s\n", pbsFilename);
  fprintf(expectFile, "expect {\n");
  fprintf(expectFile, "  \"A project was not specified\" {send_user "
          "\"error: A project account must be specified via \\$" 
          launcherAccountEnvvar "\\n\" ; exit 1}\n");
  fprintf(expectFile, "  -re $prompt\n");
  fprintf(expectFile, "}\n");
  fprintf(expectFile, "send \"cd \\$PBS_O_WORKDIR\\n\"\n");
  fprintf(expectFile, "expect -re $prompt\n");
  fprintf(expectFile, "send \"aprun -q -b -n1 -N1 ls %s_real\\n\"\n", argv[0]);
  fprintf(expectFile, "expect {\n");
  fprintf(expectFile, "  \"failed: chdir\" {send_user "
          "\"error: %s/%s_real must be launched from and/or stored on a "
          "cross-mounted file system\\n\" ; exit 1}\n", 
          basenamePtr, basenamePtr);
  fprintf(expectFile, "  -re $prompt\n");
  fprintf(expectFile, "}\n");
  fprintf(expectFile, "send \"aprun -q -n%d -N%d %s_real", 
          numLocales, procsPerNode, argv[0]);
  for (i=1; i<argc; i++) {
    fprintf(expectFile, " %s", argv[i]);
  }
  fprintf(expectFile, "\\n\"\n");
  fprintf(expectFile, "interact -o -re $prompt {return}\n");
  fprintf(expectFile, "send_user \"\\n\"\n");
  fprintf(expectFile, "send \"exit\\n\"\n");
  fclose(expectFile);

  sprintf(baseCommand, "expect %s", expectFilename);

  size = strlen(baseCommand) + 1;

  command = chpl_malloc(size, sizeof(char*), "command buffer", -1, "");
  
  sprintf(command, "%s", baseCommand);

  if (strlen(command)+1 > size) {
    chpl_internal_error("buffer overflow");
  }

  return command;
}


void chpl_launch_cleanup(void) {
  char command[1024];

  /*
  sprintf(command, "rm %s", pbsFilename);
  system(command);

  sprintf(command, "rm %s", expectFilename);
  system(command);

  sprintf(command, "rm %s", sysFilename);
  system(command);
  */
}
