/*
 Copyright 2002-2003 John Plevyak, All Rights Reserved
*/
#include "d.h"

void
d_version(char *v) {
  v += sprintf(v, "%d.%d", D_MAJOR_VERSION, D_MINOR_VERSION);
  if (D_BUILD_VERSION)
    v += sprintf(v, ".%d", D_BUILD_VERSION);
}

