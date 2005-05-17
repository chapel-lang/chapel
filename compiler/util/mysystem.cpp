#include <stdio.h>
#include <stdlib.h>
#include "misc.h"
#include "mysystem.h"

bool printSystemCommands = false;

int mysystem(const char* command, char* description, int ignorestatus) {
  if (printSystemCommands) {
    printf("%s\n\n", command);
  }

  int status = system(command);

  if (status == -1) {
    fail("system() fork failed");
  } else if (status != 0 && !ignorestatus) {
    fail(description);
  }

  return status;
}
