#include <stdio.h>
#include <string.h>
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
    size += strlen(argv[i]) + 1;
  }

  command = chpl_malloc(size, sizeof(char*), "zebra command buffer", -1, "");
  
  sprintf(command, "%s", baseCommand);
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
