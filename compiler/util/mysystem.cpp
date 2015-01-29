#include "mysystem.h"

#include "misc.h"

#include <cstdio>
#include <cstdlib>

bool printSystemCommands = false;

int mysystem(const char* command, 
             const char* description,
             bool        ignoreStatus) {
  int status = 0;

  // Treat a '#' at the start of a line as a comment
  if (command[0] != '#') {
    status = system(command);
  }

  if (status == -1) {
    USR_FATAL("system() fork failed");

  } else if (status != 0 && ignoreStatus == false) {
    USR_FATAL(description);
  }

  return status;
}
