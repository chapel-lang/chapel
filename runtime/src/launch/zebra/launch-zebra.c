#include <stdio.h>
#include <string.h>
#include "chpllaunch.h"
#include "chpl_mem.h"
#include "error.h"


static char* chpl_launch_create_command(int argc, char* argv[],
                                        int32_t numLocales) {
  int i;
  int size;
  char baseCommand[256];
  char* command;

  chpl_compute_real_binary_name(argv[0]);

  sprintf(baseCommand, "zebra -fast -r %s", chpl_get_real_binary_name());

  size = strlen(baseCommand) + 1;

  for (i=1; i<argc; i++) {
    size += strlen(argv[i]) + 3;
  }

  command = chpl_malloc(size, sizeof(char*), CHPL_RT_MD_COMMAND_BUFFER, -1, "");
  
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


void chpl_launch(int argc, char* argv[], int32_t numLocales) {
  chpl_launch_using_system(chpl_launch_create_command(argc, argv, numLocales),
                           argv[0]);
}


int chpl_launch_handle_arg(int argc, char* argv[], int argNum,
                           int32_t lineno, chpl_string filename) {
  return 0;
}


void chpl_launch_print_help(void) {
}
