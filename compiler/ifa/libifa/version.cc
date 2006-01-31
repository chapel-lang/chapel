#include <stdio.h>
#include "version.h"

#ifndef BUILD_VERSION
#define BUILD_VERSION 0
#endif

void
ifa_version(char *v) {
  v += sprintf(v, "%d.%d", MAJOR_VERSION, MINOR_VERSION);
  if (BUILD_VERSION)
    v += sprintf(v, ".%d", BUILD_VERSION);
}

