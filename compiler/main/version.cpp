#include <cstdio>
#include "driver.h"
#include "version.h"
#include "version_num.h"

void
get_version(char *v) {
  v += sprintf(v, "%d.%s.%s", MAJOR_VERSION, MINOR_VERSION, UPDATE_VERSION);
  if (BUILD_VERSION || developer)
    sprintf(v, ".%d", BUILD_VERSION);
}

