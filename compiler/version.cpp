/* -*-Mode: c++;-*-
 Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#include <stdio.h>
#include "version.h"

void
get_version(char *v) {
  v += sprintf(v, "%d.%d", MAJOR_VERSION, MINOR_VERSION);
  if (BUILD_VERSION)
    v += sprintf(v, ".%d", BUILD_VERSION);
}

