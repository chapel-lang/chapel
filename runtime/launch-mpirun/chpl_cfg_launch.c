#include <stdio.h>
#include <string.h>
#include "chpllaunch.h"
#include "chpl_mem.h"
#include "error.h"


// TODO: Un-hard-code this stuff:
static const char* mpirunPath = "$CHPL_HOME/third-party/openmpi/install/linux64-gnu/bin/";

static char* chpl_launch_create_command(int argc, char* argv[], 
                                        int32_t numLocales) {
  int i;
  int size;
  char baseCommand[256];
  char* command;
  sprintf(baseCommand, "mpirun -np %d %s_real", numLocales, argv[0]);

  size = strlen(mpirunPath) + strlen(baseCommand) + 1;

  for (i=1; i<argc; i++) {
    size += strlen(argv[i]) + 3;
  }

  command = chpl_malloc(size, sizeof(char*), CHPL_RT_MD_COMMAND_BUFFER, -1, "");
  
  sprintf(command, "%s%s", mpirunPath, baseCommand);
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


void chpl_launch(int argc, char* argv[], int32_t numLocales) {
  chpl_launch_using_system(chpl_launch_create_command(argc, argv, numLocales),
                           argv[0]);
}


int chpl_launch_handle_arg(int argc, char* argv[], int argNum,
                           int32_t lineno, chpl_string filename) {
  return 0;
}
