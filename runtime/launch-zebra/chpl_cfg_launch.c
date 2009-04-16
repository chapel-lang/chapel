#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "chpllaunch.h"
#include "chplmem.h"
#include "error.h"


char* chpl_launch_create_command(int argc, char* argv[], int32_t numLocales) {
  int i;
  int size;
  char baseCommand[256];
  char* command;
  sprintf(baseCommand, "zebra -fast -r %s_real", argv[0]);

  size = strlen(baseCommand) + 1;

  for (i=1; i<argc; i++) {
    size += strlen(argv[i]) + 3;
  }

  command = chpl_malloc(size, sizeof(char*), "zebra command buffer", false, -1, "");
  
  sprintf(command, "%s", baseCommand);
  for (i=1; i<argc; i++) {
    strcat(command, " '");
    strcat(command, argv[i]);
    strcat(command, "'");
  }

  if (strlen(command)+1 > size) {
    chpl_internal_error("buffer overflow");
  }

  return command;
}

void chpl_launch_sanity_checks(int argc, char* argv[], const char* command) {
  // Do sanity checks just before launching.
  struct stat statBuf;
  char realName[256];
  int retVal;

  retVal = snprintf(realName, 256, "%s_real", argv[0]);
  if (retVal < 0 || retVal >= 256) {
    chpl_internal_error("error generating back-end filename");
  }

  // Make sure the _real binary exists
  if (stat(realName, &statBuf) != 0) {
    char errorMsg[256];
    sprintf(errorMsg, "unable to locate file: %s", realName);
    chpl_error(errorMsg, -1, "<internal>");
  }
}

void chpl_launch_cleanup(void) {
}
