#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "chpllaunch.h"
#include "chplmem.h"
#include "error.h"

#define LAUNCH_PATH_HELP WRAP_TO_STR(LAUNCH_PATH)
#define WRAP_TO_STR(x) TO_STR(x)
#define TO_STR(x) #x

// TODO: Un-hard-code this stuff:

char* chpl_launch_create_command(int argc, char* argv[], int32_t numLocales) {
  int i;
  int size;
  char baseCommand[256];
  char realName[256];
  char* command;
  struct stat statBuf;

  sprintf(realName, "%s_real", argv[0]);
  sprintf(baseCommand, "amudprun -np %d %s", numLocales, realName);

  size = strlen(WRAP_TO_STR(LAUNCH_PATH)) + strlen(baseCommand) + 1;

  for (i=1; i<argc; i++) {
    size += strlen(argv[i]) + 3;
  }

  command = chpl_malloc(size, sizeof(char*), "amudprun command buffer", -1, "");
  
  sprintf(command, "%s%s", WRAP_TO_STR(LAUNCH_PATH), baseCommand);
  for (i=1; i<argc; i++) {
    strcat(command, " '");
    strcat(command, argv[i]);
    strcat(command, "'");
  }

  if (strlen(command)+1 > size) {
    chpl_internal_error("buffer overflow");
  }

  // Make sure the _real binary exists
  if (stat(realName, &statBuf) != 0) {
    char errorMsg[256];
    sprintf(errorMsg, "unable to locate file: %s", realName);
    chpl_error(errorMsg, -1, "<internal>");
  }

  return command;
}


void chpl_launch_cleanup(void) {
}
