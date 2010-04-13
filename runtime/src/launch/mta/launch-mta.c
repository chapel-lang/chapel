#include <stdio.h>
#include <string.h>
#include "chpllaunch.h"
#include "chpl_mem.h"
#include "error.h"

int numTeams = 0;
int maxTeams = 0;

static char* chpl_launch_create_command(int argc, char* argv[],
                                        int32_t numLocales) {
  int i;
  int size;
  char baseCommand[256];
  char* command;

  if (numLocales != 1) {
    chpl_error("XMT launcher only supports numLocales==1", 0, "<command-line>"\
);
  }

  chpl_compute_real_binary_name(argv[0]);

  if ((numTeams != 0) && (maxTeams != 0)) {
    sprintf(baseCommand, "mtarun -m %d -t %d %s", maxTeams, numTeams, chpl_get\
_real_binary_name());
  } else if (numTeams != 0) {
    sprintf(baseCommand, "mtarun -t %d %s", numTeams, chpl_get_real_binary_nam\
e());
  } else if (maxTeams != 0) {
    sprintf(baseCommand, "mtarun -m %d %s", maxTeams, chpl_get_real_binary_nam\
e());
  } else {
    sprintf(baseCommand, "mtarun %s", chpl_get_real_binary_name());
  }

  size = strlen(baseCommand) + 1;

  for (i=1; i<argc; i++) {
    size += strlen(argv[i]) + 3;
  }

  command = chpl_malloc(size, sizeof(char*), CHPL_RT_MD_COMMAND_BUFFER, -1, ""\
);

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
  if ((strcmp(argv[argNum], "--numNodes") == 0) ||
      (strcmp(argv[argNum], "-nn") == 0)) {
    numTeams = atoi(argv[argNum+1]);
    maxTeams = atoi(argv[argNum+1]);
    return 2;
  } else if (strcmp(argv[argNum], "--numTeams") == 0) {
    numTeams = atoi(argv[argNum+1]);
    return 2;
  } else if (strcmp(argv[argNum], "--maxTeams") == 0) {
    maxTeams = atoi(argv[argNum+1]);
    return 2;
  }
  return 0;
}
