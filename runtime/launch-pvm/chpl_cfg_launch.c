#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "chpllaunch.h"
#include "chplmem.h"
#include "error.h"


char* chpl_launch_create_command(int argc, char* argv[], int32_t numLocales) {
  fprintf(stderr, "Called into chpl_launch_create_command(..., numLocales=%d\n", 
	  numLocales);
  exit(1);
}

void chpl_launch_sanity_checks(int argc, char* argv[], const char* cmd) {
  fprintf(stderr, "chpl_launch_sanity_checks()\n");
  exit(1);
}

void chpl_launch_cleanup(void) {
  fprintf(stderr, "chpl_launch_cleanup()\n");
  exit(1);
}
