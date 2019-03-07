#include "param-cname.h"

#include <stdio.h>

void externproc(void) {
  printf("in externproc\n");
  exportedproc();
}
