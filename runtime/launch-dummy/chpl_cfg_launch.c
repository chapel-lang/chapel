#include <stdio.h>
#include <string.h>
#include "chpllaunch.h"
#include "chplmem.h"
#include "error.h"


static char* chpl_launch_create_command(int argc, char* argv[], 
                                        int32_t numLocales) {
  int i;
  int size;
  char baseCommand[256];
  char* command;
  if (numLocales != 1) {
    chpl_error("dummy launcher only supports numLocales==1", 0, "<command-line>");
  }
  sprintf(baseCommand, "%s_real", argv[0]);

  size = strlen(baseCommand) + 1;

  for (i=1; i<argc; i++) {
    size += strlen(argv[i]) + 3;
  }

  command = chpl_malloc(size, sizeof(char*), "dummy command buffer", false, -1, "");
  
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
  if (strcmp(argv[argNum], "--dummy") == 0) {
    printf("the dummy launcher is handling an extra argument: %s\n", 
           argv[argNum]);
    return 1;
  } else if (strcmp(argv[argNum], "--queue") == 0) {
    printf("the dummy launcher is handling two extra arguments: %s %s\n",
           argv[argNum], argv[argNum+1]);
    return 2;
  } else {
    return 0;
  }
}
