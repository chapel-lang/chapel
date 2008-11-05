#include <stdio.h>
#include <string.h>
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
  char* command;
  sprintf(baseCommand, "gasnetrun_ibv -spawner=ssh -n %d %s_real", numLocales,
          argv[0]);

  size = strlen(WRAP_TO_STR(LAUNCH_PATH)) + strlen(baseCommand) + 1;

  for (i=1; i<argc; i++) {
    size += strlen(argv[i]) + 1;
  }

  command = chpl_malloc(size, sizeof(char*), "gasnetrun_ibv command buffer", 
                        -1, "");
  
  sprintf(command, "%s%s", WRAP_TO_STR(LAUNCH_PATH), baseCommand);
  for (i=1; i<argc; i++) {
    strcat(command, " ");
    strcat(command, argv[i]);
  }

  if (strlen(command)+1 > size) {
    chpl_internal_error("buffer overflow");
  }

  return command;
}


void chpl_launch_cleanup(void) {
}
