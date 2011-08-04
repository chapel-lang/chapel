#include <stdio.h>
#include <string.h>
#include "chpllaunch.h"
#include "chpl-mem.h"
#include "error.h"

#define WRAP_TO_STR(x) TO_STR(x)
#define TO_STR(x) #x

static char* chpl_launch_create_command(int argc, char* argv[], 
                                        int32_t numLocales) {
  int i;
  int size;
  char baseCommand[256];
  char* command;
  char targetType;
  if (!strcmp(WRAP_TO_STR(LAUNCH_TARGET_PLATFORM), "x1-sim")) {
    targetType = '1';
  } else if (!strcmp(WRAP_TO_STR(LAUNCH_TARGET_PLATFORM), "x2-sim")) {
    targetType = 'b';
  } else {
    chpl_internal_error("unknown target type for apsim launcher");
  }

  chpl_compute_real_binary_name(argv[0]);
    
  sprintf(baseCommand, "apsim -P p%c %s", targetType, chpl_get_real_binary_name());

  size = strlen(WRAP_TO_STR(LAUNCH_PATH)) + strlen(baseCommand) + 2;

  for (i=1; i<argc; i++) {
    size += strlen(argv[i]) + 3;
  }

  command = chpl_mem_allocMany(size, sizeof(char), CHPL_RT_MD_COMMAND_BUFFER, -1, "");
  
  sprintf(command, "%s/%s", WRAP_TO_STR(LAUNCH_PATH), baseCommand);
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


int chpl_launch(int argc, char* argv[], int32_t numLocales) {
  return chpl_launch_using_system(chpl_launch_create_command(argc, argv, numLocales),
                                  argv[0]);
}


int chpl_launch_handle_arg(int argc, char* argv[], int argNum,
                           int32_t lineno, chpl_string filename) {
  return 0;
}


void chpl_launch_print_help(void) {
}
