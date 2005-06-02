#include <stdio.h>
#include <stdlib.h>

#include "chplrt.h"
#include "exit.h"

void _chpl_exit(int status) {
  exit(status);
}
