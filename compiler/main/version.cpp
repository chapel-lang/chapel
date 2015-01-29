#include <cstdio>
#include "driver.h"
#include "version.h"
#include "version_num.h"

void
get_version(char *v) {
  v += sprintf(v, "%d.%s.%s", MAJOR_VERSION, MINOR_VERSION, UPDATE_VERSION);
  if (strcmp(BUILD_VERSION, "0") != 0 || developer)
    sprintf(v, ".%s", BUILD_VERSION);
}

