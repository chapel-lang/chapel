#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "chpllaunch.h"
#include "chplmem.h"
#include "error.h"


char* chpl_launch_create_command(int argc, char* argv[], int32_t numLocales) {
  char baseCommand[256];
  char* command;

  sprintf(baseCommand, "echo \"spawn -> %s\" | pvm", argv[0]);

  fprintf(stderr, "Called into chpl_launch_create_command(..., numLocales=%d\n",
	  numLocales);
  return command;
}

void chpl_launch_sanity_checks(int argc, char* argv[], const char* cmd) {
  fprintf(stderr, "chpl_launch_sanity_checks()\n");
  return;
}

void chpl_launch_cleanup(void) {
  fprintf(stderr, "chpl_launch_cleanup()\n");
  return;
}
