#include <cstdio>
#include <cstdlib>
#include "misc.h"
#include "mysystem.h"

bool printSystemCommands = false;

int mysystem(const char* command, const char* description, int ignorestatus) {
  if (printSystemCommands) {
    printf("%s\n\n", command);
  }

  int status = 0;

  if (command[0] != '#') {
    status = system(command);
  }

  if (status == -1) {
    USR_FATAL("system() fork failed");
  } else if (status != 0 && !ignorestatus) {
    USR_FATAL(description);
  }

  return status;
}
