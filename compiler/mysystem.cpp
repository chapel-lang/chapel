#include <stdio.h>
#include <stdlib.h>
#include "geysa.h"

int mysystem(const char* command, char* description, int ignorestatus=0) {
  int status = system(command);

  if (status == -1) {
    fail("system() fork failed");
  } else if (status != 0 && !ignorestatus) {
    fail(description);
  }

  return status;
}
